#pragma once
//���ű�
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
	string belong;//��ǰ���ű������ĸ���(ȫ�֡��Զ��庯����main����)
	vector<myToken>words;//�������

	mysymTable(string);
	int findWord(string);//���ҷ���
	myToken& getWord(string);//��ȡ����
	string findsymType(int);
};
extern mysymTable keywordsymTable;//�ؼ��ֵķ��ű�

#endif // !MYSYMTABLEH