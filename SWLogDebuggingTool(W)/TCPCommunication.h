#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#include "AgtDataMsg.h"
#include "AgtInfoList.h"
#include "AgtInfoMsg.h"
#include "DataReqMsg.h"
#include "TextManager.h"
#include "FolderManager.h"
#include "XMLManager.h"
#include "AgtRscMsg.h"

#define TCP_PORT 18840

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#pragma once

class TCPCommunication
{
public:
	TCPCommunication(void);
	~TCPCommunication(void);

	FolderManager mFolderManager;
	TextManager mTextManager;
	XMLManager mXMLManager;
	UserConfig mUserConfig;

	WSADATA wsaData;

	void InitWinSock();
	void TCPSockInit(int& iTCPSock);
	void TCPAddrAlloc(int& iTCPSock, int iTCPPort);
	void TCPAddrAlloc_Auto(int& iTCPSock, int& iTCPPort);

	BOOL TryCnct(int& iTCPSock, char* cIP ,int iPort);
	void CnctTo(int& iTCPSock, char* cIP, int iPort);
	void AcptCnct(int& iTCPServSock, int& iTCPCltSock);

	list<string> InfoRcv(int& iTCPServSock);
	void LogFileReq(int& iRcvSocket, string sSaveDir, string sReqFileName);
	void LogFileRcv(int& iRcvSocket, char* cFileDir, char* cFileName);
	void LogFileSnd(int iRcvSocket, char* cFileDir, char* cFileName);
	char* ReqRsc(int& iTCPSock, float& CPUUsage, DWORD& RAMUsage);

	list<string> LogListReq(int& iTCPSock, char* cAgentIP);
};
