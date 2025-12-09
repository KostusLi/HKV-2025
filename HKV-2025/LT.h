#pragma once
#include <fstream>
#define LEX_SEPARATORS	'S'
#define	LEX_ID_TYPE 't'
#define	LEX_ID	'i'
#define	LEX_LITERAL	'l'
#define	LEX_ACTION 'f'
#define	LEX_TEMPLE	'm'
#define	LEX_SEPARATOR	';'
#define	LEX_COMMA	','
#define	LEX_LEFTBRACE	'['
#define	LEX_BRACELET	']'
#define	LEX_LEFTHESIS	'('
#define	LEX_RIGHTTHESIS	')'
#define	LEX_PLUS	'+'
#define	LEX_MINUS	'-'
#define	LEX_STAR	'*'
#define LEX_DIRSLASH	'/'
#define LEX_PERSENT	'%'
#define	LEX_EQUAL	'='
#define LEX_CONDITION	'?'
#define LEX_DIEZ	'#'
#define LEX_CHARGE	'w'
#define LEX_BACKUP	'r'
#define LEX_PATROL	'c'
#define LEX_CONFESSION	'o'
#define LEX_ESCAPE 'g'
#define LEX_NEWLEAF	'^'
#define LEX_COMEBACK	'e'
#define LEX_HOLLOW	'p'
#define LEX_ELDER	'n'
#define LEX_MORE	'>'
#define LEX_LESS	'<'
#define LEX_EQUALS	'&'
#define LEX_NOTEQUALS	'!'
#define LEX_MOREEQUALS	'~'
#define LEX_LESSEQUALS	'@'
#define LEX_BITAND 'z'
#define LEX_BITOR 'x'
#define LEX_BITNOT 'j'
#define LEX_CHECK 'q'
#define LEX_CASE 'v'
#define LEX_DEFAULT 'd'
#define	LEXEMA_FIXSIZE   1
#define	MAXSIZE_LT		 4096
#define	NULLDX_TI	 0xffffffff
#define LEX_LITERAL_OCT  'k'

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