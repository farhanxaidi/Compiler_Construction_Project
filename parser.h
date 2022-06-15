#pragma once
#include"Stack.h"
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

int tokennumber = 0;
int terminalnumbers = 1;
int nonterminalsnumber = 0;
string tokenarray[500];
string terminalsarray[28];
string nonterminalsarray[29];
int parsetablearray[29][27];
int cfgarray[58][12];

void readTokens()
{

	ifstream fin;
	fin.open("Tokens.txt");

	while (!fin.eof())
	{
		getline(fin, tokenarray[tokennumber]);
		if (tokenarray[tokennumber] == "") break;
		tokennumber++;
	}


}
void readterminals()
{

	ifstream fin;
	fin.open("terminals.txt");

	while (!fin.eof())
	{
		getline(fin, terminalsarray[terminalnumbers]);
		terminalnumbers++;
	}



}
void readnonterminals()
{


	ifstream fin;
	fin.open("non-terminals.txt");

	while (!fin.eof())
	{
		getline(fin, nonterminalsarray[nonterminalsnumber]);
		nonterminalsnumber;
	}

}
void readparsetable()
{

	ifstream fin;
	fin.open("parseTable.txt");

	for (int i = 0; i < 29; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			fin >> parsetablearray[i][j];
			//cout << parsetablearray[i][j]<<" ";
		}
		//cout << endl;
	}

}
void readcfg()
{
	ifstream fin;
	fin.open("cfg.txt");

	for (int i = 0; i < 58; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			fin >> cfgarray[i][j];
			//cout << cfgarray[i][j] << " ";
			if (cfgarray[i][j] == -1000) break;
		}
		//cout << endl;
	}


}
int gettoken()
{
	if (tokencount >= tokennumber)
		return NULL;
	else
		return tokencount++;

}
void goParser()
{
	readTokens();
	readnonterminals();
	readterminals();
	readparsetable();
	readcfg();
	ofstream fout;
	fout.open("parsererrors.txt");

	Stack stack;
	stack.push(0);
	while (tokencount != tokennumber)
	{
		int no = gettoken();
		string strtemp = tokenarray[no];
		char *str = new char[strtemp.length()];
		char* nextTok;
		strcpy_s(str, strtemp.length()+1, strtemp.c_str());
		char *token = strtok_s(str, ",", &nextTok);
		char *type = strtok_s(NULL, "\0", &nextTok);

		if ((token[0] == '+' || token[0] == '-') && !strcmp(type, "number") && tokenarray[no-1][0] != '+' && tokenarray[no - 1][0] != '-')
		{
			for (int i = 1; i < (int)strlen(token); i++)
			{
				tokenarray[no] += token[i];
			}
			tokenarray[no] =  + ",number";
			tokencount--;
			if (token[0] == '+')
			{
				strcpy_s(token, 2, "+");
				strcpy_s(type, 2, "+");
			}
			if (token[0] == '-')
			{
				strcpy_s(type, 2, "-");
				strcpy_s(type, 2, "-");
			}
		}

		int tokenNo = 999;

		for (int i = 1; i <= 27; i++)
		{
			if (!strcmp(token, terminalsarray[i].c_str()))
			{
				tokenNo = i;
				break;
			}
		}
		if (!strcmp(type, "number")) tokenNo = 4;
		if (!strcmp(type, "identifier")) tokenNo = 3;

		if (tokenNo == 999)
		{
			fout << token << "," << type <<" <- Error"<<endl;
			cout << token << "," << type << " <- Error" << endl;
			continue;
		}
		tokenNo--;
		int count = 0;
		while (1) {
		
			int var = stack.pop();

			if (var < 0)
			{
				int newno = var*(-1);
				if (tokenNo+1 == var*(-1))
				{
					cout << token << "," << type << " <- Parsed" << endl;
					break;
				}
			}
			if (parsetablearray[var][tokenNo] >= 0)
			{
				if (var == 20)
				{
					no = gettoken();
					strtemp = tokenarray[no];
					char *str1 = new char[strtemp.length()];
					strcpy_s(str1, strtemp.length() + 1, strtemp.c_str());
					char *temptoken = strtok_s(str1, ",", &nextTok);
					if (!strcmp(temptoken, ":="))
					{
						parsetablearray[var][tokenNo] = 35;
					}
					else
					{
						parsetablearray[var][tokenNo] = 36;
					}
					tokencount--;
				}
				int reversearr[12], j = 0;
				for (int i = 0; i < 12; i++)
				{
					if (cfgarray[parsetablearray[var][tokenNo]][i] == -1000 || cfgarray[parsetablearray[var][tokenNo]][i] == 1000)
						break;
					if (cfgarray[parsetablearray[var][tokenNo]][i] < 0)
					{
						int num = cfgarray[parsetablearray[var][tokenNo]][i];
						num = num * (-1);
						if (tokenNo + 1 == num)
						{
							cout << token << "," << type << " <- Parsed" << endl;
							count++;
							continue;
						}
					}
					reversearr[j++] = cfgarray[parsetablearray[var][tokenNo]][i];
				}
				for (int i = j - 1; i >= 0; i--)
				{
					stack.push(reversearr[i]);
				}
			}
			else if (parsetablearray[var][tokenNo] == -998) // pop
			{

			}
			else if (parsetablearray[var][tokenNo] == -997) // skip
			{
				fout << token << "," << type << " <- Error" << endl;
				cout << token << "," << type << " <- Error" << endl;
				break;
			}
			if (count>0)
			{
				break;
			}
		}
	}
	fout.close();
}