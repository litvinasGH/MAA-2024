#pragma once
#include <iostream>
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
using namespace LT;
using namespace IT;

namespace FST
{
	struct RELATION			//�����:������ -> ������� ����� ��������� ��
	{
		char symbol;		//������ ��������
		short nnode;		//����� ������� �������
		RELATION(
			unsigned char c = 0x00,
			short ns = NULL
		);
	};


	struct NODE // ������� ����� ���������
	{
		short n_relation;	 //���-�� ���������� ����
		RELATION* relations;   //����������� ����
		NODE();
		NODE(
			short n,	 //���-�� ����
			RELATION rel, ... //������ ����
		);
	};


	struct FST	//�������� �������
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