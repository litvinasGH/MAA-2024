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




    void WriteLex(LOG log, LT::LexTable &lextable, IT::IdTable &idtable)
    {
        int str = 0;
        int size = [](int number) -> int
            {
                if (number == 0) return 1;

                int count = 0;
                while (number != 0) {
                    number /= 10;
                    count++;
                }
                return count;
            }(LT::GetEntry(lextable, lextable.size - 1).sn);
            (*log.stream) << "\n—--- Представление кода в виде лексем —--- " << endl;
            for (int i = 0; i < lextable.size; i++) {
                if (lextable.table[i].sn + 1 != str) {
                    *log.stream << endl;
                    cout << endl;
                    str = lextable.table[i].sn + 1;
                    *log.stream << std::setfill('0') << std::setw(size) << str-1 << ' ';
                    cout << std::setfill('0') << std::setw(size) << str -1<< ' ';
                }
                if (lextable.table[i].lexema == 'i' || lextable.table[i].lexema == 'n' || lextable.table[i].lexema == 'l')
                {
                    *log.stream << lextable.table[i].lexema << '|' << lextable.table[i].idxTI + 1 << '|';
                    cout << lextable.table[i].lexema << '|' << lextable.table[i].idxTI + 1 << '|';
                }
                else {
                    *log.stream << lextable.table[i].lexema;
                    cout << lextable.table[i].lexema;
                }
            }
            cout << endl << endl;
            size = [](int number) -> int {
                if (number == 0) return 1;

                int count = 0;
                while (number != 0) {
                    number /= 10;
                    count++;
                }
                return count;
                }(idtable.size);
                (*log.stream) << "\n—--- Идентификаторы —--- " << endl;
                cout << "Номер\tid\tТип данных\tТип\tСвязь с лексемой\t\t\tЗначение?" << endl;
                (*log.stream) << "Номер\tid\tТип данных\tТип\tСвязь с лексемой\t\t\tЗначение?" << endl;
                for (int i = 0; i < idtable.size; i++) {
                    *log.stream << std::setfill('0') << std::setw(size) << i + 1 << ' ';
                    cout << std::setfill('0') << std::setw(size) << i + 1 << ' ';
                    cout << idtable.table[i].id << " ";
                    *log.stream << idtable.table[i].id << " ";
                    switch (idtable.table[i].iddatatype) {
                    case IT::INT:
                        cout << "int ";
                        *log.stream << "int ";
                        break;
                    case IT::STR:
                        cout << "str ";
                        *log.stream << "str ";
                        break;
                    }
                    switch (idtable.table[i].idtype) {
                    case IT::F:
                        cout << "func ";
                        *log.stream << "func ";
                        break;
                    case IT::L:
                        cout << "lit ";
                        *log.stream << "lit ";
                        break;
                    case IT::P:
                        cout << "param ";
                        *log.stream << "param ";
                        break;
                    case IT::V:
                        cout << "variable ";
                        *log.stream << "variable ";
                        break;
                    case IT::LE:
                        cout << "lexem ";
                        *log.stream << "lexem ";
                        break;
                    }
                    LT::Entry en = LT::GetEntry(lextable, idtable.table[i].idxfirstLE);
                    cout << idtable.table[i].idxfirstLE << " " << en.lexema << " " << en.sn << " ";
                    *log.stream << idtable.table[i].idxfirstLE << " " << en.lexema << " " << en.sn << " ";
                    if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT) {
                        cout << idtable.table[i].vint << " ";
                        *log.stream << idtable.table[i].vint << " ";
                    }
                    else if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::STR) {
                        cout << idtable.table[i].vstr.len << " \"" << idtable.table[i].vstr.str << "\" ";
                        *log.stream << idtable.table[i].vstr.len << " \"" << idtable.table[i].vstr.str << "\" ";
                    }
                    cout << endl;
                    *log.stream << endl;
                }
    }
}

