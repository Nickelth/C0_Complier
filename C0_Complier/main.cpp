/*
һ�����C0���Եı������������Ԫ�����ʽܡ������ָ�ͬ����!:P
*/
#include<iostream>
#include<fstream>
#include"myLexer.h"
#include"myParser.h"
#include"CommandStack.h"
using namespace std;
string path;
int main()
{
	
	cout << "��ӭʹ��C0�������������Դ�����ļ��ľ���·����" << endl;
	cout << "���磺D:\\file1\\file2\\target.txt" << endl;
	cout << "·����";
	cin >> path;
	myParser parser(path);
	cout << "�Ƿ��ӡ�﷨����������Ϣ����y/n��" << endl;
	char op;
	cin >> op;
	if (op == 'y'||op=='Y')parser.p = 1;
	parser.run();
	cout << "Ŀ�����������ϣ��Ƿ����н��ͳ���(y/n)" << endl;
	char ch;
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
	{
		CommandStack stk;
		stk.run();
	}

}