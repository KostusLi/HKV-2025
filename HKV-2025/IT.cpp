#include "stdafx.h"
#include <iomanip>

#pragma warning(disable : 4996)
#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) "   |  " W(4,line) "     |" W(14,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace IT
{
	IdTable Create(int size)
	{
		if (size > MAXSIZE_TI)
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

	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return NULLIDX_TI;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value)
	{
		bool rc = true;
		if (entry->iddatatype == SQUIRE)
		{
			int temp = atoi(value);
			if (temp > INT_MAXSIZE || temp < INT_MINSIZE)
			{
				if (temp > INT_MAXSIZE)
					temp = INT_MAXSIZE;
				if (temp < INT_MINSIZE)
					temp = INT_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
		}
		else
		{
			for (unsigned i = 1; i < strlen(value) - 1; i++)
				entry->value.vstr.str[i - 1] = value[i];
			entry->value.vstr.str[strlen(value) - 2] = '\0';
			entry->value.vstr.len = strlen(value) - 2;
		}
		return rc;
	}
	void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
	{
		*stream << "---------------------------- ÒÓÐÍÈÐÍÀß ÒÀÁËÈÖÀ ------------------------\n" << std::endl;
		*stream << "|  ¹¹ |ÑÒÐÎÊÀ Â ÒÒ| ÎÏÈÑÀÍÈÅ Ó×ÀÑÒÍÈÊÀ |       ÐÎÄÎÂÎÅ ÈÌß                 | ÐÎÄÎÂÎÅ ÄÅÐÅÂÎ (ÒÅÊÓÙÅÅ ÏÎÊÎËÅÍÈÅ)" << std::endl;
		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry* e = &idtable.table[i];
			char type[50] = "";

			switch (e->iddatatype)
			{
			case IT::IDDATATYPE::SQUIRE:
				strcat(type, " squire");
				break;
			case IT::IDDATATYPE::SCROLL:
				strcat(type, " scroll");
				break;
			case IT::IDDATATYPE::HOLLOW:
				strcat(type, " hollow");
				break;
			case IT::IDDATATYPE::RUNE:
				strcat(type, "   rune");
				break;
			case IT::IDDATATYPE::INDIGENT:
				strcat(type, "NYAMA TIPY");
				break;
			}
			switch (e->idtype)
			{
			case IT::IDTYPE::V:
				strcat(type, "  variable  ");
				break;
			case IT::IDTYPE::F:
				strcat(type, "  function  ");
				break;
			case IT::IDTYPE::P:
				strcat(type, " parameter  ");
				break;
			case IT::IDTYPE::L:
				strcat(type, "   literal  ");
				break;
			case IT::IDTYPE::S: strcat(type, "  KNIGHTFUN "); break;
			default:
				strcat(type, "NYAMA TIPY ");
				break;
			}

			*stream << STR(i, e->idxfirstLE, type, e->id);
			if (e->idtype == IT::IDTYPE::L || e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::INDIGENT)
			{
				if (e->iddatatype == IT::IDDATATYPE::SQUIRE)
					*stream << e->value.vint;
				else
					*stream << "[" << (int)e->value.vstr.len << "]" << e->value.vstr.str;
			}
			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int i = 0; i < e->value.params.count; i++)
				{
					*stream << " P" << i << ":";
					switch (e->value.params.types[i])
					{
					case IT::IDDATATYPE::SQUIRE:
						*stream << "squire |";
						break;
					case IT::IDDATATYPE::SCROLL:
						*stream << "scroll |";
						break;
					case IT::IDDATATYPE::RUNE:
						*stream << "rune |";
						break;
					case IT::IDDATATYPE::HOLLOW:
					case IT::IDDATATYPE::INDIGENT:
						*stream << "NYAMA TIPY";
						break;
					}
				}
			}
			*stream << std::endl;
		}
		*stream << "\n-------------------------------------------------------------------------\n\n";
	}
};