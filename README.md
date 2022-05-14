参考《编译与反编译技术实战》[编译与反编译技术实战-庞建民主编 刘晓楠 陶红伟 岳峰 戴超编著-微信读书 (qq.com)](https://weread.qq.com/web/bookDetail/cf9326c05d185ecf9de3029)

# 一、思路介绍

依据状态转换图进行编码：

![屏幕截图 2022-05-14 123324](F:\Obsidian_DataBase\My_Computer\08.附件\01.王道C\屏幕截图 2022-05-14 123324.png)

首先构建词法分析器的状态转换图，然后每个分支对应一个case语句即可。

# 二、参数介绍

自上向下介绍参数：

#### 1.符号结构体，存储每一个符号和符号对应的种别编码。

```cpp
struct symbol
{
	char* str;//符号
	int coding;//种别码
};
```

#### 2.char* keyword_list[34]

存储C++的34个关键字。（没有定义cout和cin，但是定义了scanf和printf，如果需要可以自己添加）

#### 3.char* operator_list[45]

存储C++的45个界符和运算符。参考书中没有加双引号，我在末尾加了双引号 **"** 。

#### 4.char ch

ch用于存储读入的一个字符，

如：#include，那么每次只读入一个字符，ch则分别依次存储#、i、n、...

#### 5.char strToken[20] = "";

strToken用于存储读入的字符串。

如上例：#include，strToken会分别存储#和include（是依次存储，具体见词法分析函数）。

#### 6.int eof_flag = 0;

结束标识

全文只有在`getNextChar`函数更改了它的值：

```cpp
void getNextChar(FILE* ffp)
{
	if ((ch = getc(ffp)) == EOF)
	{
		eof_flag = 1;
	}
	if (ch == '\n')
		row++;
}
```

这是因为词法分析会超前搜索一个字符。

**什么是超前搜索？**

比如：搜索`ch == '\n'`时，当计算机搜索到第一个=时，计算机是无法判断它是'='还是'=='的，这时就必须超前搜索一个字符，如果'='后面的字符不是'='，那么这个字符就是'='号，如果后面还有一个'='，那么这个字符就是‘==’号。

也因为需要超前搜索，所以后面会有一个回退文件指针函数`retract(FILE* ffp)`



所以就有可能超前搜索到结束字符，而这时fp的文件指针尚未结束，所以需要文件结束标识。

#### 7.int num = 1;

这是为了方便初始化符号的种别编码，也可以看到只在初始化函数`initialization()`中使用了一次。

#### 8.int row = 1;

用于在错误处理时指明行号，只在`getNextChar、error`函数中调用。`getNextChar`中是累加row行号，`error`中是输出行号。

#### 9.

struct symbol keywords[34];//34个关键字集
struct symbol identifiers[45];//45个界符集

定义成结构体集合，是因为存储符号的同时也需要存储每一个的种别编码。

#### 10.FILE* fp = NULL;

输入文件指针。就是用于读取的文件。

#### 11.ofstream out;

输出文件，将进行词法分析后的字符和种别码输出到该文件。

对于out的操作和cout是一样的。

如输出：

`cout<<...`

out也是这样：

`out<<...`

#### 12.void initialization()

初始化函数。

初始化`struct symbol keywords[34]、struct symbol identifiers[45]`。

#### 13.void getNextChar(FILE* ffp)

读取下一个字符到ch。

#### 14.void getbc(FILE* ffp)

getbc函数用于检查ch是否为空格、换行、制表符这些符号。如果是这些符号，则需要继续往后读取字符。

#### 15.bool isLetter(char ch)、bool isDigit(char ch)、bool isUnderline(char ch)

判断ch是否是字母、数字、下划线。

其中`isLetter、isDigit`使用了ctype头文件中函数。

isalpha()判断一个字符是否是字母。返回1表示是字母，返回0表示不是。

#### 16.void concat()

将字符ch输入strToken中。

#### 17.void retract(FILE* ffp)

指针超前搜索回调函数。

用文件指针做传入参数是因为一个文件只有一个读写指针。

#### 18.int reserve_string(char* str)

寻找关键字的种别编码。

#### 19.int reserve_operator(char* str)

寻找界符和算术运算符的种别编码。

#### 20.void error()

出错处理

#### 21.void LexiscalAnalyzer()

词法分析函数，用于词法分析，整体主要采用switch语句。



```cpp
int code = 0;
strcpy(strToken, "");//初始化，在主函数中对其进行迭代，每次读取一个字符串都需要初始化一次
getNextChar(fp);//读取一个字符
getbc(fp);//检查字符有效性，是否为空格、换行、制表符
switch (ch)//ch有效，开始分析
{...}
```

词法分析函数的思想对应状态转换图，先读入一个字符存入ch。

然后用switch判断ch的类型：

字母、下划线，则是关键字，

数字开头的，则是数字，

其他符号开头的，则为界符、运算符。



而在关键字和数字中，因为关键字、数字长度不一，所以都需要迭代超前搜索。



# 代码中多有重复，故摘取关键代码解析

**判断是否为关键字的case语句：**

```cpp
		while (isLetter(ch) || isDigit(ch) || isUnderline(ch))
		{
            //第一次进入循环ch必是字母或下划线，将其加入strToken中，并超前搜索下一个字符
            //后面迭代开始判断是否为字母、数字、下划线。
			concat();
			getNextChar(fp);
		}
//while循环结束后，strToken中存放的就是一个完整的字符串。
//超前搜索结束，需要回退一个位置的文件指针。
		retract(fp);
//获取strToken的种别编码
		code = reserve_string(strToken);
		if (code == 0)//code==0表示，识别的字符串是一个标识符，输出其种别编码81
		{
			printf("(%d,%s)\n", 81, strToken);//打印到屏幕
			out << "81" << " " << strToken << endl;//这里就是输出到给定文件。
            //因为上面定义了ofstream流的out。我们将在主函数中将其与文件绑定。
		}
		else
		{
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
```

**对数字的分析大体同对关键字的分析。**



**对界符、运算符的分析：以case '-' 为例：**

```cpp
		concat();//首先将识别的ch加入strToken，因为前面是case语句，已经识别了ch字符。
		getNextChar(fp);//超前搜索下一个字符，因为对于-号有--、-=、->等字符，必须进行超前搜索才能识别。
		if (ch == '>')
		{
			concat();//这一段分析同上分析关键字
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
			retract(fp);//如果都不是，则这个字符串就是-号，由于超前搜索了一个字符，所以需要回退一个字符的文件指针。
			code = reserve_operator(strToken);
			printf("(%d,%s)\n", code, strToken);
			out << code << " " << strToken << endl;
		}
		break;
```

# 主函数

```CPP
int main()
{
	initialization();//先对关键字、符号等初始化
	//char name[1024] = "E:/360MoveData/Users/dell/Desktop/in.txt";//这里是为了方便调试，所以固定了一个文件名
	cout << "从文件读入：";
	cin >> name;
	fp = fopen(name, "r");
	out.open("result.txt");//把定义的out文件流绑定到result.txt文件。
    //这里也可以像输入一样，定义一个char数组，然后指定输出文件。
	while (!feof(fp))//这是对词法分析的主循环。词法分析函数只是分析一个字符串。
	{
		if (eof_flag==1)//这是超前搜索到文件末尾标识。
		{
			exit(1);
		}
		LexiscalAnalyzer();
	}
	fclose(fp);
	out.close();
	return 0;
}
```



