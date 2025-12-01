#include "LexAnalysis.h"
#include "Graphs.h"
#include "Error.h"
#include "Log.h"
#include <stack>
#include <string>
#include <iostream>

#pragma warning(disable : 4996)

namespace Lexer
{
	// !!!      :                                            N_GRAPHS (32) !!!
	//                                LT.h (LEX_TEMPLE, LEX_ELDER, LEX_NEWLINE...)
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS,   FST::FST(GRAPH_SEPARATORS) },   // 1
		{ LEX_LITERAL,      FST::FST(GRAPH_INT_LITERAL) },  // 2
		{ LEX_LITERAL,      FST::FST(GRAPH_OCTAL_LIT) },    // 3
		{ LEX_LITERAL,      FST::FST(GRAPH_STRING_LITERAL) },// 4
		{ LEX_LITERAL,      FST::FST(GRAPH_CHAR_LITERAL) }, // 5

		{ LEX_ELDER,        FST::FST(GRAPH_ELDER) },        // 6
		{ LEX_TEMPLE,       FST::FST(GRAPH_TEMPLE) },       // 7
		{ LEX_ACTION,       FST::FST(GRAPH_ACTION) },       // 8

		//            
		{ LEX_INTSTRBOOLCHAR, FST::FST(GRAPH_SQUIRE) }, // 9
		{ LEX_INTSTRBOOLCHAR, FST::FST(GRAPH_SCROLL) }, // 10
		{ LEX_INTSTRBOOLCHAR, FST::FST(GRAPH_RUNE) },   // 11
		{ LEX_INTSTRBOOLCHAR, FST::FST(GRAPH_HONOR) },  // 12
		{ LEX_HOLLOW, FST::FST(GRAPH_HOLLOW) }, // 13

		//                    
		{ LEX_LITERAL,        FST::FST(GRAPH_FAITH) },        // 14
		{ LEX_LITERAL,        FST::FST(GRAPH_EXILE) },        // 15

		//          
		{ LEX_COMEBACK,     FST::FST(GRAPH_COMEBACK) },     // 16
		{ LEX_CHARGE,       FST::FST(GRAPH_CHARGE) },       // 17
		{ LEX_BACKUP,       FST::FST(GRAPH_BACKUP) },       // 18
		{ LEX_PATROL,       FST::FST(GRAPH_PATROL) },       // 19
		{ LEX_COUNCIL,      FST::FST(GRAPH_COUNCIL) },      // 20
		{ LEX_PATH,         FST::FST(GRAPH_PATH) },         // 21
		{ LEX_TIRESOME,     FST::FST(GRAPH_TIRESOME) },     // 22
		{ LEX_BIND,         FST::FST(GRAPH_BIND) },         // 23
		{ LEX_ALLY,         FST::FST(GRAPH_ALLY) },         // 24
		{ LEX_BANISH,		FST::FST(GRAPN_BANISH)},		//25

		//                    
		{ LEX_COMPSCROLLS,  FST::FST(GRAPH_COMPSCROLL) },   // 26
		{ LEX_CONSOLIDATE,  FST::FST(GRAPH_CONSOLIDATE) },  // 27
		{ LEX_MIGHTINESS,   FST::FST(GRAPH_MIGHTINESS) },   // 28
		{ LEX_FILAMENT,     FST::FST(GRAPH_FILAMENT) },     // 29

		//               
		{ LEX_PRINT,      FST::FST(GRAPH_CONF) },  // 30
		
		// newleaf для перевода строки
		{ LEX_NEWLINE,    FST::FST(GRAPH_NEWLEAF) },  // 31

		//                (                ,                                      )
		{ LEX_ID,           FST::FST(GRAPH_ID) }            // 32
	};

	// ---                         ---

	char* getScopeName(IT::IdTable idtable, char* prevword)
	{
		if (strcmp(prevword, KW_TEMPLE) == 0)
		{
			char* a = new char[7];
			strcpy_s(a, 7, KW_TEMPLE);
			return a;
		}
		for (int i = idtable.size - 1; i >= 0; i--)
			if (idtable.table[i].idtype == IT::IDTYPE::F)
				return idtable.table[i].id;

		return nullptr;
	}

	int DecimicalNotation(std::string input, int scaleofnot) {
		// 1.                                                 
		scaleofnot = 8;

		// 2.           ,                                
		bool isNegative = !input.empty() && input[0] == '-';

		// 3.                                         ,                              
		size_t startIdx = 0;

		//                ,                                   
		if (isNegative) {
			if (input.size() >= 2 && (input[1] == 'o' || input[1] == 'O')) {
				startIdx = 2; //            '-'   'o'/'O'
			}
			else {
				//                   ,        'o',             ,                                    ,
				//                                                         '-'
				startIdx = 1;
			}
		}
		else {
			//                ,                             
			if (input.size() >= 1 && (input[0] == 'o' || input[0] == 'O')) {
				startIdx = 1; //            'o'/'O'
			}
		}

		//                             , std::stoi                   ,                          .

		try {
			//                      ,                        ,   int              8.
			int value = std::stoi(input.substr(startIdx), nullptr, scaleofnot);

			//                     ,                    ,            .
			return isNegative ? -value : value;
		}
		catch (const std::out_of_range&) {
			//                        (                       int, TI_INT_MINSIZE/MAXSIZE)
			return isNegative ? TI_INT_MINSIZE : TI_INT_MAXSIZE;
		}
		catch (const std::invalid_argument&) {
			//          ,                           (        ,      "-o")
			return 0;
		}
	}

	int getLiteralIndex(IT::IdTable idtable, char* value, IT::IDDATATYPE type)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::L && idtable.table[i].iddatatype == type)
			{
				if (type == IT::IDDATATYPE::INT) {
					if (atoi(value) == idtable.table[i].value.vint) return i;
				}
				else if (type == IT::IDDATATYPE::BOOL) {
					// Для типа BOOL сравниваем значения (exile=1, faith=0)
					int valueInt;
					if (!strcmp(value, "exile")) {
						valueInt = 1;
					}
					else if (!strcmp(value, "faith")) {
						valueInt = 0;
					}
					else {
						valueInt = atoi(value);
					}
					if (valueInt == idtable.table[i].value.vint) return i;
				}
			}
		}
		return TI_NULLIDX;
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == LEX_MINUS || *id == '\'' || *id == '\"' || (*id == 'o' && isdigit(*(id + 1))))
			return true;
		// Проверяем литералы типа honor (bool)
		if (!strcmp(id, LITERAL_EXILE) || !strcmp(id, LITERAL_FAITH))
			return true;
		return false;
	}

	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (idtype) {
			if (!strcmp(TYPE_HOLLOW, idtype)) return IT::IDDATATYPE::VOID;
			if (!strcmp(TYPE_SQUIRE, idtype)) return IT::IDDATATYPE::INT;
			if (!strcmp(TYPE_SCROLL, idtype)) return IT::IDDATATYPE::STR;
			if (!strcmp(TYPE_RUNE, idtype))   return IT::IDDATATYPE::CHAR;
			if (!strcmp(TYPE_HONOR, idtype))  return IT::IDDATATYPE::BOOL;
		}

		if (curword) {
			if (curword[0] == '\"') return IT::IDDATATYPE::STR;
			if (curword[0] == '\'') return IT::IDDATATYPE::CHAR;
			if (isdigit(curword[0]) || curword[0] == LEX_MINUS || (curword[0] == 'o' && isdigit(curword[1])))
				return IT::IDDATATYPE::INT;
			// Проверяем литералы типа honor (bool)
			if (!strcmp(curword, LITERAL_EXILE) || !strcmp(curword, LITERAL_FAITH))
				return IT::IDDATATYPE::BOOL;
		}

		return IT::IDDATATYPE::UNDEF;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(FN_MIGHTINESS, id))		return IT::STDFNC::F_MIGHT;
		if (!strcmp(FN_COMPARE_SCROLLS, id))return IT::STDFNC::F_COMPSCR;
		if (!strcmp(FN_CONSOLIDATE, id))	return IT::STDFNC::F_CONS;
		if (!strcmp(FN_FILAMENT, id))		return IT::STDFNC::F_FILAM;
		if (!strcmp(FN_CONF_SCROLL, id))	return IT::STDFNC::F_CONFSC;
		if (!strcmp(FN_CONF_SQUIRE, id))	return IT::STDFNC::F_CONFSQHO;
		if (!strcmp(FN_CONF_RUNE, id))		return IT::STDFNC::F_CONFRU;

		return IT::STDFNC::F_NOT_STD;
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)
	{
		if (itTableIndex == TI_NULLIDX) return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return TI_NULLIDX;
	}

	char* getNextLiteralName()
	{
		static int literaldigit = 1;
		char* buf = new char[ID_MAXSIZE];
		char lich[10];
		strcpy_s(buf, ID_MAXSIZE, "LTRL");
		_itoa_s(literaldigit++, lich, 10);
		strcat_s(buf, ID_MAXSIZE, lich);
		return buf;
	}

	// Поиск переменной по локальному имени во всех scope
	// Ищет переменную, которая заканчивается на ".localName" или равна "localName"
	// Возвращает индекс последней найденной переменной (самой недавно объявленной)
	// ВАЖНО: ищет переменные во всех scope, чтобы найти правильную переменную
	int findVariableByLocalName(IT::IdTable& idtable, char* localName)
	{
		if (!localName || strlen(localName) == 0) return TI_NULLIDX;

		// Сначала проверяем точное совпадение (для глобальных переменных)
		int idx = IT::isId(idtable, localName);
		if (idx != TI_NULLIDX) return idx;

		// Затем ищем переменные, которые заканчиваются на ".localName"
		// Ищем с конца таблицы, чтобы найти самую недавно объявленную переменную
		// Это важно для правильного разрешения scope - более глубокий scope имеет приоритет
		char searchPattern[SCOPED_ID_MAXSIZE];
		sprintf_s(searchPattern, SCOPED_ID_MAXSIZE, ".%s", localName);
		size_t patternLen = strlen(searchPattern);

		// Ищем с конца таблицы - это гарантирует, что найдем самую недавно объявленную переменную
		// в самом глубоком доступном scope
		for (int i = idtable.size - 1; i >= 0; i--)
		{
			if (idtable.table[i].idtype == IT::IDTYPE::V || idtable.table[i].idtype == IT::IDTYPE::P)
			{
				const char* id = idtable.table[i].id;
				size_t idLen = strlen(id);
				
				// Проверяем точное совпадение
				if (strcmp(id, localName) == 0)
				{
					return i;
				}
				
				// Проверяем, заканчивается ли имя на ".localName"
				if (idLen >= patternLen)
				{
					if (strcmp(id + idLen - patternLen, searchPattern) == 0)
					{
						return i;
					}
				}
			}
		}

		return TI_NULLIDX;
	}

	// --- ENTRY CREATION ---
	IT::Entry* getEntry(
		Lexer::LEX& tables, char lex, char* id, char* idtype,
		bool isParam, bool isFunc, Log::LOG log, int line, bool& lex_ok)
	{
		IT::IDDATATYPE type = getType(id, idtype);
		int index = TI_NULLIDX;

		//                          
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);
		else
			index = IT::isId(tables.idtable, id);

		if (index != TI_NULLIDX) return nullptr; //         

		IT::Entry* itentry = new IT::Entry;
		itentry->iddatatype = type;
		itentry->idxfirstLE = tables.lextable.size;

		if (lex == LEX_LITERAL)
		{
			//        
			bool set_ok = IT::SetValue(itentry, id);
			if (!set_ok) {
				Log::writeError(log.stream, Error::GetError(313, line, 0));
				lex_ok = false;
			}
			if (type == IT::IDDATATYPE::CHAR && itentry->value.vstr.len != 1) {
				Log::writeError(log.stream, Error::GetError(319, line, 0));
				lex_ok = false;
			}

			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else
		{
			//              
			if (isFunc)
			{
				itentry->idtype = IT::IDTYPE::F;
				IT::STDFNC stdfunc = getStandFunction(id);

				if (stdfunc != IT::STDFNC::F_NOT_STD)
				{
					itentry->idtype = IT::IDTYPE::S;
					switch (stdfunc)
					{
					case IT::STDFNC::F_MIGHT:
						itentry->iddatatype = MIGHTINESS_TYPE;
						itentry->value.params.count = MIGHTINESS_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[MIGHTINESS_PARAMS_CNT];
						itentry->value.params.names = new char* [MIGHTINESS_PARAMS_CNT];
						for (int k = 0; k < MIGHTINESS_PARAMS_CNT; ++k) {
							itentry->value.params.types[k] = IT::MIGHTINESS_PARAMS[k];
							itentry->value.params.names[k] = nullptr;
						}
					break;
					case IT::STDFNC::F_COMPSCR:
						itentry->iddatatype = COMP_SCROL_TYPE;
						itentry->value.params.count = COMP_SCROL_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[COMP_SCROL_PARAMS_CNT];
						itentry->value.params.names = new char* [COMP_SCROL_PARAMS_CNT];
						for (int k = 0; k < COMP_SCROL_PARAMS_CNT; ++k) {
							itentry->value.params.types[k] = IT::COMP_SCROL_PARAMS[k];
							itentry->value.params.names[k] = nullptr;
						}
					break;
					case IT::STDFNC::F_CONS:
						itentry->iddatatype = CONSOLIDATE_TYPE;
						itentry->value.params.count = CONSOLIDATE_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[CONSOLIDATE_PARAMS_CNT];
						itentry->value.params.names = new char* [CONSOLIDATE_PARAMS_CNT];
						for (int k = 0; k < CONSOLIDATE_PARAMS_CNT; ++k) {
							itentry->value.params.types[k] = IT::CONSOLIDATE_PARAMS[k];
							itentry->value.params.names[k] = nullptr;
						}
					break;
					case IT::STDFNC::F_FILAM:
						itentry->iddatatype = FILAMENT_TYPE;
						itentry->value.params.count = FILAMENT_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[FILAMENT_PARAMS_CNT];
						itentry->value.params.names = new char* [FILAMENT_PARAMS_CNT];
						for (int k = 0; k < FILAMENT_PARAMS_CNT; ++k) {
							itentry->value.params.types[k] = IT::FILAMENT_PARAMS[k];
							itentry->value.params.names[k] = nullptr;
						}
						break;
					case IT::STDFNC::F_CONFSC:
						itentry->iddatatype = CONF_SCROLL_TYPE;
						itentry->value.params.count = CONF_SCROLL_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[CONF_SCROLL_PARAMS_CNT];
						itentry->value.params.names = new char* [CONF_SCROLL_PARAMS_CNT];
						itentry->value.params.types[0] = IT::CONF_SCROLL_PARAMS[0];
						itentry->value.params.names[0] = nullptr;
						break;
					case IT::STDFNC::F_CONFSQHO:
						itentry->iddatatype = CONF_SQUIREHON_TYPE;
						itentry->value.params.count = CONF_SQUIREHON_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[CONF_SQUIREHON_PARAMS_CNT];
						itentry->value.params.names = new char* [CONF_SQUIREHON_PARAMS_CNT];
						itentry->value.params.types[0] = IT::CONF_SQUIREHON_PARAMS[0];
						itentry->value.params.names[0] = nullptr;
						break;
					case IT::STDFNC::F_CONFRU:
						itentry->iddatatype = CONF_RUNE_TYPE;
						itentry->value.params.count = CONF_RUNE_PARAMS_CNT;
						itentry->value.params.types = new IT::IDDATATYPE[CONF_RUNE_PARAMS_CNT];
						itentry->value.params.names = new char* [CONF_RUNE_PARAMS_CNT];
						itentry->value.params.types[0] = IT::CONF_RUNE_PARAMS[0];
						itentry->value.params.names[0] = nullptr;
					break;
				}
				}
				else
				{
					itentry->value.params.count = 0;
					itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
					itentry->value.params.names = new char* [MAX_PARAMS_COUNT];
					for (int k = 0; k < MAX_PARAMS_COUNT; k++) {
						itentry->value.params.names[k] = nullptr;
					}
				}
			}
			else
			{
				// Параметр определяется только если isParam == true
				// Но это должно быть проверено в вызывающем коде
				itentry->idtype = isParam ? IT::IDTYPE::P : IT::IDTYPE::V;
			}
			// Копируем id в поле структуры, используя правильный размер буфера
			strncpy_s(itentry->id, SCOPED_ID_MAXSIZE, id, _TRUNCATE);
		}
		return itentry;
	}

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm)
	{
		bool lex_ok = true;
		try {
			tables.lextable = LT::Create(LT_MAXSIZE);
		}
		catch (Error::ERROR error) {
			Log::writeError(log.stream, Error::GetError(202, 0, 0));
			lex_ok = false;
			return lex_ok;
		}
		try {
			tables.idtable = IT::Create(TI_MAXSIZE);
		}
		catch (Error::ERROR error) {
			Log::writeError(log.stream, Error::GetError(203, 0, 0));
			lex_ok = false;
			return lex_ok;
		}

		bool isParam = false;
		bool isFunc = false;
		int enterPoint = 0;
		int currentFuncIndex = TI_NULLIDX; // Индекс текущей функции, для которой обрабатываются параметры

		char curword[TI_STR_MAXSIZE];
		char nextword[TI_STR_MAXSIZE];
		int curline;
		std::stack<char*> scopes;
		int braceDepth = 0; // Счетчик глубины вложенности ВСЕХ блоков (включая вложенные)
		int scopeDepth = 0; // Счетчик глубины блоков, которые создали scope (только temple и функции)

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1) strcpy_s(nextword, in.words[i + 1].word);

			curline = in.words[i].line;
			isFunc = false;
			int idxTI = TI_NULLIDX;
			bool recognized = false;

			// ---                          ---
			std::cout << "DEBUG:                 : [" << curword << "]" << std::endl;

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);
				if (FST::execute(fst))
				{
					recognized = true;
					char lexema = graphs[j].lexema;

					if (lexema == LEX_TEMPLE) enterPoint++;

					if (lexema == LEX_SEPARATORS || lexema == LEX_LEFTBRAC || lexema == LEX_RIGHTBRAC || lexema == LEX_LEFTHESIS || lexema == LEX_RIGHTTHESIS)
					{
						switch (curword[0])
						{
						case LEX_LEFTHESIS:
							// КРИТИЧЕСКИ ВАЖНО: [ может быть для параметров функции ИЛИ для условий циклов
							// Проверяем: если перед [ стоит имя функции (ID с типом F), то это параметры
							// Иначе (если patrol, charge, council), то это условие - НЕ параметры!
							isParam = false;
							currentFuncIndex = TI_NULLIDX;
							
							// Сначала проверяем, что перед [ стоит ключевое слово циклов
							if (i > 0) {
								char* prevWord = in.words[i - 1].word;
								// Если перед [ стоит ключевое слово patrol, charge, council - это условие, НЕ параметры
								if (strcmp(prevWord, "patrol") == 0 || 
									strcmp(prevWord, "charge") == 0 || 
									strcmp(prevWord, "council") == 0) {
									isParam = false; // Условие цикла - НЕ параметры
									currentFuncIndex = TI_NULLIDX;
								}
								// Иначе проверяем, что перед [ стоит функция
								else if (tables.idtable.size > 0) {
									// Проверяем, что предыдущее слово было ID и это функция
									// Ищем последнюю функцию в таблице (она должна быть создана перед [)
									for (int k = tables.idtable.size - 1; k >= 0; k--) {
										if (tables.idtable.table[k].idtype == IT::IDTYPE::F) {
											// Проверяем, совпадает ли имя функции с предыдущим словом
											if (strcmp(tables.idtable.table[k].id, prevWord) == 0) {
												// Нашли функцию - значит это параметры функции
							isParam = true;
												currentFuncIndex = k;
									break;
											}
										}
									}
								}
							}
							break;
							// LEX_RIGHTTHESIS
						case LEX_RIGHTTHESIS:
							// КРИТИЧЕСКИ ВАЖНО: сбрасываем флаги ДО обработки следующих токенов
							isParam = false;
							currentFuncIndex = TI_NULLIDX; // Сбрасываем индекс функции после закрытия списка параметров
							// Важно: сбрасываем сразу, чтобы параметры не добавлялись после закрытия скобки
							break;
						case LEX_LEFTBRAC: // '('
						case LEX_RIGHTBRAC: // ')'
							break;

						case LEX_LEFTBRACE: // '{'
						{
							// ВАЖНО: scope добавляется ТОЛЬКО для:
							// 1. Блока temple: если перед { стоит "temple"
							// 2. Блока функции: если последняя запись в таблице - функция (F)
							// Вложенные блоки (charge { }, patrol { }, backup { }) НЕ создают новый scope!
							// Они используют текущий scope из стека
							bool scopeAdded = false;
							if (i > 0 && strcmp(in.words[i - 1].word, KW_TEMPLE) == 0) {
								// Блок temple - добавляем scope "temple"
								char* t = new char[7];
								strcpy_s(t, 7, KW_TEMPLE);
								scopes.push(t);
								scopeAdded = true;
							}
							// Проверяем, что перед { стоит функция
							// ВАЖНО: функция может быть не последней записью, если после нее были созданы параметры!
							// Ищем последнюю функцию в таблице
							else if (tables.idtable.size > 0)
							{
								// Ищем последнюю функцию в таблице (она должна быть создана перед [)
								int lastFuncIdx = TI_NULLIDX;
								for (int k = tables.idtable.size - 1; k >= 0; k--) {
									if (tables.idtable.table[k].idtype == IT::IDTYPE::F) {
										lastFuncIdx = k;
								break;
									}
								}
								
								// Если нашли функцию, добавляем её имя в scope
								if (lastFuncIdx != TI_NULLIDX) {
									char* fName = new char[ID_MAXSIZE];
									strcpy_s(fName, ID_MAXSIZE, tables.idtable.table[lastFuncIdx].id);
									scopes.push(fName);
									scopeAdded = true;
								}
							}
							// Для вложенных блоков (charge { }, patrol { }, backup { }, council { })
							// НЕ добавляем новый scope - они используют текущий scope из стека
							// Увеличиваем счетчики глубины
							braceDepth++; // Увеличиваем для ВСЕХ блоков
							if (scopeAdded) {
								scopeDepth++; // Увеличиваем только для блоков, которые создали scope
							}
							break;
						}

						case LEX_BRACELET: // '}'
							// ВАЖНО: удаляем scope ТОЛЬКО если это был блок функции или temple
							// Вложенные блоки не создают scope, поэтому и не удаляют
							// Используем отдельные счетчики для отслеживания
							if (braceDepth > 0) {
								braceDepth--; // Уменьшаем для ВСЕХ блоков
								// Удаляем scope только если это был блок, который создал scope
								if (scopeDepth > 0 && !scopes.empty()) {
									delete[] scopes.top();
									scopes.pop();
									scopeDepth--;
								}
							}
							break;
						}
						lexema = curword[0];
					}

					if (lexema == LEX_ID || lexema == LEX_LITERAL)
					{
						char fullId[SCOPED_ID_MAXSIZE] = "";
						char* typeKeywords = nullptr;

						// Проверка длины идентификатора (ошибка 204)
						if (lexema == LEX_ID && strlen(curword) > ID_MAXSIZE) {
							Log::writeError(log.stream, Error::GetError(204, curline, 0));
							lex_ok = false;
							// Продолжаем обработку, но обрезаем идентификатор
							curword[ID_MAXSIZE] = '\0';
						}

						if (nextword[0] == LEX_LEFTHESIS || nextword[0] == LEX_LEFTBRAC) isFunc = true;
						if (i > 0) typeKeywords = in.words[i - 1].word;

						// КРИТИЧЕСКИ ВАЖНО: параметр может быть ТОЛЬКО если:
						// 1. isParam == true (мы внутри [ и ])
						// 2. currentFuncIndex != TI_NULLIDX (есть функция для параметров)
						// 3. предыдущее слово - это тип данных (squire, scroll, rune, honor, hollow)
						// 4. это НЕ функция
						bool isActuallyParam = false;
						if (isParam && currentFuncIndex != TI_NULLIDX && !isFunc && i > 0 && typeKeywords) {
							if (ISTYPE(typeKeywords)) {
								isActuallyParam = true;
							}
						}

						// Проверяем, существует ли идентификатор
						// ВАЖНО: сначала формируем fullId, потом ищем по нему
						int existingIdx = TI_NULLIDX;
						if (lexema == LEX_LITERAL) {
							// Для литералов формируем fullId
							strcpy_s(fullId, curword);
							existingIdx = getLiteralIndex(tables.idtable, fullId, getType(fullId, typeKeywords));
						}
						else {
							// Для идентификаторов: формируем fullId с учетом scope
							// ВАЖНО: 
							// 1. Функции НЕ должны получать scope префикс (они глобальные)
							// 2. Параметры функций НЕ должны получать scope префикс
							// 3. ВСЕ переменные внутри блоков (temple, функции) ДОЛЖНЫ получать scope префикс
							// 4. При использовании переменных (не объявлении) они тоже должны получать префикс
							if (isFunc) {
								// Функции создаются без префикса scope (глобальные)
								strcpy_s(fullId, SCOPED_ID_MAXSIZE, curword);
							}
							else if (isActuallyParam) {
								// Параметры функций не получают scope префикс
								strcpy_s(fullId, SCOPED_ID_MAXSIZE, curword);
							}
							else if (!scopes.empty()) {
								// ВСЕ остальные переменные в блоках получают scope префикс
								// Это включает:
								// - Объявления переменных (elder squire x)
								// - Использование переменных (x = 10, confession x)
								// - Переменные в условиях циклов (patrol [x < y])
								strcpy_s(fullId, SCOPED_ID_MAXSIZE, scopes.top());
								strcat_s(fullId, SCOPED_ID_MAXSIZE, ".");
								strcat_s(fullId, SCOPED_ID_MAXSIZE, curword);
							}
							else {
								// Если нет scope, используем просто локальное имя (глобальные переменные)
								// Это может быть только для переменных вне блоков (что не должно происходить)
								strcpy_s(fullId, SCOPED_ID_MAXSIZE, curword);
							}
							
							// Теперь ищем существующие переменные:
							// 1. Сначала по полному имени (с scope, например "temple.x")
							existingIdx = IT::isId(tables.idtable, fullId);
							
							// 2. Если не нашли по полному имени, ищем по локальному имени
							// ВАЖНО: это нужно для использования переменных в условиях циклов!
							// Например, когда обрабатывается "patrol [x < y]", переменные x и y уже записаны
							// как "temple.x" и "temple.y", и мы должны их найти по локальному имени
							// НО: не ищем для параметров функций (они создаются без префикса)
							if (existingIdx == TI_NULLIDX && !isLiteral(curword) && !isFunc && !isActuallyParam) {
								existingIdx = findVariableByLocalName(tables.idtable, curword);
								if (existingIdx != TI_NULLIDX) {
									// Нашли существующую переменную - используем её полное имя из таблицы
									strcpy_s(fullId, SCOPED_ID_MAXSIZE, tables.idtable.table[existingIdx].id);
								}
								else {
									// Если findVariableByLocalName не нашел, пробуем точное совпадение
									existingIdx = IT::isId(tables.idtable, curword);
									if (existingIdx != TI_NULLIDX) {
										strcpy_s(fullId, SCOPED_ID_MAXSIZE, tables.idtable.table[existingIdx].id);
									}
								}
							}
						}

						// Если идентификатор уже существует, обновляем его значение вместо создания новой записи
						if (existingIdx != TI_NULLIDX) {
							idxTI = existingIdx;
							IT::Entry* existingEntry = &tables.idtable.table[existingIdx];

							// Обновляем тип переменной, если он изменился или если новый тип не UNDEFINED
							if (existingEntry->idtype == IT::IDTYPE::V && !isFunc && !isParam) {
								IT::IDDATATYPE newType = getType(fullId, typeKeywords);
								// Обновляем тип только если:
								// 1. Новый тип не UNDEFINED (т.е. переменная объявляется с типом)
								// 2. И старый тип был UNDEFINED (т.е. переменная была использована без типа, а теперь объявляется с типом)
								if (newType != IT::IDDATATYPE::UNDEF &&
									existingEntry->iddatatype == IT::IDDATATYPE::UNDEF) {
									existingEntry->iddatatype = newType;
								}
								// Обновляем значение, если присваивается литерал
								if (lexema == LEX_LITERAL) {
									IT::SetValue(existingEntry, fullId);
								}
							}
							// Для литералов не обновляем - они должны быть уникальными

							// Параметры не добавляем для существующих идентификаторов
						}
						else {
							// Создаем новую запись
							// Используем isActuallyParam вместо isParam - это гарантирует, что только
							// идентификаторы после типа данных внутри [ ] будут параметрами
							IT::Entry* entry = getEntry(tables, lexema, fullId, typeKeywords, isActuallyParam, isFunc, log, curline, lex_ok);

							if (entry) {
								try {
									IT::Add(tables.idtable, *entry);
									idxTI = tables.idtable.size - 1;
								}
								catch (Error::ERROR error) {
									Log::writeError(log.stream, Error::GetError(203, curline, 0));
									lex_ok = false;
									idxTI = TI_NULLIDX;
								}

								// Если это параметр функции, добавляем его тип и имя в массив параметров функции
								// Параметры добавляются только если:
								// 1. isActuallyParam == true (это действительно параметр - после типа данных внутри [ ])
								// 2. entry->idtype == IT::IDTYPE::P (это действительно параметр, созданный в getEntry)
								// 3. currentFuncIndex != TI_NULLIDX (индекс функции установлен)
								// КРИТИЧЕСКАЯ ПРОВЕРКА: все три условия должны быть true одновременно
								// Если любое из условий false, параметры НЕ добавляются
								if (isActuallyParam && entry->idtype == IT::IDTYPE::P && currentFuncIndex != TI_NULLIDX) {
									// Используем currentFuncIndex - индекс функции, для которой открыт список параметров
									int funcIdx = currentFuncIndex;

									// Проверяем, что индекс функции валиден
									if (funcIdx >= 0 && funcIdx < tables.idtable.size) {
										IT::Entry* funcEntry = &tables.idtable.table[funcIdx];
										// Проверяем, что это действительно функция и массив параметров выделен
										// И что это именно та функция, для которой мы обрабатываем параметры
										if (funcEntry->idtype == IT::IDTYPE::F &&
											funcEntry->value.params.types != nullptr &&
											funcEntry->value.params.names != nullptr) {
											// Дополнительная проверка: убеждаемся, что мы все еще внутри списка параметров
											// (isParam должен быть true, что уже проверено выше)
											// Проверяем, не превышен ли лимит параметров
											if (funcEntry->value.params.count < MAX_PARAMS_COUNT) {
												// Добавляем тип параметра в массив параметров функции
												funcEntry->value.params.types[funcEntry->value.params.count] = entry->iddatatype;
												// Сохраняем имя параметра (используем curword - локальное имя без scope)
												char* paramName = new char[ID_MAXSIZE];
												strcpy_s(paramName, ID_MAXSIZE, curword);
												funcEntry->value.params.names[funcEntry->value.params.count] = paramName;
												funcEntry->value.params.count++;
											}
										}
									}
								}
								// Если isParam == false или currentFuncIndex == TI_NULLIDX, параметры НЕ добавляются
								// Если isParam == false или currentFuncIndex == TI_NULLIDX, параметры НЕ добавляются
							}
							else {
								idxTI = TI_NULLIDX;
							}
						}
					}

					LT::Entry ltEntry(lexema, curline, idxTI);
					try {
						LT::Add(tables.lextable, ltEntry);
					}
					catch (Error::ERROR error) {
						Log::writeError(log.stream, Error::GetError(202, curline, 0));
						lex_ok = false;
					}
					break;
				}
			}

			if (!recognized) {
					Log::writeError(log.stream, Error::GetError(201, curline, 0));
					lex_ok = false;
				}
			}

		LT::Entry endEntry('$', curline, TI_NULLIDX);
		LT::Add(tables.lextable, endEntry);

		//              
		while (!scopes.empty()) {
			delete[] scopes.top();
			scopes.pop();
		}

		if (enterPoint == 0) { Log::writeError(log.stream, Error::GetError(301)); lex_ok = false; }
		if (enterPoint > 1) { Log::writeError(log.stream, Error::GetError(302)); lex_ok = false; }

		return lex_ok;
	}
}
