#ifndef _RECORDFILEREADER_H_
#define _RECORDFILEREADER_H_

//start lib
#include<stdio.h>
#include<vector>
#include<string>
#include<iostream>
#include"Record.h"
//end lib

//start using
using std::vector;
using std::string;
using std::cin;
//end using

class RecordFileReader
{
private:
	FILE* file;//file to read 
	vector<string> recordStrArray;
	vector<Record> recordArray;
private:
	void Parse();//解析从文件中读取到的string数组,call RawParse() in for()
	void RawParse(const string&,Itime&,Itime&,string&);//parse a raw record
	void Read();//从文件中读取Record数据，一行读到一个string中去。
public:
	RecordFileReader();
	RecordFileReader(FILE*);
	~RecordFileReader();

public:
	vector<Record> GetRecordArray();//return the record after reading the file;call Read() & Parse();
	void SetFileToRead(FILE*);
	
};

#endif