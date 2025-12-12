#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "{ КРАХ СИСТЕМЫ }: Недопустимый код ошибки в Кодексе"),
		ERROR_ENTRY(1, "{ КРАХ СИСТЕМЫ }: Нарушение Системного Устава (Сбой)"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "{ КРАХ СИСТЕМЫ }: Отсутствует Указ (-in) на Вход"),
		ERROR_ENTRY(101, "{ КРАХ СИСТЕМЫ }: Длина Входного Указа превысила Дозволенную"),
		ERROR_ENTRY(102, "{ КРАХ СИСТЕМЫ }: Невозможно открыть Свиток Исходного Кода (-in)"),
		ERROR_ENTRY(103, "{ КРАХ СИСТЕМЫ }: Невозможно создать Свиток Летописи (-log)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY(200, "{ НЕУДАЧА РАЗВЕДКИ }: Недопустимый Символ в Исходном Свитке"),
		ERROR_ENTRY(201, "{ НЕУДАЧА РАЗВЕДКИ }: Неизвестная Последовательность Знаков (Лексем)"),
		ERROR_ENTRY(202, "{ НЕУДАЧА РАЗВЕДКИ }: Переполнение Хранилища Лексем"),
		ERROR_ENTRY(203, "{ НЕУДАЧА РАЗВЕДКИ }: Переполнение Таблицы Родов (Идентификаторов)"),
		ERROR_ENTRY(204, "{ НЕУДАЧА РАЗВЕДКИ }: Имя Рода (Идентификатор) слишком длинное"),
		ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),

		ERROR_ENTRY(300, "{ КРАХ СОВЕТА }: Род (Идентификатор) не был Объявлен"),
		ERROR_ENTRY(301, "{ КРАХ СОВЕТА }: Отсутствует Главный Храм (Точка Входа temple)"),
		ERROR_ENTRY(302, "{ КРАХ СОВЕТА }: Обнаружено несколько Главных Храмов (Точек Входа)"),
		ERROR_ENTRY(303, "{ КРАХ СОВЕТА }: В Обнародовании (Объявлении) не указан Тип Рода/Действия"),
		ERROR_ENTRY(304, "{ КРАХ СОВЕТА }: Отсутствует Ключевое Слово elder в Обнародовании"),
		ERROR_ENTRY(305, "{ КРАХ СОВЕТА }: Попытка Переобнародовать (Переопределить) Род"),
		ERROR_ENTRY(306, "{ КРАХ СОВЕТА }: Превышен Лимит Даров (Параметров) для Служения (Функции)"),
		ERROR_ENTRY(307, "{ КРАХ СОВЕТА }: Слишком много Даров при Призыве (Вызове) Служения"),
		ERROR_ENTRY(308, "{ КРАХ СОВЕТА }: Число ожидаемых Даров не соответствует Переданным"),
		ERROR_ENTRY(309, "{ КРАХ СОВЕТА }: Несовпадение Типов Переданных Даров"),
		ERROR_ENTRY(310, "{ КРАХ СОВЕТА }: Недопустимо использовать Пустой Свиток (Строковый Литерал)"),
		ERROR_ENTRY(311, "{ КРАХ СОВЕТА }: Обнаружен Символ \'\"\'. Вероятно, Свиток не был Закрыт"),
		ERROR_ENTRY(312, "{ КРАХ СОВЕТА }: Длина Свитка (Строкового Литерала) Превышена"),
		ERROR_ENTRY(313, "{ КРАХ СОВЕТА }: Недопустимое Значение squire (Целочисленный Литерал)"),
		ERROR_ENTRY(314, "{ КРАХ СОВЕТА }: Типы Родов в Выражении не Совместимы"),
		ERROR_ENTRY(315, "{ КРАХ СОВЕТА }: Тип Возвращаемого (comeback) Значения не соответствует Типу Служения (action)"),
		ERROR_ENTRY(316, "{ КРАХ СОВЕТА }: Недопустимый Свиток (Строковое Выражение) справа от Присвоения \'=\'"),
		ERROR_ENTRY(317, "{ КРАХ СОВЕТА }: Неверное Условие (check) для Ветвления"),
		ERROR_ENTRY(318, "{ КРАХ СОВЕТА }: Деление на Пустоту (Деление на Ноль)"),
		ERROR_ENTRY(319, "{ КРАХ СОВЕТА }: Превышен максимальный размер Жетона (Руны)"),
		ERROR_ENTRY(320, "{ КРАХ СОВЕТА }: Выражение в council должно быть squire/rune/scroll"),
		ERROR_ENTRY(321, "{ КРАХ СОВЕТА }: path/tiresome должны иметь тот же Тип, что и Выражение в council"),
		ERROR_ENTRY(322, "{ КРАХ СОВЕТА }: tiresome должен остаться ОДИН"),
		ERROR_ENTRY(323, "{ КРАХ СОВЕТА }: В объявлении функции нет слова action"),
		ERROR_ENTRY_NODEF(324), ERROR_ENTRY_NODEF(325),
		ERROR_ENTRY_NODEF(326), ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328), ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "{ ПАДЕНИЕ ХРАМА }: Нарушена Общая Структура Программы"),
		ERROR_ENTRY(601, "{ ПАДЕНИЕ ХРАМА }: Не найден Список Даров (Параметров) Служения"),
		ERROR_ENTRY(602, "{ ПАДЕНИЕ ХРАМА }: Нарушена Целостность Тела Служения (Функции)"),
		ERROR_ENTRY(603, "{ ПАДЕНИЕ ХРАМА }: Нарушена Целостность Тела Процедуры"),
		ERROR_ENTRY(604, "{ ПАДЕНИЕ ХРАМА }: Нарушение в Списке Даров Служения"),
		ERROR_ENTRY(605, "{ ПАДЕНИЕ ХРАМА }: Ошибка при Призыве Служения (Вызове Функции/Выражении)"),
		ERROR_ENTRY(606, "{ ПАДЕНИЕ ХРАМА }: Ошибка в Списке Фактических Даров Служения"),
		ERROR_ENTRY(607, "{ ПАДЕНИЕ ХРАМА }: Ошибка в Построении Патруля (Цикла) или Атаки (check)"),
		ERROR_ENTRY(608, "{ ПАДЕНИЕ ХРАМА }: Нарушение в Теле Патруля или Атаки"),
		ERROR_ENTRY(609, "{ ПАДЕНИЕ ХРАМА }: Неверное Условие (check) для Патруля или Атаки"),
		ERROR_ENTRY(610, "{ ПАДЕНИЕ ХРАМА }: Неверный Условный Оператор"),
		ERROR_ENTRY(611, "{ ПАДЕНИЕ ХРАМА }: Неверный Арифметический Приказ (Оператор)"),
		ERROR_ENTRY(612, "{ ПАДЕНИЕ ХРАМА }: Неверное Выражение. Ожидается Род или Литерал"),
		ERROR_ENTRY(613, "{ ПАДЕНИЕ ХРАМА }: Ошибка в Арифметическом Выражении"),
		ERROR_ENTRY(614, "{ ПАДЕНИЕ ХРАМА }: Недопустимая Синтаксическая Форма"),
		ERROR_ENTRY(615, "{ ПАДЕНИЕ ХРАМА }: Недопустимая Форма в Теле Патруля или Атаки"),
		ERROR_ENTRY(616, "{ ПАДЕНИЕ ХРАМА }: Ошибка в Конструкции council-path (Совет-Путь)"),
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