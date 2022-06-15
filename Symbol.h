#include<string>

#pragma once
class Symbol
{
	friend class Table;
private:
	char   name[15];	
	Symbol* list;	 
	Symbol* next;	
public:
	Symbol();
	Symbol(char* s);	
	const char* id();	
	Symbol* nextinlist();	
	Symbol* nextinbucket();	
	~Symbol(); 
};

