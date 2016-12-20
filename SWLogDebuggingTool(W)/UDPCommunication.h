#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <string>
#include <sstream>

#include <vector>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <time.h>

#include "AgtDataMsg.h"
#include "AgtInfoList.h"
#include "AgtInfoMsg.h"
#include "DataReqMsg.h"
#include "TextManager.h"
#include "FolderManager.h"
#include "XMLManager.h"
#include "AgtRscMsg.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#pragma once

class UDPCommunication : public DataReqMsg, public AgtInfoList, public AgtInfoMsg, public TextManager, public FolderManager
{
public:
	UDPCommunication(void);
	~UDPCommunication(void);

	XMLManager mXMLManager;

	int recvfromTimeOut(SOCKET socket, long sec, long usec);
	void WSAInit();
	void InitSocket_Wt(int& hRecvSock, int& hRecvUniSock, int& hSndSock);
	void MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort);
	void InforReq(int iSndSock, int iWatcherPort, char* cMultiGroup);
	list<string> RcvInfor(int iRcvUniSock, int iTimeout_sec);
	list<string> RcvInfor_nonTimeout(int iRcvUniSock, int iTimeout_sec);

	int GetDate();
	int GetDate_DAY();

	BOOL SndDataReq_MFC(int iSndSockUni, string sAgtIP, string sReqFileName, int iWtcPort);
	void LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName);
	char* SndDataReq(int iSndSockUni, list<string> lIPandFileList);
	void ResourceReq(int iSndSock, int iWatcherPort, char* cMultiGroup);
	void RcvRsc(int iRcvUniSock, int iTimeout_sec);
	;
	WSADATA wsaData;
	SOCKADDR_IN addr;
	struct ip_mreq Rcv_Mreq;
	int iState;

	time_t tTimer;
	struct tm tTimer_St;
	int iToday;
	char cToday[10];

	PHOSTENT pHostInfo;
	char cHostName[64], cIPAddr[15];

	string sSlctAgtIP;
};
