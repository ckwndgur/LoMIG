#include <time.h>

#pragma once

class MyClock
{
public:
	MyClock(void);
	~MyClock(void);

	int GetDate();
	int GetSec();
};
