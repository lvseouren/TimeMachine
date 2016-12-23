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
void Record::SaveToFile(const string& filename,const string& openType,saveType savetype)
{
	//Change timeStr,remove the startTime part,cuz it has output to the file already
	//string timeStr = GetTimeString();
	string timeStr = "~" + endTime.ItimeToString();
	if(savetype == BtSave)
	{
		timeStr = startTime.ItimeToString()+"~"+endTime.ItimeToString();
	}

	string timeLenStr = (endTime-startTime).ItimeLenToString();
	
	string result;
	
	result = timeStr+"——("+timeLenStr+")——"+jobDescription;
	//only save record ,the【】 will be output
	if(savetype == recordSave||savetype == BtSave)
	{
		result += workStr;
	}
	result += "\n";

	if(savetype == statTenSave || savetype == statSave)
	{
		timeLenStr = (endTime-startTime).ItimeLenToStatString();
		result = jobDescription + "——("+timeLenStr+")\n";
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
			workStr = "【L】";
		}
		else if(typeStr.compare("W") == 0)
		{
			m_workType = WRITE;
			workStr = "【W】";
		}
		else if(typeStr.compare("R") == 0)
		{
			m_workType = READ;
			workStr = "【R】";
		}
		else if(typeStr.compare("T") == 0)
		{
			m_workType = TRAIN;
			workStr = "【T】";
		}
		else if(typeStr.compare("P") == 0)
		{
			m_workType = ENTERTAINMENT;
			workStr = "【P】";
		}
		else if(typeStr.compare("WK") == 0)
		{
			m_workType = WORK;
			workStr = "【WK】";
		}
		else if(typeStr.compare("C") == 0)
		{
			m_workType = CODING;
			workStr = "【C】";
		}
		else if(typeStr.compare("B") == 0)
		{
			m_workType = WASTETIME;
			workStr = "【B】";
		}
		else//如果输入除了以上的字符串，则视为杂项
		{
			m_workType = SUNDRY;
			workStr = "【Z】";	
		}
		//TODO:去除jobDescription中的【】
		int idxThrowStart = idxStart-3;
		int idxThrowEnd = idxEnd+3;
		int jobStrEnd = jobDescription.length()-1;
		string leftJobStr;
		string rightJobStr; 
		if(idxThrowEnd <= jobStrEnd)//如果【】后面还有字符，则加到jobDescription中去。
		{
			rightJobStr = jobDescription.substr(idxThrowEnd,jobStrEnd-idxThrowEnd+ 1);
		}
		leftJobStr = jobDescription.substr(0,idxThrowStart+1);//在这之前如果【】后还有内容，则得先取出来，否则这条语句过后那部分内容就没了
		
		jobDescription = leftJobStr + rightJobStr;
	}
	else//用户没有输入【】部分，默认为杂项
	{
		/*string typeStr = "Z";*/
		m_workType = SUNDRY;
		workStr = "【Z】";
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

//merge!branch2 into branch1!
//branch_test2:modify2


//branch_test_1:modify2

