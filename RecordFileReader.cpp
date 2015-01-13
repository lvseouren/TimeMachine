#include"RecordFileReader.h"

RecordFileReader::RecordFileReader()
{
}
RecordFileReader::RecordFileReader(FILE* pf):file(pf)
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

void RecordFileReader::SetFileToRead(FILE* pf)
{
	file = pf;
}

void RecordFileReader::Read()
{
	cout<<"��r��ʽ�򿪣��ļ�ָ��λ�ã�"<<ftell(file)<<endl;
	char* tempCharArray = new char[1024];
	string recordStr;//�����и����⣬һ������ܳ���1024���ֽڣ�����ͻ��bug������ʱ����û���⣬��Ϊһ�м�¼�����ܻᳬ��1024���ֽ�
	//PS:������ܻ�����Ҳ�ȷ���Ƿ����ֻ��һ����Ч��recordStr����Ϊ��ֻ�õ���һ��char*��Ҳ�����ǿ����������ÿ�ζ�����������ֵ����ֻϣ��recordStrArray�����ֵ�����ǵ�ַ��
	while((fgets(tempCharArray,1024,file))!=NULL)
	{
		recordStr = tempCharArray;
		recordStrArray.push_back(recordStr);
	}

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
		RawParse(*itr,startTime,endTime,jobDescription);

		newRecord->SetStartTime(startTime);
		newRecord->SetEndTime(endTime);
		newRecord->SetJob(jobDescription);
		recordArray.push_back(*newRecord);
	}
}

void RecordFileReader::RawParse(const string& recordStr,Itime& startTime,Itime& endTime,string& jobDescription)
{
	//TODO:ʵ���ַ����н�ȡ�������ݵ��㷨����������һ������ȡ��Ӧ���ַ������ڶ������ַ�����ȡ��int�����ݣ�
	//�����Ȳ���һ��read�Ĺ��ܣ��������������ⷵ��һЩ����
	startTime = Itime(0,0,0,0,0,0);
	endTime = startTime;
	jobDescription = "test";
}
