#pragma once
#include <iostream>
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
using namespace LT;
using namespace IT;

namespace FST
{
	struct RELATION			//реьро:символ -> вершина графа переходов КА
	{
		char symbol;		//символ перехода
		short nnode;		//номер смежной вершины
		RELATION(
			unsigned char c = 0x00,
			short ns = NULL
		);
	};


	struct NODE // вершина графа переходов
	{
		short n_relation;	 //Кол-во ицидентных рёбер
		RELATION* relations;   //Инцидентные рёбра
		NODE();
		NODE(
			short n,	 //Кол-во рёбер
			RELATION rel, ... //Списко рёбер
		);
	};


	struct FST	//конечный автомат
	{
		const char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		FST(
			const char* s,
			short ns,
			NODE n, ...
		);
	};


	bool execute(
		FST& fst
	);
	void CheckL(In::IN file, LexTable& lextable, IdTable& table);
}