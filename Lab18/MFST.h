#pragma once
#include "stdafx.h"
#include "Log.h"
#include "GRB.h"
#include <stack>
//#include "Rules.h"
//#include "GRB.h"

class my_stack_SHORT : public stack<short> {
public:
    using std::stack<short>::c; // доступ к базовому классу
};

#define MFST_DIAGN_MAXSIZE 2 * ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024];

#define MFST_TRACE_START cout << left << "Этап" << ":"\
                                << left << " Правило"\
                                << left << " Входная лента"\
                                << left << " Стек\n";

#define MFST_TRACE1         cout << left << ++FST_TRACE_n << ": "\
                                    << left << rule.getCRule(rbuf, nrulechain)\
                                    << left << getCLenta(lbuf, lenta_position)\
                                    << left << getCSt(sbuf)\
                                    << endl;

#define MFST_TRACE2         cout << left << FST_TRACE_n << ": "\
                                    << left\
                                    << left << getCLenta(lbuf, lenta_position)\
                                    << left << getCSt(sbuf)\
                                    << endl;

#define MFST_TRACE3         cout << left << ++FST_TRACE_n << ": "\
                                    << left\
                                    << left << getCLenta(lbuf, lenta_position)\
                                    << left << getCSt(sbuf)\
                                    << endl;

#define MFST_TRACE4(c)      cout << left << ++FST_TRACE_n << ": " << left << c << endl;
#define MFST_TRACE5(c)      cout << left << FST_TRACE_n << ": " << left << c << endl;
#define MFST_TRACE6(c, k)   cout << left << FST_TRACE_n << ": " << left << c << k << endl;

#define MFST_TRACE7         cout << left << state.lenta_position << ": "\
                                    << left << rule.getCRule(rbuf, state.nrulechain) << endl;

typedef my_stack_SHORT MFSTSTSTACK;

namespace MFST {
    struct MfstState {
        short lenta_position;       // позиция ленты для работы
        short nrule;                // индекс текущего правила
        short nrulechain;           // индекс текущей цепочки правил
        MFSTSTSTACK st;             // стек автомата
        MfstState();
        MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain); // (позиция на ленте; стек автомата; индекс текущей цепочки текущего правила)
        MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain); // (позиция на ленте; стек автомата; индекс текущего правила; индекс текущей цепочки текущего правила)
    };
    class my_stack_MfstState : public stack<MfstState> {
    public:
        using std::stack<MfstState>::c;
    };
    struct Mfst {
        enum RC_STEP {
            NS_OK,                  // найдено правило и цепочка, цепочка добавлена в стек
            NS_NORULE,              // не найдено правило грамматики (ошибка в грамматике)
            NS_NORULECHAIN,         // не найдена подходящая цепочка правила (ошибка в исходном коде)
            NS_ERROR,               // несоответствующий идентификатор грамматики
            TS_OK,                  // текущее состояние ленты == ожидаемое состояние, продвигаем ленту, pop стек
            TS_NOK,                 // текущее состояние ленты != ожидаемое состояние, продвигаем ленту, pop стек
            LENTA_END,              // текущая позиция ленты >= lenta_size
            SURPRISE,               // неожиданный тип возврата (ошибка в step)
        };
        struct MfstDiagnosis {
            short lenta_position;    // позиция на ленте
            RC_STEP rc_step;         // тип возвращаемого значения этапа
            short nrule;             // индекс правила
            short nrule_chain;       // индекс цепочки правила
            MfstDiagnosis();
            MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
        } diagnosis[MFST_DIAGN_NUMBER];

        GRBALPHABET* lenta;          // подлежащая (TN/NS) лента (из LEX)
        short lenta_position;        // текущая позиция на ленте
        short nrule;                 // индекс текущего правила
        short nrulechain;            // индекс текущей цепочки, текущего правила
        short lenta_size;            // размер ленты
        GRB::Greibach grebach;       // грамматика Грейбаха
        LT::LexTable lex;            // таблица для хранения лексем
        MFSTSTSTACK st;              // стек автомата
        my_stack_MfstState storestate; // стек для хранения текущего состояния
        Mfst();
        Mfst(LT::LexTable& plex, GRB::Greibach pgrebach);
        char* getCSt(char* buf);     // получить содержимое стека
        char* getCLenta(char* buf, short pos, short n = 25);  // лента: n символов с pos
        char* getDiagnosis(short n, char* buf, Log::LOG log); // получить n-ю строку диагностики или 0
        bool savestate();            // сохранить текущее состояние автомата
        bool resetstate();           // восстановить текущее состояние автомата
        bool push_chain(GRB::Rule::Chain chain); // поместить цепочку правила в стек
        RC_STEP step();              // выполнить шаг автомата
        bool start(Log::LOG log);    // запустить автомат
        bool savediagnosis(RC_STEP pprc_step); // тип возвращаемого значения этапа
        void printrules(Log::LOG log); // выводить промежуточную информацию по правилам

        struct Deducation {
            short size;
            short* nrules;
            short* nrulechains;
            Deducation() {
                size = 0;
                nrules = 0;
                nrulechains = 0;
            };
        } deducation;

        bool savededucation();
    };
}
