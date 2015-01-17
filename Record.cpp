#include"Record.h"
extern string filename;


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
	GetTypeFromJobStr();
}
void Record::SaveToFile(const string& filename,const string& openType)
{
	string timeStr = GetTimeString();
	string timeLenStr = (endTime-startTime).ItimeLenToString();
	
	string result;
	if(m_workType != SUNDRY)
	{
		result = timeStr+"——("+timeLenStr+")——"+jobDescription+"\n";
	}
	else
	{
		result = timeStr+"——("+timeLenStr+")——"+jobDescription+"\n";
	}
	FILE* file;
	fopen_s(&file,filename.c_str(),openType.c_str());
	fprintf(file,result.c_str());
	fclose(file);
	//todo:开始时间，结束时间也要写到文件中去；工作耗时也要写到文件中去（可在record中新添加一个字段）；记录之后换行
}

string Record::GetTimeString()
{
	string StartTimeStr = startTime.ItimeToString();
	string endTimeStr = endTime.ItimeToString();

	return string(StartTimeStr+string("~")+endTimeStr);

}
void Record::GetTypeFromJobStr()
{
	string typeStr;
	int idxStart;
	int idxEnd;

	idxStart = jobDescription.find("【")+2;
	idxEnd = jobDescription.find("】")-1;
	if(idxEnd>=idxStart)
	{
		typeStr = jobDescription.substr(idxStart,idxEnd-idxStart+1);
		if(typeStr.compare("L") == 0)
		{
			m_workType = LEARN;
		}
		else if(typeStr.compare("W") == 0)
		{
			m_workType = WRITE;
		}
		else if(typeStr.compare("R") == 0)
		{
			m_workType = READ;
		}
		else if(typeStr.compare("T") == 0)
		{
			m_workType = TRAIN;
		}
		else if(typeStr.compare("P") == 0)
		{
			m_workType = ENTERTAINMENT;
		}
		else if(typeStr.compare("WK") == 0)
		{
			m_workType = WORK;
		}
		else if(typeStr.compare("C") == 0)
		{
			m_workType = CODING;
		}
		else if(typeStr.compare("B") == 0)
		{
			m_workType = WASTETIME;
		}
		else//如果输入除了以上的字符串，则视为杂项
		{
			m_workType = SUNDRY;
			
		}
	}
	else//用户没有输入【】部分，默认为杂项
	{
		/*string typeStr = "Z";*/
		m_workType = SUNDRY;
		jobDescription+="【Z】";
	}
	

	
}
workType Record::GetWorkType()
{
	return m_workType;
}
Itime Record::GetJobTimeLength()
{
	return endTime-startTime;
}

ostream& operator<<(ostream& os,const Record& record)
{
	//TODO:重载Itime类的<<操作符。。
	os<<record.startTime<<"~"<<record.endTime<<endl;
	os<<"工作："<<record.jobDescription<<endl;
	return os;
}

