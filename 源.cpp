//TODO:重构main函数，使用extract method方法。——DONE
//TODO:统计输出中增加总计时间这一项
//TODO:开始一个工作计时立刻将开始时间输出，结束时补上后面部分——DONE
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
bool isPassDay(Itime& currentTime);//跨日处理逻辑
string StrToFilename(const string& dateStr);//根据用户输入的日期生成对应文件名
//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//这是个以秒为单位的时间，超过这个时间不记录就会自动生成一条Btime记录
const string FILEPATH = "D:\\文档\\时光机\\"; 
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


	//filename = "D:\\文档\\时光机\\";//todo:将文件名称改为当天的日期
	filename = FILEPATH;
	filename = filename+lastRecordTime.ItimeToFileString();
	
	char *safeInput = new char[128];//用来过滤结束讯号的输入
	bool work = true;
	while(work)
	{
		if(currentState==NORECORD)
		{
			cout<<"请输入将要进行的工作：";
			cin.clear();
			cin.sync();//防止输入"ee"时出错
			
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
					cout<<"输入为空，请重新输入将要进行的工作：";
				}

			}

			Itime currentTime = GetCurrentTime();
			if((currentTime-lastRecordTime).ItimeToSecond() > BATTIMESWITCH)
			{
				//todo:B时间自动记录
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
			cout<<"计时中，请输入 e 来结束当前工作的计时(要结束程序请输入q)：";			
		}
		if(currentState == RECORDING)
		{
			cin.clear();
			cin.sync();//防止输入"ee"时出错
			
			cin.getline(safeInput,128);
			string tempStr = safeInput;
			if(tempStr.length()!=1)//如果输入多余一个字符，则相当于输入不合法,'a'与'q''e'都不相同，故为不合法
			{
				controlFlag = 'a';
			}
			else
			{
				controlFlag = tempStr[0];
			}

			//以下为输入处理逻辑
			if(controlFlag=='e')
			{
				//停止计时并保存当前记录
				SaveRecord(RecordArray);
			}
			else if(controlFlag == 'q')
			{
				work = false;
				//保存并关闭软件
				quit(RecordArray);
			}
			//test filereader
			else if(controlFlag == 'r')
			{
				//测试记录读取器功能
				TestRead();
			}
			else if(controlFlag == 't' || controlFlag == 'i')//统计，将结果写入到当日统计文件中去
			{
				//进行统计：
				DoStatistic(controlFlag);
			}
			else
			{
				//不合法输入
				cout<<"输入不合法:"<<tempStr<<endl<<"计时中，请输入 e 来结束当前工作的计时(要结束程序请输入q)：";
			}
		}
	}
	//start finit
	delete[] job;
	delete[] safeInput;
	//end finit

	cout<<"保存当前记录，程序正在关闭..."<<endl;
	cout<<"话说大哥麻烦你快点把我的GUI做出来好吗..."<<endl;
	//等待两秒，关闭程序
	Itime t1= GetCurrentTime();
	while(1)
	{
		Itime t2=GetCurrentTime();
		if((t2-t1).ItimeToSecond()>2)
			break;
	}
}

//以下为从main中提取出来的函数
void DoStatistic(const char& flag)
{
	string fileToStat = "D:\\文档\\时光机\\";
	string finalDateStr;
	//根据flag是i还是t来选择是默认统计还是指定统计
	if( 'i' == flag )
	{
	cout<<"请输入要统计的日期（格式为year-month-day,格式不对后果自负):";
	string dateStr;
	cin>>dateStr;
	finalDateStr = StrToFilename(dateStr);
	}
	else//t,默认统计
	{
		finalDateStr = lastRecordTime.ItimeToFileString();
	}

	fileToStat += finalDateStr;

	/*RecordFileReader reader(filename);*/
	RecordFileReader reader(fileToStat);
			
	vector<Record> todayRecordArray = reader.GetRecordArray();

	RecordStatistic RecStatter;
	RecStatter.makeStatistic(todayRecordArray);
				

	//将数据写入到文件中
	//TODO: 统计哪天的数据就保存到哪天的统计文件中去
	string stFilename = "D:\\文档\\时光机\\统计\\";
	/*stFilename += lastRecordTime.ItimeToFileString();*/
	
	stFilename += finalDateStr;
	RecStatter.PrintResult(stFilename);

	cout<<"计时中，请输入 e 来结束当前工作的计时(要结束程序请输入q)：";
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

	//以下算法的前提是,"-"只占用一个字符的长度
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

	return yearStr+"年"+monStr+"月"+dayStr+"日.txt";
}

void TestRead()
{
	cout<<"开始测试文件读取功能,以下为读取的记录："<<endl;

	RecordFileReader reader(filename);
			
	vector<Record> test = reader.GetRecordArray();
				
	vector<Record>::iterator recordIter;
	int count=1;
	for(recordIter = test.begin();recordIter!=test.end();++recordIter)
	{
		//TODO：重载record类的<<操作符
		cout<<"序号"<<count++<<"——"<<*recordIter;
	}

	cout<<"计时中，请输入 e 来结束当前工作的计时(要结束程序请输入q)：";
}

void quit(vector<Record*>& RecordArray)//q
{
	currentState=NORECORD;
	Itime currentTime = GetCurrentTime();

	//如果跨日了，更新filename使之存储到新的文件中
	if(isPassDay(currentTime))
	{
		filename = FILEPATH+currentTime.ItimeToFileString();
	}
	lastRecordTime = currentTime;
	(*RecordArray.back()).SetEndTime(currentTime);
				
	//todo:将该条记录写到文件中去
				
	(*RecordArray.back()).SaveToFile(filename,"a",recordSave);
}

void SaveRecord(vector<Record*>& RecordArray)//e
{
	currentState=NORECORD;
	Itime currentTime = GetCurrentTime();
	//如果跨日了，更新filename使之存储到新的文件中
	if(isPassDay(currentTime))
	{
		filename = FILEPATH+currentTime.ItimeToFileString();
	}
	lastRecordTime = currentTime;
	(*RecordArray.back()).SetEndTime(currentTime);
				
	//todo:将该条记录写到文件中去
				
	(*RecordArray.back()).SaveToFile(filename,"a",recordSave);			
}

//超过时间自动生成B记录
void AutoRecord(vector<Record*>& RecordArray,Itime& currentTime)
{
	Record *BTimeRecord = new Record(lastRecordTime,currentTime,"这段时间啥事没做，注意了，你在浪费时间哦【B】");
	BTimeRecord->SetJob("这段时间啥事没做，注意了，你在浪费时间哦【B】");

	//如果跨日了，更新filename使之存储到新的文件中
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