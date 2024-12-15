#include "stdafx.h"
#include <iostream>
#include <time.h>

#include <iomanip>
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "RUS");

    Log::LOG log = Log::INITLOG;
    Out::OUT out;
    try
    {

        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        Log::WriteLine(log, (char*)"Тест", (char*)" без ошибок \n", "");
        Log::WriteLine(log, (wchar_t*)L"Тест", (wchar_t*)L" без ошибок \n", L"");
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        out = Out::getout(parm.out);
        In::IN in = In::getin(parm.in);
        cout << in.text << endl;                                                                                                              
        cout << "Всего символов: " << in.size << endl;
        cout << "Всего строк: " << in.lines << endl;
        cout << "Пропущено: " << in.ignor << endl;

        Log::WriteIn(log, in);
        Out::WriteIn(out, in);
        Out::Close(out);
        LT::LexTable lextable = LT::Create(LT_MAXSIZE);
        IT::IdTable idtable = IT::CreateT(TI_MAXSIZE);
        
        FST::CheckL(in, lextable, idtable);
        
        Log::WriteLex(log, lextable, idtable);

        MFST::Mfst mfst(lextable, GRB::getGreibach());
        mfst.start(log);
        mfst.savededucation();
        mfst.printrules(log);

       

    }
    catch (Error::ERROR e)
    {
        if (e.id == 111) {
            cout << e.inext.text << endl;
            cout << "Ошибка " << e.id << ": " << e.message << endl;
            cout << "Строка " << e.inext.line + 1 << " позиция " << e.inext.col + 1 << "\n\n";
            Out::WriteError(out, e);
        }
        else if (e.id == 118) {
            cout << e.inext.text << endl;
            cout << "Ошибка " << e.id << ": " << e.message << endl;
            cout << "Строка " << e.inext.line + 1 << std::endl << "Лексема \"" << e.inext.text << "\"\n\n";
        }
        else {
            cout << "Ошибка " << e.id << ": " << e.message << endl;
        }
        if (log.stream != NULL)
            Log::WriteError(log, e);
        exit(e.id);
    }
    Log::Close(log);
    system("pause");

    return 0;
}
