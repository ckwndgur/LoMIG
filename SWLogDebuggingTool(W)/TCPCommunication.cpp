#include "StdAfx.h"
#include "TCPCommunication.h"

TCPCommunication::TCPCommunication(void)
{
}

TCPCommunication::~TCPCommunication(void)
{
}

void TCPCommunication::InitWinSock()
{
	WSAStartup(MAKEWORD(2,2), &wsaData);
}

void TCPCommunication::TCPSockInit(int& iTCPSock)
{
	iTCPSock = socket(PF_INET, SOCK_STREAM, 0);

	//int nTimeout = 3000; // 3 seconds
	//setsockopt(iTCPSock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(nTimeout));

	int iRcvSockBufSize = 1048576*2;
	setsockopt(iTCPSock, SOL_SOCKET, SO_RCVBUF, (const char*) &iRcvSockBufSize, sizeof(iRcvSockBufSize));

	if(iTCPSock == INVALID_SOCKET)
		AfxMessageBox("TCPSOCK_INIT_ERROR");
}

void TCPCommunication::TCPAddrAlloc(int& iTCPSock, int iTCPPort)
{
	SOCKADDR_IN sServAddr;

	memset(&sServAddr, 0, sizeof(sServAddr));
	sServAddr.sin_family = AF_INET;
	sServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sServAddr.sin_port = htons(iTCPPort);

	if(bind(iTCPSock, (SOCKADDR*) &sServAddr, sizeof(sServAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox("TCPAddrAlloc ERROR");
	}
}

void TCPCommunication::TCPAddrAlloc_Auto(int& iTCPSock, int& iTCPPort)
{
	SOCKADDR_IN sServAddr;

	memset(&sServAddr, 0, sizeof(sServAddr));
	sServAddr.sin_family = AF_INET;
	sServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sServAddr.sin_port = 0;

	if(bind(iTCPSock, (SOCKADDR*) &sServAddr, sizeof(sServAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox("TCPAddrAlloc ERROR");
	}
	else
	{
		//bind success
	}

	int m_socklen_t = sizeof(sServAddr);

	if (getsockname(iTCPSock, (SOCKADDR*) &sServAddr, &m_socklen_t) == -1)
	{
		AfxMessageBox("getsockname");
	}
	else
	{	
		//GetSockName for returning port number
		iTCPPort = ntohs(sServAddr.sin_port);
	}
}

BOOL TCPCommunication::TryCnct(int& iTCPSock, char* cIP ,int iPort)
{
	SOCKADDR_IN sockAddr;

	ULONG uNonBlkOpt = 1;
	int Ret;
	DWORD dwErr;

	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(cIP);
	sockAddr.sin_port = htons(iPort);

	//non-blocking TCP
	//ioctlsocket(iTCPSock, FIONBIO, &uNonBlkOpt);
	//int m_socklen_t = sizeof(sockAddr);

	Ret = connect(iTCPSock, (SOCKADDR*) &sockAddr, sizeof(sockAddr));

	return TRUE;

	//non-block TCP
	/*
	if(Ret == SOCKET_ERROR)
	{
		dwErr = WSAGetLastError();

		if(dwErr ==WSAEWOULDBLOCK)
		{
			fd_set         Write, Err;
			TIMEVAL      Timeout;

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(iTCPSock, &Write);
			FD_SET(iTCPSock, &Err);

			Timeout.tv_sec  = 10;
			Timeout.tv_usec = 100; // your timeout

			Ret = select (0,                // ignored
				NULL,           // read,
				&Write,        // Write Check
				&Err,            // Error check
				&Timeout);
			if (Ret == 0)
			{
				Sleep(1);
				//Connect Timeout - TimeoutSec
			}
			else
			{
				if (FD_ISSET(iTCPSock, &Write))
				{
					//While Connecting
					return TRUE;
				}
				if (FD_ISSET(iTCPSock, &Err))
				{
					//Select() Error
					return FALSE;
				}
			}
		}
		else
		{
			//nConnect Error
			return FALSE;
		}
	}
	else
	{
		//Connected
		return TRUE;
	}
	//Connect Fail

	return FALSE;*/
}

void TCPCommunication::AcptCnct(int& iTCPServSock, int& iTCPCltSock)
{
	SOCKADDR_IN CltAddr;
	int iszCltAddr;

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	if(listen(iTCPServSock, 5))
		AfxMessageBox("Listen ERROR");

	iszCltAddr = sizeof(CltAddr);
	iTCPCltSock = accept(iTCPServSock, (SOCKADDR*) &CltAddr, &iszCltAddr);
}

list<string> TCPCommunication::InfoRcv(int& iTCPServSock)
{
	char cRcvBuf[4096];
	int iRcvdDataLen;

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	while(1)
	{
		memset(cRcvBuf, 0, sizeof(cRcvBuf));

		iRcvdDataLen = recv(iTCPServSock, cRcvBuf, sizeof(cRcvBuf), 0);

		if(iRcvdDataLen == 0)
			break;
		if(iRcvdDataLen == -1)
			break;

		memcpy(&MyAgtInfoMsg, cRcvBuf, sizeof(MyAgtInfoMsg));

		mXMLManager.CreatXML_AgentInfo(MyAgtInfoMsg.cAgtIPAddr);
		mXMLManager.EditElementXML("AgentInfo", "AgentIP", MyAgtInfoMsg.cAgtIPAddr);
		mXMLManager.EditElementXML("AgentInfo", "AgentName", MyAgtInfoMsg.cAgtName);
		mXMLManager.EditElementXML("AgentInfo", "AgentLogFileList", MyAgtInfoMsg.cAgtFileList);

		std::string sAgtFileList(MyAgtInfoMsg.cAgtFileList);
		std::stringstream strmAgtFileList(sAgtFileList);

		std::string sSingleFileName;
		std::string sIPwithFileName;

		strcat(MyAgtInfoMsg.cAgtIPAddr, "/");

		while(strmAgtFileList.good())
		{
			getline(strmAgtFileList, sSingleFileName,'\n');

			sIPwithFileName = MyAgtInfoMsg.cAgtIPAddr;
			sIPwithFileName += sSingleFileName;

			MyAgtInfoList.AgtFileList.push_back(sIPwithFileName);	
		}
	}

	return MyAgtInfoList.AgtFileList;
}

void TCPCommunication::LogFileSnd(int iRcvSocket, char* cFileDir, char* cFileName)
{
	struct AgtDataMsgStruct MyAgtDataMsg;
	DWORD dLen;

	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));

	string sWriteData = "";

	mFolderManager.MakeDirectory(cFileDir, cFileName);

	recvfrom(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0, NULL, 0);

	sWriteData = "";
	sWriteData = MyAgtDataMsg.cLogData;

	//WriteFile(hFile, sWriteData.c_str(), strlen(sWriteData.c_str()), &dLen, 0);
	mTextManager.WriteText(cFileDir, cFileName, sWriteData, false);

	if(MyAgtDataMsg.bLastPacket == TRUE)
	{
		//printf("File Download Complete.\n");
		AfxMessageBox("File Download Done");
	}
}

void TCPCommunication::AgentDirChange(int &iRcvSocket,string sChangeDir)
{
	struct DataReqMsgStruct MyDataReqMsg;
	
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	MyDataReqMsg.nReqType = 5;
	memcpy(&MyDataReqMsg.cFileDir, sChangeDir.c_str(), strlen(sChangeDir.c_str()));


	send(iRcvSocket, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0);
	Sleep(1);

	shutdown(iRcvSocket, SD_SEND);
	closesocket(iRcvSocket);
}

void TCPCommunication::LogFileReq(int& iRcvSocket, string sSaveDir, string sReqFileName, string sFileDir)
{
	char cWtcName[64];
	char* cSaveDir;
	char* cReqFileName;
	string sWatcherIP = "";
	string sWriteData = "";
	string sBuf = "";
	string sAgentIP = "";
	stringstream sDate;
	struct DataReqMsgStruct MyDataReqMsg;
	struct AgtDataMsgStruct MyAgtDataMsg;
	PHOSTENT pHostInfo;
	int iBreaker = 0;

	memset(cWtcName, 0, sizeof(cWtcName));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	int nError = gethostname(cWtcName, sizeof(cWtcName));
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cWtcName);
		// ip address 파악
		sWatcherIP = inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]);
	}

	//port 1883;
	memcpy(&MyDataReqMsg.cFileDir, sFileDir.c_str(), strlen(sFileDir.c_str()));
	memcpy(&MyDataReqMsg.cWatcherIP, sWatcherIP.c_str(), strlen(sWatcherIP.c_str()));
	memcpy(&MyDataReqMsg.cReqFileName, sReqFileName.c_str(), strlen(sReqFileName.c_str()));
	MyDataReqMsg.iWatcherPort = TCP_PORT;
	MyDataReqMsg.nReqType = 2;
	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));
	send(iRcvSocket, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0);
	Sleep(1);
	ULONG ulBlock = 0;
	ioctlsocket(iRcvSocket, FIONBIO, &ulBlock);

	cSaveDir = &sSaveDir[0u];
	cReqFileName = &sReqFileName[0u];

	mTextManager.MakeDirectory(cSaveDir, cReqFileName);

	while (1)
	{
		Sleep(2);
		int ilen = recv(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0);
		if(ilen > 0)
		{
			sWriteData = "";
			sWriteData = MyAgtDataMsg.cLogData;
			sWriteData = sWriteData.substr(0, 15000);
			mTextManager.WriteText(cSaveDir, cReqFileName, sWriteData, MyAgtDataMsg.bLastPacket);
			if(MyAgtDataMsg.bLastPacket)
				break;
		}
		else
		{
			iBreaker++;
			if(MyAgtDataMsg.bLastPacket)
				break;
			if(iBreaker == 2500)
				break;
		}
	}

	Sleep(2);
	//AfxMessageBox("File Download Done");

	shutdown(iRcvSocket, SD_SEND);
	closesocket(iRcvSocket);
}

void TCPCommunication::LogFileRcv(int& iRcvSocket, char* cFileDir, char* cFileName)
{
	struct AgtDataMsgStruct MyAgtDataMsg;
	DWORD dLen;

	memset(&MyAgtDataMsg, 0, sizeof(struct AgtDataMsgStruct));

	string sWriteData = "";

	mFolderManager.MakeDirectory(cFileDir, cFileName);

	recvfrom(iRcvSocket, (char*)&MyAgtDataMsg, sizeof(struct AgtDataMsgStruct), 0, NULL, 0);

	sWriteData = "";
	sWriteData = MyAgtDataMsg.cLogData;

	//WriteFile(hFile, sWriteData.c_str(), strlen(sWriteData.c_str()), &dLen, 0);
	mTextManager.WriteText(cFileDir, cFileName, sWriteData, false);

	if(MyAgtDataMsg.bLastPacket == TRUE)
	{
		//printf("File Download Complete.\n");
		AfxMessageBox("File Download Done");
	}
}

char* TCPCommunication::ReqRsc(int& iTCPSock, float& CPUUsage, DWORD& RAMUsage)
{
	char cWtcName[64];
	char HDDUsage[4096];
	string sWatcherIP;
	int iRcvLen = 0;
	struct DataReqMsgStruct MyDataReqMsg;
	struct AgtRcsMsgStruct MyAgtRcsMsg;
	PHOSTENT pHostInfo;

	memset(HDDUsage, 0, sizeof(HDDUsage));
	memset(cWtcName, 0, sizeof(cWtcName));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	int nError = gethostname(cWtcName, sizeof(cWtcName));
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cWtcName);
		// ip address 파악
		sWatcherIP = inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]);
	}

	//port 1883;
	memcpy(&MyDataReqMsg.cWatcherIP, sWatcherIP.c_str(), strlen(sWatcherIP.c_str()));
	MyDataReqMsg.iWatcherPort = TCP_PORT;
	MyDataReqMsg.nReqType = 3;

	send(iTCPSock, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0);
	Sleep(1);

	while(1)
	{
		iRcvLen = recv(iTCPSock, (char*)&MyAgtRcsMsg, sizeof(struct AgtRcsMsgStruct), 0);
		if(iRcvLen>0)
			break;
	}
	CPUUsage = MyAgtRcsMsg.fCPUUsage;
	RAMUsage = MyAgtRcsMsg.dwRAMUsage;
	memcpy(&HDDUsage, MyAgtRcsMsg.cHDDUsage, sizeof(HDDUsage));

	//AfxMessageBox("Rsc Req and Rcv is Done");
	shutdown(iTCPSock, SD_SEND);
	closesocket(iTCPSock);

	return HDDUsage;
}


list<string> TCPCommunication::LogListReq(int& iTCPSock, char* cAgentIP)
{
	char cWtcName[64];
	string sWatcherIP;
	string sLogList ="";
	string sBuf = "";
	char cBuf[8096];
	int iRcvLen = 0;
	PHOSTENT pHostInfo;
	struct DataReqMsgStruct MyDataReqMsg;
	struct AgtRcsMsgStruct MyAgtRcsMsg;

	AgtInfoList MyAgtInfoList = AgtInfoList();

	memset(cWtcName, 0, sizeof(cWtcName));
	memset(&MyDataReqMsg, 0, sizeof(struct DataReqMsgStruct));

	int nError = gethostname(cWtcName, sizeof(cWtcName));
	if (nError == 0)
	{
		pHostInfo = gethostbyname(cWtcName);
		// ip address 파악
		sWatcherIP = inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]);
	}

	//port 1883;
	memcpy(&MyDataReqMsg.cWatcherIP, sWatcherIP.c_str(), strlen(sWatcherIP.c_str()));
	MyDataReqMsg.iWatcherPort = TCP_PORT;
	MyDataReqMsg.nReqType = 4;

	send(iTCPSock, (char*)&MyDataReqMsg, sizeof(struct DataReqMsgStruct), 0);
	Sleep(100);

	while(1)
	{
		memset(cBuf, 0, sizeof(cBuf));
		Sleep(3);
		iRcvLen = recv(iTCPSock, cBuf, 8096/*sizeof dBuf*/, 0);
		if(iRcvLen>0)
		{
			sBuf += cBuf;
			sLogList += sBuf.substr(0, 8095);
			sBuf ="";
			//sLogList = sLogList.substr(0,8096)
			//mXMLManager.AddXML("AgentInfo", "AgentLogFileList", sLogList.c_str());

			//sLogList = sLogList.substr(0, strlen(sLogList.c_str())-1);
			if(iRcvLen<8096)
				break;
		}
	}

	//sLogList = sLogList.substr(0, strlen(sLogList.c_str())-1);
	std::string sAgtFileList(sLogList.c_str());
	std::stringstream strmAgtFileList(sAgtFileList);

	std::string sSingleFileName;

	while(strmAgtFileList.good())
	{
		getline(strmAgtFileList, sSingleFileName,'\n');
		MyAgtInfoList.AgtFileList.push_back(sSingleFileName);	
	}

	//Agent한대당 XML파일 하나 생성 후 데이터 입력
	mXMLManager.CreatXML_AgentInfo(cAgentIP);
	mXMLManager.EditElementXML("AgentInfo", "AgentLogFileList", sLogList.c_str());

	//AfxMessageBox("Rsc Req and Rcv is Done");
	shutdown(iTCPSock, SD_SEND);
	closesocket(iTCPSock);

	return MyAgtInfoList.AgtFileList;
}