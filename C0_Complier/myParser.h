#pragma once
#include"myLexer.h"
#include"commandSet.h"
#include"mysymTable.h"
#include<iomanip>
#include<map>
#include<stack>
class func {
public:
	string name;//������
	int willreturn = 0;//�������Ƿ��з���ֵ
	int hasreturn = -1;//�������Ƿ�ʵ�ʷ���ֵ���﷨�������������ȷ��
	int varcnt = 0;//��������
	mysymTable funcTable;//��ǰ�������ű�
	commandSet commandset;//��ǰ����ָ���
	int loc = 0;//�����ĵ�ַ
	func(string s,int t):funcTable(s),commandset(s)
	{
		name = s;
		willreturn = t;
	}
};
class myParser {
public:
	myLexer lexer;
	commandSet mainSet;//main����ָ��ϣ�ͬʱҲ����Ϊȫ�ֱ������ٿռ�
	mysymTable globalTable;//ȫ�ֱ������ű�
	mysymTable mainTable;//main�������ű�
	myToken cur;//��ǰ���������
	vector<func>funcs;//��������
	int p = 0;//�Ƿ��ӡ������Ϣ
	int mainreturn = -1;//main�Ƿ����return
	vector<command>commandRes;//���յ�ָ��ջ
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
	void printsymTable();//��ӡ���з��ű�
	int sentence(string);
	void printsinglecommandstk(string, commandSet&);
	void printfcommandstk();//��ӡ����ָ��ջ
	int ifsentence(string);
	int expression(string);
	commandSet& findCommandSet(string);
	mysymTable& findsymTable(string);
	int lodIdent(commandSet&, mysymTable&, string,string);//Ϊָ����ָ��ջ���ɽ���ʶ��ȡ��ջ��������
	int whilesentence(string);
	int returnsentence(string);
	int returnRight(func&, commandSet&);//�жϺ�����return�Ƿ���ȷ
	int returnRight();//�ж�main������return�Ƿ���ȷ
	int scanfsentence(string);
	int printfsentence(string);
	int stoIdent(commandSet&, mysymTable&, string, string);//Ϊָ����ָ��ջ���ɽ�ջ��ֵ�浽��ʶ��������

};