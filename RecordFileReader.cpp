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
	cout<<"以r方式打开，文件指针位置："<<ftell(file)<<endl;
	char* tempCharArray = new char[1024];
	string recordStr;//这里有个问题，一行最大不能超过1024个字节，否则就会出bug，但暂时这样没问题，因为一行记录不可能会超过1024个字节
	//PS:这里可能会出错，我不确定是否最后只有一个有效的recordStr，因为我只用到了一个char*，也就是那块区域的数据每次都被赋予了新值，我只希望recordStrArray存的是值而不是地址。
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
		//TODO:处理recordStrArray中的字符串，一对一地生成Record
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
	//TODO:实现字符串中截取以上数据的算法（两步，第一步：截取对应的字符串，第二部：字符串中取得int型数据）
	//现在先测试一下read的功能，所以这里先随意返回一些数据
	startTime = Itime(0,0,0,0,0,0);
	endTime = startTime;
	jobDescription = "test";
}
