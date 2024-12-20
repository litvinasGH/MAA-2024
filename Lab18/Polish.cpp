#include "Polish.h"
#include "stdafx.h"
#include <queue>

namespace Polish {
	bool polishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable) {
		std::stack<LT::Entry> stack;
		std::queue<LT::Entry> queue;

		LT::Entry aggregate_symbol;
		aggregate_symbol.idxTI = -1;
		aggregate_symbol.lexema = '#';
		aggregate_symbol.sn = lextable.table[i].sn;

		LT::Entry function_symbol;
		function_symbol.idxTI = LT_TI_NULLIDX;
		function_symbol.lexema = '@';
		function_symbol.sn = lextable.table[i].sn;
		int idx;

		int lexem_counter = 0;
		int parm_counter = 0;
		int lexem_position = i;
		char* buf = new char[i];

		bool is_function = false;

		for (i; lextable.table[i].lexema != LEX_SEMICOLON; i++, lexem_counter++) {
			switch (lextable.table[i].lexema) {
			case LEX_ID:
			case LEX_LITERAL:
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::F) {
					is_function = true;
					idx = lextable.table[i].idxTI;
				}
				else {
					if (is_function)
						parm_counter++;
					queue.push(lextable.table[i]);
				}
				continue;

			case LEX_LEFTHESIS:
				stack.push(lextable.table[i]);
				continue;

			case LEX_RIGHTHESIS:
				while (stack.top().lexema != LEX_LEFTHESIS) {
					queue.push(stack.top());
					stack.pop();
					if (stack.empty())
						return false;
				}

				if (!is_function)
					stack.pop();
				else {
					function_symbol.idxTI = idx;
					idx = LT_TI_NULLIDX;
					lextable.table[i] = function_symbol;
					queue.push(lextable.table[i]);
					_itoa_s(parm_counter, buf, 2, 10);
					stack.top().lexema = buf[0];
					stack.top().idxTI = LT_TI_NULLIDX;
					stack.top().sn = function_symbol.sn;
					queue.push(stack.top());
					stack.pop();
					parm_counter = 0;
					is_function = false;
				}
				continue;

			case LEX_OPERATOR:
				while (!stack.empty() && lextable.table[i].priority <= stack.top().priority) {
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lextable.table[i]);
				continue;
			}
		}

		while (!stack.empty()) {
			if (stack.top().lexema == LEX_LEFTHESIS || stack.top().lexema == LEX_RIGHTHESIS)
				return false;

			queue.push(stack.top());
			stack.pop();
		}

		while (lexem_counter != 0) {
			if (!queue.empty()) {
				lextable.table[lexem_position++] = queue.front();
				queue.pop();
			}
			else
				lextable.table[lexem_position++] = aggregate_symbol;

			lexem_counter--;
		}

		for (int i = 0; i < lexem_position; i++) {
			if (lextable.table[i].lexema == LEX_OPERATOR || lextable.table[i].lexema == LEX_LITERAL)
				idtable.table[lextable.table[i].idxTI].idxfirstLE = i;
		}

		return true;
	}

	void startPolish(LT::LexTable& lextable, IT::IdTable& idtable) {
		for (int i = 0; i < lextable.size; i++) {
			if (lextable.table[i].lexema == '=') {
				polishNotation(i + 1, lextable, idtable);
			}

		}
	}
}