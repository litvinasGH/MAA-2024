#include <iostream>
#include "Error.h"
#include "IT.h"




namespace IT
{
	IdTable CreateT(int size)
	{

		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(116);
		}
		IdTable table;
		table.maxsize = size;
		table.size = 0;
		table.table = new Entry[size];
		return table;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size + 1 > idtable.maxsize)
			throw ERROR_THROW(117);

		idtable.table[idtable.size] = entry;
		idtable.size += 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	void Delete(IdTable& idtable)
	{
		delete[]idtable.table;
		idtable.table = nullptr;
	}
}