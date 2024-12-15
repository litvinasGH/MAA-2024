#pragma once
#include "In.h"
#define LEXEMA_FIXSIZE 1
#define LT_MAXSIZE 4096
#define LT_TI_NULLIDX 0XFFFFFFFF
#define LEX_UINTEGER 't'
#define LEX_BOOL	't'
#define LEX_ID		'i'
#define LEX_MAIN	'm'
#define LEX_LITERAL	'l'
#define LEX_FUNCTION 'f'
#define LEX_RETURN	'r'
#define LEX_CONNECT	'j'
#define LEX_LIB 'y'
#define LEX_PRINT	'p'
#define LEX_CYCLE	'u'
#define LEX_SEMICOLON ';'
#define LEX_COMMA	','
#define LEX_LEFTBRACE '{'
#define LEX_RIGHTBRACE	'}'
#define LEX_LEFTHESIS	'('
#define LEX_RIGHTHESIS	')'
#define LEX_LEFT		'v'
#define LEX_RIGHT		'v'
#define LEX_BOOL_OPERATOR 'b'
#define EMPTY_DATA		'\0'




namespace LT
{
	struct Entry
	{
		char lexema;
		int sn;	//номер строки в исхоном тексте
		int idxTI;
		char* value = (char*)""; //значение лексемы
		int priority;
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