#include"RecordFileReader.h"



RecordFileReader::RecordFileReader()
{
}
//RecordFileReader::RecordFileReader(FILE* pf):file(pf)
//{
//}
RecordFileReader::RecordFileReader(const string& FileName):filename(FileName)
{
}
RecordFileReader::~RecordFileReader()
{

}

vector<Record> RecordFileReader::GetRecordArray()	
{
	Read();
	Parse();
	return recordArray;
}

void RecordFileReader::SetFileToRead(const string& fileName)
{
	filename = fileName;
}

void RecordFileReader::Read()
{
	FILE* file;
	fopen_s(&file,filename.c_str(),"r"); 

	/*cout<<"��r��ʽ�򿪣��ļ�ָ��λ�ã�"<<ftell(file)<<endl;*/
	char* tempCharArray = new char[1024];
	string recordStr;//�����и����⣬һ������ܳ���1024���ֽڣ�����ͻ��bug������ʱ����û���⣬��Ϊһ�м�¼�����ܻᳬ��1024���ֽ�
	//PS:������ܻ�����Ҳ�ȷ���Ƿ����ֻ��һ����Ч��recordStr����Ϊ��ֻ�õ���һ��char*��Ҳ�����ǿ����������ÿ�ζ�����������ֵ����ֻϣ��recordStrArray�����ֵ�����ǵ�ַ��
	while((fgets(tempCharArray,1024,file))!=NULL)
	{
		recordStr = tempCharArray;
		recordStrArray.push_back(recordStr);
	}
	fclose(file);
}

void RecordFileReader::Parse()
{
	vector<string>::iterator itr;
	Record *newRecord = new Record;
	for(itr = recordStrArray.begin();itr!=recordStrArray.end();++itr)
	{
		//TODO:����recordStrArray�е��ַ�����һ��һ������Record
		//Itime startTime = GetStartTimeFromRecordStr(*itr);
		//Itime endTime = GetEndTimeFromRecordStr(*itr);
		//string jobDescription = GetJobFromRecordStr(*itr);
		Itime startTime;
		Itime endTime;
		string jobDescription;
		//����е���Ϣ����ȫ�����������¼
		bool result = RawParse(*itr,startTime,endTime,jobDescription);
		if(result)
		{
			newRecord->SetStartTime(startTime);
			newRecord->SetEndTime(endTime);
			newRecord->SetJob(jobDescription);
			recordArray.push_back(*newRecord);
		}
	}
}

bool RecordFileReader::RawParse(const string& recordStr,Itime& startTime,Itime& endTime,string& jobDescription)
{
	//TODO:ʵ���ַ����н�ȡ�������ݵ��㷨����������һ������ȡ��Ӧ���ַ������ڶ������ַ�����ȡ��int�����ݣ�

	string startTimeStr;
	string endTimeStr;
	
	int indexBeginStartTimeStr;//��ʼʱ���ַ����ĵ�һ���ַ�������λ��
	int indexEndStartTimeStr;//��ʼʱ���ַ��������һ���ַ�������λ�ã���'~'���ŵ���ʼλ�õ�ǰһ��λ�ã����~����Ϊһ���ַ���

	int indexBeginEndTimeStr;
	int indexEndEndTimeStr;

	int indexBeginJobStr;
	int indexEndJobStr;
	
	indexBeginStartTimeStr = 0;
	indexEndStartTimeStr = recordStr.find('~',0);

	//test lenth of ��������
	string temp = "����";
	int length = temp.length();
	//TODO:���ı��в����ڡ�������ʱ��������׳��쳣
	indexBeginEndTimeStr = indexEndStartTimeStr+1;
	indexEndEndTimeStr = recordStr.find("����")-1;

	//����record�в����ڡ�������ʱ��������ʱ�����
	if(-1 != recordStr.rfind("����"))
	{
		indexBeginJobStr = recordStr.rfind("����")+length;
		indexEndJobStr = recordStr.length()-2;
	}
	else
	{
		//��Ϣ����ȫ�����������¼
		return false;
	}

	startTimeStr = recordStr.substr(indexBeginStartTimeStr,indexEndStartTimeStr);
	endTimeStr = recordStr.substr(indexBeginEndTimeStr,indexEndEndTimeStr-indexBeginEndTimeStr+1);
	jobDescription = recordStr.substr(indexBeginJobStr,indexEndJobStr-indexBeginJobStr+1);

	//TODO:�����һ��������������ʱ���ַ���������Itime����
	startTime = StrConvertToItime(startTimeStr);
	endTime = StrConvertToItime(endTimeStr);

	return true;
}

