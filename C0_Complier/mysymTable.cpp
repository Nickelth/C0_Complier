#include"mysymTable.h"
mysymTable keywordsymTable("global");

mysymTable::mysymTable(string s)
{
	belong = s;
	
}
string mysymTable::findsymType(int i)
{
	if (i == 0)
	{
		return "nul";
	}
	if (i == 1)
	{
		return "ident";
	}
	if (i == 2)
	{
		return "number";
	}
	if (i == 3)
	{
		return "plus";
	}
	if (i == 4)
	{
		return "minus";
	}
	if (i == 5)
	{
		return "multiply";
	}
	if (i == 6)
	{		
		return "divide";
	}
	if (i == 7)
	{
		return "assign";
	}
	if (i == 8)
	{
		return "lparen";
	}
	if (i == 9)
	{
		return "rparen";
	}
	if (i == 10)
	{
		return "comma";
	}
	if (i == 11)
	{
		return "semicolon";
	}
	if (i == 12)
	{
		return "lbrace";
	}
	if (i == 13)
	{
		return "rbrace";
	}
	if (i == 14)
	{
		return "elsesym";
	}
	if (i == 15)
	{
		return "ifsym";
	}
	if (i == 16)
	{
		return "intsym";
	}
	if (i == 17)
	{
		return "mainsym";
	}
	if (i == 18)
	{
		return "printfsym";
	}
	if (i == 19)
	{
		return "returnsym";
	}
	if (i == 20)
	{
		return "scanfsym";
	}
	if (i == 21)
	{
		return "voidsym";
	}
	if (i == 22)
	{
		return "whilesym";
	}
	
	return "nul";
}

int mysymTable::findWord(string s)
{
	if (words.empty())
	{
		return -1;
	}
	for (auto i : words)
	{
		if (i.lexeme == s)
		{
			return i.type;
		}
	}
	return -1;
}
myToken& mysymTable::getWord(string s)
{
	for(int i=0;i<words.size();i++)
	{
		if (words[i].lexeme == s)
		{
			return words[i];
		}
	
	}
}