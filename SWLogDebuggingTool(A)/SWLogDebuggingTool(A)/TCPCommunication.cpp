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
	iTCPSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(iTCPSock == INVALID_SOCKET)
	{
		printf("TCPSOCK_INIT_ERROR\n");
		//AfxMessageBox("TCPSOCK_INIT_ERROR");
	}
	else
	{
		printf("TCPSOCK_INIT_SUCCESS\n");
	}
}

void TCPCommunication::TCPAddrAlloc(int& iTCPSock, int& iTCPPort)
{
	SOCKADDR_IN sServAddr;

	memset(&sServAddr, 0, sizeof(sServAddr));
	sServAddr.sin_family = AF_INET;
	sServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sServAddr.sin_port = htons(iTCPPort);

	if(bind(iTCPSock, (SOCKADDR*) &sServAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("TCPSOCK_INIT_ERROR");
		//AfxMessageBox("TCPAddrAlloc ERROR");
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
		printf("TCPAddrAlloc ERROR\n");
	}
	else
	{
		//bind success
	}

	int m_socklen_t = sizeof(sServAddr);

	if (getsockname(iTCPSock, (SOCKADDR*) &sServAddr, &m_socklen_t) == -1)
	{
		printf("getsockname Error\n");
	}
	else
	{	
		//GetSockName for returning port number
		iTCPPort = ntohs(sServAddr.sin_port);
	}
}

void TCPCommunication::AcptCnct(int& iTCPServSock, int& iTCPCltSock)
{
	SOCKADDR_IN CltAddr;
	int iszCltAddr;

	AgtInfoMsgStruct MyAgtInfoMsg = AgtInfoMsgStruct();
	AgtInfoList MyAgtInfoList = AgtInfoList();

	if(listen(iTCPServSock, 5))
		printf("TCPSOCK_INIT_ERROR");
		//AfxMessageBox("Listen ERROR");

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

void TCPCommunication::LogFileRcv(int iRcvSocket, char* cFileDir, char* cFileName)
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
	mTextManager.WriteText(cFileDir, cFileName, sWriteData);

	if(MyAgtDataMsg.bLastPacket == TRUE)
	{
		printf("File Download Complete.\n");
		//AfxMessageBox("File Download Done");
	}
}