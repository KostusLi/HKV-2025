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

using namespace std;


namespace Gener
{
	// Типы блоков для отслеживания вложенности
	enum BlockType { BLOCK_CHARGE, BLOCK_PATROL, BLOCK_COUNCIL, BLOCK_PATH, BLOCK_TIRESOME, BLOCK_OTHER };
	
	static int conditionnum = 0;
	string numberOfPoints = "k";
	string itoS(int x) { stringstream r;  r << x;  return r.str(); }

	string genCallFuncCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e = ITENTRY(i); // ������������� ���������� �������
		stack <IT::Entry> temp;
		bool stnd = (e.idtype == IT::IDTYPE::S);

		// Используем LEX_RIGHTBRAC для вызовов функций (не LEX_RIGHTTHESIS)
		for (int j = i + 1; j < tables.lextable.size && LEXEMA(j) != LEX_RIGHTBRAC; j++)
		{
			if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
				temp.push(ITENTRY(j)); // // ��������� ���� � ������ �������	
		}
		str += "\n";
		// ������������ ����
		while (!temp.empty())
		{
			if (temp.top().idtype == IT::IDTYPE::L && temp.top().iddatatype == IT::IDDATATYPE::STR || temp.top().iddatatype == IT::IDDATATYPE::CHAR)
				str = str + "push offset " + temp.top().id + "\n";
			else   str = str + "push " + temp.top().id + "\n";
			temp.pop();
		}
		if (stnd)
			str += "push offset buffer\n";
		str = str + "call " + string(e.id) + "\n";
		// �����������e �����
		return str;
	}

	string genEqualCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		// Проверяем границы перед доступом
		if (i - 1 < 0 || i - 1 >= tables.lextable.size || tables.lextable.table[i - 1].idxTI == TI_NULLIDX)
			return "";
		IT::Entry e1 = ITENTRY(i - 1); // ����� �������
		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::INT:
		{
			bool first = true;
			for (int j = i + 1; j < tables.lextable.size && LEXEMA(j) != LEX_SEPARATORS && LEXEMA(j) != LEX_SEMICOLON; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (j >= tables.lextable.size || tables.lextable.table[j].idxTI == TI_NULLIDX)
						continue;
					if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S) // ���� � ��������� ����� �������
					{
						str = str + genCallFuncCode(tables, log, j); // ������� ���������� ��������� � eax
						str = str + "push eax\n";				// ��������� ��������� � ���� ��� ����������� ���������� ���������
						break;
					}
					else if (j < tables.lextable.size && tables.lextable.table[j].idxTI != TI_NULLIDX)
						str = str + "push " + ITENTRY(j).id + "\n";
					break;
				}
				case LEX_PLUS:
					str = str + "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
				case LEX_MINUS:
					str = str + "pop ebx\npop eax\nsub eax, ebx\njnc b" + numberOfPoints + "\n" + "neg eax\n" + "b" + numberOfPoints + ": \n" + "push eax\n"; numberOfPoints = numberOfPoints + "m"; break;
				case LEX_STAR:
					str = str + "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
				case LEX_DIRSLASH:
					str = str + "pop ebx\npop eax\ncdq\nidiv ebx\npush eax\n"; break;
				case '%':
					str = str + "pop ebx\npop eax\ncdq\nmov edx,0\nidiv ebx\npush edx\n"; break;
				case LEX_ALLY:
					str = str + "pop ebx\npop eax\nor eax, ebx\npush eax\n"; break;
				case LEX_BIND:
					str = str + "pop ebx\npop eax\nand eax, ebx\npush eax\n"; break;
				case LEX_BANISH:
					str = str + "pop eax\nnot eax\npush eax\n"; break;
				// Обработка стандартных функций в постфиксной записи
				case LEX_COMPSCROLLS:
				case LEX_CONSOLIDATE:
				case LEX_MIGHTINESS:
				case LEX_FILAMENT:
				{
					// Это имя стандартной функции в постфиксной записи (после аргументов)
					string funcName;
					if (LEXEMA(j) == LEX_COMPSCROLLS) funcName = "compare_scrolls";
					else if (LEXEMA(j) == LEX_CONSOLIDATE) funcName = "consolidate";
					else if (LEXEMA(j) == LEX_MIGHTINESS) funcName = "mightiness";
					else funcName = "filament";
					
					str = str + "call " + funcName + "\n";
					str += "push offset buffer\n";
					str += "push eax\n"; // Результат функции
					break;
				}
				}
			} // конец выражения���� ����������

			str = str + "\npop ebx\nmov " + e1.id + ", ebx\n";			// ����������� ��������� � ebx 
			break;
		}
		case IT::IDDATATYPE::STR:// ��������� ����������� ������� ������ ������, �������� � ������ �������
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S)) // ����� �������
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + e1.id + ", eax";
			}
			else if (lex == LEX_LITERAL) // �������
			{
				str = str + "mov " + e1.id + ", offset " + e2.id;
			}
			else // ��(����������) - ����� �������
			{
				str = str + "mov ecx, " + e2.id + "\nmov " + e1.id + ", ecx";
			}
			break;
		}
		case IT::IDDATATYPE::CHAR:// ��������� ����������� ������� ������ ������, �������� � ������ �������
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S)) // ����� �������
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + e1.id + ", eax";
			}
			else if (lex == LEX_LITERAL) // �������
			{
				str = str + "mov " + e1.id + ", offset " + e2.id;
			}
			else // ��(����������) - ����� �������
			{
				str = str + "mov ecx, " + e2.id + "\nmov " + e1.id + ", ecx";
			}
			break;
		}
		}
		return str;
	}

	string genFunctionCode(Lexer::LEX& tables, int i, string funcname, int pcount)
	{
		if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].idxTI == TI_NULLIDX)
			return "";
		string str;
		IT::Entry e = ITENTRY(i + 1);
		IT::IDDATATYPE type = e.iddatatype;
		str = SEPSTR(funcname) + string(e.id) + string(" PROC,\n\t");
		//������ ���������
		int j = i + 3; // ������ - �� ��� ����� ����� ����������� ������
			while (j < tables.lextable.size && LEXEMA(j) != LEX_RIGHTTHESIS) //   ���� ��������� �� ��������
		{
			if (LEXEMA(j) == LEX_ID) // ��������
				str = str + string(ITENTRY(j).id) + (ITENTRY(j).iddatatype == IT::IDDATATYPE::INT ? " : sdword, " : " : dword, ");
			j++;
		}
		int f = str.rfind(',');
		if (f > 0)
			str[f] = IN_CODE_SPACE;
		str += "\n; --- save registers ---\npush ebx\npush edx\n; ----------------------";
		return str;
	}

	string genExitCode(Lexer::LEX& tables, int i, string funcname, int pcount)
	{
		string str = "; --- restore registers ---\npop edx\npop ebx\n; -------------------------\n";
		if (LEXEMA(i + 1) != LEX_SEPARATORS)	// ����� �� ������� (������� ��������)
		{
			str = str + "mov eax, " + string(ITENTRY(i + 1).id) + "\n";
		}
		str += "ret\n";
		str += funcname + " ENDP" + SEPSTREMP;
		return str;
	}

	string genConditionCode(Lexer::LEX& tables, int i, string& cyclecode)
	{
		string str;
		conditionnum++;
		cyclecode.clear();
		IT::Entry lft = ITENTRY(i + 1); // ����� �������
		IT::Entry rgt = ITENTRY(i + 3); // ������ �������
		bool w = false, r = false, c = false;
		string wstr, rstr, rstr2;

		// Ищем структуру charge/patrol: ищем закрывающую скобку условия ] и затем блоки { }
		int bracketEnd = i + 2;
		while (bracketEnd < tables.lextable.size && bracketEnd < i + 10 && LEXEMA(bracketEnd) != LEX_RIGHTTHESIS)
			bracketEnd++;
		
		// Ищем faith/exile/patrol после закрытия условия (до следующего блока или ограничения)
		for (int j = bracketEnd + 1; j < tables.lextable.size && j < bracketEnd + 30; j++) // Ограничиваем поиск // ���������� ����������� �������
		{
			if (LEXEMA(j) == LEX_FAITH) r = true;
			if (LEXEMA(j) == LEX_EXILE) w = true;
			if (LEXEMA(j) == LEX_PATROL) c = true;
			if (LEXEMA(j) == LEX_BACKUP) w = true; // backup тоже указывает на else блок
		}
		str = str + "mov edx, " + lft.id + "\ncmp edx, " + rgt.id + "\n";
		switch (LEXEMA(i + 2))
		{
		case LEX_MORE:  rstr = "jg";  wstr = "jl";  break;
		case LEX_LESS:   rstr = "jl";  wstr = "jg";  break;
		case LEX_EQUALS:    rstr = "jz";  wstr = "jnz";  break;
		case LEX_NONEQUALS:   rstr = "jnz";  wstr = "jz";  break;
		case LEX_MOREEQUALS:   rstr = "jz"; rstr2 = "jg";  wstr = "jnz";  break;
		case LEX_LESSEQUALS:   rstr = "jz"; rstr2 = "jl";  wstr = "jnz";  break;
		}

		if (LEXEMA(i + 2) == LEX_MORE || LEXEMA(i + 2) == LEX_LESS || LEXEMA(i + 2) == LEX_EQUALS || LEXEMA(i + 2) == LEX_NONEQUALS) {
			if (!c && r) str = str + "\n" + rstr + " right" + itoS(conditionnum);
			if (!c && w) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);
		}
		if (LEXEMA(i + 2) == LEX_MOREEQUALS || LEXEMA(i + 2) == LEX_LESSEQUALS) {
			if (!c && r) str = str + "\n" + rstr + " right" + itoS(conditionnum) + "\n" + rstr2 + " right" + itoS(conditionnum);
			if (!c && w) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);
		}
		if (c)
		{
			// Для patrol: условие проверяется в начале цикла
			// Если условие истинно - переходим в тело цикла
			// Если ложно - переходим к концу цикла (cyclenext)
			// Условие уже сгенерировано выше (mov edx, cmp edx)
			// rstr содержит команду перехода (jg, jl и т.д.)
			str = str + "\n" + rstr + " patrol_start" + itoS(conditionnum) + "_body\n";
			str = str + "jmp cyclenext" + itoS(conditionnum) + "\n";
			str = str + "patrol_start" + itoS(conditionnum) + "_body:\n";
			cyclecode = str;
		}
		else if (!r || !w)  str = str + "\njmp next" + itoS(conditionnum);
		return str;
	}

	vector <string> startFillVector(Lexer::LEX& tables)
	{
		vector <string> v;
		v.push_back(BEGIN);
		v.push_back(EXTERN);
		vector <string> vlt;  vlt.push_back(CONST);
		vector <string> vid;  vid.push_back(DATA);
		for (int i = 0; i < tables.idtable.size; i++)// const, data
		{
			IT::Entry e = tables.idtable.table[i];
			string str = "\t\t" + string(e.id);

			if (tables.idtable.table[i].idtype == IT::IDTYPE::L)	// ������� - � .const
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:  str = str + " sdword " + itoS(e.value.vint);  break;
				case IT::IDDATATYPE::STR:  str = str + " byte '" + string(e.value.vstr.str) + "', 0";  break;
				case IT::IDDATATYPE::CHAR:  str = str + " byte '" + string(e.value.vstr.str) + "', 0";  break;
				}
				vlt.push_back(str);
			}
			else if (tables.idtable.table[i].idtype == IT::IDTYPE::V)// ���������� - � .data
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT: str = str + " dword 0";  break;
				case IT::IDDATATYPE::STR: str = str + " dword ?";  break;
				case IT::IDDATATYPE::CHAR: str = str + " dword ?";  break;
				}
				vid.push_back(str);
			}
		}
		v.insert(v.end(), vlt.begin(), vlt.end());
		v.insert(v.end(), vid.begin(), vid.end());
		v.push_back(CODE);
		return v;
	}

	void CodeGeneration(Lexer::LEX& tables, Parm::PARM& parm, Log::LOG& log)
	{
		vector <string> v = startFillVector(tables);
		
		// Преобразуем широкий символ в обычную строку для ofstream
		char outFile[PARM_MAX_SIZE];
		size_t converted = 0;
		wcstombs_s(&converted, outFile, PARM_MAX_SIZE, parm.out, wcslen(parm.out) + 1);
		
		ofstream ofile(outFile);
		if (!ofile.is_open())
		{
			throw ERROR_THROW(104);
		}
		
		// Стек для отслеживания типов блоков при открывающих скобках {
		stack<BlockType> blockStack;
		stack<int> blockConditionNum; // Сохраняем номер условия для каждого блока
		
		string funcname;	// ��� ������� ������
		string cyclecode;	// ������ �����: cmp + j
		int pcount = 0;		//   ���������� ���������� ������� �������
		string str;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_TEMPLE:
			{
				str = str + SEPSTR("MAIN") + "main PROC";
				break;
			}
			case LEX_ACTION:
			{
				if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].idxTI == TI_NULLIDX)
					break;
				funcname = ITENTRY(i + 1).id;
				pcount = ITENTRY(i + 1).value.params.count;
				str = genFunctionCode(tables, i, funcname, pcount);
				break;
			}
			case LEX_COMEBACK:
			{
				str = genExitCode(tables, i, funcname, pcount);
				break;
			}
			case LEX_ID: // ����� �������
			{
				// Используем LEX_LEFTBRAC для вызовов функций (не LEX_LEFTHESIS)
				if (LEXEMA(i + 1) == LEX_LEFTBRAC && LEXEMA(i - 1) != LEX_ACTION) // не объявление, а вызов�� ����������, � �����
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
			case LEX_CHARGE: // �������
			{
				str = genConditionCode(tables, i, cyclecode);
				break;
			}
			case LEX_BRACELET:	// ������� �� ����� � ����� ��������
			{
				// Используем стек для определения типа закрываемого блока
				// Это правильный способ - стек обеспечивает правильное соответствие скобок
				if (blockStack.empty())
				{
					// Стек пуст - это ошибка, но продолжаем работу (может быть блок функции/temple)
					break;
				}
				
				// Берем тип блока из стека (правильное соответствие скобок!)
				BlockType blockType = blockStack.top();
				blockStack.pop();
				
				int blockCondNum = conditionnum;
				if (!blockConditionNum.empty())
				{
					blockCondNum = blockConditionNum.top();
					blockConditionNum.pop();
				}
				
				// Генерируем код в зависимости от типа блока
				switch (blockType)
				{
				case BLOCK_PATH:
				{
					// Обработка закрытия path блока - переход к концу council
					// Проверяем, не следует ли сразу другой path или tiresome
					bool hasMorePaths = false;
					for (int j = i + 1; j < tables.lextable.size && j < i + 10; j++)
					{
						if (LEXEMA(j) == LEX_PATH || LEXEMA(j) == LEX_TIRESOME)
						{
							hasMorePaths = true;
							break;
						}
						if (LEXEMA(j) == LEX_BRACELET || LEXEMA(j) == LEX_COUNCIL) // Другая закрывающая скобка или конец council
							break;
					}
					// После path блока переходим к концу council (если нет больше path)
					if (!hasMorePaths && i + 1 < tables.lextable.size && LEXEMA(i + 1) != LEX_PATH)
						str = str + "jmp council_end" + itoS(blockCondNum) + "\n";
					break;
				}
				case BLOCK_PATROL:
				{
					// Обработка закрытия patrol цикла - переход к началу
					// Переход к началу цикла (к условию patrol)
					str = str + "jmp patrol_start" + itoS(blockCondNum) + "\n";
					// Метка выхода из цикла
					str = str + "cyclenext" + itoS(blockCondNum) + ":\n";
					break;
				}
				case BLOCK_COUNCIL:
				{
					// Обработка закрытия council блока - метка конца
					str = str + "council_end" + itoS(blockCondNum) + ":\n";
					break;
				}
				case BLOCK_CHARGE:
				{
					// Обработка закрытия charge блока - метка next для if-else
					// Проверяем, следует ли после этого backup (else)
					if (i + 1 < tables.lextable.size && (LEXEMA(i + 1) == LEX_EXILE || LEXEMA(i + 1) == LEX_FAITH || LEXEMA(i + 1) == LEX_BACKUP))
					{
						// Это закрытие if блока, перед else/backup - ничего не делаем, метка next будет после else
					}
					else
					{
						// Это конец if-else конструкции
						str = str + "next" + itoS(blockCondNum) + ":\n";
					}
					break;
				}
				case BLOCK_TIRESOME:
				{
					// Обработка закрытия tiresome блока - переход к концу council
					str = str + "jmp council_end" + itoS(blockCondNum) + "\n";
					break;
				}
				case BLOCK_OTHER:
				default:
					// Обычный блок (функция, temple) - никаких специальных действий
					break;
				}
				break;
			}
			case LEX_FAITH: // ������� �����(�����)
			{
				str = str + "right" + itoS(conditionnum) + ":";
				break;
			}
			case LEX_EXILE: // ������� �������(�����)
			{
				str = str + "wrong" + itoS(conditionnum) + ":";
				break;
			}
			case LEX_PATROL: // ���� � �������� (�����)
			{
				// patrol [condition] { body }
				// Добавляем метку начала цикла для перехода из конца тела
				str = str + "patrol_start" + itoS(conditionnum) + ":\n";
				break;
			}
			// Обработка council (switch)
			case LEX_COUNCIL:
			{
				// council [E] { path literal {C} ... tiresome {C} }
				conditionnum++;
				if (i + 2 < tables.lextable.size && LEXEMA(i + 2) != LEX_RIGHTTHESIS)
				{
					// Проверяем границы и валидность индекса перед доступом
					if (tables.lextable.table[i + 2].idxTI != TI_NULLIDX)
					{
						IT::Entry expr = ITENTRY(i + 2); // Выражение после [
						str = str + "mov edx, " + expr.id + "\n";
					}
				}
				// После закрытия council блока будет добавлена метка council_end
				break;
			}
			case LEX_PATH:
			{
				// path literal { C }
				if (i + 1 < tables.lextable.size && LEXEMA(i + 1) == LEX_LITERAL)
				{
					// Проверяем границы и валидность индекса перед доступом
					if (tables.lextable.table[i + 1].idxTI != TI_NULLIDX)
					{
						IT::Entry literal = ITENTRY(i + 1);
						str = str + "cmp edx, " + literal.id + "\n";
						str = str + "jz path" + itoS(conditionnum) + "_" + itoS(i) + "\n";
					}
					// После всех path проверок переходим к tiresome или концу
					// Это будет обработано при закрытии последнего path блока
				}
				break;
			}
			case LEX_LEFTBRACE:
			{
				// Определяем тип блока по предыдущим лексемам
				BlockType blockType = BLOCK_OTHER;
				
				// Проверяем, является ли это началом блока path
				if (i > 1 && LEXEMA(i - 1) == LEX_LITERAL && i > 2 && LEXEMA(i - 2) == LEX_PATH)
				{
					// Это начало блока path - добавляем метку
					str = str + "path" + itoS(conditionnum) + "_" + itoS(i - 2) + ":\n";
					blockType = BLOCK_PATH;
				}
				// Проверяем, является ли это началом блока patrol
				else if (i > 0)
				{
					// Ищем patrol назад до 10 позиций
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_PATROL)
						{
							blockType = BLOCK_PATROL;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS) // Условие цикла уже закрыто
							break;
					}
				}
				// Проверяем, является ли это началом блока charge
				if (blockType == BLOCK_OTHER && i > 0)
				{
					// Ищем charge назад до 10 позиций
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_CHARGE)
						{
							blockType = BLOCK_CHARGE;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS) // Условие уже закрыто
							break;
					}
				}
				// Проверяем, является ли это началом блока council
				if (blockType == BLOCK_OTHER && i > 0)
				{
					// Ищем council назад до 10 позиций
					for (int j = i - 1; j >= 0 && j > i - 10; j--)
					{
						if (LEXEMA(j) == LEX_COUNCIL)
						{
							blockType = BLOCK_COUNCIL;
							break;
						}
						if (LEXEMA(j) == LEX_RIGHTTHESIS) // Условие уже закрыто
							break;
					}
				}
				// Проверяем, является ли это началом блока tiresome
				if (blockType == BLOCK_OTHER && i > 0)
				{
					// Ищем tiresome назад до 5 позиций
					for (int j = i - 1; j >= 0 && j > i - 5; j--)
					{
						if (LEXEMA(j) == LEX_TIRESOME)
						{
							blockType = BLOCK_TIRESOME;
							break;
						}
						if (LEXEMA(j) == LEX_COUNCIL) // Выходим за пределы council
							break;
					}
				}
				
				// Сохраняем тип блока и номер условия в стек
				blockStack.push(blockType);
				blockConditionNum.push(conditionnum);
				break;
			}
			case LEX_TIRESOME:
			{
				// tiresome { C } - default case
				// После всех path проверок переходим к tiresome если не совпал ни один
				// Проверяем, были ли перед этим path
				bool hasPaths = false;
				for (int j = i - 1; j >= 0 && j > i - 50; j--)
				{
					if (LEXEMA(j) == LEX_PATH)
					{
						hasPaths = true;
						break;
					}
					if (LEXEMA(j) == LEX_COUNCIL)
						break;
				}
				// Если были path, добавляем переход к tiresome после всех проверок
				// Этот переход должен быть после всех path проверок, перед первым path
				// Поэтому лучше добавить его в конце всех path проверок
				str = str + "tiresome_" + itoS(conditionnum) + ":\n";
				break;
			}
			case LEX_BACKUP: // backup { body } - else блок
			{
				// backup не требует специальной генерации кода
				// Метка wrong уже создана в genConditionCode
				// Это просто маркер else блока
				break;
			}
			case LEX_ASSIGN: // ������������ (���������� ���������)
			{
				str = genEqualCode(tables, log, i);
				// Пропускаем выражение до точки с запятой, проверяя границы
				while (i + 1 < tables.lextable.size && LEXEMA(i + 1) != LEX_SEMICOLON)
					i++;	// пропускаем выражение���������� ���������
				break;
			}
			case LEX_NEWLINE: // ������� ������ 
			{
				str = str + "push offset newline\ncall outrad\n";
				break;
			}
			case LEX_PRINT: // �����
			{
				// Проверяем границы перед доступом
				if (i + 1 >= tables.lextable.size || tables.lextable.table[i + 1].idxTI == TI_NULLIDX)
					break;
				IT::Entry e = ITENTRY(i + 1);
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:
					str = str + "\npush " + e.id + "\ncall outlich\n";
					break;
				case IT::IDDATATYPE::STR:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\npush offset " + e.id + "\ncall outrad\n";
					else  str = str + "\npush " + e.id + "\ncall outrad\n";
					break;
				case IT::IDDATATYPE::CHAR:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\npush offset " + e.id + "\ncall outrad\n";
					else  str = str + "\npush " + e.id + "\ncall outrad\n";
					break;
				}
				break;
			}
			}
			if (!str.empty())
				v.push_back(str);
			str.clear();
		}
		v.push_back(END);
		// ����� � ����
		for (auto x : v)
			ofile << x << endl;
		ofile.close();
	}
};