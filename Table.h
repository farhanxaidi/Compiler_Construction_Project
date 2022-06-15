#pragma once
#include"Symbol.h"
const unsigned HT_SIZE = 500;

class Table
{
private:
	Symbol*  ht[HT_SIZE];	
	Symbol*  first;	
	Symbol*  last;	
	unsigned count;	
public:
	Table();
	unsigned hash(char * s);
	Symbol* clear();	
	Symbol* lookup(char*s);	
	Symbol* lookup(char*s, unsigned h);	
	Symbol* insert(char*s, unsigned h);	
	Symbol* lookupInsert(char*s);	
	Symbol* symlist() { return first; }	
	unsigned symbols() { return count; }	
	~Table();
};

