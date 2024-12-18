#pragma once

#include "Error.h"
#include "In.h"
#include "IT.h" 
#include "LT.h"

namespace LEX
{
	struct LEX
	{
		IT::IdTable  idTable;
		LT::LexTable lextable;

		LEX(
			LT::LexTable _lextable, IT::IdTable  _idtable) {
			idTable = _idtable;
			lextable = _lextable;
		}
		LEX() {}
	};
}