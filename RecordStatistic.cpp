#include"RecordStatistic.h"

//TODO:完善构造函数，初始化成员
RecordStatistic::RecordStatistic():learnTime(0,0,0,0,0,0),
				 writeTime(0,0,0,0,0,0),
				 readTime(0,0,0,0,0,0),
				 badTime(0,0,0,0,0,0),
				 workTime(0,0,0,0,0,0),
				 trainTime(0,0,0,0,0,0),
				 playTime(0,0,0,0,0,0),
				 sundryTime(0,0,0,0,0,0),
				 codeTime(0,0,0,0,0,0),
				 isFirstWrite(true)
{}
RecordStatistic::~RecordStatistic()
{}

void RecordStatistic::makeStatistic(vector<Record>& todayRecordArray)
{
	vector<Record>::iterator recordIter;
	for(recordIter = todayRecordArray.begin();recordIter!=todayRecordArray.end();++recordIter)
	{
		//TODO：分类统计所有记录，输出到统计文件中
		switch((*recordIter).GetWorkType())
		{
		case LEARN:
			learnArray.push_back(*recordIter);
			learnTime += (*recordIter).GetJobTimeLength();//TODO:重载Itime的+=运算符
			break;
		case WRITE:
			writeArray.push_back(*recordIter);
			writeTime += (*recordIter).GetJobTimeLength();
			break;
		case READ:
			readArray.push_back(*recordIter);
			readTime += (*recordIter).GetJobTimeLength();
			break;
		case WASTETIME:
			badArray.push_back(*recordIter);
			badTime += (*recordIter).GetJobTimeLength();
			break;
		case WORK:
			workArray.push_back(*recordIter);
			workTime += (*recordIter).GetJobTimeLength();
			break;
		case TRAIN:
			trainArray.push_back(*recordIter);
			trainTime += (*recordIter).GetJobTimeLength();
			break;
		case ENTERTAINMENT:
			playArray.push_back(*recordIter);
			playTime += (*recordIter).GetJobTimeLength();
			break;
		case SUNDRY:
			sundryArray.push_back(*recordIter);
			sundryTime += (*recordIter).GetJobTimeLength();
			break;
		case CODING:
			codeArray.push_back(*recordIter);
			codeTime += (*recordIter).GetJobTimeLength();
			break;
		default:
			break;
		}
	}
}

void RecordStatistic::PrintResult(const string& filename)
{
	typedef pair<vector<Record>,pair<string,Itime>> recordPair;
	vector<recordPair> vecRecStr;//TODO:push_back all vector<record> in here;
	recordPair temp = make_pair(learnArray,make_pair("【L】",learnTime));
	vecRecStr.push_back(temp);
	temp = make_pair(codeArray,make_pair("【C】",codeTime));
	vecRecStr.push_back(temp);
	temp = make_pair(writeArray,make_pair("【W】",writeTime));
	vecRecStr.push_back(temp);
	temp = make_pair(readArray,make_pair("【R】",readTime));
	vecRecStr.push_back(temp);
	temp = make_pair(trainArray,make_pair("【T】",trainTime));
	vecRecStr.push_back(temp);
	temp = make_pair(playArray,make_pair("【P】",playTime));
	vecRecStr.push_back(temp);
	temp = make_pair(sundryArray,make_pair("【Z】",sundryTime));
	vecRecStr.push_back(temp);
	temp = make_pair(workArray,make_pair("【WK】",workTime));
	vecRecStr.push_back(temp);
	temp = make_pair(badArray,make_pair("【Bad】",badTime));
	vecRecStr.push_back(temp);



	vector<recordPair>::iterator  iterVecRec;
	for(iterVecRec = vecRecStr.begin();iterVecRec!=vecRecStr.end();++iterVecRec)
	{
		PrintSingleTypeRecord(iterVecRec->first,iterVecRec->second.second,iterVecRec->second.first,filename);
	}
}

void RecordStatistic::PrintSingleTypeRecord(vector<Record>& vecRecord, Itime& totalTime,const string& workTypeStr,const string& filename)
{
	//start of output
	//输出杂项类时间
	FILE* file=NULL;
	
	vector<Record>::iterator iter;

	bool first = true;
	bool hasData = false;
	for(iter = vecRecord.begin();iter!=vecRecord.end();++iter)
	{
		//TODO:写到文件中
		hasData = true;
		if(first)//将原有数据清除，重新生成统计数据
		{
			string openType = "a";
			if(isFirstWrite)
			{
				openType = "w";
				isFirstWrite = false;
			}
			fopen_s(&file,filename.c_str(),openType.c_str());
			fprintf_s(file,"——今日%s总时间为%s,以下为%s列表：\n",workTypeStr.c_str(),totalTime.ItimeLenToString().c_str(),workTypeStr.c_str());
			fclose(file);
			first = false;
		}
		//TODO:修改SaveToFile，使之接受filename作为参数
		(*iter).SaveToFile(filename,"a");
	}

	//两类工作之间空一行
	//当这类工作有数据时才空，否则不空
	if(!hasData)
	{
		fopen_s(&file,filename.c_str(),"a");
		fprintf_s(file,"——今日%s总时间为0\n\n",workTypeStr.c_str());
		fclose(file);
	}
	else
	{
		fopen_s(&file,filename.c_str(),"a");
		fprintf_s(file,"\n",workTypeStr.c_str());
		fclose(file);
	}
}