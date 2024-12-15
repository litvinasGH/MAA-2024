#pragma once
#define ID_MAXSIZE 6
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255

namespace IT
{										    
	enum IDDATATYPE { INT = 1, STR = 2 , BOOL = 3};  // Типы данных: INT (целое число) и STR (строка)
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, I = 5, LE = 6};  // Типы идентификаторов: V (переменная), F (функция), P (параметр), L (локальная переменная) , название идентиффикатора (Лексема)

	struct Entry
	{
		char* value;
		int			idxfirstLE;
		char* id;
		IDDATATYPE	iddatatype;
		IDTYPE		idtype;
			int		vint;
			struct
			{
				int len;
				char* str;
			}	vstr;
			

	};

	struct IdTable  // Структура таблицы идентификаторов
	{
		int maxsize;  // Максимальное количество элементов в таблице < TI_MAXSIZE
		int size;  // Текущий размер таблицы < maxsize
		Entry* table;  // Массив записей таблицы идентификаторов
	};

	IdTable CreateT(  // Создать таблицу идентификаторов
		int size  // Максимальное количество элементов в таблице < TI_MAXSIZE
	);

	

	void Add(  // Добавить запись в таблицу идентификаторов
		IdTable& idtable,  // Структура таблицы идентификаторов
		IT::Entry entry  // Запись, которую необходимо добавить
	);

	int search(  // Поиск записи в таблице идентификаторов
		IdTable& idtable,
		IT::Entry entry
	);

	Entry GetEntry(  // Получить запись из таблицы идентификаторов
		IdTable& idtable,  // Структура таблицы идентификаторов
		int n  // Индекс идентификатора
	);

	void Delete(IdTable& idtable);  // Удалить таблицу идентификаторов (освободить память)
}