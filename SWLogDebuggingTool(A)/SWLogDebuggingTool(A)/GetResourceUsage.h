#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Pdh.h>
#include <sstream>
#include <string>

#include "cpuu.h"
#include "AgtInfoList.h"

#pragma once

class GetResourceUsage
{
public:
	GetResourceUsage(void);
	~GetResourceUsage(void);

	static PDH_HQUERY cpuQuery;
	static PDH_HCOUNTER cpuTotal;

	CPUUsage mCPUUsage;

	//Function
	float GetCPUUsage();
	DWORDLONG GetRAMUsage();
	string GetDiskFreeSpace_All();
};
