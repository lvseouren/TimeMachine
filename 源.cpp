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
Itime GetCurrentTime()//放到time.h中去
{
	time_t now;
	time(&now);
	struct tm* t = new tm();
	localtime_s(t,&now);
	return Itime(t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}

//void PrintStastisticToFile(string filename);//输入统计数据
//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//这是个以秒为单位的时间，超过这个时间不记录就会自动生成一条Btime记录

//start 
//TODO:新建一个类，将这些数据以及相关函数封装到该类中去
				//分类建立record数组
				/*vector<Record> learnArray;
				vector<Record> writeArray;
				vector<Record> readArray;
				vector<Record> badArray;
				vector<Record> workArray;
				vector<Record> trainArray;
				vector<Record> playArray;
				vector<Record> sundryArray;
				vector<Record> codeArray;*/

				//分类保存总时间
				/*Itime learnTime(0,0,0,0,0,0);
				Itime writeTime(0,0,0,0,0,0);
				Itime readTime(0,0,0,0,0,0);
				Itime badTime(0,0,0,0,0,0);
				Itime workTime(0,0,0,0,0,0);
				Itime trainTime(0,0,0,0,0,0);
				Itime playTime(0,0,0,0,0,0);
				Itime sundryTime(0,0,0,0,0,0);
				Itime codeTime(0,0,0,0,0,0);*/
				//end
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

				RecordFileReader reader(filename);
			
				vector<Record> todayRecordArray = reader.GetRecordArray();
				////分类建立record数组
				//vector<Record> learnArray;
				//vector<Record> writeArray;
				//vector<Record> readArray;
				//vector<Record> badArray;
				//vector<Record> workArray;
				//vector<Record> trainArray;
				//vector<Record> playArray;
				//vector<Record> sundryArray;
				//vector<Record> codeArray;

				////分类保存总时间
				/*Itime learnTime(0,0,0,0,0,0);
				Itime writeTime(0,0,0,0,0,0);
				Itime readTime(0,0,0,0,0,0);
				Itime badTime(0,0,0,0,0,0);
				Itime workTime(0,0,0,0,0,0);
				Itime trainTime(0,0,0,0,0,0);
				Itime playTime(0,0,0,0,0,0);
				Itime sundryTime(0,0,0,0,0,0);
				Itime codeTime(0,0,0,0,0,0);*/

				RecordStatistic RecStatter;
				RecStatter.makeStatistic(todayRecordArray);
				//vector<Record>::iterator recordIter;
				//for(recordIter = todayRecordArray.begin();recordIter!=todayRecordArray.end();++recordIter)
				//{
				//	//TODO：分类统计所有记录，输出到统计文件中
				//	switch((*recordIter).GetWorkType())
				//	{
				//	case LEARN:
				//		learnArray.push_back(*recordIter);
				//		learnTime += (*recordIter).GetJobTimeLength();//TODO:重载Itime的+=运算符
				//		break;
				//	case WRITE:
				//		writeArray.push_back(*recordIter);
				//		writeTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case READ:
				//		readArray.push_back(*recordIter);
				//		readTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case WASTETIME:
				//		badArray.push_back(*recordIter);
				//		badTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case WORK:
				//		workArray.push_back(*recordIter);
				//		workTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case TRAIN:
				//		trainArray.push_back(*recordIter);
				//		trainTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case ENTERTAINMENT:
				//		playArray.push_back(*recordIter);
				//		playTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case SUNDRY:
				//		sundryArray.push_back(*recordIter);
				//		sundryTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	case CODING:
				//		codeArray.push_back(*recordIter);
				//		codeTime += (*recordIter).GetJobTimeLength();
				//		break;
				//	default:
				//		break;
				//	}
				//}

				//将数据写入到文件中
				string stFilename = "D:\\文档\\时光机\\统计\\";
				stFilename += lastRecordTime.ItimeToFileString();
				/*PrintStastisticToFile(stFilename);*/
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

//void PrintStastisticToFile(string filename)
//{
//	//start of output
//	//输出杂项类时间
//	FILE* file=NULL;
//	
//
//	/*cout<<"今日学习总时间为"<<learnTime<<'\t'<<"以下为学习活动列表："<<endl;*/
//	vector<Record>::iterator iter;
//
//	bool first = true;
//	for(iter = learnArray.begin();iter!=learnArray.end();++iter)
//	{
//		cout<<(*iter);
//		//TODO:写到文件中
//		if(first)//将原有数据清除，重新生成统计数据
//		{
//			fopen_s(&file,filename.c_str(),"w");
//			fprintf_s(file,"——今日学习总时间为%s,以下为学习列表：\n",learnTime.ItimeLenToString().c_str());
//			fclose(file);
//			first = false;
//		}
//		//TODO:修改SaveToFile，使之接受filename作为参数
//		(*iter).SaveToFile(filename,"a");
//	}
//
//	//TODO:将 一类工作写入到统计文件中 这个逻辑 抽出来作为一个函数，接受工作类型为输入，自动执行对应的类型统计。
//
//
//	//end of output
//	fclose(file);
//}