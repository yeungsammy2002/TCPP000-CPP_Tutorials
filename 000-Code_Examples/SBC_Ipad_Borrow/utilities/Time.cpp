#include "Time.h"

using namespace regulus::utilities;

const char * Time::s_weekday[7]{
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
};

Time Time::now()
{
	std::time_t ticks = std::time(NULL);
	long long time = ticks;
	return get_time(time);
}

Time Time::get_time(long long time)
{
	Time t;
	std::time_t ticks = time;

	t.m_unixtime = ticks;

	struct tm * ptm = std::localtime(&ticks);

	t.m_year = 1900 + ptm->tm_year;
	t.m_month = 1 + ptm->tm_mon;
	t.m_day = ptm->tm_mday;

	t.m_hour = ptm->tm_hour;
	t.m_minute = ptm->tm_min;
	t.m_second = ptm->tm_sec;

	t.str_weekday = s_weekday[ptm->tm_wday];

	char hr[3]{ 0 };
	strftime(hr, sizeof(hr), "%H", ptm);
	hr[2] = 0;
	t.str_hour = hr;

	char min[3]{ 0 };
	strftime(min, sizeof(min), "%M", ptm);
	min[2] = 0;
	t.str_minute = min;

	char sec[3]{ 0 };
	strftime(sec, sizeof(sec), "%S", ptm);
	sec[2] = 0;
	t.str_second = sec;

	return t;
}