#include "IT.h"
#include "Error.h"
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <iomanip>

#pragma warning(disable : 4996)
#define W(x, y)\
		<< std::setw(x) << (y) <<

#define STR(n, line, type, id)\
		"|" W(4,n) " |" W(11,line) " |" W(25,type) " |" W(30,id) " |"




namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
			throw ERROR_THROW(203);
		IdTable idtable;
		idtable.table = new Entry[idtable.maxsize = size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
			throw ERROR_THROW(203);
		idtable.table[idtable.size++] = entry;
	}

	//:  (), TI_NULLIDX( )
	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return TI_NULLIDX;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value) //   
	{
		bool rc = true;
		if (entry->iddatatype == INT || entry->iddatatype==BOOL)
		{
			int temp;
			// Обрабатываем литералы типа honor (bool)
			if (entry->iddatatype == BOOL) {
				if (!strcmp(value, "exile")) {
					temp = 1; // true
				} else if (!strcmp(value, "faith")) {
					temp = 0; // false
				} else {
					temp = atoi(value);
				}
			} else {
				temp = atoi(value);
			}
			
			if (temp > TI_INT_MAXSIZE || temp < TI_INT_MINSIZE)
			{
				if (temp > TI_INT_MAXSIZE)
					temp = TI_INT_MAXSIZE;
				if (temp < TI_INT_MINSIZE)
					temp = TI_INT_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
		}
		else
		{
			// Обнуляем массив перед заполнением для предотвращения мусора
			memset(entry->value.vstr.str, 0, TI_STR_MAXSIZE - 1);
			
			unsigned valueLen = strlen(value);
			if (valueLen >= 2) {
				// Копируем содержимое между кавычками
				unsigned copyLen = valueLen - 2; // Длина без кавычек
				if (copyLen > TI_STR_MAXSIZE - 2) {
					copyLen = TI_STR_MAXSIZE - 2; // Ограничиваем размером массива
					rc = false;
				}
				for (unsigned i = 1; i < valueLen - 1 && i - 1 < copyLen; i++)
					entry->value.vstr.str[i - 1] = value[i];
				entry->value.vstr.str[copyLen] = '\0';
				entry->value.vstr.len = copyLen;
			} else {
				entry->value.vstr.len = 0;
				entry->value.vstr.str[0] = '\0';
			}
		}
		return rc;
	}
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
	{
		*stream << "--------------------------------------------------------------------------------------------------------\n";
		*stream << "|  N  |Stroke in TL|ID type                   |Name                         |Variable (parameters)        |\n";
		*stream << "--------------------------------------------------------------------------------------------------------\n";
		
		// Массив для отслеживания выведенных параметров
		bool* paramPrinted = new bool[idtable.size];
		for (int k = 0; k < idtable.size; k++) {
			paramPrinted[k] = false;
		}
		
		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry* e = &idtable.table[i];
			
			// Пропускаем параметры функций - они выводятся после соответствующей функции
			if (e->idtype == IT::IDTYPE::P) {
				continue;
			}
			char type[50] = "";

			switch (e->iddatatype)
			{
			case IT::IDDATATYPE::INT:
				strcat(type, "squire");
				break;
			case IT::IDDATATYPE::STR:
				strcat(type, "scroll");
				break;
			case IT::IDDATATYPE::BOOL:
				strcat(type, "honor");
				break;
			case IT::IDDATATYPE::CHAR:
				strcat(type, "rune");
				break;
			case IT::IDDATATYPE::VOID:
				strcat(type, "hollow");
				break;
			case IT::IDDATATYPE::UNDEF:
				strcat(type, "UNDEFINED");
				break;
			}
			switch (e->idtype)
			{
			case IT::IDTYPE::V:
				strcat(type, " variable");
				break;
			case IT::IDTYPE::F:
				strcat(type, " function");
				break;
			case IT::IDTYPE::P:
				strcat(type, " parameter");
				break;
			case IT::IDTYPE::L:
				strcat(type, " literal");
				break;
			case IT::IDTYPE::S: strcat(type, " LIB FUNC"); break;
			default:
				strcat(type, " UNDEFINED");
				break;
			}

			// Формируем строку с переменной/параметрами
			char variable[200] = "";
			if (e->idtype == IT::IDTYPE::L || (e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF))
			{
				if (e->iddatatype == IT::IDDATATYPE::INT)
					sprintf_s(variable, 200, "%d", e->value.vint);
				else
					sprintf_s(variable, 200, "[%d]%s", (int)e->value.vstr.len, e->value.vstr.str);
			}
			
			// Для функций формируем строку с P0, P1, P2...
			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int j = 0; j < e->value.params.count; j++)
				{
					if (j > 0) strcat_s(variable, 200, ", ");
					char paramStr[50] = "";
					sprintf_s(paramStr, 50, "P%d", j);
					strcat_s(variable, 200, paramStr);
					strcat_s(variable, 200, ":");
					switch (e->value.params.types[j])
					{
					case IT::IDDATATYPE::INT:
						strcat_s(variable, 200, "squire");
						break;
					case IT::IDDATATYPE::STR:
						strcat_s(variable, 200, "scroll");
						break;
					case IT::IDDATATYPE::CHAR:	
						strcat_s(variable, 200, "rune");
						break;
					case IT::IDDATATYPE::BOOL:
						strcat_s(variable, 200, "honor");
						break;
					case IT::IDDATATYPE::UNDEF:
						strcat_s(variable, 200, "UNDEFINED");
						break;
					}
				}
			}

			// Ограничиваем длину имени для красивого вывода
			char name[31] = "";
			size_t idLen = strlen(e->id);
			if (idLen <= 30) {
				strncpy_s(name, 31, e->id, 30);
			} else {
				strncpy_s(name, 31, e->id, 27);
				name[27] = name[28] = name[29] = '.'; // Добавляем ... если имя слишком длинное
				name[30] = '\0';
			}

			*stream << STR(i, e->idxfirstLE, type, name);
			*stream << std::setw(30) << (variable) << " |" << std::endl;
			
			// Если это функция, выводим её параметры сразу после неё
			if ((e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S) && 
				e->value.params.count > 0 && e->value.params.names != nullptr)
			{
				// Проходим по параметрам функции в порядке их объявления
				for (int paramIdx = 0; paramIdx < e->value.params.count; paramIdx++)
				{
					if (e->value.params.names[paramIdx] == nullptr) continue;
					
					char* paramLocalName = e->value.params.names[paramIdx];
					IT::IDDATATYPE expectedType = e->value.params.types[paramIdx];
					
					// Ищем параметр в таблице по имени и типу
					for (int j = 0; j < idtable.size; j++)
					{
						if (paramPrinted[j]) continue;
						
						IT::Entry* paramEntry = &idtable.table[j];
						if (paramEntry->idtype != IT::IDTYPE::P) continue;
						if (paramEntry->iddatatype != expectedType) continue;
						
						// Проверяем имя: параметр может иметь имя "param" или "function.param"
						bool nameMatches = false;
						if (strcmp(paramEntry->id, paramLocalName) == 0) {
							nameMatches = true;
						} else {
							// Проверяем, содержит ли полное имя параметра локальное имя
							const char* dotPos = strchr(paramEntry->id, '.');
							if (dotPos != nullptr && strcmp(dotPos + 1, paramLocalName) == 0) {
								nameMatches = true;
							}
						}
						
						if (nameMatches) {
							paramPrinted[j] = true;
							
							// Формируем тип параметра
							char paramType[50] = "";
							switch (paramEntry->iddatatype)
							{
							case IT::IDDATATYPE::INT:
								strcat(paramType, "squire");
								break;
							case IT::IDDATATYPE::STR:
								strcat(paramType, "scroll");
								break;
							case IT::IDDATATYPE::BOOL:
								strcat(paramType, "honor");
								break;
							case IT::IDDATATYPE::CHAR:
								strcat(paramType, "rune");
								break;
							case IT::IDDATATYPE::UNDEF:
								strcat(paramType, "UNDEFINED");
								break;
							}
							strcat(paramType, " parameter");
							
							// Формируем значение параметра
							char paramValue[200] = "";
							if (paramEntry->iddatatype == IT::IDDATATYPE::INT)
								sprintf_s(paramValue, 200, "%d", paramEntry->value.vint);
							else if (paramEntry->iddatatype == IT::IDDATATYPE::STR)
								sprintf_s(paramValue, 200, "[%d]%s", (int)paramEntry->value.vstr.len, paramEntry->value.vstr.str);
							
							// Ограничиваем длину имени параметра
							char paramName[31] = "";
							size_t paramNameLen = strlen(paramEntry->id);
							if (paramNameLen <= 30) {
								strncpy_s(paramName, 31, paramEntry->id, 30);
							} else {
								strncpy_s(paramName, 31, paramEntry->id, 27);
								paramName[27] = paramName[28] = paramName[29] = '.';
								paramName[30] = '\0';
							}
							
							*stream << STR(j, paramEntry->idxfirstLE, paramType, paramName);
							*stream << std::setw(30) << (paramValue) << " |" << std::endl;
							
							break; // Нашли параметр, переходим к следующему
						}
					}
				}
			}
		}
		
		delete[] paramPrinted;
		*stream << "--------------------------------------------------------------------------------------------------------\n\n";
	}
};

