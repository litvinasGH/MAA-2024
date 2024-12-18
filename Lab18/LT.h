#pragma once
#include "In.h"
#include "stdafx.h"
#define LEXEMA_FIXSIZE 1
#define LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_TYPE 't'
#define LEX_ID		'i'
#define LEX_MAIN	'm'
#define LEX_LITERAL	'l'
#define LEX_FUNCTION 'f'
#define LEX_RETURN	'r'
#define LEX_CONNECT	'j'
#define LEX_LIB		'y'
#define LEX_WRITE 'w'
#define LEX_PRINT	'p'
#define LEX_CYCLE	'c'
#define LEX_SEMICOLON ';'
#define LEX_COMMA	','
#define LEX_LEFTBRACE '{'
#define LEX_RIGHTBRACE	'}'
#define LEX_BRACELET '}'
#define LEX_LEFTHESIS	'('
#define LEX_RIGHTHESIS	')'
#define LEX_LEFT		'<<'
#define LEX_RIGHT		'>>'
#define LEX_BOOL_OPERATOR 'b'
#define EMPTY_DATA		'\0'
#define LEX_READ 'w'
#define LEX_SHIFT_LEFT '<'
#define LEX_SHIFT_RIGHT '>'
#define LEX_EQUAL '='
#define LEX_OPERATOR 'h'

namespace LT
{
	
	struct Entry
	{
		char lexema;
		int sn;	//номер строки в исхоном тексте
		int idxTI;
		char* value = (char*)""; //значение лексемы
	};
	struct LexTable
	{
		int maxsize;
		int size;

		Entry* table;
	};


	


	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
}