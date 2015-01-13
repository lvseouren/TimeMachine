#ifndef RECORD_H_
#define RECORD_H_

#include"Itime.h"
#include<string>
#include<stdio.h>
using std::string;

class Record
{
public:
	Record();
	Record(Itime t1,Itime t2,string job);
public:
	void SetStartTime(Itime T);
	void SetJob(string jobStr);
	void SetEndTime(Itime T);
	void SaveToFile(FILE*);
private:
	Itime startTime;
	Itime endTime;
	string jobDescription;

	//auxiliary method
	//����startTime endTime����ʱ�������ַ���
	string GetTimeString();
};

#endif