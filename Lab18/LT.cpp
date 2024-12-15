#include "stdafx.h"
#include <iostream>
#include <cwchar>
#include <time.h>
#include "Error.h"		
#include "Parm.h"		
#include "Log.h"		
#include "In.h"	
#include "Out.h"
#include "LT.h"
#include "FST.h"


using namespace std;


namespace LT
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
		{
			throw ERROR_THROW(114);
		}
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}
	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size > LT_MAXSIZE)
		{
			throw ERROR_THROW(115);
		}
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}
	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n > LT_MAXSIZE || n < 0)
		{
			throw ERROR_THROW(13);
		}
		return lextable.table[n];
	}
	void Delete(LexTable& lextable)
	{
		delete lextable.table;
		lextable.size = 0;
		lextable.maxsize = 0;
	}
	
}


