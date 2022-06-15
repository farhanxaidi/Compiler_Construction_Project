#pragma once
#include<iostream>
using namespace std;

#define MAX 1000 //max size for stack

class Stack
{
	int top;
public:
	int myStack[MAX]; //stack array

	Stack() { top = -1; }
	bool push(int x);
	int pop();
	bool isEmpty();
};
//pushes element on to the stack
bool Stack::push(int item)
{
	if (top >= (MAX - 1)) {
		cout << "Stack Overflow!!!";
		return false;
	}
	else {
		myStack[++top] = item;
		return true;
	}
}

//removes or pops elements out of the stack
int Stack::pop()
{
	if (top < 0) {
		cout << "Stack Underflow!!";
		return 0;
	}
	else {
		int item = myStack[top--];
		myStack[top+1] = -1;
		return item;
	}
}

//check if stack is empty
bool Stack::isEmpty()
{
	return (top < 0);
}


