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
//用于saveTofile函数，区别保存记录还是统计两种情况，以决定是否输出【】
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
	//重载<<
	friend ostream& operator<<(ostream &,const Record& );
private:
	Itime startTime;
	Itime endTime;
	string jobDescription;
	string workStr;//use to save【】
	workType m_workType;

	//auxiliary method
	//根据startTime endTime返回时间区间字符串
	string GetTimeString();
	//根据jobDescription来获取工作类型，需要用户配合，即在最后输入【】.
	void GetTypeFromJobStr();
	//根据m_workType输出对应的字符串，可以当为杂项的时候,即用户没输入【】,在jobDescription后添加【Z】
	void PrintWorkType();
};

Itime StrConvertToItime(const string&);

#endif