#ifndef _RECORDSTATISTIC_H_
#define _RECORDSTATISTIC_H_

#include<map>
#include<vector>
#include<utility>

#include"Record.h"


using std::vector;
using std::map;
using std::pair;
using std::make_pair;

class RecordStatistic
{
private:
	vector<Record> learnArray;
	vector<Record> writeArray;
	vector<Record> readArray;
	vector<Record> badArray;
	vector<Record> workArray;
	vector<Record> trainArray;
	vector<Record> playArray;
	vector<Record> sundryArray;
	vector<Record> codeArray;

	Itime learnTime;
	Itime writeTime;
	Itime readTime;
	Itime badTime;
	Itime workTime;
	Itime trainTime;
	Itime playTime;
	Itime sundryTime;
	Itime codeTime;

	bool isFirstWrite;
private:
	//function use by self
	void PrintSingleTypeRecord(vector<Record>& ,Itime&,const string&,const string&);
public:
	RecordStatistic();
	~RecordStatistic();

public:
	void makeStatistic(vector<Record>&);
	void PrintResult(const string&);
};

#endif