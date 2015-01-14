//TODO:重构main函数，使用extract method方法。
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



//start function

//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//这是个以秒为单位的时间，超过这个时间不记录就会自动生成一条Btime记录

//end global variable

void main()
{
	currentState=NORECORD;
	lastRecordTime = GetCurrentTime();
	cout<<"welcome"<<endl;
	/*string job;*/
	char* job = new char[1024];
	char controlFlag;
	vector<Record*> RecordArray;


	filename = "D:\\文档\\时光机\\";//todo:将文件名称改为当天的日期
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
				Record *BTimeRecord = new Record(lastRecordTime,currentTime,"这段时间啥事没做，注意了，你在浪费时间哦【B】");
				BTimeRecord->SetJob("这段时间啥事没做，注意了，你在浪费时间哦【B】");
				lastRecordTime = currentTime;
					
				BTimeRecord->SaveToFile(filename,"a");

				RecordArray.push_back(BTimeRecord);
					
			}
				
			Record *newRecord = new Record();
			RecordArray.push_back(newRecord);
			newRecord->SetStartTime(currentTime);
			newRecord->SetJob(job);

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
			if(controlFlag=='e')
			{
				currentState=NORECORD;
				Itime currentTime = GetCurrentTime();
				lastRecordTime = currentTime;
				(*RecordArray.back()).SetEndTime(currentTime);
				
				//todo:将该条记录写到文件中去
				
				(*RecordArray.back()).SaveToFile(filename,"a");
				
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
				
				(*RecordArray.back()).SaveToFile(filename,"a");
				
			}
			//test filereader
			else if(controlFlag == 'r')
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
			else if(controlFlag == 't')//统计，将结果写入到当日统计文件中去
			{
				//TODO:remove this
				/*string tempFile = "D:\\文档\\时光机\\2015年1月13日.txt";*/
				RecordFileReader reader(filename);
			
				vector<Record> todayRecordArray = reader.GetRecordArray();

				RecordStatistic RecStatter;
				RecStatter.makeStatistic(todayRecordArray);
				

				//将数据写入到文件中
				//TODO: 统计哪天的数据就保存到哪天的统计文件中去
				string stFilename = "D:\\文档\\时光机\\统计\\";
				stFilename += lastRecordTime.ItimeToFileString();
				RecStatter.PrintResult(stFilename);
			}
			else
			{
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
