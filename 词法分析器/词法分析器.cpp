#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<iostream>
#include<fstream>
using namespace std;

struct symbol
{
	char* str;//符号
	int coding;//种别码
};
char* keyword_list[34] =
{
	"void","char","int","float","double","short","long",
	"signed","unsigned","struct","union","enum",
	"typedef","sizeof","auto","static","register",
	"extern","const","volatile","return","continue",
	"break","goto","if","else","switch","case","default",
	"for","do","while","scanf","printf"
};
char* operator_list[45] =
{
	"{","}","[","]","(",")",".","->","~","++","--","!","&","*","/",
	"%","+","-","<<",">>",">",">=","<","<=","==","!=","^","|","&&",
	"||","?","=","/=","*=","%=","+=","-=","&=","^=","|=",",","#",";",
	":","\""
};

char ch;//要读入的字符
char strToken[20] = "";//读入的字符串

int eof_flag = 0;//结束标识

int num = 1;//种别编码数字，为了实现递增
int row = 1;//错误处理时指明行号

struct symbol keywords[34];//34个关键字集
struct symbol identifiers[45];//44个界符集

FILE* fp = NULL;

ofstream out;

//给单词符号，设定种别编码
void initialization()
{
	//给关键字设置种别码
	for (int i = 0; i < 34; i++)
	{
		keywords[i].str = keyword_list[i];
		keywords[i].coding = num;
		num++;
	}

	//给界符设定种别码
	for (int i = 0; i < 45; i++)
	{
		identifiers[i].str = operator_list[i];
		identifiers[i].coding = num;
		num++;
	}
	//结束后num=80
	//数字80，标识符81
}

//读取下一个字符进入ch
void getNextChar(FILE* ffp)
{
	if ((ch = getc(ffp)) == EOF)
	{
		eof_flag = 1;
	}
	if (ch == '\n')
		row++;
}

//检查ch的字符是否为空格、回车、制表符，若为上述符号，则反复调用getNextChar函数
//直到读出正常字符
void getbc(FILE* ffp)
{
	while (ch==' '||ch=='\n'||ch=='\t')
	{
		getNextChar(ffp);
	}
}

//判断ch是否为字母
bool isLetter(char ch)
{
	return isalpha(ch);
	/*
	ctype头文件中函数，isalpha()判断一个字符是否是字母。
	返回1表示是字母，返回0表示不是。
	*/
}

//判断ch是否为数字
bool isDigit(char ch)
{
	return isdigit(ch);
}

//判断ch是否为下划线
bool isUnderline(char ch)
{
	if (ch == '_')
		return 1;
	else
	{
		return 0;
	}
}

//将输入的字符ch连接到strToken
void concat()
{
	char* temp = &ch;
	strcat(strToken,temp);
	//C 库函数 char *strcat(char *dest, const char *src) 把 src 所指向的字符串追加到 dest 所指向的字符串的结尾。
}

//搜索指针回调一个位置
void retract(FILE* ffp)
{
	fseek(ffp, -1, SEEK_CUR);
	ch = ' ';//?
}

//判断strToken中的字符串是否为保留字，若他是保留字则给出他的编码，否则返回0
int reserve_string(char* str)
{
	for (int i = 0; i < 34; i++)
	{
		if ((strcmp(str, keywords[i].str)) == 0)//如果返回值等于 0，则表示 str1 等于 str2。
			return keywords[i].coding;
	}
	return 0;
}

//返回strToken中所识别出的算符和界符编码
int reserve_operator(char* str)
{
	for (int i = 0; i < 45; i++)
	{
		if ((strcmp(str, identifiers[i].str)) == 0)
		{
			return identifiers[i].coding;
		}
	}
	return 0;
}

//出错处理
void error()
{
	printf("\n*********************************************************\n");
	printf("row %d Ivalid symbol !!!", row);
	printf("\n*********************************************************\n");
	exit(0);
}

//词法分析
void LexiscalAnalyzer()
{
	int code = 0;
	strcpy(strToken, "");//初始化
	getNextChar(fp);
	getbc(fp);
	switch (ch)
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
	case 'g':
	case 'h':
	case 'i':
	case 'j':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'o':
	case 'p':
	case 'q':
	case 'r':
	case 's':
	case 't':
	case 'u':
	case 'v':
	case 'w':
	case 'x':
	case 'y':
	case 'z':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
	case '_':
		while (isLetter(ch) || isDigit(ch) || isUnderline(ch))
		{
			concat();
			getNextChar(fp);
		}
		retract(fp);
		code = reserve_string(strToken);
		if (code == 0)
		{
			printf("(%d,%s)\n", 81, strToken);
			out << "81" << " " << strToken << endl;
		}
		else
		{
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		while (isDigit(ch))
		{
			concat();
			getNextChar(fp);
		}
		retract(fp);
		printf("(%d,%s)\n", 81, strToken);
		out << "81" << " " << strToken << endl;
		break;
	case '{':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '}':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case'[':
	case ']':
	case '(':
	case ')':
	case '.':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '-':
		concat();
		getNextChar(fp);
		if (ch == '>')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '-')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '~':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '+':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '+')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '*':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '&':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '&')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '!':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '%':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '<':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '<')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '>':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '>')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '=':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '^':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '|':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '|')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case '?':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '/':
		concat();
		getNextChar(fp);
		if (ch == '=')
		{
			concat();
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		else if (ch == '/')//跳过注释
		{
			getNextChar(fp);
			while (ch!='\n')
			{
				getNextChar(fp);
			}
			break;
		}
		else if (ch == '*')//跳过注释
		{
			getNextChar(fp);
			while (ch != '*')
			{
				getNextChar(fp);
			}
			getNextChar(fp);
			if (ch == '/');
			break;
		}
		else
		{
			retract(fp);
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
	case ',':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '#':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case ';':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case ':':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	case '"':
		concat();
		code = reserve_operator(strToken);
		printf("(%d,%s)\n", code, strToken);
		out << code << " " << strToken << endl;
		break;
	default:
		if (ch == EOF)
		{
			eof_flag = 1;
			break;
		}
		error();
	}
}

//主函数
int main()
{
	initialization();
	char name[1024] = "E:/360MoveData/Users/dell/Desktop/in.txt";
	cout << "从文件读入：";
	//cin >> name;
	fp = fopen(name, "r");
	out.open("result.txt");
	while (!feof(fp))
	{
		if (eof_flag==1)
		{
			exit(1);
		}
		LexiscalAnalyzer();
	}
	fclose(fp);
	out.close();
	return 0;
}