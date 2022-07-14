#pragma once
#include<iostream>
#include<vector>
using namespace std;
class errorReporter {
public:
	vector<string>msg = {
		"无法识别的符号",
		"整数过大",
		"语法错误",
		"变量重定义",
		"函数重定义",
		"未定义标识符",
		"返回类型不一致",
		"函数未定义"
	};
	void repor(int line, int pos, int type,string s)
	{
		cout << "编译错误！第" << line << "行第" << pos << "列，字符"<<s<<",错误信息:" << msg[type] << endl;
		exit(0);
	}
};
extern errorReporter err;
