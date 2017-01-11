#include <stdio.h>
#include <ws2tcpip.h>             //ip_mreq 구조체 정의되어있음
#include <windows.h>
#include <tchar.h>
#include <string>
#include <tchar.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <limits.h>

#include "GetDataFromAgent_WIN.h"
#include "AgtInfoMsg.h"
#include "DataReqMsg.h"
#include "DataCollisionAvoidence.h"
#include "TextManager.h"
#include "AgtDataMsg.h"
#include "XMLManager.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFSIZE 4096

#pragma once

using namespace std;

class Ucommunicatino_Agent : public AgtInfoMsg, public DataReqMsg, public TextManager, public AgtDataMsg
{
public:
	Ucommunicatino_Agent(void);
	~Ucommunicatino_Agent(void);
	
	void InitSocket_Ag(int& hRecvSock, int& hRecvUniSock, int& hSndSock);
	void InitSocket_Ag(int& hRecvSock, int& hSndSock);

	int recvfromTimeOut(SOCKET socket, long sec, long usec);
	void MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort);
	int RcvMsgDscr_Fix(int iRcvSock_multi, int iRcvSock_uni, char*& cWatcherIPOutput, int& iWatcherPortOutput);
	void SndToDst_Uni(int iSndSock, char* cDestIP, int iDestPort, char* cMessage);
	void GetDate(int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int& iSec);

	WSADATA wsaData;
	SOCKADDR_IN addr, SndAddr;
	int state;
	struct ip_mreq Rcv_Mreq;
	
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	string sFilePath;
	PHOSTENT pHostInfo;

	char cMultiRcvBuf[4096];
	char cUniRcvBuf[4096];

	time_t tTimer;
	struct tm tTimer_St;
	int iToday;
	char cToday[10];
};
