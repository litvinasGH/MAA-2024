#include "stdafx.h"
#include "In.h"
#include "Error.h"
#include "Out.h"



using namespace std;
using namespace Out;

namespace In
{
	IN getin(wchar_t infile[])
	{
		bool squat = false;
		IN in;
		OUT out;
		in.size = 0; in.lines = 1; in.ignor = 0;
		out.size = 0;
		int col = 1;
		int counter = 0;
		unsigned char* text = new unsigned char[IN_MAX_LEN_TEXT];

		ifstream fin(infile);
		if (!fin.is_open())
			throw ERROR_THROW(110);

		while (in.size < IN_MAX_LEN_TEXT)
		{
			char c;
			fin.get(c);
			unsigned char x = c;

			if (fin.eof())
			{
				if (text[in.size - 1] == ' ')
				{
					text[in.size - 1] = '\0';
					in.size--;
				}
				text[in.size] = '\0';
				break;
			}
			switch (in.code[x])
			{
			case in.T:
				
				if (x == ' ' && in.size == 0)
				{
					continue;
				}
				
				if (x == ' ' && counter != 1 && squat == false)
				{
					text[in.size] = x;
					out.size++;
					in.size++;
					col++;
					counter++;
					continue;
				}
				else if (x == ' ' && counter == 1 && squat == false)
				{
					continue;
				}
				
				
				else 
				{
					if (x == (unsigned char)'\'')
					{
						squat = !squat;
					}
					text[in.size] = x;
					out.size++;
					in.size++;
					col++;
					counter = 0;
				}
				break;
			case in.I:
				in.ignor++;
				break;
			case in.F:
				text[in.size] = (unsigned char)'\0';
				throw ERROR_THROW_IN(111, in.lines, col, text);
				break;
			default:
				text[in.size] = in.code[x];
				in.size++;
				col++;
				break;
			}
			if (x == IN_CODE_ENDL || x == '|')
			{
				in.lines++;
				col = 1;
			}

		}
		in.text = text;


		string word = "";
		for (int i = 0; i < in.size; i++)
		{
			if (in.text[i] == '\0')
			{
				word += '\0';
				in.words.push_back(word);
				word = "";
				break;
			}
			else if (in.text[i] == '\n' || in.text[i] == '|')
			{
				in.words.push_back(word);
				word = "";
				word += '|';
				in.words.push_back(word);
				in.line++;
				word = "";
			}
			else if(
				   in.text[i] == ' ' 
				|| in.text[i] == '(' 
				|| in.text[i] == ')' 
				|| in.text[i] == '{' 
				|| in.text[i] == '}' 
				|| in.text[i] == ';' 
				|| in.text[i] == ',' 
				|| in.text[i] == '+' 
				|| in.text[i] == '-' 
				|| in.text[i] == '*' 
				|| in.text[i] == '/'
				|| in.text[i] == '='
				|| in.text[i] == '|'
				|| in.text[i] == '\n')
			{
				if (word == "")
				{
					word += in.text[i];
					in.words.push_back(word);
					word = "";
				}
				else
				{
					in.words.push_back(word);
					word = "";
					word += in.text[i];
					in.words.push_back(word);
					word = "";
				}
			}
			else
			{
				word += in.text[i];
			}

		}
		
		return in;
	}
}