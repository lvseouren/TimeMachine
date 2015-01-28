#ifndef RECORD_H_
#define RECORD_H_

#include"Itime.h"
#include<string>
#include<stdio.h>
#include<iostream>
using std::string;
using std::ostream;
using std::cout;
using std::endl;

enum workType
{
	LEARN = 0,
	WRITE = 1,
	READ = 2,
	TRAIN = 3,
	ENTERTAINMENT = 4,
	WORK = 5,
	CODING = 6,
	SUNDRY = 7,
	WASTETIME = 8
};
//����saveTofile���������𱣴��¼����ͳ������������Ծ����Ƿ��������
enum saveType
{
	recordSave = 0,
	statSave = 1,
	statTenSave,
	BtSave
};
class Record
{
public:
	Record();
	Record(Itime t1,Itime t2,string job);
public:
	void SetStartTime(Itime T);
	void SetJob(string jobStr);
	void SetEndTime(Itime T);
	void SaveToFile(const string&,const string&,saveType);
	workType GetWorkType();
	Itime GetJobTimeLength();
	//����<<
	friend ostream& operator<<(ostream &,const Record& );
private:
	Itime startTime;
	Itime endTime;
	string jobDescription;
	string workStr;//use to save����
	workType m_workType;

	//auxiliary method
	//����startTime endTime����ʱ�������ַ���
	string GetTimeString();
	//����jobDescription����ȡ�������ͣ���Ҫ�û���ϣ�����������롾��.
	void GetTypeFromJobStr();
	//����m_workType�����Ӧ���ַ��������Ե�Ϊ�����ʱ��,���û�û���롾��,��jobDescription����ӡ�Z��
	void PrintWorkType();
};

Itime StrConvertToItime(const string&);

#endif