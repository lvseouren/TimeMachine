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
	//if(m_workType != SUNDRY)
	result = timeStr+"����("+timeLenStr+")����"+jobDescription+workStr+"\n";
	//else
	//{
	//	result = timeStr+"����("+timeLenStr+")����"+jobDescription+"\n";
	//}
	FILE* file;
	fopen_s(&file,filename.c_str(),openType.c_str());
	fprintf(file,result.c_str());
	fclose(file);
	//todo:��ʼʱ�䣬����ʱ��ҲҪд���ļ���ȥ��������ʱҲҪд���ļ���ȥ������record�������һ���ֶΣ�����¼֮����
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

	idxStart = jobDescription.find("��")+2;
	idxEnd = jobDescription.find("��")-1;
	if(idxEnd>=idxStart)
	{
		typeStr = jobDescription.substr(idxStart,idxEnd-idxStart+1);
		if(typeStr.compare("L") == 0)
		{
			m_workType = LEARN;
			workStr = "��L��";
		}
		else if(typeStr.compare("W") == 0)
		{
			m_workType = WRITE;
			workStr = "��W��";
		}
		else if(typeStr.compare("R") == 0)
		{
			m_workType = READ;
			workStr = "��R��";
		}
		else if(typeStr.compare("T") == 0)
		{
			m_workType = TRAIN;
			workStr = "��T��";
		}
		else if(typeStr.compare("P") == 0)
		{
			m_workType = ENTERTAINMENT;
			workStr = "��P��";
		}
		else if(typeStr.compare("WK") == 0)
		{
			m_workType = WORK;
			workStr = "��WK��";
		}
		else if(typeStr.compare("C") == 0)
		{
			m_workType = CODING;
			workStr = "��C��";
		}
		else if(typeStr.compare("B") == 0)
		{
			m_workType = WASTETIME;
			workStr = "��B��";
		}
		else//�������������ϵ��ַ���������Ϊ����
		{
			m_workType = SUNDRY;
			workStr = "��Z��";	
		}
		//TODO:ȥ��jobDescription�еġ���
		int idxThrowStart = idxStart-3;
		int idxThrowEnd = idxEnd+3;
		int jobStrEnd = jobDescription.length()-1;
		string leftJobStr;
		string rightJobStr; 
		if(idxThrowEnd <= jobStrEnd)//����������滹���ַ�����ӵ�jobDescription��ȥ��
		{
			rightJobStr = jobDescription.substr(idxThrowEnd,jobStrEnd-idxThrowEnd+ 1);
		}
		leftJobStr = jobDescription.substr(0,idxThrowStart+1);//����֮ǰ��������������ݣ������ȡ���������������������ǲ������ݾ�û��
		
		jobDescription = leftJobStr + rightJobStr;
	}
	else//�û�û�����롾�����֣�Ĭ��Ϊ����
	{
		/*string typeStr = "Z";*/
		m_workType = SUNDRY;
		workStr = "��Z��";
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
	//TODO:����Itime���<<����������
	os<<record.startTime<<"~"<<record.endTime<<endl;
	os<<"������"<<record.jobDescription<<endl;
	return os;
}

