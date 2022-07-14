#pragma once
#include"myLexer.h"
#include"commandSet.h"
#include"mysymTable.h"
#include<iomanip>
#include<map>
#include<stack>
class func {
public:
	string name;//函数名
	int willreturn = 0;//声明中是否有返回值
	int hasreturn = -1;//代码中是否实际返回值，语法分析结束后才能确定
	int varcnt = 0;//变量个数
	mysymTable funcTable;//当前函数符号表
	commandSet commandset;//当前函数指令集合
	int loc = 0;//函数的地址
	func(string s,int t):funcTable(s),commandset(s)
	{
		name = s;
		willreturn = t;
	}
};
class myParser {
public:
	myLexer lexer;
	commandSet mainSet;//main函数指令集合，同时也负责为全局变量开辟空间
	mysymTable globalTable;//全局变量符号表
	mysymTable mainTable;//main函数符号表
	myToken cur;//当前的输入符号
	vector<func>funcs;//函数集合
	int p = 0;//是否打印调试信息
	int mainreturn = -1;//main是否读到return
	vector<command>commandRes;//最终的指令栈
	myParser(string s):lexer(s),globalTable("global"),mainTable("main") ,mainSet("main"){}
	void run();
	int findFunc(string);
	func& getFunc(string);
	int program();
	int S2(myToken);
	int S3();
	int S5(string);
	int S4();
	int subprogram(string);
	int vardefine(string);
	int sentenceseq(string);
	void printfsinglesymTable(string,mysymTable&);
	void printsymTable();//打印所有符号表
	int sentence(string);
	void printsinglecommandstk(string, commandSet&);
	void printfcommandstk();//打印所有指令栈
	int ifsentence(string);
	int expression(string);
	commandSet& findCommandSet(string);
	mysymTable& findsymTable(string);
	int lodIdent(commandSet&, mysymTable&, string,string);//为指定的指令栈生成将标识符取到栈顶的命令
	int whilesentence(string);
	int returnsentence(string);
	int returnRight(func&, commandSet&);//判断函数的return是否正确
	int returnRight();//判断main函数的return是否正确
	int scanfsentence(string);
	int printfsentence(string);
	int stoIdent(commandSet&, mysymTable&, string, string);//为指定的指令栈生成将栈顶值存到标识符的命令

};