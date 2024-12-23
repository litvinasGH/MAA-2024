#pragma once
#include <stack>
#define ID_MAXSIZE 6
#define TI_MAXSIZE 4096
#define TI_INT_DEFAULT 0x00000000
#define TI_STR_DEFAULT 0x00
#define TI_NULLIDX 0xffffffff
#define TI_STR_MAXSIZE 255

namespace IT
{										    
	enum IDDATATYPE { INT = 1, STR = 2 , BOOL = 3};  // ���� ������: INT (����� �����) � STR (������)
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, I = 5};  // ���� ���������������: V (����������), F (�������), P (��������), L (��������� ����������) , �������� ��������������� (�������)

	struct Entry
	{
		std::stack<char*>view;

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
			bool vbool;
			bool flag = false;

	};

	struct IdTable  // ��������� ������� ���������������
	{
		int maxsize;  // ������������ ���������� ��������� � ������� < TI_MAXSIZE
		int size;  // ������� ������ ������� < maxsize
		Entry* table;  // ������ ������� ������� ���������������
	};

	IdTable CreateT(  // ������� ������� ���������������
		int size  // ������������ ���������� ��������� � ������� < TI_MAXSIZE
	);

	

	void Add(  // �������� ������ � ������� ���������������
		IdTable& idtable,  // ��������� ������� ���������������
		IT::Entry entry  // ������, ������� ���������� ��������
	);

	int search(  // ����� ������ � ������� ���������������
		IdTable& idtable,
		IT::Entry entry
	);

	Entry GetEntry(  // �������� ������ �� ������� ���������������
		IdTable& idtable,  // ��������� ������� ���������������
		int n  // ������ ��������������
	);

	void Delete(IdTable& idtable);  // ������� ������� ��������������� (���������� ������)
}