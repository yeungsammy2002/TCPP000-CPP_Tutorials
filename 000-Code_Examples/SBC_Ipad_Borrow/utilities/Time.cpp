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

	if (12 <= t.m_hour)
	{
		t.m_12hr_format = "pm";
		t.m_chi_12hr_format = "下午";
	}
	else
	{
		t.m_12hr_format = "am";
		t.m_chi_12hr_format = "上午";
	}

	if (12 < t.m_hour)
	{
		t.m_12hr_hour = t.m_hour - 12;
	}
	else
	{
		t.m_12hr_hour = t.m_hour;
	}

	t.str_weekday = s_weekday[ptm->tm_wday];

	switch (ptm->tm_wday)
	{
	case 0:
		t.str_chi_weekday = "星期日";
		break;
	case 1:
		t.str_chi_weekday = "星期一";
		break;
	case 2:
		t.str_chi_weekday = "星期二";
		break;
	case 3:
		t.str_chi_weekday = "星期三";
		break;
	case 4:
		t.str_chi_weekday = "星期四";
		break;
	case 5:
		t.str_chi_weekday = "星期五";
		break;
	case 6:
		t.str_chi_weekday = "星期六";
		break;
	default:
		break;
	}

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

	std::stringstream ss;
	ss << t.m_year << "-" << t.m_month << "-" << t.m_day << " (" << t.str_weekday << ") "
		<< t.str_hour << ":" << t.str_minute << ":" << t.str_second;
	t.str_time = ss.str();

	std::stringstream ss_chi;
	ss_chi << t.m_year << "年" << t.m_month << "月" << t.m_day << "號 (" << t.str_chi_weekday << ") "
		<< t.m_chi_12hr_format << t.m_12hr_hour << "時" << t.str_minute << "分" << t.str_second << "秒";
	t.str_chi_time = ss_chi.str();

	return t;
}