//start lib 
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<time.h>

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



//start function
Itime GetCurrentTime()//放到time.h中去
{
	time_t now;
	time(&now);
	struct tm* t = new tm();
	localtime_s(t,&now);
	return Itime(t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}
//end function


//start global variable

Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//这是个以秒为单位的时间，超过这个时间不记录就会自动生成一条Btime记录
//end global variable

void main()
{
	currentState=NORECORD;
	lastRecordTime = GetCurrentTime();
	cout<<"welcome"<<endl;
	string job;
	char controlFlag;
	vector<Record*> RecordArray;
	FILE* file = NULL;

	string filename = "D:\\文档\\时光机\\";//todo:将文件名称改为当天的日期
	filename = filename+lastRecordTime.ItimeToFileString();
	

	bool work = true;
	while(work)
	{
		if(currentState==NORECORD)
		{
			cout<<"请输入将要进行的工作：";
			cin.clear();
			cin.sync();//防止输入"ee"时出错
			cin>>job;
			if(job.c_str())
			{
				Itime currentTime = GetCurrentTime();
				if((currentTime-lastRecordTime).ItimeToSecond() > BATTIMESWITCH)
				{
					//todo:B时间自动记录
					Record *BTimeRecord = new Record(lastRecordTime,currentTime,"这段时间啥事没做，注意了，你在浪费时间哦");
					lastRecordTime = currentTime;
					fopen_s(&file,filename.c_str(),"a");
					BTimeRecord->SaveToFile(file);
					fclose(file);
					RecordArray.push_back(BTimeRecord);
					
				}
				
				Record *newRecord = new Record();
				RecordArray.push_back(newRecord);
				newRecord->SetStartTime(currentTime);
				newRecord->SetJob(job);
				currentState = RECORDING;
				cout<<"计时中，请输入 e 来结束当前工作的计时(要结束程序请输入q)：";
				
			}
		}
		if(currentState == RECORDING)
		{
			cin>>controlFlag;
			if(controlFlag=='e')
			{
				currentState=NORECORD;
				Itime currentTime = GetCurrentTime();
				lastRecordTime = currentTime;
				(*RecordArray.back()).SetEndTime(currentTime);
				
				//todo:将该条记录写到文件中去
				fopen_s(&file,filename.c_str(),"a");
				(*RecordArray.back()).SaveToFile(file);
				fclose(file);
				//算了， 最后再统一写到文件中去吧
			}
			else if(controlFlag == 'q')
			{
				work = false;
				currentState=NORECORD;
				Itime currentTime = GetCurrentTime();
				lastRecordTime = currentTime;
				(*RecordArray.back()).SetEndTime(currentTime);
				
				//todo:将该条记录写到文件中去
				fopen_s(&file,filename.c_str(),"a");
				(*RecordArray.back()).SaveToFile(file);
				fclose(file);
			}
			else
			{
				cout<<"illeagle input:"<<controlFlag<<"\t"<<"计时中，请输入 e 来结束当前工作的计时：";
			}
		}
	}
	cout<<"保存当前记录，程序正在关闭..."<<endl;
	//等待两秒，关闭程序
	Itime t1= GetCurrentTime();
	while(1)
	{
		Itime t2=GetCurrentTime();
		if((t2-t1).ItimeToSecond()>2)
			break;
	}
}