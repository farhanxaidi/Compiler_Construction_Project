#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include"Table.h"

using namespace std;

int tokencount = 0;
int TransitionTable[52][25];
int *AsciiMapping = new int[256];
char TokenType[91][15];
int *StateInfo = new int[52];
char Keyword[60][15];

ifstream fp("file.txt");
int state = 0;
char ch;
int newstate = 0;
char *BUFFER = new char[700];
int Bufferptr = 0;
int index = 0;


void readTransitionTable()
{
	ifstream fp("TransitionTable.txt");
	for (int i = 0; i < 52; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			fp >> TransitionTable[i][j];
		}
	}

	fp.close();

}
void readKeyword()
{
	ifstream fp("Keywords.txt");
	for (int i = 0; i < 60; i++)
	{
		fp.getline(Keyword[i], 15);


	}


	fp.close();
}
void readAsciiMapping()
{
	ifstream fp("AsciiMapping.txt");
	for (int i = 0; i < 256; i++)
	{

		fp >> AsciiMapping[i];
	}


	fp.close();
}
void readTokenType()
{
	ifstream fp("TokenType.txt");
	for (int i = 0; i < 91; i++)
	{
		fp.getline(TokenType[i], 15);


	}


	fp.close();
}
void readStateInfo()
{
	ifstream fp("StateInfo.txt");
	for (int i = 0; i < 52; i++)
	{
		fp >> StateInfo[i];
	}
}
void readFile()
{
	while (!fp.eof())
	{

		BUFFER[index] = fp.get();
		index++;
	}
	BUFFER[index - 1] = '\0';



}
char readchr()
{
	if (BUFFER[Bufferptr] == '\0')
	{
		return NULL;

	}
	else return BUFFER[Bufferptr++];
}
void keyword_checking(int &count, string checking)
{
	for (int i = 0; i < 60; i++)
	{
		if (!strcmp(checking.c_str(), Keyword[i]))
		{
			count++;
			break;
		}
	}
}
bool checkIfDoubleOperator(string checking)
{
	for (int i = 71; i < 74; i++)
	{
		if (!strcmp(checking.c_str(), TokenType[i]))
			return true;
	}
	for (int i = 79; i < 87; i++)
	{
		if (!strcmp(checking.c_str(), TokenType[i]))
			return true;
	}
	return false;
}
bool is_number(const std::string s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}
void goLexical()
{
	readTransitionTable();
	readKeyword();
	readAsciiMapping();
	readTokenType();
	readStateInfo();
	readFile();
	ofstream outfile, outfile1;
	outfile.open("Tokens.txt");
	outfile1.open("Errors.txt", ios::out);
	string checking;
	int count1 = 0;
	int flagexpo = 0;

	Table table;

	while (1)
	{
		count1 = 0;
		checking.clear();

		state = 0;
		ch = readchr();

		if (ch == '\0') break;
		while (ch == ' ' || ch == '\n' || ch == '\t')
		{
			ch = readchr();
		}
		int flag = true;
		while (!StateInfo[state])
		{
			count1 = 0;
			if (ch == ';' && checking.length() <= 0)
			{
				outfile << ";" << "," << ";" << endl;
				flag = false;
				break;
			}
			if (ch == 'E' || ch == 'e')
			{
				int l = checking.length();
				if (checking[l - 1] >= '0'&&checking[l - 1] <= '9')
				{
					newstate = 15;
					flagexpo = 1;
				}
			}
			else
			{
				newstate = TransitionTable[state][AsciiMapping[ch]];
			}

			if (newstate == -100)
			{

				if (checking == "output")
				{

					if (ch == '<') {

						checking = checking + ch;
						ch = readchr();

						if (ch == '-')
						{
							checking = checking + ch;
							Bufferptr++;
						}
					}
				}

				if (checking == "input")

				{

					if (ch == '-') {


						checking = checking + ch;
						ch = readchr();
						if (ch == '>')
						{
							checking = checking + ch;
							Bufferptr++;
						}
					}
				}
				int count = 0;

				if (checking == "") {

					checking = checking + ch;
					Bufferptr++;

				}
				keyword_checking(count, checking);
				if (count > 0)
				{
					outfile << checking << ",Keyword" << endl;
					//cout << checking << ", Keyword" << endl;

					char *temp = new char[50];
					strcpy_s(temp, 50, checking.c_str());
					temp[checking.length()] = '\0';

					table.lookupInsert(temp);
					delete[] temp;

					Bufferptr--;
				}
				else
				{

					outfile1 << checking << ",Invalid Input" << endl;
					//cout << checking << " ,Invalid Input" << endl;


					Bufferptr--;
				}
				if (ch == '\0') Bufferptr++;

				count1 = 1;
				break;
			}
			if (!StateInfo[newstate])
			{
				if (ch != ' ' && ch != '\n' && ch != '\0' || checking[0] == '"')
				{
					checking = checking + ch;

				}
				ch = readchr();
			}
			else
			{
				if (ch == '\"')
				{
					checking = checking + ch;
					Bufferptr++;
				}
				if (ch >= 'a' && ch <= 'z')
				{

				}
				else if (ch >= '0' && ch <= '9')
				{

				}
				else if ((checking.length() == 1 || checking.length() == 0))  //&& (( ch>='9' && ch<='0' && ch!='-')))

				{
					if (checking.length() == 0)
					{
						Bufferptr++;
					}
					if (ch != ' ' && ch != '\n' && ch != '\0' && !is_number(checking))
					{
						checking = checking + ch;
					}
				}


			}
			state = newstate;
		}
		if (flag == false) continue;
		if (StateInfo[state] > 0) Bufferptr--;
		if (count1 == 0 && checking != "")
		{
			if (StateInfo[state] < 0)
			{
				Bufferptr--;
				StateInfo[state] = (StateInfo[state] * (-1));
			}
			if (checkIfDoubleOperator(checking))
			{
				Bufferptr++;
			}


			outfile << checking << "," << TokenType[StateInfo[state]] << "\n";
			//cout << checking << ", " << TokenType[StateInfo[state]] << "\n";

			char *temp = new char[50];
			strcpy_s(temp, 50, checking.c_str());
			temp[checking.length()] = '\0';
			table.lookupInsert(temp);
			delete[] temp;
			//outfile << temp << ", " << TokenType[StateInfo[state]] << "\n";
			if (ch == '\0') Bufferptr++;
			//Bufferptr++;
			int l = checking.length();
			if ((StateInfo[state] == 2 && ch == '\0') || (StateInfo[state] == 1 && ch == '\0'))
			{
				//Bufferptr++;  
				break;

			}



		}

	}
	outfile.close();
	outfile1.close();

	delete[] AsciiMapping;
	delete[] BUFFER;
	delete[] StateInfo;
}
