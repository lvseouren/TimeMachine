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

Itime lastRecordTime;
STATE currentState;
const int BATTIMESWITCH = 600;//���Ǹ�����Ϊ��λ��ʱ�䣬�������ʱ�䲻��¼�ͻ��Զ�����һ��Btime��¼
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

	string filename = "D:\\�ĵ�\\ʱ���\\";//todo:���ļ����Ƹ�Ϊ���������
	filename = filename+lastRecordTime.ItimeToFileString();
	

	bool work = true;
	while(work)
	{
		if(currentState==NORECORD)
		{
			cout<<"�����뽫Ҫ���еĹ�����";
			cin.clear();
			cin.sync();//��ֹ����"ee"ʱ����
			cin>>job;
			if(job.c_str())
			{
				Itime currentTime = GetCurrentTime();
				if((currentTime-lastRecordTime).ItimeToSecond() > BATTIMESWITCH)
				{
					//todo:Bʱ���Զ���¼
					Record *BTimeRecord = new Record(lastRecordTime,currentTime,"���ʱ��ɶ��û����ע���ˣ������˷�ʱ��Ŷ");
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
				cout<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ(Ҫ��������������q)��";
				
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
				
				//todo:��������¼д���ļ���ȥ
				fopen_s(&file,filename.c_str(),"a");
				(*RecordArray.back()).SaveToFile(file);
				fclose(file);
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
				fopen_s(&file,filename.c_str(),"a");
				(*RecordArray.back()).SaveToFile(file);
				fclose(file);
			}
			else
			{
				cout<<"illeagle input:"<<controlFlag<<"\t"<<"��ʱ�У������� e ��������ǰ�����ļ�ʱ��";
			}
		}
	}
	cout<<"���浱ǰ��¼���������ڹر�..."<<endl;
	//�ȴ����룬�رճ���
	Itime t1= GetCurrentTime();
	while(1)
	{
		Itime t2=GetCurrentTime();
		if((t2-t1).ItimeToSecond()>2)
			break;
	}
}