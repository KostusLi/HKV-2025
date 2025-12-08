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
#define CONCAT_PARAMS_CNT 2					//кол-во параметров у функции concat
#define RANDOM_PARAMS_CNT 2					//кол-во параметров у функции random
#define LENGHT_PARAMS_CNT 1					//кол-во параметров у функции lenght
#define ATOII_PARAMS_CNT 1					//кол-во параметров у функции atoii
#define COMPARE_PARAMS_CNT 2				//кол-во параметров у функции compare
#define COPY_PARAMS_CNT 1					//кол-во параметров у функции copystr
#define CONCAT_TYPE IT::IDDATATYPE::STR
#define COMPARE_TYPE IT::IDDATATYPE::INT
#define LENGHT_TYPE IT::IDDATATYPE::INT
#define RANDOM_TYPE IT::IDDATATYPE::INT
#define ATOII_TYPE IT::IDDATATYPE::INT
#define COPY_TYPE IT::IDDATATYPE::STR


namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2, PROC = 3, CHAR = 4, UNDEF };//типы данных идентификаторов: числовой, строковый, без типа(для процедур), неопределенный
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };	//типы идентификаторов: переменная, функция, параметр, литерал, стандартная функция
	enum STDFNC { F_COMPARE, F_RANDOM, F_CONCAT, F_LENGHT, F_ATOII, F_COPY, F_NOT_STD };	//стандартные функции
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };//параметры функции  concatstr
	static const IDDATATYPE LENGHT_PARAMS[] = { IT::IDDATATYPE::STR };//параметры функции strlen
	static const IDDATATYPE ATOII_PARAMS[] = { IT::IDDATATYPE::STR };//параметры ф-ции atoi
	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };//параметры ф-ции atoi
	static const IDDATATYPE COMPARE_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };//параметры ф-ции atoi
	static const IDDATATYPE COPY_PARAMS[] = { IT::IDDATATYPE::STR }; //параметры функции copystr
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
