#pragma once
#include <iostream>

#define MAXSIZE_ID	16						//макс число символов идентификатора
#define SCOPED_ID_MAXSIZE   MAXSIZE_ID*2	//макс число символов идентификатор + область видимости
#define MAXSIZE_TI		4096				//макс число количество строк в таблице идентификаторов
#define INT_DEFAULT	0x00000000				//значение по умолчанию для digit
#define STR_DEFAULT	0x00					//значение по умолчанию для sting
#define NULLIDX_TI		0xffffffff			//нет элемента таблицы идентификаторов
#define STR_MAXSIZE	255						//максимальная длина строкового литерала
#define CHAR_MAXSIZE 1						//максимальная длина символьного литерала
#define INT_MAXSIZE   127			//максимальное значение для целочисленного типа
#define INT_MINSIZE   -128			//минимальное значение для целочисленного типа
#define MAX_PARAMS_COUNT 3					//максимальное количество параметров у функции
#define CONSOLIDATE_PARAMS_CNT 2					//кол-во параметров у функции concat
#define FORTUNE_PARAMS_CNT 2					//кол-во параметров у функции random
#define FILAMENT_PARAMS_CNT 1					//кол-во параметров у функции lenght
#define TRANSMUTE_PARAMS_CNT 1					//кол-во параметров у функции atoii
#define COMPARESCROLLS_PARAMS_CNT 2				//кол-во параметров у функции compare
#define OBLIVION_PARAMS_CNT 1					//кол-во параметров у функции copystr
#define CONSOLIDATE_TYPE IT::IDDATATYPE::SCROLL
#define COMPARESCROLLS_TYPE IT::IDDATATYPE::SQUIRE
#define FILAMENT_TYPE IT::IDDATATYPE::SQUIRE
#define FORTUNE_TYPE IT::IDDATATYPE::SQUIRE
#define TRANSMUTE_TYPE IT::IDDATATYPE::SQUIRE
#define OBLIVION_TYPE IT::IDDATATYPE::SCROLL


namespace IT
{
	enum IDDATATYPE { SQUIRE = 1, SCROLL = 2, HOLLOW = 3, RUNE = 4, INDIGENT };//типы данных идентификаторов: числовой, строковый, без типа(для процедур), неопределенный
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };	//типы идентификаторов: переменная, функция, параметр, литерал, стандартная функция
	enum STDFNC { F_COMPARESCROLLS, F_FORTUNE, F_CONSOLIDATE, F_FILAMENT, F_TRANSMUTE, F_OBLIVION, F_FOLK };	//стандартные функции
	static const IDDATATYPE CONSOLIDATE_PARAMS[] = { IT::IDDATATYPE::SCROLL, IT::IDDATATYPE::SCROLL };//параметры функции  concatstr
	static const IDDATATYPE FILAMENT_PARAMS[] = { IT::IDDATATYPE::SCROLL };//параметры функции strlen
	static const IDDATATYPE TRANSMUTE_PARAMS[] = { IT::IDDATATYPE::SCROLL };//параметры ф-ции atoi
	static const IDDATATYPE FORTUNE_PARAMS[] = { IT::IDDATATYPE::SQUIRE, IT::IDDATATYPE::SQUIRE };//параметры ф-ции atoi
	static const IDDATATYPE COMPARESCROLLS_PARAMS[] = { IT::IDDATATYPE::SCROLL, IT::IDDATATYPE::SCROLL };//параметры ф-ции atoi
	static const IDDATATYPE OBLIVION_PARAMS[] = { IT::IDDATATYPE::SCROLL }; //параметры функции copystr
	struct Entry
	{
		union
		{
			int	vint;            			//значение integer
			struct
			{
				int len;					//количество символов
				char str[STR_MAXSIZE - 1];//символы
			} vstr;							//значение строки
			struct
			{
				int count;					// количество параметров функции
				IDDATATYPE* types;			//типы параметров функции
			} params;
		} value;						//значение идентификатора
		int			idxfirstLE;				//индекс в таблице лексем		
		char		id[SCOPED_ID_MAXSIZE];	//идентификатор
		IDDATATYPE	iddatatype;				//тип данных
		IDTYPE		idtype;					//тип идентификатора

		Entry()							//конструктор без параметров
		{
			this->value.vint = INT_DEFAULT;
			this->value.vstr.len = NULL;
			this->value.params.count = NULL;
			this->value.params.types = nullptr;
			this->idxfirstLE = NULL;
			this->id[0] = '\0';
			this->iddatatype = IT::IDDATATYPE::INDIGENT;
			this->idtype = IT::IDTYPE::V;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype) //конструктор с параметрами
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};
	};
	struct IdTable		//экземпляр таблицы идентификаторов
	{
		int maxsize;	//ёмкость таблицы идентификаторов < TI_MAXSIZE
		int size;		//текущий размер таблицы идентификаторов < maxsize
		Entry* table;	//массив строк таблицы идентификаторов
	};
	IdTable Create(int size = NULL);	//ёмкость таблицы идентификаторов < TI_MAXSIZE
	void Add(					//добавить строку в таблицу идентификаторов
		IdTable& idtable,		//экземпляр таблицы идентификаторов
		Entry entry);			//строка таблицы идентификаторов 
	int isId(					//возврат: номер строки(если есть), TI_NULLIDX(если есть)
		IdTable& idtable,		//экземпляр таблицы идентификаторов
		char id[SCOPED_ID_MAXSIZE]);	//идентификатор
	bool SetValue(IT::Entry* entry, char* value);	//задать значение идентификатора
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable); //вывести таблицу идентификаторов
};
