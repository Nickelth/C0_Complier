#include"CommandStack.h"
#include<windows.h>
#include<fstream>
#include<string>
void CommandStack::readCommand()
{
	string path = "stackCommand.txt";
	ifstream fin;
	fin.open(path, ios::in);
	if (!fin)
	{
		cerr << "无法打开文件!" << endl;
		exit(0);
	}
	string com;
	string adr1, adr2;
	int cnt = 0;
	while (fin >> com >> adr1 >> adr2)
	{
		c_stk.push_back(make_tuple(cnt, com, stoi(adr1), stoi(adr2)));
		cnt++;
	}
	fin.close();

}
void CommandStack::findNextCommand(int& nxt)
{
	tuple<int, string, int, int> cur = c_stk[ne];
	int indice = get<0>(cur);
	string name = get<1>(cur);
	int adr1 = get<2>(cur);
	int adr2 = get<3>(cur);
	if (name == "CAL")
	{
		ne = adr2;
	}
	else if (name == "JMP")
	{
		ne = adr2;
	}
	else if (name == "JPC")
	{
		tuple<int, int, int>t = d_stk.getTop();

		if (get<2>(t) == 0)
		{
			ne = adr2;
		}
		else
		{
			ne++;
		}
	}
	else if (name == "RET")
	{

		if (d_stk.cur_layer == 0)
		{
			ne = -1;
			return;
		}
		ne = d_stk.getCurrentBaseValue() + 1;
	}
	else
	{
		ne++;
	}
}
void CommandStack::run()
{
	cout << "是否输出调试信息?y/n" << endl;
	string ch;
	cin >> ch;
	if (ch == "y")
	{
		outputmessage = 1;
	}
	else
	{
		outputmessage = 0;
	}
	readCommand();
	if (outputmessage)
	{
		cout << "展示指令栈如下:" << endl;
		for (auto i : c_stk)
		{
			cout << get<0>(i) << ' ' << get<1>(i) << ' ' << get<2>(i) << ' ' << get<3>(i) << endl;
		}
	}
	ne = 0;
	d_stk.base.push_back(0);
	while (ne != -1)
	{
		tuple<int, string, int, int> cur = c_stk[ne];
		int indice = get<0>(cur);
		string name = get<1>(cur);
		int adr1 = get<2>(cur);
		int adr2 = get<3>(cur);
		if (outputmessage)
		{
			cout << "当前命令: " << indice << ' ' << name << ' ' << adr1 << ' ' << adr2 << endl;
		}
		int cur_com = ne;
		findNextCommand(ne);

		if (name == "LIT")
		{
			d_stk.pushTop(adr2);
		}
		else if (name == "LOD")
		{
			tuple<int, int, int> t = d_stk.getValue(adr1, adr2);
			d_stk.pushTop(get<2>(t));
		}
		else if (name == "STO")
		{
			auto t = d_stk.getTop();
			d_stk.setValue(adr1, adr2, get<2>(t));
		}
		else if (name == "CAL")
		{
			d_stk.CallFunc(cur_com);
		}
		else if (name == "INT")
		{
			for (int i = 0; i < adr2; i++)
			{
				d_stk.pushTop(0, i);
			}
			if (d_stk.cur_layer == 0)//if we use INT in main,we will save -1,-1,return space in 0,1,2
			{
				d_stk.setValue(d_stk.cur_layer, 0, -1);
				d_stk.setValue(d_stk.cur_layer, 1, -1);
				d_stk.setValue(d_stk.cur_layer, 2, 0);
			}
			else//else we will save back point and which layer call this function in 0,1
			{
				d_stk.setValue(d_stk.cur_layer, 0, d_stk.backpoint);
				d_stk.setValue(d_stk.cur_layer, 1, d_stk.cur_layer - 1);
			}
		}
		else if (name == "JMP")
		{
			//do nothing
		}
		else if (name == "JPC")
		{
			//do nothing
		}
		else if (name == "ADD")
		{
			auto t1 = d_stk.getTop();
			d_stk.popTop();
			auto t2 = d_stk.getTop();
			d_stk.popTop();
			int res = get<2>(t2) + get<2>(t1);
			d_stk.pushTop(res);
		}
		else if (name == "SUB")
		{
			auto t1 = d_stk.getTop();
			d_stk.popTop();
			auto t2 = d_stk.getTop();
			d_stk.popTop();
			int res = get<2>(t2) - get<2>(t1);
			d_stk.pushTop(res);
		}
		else if (name == "MUL")
		{
			auto t1 = d_stk.getTop();
			d_stk.popTop();
			auto t2 = d_stk.getTop();
			d_stk.popTop();
			int res = get<2>(t2) * get<2>(t1);
			d_stk.pushTop(res);
		}
		else if (name == "DIV")
		{
			auto t1 = d_stk.getTop();
			d_stk.popTop();
			auto t2 = d_stk.getTop();
			d_stk.popTop();
			if (get<2>(t1) == 0)
			{
				cerr << "除0错误!" << endl;
				exit(1);
			}
			int res = get<2>(t2) / get<2>(t1);
			d_stk.pushTop(res);
		}
		else if (name == "RED")
		{
			cout << "请输入数据:" << endl;
			int v;
			cin >> v;
			d_stk.pushTop(v);
		}
		else if (name == "WRT")
		{
			auto t = d_stk.getTop();
			cout << "输出:" << endl;
			cout << get<2>(t) << endl;
		}
		else if (name == "RET")
		{
			if (d_stk.cur_layer != 0)
			{
				d_stk.LeaveFunc();
			}
		}

		if (outputmessage)
		{
			d_stk.show();
			//char ch;
			//cin >> ch;
		}
	}
}