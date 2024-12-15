#include "stdafx.h"
#include "Out.h"

#pragma warning(disable:4996)

namespace Out
{
	OUT getout(wchar_t logfile[])
	{
		OUT out;
		out.stream = new ofstream;
		out.stream->open(logfile);
		if (!out.stream->is_open()) {
			throw ERROR_THROW(112);
		}
		wcscpy_s(out.outfile, logfile);
		return out;
	}
	void WriteIn(OUT out, In::IN in) {

		*out.stream << "Исходные данные" << endl;
		*out.stream << "Строка: " << in.text << endl;
		*out.stream << "Количество символов : " << in.size << endl;
		*out.stream << "Количество строк    : " << in.lines << endl;
		*out.stream << "Проигнорировано     : " << in.ignor << endl;
	}
	void WriteError(OUT out, Error::ERROR error)
	{
		if (out.stream)
		{
			*out.stream << " ----Ошибка---- " << endl;
			*out.stream << "Ошибка " << error.id << ": " << error.message << endl;
			if (error.inext.line != -1)
			{
				*out.stream << "Строка: " << error.inext.line << endl << "Столбец: " << error.inext.col << endl << endl;
			}
		}
		else
			cout << "Ошибка " << error.id << ": " << error.message << ", строка " << error.inext.line << ", позиция " << error.inext.col << endl << endl;
	}

	void Close(OUT out)
	{
		out.stream->close();
		delete out.stream;
	}

	
}