#include "LexAnalysis.h"
#include "Graphs.h"
#include "Error.h"
#include "Log.h"
#include <stack>
#include <string>
#include <iostream>

#pragma warning(disable : 4996)

int DecimicalNotation(std::string input, int scaleofnot) {
	try {
		bool isNegative = !input.empty() && input[0] == '-';
		size_t startIdx = isNegative ? 1 : 0;

		if (startIdx + 1 < input.size() && input[startIdx] == '0'
			&& (input[startIdx + 1] == 'o' || input[startIdx + 1] == 'O'))
		{
			startIdx += 2;
		}
		else if (startIdx < input.size() && (input[startIdx] == 'o' || input[startIdx] == 'O'))
		{
			++startIdx;
		}

		int value = std::stoi(input.substr(startIdx), nullptr, scaleofnot);
		return isNegative ? -value : value;
	}
	catch (const std::out_of_range&) {
		return input[0] == '-' ? INT_MINSIZE : INT_MAXSIZE;
	}
}

namespace Lexer
{
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_LITERAL, FST::FST(GRAPH_SQUIRE_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_SCROLL_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_RUNE_LITERAL) },
		{ LEX_ELDER, FST::FST(GRAPH_ELDER) },
		{ LEX_TEMPLE, FST::FST(GRAPH_TEMPLE) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_SQUIRE) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_SCROLL) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_RUNE) },
		{ LEX_ACTION, FST::FST(GRAPH_ACTION) },
		{ LEX_HOLLOW, FST::FST(GRAPH_HOLLOW) },
		{ LEX_COMEBACK, FST::FST(GRAPH_COMEBACK) },
		{ LEX_CONFESSION, FST::FST(GRAPH_CONFESSION) },
		{ LEX_NEWLEAF, FST::FST(GRAPH_NEWLEAF) },
		{ LEX_CONDITION, FST::FST(GRAPH_CONDITION) },
		{ LEX_COUNCIL, FST::FST(GRAPH_COUNCIL) },
		{ LEX_PATH, FST::FST(GRAPH_PATH) },
		{ LEX_ESCAPE, FST::FST(GRAPH_ESCAPE) },
		{ LEX_TIRESOME, FST::FST(GRAPH_TIRESOME) },
		{ LEX_PATROL, FST::FST(GRAPH_PATROL) },
		{ LEX_BITAND, FST::FST(GRAPH_BITAND) },
		{ LEX_BITOR, FST::FST(GRAPH_BITOR) },
		{ LEX_BITNOT, FST::FST(GRAPH_BITNOT) },
		{ LEX_BACKUP, FST::FST(GRAPH_BACKUP) },
		{ LEX_CHARGE, FST::FST(GRAPH_CHARGE) },
		{ LEX_ID, FST::FST(GRAPH_ID) },
		{ LEX_LITERAL_OCT, FST::FST(GRAPH_OCT_LITERAL) }
	};

	char* getScopeName(IT::IdTable idtable, char* prevword)
	{
		char* a = new char[7];
		a[0] = 't';
		a[1] = 'e';
		a[2] = 'm';
		a[3] = 'p';
		a[4] = 'l';
		a[5] = 'e';
		a[6] = '\0';
		if (strcmp(prevword, MAIN) == 0)
			return a;


		for (int i = idtable.size - 1; i >= 0; i--)
			if (idtable.table[i].idtype == IT::IDTYPE::F)
				return idtable.table[i].id;
		return nullptr;
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type, Log::LOG log, int line)
	{
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::SQUIRE: {
					if (ittable.table[i].value.vint == atoi(value))
						return i;
					break;
				}
				case IT::IDDATATYPE::SCROLL:
					char buf[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++)
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp(ittable.table[i].value.vstr.str, buf) == 0)
						return i;
					break;
				case IT::IDDATATYPE::RUNE:
					char buk[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++)
						buk[j - 1] = value[j];
					buk[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp(ittable.table[i].value.vstr.str, buk) == 0)
						return i;
					break;
				}
			}
		}
		return NULLIDX_TI;
	}

	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (!strcmp(TYPE_VOID, idtype))
			return IT::IDDATATYPE::HOLLOW;
		if (!strcmp(TYPE_STRING, idtype))
			return IT::IDDATATYPE::SCROLL;
		if (!strcmp(TYPE_CHAR, idtype))
			return IT::IDDATATYPE::RUNE;
		if (!strcmp(TYPE_DIGIT, idtype))
			return IT::IDDATATYPE::SQUIRE;
		if (isdigit(*curword) || *curword == LEX_MINUS)
			return IT::IDDATATYPE::SQUIRE;
		else if (*curword == IN_CODE_QUOTE)
			return IT::IDDATATYPE::SCROLL;
		else if (*curword == IN_CODE_QUOTE2)
			return IT::IDDATATYPE::RUNE;

		return IT::IDDATATYPE::INDIGENT;
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)
	{
		if (itTableIndex == NULLIDX_TI)
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return NULLIDX_TI;
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == IN_CODE_QUOTE || *id == IN_CODE_QUOTE2 || *id == LEX_MINUS)
			return true;
		return false;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(COMPARESCROLLS, id))
			return IT::STDFNC::F_COMPARESCROLLS;
		if (!strcmp(FORTUNE, id))
			return IT::STDFNC::F_FORTUNE;
		if (!strcmp(CONSOLIDATE, id))
			return IT::STDFNC::F_CONSOLIDATE;
		if (!strcmp(FILAMENT, id))
			return IT::STDFNC::F_FILAMENT;
		if (!strcmp(TRANSMUTE, id))
			return IT::STDFNC::F_TRANSMUTE;
		if (!strcmp(OBLIVION, id))
			return IT::STDFNC::F_OBLIVION;
		return IT::STDFNC::F_FOLK;
	}

	char* getNextLiteralName()
	{
		static int literaldigit = 1;
		char* buf = new char[SCOPED_ID_MAXSIZE], lich[3];
		strcpy_s(buf, MAXSIZE_ID, "LTRL");
		_itoa_s(literaldigit++, lich, 10);
		strcat(buf, lich);
		return buf;
	}

	IT::Entry* getEntry(
		Lexer::LEX& tables,
		char lex,
		char* id,
		char* idtype,
		bool isParam,
		bool isFunc,
		Log::LOG log,
		int line,
		bool& lex_ok)
	{
		IT::IDDATATYPE type = getType(id, idtype);
		int index = IT::isId(tables.idtable, id);
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type, log, line);
		if (index != NULLIDX_TI)
			return nullptr;

		IT::Entry* itentry = new IT::Entry;
		itentry->iddatatype = type;

		itentry->idxfirstLE = getIndexInLT(tables.lextable, index);

		if (lex == LEX_LITERAL)
		{
			bool int_ok = IT::SetValue(itentry, id);
			if (!int_ok)
			{
				Log::writeError(log.stream, Error::GetError(313, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::SCROLL && itentry->value.vstr.len == 0)
			{
				Log::writeError(log.stream, Error::GetError(310, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::RUNE && itentry->value.vstr.len > CHAR_MAXSIZE)
			{
				Log::writeError(log.stream, Error::GetError(319, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::RUNE && itentry->value.vstr.len == 0)
			{
				Log::writeError(log.stream, Error::GetError(310, line, 0));
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else
		{
			switch (type)
			{
			case IT::IDDATATYPE::SCROLL:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = STR_DEFAULT;
				break;
			case IT::IDDATATYPE::RUNE:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = STR_DEFAULT;
				break;
			case IT::IDDATATYPE::SQUIRE:
				itentry->value.vint = INT_DEFAULT;
				break;
			}

			if (isFunc)
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_FORTUNE:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = FORTUNE_TYPE;
					itentry->value.params.count = FORTUNE_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[FORTUNE_PARAMS_CNT];
					for (int k = 0; k < FORTUNE_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::FORTUNE_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_CONSOLIDATE:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = CONSOLIDATE_TYPE;
					itentry->value.params.count = CONSOLIDATE_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[CONSOLIDATE_PARAMS_CNT];
					for (int k = 0; k < CONSOLIDATE_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::CONSOLIDATE_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_COMPARESCROLLS:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = COMPARESCROLLS_TYPE;
					itentry->value.params.count = COMPARESCROLLS_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[COMPARESCROLLS_PARAMS_CNT];
					for (int k = 0; k < COMPARESCROLLS_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::COMPARESCROLLS_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_FILAMENT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = FILAMENT_TYPE;
					itentry->value.params.count = FILAMENT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[FILAMENT_PARAMS_CNT];
					for (int k = 0; k < FILAMENT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::FILAMENT_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_TRANSMUTE:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = TRANSMUTE_TYPE;
					itentry->value.params.count = TRANSMUTE_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[TRANSMUTE_PARAMS_CNT];
					for (int k = 0; k < TRANSMUTE_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::TRANSMUTE_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_OBLIVION:
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = OBLIVION_TYPE;
					itentry->value.params.count = OBLIVION_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[OBLIVION_PARAMS_CNT];
					for (int k = 0; k < OBLIVION_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::OBLIVION_PARAMS[k];
					break;
				case IT::STDFNC::F_FOLK:
					itentry->idtype = IT::IDTYPE::F;
					break;
				}
			}
			else if (isParam)
				itentry->idtype = IT::IDTYPE::P;
			else
				itentry->idtype = IT::IDTYPE::V;

			strncpy_s(itentry->id, id, SCOPED_ID_MAXSIZE);
		}

		int i = tables.lextable.size;


		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 1].lexema !=LEX_ID_TYPE && tables.lextable.table[i - 1].lexema != LEX_ELDER && tables.lextable.table[i - 1].lexema != LEX_ACTION)
		{
			Log::writeError(log.stream, Error::GetError(300, line, 0));
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 1].lexema != LEX_ID_TYPE && tables.lextable.table[i - 1].lexema != LEX_ACTION)
		{
			Log::writeError(log.stream, Error::GetError(303, line, 0));
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_ELDER && tables.lextable.table[i - 1].lexema != LEX_ACTION)
		{
			Log::writeError(log.stream, Error::GetError(304, line, 0));
			lex_ok = false;
		}
		if (itentry->idtype == IT::IDTYPE::F)
		{
			if (i > 0 && tables.lextable.table[i - 1].lexema != LEX_ACTION)
			{
				Log::writeError(log.stream, Error::GetError(323, line, 0));
				lex_ok = false;
			}
			else if (i < 2 || (tables.lextable.table[i - 2].lexema != LEX_ID_TYPE && tables.lextable.table[i - 2].lexema != LEX_HOLLOW))
			{
				Log::writeError(log.stream, Error::GetError(303, line, 0));
				lex_ok = false;
			}
		}
		if (itentry->iddatatype == IT::IDDATATYPE::INDIGENT && tables.lextable.table[i - 1].lexema != LEX_ACTION)
		{
			Log::writeError(log.stream, Error::GetError(300, line, 0));
			lex_ok = false;
		}
		return itentry;
	}

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm)
	{
		static bool lex_ok = true;
		tables.lextable = LT::Create(MAXSIZE_LT);
		tables.idtable = IT::Create(MAXSIZE_TI);

		bool isParam = false, isFunc = false;
		int enterPoint = NULL;
		char curword[STR_MAXSIZE], nextword[STR_MAXSIZE];
		int curline;
		std::stack <char*> scopes;
		std::stack <bool> scopePushed;

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;
			isFunc = false;
			int idxTI = NULLIDX_TI;

			std::cout << "[LEXDBG_TOKEN] word=" << curword << " line=" << curline << std::endl;

			if (!strcmp(curword, "bitand") || !strcmp(curword, "bitor") || !strcmp(curword, "bitnot"))
			{
				char lexema = LEX_BITAND;
				if (!strcmp(curword, "bitor")) lexema = LEX_BITOR;
				if (!strcmp(curword, "bitnot")) lexema = LEX_BITNOT;
				std::cout << "[LEXDBG] direct keyword match: " << curword << " -> " << lexema << " line=" << curline << std::endl;
				LT::Entry* ltentry = new LT::Entry(lexema, curline, NULLIDX_TI);
				LT::Add(tables.lextable, *ltentry);
				continue;
			}

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);
				if (FST::execute(fst))
				{
					char lexema = graphs[j].lexema;
					switch (lexema)
					{
					case LEX_TEMPLE:
						enterPoint++;
						break;
					case LEX_SEPARATORS:
					{
						switch (*curword)
						{
						case LEX_LEFTHESIS:
						{
							isParam = true;
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))
							{
								char* functionname = new char[MAXSIZE_ID];
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
								if (scopename == nullptr)
									break;
								strcpy_s(functionname, MAXSIZE_ID, scopename);
								scopes.push(functionname);
							}
							break;
						}
						case LEX_RIGHTTHESIS:
						{
							isParam = false;
							if (*in.words[i - 1].word == LEX_LEFTHESIS || (i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE)))
								scopes.pop();
							break;

						}
						case LEX_LEFTBRACE:
						{
							bool shouldPushScope = true;

							// 1. Простые операторы: charge, backup, patrol
							if (i > 0 && (strcmp(in.words[i - 1].word, "backup") == 0 ||
								strcmp(in.words[i - 1].word, "charge") == 0 ||
								strcmp(in.words[i - 1].word, "patrol") == 0))
							{
								shouldPushScope = false;
							}
							// 2. Оператор path
							else if (i > 1 && strcmp(in.words[i - 2].word, "path") == 0)
							{
								shouldPushScope = false;
							}
							// 3. Оператор council
							else if (i > 0 && strcmp(in.words[i - 1].word, ")") == 0)
							{
								int k = i - 1;
								int balance = 0;
								bool isCouncil = false;
								while (k >= 0)
								{
									if (strcmp(in.words[k].word, ")") == 0) balance++;
									else if (strcmp(in.words[k].word, "(") == 0) balance--;
									if (balance == 0)
									{
										if (k > 0 && strcmp(in.words[k - 1].word, "council") == 0) isCouncil = true;
										break;
									}
									k--;
								}
								if (isCouncil) shouldPushScope = false;
							}

							// Записываем решение в стек
							scopePushed.push(shouldPushScope);

							if (shouldPushScope)
							{
								char* functionname = new char[MAXSIZE_ID];
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
								if (scopename != nullptr) {
									strcpy_s(functionname, MAXSIZE_ID, scopename);
									scopes.push(functionname);
								}
							}
							break;
						}
						case LEX_BRACELET:
						{
							if (!scopePushed.empty())
							{
								bool wasPushed = scopePushed.top();
								scopePushed.pop();

								if (wasPushed)
								{
									if (!scopes.empty())
									{
										delete[] scopes.top(); // Очистка памяти
										scopes.pop();
									}
								}
							}
							break;
						}
						}
						lexema = *curword;
						break;
					}
					case LEX_LITERAL_OCT:
					{
						int value;
						value = DecimicalNotation(curword, 8);

						tables.idtable.table[tables.idtable.size - 1].value.vint = value;
						bool isNegative = value < 0;
						if (isNegative) {
							value = -value;
						}
						int x = 0;
						char* str = (char*)malloc(12 * sizeof(char));
						while (value > 9) {
							str[x++] = (value % 10) + '0';
							value = value / 10;
						}
						str[x++] = value + '0';
						if (isNegative) {
							str[x++] = '-';
						}
						str[x] = '\0';
						char t;
						for (int u = 0; u < x / 2; u++) {
							t = str[u];
							str[u] = str[x - 1 - u];
							str[x - 1 - u] = t;
						}
						strcpy_s(curword, "");
						strcat(curword, str);
						free(str);
						lexema = LEX_LITERAL;
					}

					case LEX_ID:
					{
						char* str = new char[256];
						str = in.words[i].word;
						if (strlen(str) > MAXSIZE_ID) {
							Log::writeError(log.stream, Error::GetError(204, curline, 0));
							lex_ok = false;
							break;
						}
					}

					case LEX_LITERAL:
					{
						char id[STR_MAXSIZE] = "";
						idxTI = NULLDX_TI;
						if (*nextword == LEX_LEFTHESIS)
							isFunc = true;

						bool isKnownGlobalFunc = false;
						int globalSearchIdx = IT::isId(tables.idtable, curword);
						if (globalSearchIdx != NULLIDX_TI)
						{
							if (tables.idtable.table[globalSearchIdx].idtype == IT::IDTYPE::F ||
								tables.idtable.table[globalSearchIdx].idtype == IT::IDTYPE::S)
							{
								isKnownGlobalFunc = true;
							}
						}

						char* idtype = (isFunc && i > 1) ?
							in.words[i - 2].word : in.words[i - 1].word;
						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), MAXSIZE_ID);
						strncat(id, curword, MAXSIZE_ID);
						if (isLiteral(curword))
							strcpy_s(id, curword);

						IT::Entry* itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);
						if (itentry != nullptr)
						{
							if (isFunc)
							{
								if (getStandFunction(id) == IT::STDFNC::F_FOLK)
								{
									itentry->value.params.count = NULL;
									itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
									for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++)
									{
										if (k == In::InWord::size || in.words[k].word[0] == LEX_SEPARATOR)
											break;
										if (ISTYPE(in.words[k].word))
										{
											if (itentry->value.params.count >= MAX_PARAMS_COUNT)
											{
												Log::writeError(log.stream, Error::GetError(306, curline, 0));
												lex_ok = false;
												break;
											}
											itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
										}
									}
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else
						{
							int i = tables.lextable.size - 1;
							if (i > 0 &&  tables.lextable.table[i].lexema == LEX_ELDER
								 || tables.lextable.table[i].lexema == LEX_ACTION
								 || tables.lextable.table[i].lexema == LEX_ID_TYPE
								 || tables.lextable.table[i].lexema == LEX_HOLLOW) //вот тут проверка на 305 tables.lextable.table[i - 1].lexema == LEX_ELDER || tables.lextable.table[i - 1].lexema == LEX_ACTION || tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i - 1].lexema == LEX_HOLLOW
							{
								Log::writeError(log.stream, Error::GetError(305, curline, 0));
								std::cout << "[LEXDBG] duplicate id near declaration, word=" << curword << " line=" << curline << std::endl;
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);
							if (lexema == LEX_LITERAL)
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word), log, curline);
						}
					}
					break;
					}

					LT::Entry* ltentry = new LT::Entry(lexema, curline, idxTI);

					if (lexema == LEX_BITAND || lexema == LEX_BITOR || lexema == LEX_BITNOT)
						std::cout << "[LEXDBG] op=" << curword << " lex=" << lexema << " line=" << curline << " idxTI=" << idxTI << std::endl;
					if (lexema == LEX_ID && (strcmp(curword, "bitand") == 0 || strcmp(curword, "bitor") == 0 || strcmp(curword, "bitnot") == 0))
						std::cout << "[LEXDBG] keyword matched as ID: " << curword << " line=" << curline << " idxTI=" << idxTI << std::endl;

					LT::Add(tables.lextable, *ltentry);
					break;
				}
				else if (j == N_GRAPHS - 1)
				{
					Log::writeError(log.stream, Error::GetError(201, curline, 0));
					lex_ok = false;
				}
			}
		}

		if (enterPoint == NULL)
		{
			Log::writeError(log.stream, Error::GetError(301));
			lex_ok = false;
		}
		if (enterPoint > 1)
		{
			Log::writeError(log.stream, Error::GetError(302));
			lex_ok = false;
		}
		return lex_ok;
	}
};