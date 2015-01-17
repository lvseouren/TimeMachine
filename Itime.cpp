#include"Itime.h"
extern string filename;

Itime::Itime(int y,int m,int d,int hour,int min,int sec):year(y),month(m),day(d),hour(hour),minute(min),second(sec)
{
}
Itime::Itime()
{}
Itime Itime::operator-(const Itime& rhs)
{
	int year_differ = year-rhs.year;
	int mon_differ = month-rhs.month;
	int day_differ = (day-rhs.day);
	int hour_differ = (hour-rhs.hour);
	int min_differ = (minute-rhs.minute);
	int sec_differ = second-rhs.second;
	//TODO:修正负数值
	if(sec_differ<0)
	{
		sec_differ+=60;
		--min_differ;
	}
	if(min_differ<0)
	{
		min_differ+=60;
		--hour_differ;
	}
	if(hour_differ<0)
	{
		hour_differ+=24;
		--day_differ;
	}
	//TODO:day，month和year的负数修正并未考虑，将来需要对此进行修正
	if(day_differ<0)
	{
		day_differ = 0;
	}
	return Itime(year_differ,mon_differ,day_differ,hour_differ,min_differ,sec_differ);
}
int Itime::ItimeToSecond()
{
	bool yun_year = false;
	if(year%400==0)
		yun_year = true;
	else if(year%4==0 && year%100!=0)
		yun_year = true;
	//todo:年和月的秒数转换得分情况讨论，暂不考虑
	//int year_sec = yun_year?31622400*year:31536000*year;
	//int mon_sec = 
	int day_sec = day*86400;
	int hour_sec = hour*3600;
	int min_sec = minute*60;
	int result = day_sec+hour_sec+min_sec+second;
	return result;
}
string Itime::ItimeToString()
{
	string result;

	char TimeSecStr[128];
	char TimeMinStr[128];
	char TimeHourStr[128];
	sprintf_s(TimeSecStr,"%d",second);
	sprintf_s(TimeMinStr,"%d",minute);
	sprintf_s(TimeHourStr,"%d",hour);

	result = TimeHourStr+string(":")+TimeMinStr+string(":")+TimeSecStr;
	return result;
}

string Itime::ItimeLenToString()
{
	//修正时间
	if(hour<0)
	{
		hour+=24;
		//说明跨天了，将记录写到新的文件中——it doesn't work any more ,because operator- has fix the hour so hour is always >= 0;
		/*Itime curTime = GetCurrentTime();
		filename = "D:\\文档\\时光机\\"+ curTime.ItimeToFileString();*/
	}
	if(minute<0)
	{
		--hour;
		minute+=60;
	}
	if(second<0)
	{
		--minute;
		second+=60;
	}

	string result;

	char TimeSecStr[128];
	char TimeMinStr[128];
	char TimeHourStr[128];
	sprintf_s(TimeSecStr,"%d",second);
	sprintf_s(TimeMinStr,"%d",minute);
	sprintf_s(TimeHourStr,"%d",hour);

	result =string("共计") + TimeHourStr+string("h")+TimeMinStr+string("m")+TimeSecStr+string("s");
	return result;
}

string Itime::ItimeToFileString()
{
	string result;

	char TimeYearStr[128];
	char TimeMonthStr[128];
	char TimeDayStr[128];
	sprintf_s(TimeYearStr,"%d",year);
	sprintf_s(TimeMonthStr,"%d",month);
	sprintf_s(TimeDayStr,"%d",day);

	result = TimeYearStr+string("年")+TimeMonthStr+string("月")+TimeDayStr+string("日.txt");
	return result;
}


ostream& operator<<(ostream& os,Itime time)
{
	os<<time.hour<<":"<<time.minute<<":"<<time.second;
	return os;
}
Itime Itime::operator+=(const Itime& rhs)
{
	int h = hour+rhs.hour;
	int m = minute+rhs.minute;
	int s = second+rhs.second;
	if(s>=60)
	{
		s-=60;
		++m;
	}
	if(m>=60)
	{
		m-=60;
		++h;
	}
	return Itime(year+=rhs.year,month+=rhs.month,day+=rhs.day,hour=h,minute=m,second=s);
}

bool Itime::operator<(const Itime& rhs)
{
	if(day<rhs.day)
		return true;
	else if(hour<rhs.hour)
		return true;
	else if(minute<rhs.minute)
		return true;
	else if(second<rhs.second)
		return true;
	else
		return false;
}
Itime StrConvertToItime(const string&timeStr)
{
	int startHour;
	int endHour;
	int startMin;
	int endMin;
	int startSec;
	int endSec;

	//以下算法的前提是,':'只占用一个字符的长度
	string test = ":";
	int lengh = test.length();

	startHour = 0;
	endHour = timeStr.find(':');

	startMin = endHour+1;
	endMin = timeStr.rfind(':')-1;

	startSec = endMin+2;
	endSec = timeStr.length()-1;

	string hourStr = timeStr.substr(startHour,endHour);
	string minStr = timeStr.substr(startMin,endMin);
	string secStr = timeStr.substr(startSec,endSec);

	int hour = atoi(hourStr.c_str());
	int min = atoi(minStr.c_str());
	int sec = atoi(secStr.c_str());

	return Itime(0,0,0,hour,min,sec);
}

Itime GetCurrentTime()//放到time.h中去
{
	time_t now;
	time(&now);
	struct tm* t = new tm();
	localtime_s(t,&now);
	return Itime(t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}
