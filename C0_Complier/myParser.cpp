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
	//���ؼ��ּ���ؼ��ֱ�
	program();

	//���﷨�������������ɽ��������ǵõ���ÿ��������ָ��ջ
	//����ʱ�����������������Ҫ��
	//1.�����к�����ָ��ջ��������ͬʱָ��ÿ�������ĵ�ַ�����ݴ�ת����ָ��ջ�����е���Ե�ַ
	//2.������һ��ȷ���ĺ�����ַ��Ϊ���к����������ȷ����ַ
	command fir("JMP", 0, 0);//�ʼ��ת��main����䣬֮��ȷ��main�ĵ�ַ����޸����ַ
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
				cout << "���º���������ȴû�ж��壡:" << commandRes[i].calfunc << endl;
				exit(0);
			}
		}
	}

	if (p)printsymTable();
	if(p)cout << "��ע�⣬�����ָ��ջ��CAL��仹û���Ѱַ������Ե�ַҲ��û���ת��" << endl;
	if (p)printfcommandstk();
	int j = 0;
	cout << "���յ�����ָ��ջ:" << endl;
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
	cout << "ָ��ջ�Ѵ�����Ŀ�ļ�����stackCommand.txt�ļ���" << endl;
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
	cout << name << "�ķ��ű�:" << endl;
	if (t.words.empty())
	{
		cout << "----------------------------------------------------------------------------------------" << endl;
		cout << "���ű�Ϊ�գ�" << endl;
		cout << "----------------------------------------------------------------------------------------" << endl;
	}
	else
	{
		cout << "----------------------------------------------------------------------------------------" << endl;
		cout << "|\t" << "����\t" << "|\t" << "����\t" << "|\t" << "��\t" << "|\t" << "��Ե�ַ\t" << "|" << endl;
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
	printfsinglesymTable("ȫ�ֱ���", globalTable);
	for (auto i : funcs)
	{
		printfsinglesymTable(i.name, i.funcTable);
	}
	printfsinglesymTable("main����", mainTable);
}
void myParser::printsinglecommandstk(string name, commandSet& c)
{
	cout << name << "��ָ��ջ" << endl;
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
		cout << "����״̬program,��ʼ�﷨����" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.intsym)
	{
		if (p)cout << "����int" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "������ʶ��" << cur.lexeme << endl;
		}
		if (S2(cur) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.voidsym)
	{
		if (p)
			cout << "����void" << endl;
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
		cout << "����״̬S2" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.lparen)
	{
		if (p)
			cout << "����������" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.rparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "����������" << endl;
			if (findFunc(id.lexeme) == 0)
			{
				func f(id.lexeme, 1);
				funcs.push_back(f);
				if (p)
					cout << "����" << f.name << "������ű�" << endl;
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
			cout << "��������" << endl;
		if (globalTable.findWord(id.lexeme) == -1)
		{
			id.offset = globalTable.words.size() + 3;//ȫ�ֱ����ĵ�ַ��3��ʼ
			globalTable.words.push_back(id);
			if (p)
				cout << "����" << id.lexeme << "������ű�" << endl;
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
			cout << "�����ֺţ�ȫ�ֱ�����������" << endl;
		if (globalTable.findWord(id.lexeme) == -1)
		{
			if (p)
				cout << "����" << id.lexeme << "������ű�" << endl;
			id.offset = globalTable.words.size() + 3;//ȫ�ֱ����ĵ�ַ��3��ʼ
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
		cout << "����S5" << endl;
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
				cout << "������ʶ��" << cur.lexeme << endl;
			if (globalTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = globalTable.words.size() + 3;//ȫ�ֱ����ĵ�ַ��3��ʼ
				globalTable.words.push_back(cur);
				if (p)
					cout << "����" << cur.lexeme << "������ű�" << endl;
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
				cout << "��������" << endl;
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
				cout << "�����ֺ�,ȫ�ֱ�����������" << endl;
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
				cout << "������ʶ��" << cur.lexeme << endl;
			if (f.funcTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = f.funcTable.words.size() + 2;//���������ĵ�ַ��2��ʼ
				f.funcTable.words.push_back(cur);
				if (p)
					cout << "����" << cur.lexeme << "������ű�" << endl;
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
				cout << "��������" << endl;
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
				cout << "�����ֺ�,����" << funcname << "������������" << endl;
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
				cout << "������ʶ��" << cur.lexeme << endl;
			if (mainTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = mainTable.words.size() + globalTable.words.size() + 3;//main������global�����ڴ洢������һ���
				mainTable.words.push_back(cur);
				if (p)
					cout << "����" << cur.lexeme << "������ű�" << endl;
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
				cout << "��������" << endl;
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
				cout << "�����ֺ�,main����������������" << endl;
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
		cout << "����S4" << endl;
	if (cur.type == keywordsymTable.intsym)
	{
		if (p)
			cout << "����int" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "������ʶ��" << cur.lexeme << endl;
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
				cout << "����������" << endl;
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
				cout << "����������" << endl;
		}
		func f(tmp.lexeme, 1);
		if (findFunc(tmp.lexeme) == 0)
		{
			funcs.push_back(f);
			if (p)
				cout << "����" << f.name << "������ű�" << endl;
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
			cout << "����void" << endl;
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
		cout << "����S3" << endl;
	cur = lexer.getsym();
	if (cur.type == keywordsymTable.ident)
	{
		if (p)
			cout << "������ʶ��" << cur.lexeme << endl;
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
				cout << "����������" << endl;
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
				cout << "����������" << endl;
		}
		if (findFunc(tmp.lexeme) == 0)
		{
			func f(tmp.lexeme, 0);
			funcs.push_back(f);
			if (p)
				cout << "����" << f.name << "������ű�" << endl;
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
			cout << "����main" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.lparen)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)
				cout << "����������" << endl;
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
				cout << "����������" << endl;
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
		if (f.hasreturn == -1)//û��return���
		{
			if (f.willreturn == 0)
			{
				command com("RET", 0, 0);//��ʱҪ��һ��retָ��
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
	else if (mainreturn == 1)//�����ϲ���ִ�У���Ϊreturnsentence���Ѿ��жϹ�
	{
		err.repor(lexer.line, lexer.curpos, 6, "main");
		return 0;
	}
	else if (mainreturn == -1)
	{
		command com("RET", 0, 0);//��ʱҪ��һ��retָ��
		mainSet.stk.push_back(com);
	}
}
int myParser::subprogram(string funcname)//subprogram������ֳ��������һ���ʷ���ԪΪֹ
{
	if (p)
		cout << "����ֳ���" << endl;
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
			cout << "����������" << endl;
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.intsym)
		{
			if (p)cout << "����int" << endl;
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
					if (p)cout << "�����һ����ţ�����" << funcname << "����" << endl;
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
		else if (cur.type == keywordsymTable.rbrace)//�ֳ�����ʲô��û��
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
			if (p)cout << "�����һ�����" << endl;
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
	if (p)cout << "����״̬vardefine,Ϊ����" << funcname << "���Ʒ��ű�" << endl;
	if (funcname != "main") {
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)cout << "������ʶ��" << cur.lexeme << endl;
			func& f = getFunc(funcname);
			if (f.funcTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = f.funcTable.words.size() + 2;//�����ı���ƫ�Ƶ�ַ��2��ʼ
				f.funcTable.words.push_back(cur);
				if (p)cout << "����" << cur.lexeme << "�Ѳ�����ű�" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
			cur = lexer.getsym();
			if (cur.type == keywordsymTable.comma)
			{
				if (p)cout << "��������" << endl;
				if (S5(funcname) == 1)return 1;
				else return 0;
			}
			else if (cur.type == keywordsymTable.semicolon)
			{
				if (p)cout << "�����ֺţ�����" << funcname << "������������" << endl;
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
			if (p)cout << "������ʶ��" << cur.lexeme << endl;
			if (mainTable.findWord(cur.lexeme) == -1)
			{
				cur.offset = mainTable.words.size() + globalTable.words.size() + 3;
				mainTable.words.push_back(cur);
				if (p)cout << "����" << cur.lexeme << "�Ѳ�����ű�" << endl;
			}
			else
			{
				err.repor(lexer.line, lexer.curpos, 3, cur.lexeme);
				return 0;
			}
			cur = lexer.getsym();
			if (cur.type == keywordsymTable.comma)
			{
				if (p)cout << "��������" << endl;
				if (S5(funcname) == 1)return 1;
				else return 0;
			}
			else if (cur.type == keywordsymTable.semicolon)
			{
				if (p)cout << "�����ֺţ�����" << funcname << "������������" << endl;
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
int myParser::sentenceseq(string funcname)//sentenceseq����뵽�һ�����֮��һ���ʷ���ԪΪ
{
	if (p)cout << "����״̬sentenceseq,������������ں���" << funcname << endl;

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
		else if (cur.type == keywordsymTable.rbrace)//ע��������д����һ����Ŷ�������������
		{
			if (p)cout << "�����һ����ţ�������н���" << endl;
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
int myParser::sentence(string funcname)//�������ﶨ��sentence����һ����䣬Ȼ�󽫸��������һ���ַ�������
										//ע��sentence��Ҫ�����ȶ���һ���ַ�
{
	if (p)cout << "����״̬sentence,��������ں���" << funcname << endl;
	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	if (cur.type == keywordsymTable.ifsym)
	{
		if (p)cout << "����if" << endl;
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
		if (p)cout << "����while" << endl;
		if (whilesentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.returnsym)
	{
		if (p)cout << "����return" << endl;
		if (returnsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.scanfsym)
	{
		if (p)cout << "����scanf" << endl;
		if (scanfsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.printfsym)
	{
		if (p)cout << "����printf" << endl;
		if (printfsentence(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.ident)
	{
		if (p)cout << "������ʶ��" << cur.lexeme << endl;
		myToken id = cur;
		cur = lexer.getsym();
		if (cur.type == keywordsymTable.lparen)
		{
			if (p)cout << "����������" << endl;
			cur = lexer.getsym();
			if (cur.type != keywordsymTable.rparen)
			{

				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "����������" << endl;
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
					if (p)cout << "�����ֺ�" << endl;
					cur = lexer.getsym();
					return 1;
				}
			}
		}
		else if (cur.type == keywordsymTable.assign)
		{
			if (p)cout << "������ֵ���" << endl;
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
		if (p)cout << "����������" << endl;
		cur = lexer.getsym();
		if (sentenceseq(funcname) == 1)return 1;
		else return 0;
	}
	else if (cur.type == keywordsymTable.semicolon)
	{
		if (p)cout << "�����ֺ�" << endl;
		cur = lexer.getsym();
		return 1;//ע�ⵥ���ķֺ�Ҳ�ǺϷ����
	}
}
int myParser::ifsentence(string funcname)
{
	if (p)cout << "����״̬ifsentence,��if����ں���" << funcname << "��" << endl;
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
		if (p)cout << "����������" << endl;
		if (expression(funcname) == 1)
		{
			//cur = lexer.getsym();
			if (cur.type != keywordsymTable.rparen)//while��if�ı��ʽ�������������Ϊֹ
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "����������" << endl;
				command com("JPC", 0, 0);
				c.stk.push_back(com);
				int ref = c.stk.size() - 1;
				cur = lexer.getsym();
				if (sentence(funcname) == 1)
				{
					if (cur.type == keywordsymTable.elsesym)
					{
						if (p)cout << "����else" << endl;
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
	���ڵ�ǰ�����ķ��ű�����ҷ����Ƿ���ڣ�Ȼ��ȥȫ�ֱ������ű���ҷ����Ƿ����
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
	if (p)cout << "������ʽ״̬���ñ��ʽ�ں���" << funcname << "��" << endl;
	/*�����ǵ��﷨�У����ʽһ����)��;��β*/
	stack<char>stk;//���ڵ����ʽ��Ϊif����while���б�ʽʱ��������Ҫ��if��while������������ʽ�е���������
					//����������һ��ջ��������ƥ�䣬��������һ���޷�ƥ���������ʱ�����Ǿ���Ϊ���ʽ����
	vector<myToken> expr;//���ʽ��Ҫ���������Ժ����Ԥ������Ϊ������Ҫ�ж����е�-�Ǽ��Ż��Ǹ���
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
				myToken tmp(-1, "#", 0);//����Ǹ��ţ�ת��Ϊ#
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
			if (!expr.empty() && expr.back().lexeme == "#")//��-identת��Ϊ(0-ident)
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
			expr[i].type = -2;//�����������������ʶ����Ϊ���������
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
		cout << "�������ʽ:";
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
	if (expr2.size() == 1)//������������ʽ��ֻ��һ�������������û������ֻ��߱�ʶ��
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
	auto priority = [&](myToken cur)//��������������ȡ������������ȼ�
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
	//������ǽ���׺���ʽת��Ϊ��׺���ʽ���д���
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
		cout << "ת�����ú�׺���ʽΪ:" << endl;
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
	if (p)cout << "����״̬whilesentence,��whileѭ���ں���" << funcname << "��" << endl;
	int st = c.stk.size();//��¼while���Ŀ�ʼ��ַ
	cur = lexer.getsym();
	if (cur.type != keywordsymTable.lparen)
	{
		err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
		return 0;
	}
	else
	{
		if (p)cout << "����������" << endl;
		if (expression(funcname) == 1)
		{
			if (cur.type != keywordsymTable.rparen)
			{
				err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
				return 0;
			}
			else
			{
				if (p)cout << "����������" << endl;
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
int myParser::returnsentence(string funcname)//ע�⣬returnsentence��Ҫ����ά��������hasreturn��willreturn
{
	if (p)cout << "����״̬returnsentence,��return����ں���" << funcname << "��" << endl;
	//cur = lexer.getsym();
	func& f = getFunc(funcname);

	commandSet& c = findCommandSet(funcname);
	mysymTable& s = findsymTable(funcname);
	if (expression(funcname) == -1)//��������Ƿֺ�
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

			cur = lexer.getsym();//ע��sentence��Ҫ�ٶ���֮���һ���ַ�����ͬ
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
			command com("STO", 0, 2);//��0���2��Ԫ�Ƿ���ֵ�ռ�
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
	���ڵ�ǰ�����ķ��ű�����ҷ����Ƿ���ڣ�Ȼ��ȥȫ�ֱ������ű���ҷ����Ƿ����
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
	if (p)cout << "����״̬scanf,�ö���������ں���" << funcname << endl;
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
		if (p)cout << "����������" << endl;
		cur = lexer.getsym();
		if (cur.type != keywordsymTable.ident)
		{
			err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
			return 0;
		}
		else
		{
			if (p)cout << "������ʶ��" << cur.lexeme << endl;
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
				if (p)cout << "����������" << endl;
				cur = lexer.getsym();
				if (cur.type != keywordsymTable.semicolon)
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
				else
				{
					if (p)cout << "�����ֺ�" << endl;
					cur = lexer.getsym();
					return 1;
				}
			}
		}
	}
}
int myParser::printfsentence(string funcname)
{
	if (p)cout << "����״̬printf,�ö���������ں���" << funcname << endl;
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
		if (p)cout << "����������" << endl;
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
				if (p)cout << "����������" << endl;
				cur = lexer.getsym();
				if (cur.type != keywordsymTable.semicolon)
				{
					err.repor(lexer.line, lexer.curpos, 2, cur.lexeme);
					return 0;
				}
				else
				{
					if (p)cout << "�����ֺ�" << endl;
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
