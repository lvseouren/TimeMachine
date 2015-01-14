//start lib 
#include<iostream>
#include<string>
#include<vector>
#include<stdio.h>
#include<time.h>
#include"RecordFileReader.h"

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
//end function


//start global variable
string filename;
Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//���Ǹ�����Ϊ��λ��ʱ�䣬�������ʱ�䲻��¼�ͻ��Զ�����һ��Btime��¼
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
	FILE* file = NULL;

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
			if(1)
			{
				Itime currentTime = GetCurrentTime();
				if((currentTime-lastRecordTime).ItimeToSecond() > BATTIMESWITCH)
				{
					//todo:Bʱ���Զ���¼
					Record *BTimeRecord = new Record(lastRecordTime,currentTime,"���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ");
					lastRecordTime = currentTime;
					
					BTimeRecord->SaveToFile(file,"a");

					RecordArray.push_back(BTimeRecord);
					
				}
				
				Record *newRecord = new Record();
				RecordArray.push_back(newRecord);
				newRecord->SetStartTime(currentTime);
				newRecord->SetJob(job);
				currentState = RECORDING;
				cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";
				
			}
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
				
				(*RecordArray.back()).SaveToFile(file,"a");
				
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
				
				(*RecordArray.back()).SaveToFile(file,"a");
				
			}
			//test filereader
			else if(controlFlag == 'r')
			{
				cout<<"��ʼ�����ļ���ȡ����,����Ϊ��ȡ�ļ�¼��"<<endl;
				/*fopen_s(&file,filename.c_str(),"r");*/
				RecordFileReader reader(filename);
			
				vector<Record> test = reader.GetRecordArray();
				/*fclose(file);*/
				vector<Record>::iterator recordIter;
				int count=1;
				for(recordIter = test.begin();recordIter!=test.end();++recordIter)
				{
					//TODO������record���<<������
					cout<<"���"<<count++<<"����"<<*recordIter;
				}

				cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";

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