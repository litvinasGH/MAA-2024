#include "Error.h"
namespace Error
{
	// 0   - 99 - системные ошибки
	// 100 - 109 - ошибки параметров
	// 110 - 119 - ошибки открытия и чтения файлов
	// 120 - 129 - лексические ошибки
	// 600 - 610  - синтаксическая ошибка
	// 700 - 710  - семантическая ошибка

	// TODO: добавить в список ошибок
	// 1. Проверить ошибки на работоспособность (проверить, что они выдаются), пометить + или -
	// 2. Добавить в список ошибок


	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "[SYS]: Недопустимый код ошибки"), // код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "[SYS]: Системный сбой"),
		ERROR_ENTRY(2, "[SYS]: Недопустимое количество ошибок"),
		ERROR_ENTRY(3, "[SYS]: "),
		ERROR_ENTRY_NODEF(4),
		ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6),
		ERROR_ENTRY_NODEF(7),
		ERROR_ENTRY_NODEF(8),
		ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10),
		ERROR_ENTRY_NODEF10(20),
		ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70),
		ERROR_ENTRY_NODEF10(80),
		ERROR_ENTRY(91, "Лексический анализатор: строковый литерал превышает допустимый размер"),
		ERROR_ENTRY(92, "Лексический анализатор: строковый литерал не распознан"),
		ERROR_ENTRY(93, "Лексический анализатор: идентификатор не распознан"),
		ERROR_ENTRY(94, "Лексический анализатор: найдено несколько функций main"),
		ERROR_ENTRY(95, "Лексический анализатор: функция объявлена несколько раз"),
		ERROR_ENTRY(96, "Лексический анализатор: переменная объявлена несколько раз"),
		ERROR_ENTRY(97, "Лексический анализатор: неизвестная переменная"),
		ERROR_ENTRY(98, "Лексический анализатор: не найдена точка входа(main)"),
		ERROR_ENTRY(99, "Лексический анализатор: ключевое слово используется как индификатор"),

		ERROR_ENTRY(100, "[PARM]: Параметр -in должен быть задан"), // +
		ERROR_ENTRY_NODEF(101),
		ERROR_ENTRY_NODEF(102),
		ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "[PARM]: Превышена длина входного параметра"), // +
		ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106),
		ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),
		ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "[IN]: Ошибка при открытии файла с исходным кодом (-in)"), // +
		ERROR_ENTRY(111, "[IN]: Недопустимый символ в исходном файле (-in)"), // +
		ERROR_ENTRY(112, "[IN]: Превышен предел размера входного файла (-in)"), // +
		ERROR_ENTRY(113, "[IN]: Превышен предел размера лексемы"), // +
		ERROR_ENTRY(114, "[IN]: Отсуствует закрывающая кавычка"), // +
		ERROR_ENTRY(115, "[IN]: Входная строка превышает максимальный размер"), // +
		ERROR_ENTRY(116, "[IN]: Количество открывающих фигурных скобок не совпадает с количеством закрывающих"), // +
		ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120, "[LT]: Ошибка при разборе токена"), // +
		ERROR_ENTRY(121, "[LT]: Используется необъявленный идентификатор"), // +
		ERROR_ENTRY(122, "[LT]: Идентификатор не имеет типа"),
		ERROR_ENTRY_NODEF(123),
		ERROR_ENTRY(124, "[LT]: Отсутствует точка входа"), // +
		ERROR_ENTRY(125, "[LT]: Обнаружена вторая точка входа"),
		ERROR_ENTRY_NODEF(126),
		ERROR_ENTRY_NODEF(127),
		ERROR_ENTRY_NODEF(128),
		ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY(130,"[Синтаксис]: Неправильная структура программы"),
		ERROR_ENTRY(131,"[Синтаксис]: Неправильный идентификатор"),
		ERROR_ENTRY(132,"[Синтаксис]: Произошла ошибка в параметрах функции"),
		ERROR_ENTRY(133,"[Синтаксис]: Произошла ошибка в параметрах функции, которая в данный момент вызывается."),
		ERROR_ENTRY(134,"[Синтаксис]: Формула в параметрах вычисляемой функции"),
		ERROR_ENTRY_NODEF(135),
		ERROR_ENTRY_NODEF(136),
		ERROR_ENTRY_NODEF(137),
		ERROR_ENTRY_NODEF(138),
		ERROR_ENTRY_NODEF(139),
		ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180),
		ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY_NODEF100(300),
		ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "[Синтаксис]: Неверная структура программы"),
		ERROR_ENTRY(601, "[Синтаксис]: Ошибочный оператор"),
		ERROR_ENTRY(602, "[Синтаксис]: Ошибка в выражении"),
		ERROR_ENTRY(603, "[Синтаксис]: Ошибка в параметрах функции"),
		ERROR_ENTRY(604, "[Синтаксис]: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(605, "[Синтаксис]: Ошибка знака в выражении"),
		ERROR_ENTRY(606, "[Синтаксис]: Ошибка синтаксического анализа"),
		ERROR_ENTRY_NODEF(607),
		ERROR_ENTRY(608, "[Синтаксис]: Обнаружена синтаксическая ошибка (смотри журнал Log)"),
		ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610),
		ERROR_ENTRY_NODEF10(620),
		ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660),
		ERROR_ENTRY_NODEF10(670),
		ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY(700, "[Семантика]: Повторное объявление идентификатора"),
		ERROR_ENTRY(701, "[Семантика]: Ошибка в возвращаемом значении"),
		ERROR_ENTRY(702, "[Семантика]: Ошибка в передаваемых значениях в функции: количество параметров не совпадает"),
		ERROR_ENTRY(703, "[Семантика]: Ошибка в передаваемых значениях в функции: типы параметров не совпадают"),
		ERROR_ENTRY(704, "[Семантика]: Нарушены типы данных в выражении"),
		ERROR_ENTRY(705, "[Семантика]: Ошибка экспорта: в библиотеке нет такой функции"),
		ERROR_ENTRY(706, "[Семантика]: Ошибка экспорта: неверные параметры"),
		ERROR_ENTRY(707, "[Семантика]: Ошибка экспорта: ошибочный тип возвращаемого значения"),
		ERROR_ENTRY(708, "[Семантика]: Ошибочный оператор: неизвестный идентификатор"),
		ERROR_ENTRY_NODEF(709),
		ERROR_ENTRY_NODEF10(710),
		ERROR_ENTRY_NODEF10(720),ERROR_ENTRY_NODEF10(730),ERROR_ENTRY_NODEF10(740),
		ERROR_ENTRY_NODEF10(750),ERROR_ENTRY_NODEF10(760),ERROR_ENTRY_NODEF10(770),ERROR_ENTRY_NODEF10(780),
		ERROR_ENTRY_NODEF10(790),
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id)
	{
		ERROR out;

		if (0 > id || id > ERROR_MAX_ENTRY)
			out = errors[0];
		else
			out = errors[id];
		return out;
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR out;

		if (0 > id || id > ERROR_MAX_ENTRY)
			out = errors[0];
		else
			out = errors[id];
		out.inext.line = line;
		out.inext.col = col;
		out.inext.text = (unsigned char*)"";
		return out;
	}
}