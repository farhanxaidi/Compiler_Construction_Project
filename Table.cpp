#include "Table.h"

Table::Table()
{
	for (int i = 0; i < HT_SIZE; i++)
	{
		ht[i] = 0;
	}
	first = last = 0;
	count = 0;
}
unsigned Table::hash(char *s)
{
	unsigned hval = 0;
	while (*s != '\0')
	{
		hval = (hval << 4) + *s;
		s++;
	}
	return hval;
}
Symbol * Table::clear()
{
	Symbol* list = first;
	for (int i = 0; i<HT_SIZE; i++)
		ht[i] = 0;
	first = last = 0;
	count = 0;
	return list;
}
Symbol * Table::lookup(char * s)
{
	unsigned h = hash(s);
	return lookup(s, h);
}
Symbol * Table::lookup(char * s, unsigned h)
{
	// Lookup name s with hash value h
	// Hash value is passed to avoid its computation

	unsigned index = h % HT_SIZE;
	Symbol* sym = ht[index];
	while (sym != 0) {
		if (strcmp(sym->name, s) == 0)
			break;
		sym = sym->next;
	}
	return sym;
}
Symbol * Table::lookupInsert(char * s) // bas isko use karna ha
{

	unsigned h = hash(s);	// Computed once
	Symbol* sym; sym = lookup(s, h);	// Locate symbol first
	if (sym == 0)
	{
		// If not found
		sym = insert(s, h);// Insert a new symbol
	}
	return sym;
}
Symbol * Table::insert(char * s, unsigned h)
{

	unsigned index = h % HT_SIZE;
	Symbol* sym = new Symbol(s);
	sym->next = ht[index];
	ht[index] = sym;
	if (count == 0)
	{
		first = last = sym;
	}
	else
	{
		last->list = sym;
		last = sym;
	}
	count++;
	return sym;

}
Table::~Table()
{

}
