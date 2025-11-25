#pragma once	
#include <iostream>
#include <cstring>

#define ID_MAXSIZE 20
#define SCOPED_ID_MAXSIZE   ID_MAXSIZE*2
#define TI_MAXSIZE 2000
#define TI_INT_DEFAULT 0x00000000		
#define TI_STR_DEFAULT '\0'
#define TI_BOOL_DEFAULT 0
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255
#define CHAR_MAXSIZE 1
#define TI_INT_MAXSIZE 127
#define TI_INT_MINSIZE -128
#define MAX_PARAMS_COUNT 3
#define COMP_SCROL_PARAMS_CNT 2
#define CONSOLIDATE_PARAMS_CNT 2
#define MIGHTINESS_PARAMS_CNT 2
#define CONF_SCROLL_PARAMS_CNT 1
#define CONF_SQUIREHON_PARAMS_CNT 1
#define FILAMENT_PARAMS_CNT 1
#define CONF_RUNE_PARAMS_CNT 1
#define COMP_SCROL_TYPE IT::IDDATATYPE::BOOL
#define CONSOLIDATE_TYPE IT::IDDATATYPE::STR
#define MIGHTINESS_TYPE IT::IDDATATYPE::INT
#define FILAMENT_TYPE IT::IDDATATYPE::INT
#define CONF_SCROLL_TYPE IT::IDDATATYPE::VOID
#define CONF_SQUIREHON_TYPE IT::IDDATATYPE::VOID
#define CONF_RUNE_TYPE IT::IDDATATYPE::VOID


namespace IT {
    enum IDDATATYPE { INT = 1, STR = 2, BOOL = 3, CHAR = 4, VOID = 5, UNDEF=6 };
    enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S=5 };
	enum STDFNC {F_COMPSCR, F_CONS, F_MIGHT, F_CONFSC, F_CONFSQHO, F_FILAM, F_CONFRU, F_NOT_STD};

	static const IDDATATYPE COMP_SCROL_PARAMS[] = {IT::IDDATATYPE::STR, IT::IDDATATYPE::STR};
	static const IDDATATYPE CONSOLIDATE_PARAMS[] = {IT::IDDATATYPE::STR, IT::IDDATATYPE::STR};
	static const IDDATATYPE MIGHTINESS_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };
	static const IDDATATYPE CONF_SCROLL_PARAMS[] = {IT::IDDATATYPE::STR};
	static const IDDATATYPE CONF_SQUIREHON_PARAMS[] = {IT::IDDATATYPE::INT};
	static const IDDATATYPE FILAMENT_PARAMS[] = {IT::IDDATATYPE::STR};
	static const IDDATATYPE CONF_RUNE_PARAMS[] = {IT::IDDATATYPE::CHAR};

    struct Entry {
		union
		{
			int	vint;
			struct
			{
				int len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr;
			struct
			{
				int count;
				IDDATATYPE* types;
				char** names; // Массив имен параметров
			} params;
		} value;
		int			idxfirstLE;		
		char		id[SCOPED_ID_MAXSIZE];
		IDDATATYPE	iddatatype;
		IDTYPE		idtype;

		Entry()
		{
			// Инициализация всех полей для предотвращения мусора
			memset(this->id, 0, SCOPED_ID_MAXSIZE);
			this->idxfirstLE = 0;
			this->iddatatype = IDDATATYPE::UNDEF;
			this->idtype = IDTYPE::V;
			
			// Инициализация union
			this->value.vint = TI_INT_DEFAULT;
			this->value.vstr.len = 0;
			memset(this->value.vstr.str, 0, TI_STR_MAXSIZE - 1);
			this->value.params.count = 0;
			this->value.params.types = nullptr;
			this->value.params.names = nullptr;
		};
		Entry(char* id, int idxLT, IDDATATYPE datatype, IDTYPE idtype)
		{
			strncpy_s(this->id, id, SCOPED_ID_MAXSIZE - 1);
			this->idxfirstLE = idxLT;
			this->iddatatype = datatype;
			this->idtype = idtype;
		};

    };

    struct IdTable {
        int maxsize;
        int size;
        Entry* table;
    };

	IdTable Create(int size = NULL);	//������� ������� ��������������� < TI_MAXSIZE
	void Add(					//�������� ������ � ������� ���������������
		IdTable& idtable,		//��������� ������� ���������������
		Entry entry);			//������ ������� ��������������� 
	int isId(					//�������: ����� ������(���� ����), TI_NULLIDX(���� ����)
		IdTable& idtable,		//��������� ������� ���������������
		char id[SCOPED_ID_MAXSIZE]);	//�������������
	bool SetValue(IT::Entry* entry, char* value);	//������ �������� ��������������
	bool SetValue(IT::IdTable& idtable, int index, char* value);
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable); //������� ������� ���������������
}
