#pragma once
#include<iostream>
#include<vector>
using namespace std;
class errorReporter {
public:
	vector<string>msg = {
		"�޷�ʶ��ķ���",
		"��������",
		"�﷨����",
		"�����ض���",
		"�����ض���",
		"δ�����ʶ��",
		"�������Ͳ�һ��",
		"����δ����"
	};
	void repor(int line, int pos, int type,string s)
	{
		cout << "������󣡵�" << line << "�е�" << pos << "�У��ַ�"<<s<<",������Ϣ:" << msg[type] << endl;
		exit(0);
	}
};
extern errorReporter err;
