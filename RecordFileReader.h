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
	//FILE* file;//file to read 
	string filename;//���ļ�������FILE*ʹ��fopen��fclose�ܹ������Ͻ���
	vector<string> recordStrArray;
	vector<Record> recordArray;
private:
	void Parse();//�������ļ��ж�ȡ����string����,call RawParse() in for()
	bool RawParse(const string&,Itime&,Itime&,string&);//parse a raw record
	void Read();//���ļ��ж�ȡRecord���ݣ�һ�ж���һ��string��ȥ��
public:
	RecordFileReader();
	RecordFileReader(const string&);
	~RecordFileReader();

public:
	vector<Record> GetRecordArray();//return the record after reading the file;call Read() & Parse();
	void SetFileToRead(const string&);
	
};

#endif