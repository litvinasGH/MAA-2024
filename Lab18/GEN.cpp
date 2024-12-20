#include "GEN.h"
#include "stdafx.h"
using namespace std;

namespace Gen {
	Generator::Generator(LT::LexTable& plexT, IT::IdTable& pidT, wchar_t pout[])
	{
		lexT = plexT;
		idT = pidT;
		out = ofstream(pout, ios_base::out);

		Head();
		Const();
		Data();
		Code();
	}

	void Generator::Head() {
		out << ".586P\n";
		out << ".model flat, stdcall\n";

		out << "includelib libucrt.lib\n";
		out << "includelib kernel32.lib\n";
		out << "includelib ../Debug/MAA2024-L.lib\n";
		out << "ExitProcess PROTO :DWORD\n\n";

		out << "EXTRN strToByte: proc\n";
		out << "EXTRN slen: proc\n";
		out << "EXTRN OutputInt: proc\n";
		out << "EXTRN OutputStr: proc\n";
		out << "EXTRN OutputIntLn: proc\n";
		out << "EXTRN OutputStrLn: proc\n";

		out << "\n.stack 4096\n\n";
	}

	void Generator::Const() {
		out << ".const\n";
		for (int i = 0; i < idT.size; i++) {
			if (idT.table[i].idtype == IT::L) {
				out << "\t" << idT.table[i].id;
				if (idT.table[i].iddatatype == IT::INT)
					out << " DWORD " << idT.table[i].vint;
				if (idT.table[i].iddatatype == IT::BOOL)
					out << " BYTE " << idT.table[i].vbool;
				out << "\n";
			}
		}
	}

	void Generator::Data() {
		out << "\n.data\n";
		out << "\tbuffer BYTE 256 dup(0)\n";
		out << "\tbuffecx DWORD ?\n";
		for (int i = 0; i < lexT.size; i++) {
			if (lexT.table[i].lexema == LEX_TYPE) {
				if (idT.table[lexT.table[i + 2].idxTI].idtype == IT::V) {
					out << "\t" << idT.table[lexT.table[i + 2].idxTI].id;
					if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::BOOL)
						out << " BYTE 0\n";
					else if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::INT)
						out << " DWORD 0\n";
				}
			}
		}
	}

	void Generator::Code() {
		stack<char*> stk;
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0,
			num_of_cycles = 0,
			countParm = 0;
		string strret = string(),
			cycle_code = string(),
			func_name = string();
		bool flag_func = false,
			flag_ret = false,
			flag_body = false,
			flag_callfunc = false,

			flag_cycle = false;

		int result_position;

		out << "\n.code\n\n";
		for (int i = 0; i < lexT.size; i++) {
			switch (lexT.table[i].lexema) {

			case LEX_FUNCTION:
				while (lexT.table[i].lexema != LEX_RIGHTHESIS) {
					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::F) {

						out << (func_name = string((char*)idT.table[lexT.table[i].idxTI].id)) << " PROC ";
					}

					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::P) {
						out << idT.table[lexT.table[i].idxTI].id << " : ";
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT)
							out << "DWORD";
						else
							out << "BYTE";
					}

					if (lexT.table[i].lexema == LEX_COMMA)
						out << ", ";

					i++;
				}
				flag_func = true;
				out << "\n";
				break;

			case LEX_MAIN:
				flag_body = true;
				out << "main PROC\n";
				break;

			case LEX_EQUAL:
				result_position = i - 1;
				while (lexT.table[i].lexema != LEX_SEMICOLON) {
					switch (lexT.table[i].lexema) {
					case LEX_ID:
					case LEX_LITERAL:
						if (idT.table[lexT.table[i].idxTI].idtype == IT::F)
							flag_callfunc = true;
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT || idT.table[lexT.table[i].idxTI].iddatatype == IT::BOOL) {
							out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";
							stk.push(idT.table[lexT.table[i].idxTI].id);
							break;
						}

					case LEX_OPERATOR:
						switch (lexT.table[i].op) {

						case '<':
							out << "\tpop ebx \n" << "\tpop eax \n";
							out << "\tmov cl, bl\n";
							out << "\tshr eax, cl\n";
							out << "\tpush eax\n";
							break;
						case '>':
							out << "\tpop ebx \n" << "\tpop eax \n";
							out << "\tmov cl, bl\n";
							out << "\tshl eax, cl\n";
							out << "\tpush eax\n";
							break;
						}
						break;

					case '@':
						countParm = (char)lexT.table[i + 1].lexema - '0';

						for (int j = 1; j <= countParm; j++)
							out << "\tpop edx\n";

						for (int j = 1; j <= countParm; j++) {
							out << "\tpush " << stk.top() << "\n";
							stk.pop();
						}
						out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n\tpush eax\n";
						flag_callfunc = false;
						break;
					}

					i++;
				}

				out << "\tpop " << idT.table[lexT.table[result_position].idxTI].id << "\n";
				break;

			case '@':
				countParm = (char)lexT.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++) {
					if (idT.table[lexT.table[i - j].idxTI].iddatatype == IT::BOOL || idT.table[lexT.table[i - j].idxTI].iddatatype == IT::INT)
						out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					else {
						if (idT.table[lexT.table[i - j].idxTI].idtype == IT::L)
							out << "\tpush offset " << idT.table[lexT.table[i - j].idxTI].id << "\n";
						else
							out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					}
				}

				out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n";
				break;

			case LEX_RETURN:
				out << "\tpush ";
				if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
					if(idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::BOOL || idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
					out << idT.table[lexT.table[i + 1].idxTI].vint << "\n";
				else
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n";
				if (flag_func) {
					out << "\tjmp local" << num_of_ret << "\n";
					flag_ret = true;
				}
				if (flag_body) {
					out << "\t\tjmp theend\n";
					flag_ret = true;
				}
				break;

			case LEX_BRACELET:
				if (flag_body && !flag_func && !flag_cycle) {
					if (flag_ret) {
						out << "theend:\n";
						flag_ret = false;
					}
					out << "call ExitProcess\nmain ENDP\nend main";
				}
				if (flag_func && !flag_cycle) {
					if (flag_ret) {
						out << "local" << num_of_ret++ << ":\n";
						out << "\tpop eax\n\tret\n";
						flag_ret = false;
					}

					out << func_name << " ENDP\n\n";
					flag_func = false;
				}

				if (flag_cycle) {

					out << "\tmov ecx, buffecx\n";
					out << "loop cycle" << num_of_cycles << "\n";

					flag_cycle = false;
				}

				break;
				break;



			case LEX_CYCLE:
				flag_cycle = true;

				break;


			case LEX_LEFTHESIS:
				if (flag_cycle) {

					out << "\tmov ecx, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
					out << "cycle" << num_of_cycles << ":\n";
					out << "\tmov buffecx, ecx\n";

				}
				break;

			case LEX_RIGHTHESIS:

				break;

			case LEX_LIBFUNC:

				if(strcmp(idT.table[lexT.table[i].lexema].id, (char*)"writeInt") == false)
				{
					if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
						out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall writeInt\n";
				}
				
				break;

			
			}
		}
	}
}