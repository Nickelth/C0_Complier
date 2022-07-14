#include"myParser.h"
void myParser::run()
{
	keywordsymTable.words.push_back(myToken(keywordsymTable.elsesym, "else", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.ifsym, "if", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.intsym, "int", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.mainsym, "main", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.printfsym, "printf", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.returnsym, "return", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.scanfsym, "scanf", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.voidsym, "void", 0));
	keywordsymTable.words.push_back(myToken(keywordsymTable.whilesym, "while", 0));
	//将关键字加入关键字表
	program();

	//在语法分析、代码生成结束后，我们得到了每个函数的指令栈
	//但此时，我们仍有两项工作需要做
	//1.将所有函数的指令栈连起来，同时指定每个函数的地址，并据此转换其指令栈内所有的相对地址
	//2.根据上一步确定的函数地址，为所有函数调用语句确定地址
	command fir("JMP", 0, 0);//最开始跳转到main的语句，之后确定main的地址后会修改其地址
	commandRes.push_back(fir);
	int cnt = 1;
	for (int i = 0; i < funcs.size(); i++)
	{
		funcs[i].loc = cnt;
		for (int j = 0; j < funcs[i].commandset.stk.size(); j++)
		{
			command& c = funcs[i].commandset.stk[j];
			if (c.op == "JMP" || c.op == "JPC")
			{
				c.adr2 += cnt;
			}
			commandRes.push_back(c);
		}
		cnt += funcs[i].commandset.stk.size();
	}
	commandRes[0].adr2 = cnt;
	for (int i = 0; i < mainSet.stk.size(); i++)
	{
		command& c = mainSet.stk[i];
		if (c.op == "JMP" || c.op == "JPC")
		{
			c.adr2 += cnt;
		}
		commandRes.push_back(c);
		
	}
	cnt += mainSet.stk.size();
	for (int i = 0; i < commandRes.size(); i++)
	{
		if (commandRes[i].op == "CAL")
		{
			int f = 0;
			for (int j = 0; j < funcs.size(); j++)
			{
				if (funcs[j].name == commandRes[i].calfunc)
				{
					commandRes[i].adr2 = funcs[j].loc;
					f = 1;
					break;
				}
			}
			if (!f) {
				cout << "以下函数被调用却没有定义！:" << commandRes[i].calfunc << endl;
				exit(0);
			}
		}
	}

	if (p)printsymTable();
	if(p)cout << "请注意，下面的指令栈中CAL语句还没完成寻址，各相对地址也还没完成转换" << endl;
	if (p)printfcommandstk();
	int j = 0;
	cout << "最终的完整指令栈:" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	for (auto i :commandRes)
	{
		cout << j++<<' '<<i.op << "\t" << i.adr1 << "\t" << i.adr2 << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
	}
	ofstream fout;
	fout.open("stackCommand.txt", ios::out);
	for (auto i : commandRes)
	{
		fout << i.op << ' ' << i.adr1 << ' ' << i.adr2 << endl;
	}
	cout << "指令栈已存至项目文件夹下stackCommand.txt文件中" << endl;
	fout.close();
	lexer.fin.close();
}
commandSet& myParser::findCommandSet(string name)
{
	if (name == "main")
	{
		return mainSet;
	}
	else
	{
		func& f = getFunc(name);
		return f.commandset;
	}
}
mysymTable& myParser::findsymTable(string name)
{
	if (name == "global")
	{
		return globalTable;
	}
	else if (name == "main")
	{
		return mainTable;
	}
	else
	{
		func& f = getFunc(name);
		return f.funcTable;
	}
}
void myParser::printfsinglesymTable(string name, mysymTable& t)
{
	cout << name << "的符号表:" << endl;
	if (t.words.empty())
	{
		cout << "----------------------------------------------------------------------------------------" << endl;
		cout << "符号表为空！" << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
	}
	else
	{
		cout << "----------------------------------------------------------------------------------------" << endl;
		cout << "|\t" << "符号\t" << "|\t" << "类型\t" << "|\t" << "域\t" << "|\t" << "相对地址\t" << "|" << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
		for (auto i : t.words)
		{
			cout << "|\t" << i.lexeme << "\t|\t" << keywordsymTable.findsymType(i.type) << "\t|\t" << name << "\t|\t" << i.offset << "\t|" << endl;
			cout << "----------------------------------------------------------------------------------------" << endl;
		}
	}
}
void myParser::printsymTable()
{
	printfsinglesymTable("全局变量", globalTable);
	for (auto i : funcs)
	{
		printfsinglesymTable(i.name, i.funcTable);
	}
	printfsinglesymTable("main函数", mainTable);
}
void myParser::printsinglecommandstk(string name, commandSet& c)
{
	cout << name << "的指令栈" << endl;
	cout << "----------------------------------------------------------------------------------------" << endl;
	for (auto i : c.stk)
	{
		cout << i.op << "\t" << i.adr1 << "\t" << i.adr2 << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
	}
}
void myParser::printfcommandstk()
{
	
	printsinglecommandstk("main", mainSet);
	for (auto i : funcs)
	{
		printsinglecommandstk(i.name, i.commandset);
	}
}
int myParser::findFunc(string s)
{
	for (auto i : funcs)
	{
		if (i.name == s)
		{
			return 1;
		}
	}
	return 0;
}
func& myParser::getFunc(string s)
{

	for (int i = 0; i < funcs.size(); i++)
	{
		if (funcs[i].name == s)
		{
			return funcs[i];
		}
	}
}
int myParser::program()
{
	if (p)
		cout << "进入状态program,开始语法分析" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.intsym)
	{
		if (p)cout << "读到int" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到标识符" << cur.lexeme << endl;
		}
		if (S2(cur) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.voidsym)
	{
		if (p)
			cout << "读到void" << endl;
		if (S3() == 1)return 1;
		else return 0;
	}
	else
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
}
int myParser::S2(myToken id)
{
	if (p)
		cout << "进入状态S2" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.lparen)
	{
		if (p)
			cout << "读到左括号" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.rparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到右括号" << endl;
			if (findFunc(id.lexeme) == 0)
			{
				func f(id.lexeme, 1);
				funcs.push_back(f);
				if (p)
					cout << "函数" << f.name << "加入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 4, cur.lexeme);
				return 0;
			}
			if (subprogram(id.lexeme) == 1)
			{
				if (S4() == 1)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else if (cur.type == keywordsymTable.comma)
	{
		if (p)
			cout << "读到逗号" << endl;
		if (globalTable.findWord(id.lexeme) == -1)
		{
			id.offset = globalTable.words.size() + 3;//全局变量的地址从3开始
			globalTable.words.push_back(id);
			if (p)
				cout << "变量" << id.lexeme << "加入符号表" << endl;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
			return 0;
		}
		if (S5("global") == 1)
			return 1;
		else
			return 0;
	}
	else if (cur.type == keywordsymTable.semicolon)
	{
		if (p)
			cout << "读到分号，全局变量声明结束" << endl;
		if (globalTable.findWord(id.lexeme) == -1)
		{
			if (p)
				cout << "变量" << id.lexeme << "加入符号表" << endl;
			id.offset = globalTable.words.size() + 3;//全局变量的地址从3开始
			globalTable.words.push_back(id);
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
			return 0;
		}
		cur = lexer.getsym();
		S4();
		return 1;
	}
	else
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
}
int myParser::S5(string funcname)
{
	if (p)
		cout << "进入S5" << endl;
	if (funcname == "global") {
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到标识符" << cur.lexeme << endl;
			if (globalTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = globalTable.words.size() + 3;//全局变量的地址从3开始
				globalTable.words.push_back(cur);
				if (p)
					cout << "变量" << cur.lexeme << "加入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
		}
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.comma)
		{
			if (p)
				cout << "读到逗号" << endl;
			if (S5(funcname) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (cur.type == keywordsymTable.semicolon)
		{
			if (p)
				cout << "读到分号,全局变量声明结束" << endl;
			cur = lexer.getsym();
			if (S4() == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		}
	}
	else if (funcname != "main")
	{
		cur = lexer.getsym();
		func& f = getFunc(funcname);
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到标识符" << cur.lexeme << endl;
			if (f.funcTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = f.funcTable.words.size() + 2;//函数变量的地址从2开始
				f.funcTable.words.push_back(cur);
				if (p)
					cout << "变量" << cur.lexeme << "加入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
		}
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.comma)
		{
			if (p)
				cout << "读到逗号" << endl;
			if (S5(funcname) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (cur.type == keywordsymTable.semicolon)
		{
			if (p)
				cout << "读到分号,函数" << funcname << "变量声明结束" << endl;
			cur = lexer.getsym();
			//command c("INT", 0, f.funcTable.words.size() + 2);
			//f.commandset.stk.push_back(c);
			return 1;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}
	else if (funcname == "main")
	{
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到标识符" << cur.lexeme << endl;
			if (mainTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = mainTable.words.size() + globalTable.words.size() + 3;//main变量和global变量在存储上是在一起的
				mainTable.words.push_back(cur);
				if (p)
					cout << "变量" << cur.lexeme << "加入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
		}
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.comma)
		{
			if (p)
				cout << "读到逗号" << endl;
			if (S5(funcname) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (cur.type == keywordsymTable.semicolon)
		{
			if (p)
				cout << "读到分号,main函数变量声明结束" << endl;
			cur = lexer.getsym();
			//command c("INT", 0, globalTable.words.size() + mainTable.words.size() + 3);
			//mainSet.stk.push_back(c);
			return 1;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}
}
int myParser::S4()
{
	if (p)
		cout << "进入S4" << endl;
	if (cur.type == keywordsymTable.intsym)
	{
		if (p)
			cout << "读到int" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到标识符" << cur.lexeme << endl;
		}
		myToken tmp = cur;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.lparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到左括号" << endl;
		}
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.rparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到右括号" << endl;
		}
		func f(tmp.lexeme, 1);
		if (findFunc(tmp.lexeme) == 0)
		{
			funcs.push_back(f);
			if (p)
				cout << "函数" << f.name << "加入符号表" << endl;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 4, tmp.lexeme);
		}
		if (subprogram(tmp.lexeme) == 1)
		{
			if (S4() == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if (cur.type == keywordsymTable.voidsym)
	{
		if (p)
			cout << "读到void" << endl;
		if (S3() == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
}
int myParser::S3()
{
	if (p)
		cout << "进入S3" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.ident)
	{
		if (p)
			cout << "读到标识符" << cur.lexeme << endl;
		myToken tmp = cur;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.lparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到左括号" << endl;
		}
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.rparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到右括号" << endl;
		}
		if (findFunc(tmp.lexeme) == 0)
		{
			func f(tmp.lexeme, 0);
			funcs.push_back(f);
			if (p)
				cout << "函数" << f.name << "加入符号表" << endl;
		}
		if (subprogram(tmp.lexeme) == 1)
		{
			if (S4() == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else if (cur.type == keywordsymTable.mainsym)
	{
		if (p)
			cout << "读到main" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.lparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到左括号" << endl;
		}
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.rparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "读到右括号" << endl;
		}
		if (subprogram("main") == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
}
int myParser::returnRight(func& f, commandSet& c)
{
	if (f.hasreturn == f.willreturn)
	{
		return 1;
	}
	else
	{
		if (f.hasreturn == -1)//没有return语句
		{
			if (f.willreturn == 0)
			{
				command com("RET", 0, 0);//此时要补一条ret指令
				c.stk.push_back(com);
				return 1;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 6, f.name);
				return 0;
			}
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 6, f.name);
			return 0;
		}
	}
}
int myParser::returnRight()
{
	if (mainreturn == 0)
	{
		return 1;
	}
	else if (mainreturn == 1)//理论上不会执行，因为returnsentence里已经判断过
	{
		err.repor(lexer.line, lexer.curpos, 6, "main");
		return 0;
	}
	else if (mainreturn == -1)
	{
		command com("RET", 0, 0);//此时要补一条ret指令
		mainSet.stk.push_back(com);
	}
}
int myParser::subprogram(string funcname)//subprogram会读到分程序后面再一个词法单元为止
{
	if (p)
		cout << "进入分程序" << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	func& f = getFunc(funcname);
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lbrace)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)
			cout << "读到左花括号" << endl;
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.intsym)
		{
			if (p)cout << "读到int" << endl;
			if (vardefine(funcname) == 1) {
				if (funcname == "main")
				{
					command com("INT", 0, globalTable.words.size() + mainTable.words.size() + 3);
					c.stk.push_back(com);
				}
				else
				{
					command com("INT", 0, s.words.size() + 2);
					c.stk.push_back(com);
				}
				cur = lexer.getsym();
				if (cur.type == keywordsymTable.ifsym || cur.type == keywordsymTable.whilesym || cur.type == keywordsymTable.returnsym
					|| cur.type == keywordsymTable.scanfsym || cur.type == keywordsymTable.printfsym || cur.type == keywordsymTable.lbrace
					|| cur.type == keywordsymTable.semicolon || cur.type == keywordsymTable.ident)
				{
					if (sentenceseq(funcname) == 1) {
						if (funcname != "main") {
							if (returnRight(f, c))
								return 1;
							else return 0;
						}
						else
						{
							if (returnRight())
							{
								return 1;
							}
							else
							{
								return 0;
							}
						}
					}
					else return 0;
				}
				else if (cur.type == keywordsymTable.rbrace)
				{
					if (p)cout << "读到右花括号，函数" << funcname << "结束" << endl;
					cur = lexer.getsym();
					if (funcname != "main") {
						if (returnRight(f, c))
							return 1;
						else return 0;
					}
					else
					{
						if (returnRight())
						{
							return 1;
						}
						else
						{
							return 0;
						}
					}
				}
				else
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
			}
			else return 0;
		}
		else if (cur.type == keywordsymTable.ifsym || cur.type == keywordsymTable.whilesym || cur.type == keywordsymTable.returnsym
			|| cur.type == keywordsymTable.scanfsym || cur.type == keywordsymTable.printfsym || cur.type == keywordsymTable.lbrace
			|| cur.type == keywordsymTable.semicolon || cur.type == keywordsymTable.ident)
		{
			if (funcname == "main")
			{
				command com("INT", 0, globalTable.words.size() + mainTable.words.size() + 3);
				c.stk.push_back(com);
			}
			else
			{
				command com("INT", 0, s.words.size() + 2);
				c.stk.push_back(com);
			}
			if (sentenceseq(funcname) == 1)
			{
				if (funcname != "main") {
					if (returnRight(f, c))
						return 1;
					else return 0;
				}
				else
				{
					if (returnRight())
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
			else return 0;
		}
		else if (cur.type == keywordsymTable.rbrace)//分程序里什么都没有
		{
			if (funcname == "main")
			{
				command com("INT", 0, globalTable.words.size() + mainTable.words.size() + 3);
				c.stk.push_back(com);
				if (funcname != "main") {
					if (returnRight(f, c))
						return 1;
					else return 0;
				}
				else
				{
					if (returnRight())
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
			else
			{
				cur = lexer.getsym();
				command com("INT", 0, s.words.size() + 2);
				c.stk.push_back(com);
				if (funcname != "main") {
					if (returnRight(f, c))
						return 1;
					else return 0;
				}
				else
				{
					if (returnRight())
					{
						return 1;
					}
					else
					{
						return 0;
					}
				}
			}
			if (p)cout << "读到右花括号" << endl;
			return 1;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}

}
int myParser::vardefine(string funcname)
{
	commandSet& c = findCommandSet(funcname);
	if (p)cout << "进入状态vardefine,为函数" << funcname << "编制符号表" << endl;
	if (funcname != "main") {
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)cout << "读到标识符" << cur.lexeme << endl;
			func& f = getFunc(funcname);
			if (f.funcTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = f.funcTable.words.size() + 2;//函数的变量偏移地址从2开始
				f.funcTable.words.push_back(cur);
				if (p)cout << "变量" << cur.lexeme << "已插入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
			cur = lexer.getsym();
			if (cur.type == keywordsymTable.comma)
			{
				if (p)cout << "读到逗号" << endl;
				if (S5(funcname) == 1)return 1;
				else return 0;
			}
			else if (cur.type == keywordsymTable.semicolon)
			{
				if (p)cout << "读到分号，函数" << funcname << "符号声明结束" << endl;
				//command c("INT", 0, f.funcTable.words.size() + 2);
				//f.commandset.stk.push_back(c);
				return 1;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
		}
	}
	else
	{
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)cout << "读到标识符" << cur.lexeme << endl;
			if (mainTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = mainTable.words.size() + globalTable.words.size() + 3;
				mainTable.words.push_back(cur);
				if (p)cout << "变量" << cur.lexeme << "已插入符号表" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
			cur = lexer.getsym();
			if (cur.type == keywordsymTable.comma)
			{
				if (p)cout << "读到逗号" << endl;
				if (S5(funcname) == 1)return 1;
				else return 0;
			}
			else if (cur.type == keywordsymTable.semicolon)
			{
				if (p)cout << "读到分号，函数" << funcname << "符号声明结束" << endl;
				//command com("INT", 0, globalTable.words.size() + mainTable.words.size() + 3);
				//c.stk.push_back(com);
				return 1;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
		}
	}
}
int myParser::sentenceseq(string funcname)//sentenceseq会读入到右花括号之后一个词法单元为
{
	if (p)cout << "进入状态sentenceseq,该语句序列属于函数" << funcname << endl;

	if (sentence(funcname) == 1)
	{
		//cur = lexer.getsym();
		if (cur.type == keywordsymTable.ifsym || cur.type == keywordsymTable.whilesym || cur.type == keywordsymTable.returnsym
			|| cur.type == keywordsymTable.scanfsym || cur.type == keywordsymTable.printfsym || cur.type == keywordsymTable.lbrace
			|| cur.type == keywordsymTable.semicolon || cur.type == keywordsymTable.ident)
		{
			if (sentenceseq(funcname) == 1)
			{
				return 1;
			}
			else return 0;
		}
		else if (cur.type == keywordsymTable.rbrace)//注意语句序列处理右花括号而不处理左花括号
		{
			if (p)cout << "读到右花括号，语句序列结束" << endl;
			cur = lexer.getsym();
			return 1;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}
	else
	{
		return 0;
	}

}
int myParser::sentence(string funcname)//我们这里定义sentence处理一个语句，然后将该语句后面的一个字符读进来
										//注意sentence需要你事先读入一个字符
{
	if (p)cout << "进入状态sentence,该语句属于函数" << funcname << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	if (cur.type == keywordsymTable.ifsym)
	{
		if (p)cout << "读到if" << endl;
		if (ifsentence(funcname) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if (cur.type == keywordsymTable.whilesym)
	{
		if (p)cout << "读到while" << endl;
		if (whilesentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.returnsym)
	{
		if (p)cout << "读到return" << endl;
		if (returnsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.scanfsym)
	{
		if (p)cout << "读到scanf" << endl;
		if (scanfsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.printfsym)
	{
		if (p)cout << "读到printf" << endl;
		if (printfsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.ident)
	{
		if (p)cout << "读到标识符" << cur.lexeme << endl;
		myToken id = cur;
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.lparen)
		{
			if (p)cout << "读到左括号" << endl;
			cur = lexer.getsym();
			if (cur.type != keywordsymTable.rparen)
			{

				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "读到右括号" << endl;
				command com("CAL", 0, 0);
				com.calfunc = id.lexeme;
				command com2("LOD", 0, 2);
				c.stk.push_back(com);
				c.stk.push_back(com2);
				cur = lexer.getsym();
				if (cur.type != keywordsymTable.semicolon)
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
				else
				{
					if (p)cout << "读到分号" << endl;
					cur = lexer.getsym();
					return 1;
				}
			}
		}
		else if (cur.type == keywordsymTable.assign)
		{
			if (p)cout << "读到赋值语句" << endl;
			if (expression(funcname) == 1)
			{
				stoIdent(c, s, id.lexeme, funcname);
				cur = lexer.getsym();
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}
	else if (cur.type == keywordsymTable.lbrace)
	{
		if (p)cout << "读到左花括号" << endl;
		cur = lexer.getsym();
		if (sentenceseq(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.semicolon)
	{
		if (p)cout << "读到分号" << endl;
		cur = lexer.getsym();
		return 1;//注意单个的分号也是合法语句
	}
}
int myParser::ifsentence(string funcname)
{
	if (p)cout << "进入状态ifsentence,该if语句在函数" << funcname << "中" << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lparen)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)cout << "读到左括号" << endl;
		if (expression(funcname) == 1)
		{
			//cur = lexer.getsym();
			if (cur.type != keywordsymTable.rparen)//while和if的表达式会读到其右括号为止
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "读到右括号" << endl;
				command com("JPC", 0, 0);
				c.stk.push_back(com);
				int ref = c.stk.size() - 1;
				cur = lexer.getsym();
				if (sentence(funcname) == 1)
				{
					if (cur.type == keywordsymTable.elsesym)
					{
						if (p)cout << "读到else" << endl;
						command com2("JMP", 0, 0);
						c.stk.push_back(com2);
						int ref2 = c.stk.size() - 1;
						c.stk[ref].adr2 = c.stk.size();
						cur = lexer.getsym();
						if (sentence(funcname) == 1)
						{
							c.stk[ref2].adr2 = c.stk.size();
							return 1;
						}
						else
						{
							return 0;
						}
					}
					else
					{
						c.stk[ref].adr2 = c.stk.size();
						return 1;
					}
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
	}
}
int myParser::lodIdent(commandSet& c, mysymTable& s, string name, string funcname)
{
	/*
	先在当前函数的符号表里查找符号是否存在，然后去全局变量符号表查找符号是否存在
	*/
	
	int t = s.findWord(name);
	if (t == -1)
	{
		int t2 = globalTable.findWord(name);
		
		if (t2==-1)
		{
			err.repor(lexer.line, lexer.curpos, 5, name);
			return 0;
		}
		else
		{
			myToken& tmp2 = globalTable.getWord(name);
			command com("LOD", 0, tmp2.offset);
			c.stk.push_back(com);
			return 1;
		}
	}
	else
	{
		myToken& tmp = s.getWord(name);
		if (funcname == "main")
		{
			command com("LOD", 0, tmp.offset);
			c.stk.push_back(com);
			return 1;
		}
		else
		{
			command com("LOD", 1, tmp.offset);
			c.stk.push_back(com);
			return 1;
		}
	}
}
int myParser::expression(string funcname)
{
	if (p)cout << "进入表达式状态，该表达式在函数" << funcname << "中" << endl;
	/*在我们的语法中，表达式一定以)或;结尾*/
	stack<char>stk;//由于当表达式作为if或者while的判别式时，我们需要将if或while的右括号与表达式中的括号区别开
					//所以我们用一个栈来做括号匹配，当遇到第一个无法匹配的右括号时，我们就认为表达式结束
	vector<myToken> expr;//表达式需要整个读入以后进行预处理，因为我们需要判断其中的-是减号还是负号
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	do {
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.lparen)
		{
			stk.push('(');
			expr.push_back(cur);
		}
		else if (cur.type == keywordsymTable.rparen)
		{
			if (stk.empty())
			{
				break;
			}
			else
			{
				stk.pop();
			}
			expr.push_back(cur);
		}
		else if (cur.type == keywordsymTable.minus)
		{
			if (expr.empty() || expr.back().type == keywordsymTable.lparen || expr.back().type == keywordsymTable.plus ||
				expr.back().type == keywordsymTable.minus || expr.back().type == keywordsymTable.multiply ||
				expr.back().type == keywordsymTable.divide)
			{
				myToken tmp(-1, "#", 0);//如果是负号，转换为#
				expr.push_back(tmp);
			}
			else
			{
				expr.push_back(cur);
			}
		}
		else if (cur.type == keywordsymTable.semicolon)
		{
			break;
		}
		else if (cur.type == keywordsymTable.plus || cur.type == keywordsymTable.multiply || cur.type == keywordsymTable.divide)
		{
			expr.push_back(cur);
		}
		else if (cur.type == keywordsymTable.number)
		{
			if (!expr.empty() && expr.back().lexeme == "#")
			{
				expr.pop_back();
				cur.val = -cur.val;
				expr.push_back(cur);
			}
			else
			{
				expr.push_back(cur);
			}
		}
		else if (cur.type == keywordsymTable.ident)
		{
			if (!expr.empty() && expr.back().lexeme == "#")//将-ident转化为(0-ident)
			{
				expr.pop_back();
				myToken tmp1(keywordsymTable.lparen, "(", 0);
				myToken tmp2(keywordsymTable.rparen, ")", 0);
				myToken tmp3(keywordsymTable.number, "", 0);
				myToken tmp4(keywordsymTable.minus, "-", 0);
				expr.push_back(tmp1);
				expr.push_back(tmp3);
				expr.push_back(tmp4);
				expr.push_back(cur);
				expr.push_back(tmp2);
			}
			else
			{
				expr.push_back(cur);
			}
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}

	} while (1);
	if (expr.empty())
	{
		return -1;
	}
	if (!stk.empty())
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	vector<myToken>expr2;
	for (int i = 0; i < expr.size(); i++)
	{
		if (i + 2 < expr.size() && expr[i].type == keywordsymTable.ident && expr[i + 1].type == keywordsymTable.lparen && expr[i + 2].type == keywordsymTable.rparen)
		{
			expr[i].type = -2;//如果读到函数，将标识符置为特殊的类型
			expr2.push_back(expr[i]);
			i = i + 2;
		}
		else
		{
			expr2.push_back(expr[i]);
		}
	}
	if (p)
	{
		cout << "读到表达式:";
		for (auto i : expr2)
		{
			if (i.type == keywordsymTable.number)
			{
				cout << i.val << ' ';
			}
			else if (i.type != -2)
			{
				cout << i.lexeme << ' ';
			}
			else
			{
				cout << i.lexeme << "() ";
			}
		}
		cout << endl;
	}
	if (expr2.size() == 1)//特殊情况，表达式中只有一个量，函数调用或者数字或者标识符
	{
		if (expr2[0].type == -2)
		{
			command com("CAL", 0, 0);
			com.calfunc = expr2[0].lexeme;
			c.stk.push_back(com);
			command com2("LOD", 0, 2);
			c.stk.push_back(com2);
			return 1;
		}
		else if (expr2[0].type == keywordsymTable.ident)
		{
			lodIdent(c, s, expr2[0].lexeme, funcname);
			return 1;
		}
		else if (expr2[0].type == keywordsymTable.number)
		{
			command com("LIT", 0, expr2[0].val);
			c.stk.push_back(com);
			return 1;
		}
		else
		{
			err.repor(lexer.line, lexer.curpos, 2, expr2[0].lexeme);
			return 0;
		}
	}
	auto priority = [&](myToken cur)//该匿名函数用于取得运算符的优先级
	{
		string s = cur.lexeme;
		if (s == "+" || s == "-")
		{
			return 1;
		}
		else if (s == "*" || s == "/")
		{
			return 2;
		}
		else if (s == "(" || s == ")")
		{
			return 0;
		}
	};
	//这里，我们将中缀表达式转换为后缀表达式进行处理
	vector<myToken>expr3;
	vector<myToken>op;
	vector<myToken>num;
	for (auto i : expr2)
	{
		if (i.type == keywordsymTable.number || i.type == -2 || i.type == keywordsymTable.ident)
		{
			expr3.push_back(i);
		}
		else if (i.type == keywordsymTable.lparen)
		{
			op.push_back(i);
		}
		else if (i.type == keywordsymTable.rparen)
		{
			myToken tmp;
			while (1)
			{
				tmp = op.back();
				if (tmp.type == keywordsymTable.lparen)
				{
					op.pop_back();
					break;
				}
				else
				{
					expr3.push_back(tmp);
					op.pop_back();
				}
			}
		}
		else
		{
			if (op.empty())
			{
				op.push_back(i);
			}
			else
			{
				int curop = priority(i);
				int preop = priority(op.back());
				if (curop > preop)
				{
					op.push_back(i);
				}
				else
				{
					while (!op.empty() && curop <= preop)
					{
						expr3.push_back(op.back());
						op.pop_back();
						if (op.empty())break;
						else preop = priority(op.back());
					}
					op.push_back(i);
				}
			}
		}
	}
	while (!op.empty())
	{
		expr3.push_back(op.back());
		op.pop_back();
	}
	if (p) {
		cout << "转化所得后缀表达式为:" << endl;
		for (auto i : expr3)
		{
			if (i.type == keywordsymTable.number)
			{
				cout << i.val << ' ';
			}
			else if (i.type != -2)
			{
				cout << i.lexeme << ' ';
			}
			else
			{
				cout << i.lexeme << "() ";
			}
		}
		cout << endl;
	}

	op.clear();
	num.clear();
	for (auto i : expr3)
	{
		if (i.type == keywordsymTable.number)
		{
			command com("LIT", 0, i.val);
			c.stk.push_back(com);
		}
		else if (i.type == keywordsymTable.ident)
		{
			lodIdent(c, s, i.lexeme, funcname);
		}
		else if (i.type == -2)
		{
			command com("CAL", 0, 0);
			com.calfunc = i.lexeme;
			c.stk.push_back(com);
			command com2("LOD", 0, 2);
			c.stk.push_back(com2);
		}
		else
		{
			if (i.type == keywordsymTable.plus)
			{
				command com("ADD", 0, 0);
				c.stk.push_back(com);
			}
			else if (i.type == keywordsymTable.minus)
			{
				command com("SUB", 0, 0);
				c.stk.push_back(com);
			}
			else if (i.type == keywordsymTable.multiply)
			{
				command com("MUL", 0, 0);
				c.stk.push_back(com);
			}
			else if (i.type == keywordsymTable.divide)
			{
				command com("DIV", 0, 0);
				c.stk.push_back(com);
			}
		}
	}
	return 1;


}
int myParser::whilesentence(string funcname)
{
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	if (p)cout << "进入状态whilesentence,该while循环在函数" << funcname << "中" << endl;
	int st = c.stk.size();//记录while语句的开始地址
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lparen)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)cout << "读到左括号" << endl;
		if (expression(funcname) == 1)
		{
			if (cur.type != keywordsymTable.rparen)
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "读到右括号" << endl;
				command com("JPC", 0, 0);
				c.stk.push_back(com);
				int ref = c.stk.size() - 1;
				cur = lexer.getsym();
				if (sentence(funcname) == 1)
				{
					command com2("JMP", 0, st);
					c.stk.push_back(com2);
					c.stk[ref].adr2 = c.stk.size();
					return 1;
				}
				else
				{
					return 0;
				}
			}

		}
	}
}
int myParser::returnsentence(string funcname)//注意，returnsentence需要负责维护函数的hasreturn和willreturn
{
	if (p)cout << "进入状态returnsentence,该return语句在函数" << funcname << "中" << endl;
	//cur = lexer.getsym();
	func& f = getFunc(funcname);

	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	if (expression(funcname) == -1)//如果后面是分号
	{
		command com("RET", 0, 0);
		c.stk.push_back(com);
		if (funcname != "main") {
			f.hasreturn = 0;
			if (f.willreturn != f.hasreturn)
			{
				err.repor(lexer.line, lexer.curpos, 6, funcname);
				return 0;
			}

			cur = lexer.getsym();//注意sentence是要再读入之后的一个字符，下同
			return 1;
		}
		else
		{
			mainreturn = 0;
			cur = lexer.getsym();
			return 1;
		}
	}
	else
	{
		if (funcname != "main") {
			f.hasreturn = 1;
			command com("STO", 0, 2);//第0层第2单元是返回值空间
			command com2("RET", 0, 0);
			c.stk.push_back(com);
			c.stk.push_back(com2);
			cur = lexer.getsym();
			return 1;
		}
		else
		{
			mainreturn = 1;
			err.repor(lexer.line, lexer.curpos, 6, "main");
			return 0;
		}
	}
}
int myParser::stoIdent(commandSet& c, mysymTable& s, string name, string funcname)
{
	/*
	先在当前函数的符号表里查找符号是否存在，然后去全局变量符号表查找符号是否存在
	*/
	int t = s.findWord(name);
	
	if (t==-1)
	{
		int t2 = globalTable.findWord(name);
		
		if (t2==-1)
		{
			err.repor(lexer.line, lexer.curpos, 5, name);
			return 0;
		}
		else
		{
			myToken& tmp2 = globalTable.getWord(name);
			command com("STO", 0, tmp2.offset);
			c.stk.push_back(com);
			return 1;
		}
	}
	else
	{
		myToken& tmp = s.getWord(name);
		if (funcname == "main")
		{
			command com("STO", 0, tmp.offset);
			c.stk.push_back(com);
			return 1;
		}
		else
		{
			command com("STO", 1, tmp.offset);
			c.stk.push_back(com);
			return 1;
		}
	}
}
int myParser::scanfsentence(string funcname)
{
	if (p)cout << "进入状态scanf,该读入语句属于函数" << funcname << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lparen)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)cout << "读到左括号" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)cout << "读到标识符" << cur.lexeme << endl;
			command com("RED", 0, 0);
			c.stk.push_back(com);
			stoIdent(c, s, cur.lexeme, funcname);
			cur = lexer.getsym();
			if (cur.type != keywordsymTable.rparen)
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "读到右括号" << endl;
				cur = lexer.getsym();
				if (cur.type != keywordsymTable.semicolon)
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
				else
				{
					if (p)cout << "读到分号" << endl;
					cur = lexer.getsym();
					return 1;
				}
			}
		}
	}
}
int myParser::printfsentence(string funcname)
{
	if (p)cout << "进入状态printf,该读入语句属于函数" << funcname << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lparen)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)cout << "读到左括号" << endl;
		if (expression(funcname) == 1)
		{
			command com("WRT", 0, 0);
			c.stk.push_back(com);
			if (cur.type != keywordsymTable.rparen)
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "读到右括号" << endl;
				cur = lexer.getsym();
				if (cur.type != keywordsymTable.semicolon)
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
				else
				{
					if (p)cout << "读到分号" << endl;
					cur = lexer.getsym();
					return 1;
				}
			}

		}
		else
		{
			return 0;
		}
	}
}
