#include "FST.h"
#include <iostream>
#include <stdarg.h>
#include "stdafx.h"
#include <cstring>
#include <stack>
#define _CRT_SECURE_NO_WARNINGS
using namespace LT;
using namespace IT;
using namespace std;
namespace FST
{
    RELATION::RELATION(unsigned char c, short ns)
    {
        symbol = c;
        nnode = ns;
    };

    NODE::NODE() {
        n_relation = 0;
        RELATION* relations = nullptr;
    }
    NODE::NODE(short n, RELATION re1, ...) {
        n_relation = n;
        relations = new RELATION[n];

        va_list args;
        va_start(args, re1);
        relations[0] = re1;
        for (int i = 1; i < n; i++) {
            relations[i] = va_arg(args, RELATION);
        }

        va_end(args);
    }

    FST::FST(const char* s, short ns, NODE n, ...) {
        string = s;
        nstates = ns;
        nodes = new NODE[ns];

        va_list args;
        va_start(args, n);
        nodes[0] = n;

        for (int k = 1; k < ns; k++) {
            nodes[k] = va_arg(args, NODE);
        }

        va_end(args);
        rstates = new short[nstates];
        memset(rstates, 0xff, sizeof(short) * nstates);
        rstates[0] = 0;
        position = -1;
    }

	void compare(IT::Entry l, IT::Entry f)
	{
		if (l.id == f.id)
		{
			return;
		}
		if (l.iddatatype != f.iddatatype)
			throw ERROR_THROW(700)
	}
	void seek(IT::Entry l, IT::Entry f)
	{
		if (l.idtype == f.idtype)
		{
			return;
		}
	}

	int parseInteger(char* str)
	{
		// Проверяем, начинается ли строка с "0x", "0b" или "0"
		if (str[1] != '\0' && str[0] == '0') {
			if (str[1] == 'x' || str[1] == 'X') {
				// Шестнадцатеричное число
				return std::stoi(str + 2, (size_t*)nullptr, 16);
			}
			else if (str[1] == 'b' || str[1] == 'B') {
				if(str[3] > '1')
					throw ERROR_THROW(122);
				return std::stoi(str + 2, (size_t*)nullptr, 2);
			}
			else {
				// Восьмеричное число
				return std::stoi(str, (size_t*)nullptr, 8);
			}
		}
		// Десятичное число
		return std::stoi(str);
	}
	void findBad(bool typeD, LT::LexTable lextable, IT::IdTable idtable)
	{
		
		for (int i = 1; i < lextable.size; i++)
		{

			if ((lextable.table[i].lexema == 'm' || lextable.table[i].lexema == 't' || lextable.table[i].lexema == 'n' || lextable.table[i].lexema == 'q' || lextable.table[i].lexema == 'c') && lextable.table[i - 1].lexema == 't')
			{
				throw ERROR_THROW(98);
			}

			
		}
	}

	void typeFind(LT::LexTable lextable, IT::IdTable idtable, IT::Entry checking)
	{
		IT::Entry temp;
		bool flag = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (i >= 2 && lextable.table[i - 2].lexema == 'f' && lextable.table[i - 1].lexema == 't')
			{
				temp = idtable.table[lextable.table[i].idxTI];
			}
			else if (lextable.table[i - 1].lexema == 'r')
			{
				
				if ((checking.iddatatype != IT::INT && temp.iddatatype == IT::INT) || ((checking.iddatatype != IT::BOOL && temp.iddatatype == IT::BOOL)))
					throw ERROR_THROW(700);
			}
		}


	}


    bool step(FST& fst, short*& rstates) {
        bool rc = false;
        std::swap(rstates, fst.rstates);
        for (short i = 0; i < fst.nstates; i++)
        {
            if (rstates[i] == fst.position)
            {
                for (short j = 0; j < fst.nodes[i].n_relation; j++)
                {
                    if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
                    {
                        fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
                        rc = true;
                    }
                }
            }
        }
        return rc;
    }
    bool execute(FST& fst)
    {
        short* rstates = new short[fst.nstates];
        memset(rstates, 0xff, sizeof(short) * fst.nstates);

        short lstring = strlen(fst.string);

        bool rc = true;

        for (short i = 0; i < lstring && rc; i++)
        {
            fst.position++;
            rc = step(fst, rstates);
        }

        delete[] rstates;
        return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
    }



	void CheckL(In::IN file, LexTable &lextable, IdTable& table)
	{
		
		stack<char*>viewment;


		viewment.push((char*)"global");
		string word = ";,{}()=";
		bool boolean = false;
		bool funcF = false;
		bool typeD = false;
		bool flagParm = false;
		bool equals = false;
		int line = 1;
		for (int i = 0; i < file.words.size(); i++)
		{
			bool flag = false;
			bool repeat = false;
			string StringLine = file.words[i];
			char* cstr = new char[StringLine.size() + 1];
			strcpy_s(cstr, (StringLine.size() + 1) * sizeof(char), StringLine.c_str());
			
			for (int j = 0; j < 7; j++)
			{

				string tempW = "";
				tempW += word[j];

				if (StringLine == tempW)
				{
					LT::Add(lextable, { word[j], line, NULL});
					
					flag = true;
					if (StringLine == "(" && lextable.table[lextable.size-4].lexema == 'f')
						flagParm = true;
					else if (StringLine == ")")
						flagParm = false;
					else if (StringLine == "=")
					{
						equals = true;
					}
					break;
				}
				
			}
			

			if (StringLine == " ")
			{
				continue;
			}
			if (StringLine == "|" || StringLine == "\n")
			{
				line++;
				continue;
			}
			if (StringLine == ">>")
			{
				LT::Add(lextable, { '>', line, NULL, cstr, 2, '>' });
				flag = true;
				continue;
			}
			else if (StringLine == "<<")
			{
				LT::Add(lextable, { '<', line, NULL, cstr, 2, '<'});
				flag = true;
				continue;
			}
			
			

			


			FST uint
			(
				(char*)cstr,
				5,
				NODE(1, RELATION('u', 1)),
				NODE(1, RELATION('i', 2)),
				NODE(1, RELATION('n', 3)),
				NODE(1, RELATION('t', 4)),
				NODE()
			);

			FST boo
			(
				(char*)cstr,
				5,
				NODE(1, RELATION('b', 1)),
				NODE(1, RELATION('o', 2)),
				NODE(1, RELATION('o', 3)),
				NODE(1, RELATION('l', 4)),
				NODE()
			);

			if (execute(uint) || execute(boo))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, {'t', line, NULL, cstr });
				if (StringLine == "bool")
					boolean = true;
				typeD = true;
				if (lextable.size > 1)
					findBad(typeD, lextable, table);
				continue;
			}

			

			FST func
			(
				(char*)cstr,
				9,
				NODE(1, RELATION('f', 1)),
				NODE(1, RELATION('u', 2)),
				NODE(1, RELATION('n', 3)),
				NODE(1, RELATION('c', 4)),
				NODE(1, RELATION('t', 5)),
				NODE(1, RELATION('i', 6)),
				NODE(1, RELATION('o', 7)),
				NODE(1, RELATION('n', 8)),
				NODE()
			);

			FST writeInt
			(
				(char*)cstr,
				9,
				NODE(1, RELATION('w', 1)),
				NODE(1, RELATION('r', 2)),
				NODE(1, RELATION('i', 3)),
				NODE(1, RELATION('t', 4)),
				NODE(1, RELATION('e', 5)),
				NODE(1, RELATION('I', 6)),
				NODE(1, RELATION('n', 7)),
				NODE(1, RELATION('t', 8)),
				NODE()
			);

			FST today
			(
				(char*)cstr,
				6,
				NODE(1, RELATION('t', 1)),
				NODE(1, RELATION('o', 2)),
				NODE(1, RELATION('d', 3)),
				NODE(1, RELATION('a', 4)),
				NODE(1, RELATION('y', 5)),
				NODE()
			);

			FST showtime
			(
				(char*)cstr,
				9,
				NODE(1, RELATION('s', 1)),
				NODE(1, RELATION('h', 2)),
				NODE(1, RELATION('o', 3)),
				NODE(1, RELATION('w', 4)),
				NODE(1, RELATION('t', 5)),
				NODE(1, RELATION('i', 6)),
				NODE(1, RELATION('m', 7)),
				NODE(1, RELATION('e', 8)),
				NODE()
			);

			FST ruslang
			(
				(char*)cstr,
				8,
				NODE(1, RELATION('r', 1)),
				NODE(1, RELATION('u', 2)),
				NODE(1, RELATION('s', 3)),
				NODE(1, RELATION('l', 4)),
				NODE(1, RELATION('a', 5)),
				NODE(1, RELATION('n', 6)),
				NODE(1, RELATION('g', 7)),
				NODE()
			);

			FST showInt
			(
				(char*)cstr,
				8,
				NODE(1, RELATION('s', 1)),
				NODE(1, RELATION('h', 2)),
				NODE(1, RELATION('o', 3)),
				NODE(1, RELATION('w', 4)),
				NODE(1, RELATION('I', 5)),
				NODE(1, RELATION('n', 6)),
				NODE(1, RELATION('t', 7)),
				NODE()
			);

			if (execute(func))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'f', line, NULL, cstr });
				funcF = true;
				continue;
			}

			if (execute(showtime) || execute(today) || execute(writeInt) || execute(ruslang) || execute(showInt))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'L', line, table.size, cstr });
				IT::Entry tempEntry;
				tempEntry.view = viewment;
				tempEntry.idxfirstLE = lextable.size - 1;
				tempEntry.id = cstr;
				tempEntry.vstr.str = cstr;
				tempEntry.vstr.len = StringLine.length();
				tempEntry.iddatatype = IT::STR;
				tempEntry.idtype = IT::F;
				tempEntry.view = viewment;
				tempEntry.flag = true;
				IT::Add(table, tempEntry);
				continue;
			}

			FST mainone
			(
				(char*)cstr,
				5,
				NODE(1, RELATION('m', 1)),
				NODE(1, RELATION('a', 2)),
				NODE(1, RELATION('i', 3)),
				NODE(1, RELATION('n', 4)),
				NODE()
			);

			if (execute(mainone))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'm', line, NULL, cstr });
				viewment.pop();
				viewment.push((char*)"main");
				continue;
			}

			FST ret
			(
				(char*)cstr,
				7,
				NODE(1, RELATION('r', 1)),
				NODE(1, RELATION('e', 2)),
				NODE(1, RELATION('t', 3)),
				NODE(1, RELATION('u', 4)),
				NODE(1, RELATION('r', 5)),
				NODE(1, RELATION('n', 6)),
				NODE()
			);
			if (execute(ret))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'r', line, NULL, cstr });
				continue;
			}

			

			FST write
			(
				(char*)cstr,
				6,
				NODE(1, RELATION('w', 1)),
				NODE(1, RELATION('r', 2)),
				NODE(1, RELATION('i', 3)),
				NODE(1, RELATION('t', 4)),
				NODE(1, RELATION('e', 5)),
				NODE()
			);

			if (execute(write))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'w', line, NULL, cstr });

				continue;
			}


			FST print
			(
				(char*)cstr,
				6,
				NODE(1, RELATION('p', 1)),
				NODE(1, RELATION('r', 2)),
				NODE(1, RELATION('i', 3)),
				NODE(1, RELATION('n', 4)),
				NODE(1, RELATION('t', 5)),
				NODE()
			);
			if (execute(print))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'p', line, NULL, cstr });

				continue;
			}

			

			FST	cycle(
				(char*)cstr,
				6,
				NODE(1, RELATION('c', 1)),
				NODE(1, RELATION('y', 2)),
				NODE(1, RELATION('c', 3)),
				NODE(1, RELATION('l', 4)),
				NODE(1, RELATION('e', 5)),
				NODE()

			);

			if (execute(cycle))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'c', line, NULL, cstr });
				continue;
			}

			FST truth(
				(char*)cstr,
				5,
				NODE(1, RELATION('t', 1)),
				NODE(1, RELATION('r', 2)),
				NODE(1, RELATION('u', 3)),
				NODE(1, RELATION('e', 4)),
				NODE()
			);

			FST lie(
				(char*)cstr,
				6,
				NODE(1, RELATION('f', 1)),
				NODE(1, RELATION('a', 2)),
				NODE(1, RELATION('l', 3)),
				NODE(1, RELATION('s', 4)),
				NODE(1, RELATION('e', 5)),
				NODE()
			);


			if (execute(truth) || execute(lie))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'q', line, table.size, cstr });
				IT::Entry tempEntry;
				tempEntry.flag = true;
				tempEntry.idxfirstLE = lextable.size - 1;
				if (StringLine == "false")
				{
					tempEntry.vbool = false;
				}
				else
				{
					tempEntry.vbool = true;
				}
				tempEntry.id = cstr;
				tempEntry.iddatatype = IDDATATYPE::BOOL;
				if (flagParm == true)
					tempEntry.idtype = IDTYPE::P;
				else  tempEntry.idtype = IDTYPE::L
					;
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
					table.table[table.size - 1].flag = true;
				}
				IT::Add(table, tempEntry);
				
				repeat = false;
				continue;
			
			}


			FST strLit(
				(char*)cstr,
				4,
				NODE(1, RELATION('\'', 1)),
				NODE(256,
					RELATION('A', 1), RELATION('A', 2), RELATION('B', 1), RELATION('B', 2),
					RELATION('C', 1), RELATION('C', 2), RELATION('D', 1), RELATION('D', 2),
					RELATION('E', 1), RELATION('E', 2), RELATION('F', 1), RELATION('F', 2),
					RELATION('G', 1), RELATION('G', 2), RELATION('H', 1), RELATION('H', 2),
					RELATION('I', 1), RELATION('I', 2), RELATION('J', 1), RELATION('J', 2),
					RELATION('K', 1), RELATION('K', 2), RELATION('L', 1), RELATION('L', 2),
					RELATION('M', 1), RELATION('M', 2), RELATION('N', 1), RELATION('N', 2),
					RELATION('O', 1), RELATION('O', 2), RELATION('P', 1), RELATION('P', 2),
					RELATION('Q', 1), RELATION('Q', 2), RELATION('R', 1), RELATION('R', 2),
					RELATION('S', 1), RELATION('S', 2), RELATION('T', 1), RELATION('T', 2),
					RELATION('U', 1), RELATION('U', 2), RELATION('V', 1), RELATION('V', 2),
					RELATION('W', 1), RELATION('W', 2), RELATION('X', 1), RELATION('X', 2),
					RELATION('Y', 1), RELATION('Y', 2), RELATION('Z', 1), RELATION('Z', 2),
					RELATION('a', 1), RELATION('a', 2), RELATION('b', 1), RELATION('b', 2),
					RELATION('c', 1), RELATION('c', 2), RELATION('d', 1), RELATION('d', 2),
					RELATION('e', 1), RELATION('e', 2), RELATION('f', 1), RELATION('f', 2),
					RELATION('g', 1), RELATION('g', 2), RELATION('h', 1), RELATION('h', 2),
					RELATION('i', 1), RELATION('i', 2), RELATION('j', 1), RELATION('j', 2),
					RELATION('k', 1), RELATION('k', 2), RELATION('l', 1), RELATION('l', 2),
					RELATION('m', 1), RELATION('m', 2), RELATION('n', 1), RELATION('n', 2),
					RELATION('o', 1), RELATION('o', 2), RELATION('p', 1), RELATION('p', 2),
					RELATION('q', 1), RELATION('q', 2), RELATION('r', 1), RELATION('r', 2),
					RELATION('s', 1), RELATION('s', 2), RELATION('t', 1), RELATION('t', 2),
					RELATION('u', 1), RELATION('u', 2), RELATION('v', 1), RELATION('v', 2),
					RELATION('w', 1), RELATION('w', 2), RELATION('x', 1), RELATION('x', 2),
					RELATION('y', 1), RELATION('y', 2), RELATION('z', 1), RELATION('z', 2),
					RELATION('А', 1), RELATION('А', 2), RELATION('Б', 1), RELATION('Б', 2),
					RELATION('В', 1), RELATION('В', 2), RELATION('Г', 1), RELATION('Г', 2),
					RELATION('Д', 1), RELATION('Д', 2), RELATION('Е', 1), RELATION('Е', 2),
					RELATION('Ё', 1), RELATION('Ё', 2), RELATION('Ж', 1), RELATION('Ж', 2),
					RELATION('З', 1), RELATION('З', 2), RELATION('И', 1), RELATION('И', 2),
					RELATION('Й', 1), RELATION('Й', 2), RELATION('К', 1), RELATION('К', 2),
					RELATION('Л', 1), RELATION('Л', 2), RELATION('М', 1), RELATION('М', 2),
					RELATION('Н', 1), RELATION('Н', 2), RELATION('О', 1), RELATION('О', 2),
					RELATION('П', 1), RELATION('П', 2), RELATION('Р', 1), RELATION('Р', 2),
					RELATION('С', 1), RELATION('С', 2), RELATION('Т', 1), RELATION('Т', 2),
					RELATION('У', 1), RELATION('У', 2), RELATION('Ф', 1), RELATION('Ф', 2),
					RELATION('Х', 1), RELATION('Х', 2), RELATION('Ц', 1), RELATION('Ц', 2),
					RELATION('Ч', 1), RELATION('Ч', 2), RELATION('Ш', 1), RELATION('Ш', 2),
					RELATION('Щ', 1), RELATION('Щ', 2), RELATION('Ъ', 1), RELATION('Ъ', 2),
					RELATION('Ы', 1), RELATION('Ы', 2), RELATION('Ь', 1), RELATION('Ь', 2),
					RELATION('Э', 1), RELATION('Э', 2), RELATION('Ю', 1), RELATION('Ю', 2),
					RELATION('Я', 1), RELATION('Я', 2), RELATION('а', 1), RELATION('а', 2),
					RELATION('б', 1), RELATION('б', 2), RELATION('в', 1), RELATION('в', 2),
					RELATION('г', 1), RELATION('г', 2), RELATION('д', 1), RELATION('д', 2),
					RELATION('е', 1), RELATION('е', 2), RELATION('ё', 1), RELATION('ё', 2),
					RELATION('ж', 1), RELATION('ж', 2), RELATION('з', 1), RELATION('з', 2),
					RELATION('и', 1), RELATION('и', 2), RELATION('й', 1), RELATION('й', 2),
					RELATION('к', 1), RELATION('к', 2), RELATION('л', 1), RELATION('л', 2),
					RELATION('м', 1), RELATION('м', 2), RELATION('н', 1), RELATION('н', 2),
					RELATION('о', 1), RELATION('о', 2), RELATION('п', 1), RELATION('п', 2),
					RELATION('р', 1), RELATION('р', 2), RELATION('с', 1), RELATION('с', 2),
					RELATION('т', 1), RELATION('т', 2), RELATION('у', 1), RELATION('у', 2),
					RELATION('ф', 1), RELATION('ф', 2), RELATION('х', 1), RELATION('х', 2),
					RELATION('ц', 1), RELATION('ц', 2), RELATION('ч', 1), RELATION('ч', 2),
					RELATION('ш', 1), RELATION('ш', 2), RELATION('щ', 1), RELATION('щ', 2),
					RELATION('ъ', 1), RELATION('ъ', 2), RELATION('ы', 1), RELATION('ы', 2),
					RELATION('ь', 1), RELATION('ь', 2), RELATION('э', 1), RELATION('э', 2),
					RELATION('ю', 1), RELATION('ю', 2), RELATION('я', 1), RELATION('я', 2),
					RELATION('0', 1), RELATION('0', 2), RELATION('1', 1), RELATION('1', 2),
					RELATION('2', 1), RELATION('2', 2), RELATION('3', 1), RELATION('3', 2),
					RELATION('4', 1), RELATION('4', 2), RELATION('5', 1), RELATION('5', 2),
					RELATION('6', 1), RELATION('6', 2), RELATION('7', 1), RELATION('7', 2),
					RELATION('8', 1), RELATION('8', 2), RELATION('9', 1), RELATION('9', 2),
					RELATION(' ', 1), RELATION(' ', 2)),
				NODE(1, RELATION('\'', 3)),
				NODE()
			);

			
			if (execute(strLit))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'l', line, table.size, cstr });
				IT::Entry tempEntry;
				tempEntry.flag = true;
				tempEntry.idxfirstLE = lextable.size-1;
				tempEntry.vstr.str = (char*)cstr;
				tempEntry.view = viewment;
				tempEntry.id = cstr;
				tempEntry.vstr.len = StringLine.length();
				tempEntry.iddatatype = IDDATATYPE::STR;
				if (flagParm == true)
					tempEntry.idtype = IDTYPE::P;
				else  tempEntry.idtype = IDTYPE::L;
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
					table.table[table.size - 1].flag = true;
				}
				IT::Add(table, tempEntry);
				repeat = false;
				continue;
			}

			FST intLit(
				(char*)cstr,
				7,
				NODE(20,
					RELATION('0', 1), RELATION('0', 6), 
					RELATION('1', 5), RELATION('2', 5), RELATION('3', 5), RELATION('4', 5),
					RELATION('5', 5), RELATION('6', 5), RELATION('7', 5), RELATION('8', 5), RELATION('9', 5),
					RELATION('1', 6), RELATION('2', 6), RELATION('3', 6), RELATION('4', 6),
					RELATION('5', 6), RELATION('6', 6), RELATION('7', 6), RELATION('8', 6), RELATION('9', 6)
				),
	
				NODE(13,
					RELATION('x', 3), RELATION('X', 3), 
					RELATION('b', 4), RELATION('B', 4),
					RELATION('0', 1), RELATION('0', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2),
					RELATION('5', 2), RELATION('6', 2), RELATION('7', 2) 
				),
				NODE(16,
					RELATION('0', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2),
					RELATION('5', 2), RELATION('6', 2), RELATION('7', 2),
					RELATION('0', 6), RELATION('1', 6), RELATION('2', 6), RELATION('3', 6), RELATION('4', 6),
					RELATION('5', 6), RELATION('6', 6), RELATION('7', 6) 
				),

				NODE(44,
					
					RELATION('0', 3), RELATION('1', 3), RELATION('2', 3), RELATION('3', 3), RELATION('4', 3), RELATION('5', 3),
					RELATION('6', 3), RELATION('7', 3), RELATION('8', 3), RELATION('9', 3),
					RELATION('a', 3), RELATION('b', 3), RELATION('c', 3), RELATION('d', 3),
					RELATION('e', 3), RELATION('f', 3), RELATION('A', 3), RELATION('B', 3),
					RELATION('C', 3), RELATION('D', 3), RELATION('E', 3), RELATION('F', 3),

					RELATION('0', 6), RELATION('1', 6), RELATION('2', 6), RELATION('3', 6), RELATION('4', 6), RELATION('5', 6),
					RELATION('6', 6), RELATION('7', 6), RELATION('8', 6), RELATION('9', 6),
					RELATION('a', 6), RELATION('b', 6), RELATION('c', 6), RELATION('d', 6),
					RELATION('e', 6), RELATION('f', 6), RELATION('A', 6), RELATION('B', 6),
					RELATION('C', 6), RELATION('D', 6), RELATION('E', 6), RELATION('F', 6)
				),
				NODE(4,
					RELATION('0', 4), RELATION('1', 4),
					RELATION('0', 6), RELATION('1', 6)
				),
				NODE(20,
					RELATION('0', 5), RELATION('1', 5), RELATION('2', 5), RELATION('3', 5), RELATION('4', 5),
					RELATION('5', 5), RELATION('6', 5), RELATION('7', 5), RELATION('8', 5), RELATION('9', 5),
					RELATION('0', 6), RELATION('1', 6), RELATION('2', 6), RELATION('3', 6), RELATION('4', 6),
					RELATION('5', 6), RELATION('6', 6), RELATION('7', 6), RELATION('8', 6), RELATION('9', 6)
				),

				NODE()
			);

			

			if (execute(intLit))
			{
				if (typeD == true)
					throw ERROR_THROW(98);
				LT::Add(lextable, { 'n', line, table.size, cstr });
				IT::Entry tempEntry;
				if (atoi(cstr) > 255 || atoi(cstr) < 0)
					throw ERROR_THROW(90);
				tempEntry.idxfirstLE = lextable.size-1;
				tempEntry.flag = true;
				tempEntry.view = viewment;
				tempEntry.vint = parseInteger(cstr);
				tempEntry.id = cstr;
				tempEntry.iddatatype = IDDATATYPE::INT;
				if (flagParm == true)
					tempEntry.idtype = IDTYPE::P;
				else  tempEntry.idtype = IDTYPE::L;
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
					table.table[table.size - 1].flag = true;
				}
				IT::Add(table, tempEntry);
				
				repeat = false;
				continue;
			}


			FST iLex(
				(char*)cstr,
				3,
				NODE(156,
					RELATION('A', 0), RELATION('A', 1), RELATION('A', 2),
					RELATION('B', 0), RELATION('B', 1), RELATION('B', 2),
					RELATION('C', 0), RELATION('C', 1), RELATION('C', 2),
					RELATION('D', 0), RELATION('D', 1), RELATION('D', 2),
					RELATION('E', 0), RELATION('E', 1), RELATION('E', 2),
					RELATION('F', 0), RELATION('F', 1), RELATION('F', 2),
					RELATION('G', 0), RELATION('G', 1), RELATION('G', 2),
					RELATION('H', 0), RELATION('H', 1), RELATION('H', 2),
					RELATION('I', 0), RELATION('I', 1), RELATION('I', 2),
					RELATION('J', 0), RELATION('J', 1), RELATION('J', 2),
					RELATION('K', 0), RELATION('K', 1), RELATION('K', 2),
					RELATION('L', 0), RELATION('L', 1), RELATION('L', 2),
					RELATION('M', 0), RELATION('M', 1), RELATION('M', 2),
					RELATION('N', 0), RELATION('N', 1), RELATION('N', 2),
					RELATION('O', 0), RELATION('O', 1), RELATION('O', 2),
					RELATION('P', 0), RELATION('P', 1), RELATION('P', 2),
					RELATION('Q', 0), RELATION('Q', 1), RELATION('Q', 2),
					RELATION('R', 0), RELATION('R', 1), RELATION('R', 2),
					RELATION('S', 0), RELATION('S', 1), RELATION('S', 2),
					RELATION('T', 0), RELATION('T', 1), RELATION('T', 2),
					RELATION('U', 0), RELATION('U', 1), RELATION('U', 2),
					RELATION('V', 0), RELATION('V', 1), RELATION('V', 2),
					RELATION('W', 0), RELATION('W', 1), RELATION('W', 2),
					RELATION('X', 0), RELATION('X', 1), RELATION('X', 2),
					RELATION('Y', 0), RELATION('Y', 1), RELATION('Y', 2),
					RELATION('Z', 0), RELATION('Z', 1), RELATION('Z', 2),
					RELATION('a', 0), RELATION('a', 1), RELATION('a', 2),
					RELATION('b', 0), RELATION('b', 1), RELATION('b', 2),
					RELATION('c', 0), RELATION('c', 1), RELATION('c', 2),
					RELATION('d', 0), RELATION('d', 1), RELATION('d', 2),
					RELATION('e', 0), RELATION('e', 1), RELATION('e', 2),
					RELATION('f', 0), RELATION('f', 1), RELATION('f', 2),
					RELATION('g', 0), RELATION('g', 1), RELATION('g', 2),
					RELATION('h', 0), RELATION('h', 1), RELATION('h', 2),
					RELATION('i', 0), RELATION('i', 1), RELATION('i', 2),
					RELATION('j', 0), RELATION('j', 1), RELATION('j', 2),
					RELATION('k', 0), RELATION('k', 1), RELATION('k', 2),
					RELATION('l', 0), RELATION('l', 1), RELATION('l', 2),
					RELATION('m', 0), RELATION('m', 1), RELATION('m', 2),
					RELATION('n', 0), RELATION('n', 1), RELATION('n', 2),
					RELATION('o', 0), RELATION('o', 1), RELATION('o', 2),
					RELATION('p', 0), RELATION('p', 1), RELATION('p', 2),
					RELATION('q', 0), RELATION('q', 1), RELATION('q', 2),
					RELATION('r', 0), RELATION('r', 1), RELATION('r', 2),
					RELATION('s', 0), RELATION('s', 1), RELATION('s', 2),
					RELATION('t', 0), RELATION('t', 1), RELATION('t', 2),
					RELATION('u', 0), RELATION('u', 1), RELATION('u', 2),
					RELATION('v', 0), RELATION('v', 1), RELATION('v', 2),
					RELATION('w', 0), RELATION('w', 1), RELATION('w', 2),
					RELATION('x', 0), RELATION('x', 1), RELATION('x', 2),
					RELATION('y', 0), RELATION('y', 1), RELATION('y', 2),
					RELATION('z', 0), RELATION('z', 1), RELATION('z', 2),
					RELATION('_', 0), RELATION('_', 1)
				),
				NODE(126,
					RELATION('0', 1), RELATION('0', 2),
					RELATION('1', 1), RELATION('1', 2),
					RELATION('2', 1), RELATION('2', 2),
					RELATION('3', 1), RELATION('3', 2),
					RELATION('4', 1), RELATION('4', 2),
					RELATION('5', 1), RELATION('5', 2),
					RELATION('6', 1), RELATION('6', 2),
					RELATION('7', 1), RELATION('7', 2),
					RELATION('8', 1), RELATION('8', 2),
					RELATION('9', 1), RELATION('9', 2),
					RELATION('_', 1), RELATION('_', 2),
					RELATION('A', 1), RELATION('A', 2),
					RELATION('B', 1), RELATION('B', 2),
					RELATION('C', 1), RELATION('C', 2),
					RELATION('D', 1), RELATION('D', 2),
					RELATION('E', 1), RELATION('E', 2),
					RELATION('F', 1), RELATION('F', 2),
					RELATION('G', 1), RELATION('G', 2),
					RELATION('H', 1), RELATION('H', 2),
					RELATION('I', 1), RELATION('I', 2),
					RELATION('J', 1), RELATION('J', 2),
					RELATION('K', 1), RELATION('K', 2),
					RELATION('L', 1), RELATION('L', 2),
					RELATION('M', 1), RELATION('M', 2),
					RELATION('N', 1), RELATION('N', 2),
					RELATION('O', 1), RELATION('O', 2),
					RELATION('P', 1), RELATION('P', 2),
					RELATION('Q', 1), RELATION('Q', 2),
					RELATION('R', 1), RELATION('R', 2),
					RELATION('S', 1), RELATION('S', 2),
					RELATION('T', 1), RELATION('T', 2),
					RELATION('U', 1), RELATION('U', 2),
					RELATION('V', 1), RELATION('V', 2),
					RELATION('W', 1), RELATION('W', 2),
					RELATION('X', 1), RELATION('X', 2),
					RELATION('Y', 1), RELATION('Y', 2),
					RELATION('Z', 1), RELATION('Z', 2),
					RELATION('a', 1), RELATION('a', 2),
					RELATION('b', 1), RELATION('b', 2),
					RELATION('c', 1), RELATION('c', 2),
					RELATION('d', 1), RELATION('d', 2),
					RELATION('e', 1), RELATION('e', 2),
					RELATION('f', 1), RELATION('f', 2),
					RELATION('g', 1), RELATION('g', 2),
					RELATION('h', 1), RELATION('h', 2),
					RELATION('i', 1), RELATION('i', 2),
					RELATION('j', 1), RELATION('j', 2),
					RELATION('k', 1), RELATION('k', 2),
					RELATION('l', 1), RELATION('l', 2),
					RELATION('m', 1), RELATION('m', 2),
					RELATION('n', 1), RELATION('n', 2),
					RELATION('o', 1), RELATION('o', 2),
					RELATION('p', 1), RELATION('p', 2),
					RELATION('q', 1), RELATION('q', 2),
					RELATION('r', 1), RELATION('r', 2),
					RELATION('s', 1), RELATION('s', 2),
					RELATION('t', 1), RELATION('t', 2),
					RELATION('u', 1), RELATION('u', 2),
					RELATION('v', 1), RELATION('v', 2),
					RELATION('w', 1), RELATION('w', 2),
					RELATION('x', 1), RELATION('x', 2),
					RELATION('y', 1), RELATION('y', 2),
					RELATION('z', 1), RELATION('z', 2)),
				NODE()
			);
			if (execute(iLex))
			{
				if (StringLine.length() > 5)
					throw ERROR_THROW(92);
				LT::Add(lextable, { 'i', line, table.size, cstr });
				IT::Entry tempEntry;
				tempEntry.view = viewment;
				tempEntry.idxfirstLE = lextable.size-1;
				tempEntry.id = cstr;
				tempEntry.vstr.str = cstr;
				tempEntry.vstr.len = StringLine.length();
				if (boolean == true)
				{
					tempEntry.iddatatype = IDDATATYPE::BOOL;
					boolean = false;
				}
				else tempEntry.iddatatype = IDDATATYPE::INT;
				if (funcF == true || (funcF == true && typeD == true))
				{
					tempEntry.idtype = IDTYPE::F;
					tempEntry.flag = true;
					viewment.push(cstr);
				}
				else if (flagParm == true)
				{
					tempEntry.idtype = IDTYPE::P;
					tempEntry.flag = true;
				}

				else 
					tempEntry.idtype = IDTYPE::V;
				bool absence = false;
				bool field_of = false;
				for (int j = 0; j < table.size; j++)
				{
					if (strcmp(table.table[j].id, "today") == false)
						continue;
					
					if ((tempEntry.idtype == IT::V || tempEntry.idtype == IT::P || tempEntry.idtype == IT::F) 
						&& (strcmp(tempEntry.id, table.table[j].id) == false) 
						&&  typeD == false 
						&& (strcmp(tempEntry.view.top(), table.table[j].view.top()) == false || table.table[j].view.size()==1))
					{
						absence = true;
						tempEntry.flag = true;
						table.table[j].flag = true;
					}
					
					if (!strcmp(tempEntry.id, table.table[j].id) && typeD == true && (strcmp(tempEntry.view.top(), table.table[j].view.top()) == false) && tempEntry.idtype == IT::F)
					{
						throw ERROR_THROW(94);
					}
					else if (!strcmp(tempEntry.id, table.table[j].id) && typeD == true && (strcmp(tempEntry.view.top(), table.table[j].view.top()) == false))
					{
						throw ERROR_THROW(699);
					}
					
				}


				for (int j = 0; j < table.size; j++)
				{
				if (absence == false && typeD == false && j == table.size - 1 && strcmp(table.table[j].id, "today") == true)
				{
					throw ERROR_THROW(707);
				}
				}

				if (field_of == true)
				{
					throw ERROR_THROW(707);
				}
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
					tempEntry.flag = true;
					table.table[table.size - 1].flag = true;
				}
				
				if (absence == false && (funcF == true || (funcF == true && typeD == true)))
				{

					IT::Add(table, tempEntry);
				}
				else if (absence == false)
				{
					IT::Add(table, tempEntry);
				}
				if (lextable.table[lextable.size - 2].lexema == 'r')
				{
					typeFind(lextable, table, tempEntry);
					viewment.pop();
				}
				if (lextable.table[lextable.size - 2].lexema == 'c' && tempEntry.iddatatype != IT::INT)
				{
					throw ERROR_THROW(125);
				}
				typeD = false;
				repeat = false;
				funcF = false;
				continue;
			}

		}
		bool main_presence = false;

		for (int i = 0; i < lextable.size; i++)
		{
			if(lextable.table[i-2].lexema =='c' && table.table[lextable.table[i].idxTI].iddatatype == IT::STR)
				throw ERROR_THROW(125)
			if (lextable.table[i].lexema == 'm' && main_presence == false)
				main_presence = true;
			else if (lextable.table[i].lexema == 'm' && main_presence == true)
				throw ERROR_THROW(118);
		}
		if (main_presence == false)
			throw ERROR_THROW(123);

		for(int i = 0; i < table.size; i++)
		{
			IT::Entry lexEntry = table.table[i];
			if (lexEntry.flag == false)
				throw ERROR_THROW(120);

		}
	}


}