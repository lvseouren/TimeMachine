#include"Record.h"

Record::Record(Itime t1,Itime t2,string job):startTime(t1),endTime(t2),jobDescription(job)
{
	
}
Record::Record()
{}
void Record::SetStartTime(Itime t)
{
	startTime = t;
}
void Record::SetEndTime(Itime t)
{
	endTime = t;
}
void Record::SetJob(string job)
{
	jobDescription = job;
}
void Record::SaveToFile(FILE* file)
{
	string timeStr = GetTimeString();
	string timeLenStr = (endTime-startTime).ItimeLenToString();

	string result = timeStr+"����("+timeLenStr+")����"+jobDescription+"\n";

	fprintf(file,result.c_str());
	//todo:��ʼʱ�䣬����ʱ��ҲҪд���ļ���ȥ��������ʱҲҪд���ļ���ȥ������record�������һ���ֶΣ�����¼֮����
}

string Record::GetTimeString()
{
	string StartTimeStr = startTime.ItimeToString();
	string endTimeStr = endTime.ItimeToString();

	return string(StartTimeStr+string("~")+endTimeStr);

}

ostream& operator<<(ostream& os,const Record& record)
{
	//TODO:����Itime���<<����������
	os<<record.startTime<<"~"<<record.endTime<<endl;
	os<<"����"<<record.jobDescription<<endl;
	return os;
}