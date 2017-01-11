#include <Windows.h>

#pragma once

class AgtRscMsg
{
public:
	AgtRscMsg(void);
	~AgtRscMsg(void);
};

struct AgtRcsMsgStruct
{
	short nReqType;
	float fCPUUsage;
	DWORD dwRAMUsage;
	char cHDDUsage[4096];
};