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

	/*cout<<"以r方式打开，文件指针位置："<<ftell(file)<<endl;*/
	char* tempCharArray = new char[1024];
	string recordStr;//这里有个问题，一行最大不能超过1024个字节，否则就会出bug，但暂时这样没问题，因为一行记录不可能会超过1024个字节
	//PS:这里可能会出错，我不确定是否最后只有一个有效的recordStr，因为我只用到了一个char*，也就是那块区域的数据每次都被赋予了新值，我只希望recordStrArray存的是值而不是地址。
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
		//TODO:处理recordStrArray中的字符串，一对一地生成Record
		//Itime startTime = GetStartTimeFromRecordStr(*itr);
		//Itime endTime = GetEndTimeFromRecordStr(*itr);
		//string jobDescription = GetJobFromRecordStr(*itr);
		Itime startTime;
		Itime endTime;
		string jobDescription;
		//如果行的信息不完全，则丢弃这个记录
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
	//TODO:实现字符串中截取以上数据的算法（两步，第一步：截取对应的字符串，第二部：字符串中取得int型数据）

	string startTimeStr;
	string endTimeStr;
	
	int indexBeginStartTimeStr;//开始时间字符串的第一个字符的所在位置
	int indexEndStartTimeStr;//开始时间字符串的最后一个字符的所在位置，即'~'符号的起始位置的前一个位置（如果~长度为一个字符）

	int indexBeginEndTimeStr;
	int indexEndEndTimeStr;

	int indexBeginJobStr;
	int indexEndJobStr;
	
	indexBeginStartTimeStr = 0;
	indexEndStartTimeStr = recordStr.find('~',0);

	//test lenth of “——”
	string temp = "——";
	int length = temp.length();
	//TODO:当文本中不存在“——”时，程序会抛出异常
	indexBeginEndTimeStr = indexEndStartTimeStr+1;
	indexEndEndTimeStr = recordStr.find("——")-1;

	//处理当record中不存在“——”时，即空行时的情况
	if(-1 != recordStr.rfind("——"))
	{
		indexBeginJobStr = recordStr.rfind("——")+length;
		indexEndJobStr = recordStr.length()-2;
	}
	else
	{
		//信息不完全，丢弃这个记录
		return false;
	}

	startTimeStr = recordStr.substr(indexBeginStartTimeStr,indexEndStartTimeStr);
	endTimeStr = recordStr.substr(indexBeginEndTimeStr,indexEndEndTimeStr-indexBeginEndTimeStr+1);
	jobDescription = recordStr.substr(indexBeginJobStr,indexEndJobStr-indexBeginJobStr+1);

	//TODO:在添加一个函数——输入时间字符串，返回Itime对象
	startTime = StrConvertToItime(startTimeStr);
	endTime = StrConvertToItime(endTimeStr);

	return true;
}

