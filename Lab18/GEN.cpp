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
		out << "includelib MAA2024-L.lib\n";
		out << "ExitProcess PROTO :DWORD\n\n";

		out << "EXTRN showtime: proc\n";
		out << "EXTRN ruslang: proc\n";
		out << "EXTRN today: proc\n";
		out << "EXTRN showInt: proc\n";

		out << "\n.stack 4096\n\n";
	}

	void Generator::Const() {
		out << ".const\n";
		for (int i = 0; i < idT.size; i++) {
			if (idT.table[i].idtype == IT::L) {
				out << "\t" << idT.table[i].id;
				if (idT.table[i].iddatatype == IT::INT)
					out << " BYTE " << idT.table[i].vint;
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
		for (int i = 0; i < idT.size; i++) {
			if (idT.table[i].idtype == IT::V) {
				out << "\t" << idT.table[i].id;
				out << " BYTE 0\n";
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
				out << "main PROC\n\tcall ruslang\n";
				break;
			case 'L':
				if (lexT.table[i + 2].lexema == 'i' || lexT.table[i + 2].lexema == 'n') {
					out << "\tmov al, " << idT.table[lexT.table[i + 2].idxTI].id << "\n\tpush ax\n";
				}
				out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n\tpush eax\n";
				i += 2;
				
				break;

			case '<':
				out << "\tmov al, " << idT.table[lexT.table[i - 1].idxTI].id << "\n\tpush ax\n";
				out << "\tmov al, " << idT.table[lexT.table[++i].idxTI].id << "\n\tpush ax\n";

				out << "\tpop ebx \n" << "\tpop eax \n";
				out << "\tmov cl, bl\n";
				out << "\tshr eax, cl\n";
				out << "\tpush eax\n";
				out << "\tpop ax\n\tmov " << idT.table[lexT.table[i - 2].idxTI].id << ", al\n";
				break;
			case '>':
				out << "\tmov al, " << idT.table[lexT.table[i - 1].idxTI].id << "\n\tpush ax\n";
				out << "\tmov al, " << idT.table[lexT.table[++i].idxTI].id << "\n\tpush ax\n";

				out << "\tpop ebx \n" << "\tpop eax \n";
				out << "\tmov cl, bl\n";
				out << "\tshl eax, cl\n";
				out << "\tpush eax\n";
				out << "\tpop ax\n\tmov " << idT.table[lexT.table[i - 2].idxTI].id << ", al\n";
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
							out << "\tmov al, " << idT.table[lexT.table[i].idxTI].id << "\n\tpush ax\n";
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

				out << "\tpop ax\n\tmov " << idT.table[lexT.table[result_position].idxTI].id << ", al\n";
				break;

			case '@':
				countParm = (char)lexT.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++) {
					if (idT.table[lexT.table[i - j].idxTI].iddatatype == IT::BOOL || idT.table[lexT.table[i - j].idxTI].iddatatype == IT::INT)
						out << "\tmov al," << idT.table[lexT.table[i - j].idxTI].id << "\n\tpush ax\n";
				}

				out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n";
				break;

			case LEX_RETURN:
				out << "\tmov al, ";
				if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::BOOL || idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tpush ax\n";
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

					out << "\tmov ebx, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";//я лох помогите
					out << "cycle" << num_of_cycles << ":\n";
					out << "\tmov buffecx, ecx\n";

				}
				break;

			case LEX_RIGHTHESIS:

				break;
			}
			
		}
	}
}