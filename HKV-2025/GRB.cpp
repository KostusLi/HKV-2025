#include "stdafx.h"
#include <cstdarg>
#define GRB_ERROR_SERIES 600

typedef short GRBALPHABET;

namespace GRB
{
	// Терминалы для замены указаны в LT.h, а автоматы конечные для слов в GRaphs.h
	// Терминалы (символы): a(action fn), h(action hollow/proc), m(temple),
	// e(elder), t(type), i(id), l(literal), r(return/comeback), c(charge), b(backup),
	// w(patrol), s(council), p(path), d(tiresome), o/confessionScroll, u/confessionSquire, j/confessionRune,
	// x,y,z,n - stdlib funcs; &,|,~ - bit ops; !,? - equality/inequality
	// ^ - newleaf (перевод строки)
	// Скобки: [ ] - параметры и условия / вызовы; ( ) - вызовы для функций; { } - тело функции

	Greibach greibach(NS('S'), TS('$'), 16,

		// S - программа: объявления функций (функция/процедура) и блок temple
		Rule(NS('S'), GRB_ERROR_SERIES, 5,						// 600 - ошибка в объявлении функции
			// action type id [ F ] { T r E ; } S  - функция с return
			Rule::Chain(13, TS('a'), TS('t'), TS('i'), TS('['), NS('F'), TS(']'), TS('{'), NS('T'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			// action type id [ F ] { T r E ; }  - функция с return (последняя)
			Rule::Chain(12, TS('a'), TS('t'), TS('i'), TS('['), NS('F'), TS(']'), TS('{'), NS('T'), TS('r'), NS('E'), TS(';'), TS('}')),
			// action hollow id [ F ] { P r ; } S  - процедура
			Rule::Chain(12, TS('a'), TS('h'), TS('i'), TS('['), NS('F'), TS(']'), TS('{'), NS('P'), TS('r'), TS(';'), TS('}'), NS('S')),
			// action hollow id [ F ] { P r ; }  - процедура (последняя)
			Rule::Chain(11, TS('a'), TS('h'), TS('i'), TS('['), NS('F'), TS(']'), TS('{'), NS('P'), TS('r'), TS(';'), TS('}')),
			// temple { N }  - главный блок
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))
		),

		// N - тело temple: различные конструкции
		Rule(NS('N'), GRB_ERROR_SERIES + 14, 30,				// 614 - ошибка в теле программы
			// Объявления
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS(';')),	// e t i ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS(';'), NS('N')),	// e t i ; N
			// Объявления с присваиванием
			Rule::Chain(6, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),	// e t i = E ;
			Rule::Chain(7, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),	// e t i = E ; N
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';')),	// e t i = l ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';'), NS('N')),	// e t i = l ; N
			Rule::Chain(10, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),	// e t i = i ( W ) ; N
			Rule::Chain(9, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// e t i = i ( W ) ;
			// Присваивания
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),	// i = E ;
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),	// i = E ; N
			Rule::Chain(4, TS('i'), TS('='), TS('l'), TS(';')),	// i = l ;
			Rule::Chain(5, TS('i'), TS('='), TS('l'), TS(';'), NS('N')),	// i = l ; N
			Rule::Chain(8, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')),	// i = i ( E ) ; N
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';')),	// i = i ( E ) ;
			// Вызовы функций
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// i ( W ) ;
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),	// i ( W ) ; N
			// Выводы
			Rule::Chain(3, TS('u'), TS('i'), TS(';')),	// u ( i ) ;
			Rule::Chain(4, TS('u'), TS('i'), TS(';'), NS('N')),	// u ( i ) ; N
			Rule::Chain(3, TS('u'), TS('l'), TS(';')),	// u ( l ) ;
			Rule::Chain(4, TS('u'), TS('l'), TS(';'), NS('N')),	// u ( l ) ; N
			// Перевод строки
			Rule::Chain(2, TS('^'), TS(';')),	// ^ ;  (newleaf)
			Rule::Chain(3, TS('^'), TS(';'), NS('N')),	// ^ ; N
			// Условные конструкции
			Rule::Chain(2, TS('c'), NS('X')),	// X
			Rule::Chain(2, TS('w'), NS('X')),
			Rule::Chain(3, TS('c'), NS('X'), NS('N')),	// X
			Rule::Chain(3, TS('w'), NS('X'), NS('N')),
			Rule::Chain(3, TS('i'), TS('='), NS('L')),	// L
			Rule::Chain(4, TS('i'), TS('='), NS('L'), NS('N')),	// L N
			Rule::Chain(2, TS('s'), NS('H')),	// H
			Rule::Chain(3, TS('s'), NS('H'), NS('N'))	// H N
		),

		// T - тело функции (с return)
		Rule(NS('T'), GRB_ERROR_SERIES + 3, 32,					// 603 - ошибка в теле функции
			// Объявления
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS(';')),	// e t i ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS(';'), NS('T')),	// e t i ; T
			// Объявления с присваиванием
			Rule::Chain(6, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),	// e t i = E ;
			Rule::Chain(7, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('T')),	// e t i = E ; T
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';')),	// e t i = l ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';'), NS('T')),	// e t i = l ; T
			Rule::Chain(10, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('T')),	// e t i = i ( W ) ; T
			Rule::Chain(9, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// e t i = i ( W ) ;
			// Присваивания
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),	// i = E ;
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('T')),	// i = E ; T
			Rule::Chain(4, TS('i'), TS('='), TS('l'), TS(';')),	// i = l ;
			Rule::Chain(5, TS('i'), TS('='), TS('l'), TS(';'), NS('T')),	// i = l ; T
			Rule::Chain(8, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';'), NS('T')),	// i = i ( E ) ; T
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';')),	// i = i ( E ) ;
			// Return
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),	// r E ;
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('T')),	// r E ; T
			// Вызовы функций
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// i ( W ) ;
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('T')),	// i ( W ) ; T
			// Выводы
			Rule::Chain(3, TS('u'), TS('i'), TS(';')),	// u ( i ) ;
			Rule::Chain(4, TS('u'), TS('i'), TS(';'), NS('T')),	// u ( i ) ; N
			Rule::Chain(3, TS('u'), TS('l'), TS(';')),	// u ( l ) ;
			Rule::Chain(4, TS('u'), TS('l'), TS(';'), NS('T')),	// u ( l ) ; N
			// Перевод строки
			Rule::Chain(2, TS('^'), TS(';')),	// ^ ;  (newleaf)
			Rule::Chain(3, TS('^'), TS(';'), NS('T')),	// ^ ; T
			// Условные конструкции
			Rule::Chain(2, TS('c'), NS('X')),	// X
			Rule::Chain(2, TS('w'), NS('X')),
			Rule::Chain(3, TS('c'), NS('X'), NS('T')),	// X
			Rule::Chain(3, TS('w'), NS('X'), NS('T')),
			Rule::Chain(3, TS('i'), TS('='), NS('L')),	// L
			Rule::Chain(4, TS('i'), TS('='), NS('L'), NS('T')),
			Rule::Chain(2, TS('s'), NS('H')),	// H
			Rule::Chain(3, TS('s'), NS('H'), NS('T'))	// H T
		),

		// P - тело процедуры (без return)
		Rule(NS('P'), GRB_ERROR_SERIES + 3, 30,					// 603 - ошибка в теле функции
			// Объявления
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS(';')),	// e t i ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS(';'), NS('P')),	// e t i ; P
			// Объявления с присваиванием
			Rule::Chain(6, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),	// e t i = E ;
			Rule::Chain(7, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('P')),	// e t i = E ; P
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';')),	// e t i = l ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';'), NS('P')),	// e t i = l ; P
			Rule::Chain(10, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('P')),	// e t i = i ( W ) ; P
			Rule::Chain(9, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// e t i = i ( W ) ;
			// Присваивания
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),	// i = E ;
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('P')),	// i = E ; P
			Rule::Chain(4, TS('i'), TS('='), TS('l'), TS(';')),	// i = l ;
			Rule::Chain(5, TS('i'), TS('='), TS('l'), TS(';'), NS('P')),	// i = l ; P
			Rule::Chain(8, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';'), NS('P')),	// i = i ( E ) ; P
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';')),	// i = i ( E ) ;
			// Вызовы функций
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// i ( W ) ;
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('P')),	// i ( W ) ; P
			// Выводы
			Rule::Chain(3, TS('u'), TS('i'), TS(';')),	// u ( i ) ;
			Rule::Chain(4, TS('u'), TS('i'), TS(';'), NS('P')),	// u ( i ) ; N
			Rule::Chain(3, TS('u'), TS('l'), TS(';')),	// u ( l ) ;
			Rule::Chain(4, TS('u'), TS('l'), TS(';'), NS('P')),	// u ( l ) ; N
			// Перевод строки
			Rule::Chain(2, TS('^'), TS(';')),	// ^ ;  (newleaf)
			Rule::Chain(3, TS('^'), TS(';'), NS('P')),	// ^ ; P
			// Условные конструкции
			Rule::Chain(2, TS('c'), NS('X')),	// X
			Rule::Chain(2, TS('w'), NS('X')),
			Rule::Chain(3, TS('c'), NS('X'), NS('P')),	// X
			Rule::Chain(3, TS('w'), NS('X'), NS('P')),
			Rule::Chain(3, TS('i'), TS('='), NS('L')),	// L
			Rule::Chain(4, TS('i'), TS('='), NS('L'), NS('P')),	// L P
			Rule::Chain(2, TS('s'), NS('H')),	// H
			Rule::Chain(3, TS('s'), NS('H'), NS('P'))	// H P
		),

		// C - тело внутри charge/patrol/council/path/tiresome
		Rule(NS('C'), GRB_ERROR_SERIES + 14, 30,					// 614 - ошибка в теле программы
			// Объявления
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS(';')),	// e t i ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS(';'), NS('C')),	// e t i ; C
			// Объявления с присваиванием
			Rule::Chain(6, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),	// e t i = E ;
			Rule::Chain(7, TS('e'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('C')),	// e t i = E ; C
			Rule::Chain(4, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';')),	// e t i = l ;
			Rule::Chain(5, TS('e'), TS('t'), TS('i'), TS('='), TS('l'), TS(';'), NS('C')),	// e t i = l ; C
			Rule::Chain(10, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('C')),	// e t i = i ( W ) ; C
			Rule::Chain(9, TS('e'), TS('t'), TS('i'), TS('='), TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// e t i = i ( W ) ;
			// Присваивания
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),	// i = E ;
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('C')),	// i = E ; C
			Rule::Chain(4, TS('i'), TS('='), TS('l'), TS(';')),	// i = l ;
			Rule::Chain(5, TS('i'), TS('='), TS('l'), TS(';'), NS('C')),	// i = l ; C
			Rule::Chain(8, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';'), NS('C')),	// i = i ( E ) ; C
			Rule::Chain(7, TS('i'), TS('='), TS('i'), TS('('), NS('E'), TS(')'), TS(';')),	// i = i ( E ) ;
			// Вызовы функций
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),	// i ( W ) ;
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('C')),	// i ( W ) ; C
			// Выводы
			Rule::Chain(3, TS('u'), TS('i'), TS(';')),	// u ( i ) ;
			Rule::Chain(4, TS('u'), TS('i'), TS(';'), NS('C')),	// u ( i ) ; N
			Rule::Chain(3, TS('u'), TS('l'), TS(';')),	// u ( l ) ;
			Rule::Chain(4, TS('u'), TS('l'), TS(';'), NS('C')),	// u ( l ) ; N
			// Перевод строки
			Rule::Chain(2, TS('^'), TS(';')),	// ^ ;  (newleaf)
			Rule::Chain(3, TS('^'), TS(';'), NS('C')),	// ^ ; C
			// Условные конструкции
			Rule::Chain(2, TS('c'), NS('X')),	// X
			Rule::Chain(2, TS('w'), NS('X')),
			Rule::Chain(3, TS('c'), NS('X'), NS('C')),	// X
			Rule::Chain(3, TS('w'), NS('X'), NS('C')),
			Rule::Chain(3, TS('i'), TS('='), NS('L')),	// L
			Rule::Chain(4, TS('i'), TS('='), NS('L'), NS('C')),	// L C
			Rule::Chain(2, TS('s'), NS('H')),	// H
			Rule::Chain(3, TS('s'), NS('H'), NS('C'))	// H C
		),

		// F - список параметров функции: t i (, t i)*  или  t i  или  i
		Rule(NS('F'), GRB_ERROR_SERIES + 4, 3,					// 604 - ошибка в списке параметров функции
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F')),	// t i , F
			Rule::Chain(2, TS('t'), TS('i')),	// t i
			Rule::Chain(1, TS('i'))	// i
		),

		// W - список аргументов вызова: i | l | i, W | l, W | )
		Rule(NS('W'), GRB_ERROR_SERIES + 6, 5,					// 606 - ошибка в списке передаваемых параметров функции
			Rule::Chain(1, TS('i')),	// i
			Rule::Chain(3, TS('i'), TS(','), NS('W')),	// i , W
			Rule::Chain(1, TS('l')),	// l
			Rule::Chain(3, TS('l'), TS(','), NS('W')),	// l , W
			Rule::Chain(1, TS(')'))	// )  (пустой список)
		),

		// E - выражение: i | l | ( E ) | i ( W ) | i M | l M | ( E ) M | вызовы stdlib
		Rule(NS('E'), GRB_ERROR_SERIES + 5, 13,					// 605 - ошибка в списке вызова/присваивания
			Rule::Chain(1, TS('i')),	// i
			Rule::Chain(1, TS('l')),	// l
			Rule::Chain(3, TS('('), NS('E'), TS(')')),	// ( E )
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),	// i ( W )
			Rule::Chain(4, TS('i'), TS('('), NS('F'), TS(')')),	// i ( F )  (для совместимости)
			Rule::Chain(2, TS('i'), NS('M')),	// i M
			Rule::Chain(2, TS('l'), NS('M')),	// l M
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),	// ( E ) M
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),	// i ( W )  (дубликат)
			Rule::Chain(4, TS('z'), TS('('), NS('W'), TS(')')),	// z ( W )  (mightiness)
			Rule::Chain(4, TS('y'), TS('('), NS('W'), TS(')')),	// y ( W )  (consolidate)
			Rule::Chain(4, TS('x'), TS('('), NS('W'), TS(')')),	// x ( W )  (compare_scrolls)
			Rule::Chain(4, TS('n'), TS('('), NS('W'), TS(')'))	// n ( W )  (filament)
		),

		// M - арифметические операции: A E
		Rule(NS('M'), GRB_ERROR_SERIES + 13, 8,					// 613 - ошибка в арифметическом выражении
			Rule::Chain(3, TS('+'), NS('E'), NS('A')),	// A E
			Rule::Chain(3, TS('*'), NS('E'), NS('A')),
			Rule::Chain(3, TS('/'), NS('E'), NS('A')),
			Rule::Chain(3, TS('-'), NS('E'), NS('A')),
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E'))
		),

		// A - арифметические операторы: +, -, *, /, %
		Rule(NS('A'), GRB_ERROR_SERIES + 11, 5,					// 611 - ошибка арифметической операции
			Rule::Chain(1, TS('+')),	// +
			Rule::Chain(1, TS('-')),	// -
			Rule::Chain(1, TS('*')),	// *
			Rule::Chain(1, TS('/')),	// /
			Rule::Chain(1, TS('%'))	// %
		),

		// I - условие: i!i | i!l | i?i | i?l | i>i | i>l | i<i | i<l | i>=i | i>=l | i<=i | i<=l
		Rule(NS('I'), GRB_ERROR_SERIES + 9, 12,					// 609 - ошибка в условии функции/процедуры выражения
			Rule::Chain(3, TS('i'), TS('!'), TS('i')),	// i ! i
			Rule::Chain(3, TS('i'), TS('!'), TS('l')),	// i ! l
			Rule::Chain(3, TS('i'), TS('?'), TS('i')),	// i ? i
			Rule::Chain(3, TS('i'), TS('?'), TS('l')),	// i ? l
			Rule::Chain(3, TS('i'), TS('>'), TS('i')),	// i > i
			Rule::Chain(3, TS('i'), TS('>'), TS('l')),	// i > l
			Rule::Chain(3, TS('i'), TS('<'), TS('i')),	// i < i
			Rule::Chain(3, TS('i'), TS('<'), TS('l')),	// i < l
			Rule::Chain(3, TS('i'), TS('#'), TS('i')),	// i > = i
			Rule::Chain(3, TS('i'), TS('#'), TS('l')),	// i > = l
			Rule::Chain(3, TS('i'), TS('@'), TS('i')),	// i < = i
			Rule::Chain(3, TS('i'), TS('@'), TS('l'))	// i < = l
		),

		// L - битовые операции: i=i&i | i=l&l | i=i|i | i=l|l | i=~i | i=~l | i=i&l | i=l&i | i=i|l | i=l|i
		Rule(NS('L'), GRB_ERROR_SERIES + 12, 10,					// 612 - ошибка начального значения
			Rule::Chain(4, TS('i'), TS('&'), TS('i'), TS(';')),	// i = i & i ;
			Rule::Chain(4, TS('l'), TS('&'), TS('l'), TS(';')),	// i = l & l ;
			Rule::Chain(4, TS('i'), TS('|'), TS('i'), TS(';')),	// i = i | i ;
			Rule::Chain(4, TS('l'), TS('|'), TS('l'), TS(';')),	// i = l | l ;
			Rule::Chain(3, TS('~'), TS('i'), TS(';')),	// i = ~ i ;
			Rule::Chain(3, TS('~'), TS('l'), TS(';')),	// i = ~ l ;
			Rule::Chain(4, TS('i'), TS('&'), TS('l'), TS(';')),	// i = i & l ;
			Rule::Chain(4, TS('l'), TS('&'), TS('i'), TS(';')),	// i = l & i ;
			Rule::Chain(4, TS('i'), TS('|'), TS('l'), TS(';')),	// i = i | l ;
			Rule::Chain(4, TS('l'), TS('|'), TS('i'), TS(';'))	// i = l | i ;
		),

		// X - charge/patrol: c [ I ] { C } b { C } | c [ I ] { C } | w [ I ] { C }
		Rule(NS('X'), GRB_ERROR_SERIES + 7, 3,					// 607 - ошибка для условных конструкций/циклов/выражений
			Rule::Chain(10, TS('['), NS('I'), TS(']'), TS('{'), NS('C'), TS('}'), TS('b'), TS('{'), NS('C'), TS('}')),	// c [ I ] { C } b { C }
			Rule::Chain(6, TS('['), NS('I'), TS(']'), TS('{'), NS('C'), TS('}')),	// c [ I ] { C }
			Rule::Chain(6, TS('['), NS('I'), TS(']'), TS('{'), NS('C'), TS('}'))	// w [ I ] { C }
		),

		// H - council: s [ E ] { R } | s [ E ] { D }
		Rule(NS('H'), GRB_ERROR_SERIES + 2, 2,					// 602 - ошибка в конструкции council-path
			Rule::Chain(6, TS('['), NS('E'), TS(']'), TS('{'), NS('R'), TS('}')),	// s [ E ] { R }
			Rule::Chain(6, TS('['), NS('E'), TS(']'), TS('{'), NS('D'), TS('}'))	// s [ E ] { D }
		),

		// R - council paths: p l { C } R | p l { C } D | p l { C }
		Rule(NS('R'), GRB_ERROR_SERIES + 2, 3,					// 602 - ошибка в конструкции council-path
			Rule::Chain(6, TS('p'), TS('l'), TS('{'), NS('C'), TS('}'), NS('R')),	// p l { C } R
			Rule::Chain(6, TS('p'), TS('l'), TS('{'), NS('C'), TS('}'), NS('D')),	// p l { C } D
			Rule::Chain(5, TS('p'), TS('l'), TS('{'), NS('C'), TS('}'))	// p l { C }
		),

		// D - tiresome: d { C }
		Rule(NS('D'), GRB_ERROR_SERIES + 2, 1,					// 602 - ошибка в конструкции council-path
			Rule::Chain(4, TS('d'), TS('{'), NS('C'), TS('}'))	// d { C }
		)

	);


	// ----------------- Реализация конструкторов правил и цепочек -----------------

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		nt[0] = s;
		if (psize > 1)
		{
			va_list args;
			va_start(args, s);
			for (short i = 1; i < psize; ++i)
				nt[i] = (GRBALPHABET)va_arg(args, int);
			va_end(args);
		}
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		chains[0] = c;
		if (psize > 1)
		{
			va_list args;
			va_start(args, c);
			for (int i = 1; i < size; ++i)
				chains[i] = va_arg(args, Chain);
			va_end(args);
		}
	};


	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		rules[0] = r;
		if (psize > 1)
		{
			va_list args;
			va_start(args, r);
			for (int i = 1; i < size; ++i)
				rules[i] = va_arg(args, Rule);
			va_end(args);
		}
	}

	Greibach getGreibach()
	{
		return greibach;
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			++k;
		if (k < size)
			prule = rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
			++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; ++i)
			b[i] = alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}

} // namespace GRB
