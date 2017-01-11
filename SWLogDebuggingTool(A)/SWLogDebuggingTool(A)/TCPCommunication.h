#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>
#include "AgtDataMsg.h"
#include "AgtInfoList.h"
#include "AgtInfoMsg.h"
#include "DataReqMsg.h"
#include "FolderManager.h"
#include "TextManager.h"
#include "XMLManager.h"

#pragma once

class TCPCommunication
{
public:
	TCPCommunication(void);
	~TCPCommunication(void);

	WSADATA wsaData;

	FolderManager mFolderManager;
	TextManager mTextManager;
	XMLManager mXMLManager;

	void InitWinSock();
	void TCPSockInit(int& iTCPSock);
	void TCPAddrAlloc(int& iTCPSock, int& iTCPPort);
	void TCPAddrAlloc_Auto(int& iTCPSock, int& iTCPPort);

	void CnctTo(int& iTCPSock, char* cIP, int iPort);
	void AcptCnct(int& iTCPServSock, int& iTCPCltSock);
	
	list<string> InfoRcv(int& iTCPServSock);
	void LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName);
};
