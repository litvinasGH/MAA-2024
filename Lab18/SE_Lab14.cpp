#include "stdafx.h"
#include <iostream>
#include <time.h>
#include "LEX.h"
#include <iomanip>
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "GEN.h"
#include "Polish.h"
#include <ctime>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "RUS");
    system("color");

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
        LT::LexTable lextable = LT::Create(LT_MAXSIZE);
        IT::IdTable idtable = IT::CreateT(TI_MAXSIZE);
        FST::CheckL(in, lextable, idtable);
 
        Log::WriteLex(log, lextable, idtable);
        LT::LexTable newlex = LT::Create(LT_MAXSIZE);
        IT::IdTable newtable = IT::CreateT(TI_MAXSIZE);
        for (int i = 0; i < lextable.size; i++)
        {
            newlex.table[i] = lextable.table[i];
        }
        newlex.size = lextable.size;
        newtable.size = idtable.size;
        for (int i = 0; i < idtable.size; i++)
        {
            newtable.table[i] = idtable.table[i];
        }
        MFST::Mfst mfst(lextable, GRB::getGreibach());
        mfst.start(log);
        mfst.savededucation();
        mfst.printrules(log);
        Polish::startPolish(newlex, newtable);
        int bingus = 0;
        for (int i = 0; i < newtable.size; i++) {
            if (newtable.table[i].idtype == IT::L) {
                int g = 0;
                int l = bingus;
                int h = 0;
                char* nametmp = new char[6];
                nametmp[g++] = 'L';
                char* tmp_number = new char[6];
                if (bingus == 0) {
                    nametmp[g++] = '0';
                }

                else {
                    while (l> 0) {
                        tmp_number[h++] = l % 10 + '0';
                        l /= 10;
                    }
                    tmp_number[h] = '\0';
                    h--;
                    while (h >= 0) {

                        nametmp[g++] = tmp_number[h--];
                    }
                }

                nametmp[g] = '\0';
                bingus++;
                newtable.table[i].id = nametmp;
            }
            else if (newtable.table[i].idtype == IT::V) {
                char* name = new char[6];
                int g = 0;
                while(newtable.table[i].id[g] != '\0') {
                    name[g] = newtable.table[i].id[g];
                    g++;
                }
                name[g++] = newtable.table[i].view.top()[0];
                name[g] = '\0';
                newtable.table[i].id = name;
            }
        }

        Gen::Generator Generate(newlex, newtable, parm.out);
        In::Delete(in);
        Out::Close(out);
        return 0;
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
        system("pause");
    }
    Log::Close(log);
    system("pause");

    return 0;
}
