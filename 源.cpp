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
Itime GetCurrentTime()//�ŵ�time.h��ȥ
{
	time_t now;
	time(&now);
	struct tm* t = new tm();
	localtime_s(t,&now);
	return Itime(t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}

//void PrintStastisticToFile(string filename);//����ͳ������
//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//���Ǹ�����Ϊ��λ��ʱ�䣬�������ʱ�䲻��¼�ͻ��Զ�����һ��Btime��¼

//start 
//TODO:�½�һ���࣬����Щ�����Լ���غ�����װ��������ȥ
				//���ཨ��record����
				/*vector<Record> learnArray;
				vector<Record> writeArray;
				vector<Record> readArray;
				vector<Record> badArray;
				vector<Record> workArray;
				vector<Record> trainArray;
				vector<Record> playArray;
				vector<Record> sundryArray;
				vector<Record> codeArray;*/

				//���ౣ����ʱ��
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


	filename = "D:\\�ĵ�\\ʱ���\\";//todo:���ļ����Ƹ�Ϊ���������
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
				Record *BTimeRecord = new Record(lastRecordTime,currentTime,"���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ��B��");
				BTimeRecord->SetJob("���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ��B��");
				lastRecordTime = currentTime;
					
				BTimeRecord->SaveToFile(filename,"a");

				RecordArray.push_back(BTimeRecord);
					
			}
				
			Record *newRecord = new Record();
			RecordArray.push_back(newRecord);
			newRecord->SetStartTime(currentTime);
			newRecord->SetJob(job);

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
			if(controlFlag=='e')
			{
				currentState=NORECORD;
				Itime currentTime = GetCurrentTime();
				lastRecordTime = currentTime;
				(*RecordArray.back()).SetEndTime(currentTime);
				
				//todo:��������¼д���ļ���ȥ
				
				(*RecordArray.back()).SaveToFile(filename,"a");
				
				//���ˣ� �����ͳһд���ļ���ȥ��
			}
			else if(controlFlag == 'q')
			{
				work = false;
				currentState=NORECORD;
				Itime currentTime = GetCurrentTime();
				lastRecordTime = currentTime;
				(*RecordArray.back()).SetEndTime(currentTime);
				
				//todo:��������¼д���ļ���ȥ
				
				(*RecordArray.back()).SaveToFile(filename,"a");
				
			}
			//test filereader
			else if(controlFlag == 'r')
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
			else if(controlFlag == 't')//ͳ�ƣ������д�뵽����ͳ���ļ���ȥ
			{

				RecordFileReader reader(filename);
			
				vector<Record> todayRecordArray = reader.GetRecordArray();
				////���ཨ��record����
				//vector<Record> learnArray;
				//vector<Record> writeArray;
				//vector<Record> readArray;
				//vector<Record> badArray;
				//vector<Record> workArray;
				//vector<Record> trainArray;
				//vector<Record> playArray;
				//vector<Record> sundryArray;
				//vector<Record> codeArray;

				////���ౣ����ʱ��
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
				//	//TODO������ͳ�����м�¼�������ͳ���ļ���
				//	switch((*recordIter).GetWorkType())
				//	{
				//	case LEARN:
				//		learnArray.push_back(*recordIter);
				//		learnTime += (*recordIter).GetJobTimeLength();//TODO:����Itime��+=�����
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

				//������д�뵽�ļ���
				string stFilename = "D:\\�ĵ�\\ʱ���\\ͳ��\\";
				stFilename += lastRecordTime.ItimeToFileString();
				/*PrintStastisticToFile(stFilename);*/
				RecStatter.PrintResult(stFilename);
			}
			else
			{
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

//void PrintStastisticToFile(string filename)
//{
//	//start of output
//	//���������ʱ��
//	FILE* file=NULL;
//	
//
//	/*cout<<"����ѧϰ��ʱ��Ϊ"<<learnTime<<'\t'<<"����Ϊѧϰ��б�"<<endl;*/
//	vector<Record>::iterator iter;
//
//	bool first = true;
//	for(iter = learnArray.begin();iter!=learnArray.end();++iter)
//	{
//		cout<<(*iter);
//		//TODO:д���ļ���
//		if(first)//��ԭ�������������������ͳ������
//		{
//			fopen_s(&file,filename.c_str(),"w");
//			fprintf_s(file,"��������ѧϰ��ʱ��Ϊ%s,����Ϊѧϰ�б�\n",learnTime.ItimeLenToString().c_str());
//			fclose(file);
//			first = false;
//		}
//		//TODO:�޸�SaveToFile��ʹ֮����filename��Ϊ����
//		(*iter).SaveToFile(filename,"a");
//	}
//
//	//TODO:�� һ�๤��д�뵽ͳ���ļ��� ����߼� �������Ϊһ�����������ܹ�������Ϊ���룬�Զ�ִ�ж�Ӧ������ͳ�ơ�
//
//
//	//end of output
//	fclose(file);
//}