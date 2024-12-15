#pragma once
#include <fstream>
#include "In.h"
#include "Parm.h"
#include "Error.h"
using namespace std;

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		ofstream* stream;
		int size;
		unsigned char* text;
	};

	static const OUT INITOUT{ L"", NULL };
	OUT getout(wchar_t OUTfile[]);
	void WriteLin(OUT out, const char* c, ...);
	void WriteLi(OUT out, const wchar_t* c, ...);
	void Writeout(OUT out);
	void WriteParm(OUT out, Parm::PARM parm);
	void WriteIn(OUT out, In::IN in);
	void WriteError(OUT out, Error::ERROR error);
	void Close(OUT out);
};