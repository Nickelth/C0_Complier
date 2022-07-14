#include"myLexer.h"
myLexer::myLexer(string path)
{
	this->path = path;
	fin.open(path, ios::in);
	if (!fin)
	{
		cout << "Oops!无法打开文件！:(" << endl;
		exit(0);
	}
	else
	{
		cout << "成功打开文件!:D" << endl;
	}
}
int myLexer::isKeyword(string s)
{
	int t = keywordsymTable.findWord(s);
	return  t;
}
/*int myLexer::isIdent(string s)
{
	if (s.empty() || (!isalpha(s[0]) && s[0] != '_'))
	{
		return -1;
	}
	else
	{
		for (auto i : s)
		{
			if (isalpha(i) || i == '_' || isdigit(i))
			{
				continue;
			}
			else
			{
				return -1;
			}
		}
		return 1;
	}
}*/
/*int myLexer::isNumber(string s)
{
	int res=0;
	for (auto i : s)
	{
		if (!isdigit(i))
		{
			return -1;
		}
		else
		{
			res = res * 10 + (int)(i - '0');
		}
	}
	if (s.length() > 10)
	{
		string msg = "检测到过大数字！";
		err.repor(line, curpos, msg);
		exit(0);
	}
	return res;
}*/
int myLexer::isOperator(char s)
{
	if (s == '+')
	{
		return keywordsymTable.plus;
	}
	else if (s == '-')
	{
		return keywordsymTable.minus;
	}
	else if (s == '*')
	{
		return keywordsymTable.multiply;
	}
	else if (s == '/')
	{
		return keywordsymTable.divide;
	}
	else if (s == '=')
	{
		return keywordsymTable.assign;
	}
	else
	{
		return -1;
	}
}
int myLexer::isDelimiter(char s)
{
	if (s == '(')
	{
		return keywordsymTable.lparen;
	}
	else if (s == ')')
	{
		return keywordsymTable.rparen;
	}
	else if (s == '{')
	{
		return keywordsymTable.lbrace;
	}
	else if (s == '}')
	{
		return keywordsymTable.rbrace;
	}
	else if (s == ',')
	{
		return keywordsymTable.comma;
	}
	else if (s == ';')
	{
		return keywordsymTable.semicolon;
	}
	else
	{
		return -1;
	}
}
myToken myLexer::getsym()
{
	auto check = [&](string s)
	{
		for (auto i : s)
		{
			if (i != ' ' && i != 9)
				return 1;
		}
		return 0;
	};//用于防止读入的一行全是空格
	while (buffer.empty() || curpos == buffer.length()||!check(buffer)) {//读入新的一行
		if (fin.peek() == EOF)//如果读到文件末尾，返回一个特殊的词法单元
		{
			return myToken(-1, "", -1);
		}
		getline(fin, buffer);
		line++;
		curpos = 0;
	}
	while (buffer[curpos] == ' ' || buffer[curpos] == 9)curpos++;//忽略空格和tab

	int tmp;
	if (buffer[curpos] == '_' || isalpha(buffer[curpos]))
	{
		string s;
		while(buffer[curpos]=='_'||isalpha(buffer[curpos])||isdigit(buffer[curpos]))s += buffer[curpos++];
		if ((tmp = isKeyword(s))!=-1)
		{
			myToken cur(tmp, s, 0);
			return cur;
		}
		else
		{
			myToken cur(keywordsymTable.ident, s, 0);
			return cur;
		}
	}
	else if (isdigit(buffer[curpos]))
	{
		int res=0;
		string num;
		while (isdigit(buffer[curpos]))
		{
			num += buffer[curpos];
			res = res * 10 + buffer[curpos]-'0';
			curpos++;
		}
		string int_max = "2147483647";
		auto cmp = [](string a,string b)->int{
			if (a.length() > b.length())return 1;
			else if (a.length() < b.length())return 0;
			else
			{
				for (int i = 0; i < a.length(); i++)
				{
					if (a[i] > b[i])return 1;
				}
				return 0;
			}
		};
		if (cmp(num,int_max)== 1)
		{
			err.repor(line, curpos, 1,num);
			exit(0);
		}
		myToken cur(keywordsymTable.number, num, res);
		return cur;
	}
	else if((tmp=isOperator(buffer[curpos]))!=-1)
	{
		string s;
		s += buffer[curpos];
		curpos++;
		myToken cur(tmp, s, 0);
		return cur;
	}
	else if ((tmp = isDelimiter(buffer[curpos])) != -1)
	{
		string s;
		s += buffer[curpos];
		curpos++;
		myToken cur(tmp, s, 0);
		return cur;
	}
	else
	{
		string t;
		t+= buffer[curpos];
		string s = "";
		myToken cur(keywordsymTable.nul, s, 0);
		curpos++;
		err.repor(line, curpos,0,t);
		return cur;
	}
}
