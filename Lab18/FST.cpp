#include "FST.h"
#include <iostream>
#include <stdarg.h>
#include "stdafx.h"
#include <cstring>
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
				
				if ((checking.iddatatype == IT::INT && temp.iddatatype == IT::INT) && ((checking.iddatatype == IT::BOOL && temp.iddatatype == IT::BOOL)))
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
					if (StringLine == "(")
						flagParm = true;
					else if (StringLine == ")")
						flagParm = false;
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
				LT::Add(lextable, { 'h', line, NULL, (char*)'>'});
				flag = true;
				continue;
			}
			else if (StringLine == "<<")
			{
				LT::Add(lextable, { 'h', line, NULL, (char*)'<'});
				flag = true;
				continue;
			}
			
			if (StringLine == "=")
			{
				equals = true;
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

			if (execute(func))
			{
				LT::Add(lextable, { 'f', line, NULL, cstr });
				funcF = true;
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
				LT::Add(lextable, { 'm', line, NULL, cstr });

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
				LT::Add(lextable, { 'r', line, NULL, cstr });
				continue;
			}

			FST connect
			(
				(char*)cstr,
				8,
				NODE(1, RELATION('c', 1)),
				NODE(1, RELATION('o', 2)),
				NODE(1, RELATION('n', 3)),
				NODE(1, RELATION('n', 4)),
				NODE(1, RELATION('e', 5)),
				NODE(1, RELATION('c', 6)),
				NODE(1, RELATION('t', 7)),
				NODE()
			);

			if (execute(connect))
			{
				LT::Add(lextable, { 'j', line, NULL, cstr });
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
				
			
				LT::Add(lextable, { 'q', line, table.size, cstr });
				IT::Entry tempEntry;

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
				}
				IT::Add(table, tempEntry);
				
				repeat = false;
				continue;
			
			}


			FST lib(
				(char*)cstr,
				6,
				NODE(1, RELATION('c', 1)),
				NODE(1, RELATION('t', 2)),
				NODE(1, RELATION('i', 3)),
				NODE(1, RELATION('m', 4)),
				NODE(1, RELATION('e', 5)),
				NODE()
				);

			if (execute(lib))
			{
				LT::Add(lextable, { 'y', line, NULL, cstr});
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
					RELATION('À', 1), RELATION('À', 2), RELATION('Á', 1), RELATION('Á', 2),
					RELATION('Â', 1), RELATION('Â', 2), RELATION('Ã', 1), RELATION('Ã', 2),
					RELATION('Ä', 1), RELATION('Ä', 2), RELATION('Å', 1), RELATION('Å', 2),
					RELATION('¨', 1), RELATION('¨', 2), RELATION('Æ', 1), RELATION('Æ', 2),
					RELATION('Ç', 1), RELATION('Ç', 2), RELATION('È', 1), RELATION('È', 2),
					RELATION('É', 1), RELATION('É', 2), RELATION('Ê', 1), RELATION('Ê', 2),
					RELATION('Ë', 1), RELATION('Ë', 2), RELATION('Ì', 1), RELATION('Ì', 2),
					RELATION('Í', 1), RELATION('Í', 2), RELATION('Î', 1), RELATION('Î', 2),
					RELATION('Ï', 1), RELATION('Ï', 2), RELATION('Ð', 1), RELATION('Ð', 2),
					RELATION('Ñ', 1), RELATION('Ñ', 2), RELATION('Ò', 1), RELATION('Ò', 2),
					RELATION('Ó', 1), RELATION('Ó', 2), RELATION('Ô', 1), RELATION('Ô', 2),
					RELATION('Õ', 1), RELATION('Õ', 2), RELATION('Ö', 1), RELATION('Ö', 2),
					RELATION('×', 1), RELATION('×', 2), RELATION('Ø', 1), RELATION('Ø', 2),
					RELATION('Ù', 1), RELATION('Ù', 2), RELATION('Ú', 1), RELATION('Ú', 2),
					RELATION('Û', 1), RELATION('Û', 2), RELATION('Ü', 1), RELATION('Ü', 2),
					RELATION('Ý', 1), RELATION('Ý', 2), RELATION('Þ', 1), RELATION('Þ', 2),
					RELATION('ß', 1), RELATION('ß', 2), RELATION('à', 1), RELATION('à', 2),
					RELATION('á', 1), RELATION('á', 2), RELATION('â', 1), RELATION('â', 2),
					RELATION('ã', 1), RELATION('ã', 2), RELATION('ä', 1), RELATION('ä', 2),
					RELATION('å', 1), RELATION('å', 2), RELATION('¸', 1), RELATION('¸', 2),
					RELATION('æ', 1), RELATION('æ', 2), RELATION('ç', 1), RELATION('ç', 2),
					RELATION('è', 1), RELATION('è', 2), RELATION('é', 1), RELATION('é', 2),
					RELATION('ê', 1), RELATION('ê', 2), RELATION('ë', 1), RELATION('ë', 2),
					RELATION('ì', 1), RELATION('ì', 2), RELATION('í', 1), RELATION('í', 2),
					RELATION('î', 1), RELATION('î', 2), RELATION('ï', 1), RELATION('ï', 2),
					RELATION('ð', 1), RELATION('ð', 2), RELATION('ñ', 1), RELATION('ñ', 2),
					RELATION('ò', 1), RELATION('ò', 2), RELATION('ó', 1), RELATION('ó', 2),
					RELATION('ô', 1), RELATION('ô', 2), RELATION('õ', 1), RELATION('õ', 2),
					RELATION('ö', 1), RELATION('ö', 2), RELATION('÷', 1), RELATION('÷', 2),
					RELATION('ø', 1), RELATION('ø', 2), RELATION('ù', 1), RELATION('ù', 2),
					RELATION('ú', 1), RELATION('ú', 2), RELATION('û', 1), RELATION('û', 2),
					RELATION('ü', 1), RELATION('ü', 2), RELATION('ý', 1), RELATION('ý', 2),
					RELATION('þ', 1), RELATION('þ', 2), RELATION('ÿ', 1), RELATION('ÿ', 2),
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
				LT::Add(lextable, { 'l', line, table.size, cstr });
				IT::Entry tempEntry;
				tempEntry.idxfirstLE = lextable.size-1;
				tempEntry.vstr.str = (char*)cstr;
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
				}
				IT::Add(table, tempEntry);
				repeat = false;
				continue;
			}

			FST intLit
			(
				(char*)cstr,
				2,
				NODE(
					20,
					RELATION('0', 0), RELATION('0', 1),
					RELATION('1', 0), RELATION('1', 1),
					RELATION('2', 0), RELATION('2', 1),
					RELATION('3', 0), RELATION('3', 1),
					RELATION('4', 0), RELATION('4', 1),
					RELATION('5', 0), RELATION('5', 1),
					RELATION('6', 0), RELATION('6', 1),
					RELATION('7', 0), RELATION('7', 1),
					RELATION('8', 0), RELATION('8', 1),
					RELATION('9', 0), RELATION('9', 1)
				),
				NODE()
			);

			if (execute(intLit))
			{
				LT::Add(lextable, { 'n', line, table.size, cstr });
				IT::Entry tempEntry;
				if (atoi(cstr) > 255 || atoi(cstr) < 0)
					throw ERROR_THROW(90);
				tempEntry.idxfirstLE = lextable.size-1;
				tempEntry.vint = atoi(cstr);
				tempEntry.id = cstr;
				tempEntry.iddatatype = IDDATATYPE::INT;
				if (flagParm == true)
					tempEntry.idtype = IDTYPE::P;
				else  tempEntry.idtype = IDTYPE::L;
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
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
					funcF = false;
				}
				else if (flagParm == true)
					tempEntry.idtype = IDTYPE::P;

				else tempEntry.idtype = IDTYPE::V;
				bool absence = false;
				for (int i = 0; i < table.size; i++)
				{
					if (tempEntry.idtype == IT::F && tempEntry.idtype == table.table[i].idtype)
					{
						tempEntry = table.table[i];
					}

					if (!strcmp(tempEntry.id, table.table[i].id) && typeD == false)
					{
						absence = true;
					}
					if (!strcmp(tempEntry.id, table.table[i].id) && typeD == true)
					{
						throw ERROR_THROW(699);
					}
					else if (absence == false && typeD == false && i == table.size - 1)
					{
						throw ERROR_THROW(707);
					}
					
				}
				if (equals == true)
				{
					compare(tempEntry, table.table[table.size - 1]);
					equals = false;
				}

				if(absence == false)
				IT::Add(table, tempEntry);
				if (lextable.table[lextable.size - 2].lexema == 'r')
					typeFind(lextable, table, tempEntry);

				typeD = false;
				repeat = false;
				continue;
			}

		}
		bool main_presence = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == 'm' && main_presence == false)
				main_presence = true;
			else if (lextable.table[i].lexema == 'm' && main_presence == true)
				throw ERROR_THROW(93);
		}
		if (main_presence == false)
			throw ERROR_THROW(97);
	}

}