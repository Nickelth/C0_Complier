#pragma once
#include<iostream>
#include<tuple>
#include<vector>
#include"DataStack.h"
using namespace std;
class CommandStack {
public:
	int ne = 0;//next command
	int outputmessage = 0;
	vector<tuple<int, string, int, int> >c_stk;//indice,name,address1,address2
	DataStack d_stk;
	void readCommand();

	void run();
	void findNextCommand(int&);


};


