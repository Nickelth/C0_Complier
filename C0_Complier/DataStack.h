#pragma once
#include<iostream>
#include<tuple>
#include<vector>
using namespace std;
class DataStack {
public:
	vector <tuple<int, int, int> >stk;//layer,offset,value
	vector<int>base;//base[i] means the base address of layer i
	int cur_layer = 0;
	int backpoint = -1;
	int top = 0;
	void show();
	tuple<int, int, int>getTop();
	int getCuurentLayer();
	int getCurrentBase();
	int getCurrentBaseValue();//get the value saved in current base address
	void pushTop(int);//push the value into the top of stack
	void pushTop(int, int);//pushTop with decided offset
	void popTop();
	tuple<int, int, int> getValue(int, int);//first parameter means the layer,and the second means the offset,this function return value with selected layer and offset
	/*
	2021.12.11 update:this function was changed:if layer is 0,it will return the selected value in layer 0;if layer is 1,it will return the selected value in current layer.
	*/
	void setValue(int, int, int);//similar to the setValue(),but the third parameter means the value we want to set
	/*
	2021.12.11 update:this function was changed:if layer is 0,it will change the selected value in layer 0;if layer is 1,it will change the selected value in current layer.
	*/
	void CallFunc(int);//when calling a function,we need to update base address and top point;the parameter is indice of current command
	void LeaveFunc();//when we leave func, update cur_layer,base address array,as well.
};
