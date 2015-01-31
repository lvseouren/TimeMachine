#ifndef ITIME_H_
#define ITIME_H_
#include<string>
#include<iostream>
#include<time.h>
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
	//��������Ϊ��λ��ʱ����
	//todo:�޸�Itime���-�������ʹ֮����Itime���͵�ֵ
	//todo:���һ��method������Itime���������Ϊ��λ������
	Itime operator-(const Itime& rhs);
	Itime operator+=(const Itime& rhs);
	Itime operator+(const Itime& rhs);
	bool operator<(const Itime& rhs);

	int ItimeToSecond();
	string ItimeToString();
	//�˺����������ع�����ʱ�ַ���
	string ItimeLenToString();
	//�˺�����������ͳ�Ƽ�ʱ�ַ���
	string ItimeLenToStatString();
	//�˺������������ļ����ַ���
	string ItimeToFileString();
	//�˺�������ʮ��ͳ���ļ����ַ���
	string GetTenStatFileName();
	friend ostream& operator<<(ostream&,Itime);
};
Itime GetCurrentTime();

#endif