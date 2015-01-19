//TODO:�ع�main������ʹ��extract method����������DONE
//TODO:ͳ������������ܼ�ʱ����һ��
//TODO:��ʼһ��������ʱ���̽���ʼʱ�����������ʱ���Ϻ��沿�֡���DONE
//start lib 
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<time.h>

#include"RecordFileReader.h"
#include"RecordStatistic.h"

//end lib 

//start my header
#include"Itime.h"
#include"Record.h"
//end my header

//start using
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
//end using


//start class
enum STATE
{
	NORECORD=0,
	RECORDING=1
};



//start function declare
void DoStatistic(const char&);//t
void TestRead();//r
void quit(vector<Record*>& RecordArray);//q
void SaveRecord(vector<Record*>& RecordArray);//e
void AutoRecord(vector<Record*>& RecordArray,Itime& currentTime);//B
bool isPassDay(Itime& currentTime);//���մ����߼�
string StrToFilename(const string& dateStr);//�����û�������������ɶ�Ӧ�ļ���
//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//���Ǹ�����Ϊ��λ��ʱ�䣬�������ʱ�䲻��¼�ͻ��Զ�����һ��Btime��¼
const string FILEPATH = "D:\\�ĵ�\\ʱ���\\"; 
//end global variable

void main()
{
	currentState=NORECORD;
	lastRecordTime = GetCurrentTime();
	cout<<"welcome to TIMEMACHINE!"<<endl;
	/*string job;*/
	char* job = new char[1024];
	char controlFlag;
	vector<Record*> RecordArray;


	//filename = "D:\\�ĵ�\\ʱ���\\";//todo:���ļ����Ƹ�Ϊ���������
	filename = FILEPATH;
	filename = filename+lastRecordTime.ItimeToFileString();
	
	char *safeInput = new char[128];//�������˽���Ѷ�ŵ�����
	bool work = true;
	while(work)
	{
		if(currentState==NORECORD)
		{
			cout<<"�����뽫Ҫ���еĹ�����";
			cin.clear();
			cin.sync();//��ֹ����"ee"ʱ����
			
			while(1)
			{
				cin.getline(job,1024);
				string safeInputForJobStr = job;
				if(!safeInputForJobStr.empty())
				{
					break;
				}
				else
				{
					cout<<"����Ϊ�գ����������뽫Ҫ���еĹ�����";
				}

			}

			Itime currentTime = GetCurrentTime();
			if((currentTime-lastRecordTime).ItimeToSecond() > BATTIMESWITCH)
			{
				//todo:Bʱ���Զ���¼
				AutoRecord(RecordArray,currentTime);
			}
				
			Record *newRecord = new Record();
			RecordArray.push_back(newRecord);
			newRecord->SetStartTime(currentTime);
			newRecord->SetJob(job);
			
			//output the start time to the file
			FILE* file;
			fopen_s(&file,filename.c_str(),"a");
			fprintf(file,currentTime.ItimeToString().c_str());
			fclose(file);

			currentState = RECORDING;
			cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";			
		}
		if(currentState == RECORDING)
		{
			cin.clear();
			cin.sync();//��ֹ����"ee"ʱ����
			
			cin.getline(safeInput,128);
			string tempStr = safeInput;
			if(tempStr.length()!=1)//����������һ���ַ������൱�����벻�Ϸ�,'a'��'q''e'������ͬ����Ϊ���Ϸ�
			{
				controlFlag = 'a';
			}
			else
			{
				controlFlag = tempStr[0];
			}

			//����Ϊ���봦���߼�
			if(controlFlag=='e')
			{
				//ֹͣ��ʱ�����浱ǰ��¼
				SaveRecord(RecordArray);
			}
			else if(controlFlag == 'q')
			{
				work = false;
				//���沢�ر����
				quit(RecordArray);
			}
			//test filereader
			else if(controlFlag == 'r')
			{
				//���Լ�¼��ȡ������
				TestRead();
			}
			else if(controlFlag == 't' || controlFlag == 'i')//ͳ�ƣ������д�뵽����ͳ���ļ���ȥ
			{
				//����ͳ�ƣ�
				DoStatistic(controlFlag);
			}
			else
			{
				//���Ϸ�����
				cout<<"���벻�Ϸ�:"<<tempStr<<endl<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";
			}
		}
	}
	//start finit
	delete[] job;
	delete[] safeInput;
	//end finit

	cout<<"���浱ǰ��¼���������ڹر�..."<<endl;
	cout<<"��˵����鷳������ҵ�GUI����������..."<<endl;
	//�ȴ����룬�رճ���
	Itime t1= GetCurrentTime();
	while(1)
	{
		Itime t2=GetCurrentTime();
		if((t2-t1).ItimeToSecond()>2)
			break;
	}
}

//����Ϊ��main����ȡ�����ĺ���
void DoStatistic(const char& flag)
{
	string fileToStat = "D:\\�ĵ�\\ʱ���\\";
	string finalDateStr;
	//����flag��i����t��ѡ����Ĭ��ͳ�ƻ���ָ��ͳ��
	if( 'i' == flag )
	{
	cout<<"������Ҫͳ�Ƶ����ڣ���ʽΪyear-month-day,��ʽ���Ժ���Ը�):";
	string dateStr;
	cin>>dateStr;
	finalDateStr = StrToFilename(dateStr);
	}
	else//t,Ĭ��ͳ��
	{
		finalDateStr = lastRecordTime.ItimeToFileString();
	}

	fileToStat += finalDateStr;

	/*RecordFileReader reader(filename);*/
	RecordFileReader reader(fileToStat);
			
	vector<Record> todayRecordArray = reader.GetRecordArray();

	RecordStatistic RecStatter;
	RecStatter.makeStatistic(todayRecordArray);
				

	//������д�뵽�ļ���
	//TODO: ͳ����������ݾͱ��浽�����ͳ���ļ���ȥ
	string stFilename = "D:\\�ĵ�\\ʱ���\\ͳ��\\";
	/*stFilename += lastRecordTime.ItimeToFileString();*/
	
	stFilename += finalDateStr;
	RecStatter.PrintResult(stFilename);

	cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";
}
string StrToFilename(const string& dateStr)
{
	//TODO:hour,min,sec change to year month day;
	int startYear;
	int endYear;
	int startMon;
	int endMon;
	int startDay;
	int endDay;

	//�����㷨��ǰ����,"-"ֻռ��һ���ַ��ĳ���
	string test = "-";
	int lengh = test.length();

	startYear = 0;
	endYear = dateStr.find('-')-1;

	startMon = endYear+2;
	endMon = dateStr.rfind('-')-1;

	startDay = endMon+2;
	endDay = dateStr.length()-1;

	string yearStr = dateStr.substr(startYear,endYear-startYear+1);
	string monStr = dateStr.substr(startMon,endMon-startMon+1);
	string dayStr = dateStr.substr(startDay,endDay-startDay+1);

	return yearStr+"��"+monStr+"��"+dayStr+"��.txt";
}

void TestRead()
{
	cout<<"��ʼ�����ļ���ȡ����,����Ϊ��ȡ�ļ�¼��"<<endl;

	RecordFileReader reader(filename);
			
	vector<Record> test = reader.GetRecordArray();
				
	vector<Record>::iterator recordIter;
	int count=1;
	for(recordIter = test.begin();recordIter!=test.end();++recordIter)
	{
		//TODO������record���<<������
		cout<<"���"<<count++<<"����"<<*recordIter;
	}

	cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";
}

void quit(vector<Record*>& RecordArray)//q
{
	currentState=NORECORD;
	Itime currentTime = GetCurrentTime();

	//��������ˣ�����filenameʹ֮�洢���µ��ļ���
	if(isPassDay(currentTime))
	{
		filename = FILEPATH+currentTime.ItimeToFileString();
	}
	lastRecordTime = currentTime;
	(*RecordArray.back()).SetEndTime(currentTime);
				
	//todo:��������¼д���ļ���ȥ
				
	(*RecordArray.back()).SaveToFile(filename,"a",recordSave);
}

void SaveRecord(vector<Record*>& RecordArray)//e
{
	currentState=NORECORD;
	Itime currentTime = GetCurrentTime();
	//��������ˣ�����filenameʹ֮�洢���µ��ļ���
	if(isPassDay(currentTime))
	{
		filename = FILEPATH+currentTime.ItimeToFileString();
	}
	lastRecordTime = currentTime;
	(*RecordArray.back()).SetEndTime(currentTime);
				
	//todo:��������¼д���ļ���ȥ
				
	(*RecordArray.back()).SaveToFile(filename,"a",recordSave);			
}

//����ʱ���Զ�����B��¼
void AutoRecord(vector<Record*>& RecordArray,Itime& currentTime)
{
	Record *BTimeRecord = new Record(lastRecordTime,currentTime,"���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ��B��");
	BTimeRecord->SetJob("���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ��B��");

	//��������ˣ�����filenameʹ֮�洢���µ��ļ���
	if(isPassDay(currentTime))
	{
		filename = FILEPATH+currentTime.ItimeToFileString();
	}
	lastRecordTime = currentTime;
					
	BTimeRecord->SaveToFile(filename,"a",statSave);

	RecordArray.push_back(BTimeRecord);
}

bool isPassDay(Itime& currentTime)//when add const ,it can't pass compile--why?
{
	if(currentTime < lastRecordTime)
		return true;
}