#include "stdafx.h"
#define GRB_ERROR_SERIES 600

typedef short GRBALPHABET;

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 17,

		// S - Стартовый символ (temple)
		// Ошибка 600: Нарушена структура программы
		Rule(NS('S'), GRB_ERROR_SERIES + 0, 3,
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')), // Функция (action)
			Rule::Chain(6, TS('p'), TS('f'), TS('i'), NS('P'), NS('G'), NS('S')), // Процедура (hollow action)
			Rule::Chain(4, TS('m'), TS('['), NS('K'), TS(']'))                    // Главный вход (temple)
		),

		// T - Тело функции (возвращает значение)
		// Ошибка 602: Нарушена целостность тела функции
		Rule(NS('T'), GRB_ERROR_SERIES + 2, 2,
			Rule::Chain(5, TS('['), TS('e'), NS('V'), TS(';'), TS(']')),          // return value;
			Rule::Chain(6, TS('['), NS('K'), TS('e'), NS('V'), TS(';'), TS(']'))  // code... return value;
		),

		// G - Тело процедуры (void)
		// Ошибка 603: Нарушена целостность тела процедуры
		Rule(NS('G'), GRB_ERROR_SERIES + 3, 2,
			Rule::Chain(4, TS('['), TS('e'), TS(';'), TS(']')),                   // return;
			Rule::Chain(5, TS('['), NS('K'), TS('e'), TS(';'), TS(']'))           // code... return;
		),

		// P - Параметры функций
		// Ошибка 601: Не найден список параметров
		Rule(NS('P'), GRB_ERROR_SERIES + 1, 2,
			Rule::Chain(3, TS('('), NS('E'), TS(')')), // (param1, param2)
			Rule::Chain(2, TS('('), TS(')'))           // ()
		),

		// E - Перечисление параметров (внутри P)
		// Ошибка 604: Ошибка в списке параметров
		Rule(NS('E'), GRB_ERROR_SERIES + 4, 2,
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E')), // type id, ...
			Rule::Chain(2, TS('t'), TS('i'))                    // type id
		),

		// F - Вызов функции (аргументы)
		// Ошибка 605: Ошибка при вызове функции
		Rule(NS('F'), GRB_ERROR_SERIES + 5, 2,
			Rule::Chain(3, TS('('), NS('N'), TS(')')), // (arg1, arg2)
			Rule::Chain(2, TS('('), TS(')'))           // ()
		),

		// N - Перечисление аргументов (внутри F)
		// Ошибка 606: Ошибка в списке аргументов
		Rule(NS('N'), GRB_ERROR_SERIES + 6, 4,
			Rule::Chain(1, TS('i')),                   // id
			Rule::Chain(1, TS('l')),                   // literal
			Rule::Chain(3, TS('i'), TS(','), NS('N')), // id, ...
			Rule::Chain(3, TS('l'), TS(','), NS('N'))  // literal, ...
		),

		// K - Основной блок кода (Statement List)
		// Ошибка 614: Недопустимая синтаксическая форма (общая ошибка оператора)
		Rule(NS('K'), GRB_ERROR_SERIES + 14, 16,
			// Объявление и инициализация: elder type id = expr;
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('W'), TS(';')),

			// Объявление без инициализации: elder type id;
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('K')),
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),

			// Присваивание: id = expr;
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('K')),
			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),

			// Вывод: confession ...;
			Rule::Chain(4, TS('o'), NS('V'), TS(';'), NS('K')),
			Rule::Chain(3, TS('o'), NS('V'), TS(';')),

			// Новая строка: newleaf;
			Rule::Chain(3, TS('^'), TS(';'), NS('K')),
			Rule::Chain(2, TS('^'), TS(';')),

			// Управляющие конструкции: check ... # ...
			// Ошибка 607 (в R) или 609 (в Z)
			Rule::Chain(5, TS('?'), NS('Z'), TS('#'), NS('R'), NS('K')),
			Rule::Chain(4, TS('?'), NS('Z'), TS('#'), NS('R')),

			// Вызов функции как процедура: func();
			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('K')),
			Rule::Chain(3, TS('i'), NS('F'), TS(';')),

			// Council (switch)
			Rule::Chain(8, TS('q'), TS('('), NS('W'), TS(')'), TS('['), NS('J'), TS(']'), NS('K')),
			Rule::Chain(7, TS('q'), TS('('), NS('W'), TS(')'), TS('['), NS('J'), TS(']'))
		),

		// R - Ветвления и Циклы (продолжение после check condition #)
		// Ошибка 607: Ошибка в построении Патруля или Атаки
		Rule(NS('R'), GRB_ERROR_SERIES + 7, 3,
			// IF-ELSE: charge [...] backup [...] #
			Rule::Chain(5, TS('w'), NS('Y'), TS('r'), NS('Y'), TS('#')),

			// IF (без else): charge [...] # (если нужно, раскомментируй)
			 Rule::Chain(3, TS('w'), NS('Y'), TS('#')), 

			// WHILE: patrol [...] #
			Rule::Chain(3, TS('c'), NS('Y'), TS('#'))
		),

		// Y - Блок кода в фигурных скобках (scope)
		// Ошибка 608: Нарушение в теле Патруля или Атаки
		Rule(NS('Y'), GRB_ERROR_SERIES + 8, 1,
			Rule::Chain(3, TS('['), NS('K'), TS(']'))
		),

		// Z - Условие сравнения
		// Ошибка 609: Неверное условие
		// ТЕПЕРЬ ПОДДЕРЖИВАЕТ ВЫРАЖЕНИЯ: (a+b) > (c*2)
		Rule(NS('Z'), GRB_ERROR_SERIES + 9, 2,
			Rule::Chain(3, TS('i'), NS('L'), NS('W')),
			Rule::Chain(3, TS('l'), NS('L'), NS('W'))
		),

		// L - Операторы сравнения
		// Ошибка 610: Неверный оператор сравнения
		Rule(NS('L'), GRB_ERROR_SERIES + 10, 6,
			Rule::Chain(1, TS('<')),
			Rule::Chain(1, TS('>')),
			Rule::Chain(1, TS('&')), // ==
			Rule::Chain(1, TS('!')), // !=
			Rule::Chain(1, TS('~')), // >=
			Rule::Chain(1, TS('@'))  // <=
		),

		// W - Выражения (Expression)
		// Ошибка 613: Ошибка в арифметическом выражении
		Rule(NS('W'), GRB_ERROR_SERIES + 13, 9,
			// Приоритет: сначала сложные, чтобы жадный алгоритм пытался взять больше
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('A'), NS('W')), // (W) op W
			Rule::Chain(3, TS('('), NS('W'), TS(')')),                   // (W)

			Rule::Chain(3, TS('i'), NS('A'), NS('W')), // id op W
			Rule::Chain(3, TS('l'), NS('A'), NS('W')), // lit op W
			Rule::Chain(4, TS('i'), NS('F'), NS('A'), NS('W')), // func() op W

			Rule::Chain(2, TS('i'), NS('F')),          // func()

			Rule::Chain(2, TS('j'), NS('W')),          // bitnot W

			Rule::Chain(1, TS('i')),                   // id
			Rule::Chain(1, TS('l'))                    // lit
		),

		// A - Арифметические операторы
		// Ошибка 611: Неверный арифметический оператор
		Rule(NS('A'), GRB_ERROR_SERIES + 11, 7,
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('/')),
			Rule::Chain(1, TS('%')),
			Rule::Chain(1, TS('z')), // bitand
			Rule::Chain(1, TS('x'))  // bitor
		),

		// V - Операнд для вывода (confession)
		// Ошибка 612: Ожидается Род или Литерал
		Rule(NS('V'), GRB_ERROR_SERIES + 12, 3,
			Rule::Chain(1, TS('l')), // literal
			Rule::Chain(1, TS('i')), // id
			Rule::Chain(1, TS('k'))  // ??? возможно octal literal, если он отдельным токеном
		),

		// J - Тело Council (switch cases)
		// Ошибка 616: Ошибка в конструкции council-path
		Rule(NS('J'), GRB_ERROR_SERIES + 16, 6,
			// path val [ code ] break; J (рекурсия)
			Rule::Chain(8, TS('v'), NS('V'), TS('['), NS('K'), TS('g'), TS(';'), TS(']'), NS('J')),
			// path val [ code ] break; (конец)
			Rule::Chain(7, TS('v'), NS('V'), TS('['), NS('K'), TS('g'), TS(';'), TS(']')),

			Rule::Chain(6, TS('v'), NS('V'), TS('['), NS('K'), TS(']'), NS('J')),
			// path val [ code ] break; (конец)
			Rule::Chain(5, TS('v'), NS('V'), TS('['), NS('K'), TS(']')),

			// tiresome [ code ] break; (default, рекурсия если стоит не последним, но обычно default последний)
			Rule::Chain(7, TS('d'), TS('['), NS('K'), TS('g'), TS(';'), TS(']'), NS('J')),
			Rule::Chain(6, TS('d'), TS('['), NS('K'), TS('g'), TS(';'), TS(']'))
		)
	);

	// Реализация методов GRB (остается старой, только конструкторы)
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; ++i)
			chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; ++i)
			rules[i] = p[i];
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
}