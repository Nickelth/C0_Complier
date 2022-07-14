#include"DataStack.h"
void DataStack::show()
{
	if (stk.empty())
	{
		cout << "数据栈为空!" << endl;
		return;
	}
	cout << "展示数据栈如下:" << endl;
	cout << "layer\toffset\tvalues" << endl;
	cout << "-------------------" << endl;
	for (int i = stk.size() - 1; i >= 0; i--)
	{
		cout << get<0>(stk[i]) << '\t' << get<1>(stk[i]) << '\t' << get<2>(stk[i]) << endl;
		cout << "-------------------" << endl;
	}
}
tuple<int, int, int> DataStack::getTop()
{
	return stk[top];
}
int DataStack::getCuurentLayer()
{
	return cur_layer;
}
int DataStack::getCurrentBase()
{
	int t = base[cur_layer];
	return t;
}
int DataStack::getCurrentBaseValue()
{
	int t = getCurrentBase();
	tuple<int, int, int>tup = stk[t];
	return get<2>(tup);
}
void DataStack::pushTop(int a)
{
	if (stk.empty())
	{
		stk.push_back(make_tuple(0, 0, 0));
		top = 0;
		return;
	}
	tuple<int, int, int>t = getTop();
	int layer = cur_layer;
	int offset = get<1>(t) + 1;
	int value = a;
	stk.push_back(make_tuple(layer, offset, value));
	top++;
}
void DataStack::pushTop(int a, int offset)
{
	if (stk.empty())
	{
		stk.push_back(make_tuple(0, offset, 0));
		top = 0;
		return;
	}
	tuple<int, int, int>t = getTop();
	int layer = cur_layer;
	int value = a;
	stk.push_back(make_tuple(layer, offset, value));
	top++;
}
void DataStack::popTop()
{
	stk.pop_back();
	top--;
}
tuple<int, int, int> DataStack::getValue(int layer, int offset)
{
	if (layer == 0)
	{
		int b = base[0];
		while (offset--)
		{
			b++;
		}
		return stk[b];
	}
	else
	{
		int b = base[cur_layer];
		while (offset--)
		{
			b++;
		}
		return stk[b];
	}
	/*int b = base[layer];
	while (offset--)
	{
		b++;
	}
	return stk[b];*/
}
void DataStack::setValue(int layer, int offset, int v)
{
	if (layer == 0) {
		int b = base[0];
		while (offset--)
		{
			b++;
		}
		get<2>(stk[b]) = v;
	}
	else
	{
		int b = base[cur_layer];
		while (offset--)
		{
			b++;
		}
		get<2>(stk[b]) = v;
	}
}
void DataStack::CallFunc(int cur)
{
	int new_layer = base.size();
	cur_layer = new_layer;
	int new_base = top + 1;
	base.push_back(new_base);
	backpoint = cur;
	//we don't update top point here,we will update it when we use INT
}
void DataStack::LeaveFunc()
{
	top = getCurrentBase() - 1;
	base.pop_back();
	cur_layer = base.size() - 1;
	backpoint = -1;
	vector<tuple<int, int, int> >::iterator iter = stk.begin();
	advance(iter, top + 1);//move itar top+1 steps
	stk.erase(iter, stk.end());
}