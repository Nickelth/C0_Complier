#pragma once
#include<iostream>
#include<vector>
using namespace std;
class command {
public:
	int indice;
	string op;
	int adr1, adr2;
	string calfunc="";//һ������ı���������CAL����д洢Ҫ��ת�ĺ�����
	command(string a, int b, int c)
	{
		indice = 0;
		op = a;
		adr1 = b;
		adr2 = c;
	}
};
class commandSet {
public:
	string belong;
	vector<command>stk;
	commandSet(string s)
	{
		belong = s;
	}
};
