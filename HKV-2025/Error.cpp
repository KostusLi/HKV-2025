#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "{ SYSTEM FALLACY }: Недопустимый код ошибки"),
		ERROR_ENTRY(1, "{ SYSTEM FALLACY }: Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "{ SYSTEM FALLACY }: Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "{ SYSTEM FALLACY }: Превышена длина входного параметра"),
		ERROR_ENTRY(102, "{ SYSTEM FALLACY }: Ошибка при открытии файла с исходным кодом(-in)"),
		ERROR_ENTRY(103, "{ SYSTEM FALLACY }: Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "{ LEXICAL FALLACY }: Недопустимый символ в исходном файле(-in)"),
		ERROR_ENTRY(201, "{ LEXICAL FALLACY }: Неизвестная последовательность символов"),
		ERROR_ENTRY(202, "{ LEXICAL FALLACY }: Превышен размер таблицы лексем"),
		ERROR_ENTRY(203, "{ LEXICAL FALLACY }: Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(204, "{ LEXICAL FALLACY }: Превышена допустимая длина идентификатора"),
		ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "{ SEMANTIC FALLACY }: Необъявленный идентификатор"),
		ERROR_ENTRY(301, "{ SEMANTIC FALLACY }: Отсутствует точка входа main"),
		ERROR_ENTRY(302, "{ SEMANTIC FALLACY }: Обнаружено несколько точек входа main"),
		ERROR_ENTRY(303, "{ SEMANTIC FALLACY }: В объявлении не указан тип идентификатора"),
		ERROR_ENTRY(304, "{ SEMANTIC FALLACY }: В объявлении отсутствует ключевое слово elder"),
		ERROR_ENTRY(305, "{ SEMANTIC FALLACY }: Попытка переопределения идентификатора"),
		ERROR_ENTRY(306, "{ SEMANTIC FALLACY }: Превышено максимальное количество параметров функции"),
		ERROR_ENTRY(307, "{ SEMANTIC FALLACY }: Слишком много параметров в вызове"),
		ERROR_ENTRY(308, "{ SEMANTIC FALLACY }: Кол-во ожидаемыех функцией и передаваемых параметров не совпадают"),
		ERROR_ENTRY(309, "{ SEMANTIC FALLACY }: Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(310, "{ SEMANTIC FALLACY }: Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(311, "{ SEMANTIC FALLACY }: Обнаружен символ \'\"\'. Возможно, не закрыт строковый литерал"),
		ERROR_ENTRY(312, "{ SEMANTIC FALLACY }: Превышен размер строкового литерала"),
		ERROR_ENTRY(313, "{ SEMANTIC FALLACY }: Недопустимый целочисленный литерал"),
		ERROR_ENTRY(314, "{ SEMANTIC FALLACY }: Типы данных в выражении не совпадают"),
		ERROR_ENTRY(315, "{ SEMANTIC FALLACY }: Тип функции и возвращаемого значения не совпадают"),
		ERROR_ENTRY(316, "{ SEMANTIC FALLACY }: Недопустимое строковое выражение справа от знака \'=\'"),
		ERROR_ENTRY(317, "{ SEMANTIC FALLACY }: Неверное условное выражение"),
		ERROR_ENTRY(318, "{ SEMANTIC FALLACY }: Деление на ноль"),
		ERROR_ENTRY(319, "{ SEMANTIC FALLACY }: Превышен максимальный размер токина"),
		ERROR_ENTRY(320, "{ SEMANTIC FALLACY }: выражение в council должно быть squire/rune/scroll"),
		ERROR_ENTRY(321, "{ SEMANTIC FALLACY }: path/tiresome должны иметь тип такой же, как и в council"),
		ERROR_ENTRY_NODEF(322), ERROR_ENTRY_NODEF(323), ERROR_ENTRY_NODEF(324), ERROR_ENTRY_NODEF(325),
		ERROR_ENTRY_NODEF(326), ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328), ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "{ SYNTAX FALLACY }: Неверная структура программы"),
		ERROR_ENTRY(601, "{ SYNTAX FALLACY }: Не найден список параметров функции"),
		ERROR_ENTRY(602, "{ SYNTAX FALLACY }: Ошибка в теле функции"),
		ERROR_ENTRY(603, "{ SYNTAX FALLACY }: Ошибка в теле процедуры"),
		ERROR_ENTRY(604, "{ SYNTAX FALLACY }: Ошибка в списке параметров функции"),
		ERROR_ENTRY(605, "{ SYNTAX FALLACY }: Ошибка в вызове функции/выражении"),
		ERROR_ENTRY(606, "{ SYNTAX FALLACY }: Ошибка в списке фактических параметров функции"),
		ERROR_ENTRY(607, "{ SYNTAX FALLACY }: Ошибка при констуировании цикла/условного выражения"),
		ERROR_ENTRY(608, "{ SYNTAX FALLACY }: Ошибка в теле цикла/условного выражения"),
		ERROR_ENTRY(609, "{ SYNTAX FALLACY }: Ошибка в условии цикла/условного выражения"),
		ERROR_ENTRY(610, "{ SYNTAX FALLACY }: Неверный условный оператор"),
		ERROR_ENTRY(611, "{ SYNTAX FALLACY }: Неверный арифметический оператор"),
		ERROR_ENTRY(612, "{ SYNTAX FALLACY }: Неверное выражение. Ожидаются только идентификаторы/литералы"),
		ERROR_ENTRY(613, "{ SYNTAX FALLACY }: Ошибка в арифметическом выражении"),
		ERROR_ENTRY(614, "{ SYNTAX FALLACY }: Недопустимая синтаксическая конструкция"),
		ERROR_ENTRY(615, "{ SYNTAX FALLACY }: Недопустимая синтаксическая конструкция в теле цикла/условного выражения"),
		ERROR_ENTRY(616, "{ SYNTAX FALLACY }: Ошибка в контрукции council-path"),
		ERROR_ENTRY_NODEF(617),ERROR_ENTRY_NODEF(618), ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};


	ERROR GetError(int id)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		else
			return errors[id];
	}

	ERROR GetError(int id, int line, int col)
	{
		ERROR err = GetError(id);
		err.position.col = col;
		err.position.line = line;
		return err;
	}
}