#include "StdAfx.h"
#include "MyClock.h"

MyClock::MyClock(void)
{
}

MyClock::~MyClock(void)
{
}

int MyClock::GetDate()
{
	time_t tTimer;
	struct tm tTimer_St;
	int iToday;

	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	iToday = (tTimer_St.tm_year + 1900) * 10000000000 + (tTimer_St.tm_mon + 1) * 100000000 + (tTimer_St.tm_mday)*1000000+(tTimer_St.tm_hour)*10000+(tTimer_St.tm_min)*100+(tTimer_St.tm_sec);	

	return iToday;
}

int MyClock::GetSec()
{
	time_t tTimer;
	struct tm tTimer_St;

	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	return tTimer_St.tm_sec;
}