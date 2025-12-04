#include "Generator.h"
#include "Parm.h"
#include "LexAnalysis.h"
#include "IT.h"
#include "LT.h"
#include "Error.h"
#include <sstream>
#include <cstring>
#include <iosfwd>
#include <stack>
#include <vector>
#include <cwchar>
#include <locale>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <cstdio>

using namespace std;

namespace Gener
{
	// Типы блоков для отслеживания вложенности
	enum BlockType { BLOCK_CHARGE, BLOCK_PATROL, BLOCK_COUNCIL, BLOCK_PATH, BLOCK_TIRESOME, BLOCK_OTHER };
	
	static int conditionnum = 0;
	string numberOfPoints = "k";
	string itoS(int x) { stringstream r;  r << x;  return r.str(); }

	// Генерация кода вызова стандартной функции библиотеки
	// Все функции используют stdcall: параметры справа налево, стек очищается функцией
	string genStdLibCall(Lexer::LEX& tables, Log::LOG& log, int i, IT::Entry& funcEntry)
	{
		string str;
		if (i >= tables.lextable.size || tables.lextable.table[i].idxTI == TI_NULLIDX)
			return "";
		
		string funcName = string(funcEntry.id);
		int paramCount = funcEntry.value.params.count;
		
		str += "\n";
		
		// В постфиксной нотации аргументы уже в стеке вычислений в порядке: первый, второй, третий (сверху вниз)
		// Для stdcall нужно передать в порядке: последний, предпоследний, ..., первый (справа налево)
		// То есть нужно извлечь все аргументы и push их в обратном порядке
		
		if (funcName == "filament")
		{
			// filament(char* str, int* result) - stdcall, 2 параметра
			// Порядок для stdcall: result (последний), str (первый)
			// В стеке: str (верх), нужно: result, str
			str += "pop eax\n"; // str
			str += "push offset temp\n"; // result (последний параметр)
			str += "push eax\n"; // str (первый параметр)
			str += "call " + funcName + "\n";
			// Стек очищается функцией (ret 8), не нужно add esp
			str += "mov eax, [temp]\n"; // Результат в eax
		}
		else if (funcName == "consolidate")
		{
			// consolidate(char* dest, char* src, char* result) - stdcall, 3 параметра
			// Порядок для stdcall: result (последний), src (средний), dest (первый)
			// В стеке: dest (верх), src (ниже), нужно: result, src, dest
			str += "pop eax\n"; // dest
			str += "pop ebx\n"; // src
			str += "push offset buffer\n"; // result (последний параметр)
			str += "push ebx\n"; // src (средний параметр)
			str += "push eax\n"; // dest (первый параметр)
			str += "call " + funcName + "\n";
			// Стек очищается функцией (ret 12), не нужно add esp
			str += "mov eax, offset buffer\n"; // Результат в eax
		}
		else if (funcName == "compare_scrolls")
		{
			// compare_scrolls(char* str1, char* str2, int* result) - stdcall, 3 параметра
			// Порядок для stdcall: result (последний), str2 (средний), str1 (первый)
			// В стеке: str1 (верх), str2 (ниже), нужно: result, str2, str1
			str += "pop eax\n"; // str1
			str += "pop ebx\n"; // str2
			str += "push offset temp\n"; // result (последний параметр)
			str += "push ebx\n"; // str2 (средний параметр)
			str += "push eax\n"; // str1 (первый параметр)
			str += "call " + funcName + "\n";
			// Стек очищается функцией (ret 12), не нужно add esp
			str += "mov eax, [temp]\n"; // Результат в eax
		}
		else if (funcName == "mightiness")
		{
			// mightiness(int a, int b, int* result) - stdcall, 3 параметра
			// Порядок для stdcall: result (последний), b (средний), a (первый)
			// В стеке: a (верх), b (ниже), нужно: result, b, a
			str += "pop eax\n"; // a
			str += "pop ebx\n"; // b
			str += "push offset temp\n"; // result (последний параметр)
			str += "push ebx\n"; // b (средний параметр)
			str += "push eax\n"; // a (первый параметр)
			str += "call " + funcName + "\n";
			// Стек очищается функцией (ret 12), не нужно add esp
			str += "mov eax, [temp]\n"; // Результат в eax
		}
		
		return str;
	}

	// Генерация кода вызова функции
	string genCallFuncCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		if (i >= tables.lextable.size || tables.lextable.table[i].idxTI == TI_NULLIDX)
			return "";
		
		IT::Entry e = ITENTRY(i);
		bool stnd = (e.idtype == IT::IDTYPE::S);
		
		if (stnd)
		{
			return genStdLibCall(tables, log, i, e);
		}
		
		// Пользовательская функция
		// В постфиксной нотации аргументы уже в стеке вычислений в порядке: первый, второй, третий (сверху вниз)
		// Для stdcall нужно передать в порядке: последний, предпоследний, ..., первый (справа налево)
		int paramCount = e.value.params.count;
		
		str += "\n";
		
		if (paramCount == 0)
		{
			str += "call " + string(e.id) + "\n";
			return str;
		}
		
		// Извлекаем параметры из стека вычислений и передаем функции в правильном порядке для stdcall
		// В стеке: первый (верх), второй (ниже), третий (еще ниже)
		// Для stdcall: третий (первым push), второй, первый (последним push)
		if (paramCount == 1)
		{
			// Один параметр - порядок не меняется
			str += "pop eax\n";
			str += "push eax\n";
		}
		else if (paramCount == 2)
		{
			// Два параметра: в стеке первый, второй -> для stdcall: второй, первый
			str += "pop eax\n"; // первый
			str += "pop ebx\n"; // второй
			str += "push ebx\n"; // второй (первым push для stdcall)
			str += "push eax\n"; // первый (последним push для stdcall)
		}
		else if (paramCount == 3)
		{
			// Три параметра: в стеке первый, второй, третий -> для stdcall: третий, второй, первый
			str += "pop eax\n"; // первый
			str += "pop ebx\n"; // второй
			str += "pop ecx\n"; // третий
			str += "push ecx\n"; // третий (первым push для stdcall)
			str += "push ebx\n"; // второй
			str += "push eax\n"; // первый (последним push для stdcall)
		}
		
		str += "call " + string(e.id) + "\n";
		// Стек очищается функцией (ret N), не нужно add esp
		
		return str;
	}

	// Генерация кода присваивания с обработкой постфиксной польской нотации
	string genEqualCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		if (i - 1 < 0 || i - 1 >= tables.lextable.size || tables.lextable.table[i - 1].idxTI == TI_NULLIDX)
			return "";
		
		IT::Entry e1 = ITENTRY(i - 1);
		
		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::INT:
		case IT::IDDATATYPE::BOOL:
		{
			// Обрабатываем выражение в постфиксной польской нотации
			for (int j = i + 1; j < tables.lextable.size && LEXEMA(j) != LEX_SEMICOLON; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (j >= tables.lextable.size || tables.lextable.table[j].idxTI == TI_NULLIDX)
						continue;
					
					IT::Entry entry = ITENTRY(j);
					
					// В постфиксной нотации функции идут ПОСЛЕ аргументов
					if (entry.idtype == IT::IDTYPE::F || entry.idtype == IT::IDTYPE::S)
					{
						str += genCallFuncCode(tables, log, j);
						str += "push eax\n";
						// Пропускаем аргументы функции
						int parenCount = 1;
						j++;
						while (j < tables.lextable.size && parenCount > 0)
						{
							if (LEXEMA(j) == LEX_LEFTBRAC) parenCount++;
							else if (LEXEMA(j) == LEX_RIGHTBRAC) parenCount--;
							j++;
						}
						j--; // Вернемся на правильную позицию
					}
					else
					{
						str += "push " + string(entry.id) + "\n";
					}
					break;
				}
				case LEX_PLUS:
					str += "pop ebx\npop eax\nadd eax, ebx\npush eax\n";
					break;
				case LEX_MINUS:
					str += "pop ebx\npop eax\nsub eax, ebx\npush eax\n";
					break;
				case LEX_STAR:
					str += "pop ebx\npop eax\nimul eax, ebx\npush eax\n";
					break;
				case LEX_DIRSLASH:
					str += "pop ebx\npop eax\ncdq\nidiv ebx\npush eax\n";
					break;
				case '%':
					str += "pop ebx\npop eax\ncdq\nidiv ebx\npush edx\n";
					break;
				case LEX_ALLY:
					str += "pop ebx\npop eax\nor eax, ebx\npush eax\n";
					break;
				case LEX_BIND:
					str += "pop ebx\npop eax\nand eax, ebx\npush eax\n";
					break;
				case LEX_BANISH:
					str += "pop eax\nnot eax\npush eax\n";
					break;
				case LEX_COMPSCROLLS:
				case LEX_CONSOLIDATE:
				case LEX_MIGHTINESS:
				case LEX_FILAMENT:
				{
					str += genCallFuncCode(tables, log, j);
					str += "push eax\n";
					// Пропускаем аргументы функции
					int parenCount = 1;
					j++;
					while (j < tables.lextable.size && parenCount > 0)
					{
						if (LEXEMA(j) == LEX_LEFTBRAC) parenCount++;
						else if (LEXEMA(j) == LEX_RIGHTBRAC) parenCount--;
						j++;
					}
					j--; // Вернемся на правильную позицию
					break;
				}
				}
			}
			
			str += "pop eax\nmov " + string(e1.id) + ", eax\n";
			break;
		}
		case IT::IDDATATYPE::STR:
		{
			char lex = LEXEMA(i + 1);
			if (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
			{
				IT::Entry e2 = ITENTRY(i + 1);
				if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S))
				{
					str += genCallFuncCode(tables, log, i + 1);
					str += "mov " + string(e1.id) + ", eax\n";
				}
				else if (lex == LEX_LITERAL)
				{
					str += "mov " + string(e1.id) + ", offset " + string(e2.id) + "\n";
				}
				else
				{
					str += "mov eax, " + string(e2.id) + "\nmov " + string(e1.id) + ", eax\n";
				}
			}
			break;
		}
		case IT::IDDATATYPE::CHAR:
		{
			char lex = LEXEMA(i + 1);
			if (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
			{
				IT::Entry e2 = ITENTRY(i + 1);
				if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S))
				{
					str += genCallFuncCode(tables, log, i + 1);
					str += "mov " + string(e1.id) + ", eax\n";
				}
				else if (lex == LEX_LITERAL)
				{
					str += "mov " + string(e1.id) + ", offset " + string(e2.id) + "\n";
				}
				else
				{
					str += "mov eax, " + string(e2.id) + "\nmov " + string(e1.id) + ", eax\n";
				}
			}
			break;
		}
		}
		return str;
	}

	// Генерация кода функции PROC
	string genFunctionCode(Lexer::LEX& tables, int i, string& funcname, int& pcount)
	{
		// После LEX_ACTION идет тип (LEX_INTSTRBOOLCHAR), затем имя функции (LEX_ID)
		// Структура: action тип имя [параметры]
		if (i + 2 >= tables.lextable.size || tables.lextable.table[i + 2].idxTI == TI_NULLIDX)
			return "";
		
		string str;
		IT::Entry e = ITENTRY(i + 2); // i+2 - это имя функции (i+0=ACTION, i+1=тип, i+2=имя)
		string funcId = string(e.id);
		funcname = funcId; // Сохраняем имя функции
		pcount = e.value.params.count;
		
		// Формируем PROC без параметров (параметры извлекаются вручную из стека)
		str = SEPSTR(funcname) + funcId + " PROC\n";
		
		// Стандартный пролог функции
		str += "push ebp\n";
		str += "mov ebp, esp\n";
		
		// Сохраняем регистры
		str += "push ebx\n";
		str += "push esi\n";
		str += "push edi\n";
		
		// Извлекаем параметры из стека в локальные переменные/регистры
		// Параметры находятся по адресам [ebp+8], [ebp+12], [ebp+16] и т.д.
		// (ebp+4 - это адрес возврата, ebp+8 - первый параметр)
		// Параметры находятся в квадратных скобках [ ], ищем LEX_RIGHTTHESIS (])
		int j = i + 3; // Пропускаем ACTION, тип, имя функции
		// Ищем начало параметров - LEX_LEFTHESIS ([)
		while (j < tables.lextable.size && LEXEMA(j) != LEX_LEFTHESIS)
			j++;
		
		if (j < tables.lextable.size)
		{
			j++; // Пропускаем [
			int paramOffset = 8; // Первый параметр на [ebp+8]
			while (j < tables.lextable.size && LEXEMA(j) != LEX_RIGHTTHESIS) // Ищем ]
			{
				if (LEXEMA(j) == LEX_ID && tables.lextable.table[j].idxTI != TI_NULLIDX)
				{
					IT::Entry paramEntry = ITENTRY(j);
					if (paramEntry.idtype == IT::IDTYPE::P)
					{
						// Копируем параметр из стека в локальную переменную
						str += "mov eax, [ebp+" + itoS(paramOffset) + "]\n";
						str += "mov " + string(paramEntry.id) + ", eax\n";
						paramOffset += 4; // Следующий параметр на 4 байта дальше
					}
				}
				j++;
			}
		}
		
		return str;
	}

	// Генерация кода выхода из функции ENDP
	string genExitCode(Lexer::LEX& tables, int i, string funcname, int pcount)
	{
		string str = "; --- restore registers ---\npop edi\npop esi\npop ebx\nmov esp, ebp\npop ebp\n; -------------------------\n";
		
		if (LEXEMA(i + 1) != LEX_SEMICOLON && i + 1 < tables.lextable.size && tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
		{
			IT::Entry returnEntry = ITENTRY(i + 1);
			if (returnEntry.idtype == IT::IDTYPE::V || returnEntry.idtype == IT::IDTYPE::L || returnEntry.idtype == IT::IDTYPE::P)
			{
				str += "mov eax, " + string(returnEntry.id) + "\n";
			}
			else
			{
				str += "mov eax, 0\n";
			}
		}
		else
		{
			str += "mov eax, 0\n";
		}
		
		str += "ret";
		if (pcount > 0)
		{
			str += " " + itoS(pcount * 4);
		}
		str += "\n";
		
		// Всегда генерируем ENDP с именем функции
		if (!funcname.empty())
			str += funcname + " ENDP" + SEPSTREMP;
		else
			str = str + "ENDP" + SEPSTREMP;
		
		return str;
	}

	// Генерация кода условия
	string genConditionCode(Lexer::LEX& tables, int i, string& cyclecode)
	{
		string str;
		conditionnum++;
		cyclecode.clear();
		
		if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].idxTI == TI_NULLIDX ||
			i + 3 >= tables.lextable.size || tables.lextable.table[i + 3].idxTI == TI_NULLIDX)
			return "";
		
		IT::Entry lft = ITENTRY(i + 1);
		IT::Entry rgt = ITENTRY(i + 3);
		bool w = false, r = false, c = false;
		string wstr, rstr;
		
		// Ищем закрывающую скобку условия ]
		int bracketEnd = i + 2;
		while (bracketEnd < tables.lextable.size && bracketEnd < i + 10 && LEXEMA(bracketEnd) != LEX_RIGHTTHESIS)
			bracketEnd++;
		
		// Ищем faith/exile/patrol/backup после условия
		for (int j = bracketEnd + 1; j < tables.lextable.size && j < bracketEnd + 30; j++)
		{
			if (LEXEMA(j) == LEX_FAITH) r = true;
			if (LEXEMA(j) == LEX_EXILE) w = true;
			if (LEXEMA(j) == LEX_PATROL) c = true;
			if (LEXEMA(j) == LEX_BACKUP) w = true;
		}
		
		if (strlen(lft.id) == 0 || strlen(rgt.id) == 0)
			return "";
		
		str += "mov edx, " + string(lft.id) + "\ncmp edx, " + string(rgt.id) + "\n";
		
		switch (LEXEMA(i + 2))
		{
		case LEX_MORE:  rstr = "jg";  wstr = "jle";  break;
		case LEX_LESS:   rstr = "jl";  wstr = "jge";  break;
		case LEX_EQUALS:    rstr = "je";  wstr = "jne";  break;
		case LEX_NONEQUALS:   rstr = "jne";  wstr = "je";  break;
		case LEX_MOREEQUALS:   rstr = "jge";  wstr = "jl";  break;
		case LEX_LESSEQUALS:   rstr = "jle";  wstr = "jg";  break;
		}
		
		if (!c)
		{
			if (r) str += "\n" + rstr + " right" + itoS(conditionnum) + "\n";
			if (w) str += wstr + " wrong" + itoS(conditionnum) + "\n";
			if (!r && !w) str += "\njmp next" + itoS(conditionnum) + "\n";
		}
		else
		{
			// Для patrol цикла
			str += "\n" + rstr + " patrol_start" + itoS(conditionnum) + "_body\n";
			str += "jmp cyclenext" + itoS(conditionnum) + "\n";
			str += "patrol_start" + itoS(conditionnum) + "_body:\n";
			cyclecode = str;
		}
		
		return str;
	}

	// Заполнение начальных секций
	vector <string> startFillVector(Lexer::LEX& tables)
	{
		vector <string> v;
		v.push_back(BEGIN);
		v.push_back(EXTERN);
		vector <string> vlt;  vlt.push_back(CONST);
		vector <string> vid;  vid.push_back(DATA);
		
		for (int i = 0; i < tables.idtable.size; i++)
		{
			IT::Entry e = tables.idtable.table[i];
			string str = "\t\t" + string(e.id);
			
			if (tables.idtable.table[i].idtype == IT::IDTYPE::L)
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:
				case IT::IDDATATYPE::BOOL:
					str = str + " sdword " + itoS(e.value.vint);
					break;
				case IT::IDDATATYPE::STR:
					if (e.value.vstr.len > 0 && e.value.vstr.str[0] != '\0')
						str = str + " byte '" + string(e.value.vstr.str) + "', 0";
					else
						continue;
					break;
				case IT::IDDATATYPE::CHAR:
					if (e.value.vstr.len > 0 && e.value.vstr.str[0] != '\0')
						str = str + " byte '" + string(e.value.vstr.str) + "', 0";
					else
						continue;
					break;
				default:
					continue;
				}
				vlt.push_back(str);
			}
			else if (tables.idtable.table[i].idtype == IT::IDTYPE::V || tables.idtable.table[i].idtype == IT::IDTYPE::P)
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:
				case IT::IDDATATYPE::BOOL:
					str = str + " dword 0";
					break;
				case IT::IDDATATYPE::STR:
					str = str + " dword ?";
					break;
				case IT::IDDATATYPE::CHAR:
					str = str + " dword ?";
					break;
				default:
					continue;
				}
				vid.push_back(str);
			}
		}
		
		v.insert(v.end(), vlt.begin(), vlt.end());
		v.insert(v.end(), vid.begin(), vid.end());
		v.push_back(CODE);
		return v;
	}

	// Основная функция генерации кода
	void CodeGeneration(Lexer::LEX& tables, Parm::PARM& parm, Log::LOG& log)
	{
		vector <string> v = startFillVector(tables);
		
		ofstream ofile;
		ofile.open("../Assembler/out.asm", ios::out | ios::trunc);
		
		if (!ofile.is_open())
		{
			throw ERROR_THROW(104);
		}
		
		stack<BlockType> blockStack;
		stack<int> blockConditionNum;
		
		string funcname;
		string cyclecode;
		int pcount = 0;
		bool inFunction = false; // Флаг: находимся ли мы внутри функции
		string str;
		
		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_ACTION:
			{
				// После LEX_ACTION идет тип, затем имя функции (i+2)
				if (i + 2 >= tables.lextable.size || tables.lextable.table[i + 2].idxTI == TI_NULLIDX)
					break;
				str = genFunctionCode(tables, i, funcname, pcount);
				inFunction = true; // Начинаем функцию
				break;
			}
			case LEX_COMEBACK:
			{
				str = genExitCode(tables, i, funcname, pcount);
				inFunction = false; // Заканчиваем функцию
				funcname.clear(); // Очищаем имя функции после завершения
				pcount = 0;
				break;
			}
			case LEX_TEMPLE:
			{
				str = str + SEPSTR("MAIN") + "main PROC";
				break;
			}
			case LEX_ID:
			{
				if (LEXEMA(i + 1) == LEX_LEFTBRAC && LEXEMA(i - 2) != LEX_ACTION)
					str = genCallFuncCode(tables, log, i);
				break;
			}
			case LEX_COMPSCROLLS:
			case LEX_CONSOLIDATE:
			case LEX_MIGHTINESS:
			case LEX_FILAMENT:
			{
				if (LEXEMA(i + 1) == LEX_LEFTBRAC)
					str = genCallFuncCode(tables, log, i);
				break;
			}
			case LEX_CHARGE:
			{
				str = genConditionCode(tables, i, cyclecode);
				break;
			}
			case LEX_PATROL:
			{
				conditionnum++;
				blockConditionNum.push(conditionnum);
				// Метка начала цикла уже создана в genConditionCode
				break;
			}
			case LEX_BRACELET:
			{
				if (blockStack.empty())
					break;
				
				BlockType blockType = blockStack.top();
				blockStack.pop();
				
				int blockCondNum = conditionnum;
				if (!blockConditionNum.empty())
				{
					blockCondNum = blockConditionNum.top();
					blockConditionNum.pop();
				}
				
				switch (blockType)
				{
				case BLOCK_PATH:
				{
					// Проверяем, есть ли еще path/tiresome блоки после
					bool hasMorePaths = false;
					for (int j = i + 1; j < tables.lextable.size && j < i + 30; j++)
					{
						if (LEXEMA(j) == LEX_PATH)
						{
							hasMorePaths = true;
							break;
						}
						if (LEXEMA(j) == LEX_TIRESOME)
						{
							// После tiresome будет council_end, не нужно здесь
							break;
						}
						if (LEXEMA(j) == LEX_BRACELET && j > i + 1)
							break;
						if (LEXEMA(j) == LEX_COUNCIL)
							break;
					}
					if (!hasMorePaths && i + 1 < tables.lextable.size && LEXEMA(i + 1) != LEX_PATH && LEXEMA(i + 1) != LEX_TIRESOME)
					{
						// Это последний path блок, генерируем переход и метку
						str += "jmp council_end" + itoS(blockCondNum) + "\n";
						// Метка council_end будет сгенерирована при закрытии council блока
					}
					break;
				}
				case BLOCK_PATROL:
				{
					str += "jmp patrol_start" + itoS(blockCondNum) + "\n";
					str += "cyclenext" + itoS(blockCondNum) + ":\n";
					break;
				}
				case BLOCK_COUNCIL:
				{
					// Метка council_end генерируется при закрытии последнего блока council
					// Проверяем, есть ли еще path/tiresome блоки после
					bool hasMorePaths = false;
					bool hasTiresome = false;
					for (int j = i + 1; j < tables.lextable.size && j < i + 50; j++)
					{
						if (LEXEMA(j) == LEX_PATH)
						{
							hasMorePaths = true;
							break;
						}
						if (LEXEMA(j) == LEX_TIRESOME)
						{
							hasTiresome = true;
							// tiresome - это последний блок, после него должна быть метка council_end
							break;
						}
						if (LEXEMA(j) == LEX_BRACELET && j > i + 1)
						{
							// Если следующий блок - это закрывающая скобка другого блока, значит это конец council
							break;
						}
						if (LEXEMA(j) == LEX_COUNCIL)
							break;
					}
					// Генерируем метку, если это последний блок (нет path после, или есть tiresome)
					if (!hasMorePaths || hasTiresome)
					{
						str += "council_end" + itoS(blockCondNum) + ":\n";
					}
					break;
				}
				case BLOCK_CHARGE:
				{
					if (i + 1 < tables.lextable.size && (LEXEMA(i + 1) == LEX_EXILE || LEXEMA(i + 1) == LEX_FAITH || LEXEMA(i + 1) == LEX_BACKUP))
					{
						// Перед else - ничего не делаем
					}
					else
					{
						str += "next" + itoS(blockCondNum) + ":\n";
					}
					break;
				}
				case BLOCK_TIRESOME:
				{
					// После tiresome блока всегда должна быть метка council_end
					str += "jmp council_end" + itoS(blockCondNum) + "\n";
					// Генерируем метку council_end после tiresome
					bool isLastBlock = true;
					for (int j = i + 1; j < tables.lextable.size && j < i + 20; j++)
					{
						if (LEXEMA(j) == LEX_PATH || LEXEMA(j) == LEX_TIRESOME)
						{
							isLastBlock = false;
							break;
						}
						if (LEXEMA(j) == LEX_BRACELET && j > i + 1)
							break;
					}
					if (isLastBlock)
					{
						str += "council_end" + itoS(blockCondNum) + ":\n";
					}
					break;
				}
				case BLOCK_OTHER:
				default:
					break;
				}
				break;
			}
			case LEX_FAITH:
			{
				str += "right" + itoS(conditionnum) + ":\n";
				break;
			}
			case LEX_EXILE:
			{
				str += "wrong" + itoS(conditionnum) + ":\n";
				break;
			}
			case LEX_LEFTBRACE:
			{
				BlockType blockType = BLOCK_OTHER;
				
				// Проверяем, является ли это началом тела функции
				// После ACTION идет тип, имя функции, параметры в [], затем {
				if (i > 0)
				{
					for (int j = i - 1; j >= 0 && j > i - 20; j--)
					{
						if (LEXEMA(j) == LEX_ACTION)
						{
							// Это начало тела функции
							blockType = BLOCK_OTHER; // Функция обрабатывается отдельно
							break;
						}
						if (LEXEMA(j) == LEX_TEMPLE)
						{
							// Это начало тела main
							blockType = BLOCK_OTHER;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS)
							break;
					}
				}
				
				if (i > 1 && LEXEMA(i - 1) == LEX_LITERAL && i > 2 && LEXEMA(i - 2) == LEX_PATH)
				{
					str += "path" + itoS(conditionnum) + "_" + itoS(i - 2) + ":\n";
					blockType = BLOCK_PATH;
				}
				else if (blockType == BLOCK_OTHER && i > 0)
				{
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_PATROL)
						{
							blockType = BLOCK_PATROL;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS)
							break;
					}
				}
				
				if (blockType == BLOCK_OTHER && i > 0)
				{
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_CHARGE)
						{
							blockType = BLOCK_CHARGE;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS)
							break;
					}
				}
				
				if (blockType == BLOCK_OTHER && i > 0)
				{
					for (int j = i - 1; j >= 0 && j > i - 5; j--)
					{
						if (LEXEMA(j) == LEX_TIRESOME)
						{
							blockType = BLOCK_TIRESOME;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS)
							break;
					}
				}
				
				if (blockType == BLOCK_OTHER && i > 0)
				{
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_COUNCIL)
						{
							blockType = BLOCK_COUNCIL;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS)
							break;
					}
				}
				
				blockStack.push(blockType);
				break;
			}
			case LEX_COUNCIL:
			{
				conditionnum++;
				blockConditionNum.push(conditionnum);
				if (i + 2 < tables.lextable.size && LEXEMA(i + 2) != LEX_RIGHTTHESIS)
				{
					if (tables.lextable.table[i + 2].idxTI != TI_NULLIDX)
					{
						IT::Entry expr = ITENTRY(i + 2);
						str += "mov edx, " + string(expr.id) + "\n";
					}
				}
				break;
			}
			case LEX_PATH:
			{
				if (i + 1 < tables.lextable.size && LEXEMA(i + 1) == LEX_LITERAL)
				{
					if (tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
					{
						IT::Entry literal = ITENTRY(i + 1);
						str += "cmp edx, " + string(literal.id) + "\n";
						str += "je path" + itoS(conditionnum) + "_" + itoS(i) + "\n";
					}
				}
				break;
			}
			case LEX_ASSIGN:
			{
				str = genEqualCode(tables, log, i);
				while (i + 1 < tables.lextable.size && LEXEMA(++i) != LEX_SEMICOLON);
				break;
			}
			case LEX_PRINT:
			{
				if (i + 1 < tables.lextable.size && tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
				{
					IT::Entry e = ITENTRY(i + 1);
					switch (e.iddatatype)
					{
					case IT::IDDATATYPE::INT:
					case IT::IDDATATYPE::BOOL:
						// confession(data, type) - stdcall, порядок: type (последний), data (первый)
						str += "\npush 1\n"; // type (последний параметр)
						str += "push " + string(e.id) + "\n"; // data (первый параметр)
						str += "call confession\n";
						// Стек очищается функцией (ret 8), не нужно add esp
						break;
					case IT::IDDATATYPE::STR:
						// confession(data, type) - stdcall, порядок: type (последний), data (первый)
						// Для STR переменная содержит указатель на строку (dword), нужно использовать содержимое
						if (e.idtype == IT::IDTYPE::L)
						{
							// Литерал - используем offset
							str += "\npush 2\n"; // type (последний параметр)
							str += "push offset " + string(e.id) + "\n"; // data (первый параметр)
						}
						else
						{
							// Переменная - используем содержимое (указатель на строку)
							str += "\npush 2\n"; // type (последний параметр)
							str += "push " + string(e.id) + "\n"; // data (содержимое переменной = указатель)
						}
						str += "call confession\n";
						// Стек очищается функцией (ret 8), не нужно add esp
						break;
					case IT::IDDATATYPE::CHAR:
						// confession(data, type) - stdcall, порядок: type (последний), data (первый)
						// Для CHAR переменная содержит указатель на строку (dword), нужно использовать содержимое
						if (e.idtype == IT::IDTYPE::L)
						{
							// Литерал - используем offset
							str += "\npush 4\n"; // type (последний параметр)
							str += "push offset " + string(e.id) + "\n"; // data (первый параметр)
						}
						else
						{
							// Переменная - используем содержимое (указатель на строку)
							str += "\npush 4\n"; // type (последний параметр)
							str += "push " + string(e.id) + "\n"; // data (содержимое переменной = указатель)
						}
						str += "call confession\n";
						// Стек очищается функцией (ret 8), не нужно add esp
						break;
					}
					// Пропускаем аргумент print, чтобы он не обрабатывался повторно
					i++;
				}
				break;
			}
			case LEX_NEWLINE:
			{
				str += "push offset newline\ncall outrad\n";
				break;
			}
			case LEX_TIRESOME:
			{
				int blockCondNum = conditionnum;
				if (!blockConditionNum.empty())
				{
					blockCondNum = blockConditionNum.top();
				}
				str += "jmp council_end" + itoS(blockCondNum) + "\n";
				break;
			}
			}
			
			if (!str.empty())
				v.push_back(str);
			str.clear();
		}
		
		v.push_back(END);
		
		for (auto x : v)
			ofile << x << endl;
		ofile.close();
	}
};
