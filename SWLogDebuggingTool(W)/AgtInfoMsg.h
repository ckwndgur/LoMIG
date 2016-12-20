#pragma once

class AgtInfoMsg
{
public:
	AgtInfoMsg(void);
	~AgtInfoMsg(void);
};

struct AgtInfoMsgStruct
{
	short nReqType;
	char cAgtIPAddr[15];
	char cAgtName[64];
	char cAgtLogDir[256];
	char cAgtFileList[2048];
};