#pragma once
#ifndef MYTOKENH
#define MYTOKENH


#include<iostream>
using namespace std;
class myToken {
public:
	int type;
	string lexeme;
	int val;
	int offset;//���ž������������ַ��ƫ����
	myToken()
	{
		type = 0;
		lexeme = "";
		val = 0;
		offset = 0;
	}
	myToken(int a, string b, int c)
	{
		type = a;
		lexeme = b;
		val = c;
		offset = 0;
	}
};
#endif // !MYTOKEN.H