#pragma once

class DataReqMsg
{
public:
	DataReqMsg(void);
	~DataReqMsg(void);
};

struct DataReqMsgStruct 
{
	short nReqType;
	char cWatcherIP[15];
	int iWatcherPort;
	char cReqFileName[256];
	char cFileDir[256];
};