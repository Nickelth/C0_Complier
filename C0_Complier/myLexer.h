#pragma once
/*
欢迎来到词法分析器！:D
*/
#ifndef MYLEXERH
#define MYLEXERH
#include<iostream>
#include<string>
#include<fstream>
#include<ctype.h>
#include"mysymTable.h"
#include"errorReporter.h"
using namespace std;
class myLexer {
public:
	ifstream fin;
	string path;
	string buffer;
	int curpos = 0;
	int line = 0;
	myLexer(string);
	myToken getsym();
	int isKeyword(string);
	//int isIdent(string);
	//int isNumber(string);
	int isOperator(char);
	int isDelimiter(char);
};
#endif