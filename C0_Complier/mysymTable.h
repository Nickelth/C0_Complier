#pragma once
//符号表
#ifndef MYSYMTABLEH
#define MYSYMTABLEH


#include<iostream>
#include<vector>
#include<algorithm>
#include"myToken.h"
using namespace std;
class mysymTable
{
public:
	int nul = 0, ident = 1, number = 2, plus = 3, minus = 4, multiply = 5, divide = 6,
		assign = 7, lparen = 8, rparen = 9, comma = 10, semicolon = 11, lbrace = 12,
		rbrace = 13;
	int elsesym = 14, ifsym = 15, intsym = 16, mainsym = 17, printfsym = 18, 
		returnsym = 19, scanfsym = 20,voidsym = 21, whilesym = 22;
	string belong;//当前符号表属于哪个域(全局、自定义函数、main函数)
	vector<myToken>words;//保存符号

	mysymTable(string);
	int findWord(string);//查找符号
	myToken& getWord(string);//获取符号
	string findsymType(int);
};
extern mysymTable keywordsymTable;//关键字的符号表

#endif // !MYSYMTABLEH