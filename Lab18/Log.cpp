#include "stdafx.h"
#include "Log.h"
#include <iomanip>
#pragma warning(disable:4996)

namespace Log
{
	LOG getlog(wchar_t logfile[]) 
	{ 
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (!log.stream->is_open()) {
			throw ERROR_THROW(112);
		}
		wcscpy_s(log.logfile, logfile);
		return log;
	}

	void WriteLine(LOG log, const char* c, ...) 
	{
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "") {
			*log.stream << ptr[i++];
		}
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[50];
		int i = 0;
		while (ptr[i] != L"") 
        {
			wcstombs(temp, ptr[i++], sizeof(temp));
			*log.stream << temp;
		}
	}
	void WriteLog(LOG log) 
	{
		char date[50];
		tm local;
		time_t currentTime;
		currentTime = time(NULL);
		localtime_s(&local, &currentTime);
		strftime(date, 100, "%d.%m.%Y %H:%M:%S ----", &local);
		*log.stream << "-----протокол----" << date << endl;
	}
	void WriteParm(LOG log, Parm::PARM parm) 
	{
		char buff[PARM_MAX_SIZE];
		size_t tsize = 0;

		*log.stream << " ----параметры---- " << endl;
		wcstombs_s(&tsize, buff, parm.log, PARM_MAX_SIZE);
		*log.stream << "-log: " << buff << endl;
		wcstombs_s(&tsize, buff, parm.out, PARM_MAX_SIZE);
		*log.stream << "-out: " << buff << endl;
		wcstombs_s(&tsize, buff, parm.in, PARM_MAX_SIZE);
		*log.stream << "-in : " << buff << endl;

	}
	void WriteIn(LOG log, In::IN in) {

		*log.stream << "Исходные данные" << endl;
		*log.stream << "Количество символов : " << in.size << endl;
		*log.stream << "Количество строк    : " << in.lines << endl;
		*log.stream << "Проигнорировано     : " << in.ignor << endl;
	}

    

	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream)
		{
			*log.stream << " ----Ошибка---- " << endl;
			*log.stream << "Ошибка " << error.id << ": " << error.message << endl;
			if (error.inext.line != -1)
			{
				*log.stream << "Строка: " << error.inext.line << endl << "Столбец: " << error.inext.col << endl << endl;
			}
		}
		else
			cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << endl << endl;
	}

	void Close(LOG log) 
	{
		log.stream->close();
		delete log.stream;
	}




	void WriteLex(LOG log, LT::LexTable lextable, IT::IdTable idtable) {
		int str = 0;
		int size = [](int number) -> int {
			if (number == 0) return 1;

			int count = 0;
			while (number != 0) {
				number /= 10;
				count++;
			}
			return count;
			}(LT::GetEntry(lextable, lextable.size - 1).sn);
			(*log.stream) << "\n--—- Представления кода в виде лексем ——-- " << endl;
			for (int i = 0; i < lextable.size; i++) {
				if (lextable.table[i].sn + 1 != str) {
					*log.stream << endl;
					cout << endl;
					str = lextable.table[i].sn + 1;
					*log.stream << std::setfill('0') << std::setw(size) << str << ' ';
					cout << std::setfill('0') << std::setw(size) << str << ' ';
				}
				switch (lextable.table[i].lexema)
				{
				case 'i':
					*log.stream << lextable.table[i].lexema << '|' << lextable.table[i].idxTI + 1 << '|';
					cout << lextable.table[i].lexema << "\033[93m|\033[0m" << lextable.table[i].idxTI + 1 << "\033[93m|\033[0m";
					break;
				case '$':
					*log.stream << lextable.table[i].lexema;
					cout << "\033[31m" << lextable.table[i].lexema << "\033[0m";
					break;
				default:
					*log.stream << lextable.table[i].lexema;
					cout << lextable.table[i].lexema;
					break;
				}
			}
			int maxl = 7;
			string st;
			
			(*log.stream) << "\n--—------------------------------------------- Идентификаторы ---------------------------------------——-----------------" << endl;
			cout << "\n---------------------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "|  Номер  |    id    | Тип данных |     Тип     | Связь (Номер Лексема Строка) |" << right << std::setfill(' ') <<
				setw(maxl) << "Видимость" << "  | Значение ? " << endl;
			cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
			(*log.stream) << "|  Номер  |    id    | Тип данных |     Тип     | Связь (Номер Лексема Строка) |" << right << std::setfill(' ') <<
				setw(maxl) << "Видимость" << "  | Значение ? " << endl;
			(*log.stream) << "-------------------------------------------------------------------------------------------------------------------------" << endl;

			for (int i = 0; i < idtable.size; i++) {
				cout << "| " << std::setw(7) << i + 1 << " | ";
				(*log.stream) << "| " << std::setw(7) << i + 1 << " | ";

				cout << std::setfill(' ') << std::setw(8) << idtable.table[i].id << " | ";
				(*log.stream) << std::setfill(' ') << std::setw(8) << idtable.table[i].id << " | ";

				switch (idtable.table[i].iddatatype) {
				case IT::INT:
					cout << std::setw(10) << "int" << " | ";
					(*log.stream) << std::setw(10) << "int" << " | ";
					break;
				case IT::STR:
					cout << std::setw(10) << "str" << " | ";
					(*log.stream) << std::setw(10) << "str" << " | ";
					break;
				case IT::BOOL:
					cout << std::setw(10) << "bool" << " | ";
					(*log.stream) << std::setw(10) << "bool" << " | ";
					break;
				}

				switch (idtable.table[i].idtype) {
				case IT::F:
					cout << std::setw(11) << "функция" << " | ";
					(*log.stream) << std::setw(11) << "функция" << " | ";
					break;
				case IT::L:
					cout << std::setw(11) << "литерал" << " | ";
					(*log.stream) << std::setw(11) << "литерал" << " | ";
					break;
				case IT::P:
					cout << std::setw(11) << "параметр" << " | ";
					(*log.stream) << std::setw(11) << "параметр" << " | ";
					break;
				case IT::V:
					cout << std::setw(11) << "переменная" << " | ";
					(*log.stream) << std::setw(11) << "переменная" << " | ";
					break;
				
				}

				LT::Entry en = LT::GetEntry(lextable, idtable.table[i].idxfirstLE);
				cout << std::setw(9) << idtable.table[i].idxfirstLE + 1 << std::setw(9) << en.lexema << std::setw(9) << en.sn << "  | ";
				(*log.stream) << std::setw(9) << idtable.table[i].idxfirstLE + 1 << std::setw(9) << en.lexema << std::setw(9) << en.sn + 1 << "  | ";

				
				
				// Вывод значения
				if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT) {
					cout << idtable.table[i].vint << " ";
					(*log.stream) << idtable.table[i].vint << " ";
				}
				else if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::STR) {
					cout << idtable.table[i].vstr.len << " " << idtable.table[i].vstr.str << " ";
					(*log.stream) << idtable.table[i].vstr.len << " " << idtable.table[i].vstr.str << " ";
				}
				else if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::BOOL) {
					cout << idtable.table[i].vbool << " ";
					(*log.stream) << idtable.table[i].vbool << " ";
				}

				cout << endl;
				(*log.stream) << endl;
			}
			cout << "---------------------------------------------------------------------------------------------------------" << endl;
			(*log.stream) << "---------------------------------------------------------------------------------------------------------" << endl;
			/*(*log.stream) << "\n--—------------------------------------------- Идентификаторы ---------------------------------------——-- " << endl;
			cout << "\n---------------------------------------------------------------------------------------------------------" << endl;
			cout << "|  Номер  |    id    | Тип данных |    Тип    | Связь (Номер Лексема Строка) | Значение?" << endl;
			cout << "---------------------------------------------------------------------------------------------------------" << endl;
			(*log.stream) << "|  Номер  |    id    | Тип данных |    Тип    | Связь (Номер Лексема Строка) | Значение?" << endl;
			(*log.stream) << "---------------------------------------------------------------------------------------------------------" << endl;

			for (int i = 0; i < idtable.size; i++) {
				cout << "| " << std::setfill('0') << std::setw(7) << i + 1 << " | ";
				(*log.stream) << "| " << std::setfill('0') << std::setw(7) << i + 1 << " | ";

				cout << std::setfill(' ') <<std::setw(8) << idtable.table[i].id << " | ";
				(*log.stream) << std::setfill(' ') << std::setw(8) << idtable.table[i].id << " | ";

				switch (idtable.table[i].iddatatype) {
				case IT::INT:
					cout << std::setw(10) << "int" << " | ";
					(*log.stream) << std::setw(10) << "int" << " | ";
					break;
				case IT::STR:
					cout << std::setw(10) << "str" << " | ";
					(*log.stream) << std::setw(10) << "str" << " | ";
					break;
				}

				switch (idtable.table[i].idtype) {
				case IT::F:
					cout << std::setw(9) << "func" << " | ";
					(*log.stream) << std::setw(9) << "func" << " | ";
					break;
				case IT::L:
					cout << std::setw(9) << "lit" << " | ";
					(*log.stream) << std::setw(9) << "lit" << " | ";
					break;
				case IT::P:
					cout << std::setw(9) << "param" << " | ";
					(*log.stream) << std::setw(9) << "param" << " | ";
					break;
				case IT::V:
					cout << std::setw(9) << "variable" << " | ";
					(*log.stream) << std::setw(9) << "variable" << " | ";
					break;
				}

				LT::Entry en = LT::GetEntry(lextable, idtable.table[i].idxfirstLE);
				cout << std::setw(9) <<idtable.table[i].idxfirstLE + 1 << std::setw(9) << en.lexema << std::setw(9) << en.sn << "  | ";
				(*log.stream) << std::setw(9) << idtable.table[i].idxfirstLE + 1 << std::setw(9) << en.lexema << std::setw(9) << en.sn + 1 << "  | ";

				if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT) {
					cout << idtable.table[i].value.vint << " ";
					(*log.stream) << idtable.table[i].value.vint << " ";
				}
				else if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::STR) {
					cout << idtable.table[i].value.vstr.len << " " << idtable.table[i].value.vstr.str << " ";
					(*log.stream) << idtable.table[i].value.vstr.len << " " << idtable.table[i].value.vstr.str << " ";
				}
				cout << endl;
				(*log.stream) << endl;
			}
			cout << "---------------------------------------------------------------------------------------------------------" << endl;
			(*log.stream) << "---------------------------------------------------------------------------------------------------------" << endl;*/



	}
}

