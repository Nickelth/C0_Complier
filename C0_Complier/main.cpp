/*
一个针对C0语言的编译器，由李纪元、韩允杰、苏日乐格共同开发!:P
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
	
	cout << "欢迎使用C0编译程序，请输入源代码文件的绝对路径：" << endl;
	cout << "比如：D:\\file1\\file2\\target.txt" << endl;
	cout << "路径：";
	cin >> path;
	myParser parser(path);
	cout << "是否打印语法分析调试信息？（y/n）" << endl;
	char op;
	cin >> op;
	if (op == 'y'||op=='Y')parser.p = 1;
	parser.run();
	cout << "目标代码生成完毕！是否运行解释程序？(y/n)" << endl;
	char ch;
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
	{
		CommandStack stk;
		stk.run();
	}

}