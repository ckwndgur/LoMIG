#pragma once

class AgtDataMsg
{
public:
	AgtDataMsg(void);
	~AgtDataMsg(void);
};

struct AgtDataMsgStruct
{
	short nReqType;
	char cLogData[15000];
	long lTotalDataSize;
	bool bLastPacket;
};