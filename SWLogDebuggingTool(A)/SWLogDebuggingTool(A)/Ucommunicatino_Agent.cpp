#include "Ucommunicatino_Agent.h"

#define SERVER "234.56.78.9"
#define PORT 1883
#define FILEDIRECTORY "c:\\Temp\\CoreDebug\\*"

Ucommunicatino_Agent::Ucommunicatino_Agent(void)
{
}

Ucommunicatino_Agent::~Ucommunicatino_Agent(void)
{
}

//소켓통신을 위한 초기화 작업
//주소설정, WSA사용 설정, 소켓 생성 등..
void Ucommunicatino_Agent::InitSocket_Ag(int& hRecvSock, int& hRecvUniSock, int& hSndSock)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//소켓생성
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	else
		printf("1st Socket Open Success\n");

	hRecvUniSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvUniSock == INVALID_SOCKET)
		fputs("hRecvUniSock() error\n", stderr);
	else
		printf("2nb Socket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);
	else
		printf("3rd Socket Open Success\n");
	//hRecvSock, hSndSock, hRecvUniSock UDP소켓을 생성해줌
}

void Ucommunicatino_Agent::InitSocket_Ag(int& hRecvSock, int& hSndSock)
{
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		fputs("WSAStartup() error\n", stderr);

	//소켓생성
	hRecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hRecvSock == INVALID_SOCKET)
		fputs("Rsocket() error\n", stderr);
	else
		printf("RSocket Open Success\n");

	hSndSock = socket(PF_INET, SOCK_DGRAM, 0);
	if(hSndSock == INVALID_SOCKET)
		fputs("Ssocket() error\n", stderr);
	else
		printf("SSocket Open Success\n");
}

void Ucommunicatino_Agent::MultiGroupRcvSet(int iRcvSocket, char* MultiGroupAddr, int MultiGroupPort)
{
	int iPort = MultiGroupPort;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(iPort);

	while(1)
	{
		if(bind(iRcvSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			printf("Try with another port...\n");
			iPort++;
			addr.sin_port = htons(iPort);
		}
		else
		{
			printf("Binded with Port Num : %d...\n", iPort);
			break;
		}
	}

	//ip_mreq구조체에 가입할 그룹의 주소지정
	Rcv_Mreq.imr_multiaddr.s_addr = inet_addr(MultiGroupAddr);           //클레스 D의 멀티캐스트 주소
	Rcv_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //로컬주소

	//IP_ADD_MEMBERSHIP : 멀티캐스트 그룹에 가입
	state = setsockopt(iRcvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&Rcv_Mreq, sizeof(Rcv_Mreq));
	if(state == SOCKET_ERROR)
		fputs("Rsetsockopt() error\n", stderr);
}

int Ucommunicatino_Agent::recvfromTimeOut(SOCKET socket, long sec, long usec)
{
	// Setup timeval variable
	timeval timeout;
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;
	// Setup fd_set structure
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	// Return value:
	// -1: error occured
	// 0: timed out
	// >0: data ready to be read
	return select(0, &fds, 0, 0, &timeout);
}

//수신한 메시지가 에이전트가 실행중인 노드의 IP, 로그파일 경로, 파일목록 등을 요구하는 메시지 인지,
//로그파일에서 해당하는 데이터를 전송해달라는 메시지인지 판별(DiSCRiminator)하는 함수
int Ucommunicatino_Agent::RcvMsgDscr_Fix(int iRcvSock_multi, int iRcvSock_uni, char*& cWatcherIPOutput, int& iWatcherPortOutput)
{
	struct DataReqMsgStruct MyDataReqMsg;
	memset(cMultiRcvBuf, 0, sizeof(cMultiRcvBuf));

	int iMulti_res = recvfromTimeOut(iRcvSock_multi, 1, 0);
	while(recvfromTimeOut(iRcvSock_multi, 1, 0)!=0)
	{
		switch(iMulti_res)
		{
		case 0:
			Sleep(1);
			break;
		case -1:
			printf("RcvTimeOut Error");
			break;
		default:
			recvfrom(iRcvSock_multi, cMultiRcvBuf, sizeof(cMultiRcvBuf), 0, NULL, 0);
			memcpy(&MyDataReqMsg, cMultiRcvBuf, sizeof(MyDataReqMsg));
			//Agent 정보를 요청한 경우
			printf("요청타입 : %d\n", MyDataReqMsg.nReqType);
			printf("와처 IP  : ");
			printf(MyDataReqMsg.cWatcherIP);

			memcpy(cWatcherIPOutput, MyDataReqMsg.cWatcherIP, sizeof(MyDataReqMsg.cWatcherIP));

			printf("\n");
			printf("와처Port : %d\n", MyDataReqMsg.iWatcherPort);
			printf("=============================================================\n");
			printf("================ Wating Message From Watcher ================\n");
			printf("=============================================================\n");
			return 0;
			break;
		}
	}

	int iUni_res = recvfromTimeOut(iRcvSock_uni, 1, 0);
	memset(cUniRcvBuf, 0, sizeof(cUniRcvBuf));

	while(recvfromTimeOut(iRcvSock_uni, 1, 0)!=0)
	{
		switch(iUni_res)
		{
		case 0:
			break;
		case -1:
			printf("RcvTimeOut Error");
			break;
		default:
			recvfrom(iRcvSock_uni, cMultiRcvBuf, sizeof(cMultiRcvBuf), 0, NULL, 0);
			memcpy(&MyDataReqMsg, cMultiRcvBuf, sizeof(MyDataReqMsg));
			//Agent의 로그데이터를 요청한 경우
			printf("요청타입 : %d\n", MyDataReqMsg.nReqType);
			printf("와처 IP  : ");
			printf(MyDataReqMsg.cWatcherIP);
			printf("\n");
			printf("와처Port : %d\n", MyDataReqMsg.iWatcherPort);
			printf("요청받은 파일이름 : ");
			printf(MyDataReqMsg.cReqFileName);
			printf("\n");
			printf("=============================================================\n");
			printf("================ Wating Message From Watcher ================\n");
			printf("=============================================================\n");
			return 1;
			break;
		}
	}
	return -1;
}


void Ucommunicatino_Agent::SndToDst_Uni(int iSndSock, char* cDestIP, int iDestPort, char* cMessage)
{
	memset(&SndAddr, 0, sizeof(SndAddr));
	SndAddr.sin_family = AF_INET;
	SndAddr.sin_addr.s_addr = inet_addr(cDestIP); //전송받은 와처의 IP를 입력
	SndAddr.sin_port = htons(iDestPort);
	
	if(sendto(iSndSock, cMessage, strlen(cMessage), 0, (SOCKADDR*)&SndAddr, sizeof(SndAddr))==-1)
		printf("Agent Info Trans Failed\n");

	memset(cMessage, 0, sizeof(cMessage));
}

void Ucommunicatino_Agent::GetDate(int& iYear, int& iMonth, int& iDay, int& iHour, int& iMin, int& iSec)
{
	tTimer = time(NULL);

	localtime_s(&tTimer_St, &tTimer);

	iYear = (tTimer_St.tm_year + 1900);
	iMonth = (tTimer_St.tm_mon + 1);
	iDay = (tTimer_St.tm_mday);
	iHour = (tTimer_St.tm_hour);
	iMin = (tTimer_St.tm_min);
	iSec = (tTimer_St.tm_sec);	
}
