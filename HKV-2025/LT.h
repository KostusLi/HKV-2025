#pragma once
#include <iostream>
#define LEX_SEPARATORS 'S'
#define LEX_INTSTRBOOLCHAR 't'
#define LEX_HOLLOW 'h'
#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_ACTION 'a'
#define LEX_ELDER 'e'
#define LEX_COMEBACK 'r'
#define LEX_PRINT 'u'
#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTHESIS '['
#define LEX_RIGHTTHESIS ']'
#define LEX_PLUS '+'
#define LEX_MINUS '-'
#define LEX_STAR '*'
#define LEX_DIRSLASH '/'
#define LEX_TEMPLE 'm'
#define LEX_ASSIGN '='
#define LEX_EQUALS '!'
#define LEX_NONEQUALS '?'
#define LEX_CHARGE 'c'
#define LEX_BACKUP 'b'
#define LEX_PATROL 'w'
#define LEX_COUNCIL 's'
#define LEX_PATH 'p'
#define LEX_TIRESOME 'd'
#define LEX_BIND '&'
#define LEX_ALLY '|'
#define LEX_BANISH '~'
#define LEX_COMPSCROLLS 'x'
#define LEX_CONSOLIDATE 'y'
#define LEX_MIGHTINESS 'z'
#define LEX_FILAMENT 'n'
#define LEX_MORE '>'	
#define LEX_LESS '<'
#define LEX_EXILE '0'
#define LEX_FAITH '1'
#define LEX_LEFTBRAC '('
#define LEX_RIGHTBRAC ')'
#define LEX_LITERAL_OCTAL 'k'
#define	LEXEMA_FIXSIZE 1	    
#define	LT_MAXSIZE 6000	
#define	NULLDX_TI 0xffffffff

namespace LT
{
	struct Entry
	{
		char lexema;
		int sn;
		int idxTI;

		Entry();
		Entry(char lexema, int snn, int idxti = NULLDX_TI);
	};

	struct LexTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	void writeLexTable(std::ostream* stream, LT::LexTable& lextable);
	void writeLexemsOnLines(std::ostream* stream, LT::LexTable& lextable);
};