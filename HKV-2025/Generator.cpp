#include "Generator.h"
#include "Parm.h"
#include "LexAnalysis.h"
#include "IT.h"
#include "LT.h"
#include <sstream>
#include <cstring>
#include <iosfwd>
#include <stack>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

using namespace std;


namespace Gener
{
	static int conditionnum = 0;
	static int switchnum = 0;
	string numberOfPoints = "k";
	string itoS(int x) { stringstream r;  r << x;  return r.str(); }

	string genCallFuncCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e = ITENTRY(i);
		stack <IT::Entry> temp;
		bool stnd = (e.idtype == IT::IDTYPE::S);

		for (int j = i + 1; LEXEMA(j) != LEX_RIGHTTHESIS; j++)
		{
			if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
				temp.push(ITENTRY(j));
		}
		str += "\n";
		while (!temp.empty())
		{
			if (temp.top().idtype == IT::IDTYPE::L && temp.top().iddatatype == IT::IDDATATYPE::SCROLL || temp.top().iddatatype == IT::IDDATATYPE::RUNE)
				str = str + "push offset " + temp.top().id + "\n";
			else   str = str + "push " + temp.top().id + "\n";
			temp.pop();
		}
		if (stnd)
			str += "push offset buffer\n";
		str = str + "call " + string(e.id) + IN_CODE_ENDL;
		return str;
	}

	string genEqualCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e1 = ITENTRY(i - 1);
		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::SQUIRE:
		{
			bool first = true;
			for (int j = i + 1; LEXEMA(j) != LEX_SEPARATOR; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S)
					{
						str = str + genCallFuncCode(tables, log, j);
						str = str + "push eax\n";
						while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
						break;
					}
					else  str = str + "push " + ITENTRY(j).id + "\n";
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
				case LEX_PERSENT:
					str = str + "pop ebx\npop eax\ncdq\nmov edx,0\nidiv ebx\npush edx\n"; break;
				case LEX_BITAND:
					str = str + "pop ebx\npop eax\nand eax, ebx\npush eax\n"; break;
				case LEX_BITOR:
					str = str + "pop ebx\npop eax\nor eax, ebx\npush eax\n"; break;
				case LEX_BITNOT:
					str = str + "pop eax\nnot eax\npush eax\n"; break;
				}
			}
			str = str + "\npop ebx\nmov " + e1.id + ", ebx\n";
			break;
		}
		case IT::IDDATATYPE::SCROLL:
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S))
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + e1.id + ", eax";
			}
			else if (lex == LEX_LITERAL)
			{
				str = str + "mov " + e1.id + ", offset " + e2.id;
			}
			else
			{
				str = str + "mov ecx, " + e2.id + "\nmov " + e1.id + ", ecx";
			}
			break;
		}
		case IT::IDDATATYPE::RUNE:
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S))
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + e1.id + ", eax";
			}
			else if (lex == LEX_LITERAL)
			{
				str = str + "mov " + e1.id + ", offset " + e2.id;
			}
			else
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
		string str;
		IT::Entry e = ITENTRY(i + 1);
		IT::IDDATATYPE type = e.iddatatype;
		str = SEPSTR(funcname) + string(e.id) + string(" PROC,\n\t");
		int j = i + 3;
		while (LEXEMA(j) != LEX_RIGHTTHESIS)
		{
			if (LEXEMA(j) == LEX_ID)
				str = str + string(ITENTRY(j).id) + (ITENTRY(j).iddatatype == IT::IDDATATYPE::SQUIRE ? " : sdword, " : " : dword, ");
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
		if (LEXEMA(i + 1) != LEX_SEPARATOR)
		{
			str = str + "mov eax, " + string(ITENTRY(i + 1).id) + "\n";
		}
		str += "ret\n";
		str += funcname + " ENDP" + SEPSTREMP;
		return str;
	}

	/*string genConditionCode(Lexer::LEX& tables, int i, string& cyclecode)
	{
		string str;
		conditionnum++;
		cyclecode.clear();
		IT::Entry lft = ITENTRY(i + 1);
		IT::Entry rgt = ITENTRY(i + 3);
		bool w = false, r = false, c = false;
		string wstr, rstr, rstr2;

		for (int j = i + 5; LEXEMA(j) != LEX_STENKA; j++)
		{
			if (LEXEMA(j) == LEX_CHARGE) r = true;
			if (LEXEMA(j) == LEX_BACKUP) w = true;
			if (LEXEMA(j) == LEX_PATROL) c = true;
		}
		str = str + "mov edx, " + lft.id + "\ncmp edx, " + rgt.id + "\n";
		switch (LEXEMA(i + 2))
		{
		case LEX_MORE:  rstr = "jg";  wstr = "jl";  break;
		case LEX_LESS:   rstr = "jl";  wstr = "jg";  break;
		case LEX_EQUALS:    rstr = "jz";  wstr = "jnz";  break;
		case LEX_NOTEQUALS:   rstr = "jnz";  wstr = "jz";  break;
		case LEX_MOREEQUALS:   rstr = "jz"; rstr2 = "jg";  wstr = "jnz";  break;
		case LEX_LESSEQUALS:   rstr = "jz"; rstr2 = "jl";  wstr = "jnz";  break;
		}

		if (LEXEMA(i + 2) == LEX_MORE || LEXEMA(i + 2) == LEX_LESS || LEXEMA(i + 2) == LEX_EQUALS || LEXEMA(i + 2) == LEX_NOTEQUALS) {
			if (!c && r) str = str + "\n" + rstr + " right" + itoS(conditionnum);
			if (!c && w) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);
		}
		if (LEXEMA(i + 2) == LEX_MOREEQUALS || LEXEMA(i + 2) == LEX_LESSEQUALS) {
			if (!c && r) str = str + "\n" + rstr + " right" + itoS(conditionnum) + "\n" + rstr2 + " right" + itoS(conditionnum);
			if (!c && w) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);
		}
		if (c)
		{
			str = str + "\n" + rstr + " cycle" + itoS(conditionnum);
			cyclecode = str;
			str = str + "\njmp cyclenext" + itoS(conditionnum);
		}
		else if (!r || !w)  str = str + "\njmp next" + itoS(conditionnum);
		return str;
	}*/

	string genConditionCode(Lexer::LEX& tables, int i, string& cyclecode)
	{
		string str;
		conditionnum++;
		cyclecode.clear();
		IT::Entry lft = ITENTRY(i + 1);
		IT::Entry rgt = ITENTRY(i + 3);
		bool w = false, r = false, c = false;
		string wstr, rstr;

		// Определяем наличие блоков charge (r), backup (w), patrol (c)
		for (int j = i + 5; LEXEMA(j) != LEX_STENKA; j++)
		{
			if (LEXEMA(j) == LEX_CHARGE) r = true;
			if (LEXEMA(j) == LEX_BACKUP) w = true;
			if (LEXEMA(j) == LEX_PATROL) c = true;
		}

		str = str + "mov edx, " + lft.id + "\ncmp edx, " + rgt.id + "\n";

		// ПОЛНОСТЬЮ ОБНОВЛЕННАЯ ЛОГИКА ПЕРЕХОДОВ
		switch (LEXEMA(i + 2))
		{
			// > (MORE)
			// Если истина: JG (Greater), Если ложь: JLE (Less or Equal)
		case LEX_MORE:       rstr = "jg";  wstr = "jle"; break;

			// < (LESS)
			// Если истина: JL (Less), Если ложь: JGE (Greater or Equal)
		case LEX_LESS:       rstr = "jl";  wstr = "jge"; break;

			// == (EQUALS) (&)
			// Если истина: JE (Equal), Если ложь: JNE (Not Equal)
		case LEX_EQUALS:     rstr = "je";  wstr = "jne"; break;

			// != (NOTEQUALS) (!)
			// Если истина: JNE (Not Equal), Если ложь: JE (Equal)
		case LEX_NOTEQUALS:  rstr = "jne"; wstr = "je";  break;

			// >= (MOREEQUALS) (~)
			// Если истина: JGE (Greater or Equal), Если ложь: JL (Less)
		case LEX_MOREEQUALS: rstr = "jge"; wstr = "jl";  break;

			// <= (LESSEQUALS) (@)
			// Если истина: JLE (Less or Equal), Если ложь: JG (Greater)
		case LEX_LESSEQUALS: rstr = "jle"; wstr = "jg";  break;
		}

		// Генерация прыжков

		// 1. Блок Цикла (patrol)
		if (c)
		{
			str = str + "\n" + rstr + " cycle" + itoS(conditionnum);
			cyclecode = str; // Сохраняем код для повторной вставки в конце цикла
			str = str + "\njmp cyclenext" + itoS(conditionnum); // Если условие не выполнилось - выход
		}
		else
		{
			// 2. Блок Условия (check)

			// Если условие ИСТИННО и есть блок charge -> прыгаем в charge
			if (r) str = str + "\n" + rstr + " right" + itoS(conditionnum);

			// Если условие ЛОЖНО и есть блок backup -> прыгаем в backup
			if (w) str = str + "\n" + wstr + " wrong" + itoS(conditionnum);

			// ВАЖНО: Защита от "проваливания"
			// Если условие ЛОЖНО, но блока backup НЕТ, мы должны перепрыгнуть через charge (в next)
			// Иначе программа выполнит charge, даже если условие ложно.
			if (!w)
			{
				str = str + "\n" + wstr + " next" + itoS(conditionnum);
			}
		}

		return str;
	}

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
				case IT::IDDATATYPE::SQUIRE:  str = str + " sdword " + itoS(e.value.vint);  break;
				case IT::IDDATATYPE::SCROLL:  str = str + " byte '" + string(e.value.vstr.str) + "', 0";  break;
				case IT::IDDATATYPE::RUNE:  str = str + " byte '" + string(e.value.vstr.str) + "', 0";  break;
				}
				vlt.push_back(str);
			}
			else if (tables.idtable.table[i].idtype == IT::IDTYPE::V)
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::SQUIRE: str = str + " dword 0";  break;
				case IT::IDDATATYPE::SCROLL: str = str + " dword ?";  break;
				case IT::IDDATATYPE::RUNE: str = str + " dword ?";  break;
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
		ofstream ofile(parm.out);
		string funcname;
		string cyclecode;
		int pcount;
		string str;

		struct SwitchCtx
		{
			int id;
			int endPos;
			string endLabel;
			string defaultLabel;
			bool hasDefault;
			std::map<int, std::pair<int, string>> cases;
		};
		std::stack<SwitchCtx> switchStack;
		std::unordered_set<std::string> emittedLabels;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_COUNCIL:
			{
				int exprPos = -1;
				int j = i + 1;
				while (j < tables.lextable.size && LEXEMA(j) != LEX_RIGHTTHESIS)
				{
					if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)
					{
						exprPos = j;
						break;
					}
					++j;
				}
				if (exprPos == -1) break;

				int outerBegin = -1;
				while (j < tables.lextable.size && LEXEMA(j) != LEX_LEFTBRACE) ++j;
				if (j < tables.lextable.size && LEXEMA(j) == LEX_LEFTBRACE)
					outerBegin = j;
				if (outerBegin == -1) break;

				SwitchCtx ctx{};
				ctx.id = ++switchnum;
				ctx.endLabel = "switch_end" + itoS(ctx.id);
				ctx.hasDefault = false;
				int depth = 0;
				int caseNum = 0;
				for (int p = outerBegin; p < tables.lextable.size; ++p)
				{
					if (LEXEMA(p) == LEX_LEFTBRACE) depth++;
					if (LEXEMA(p) == LEX_BRACELET)
					{
						depth--;
						if (depth == 0)
						{
							ctx.endPos = p;
							break;
						}
					}
					if (LEXEMA(p) == LEX_PATH)
					{
						string lbl = "case" + itoS(ctx.id) + "_" + itoS(++caseNum);
						int valIdx = tables.lextable.table[p + 1].idxTI;
						ctx.cases[p] = { valIdx, lbl };
					}
					if (LEXEMA(p) == LEX_TIRESOME)
					{
						ctx.hasDefault = true;
						ctx.defaultLabel = "default" + itoS(ctx.id);
					}
				}
				if (ctx.endPos == 0) ctx.endPos = tables.lextable.size - 1;

				IT::Entry expr = ITENTRY(exprPos);
				string exprOperand;
				if (expr.idtype == IT::IDTYPE::L && (expr.iddatatype == IT::IDDATATYPE::SCROLL || expr.iddatatype == IT::IDDATATYPE::RUNE))
					exprOperand = "offset " + string(expr.id);
				else
					exprOperand = string(expr.id);

				str = "mov eax, " + exprOperand + "\n";
				for (auto& kv : ctx.cases)
				{
					int idx = kv.second.first;
					string lbl = kv.second.second;
					IT::Entry ce = tables.idtable.table[idx];
					string caseOperand;
					if (ce.idtype == IT::IDTYPE::L && (ce.iddatatype == IT::IDDATATYPE::SCROLL || ce.iddatatype == IT::IDDATATYPE::RUNE))
						caseOperand = "offset " + string(ce.id);
					else
						caseOperand = string(ce.id);
					str += "cmp eax, " + caseOperand + "\nje " + lbl + "\n";
				}
				if (ctx.hasDefault)
					str += "jmp " + ctx.defaultLabel + "\n";
				else
					str += "jmp " + ctx.endLabel + "\n";

				switchStack.push(ctx);
				break;
			}
			case LEX_TEMPLE:
			{
				str = str + SEPSTR("MAIN") + "main PROC";
				break;
			}
			case LEX_ACTION:
			{
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
			case LEX_ID:
			{
				if (LEXEMA(i + 1) == LEX_LEFTHESIS && LEXEMA(i - 1) != LEX_ACTION)
					str = genCallFuncCode(tables, log, i);
				break;
			}
			case LEX_CONDITION:
			{
				str = genConditionCode(tables, i, cyclecode);
				break;
			}
			case LEX_BRACELET:
			{
				if (!switchStack.empty() && i == switchStack.top().endPos)
				{
					std::string lbl = switchStack.top().endLabel + ":";
					if (emittedLabels.insert(lbl).second)
						str = str + lbl;
					else
						str.clear();
					switchStack.pop();
					break;
				}
				if (LEXEMA(i + 1) == LEX_BACKUP || LEXEMA(i + 1) == LEX_CHARGE)
					str = str + "jmp next" + itoS(conditionnum);
			}
			case LEX_STENKA:
			{
				if (LEXEMA(i - 1) == LEX_BRACELET)
				{
					bool c = false;
					for (int j = i; j > 0 && LEXEMA(j) != LEX_CONDITION; j--)
						if (LEXEMA(j) == LEX_PATROL)
							c = true;
					if (c)
					{
						std::string lbl = "cyclenext" + itoS(conditionnum) + ":";
						if (emittedLabels.insert(lbl).second)
							str = cyclecode + "\n" + lbl;
						else
							str.clear();
					}
					else
					{
						std::string lbl = "next" + itoS(conditionnum) + ":";
						if (emittedLabels.insert(lbl).second)
							str += lbl;
						else
							str.clear();
					}
				}
				break;
			}
			case LEX_CHARGE:
			{
				std::string lbl = "right" + itoS(conditionnum) + ":";
				if (emittedLabels.insert(lbl).second)
					str = str + lbl;
				else
					str.clear();
				break;
			}
			case LEX_PATH:
			{
				if (!switchStack.empty())
				{
					auto it = switchStack.top().cases.find(i);
					if (it != switchStack.top().cases.end())
						str = str + it->second.second + ":";
				}
				break;
			}
			case LEX_ESCAPE:
			{
				if (!switchStack.empty())
					str = str + "jmp " + switchStack.top().endLabel + "\n";
				break;
			}
			case LEX_TIRESOME:
			{
				if (!switchStack.empty() && switchStack.top().hasDefault)
					str = str + switchStack.top().defaultLabel + ":";
				break;
			}
			case LEX_BACKUP:
			{
				std::string lbl = "wrong" + itoS(conditionnum) + ":";
				if (emittedLabels.insert(lbl).second)
					str = str + lbl;
				else
					str.clear();
				break;
			}
			case LEX_PATROL:
			{
				std::string lbl = "cycle" + itoS(conditionnum) + ":";
				if (emittedLabels.insert(lbl).second)
					str = str + lbl;
				else
					str.clear();
				break;
			}
			case LEX_EQUAL:
			{
				str = genEqualCode(tables, log, i);
				while (LEXEMA(++i) != LEX_SEPARATOR);
				break;
			}
			case LEX_NEWLEAF:
			{
				str = str + "push offset newline\ncall confessionscroll\n";
				break;
			}
			case LEX_CONFESSION:
			{
				IT::Entry e = ITENTRY(i + 1);
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::SQUIRE:
					str = str + "\npush " + e.id + "\ncall confessionsquire\n";
					break;
				case IT::IDDATATYPE::SCROLL:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\npush offset " + e.id + "\ncall confessionscroll\n";
					else  str = str + "\npush " + e.id + "\ncall confessionscroll\n";
					break;
				case IT::IDDATATYPE::RUNE:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\npush offset " + e.id + "\ncall confessionscroll\n";
					else  str = str + "\npush " + e.id + "\ncall confessionscroll\n";
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
		for (auto x : v)
			ofile << x << endl;
		ofile.close();
	}
};