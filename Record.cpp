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

	string result = timeStr+"——("+timeLenStr+")——"+jobDescription+"\n";

	fprintf(file,result.c_str());
	//todo:开始时间，结束时间也要写到文件中去；工作耗时也要写到文件中去（可在record中新添加一个字段）；记录之后换行
}

string Record::GetTimeString()
{
	string StartTimeStr = startTime.ItimeToString();
	string endTimeStr = endTime.ItimeToString();

	return string(StartTimeStr+string("~")+endTimeStr);

}

ostream& operator<<(ostream& os,const Record& record)
{
	//TODO:重载Itime类的<<操作符。。
	os<<record.startTime<<"~"<<record.endTime<<endl;
	os<<"任务："<<record.jobDescription<<endl;
	return os;
}