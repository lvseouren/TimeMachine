#ifndef ITIME_H_
#define ITIME_H_
#include<string>
#include<iostream>
using std::ostream;
using std::string;
struct Itime
{
	Itime();
	Itime(int,int,int,int,int,int);
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	//返回以秒为单位的时间间隔
	//todo:修改Itime类的-运算符，使之返回Itime类型的值
	//todo:添加一个method，输入Itime，输出以秒为单位的整数
	Itime operator-(const Itime& rhs);
	int ItimeToSecond();
	string ItimeToString();
	//此函数用来返回工作计时字符串
	string ItimeLenToString();
	//此函数用来返回文件名字符串
	string ItimeToFileString();

	friend ostream& operator<<(ostream&,Itime);
};

#endif