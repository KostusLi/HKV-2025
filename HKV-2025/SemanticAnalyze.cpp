#include "IT.h"
#include "LT.h"
#include "Error.h"
#include "LexAnalysis.h"
#include "SemanticAnalyze.h"
#include <set>
#include <string> // Добавлено для формирования ключей set

namespace Semantic
{
	bool Semantic::semanticsCheck(Lexer::LEX& tables, Log::LOG& log)
	{
		bool sem_ok = true;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_ELDER:
			{
				if (tables.lextable.table[i + 1].lexema != LEX_ID_TYPE)
				{
					sem_ok = false;
					Log::writeError(log.stream, Error::GetError(303, tables.lextable.table[i].sn, 0));
				}
				break; // Добавлен break для безопасности
			}
			case LEX_DIRSLASH: // Деление
			case LEX_PERSENT:  // Остаток от деления (Добавил проверку на 0 для %)
			{
				// Проверка деления на ноль (литерал или переменную, если возможно отследить)
				int k = i;
				// Проверка: x / y (если y - переменная)
				if (tables.lextable.table[i + 1].lexema == LEX_ID)
				{
					// Простая эвристика: ищем объявление переменной выше по коду
					for (k; k > 0; k--)
					{
						if (tables.lextable.table[k].lexema == LEX_ID)
						{
							if (tables.idtable.table[tables.lextable.table[k].idxTI].id == tables.idtable.table[tables.lextable.table[i + 1].idxTI].id)
							{
								// Если нашли объявление и там инициализация нулем (elder squire y = 0;)
								if (tables.lextable.table[k + 2].lexema == LEX_LITERAL && tables.idtable.table[tables.lextable.table[k + 2].idxTI].value.vint == 0)
								{
									sem_ok = false;
									Log::writeError(log.stream, Error::GetError(318, tables.lextable.table[k].sn, 0));
								}
							}
						}
					}
				}
				// Проверка: x / 0 (литерал)
				if (tables.lextable.table[i + 1].lexema == LEX_LITERAL)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::SQUIRE &&
						tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint == 0)
					{
						sem_ok = false;
						Log::writeError(log.stream, Error::GetError(318, tables.lextable.table[i].sn, 0));
					}
				}
				break;
			}
			case LEX_EQUAL:
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != NULLIDX_TI)
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;
					bool ignore = false;

					for (int k = i + 1; tables.lextable.table[k].lexema != LEX_SEPARATOR; k++)
					{
						if (k == tables.lextable.size)
							break;
						if (tables.lextable.table[k].idxTI != NULLIDX_TI)
						{
							if (!ignore)
							{
								IT::IDDATATYPE righttype = tables.idtable.table[tables.lextable.table[k].idxTI].iddatatype;
								// Разрешаем присваивать результат функций, если типы совпадают (грубая проверка)
								if (righttype != IT::IDDATATYPE::INDIGENT && lefttype != righttype)
								{
									Log::writeError(log.stream, Error::GetError(314, tables.lextable.table[k].sn, 0));
									sem_ok = false;
									break;
								}
							}
							if (tables.lextable.table[k + 1].lexema == LEX_LEFTHESIS)
							{
								ignore = true;
								continue;
							}
							if (ignore && tables.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
							{
								ignore = false;
								continue;
							}
						}
						// Запрет арифметики для строк
						if (lefttype == IT::IDDATATYPE::SCROLL)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR || l == LEX_DIRSLASH || l == LEX_PERSENT || l == LEX_BITAND || l == LEX_BITOR || l == LEX_BITNOT)
							{
								Log::writeError(log.stream, Error::GetError(316, tables.lextable.table[k].sn, 0));
								sem_ok = false;
								break;
							}
						}
					}
				}
				break;
			}

			case LEX_ID:
			{
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];

				// Проверка возвращаемого значения (comeback)
				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_ACTION)
				{
					if (e.idtype == IT::IDTYPE::F)
					{
						for (int k = i + 1; ; k++)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_COMEBACK)
							{
								int next = tables.lextable.table[k + 1].idxTI;
								if (next != NULLIDX_TI)
								{
									// Проверка совместимости типа возврата
									if (tables.idtable.table[next].iddatatype != e.iddatatype)
									{
										Log::writeError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, 0));
										sem_ok = false;
										break;
									}
								}
								break;
							}
							if (k == tables.lextable.size) break;
						}
					}
				}
				// Проверка параметров при вызове функции
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTHESIS && tables.lextable.table[i - 1].lexema != LEX_ACTION)
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S)
					{
						int paramscount = NULL;
						for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
						{
							if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
							{
								paramscount++;
								if (e.value.params.count == NULL)
									break;

								IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
								if (paramscount > 3)
								{
									Log::writeError(log.stream, Error::GetError(307, tables.lextable.table[i].sn, 0));
									sem_ok = false;
								}
								if (ctype != e.value.params.types[paramscount - 1] && paramscount <= e.value.params.count)
								{
									Log::writeError(log.stream, Error::GetError(309, tables.lextable.table[i].sn, 0));
									sem_ok = false;
									break;
								}
							}
							if (j == tables.lextable.size)
								break;
						}
						if (paramscount != e.value.params.count)
						{
							Log::writeError(log.stream, Error::GetError(308, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
					}
				}
				break;
			}
			case LEX_COUNCIL:
			{
				int exprPos = -1;
				for (int j = i + 1; j < tables.lextable.size && tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					if (tables.lextable.table[j].idxTI != NULLIDX_TI)
					{
						exprPos = j;
						break;
					}

				IT::IDDATATYPE exprType = IT::IDDATATYPE::INDIGENT;
				if (exprPos != -1 && tables.lextable.table[exprPos].idxTI != NULLIDX_TI)
					exprType = tables.idtable.table[tables.lextable.table[exprPos].idxTI].iddatatype;

				if (exprType != IT::IDDATATYPE::SQUIRE && exprType != IT::IDDATATYPE::SCROLL && exprType != IT::IDDATATYPE::RUNE)
				{
					sem_ok = false;
					Log::writeError(log.stream, Error::GetError(320, tables.lextable.table[i].sn, 0));
				}

				int rightThesis = -1;
				for (int j = i; j < tables.lextable.size; j++)
					if (tables.lextable.table[j].lexema == LEX_RIGHTTHESIS)
					{
						rightThesis = j;
						break;
					}

				int bodyBegin = -1;
				if (rightThesis != -1)
					for (int j = rightThesis + 1; j < tables.lextable.size; j++)
						if (tables.lextable.table[j].lexema == LEX_LEFTBRACE)
						{
							bodyBegin = j;
							break;
						}

				if (bodyBegin != -1 && rightThesis != -1)
				{
					bool defaultFound = false;
					std::set<std::string> seenPaths; // Сет для хранения значений path, чтобы искать дубликаты

					int depth = 0;
					for (int j = bodyBegin; j < tables.lextable.size; j++)
					{
						if (tables.lextable.table[j].lexema == LEX_LEFTBRACE)
							depth++;
						if (tables.lextable.table[j].lexema == LEX_BRACELET)
						{
							depth--;
							if (depth == 0)
								break;
						}

						// Проверка PATH
						if (tables.lextable.table[j].lexema == LEX_PATH)
						{
							if (j + 1 >= tables.lextable.size || tables.lextable.table[j + 1].idxTI == NULLIDX_TI)
							{
								sem_ok = false;
								Log::writeError(log.stream, Error::GetError(321, tables.lextable.table[j].sn, 0));
								continue;
							}

							IT::Entry* pathVal = &tables.idtable.table[tables.lextable.table[j + 1].idxTI];
							IT::IDDATATYPE caseType = pathVal->iddatatype;

							// 1. Проверка типов
							if (exprType != IT::IDDATATYPE::INDIGENT && caseType != exprType)
							{
								sem_ok = false;
								Log::writeError(log.stream, Error::GetError(321, tables.lextable.table[j].sn, 0));
							}

							// 2. Проверка дубликатов path (НОВОЕ)
							std::string key;
							if (caseType == IT::IDDATATYPE::SQUIRE)
								key = "int_" + std::to_string(pathVal->value.vint);
							else if (caseType == IT::IDDATATYPE::SCROLL || caseType == IT::IDDATATYPE::RUNE)
								key = "str_" + std::string(pathVal->value.vstr.str);

							if (seenPaths.count(key))
							{
								sem_ok = false;
								// Используем 322 (дубликат tiresome) или создаем новую, но 322 логически подходит как "Повтор метки"
								Log::writeError(log.stream, Error::GetError(325, tables.lextable.table[j].sn, 0));
							}
							seenPaths.insert(key);
						}

						// Проверка TIRESOME (default)
						if (tables.lextable.table[j].lexema == LEX_TIRESOME)
						{
							if (defaultFound)
							{
								sem_ok = false;
								Log::writeError(log.stream, Error::GetError(322, tables.lextable.table[j].sn, 0));
							}
							defaultFound = true;
						}
					}
				}
				break;
			}

			// Проверка операций сравнения, а также БИТОВЫХ операций (Добавлены LEX_BITAND, LEX_BITOR, LEX_PERSENT)
			case LEX_MORE:	case LEX_LESS: case LEX_EQUALS:   case LEX_NOTEQUALS:	case LEX_MOREEQUALS:	case LEX_LESSEQUALS:
			case LEX_BITAND: case LEX_BITOR:
			{
				bool flag = true;
				// Проверяем левый операнд
				if (i > 1 && tables.lextable.table[i - 1].idxTI != NULLIDX_TI)
				{
					if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype != IT::IDDATATYPE::SQUIRE)
						flag = false;
				}
				// Проверяем правый операнд
				if (tables.lextable.table[i + 1].idxTI != NULLIDX_TI)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::SQUIRE)
						flag = false;
				}
				if (!flag)
				{
					// Используем 317 (Неверное условие) или 314 (Несовместимые типы)
					Log::writeError(log.stream, Error::GetError(317, tables.lextable.table[i].sn, 0));
					sem_ok = false;
				}
				break;
			}
			// Унарный bitnot
			case LEX_BITNOT:
			{
				bool flag = true;
				if (tables.lextable.table[i + 1].idxTI != NULLIDX_TI)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype != IT::IDDATATYPE::SQUIRE)
						flag = false;
				}
				if (!flag)
				{
					Log::writeError(log.stream, Error::GetError(317, tables.lextable.table[i].sn, 0));
					sem_ok = false;
				}
				break;
			}
			}
		}
		return sem_ok;
	}
};