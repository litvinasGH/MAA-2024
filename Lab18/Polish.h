#pragma once
#include "stdafx.h"
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "Error.h"

namespace Polish {
	void startPolish(LT::LexTable& lextable, IT::IdTable& idtable);
	bool polishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable);
}