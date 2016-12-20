#include "StdAfx.h"
#include "GetDataFromAgent_WIN.h"

GetDataFromAgent_WIN::GetDataFromAgent_WIN(void)
{
}

GetDataFromAgent_WIN::~GetDataFromAgent_WIN(void)
{
}

char* GetDataFromAgent_WIN::GetAgentName(int& iAgtNameSize)
{
	char cAgtName[64];
	int nError = gethostname(cAgtName, sizeof(cAgtName));
	iAgtNameSize = sizeof(cAgtName);

	return cAgtName;
}
char* GetDataFromAgent_WIN::GetAgentIP()
{
	char AgtNameInfoBuf[64];
	char AgtIPInfoBuf[15];

	int nError = gethostname(AgtNameInfoBuf, sizeof(AgtNameInfoBuf));

	if (nError == 0)
	{
		PHOSTENT pHostInfo = gethostbyname(AgtNameInfoBuf);
		// ip address 파악
		strcpy(AgtIPInfoBuf, inet_ntoa(*(struct in_addr*)pHostInfo->h_addr_list[0]));
	}
	return AgtIPInfoBuf;
}

char* GetDataFromAgent_WIN::GetAgentFileInfo(char* cAgtLogPathBuf)
{
	char cFileListOutput[2048];
	char cFailReturn[2] = "F";

	int iFileFlag = 0;

	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cAgtLogPathBuf, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			//fputs("FindFirstFile() error\n", stderr);
			iFileFlag = 1;
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	if(iFileFlag == 1)
		return cFailReturn;

	printf("==================================================\n");
	printf("파일 리스트 출력\n");
	printf("==================================================\n");
	int iDeletPoint = 0;

	do{
		if(iDeletPoint <2)
		{
			iDeletPoint++;
		}
		else
		{
			printf("%s\n", FindData.cFileName);
			strcpy(cFileListOutput, FindData.cFileName);
			strcat(cFileListOutput, "\n");
			iDeletPoint++;
		}
	}while(FindNextFile(hFind, &FindData));

	FindClose(hFind);

	return cFileListOutput;
}


char* GetDataFromAgent_WIN::GetLogDir(char* cLogPathBuf)
{
	char cFileList[2048];
	char cFailReturn[2] = "F";
	list<string> lFileList;

	char cLogFilePath[256];
	char cOpenFolder[257];

	string sUnderDir = "";

	int iFileFlag = 0;
	int iIndex = 0;
	int sel = 0;
	int iListSize = 0;

	memset(cLogFilePath, 0, sizeof(cLogFilePath));
	memset(cOpenFolder, 0, sizeof(cOpenFolder));

	strcpy(cLogFilePath, cLogPathBuf);
	strcpy(cOpenFolder, cLogFilePath);

	strcat(cOpenFolder, "*");

	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cOpenFolder, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			//fputs("FindFirstFile() error\n", stderr);
			iFileFlag = -1;
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	if(iFileFlag == -1)
		return cFailReturn;

	printf("==================================================\n");
	printf("로그 파일을 수신한 날짜를 선택하세요.\n");
	printf("==================================================\n");
	int iDeletPoint = 0;

	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else if(iDeletPoint == 2)
		{
			strcpy(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
			iDeletPoint++;
		}
		else
		{
			strcat(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
		}
	}while(FindNextFile(hFind, &FindData));

	std::string sDateList(cFileList);
	std::stringstream strmDateList(sDateList);

	std::string sSingleDate;

	while(strmDateList.good())
	{
		getline(strmDateList, sSingleDate,'\n');
		lFileList.push_back(sSingleDate);	
	}

	list<string>::iterator iterStartList = lFileList.begin();
	list<string>::iterator iterEndList = lFileList.end();
	iListSize  = lFileList.size();

	while(1)
	{		
		if(iIndex == (iListSize-1))
		{
			break;
		}
		cout<< iIndex << ". " << *iterStartList <<endl;
		iterStartList++;
		iIndex++;
	}
	cout << " 날짜 선택 : ";
	cin >> sel;

	iterStartList = lFileList.begin();

	if(sel > lFileList.size()-2)
		return cFailReturn;

	for(int i = 0;i != sel;i++)
	{
		iterStartList ++;
	}

	sUnderDir = *iterStartList;
	strcat(cLogFilePath, sUnderDir.c_str());
	strcat(cLogFilePath, "\\");
	strcpy(cOpenFolder, cLogFilePath);

	strcat(cOpenFolder, "*");

	lFileList.clear();

	printf("==================================================\n");
	printf("해당하는 Agent의 IP를 선택하세요.\n");
	printf("==================================================\n");
	iIndex = 0;
	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cOpenFolder, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			//fputs("FindFirstFile() error\n", stderr);
			iFileFlag = -1;
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	if(iFileFlag == -1)
		return cFailReturn;

	iDeletPoint = 0;

	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else if(iDeletPoint == 2)
		{
			strcpy(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
			iDeletPoint++;
		}
		else
		{
			strcat(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
		}
	}while(FindNextFile(hFind, &FindData));

	std::string sIPList(cFileList);
	std::stringstream strmIPList(sIPList);

	std::string sIP;

	while(strmIPList.good())
	{
		getline(strmIPList, sIP,'\n');
		lFileList.push_back(sIP);	
	}

	iterStartList = lFileList.begin();
	iterEndList = lFileList.end();

	iListSize  = lFileList.size();

	iIndex = 0;
	while(1)
	{		
		if(iIndex == (iListSize-1))
		{
			break;
		}
		cout<< iIndex << ". " << *iterStartList <<endl;
		iIndex++;
		iterStartList++;
	}
	cout << " Agent IP선택 : ";
	cin >> sel;

	if(sel > lFileList.size()-2)
		return cFailReturn;

	iterStartList = lFileList.begin();

	for(int i = 0;i != sel;i++)
	{
		iterStartList ++;
	}

	sUnderDir = *iterStartList;

	memset(cOpenFolder, 0, sizeof(cOpenFolder));

	strcat(cLogFilePath, sUnderDir.c_str());
	strcat(cLogFilePath, "\\");
	strcpy(cOpenFolder, cLogFilePath);

	lFileList.clear();
	FindClose(hFind);

	return cOpenFolder;
}


string GetDataFromAgent_WIN::GetLogFileDir(char* cLogPathBuf)
{
	char cFileList[2048];
	char cFailReturn[2] = "F";
	list<string> lFileList;
	char cLogFilePath[256];
	char cOpenFolder[257];
	string sUnderDir = "";
	int iFileFlag = 0;
	int iIndex = 0;
	int sel = 0;
	int iListSize = 0;
	int iDeletPoint = 0;

	memset(cLogFilePath, 0, sizeof(cLogFilePath));
	memset(cOpenFolder, 0, sizeof(cOpenFolder));

	strcpy(cLogFilePath, cLogPathBuf);
	strcpy(cOpenFolder, cLogFilePath);

	strcat(cOpenFolder, "*");

	printf("==================================================\n");
	printf("해당하는 로그파일을 선택하세요.\n");
	printf("==================================================\n");

	iIndex = 0;

	do
	{
		//파일 경로를 추출
		hFind = FindFirstFile(cOpenFolder, &FindData);

		if(hFind==INVALID_HANDLE_VALUE)
		{
			//fputs("FindFirstFile() error\n", stderr);
			iFileFlag = -1;
			break;
		}
	}while(hFind==INVALID_HANDLE_VALUE);

	if(iFileFlag == -1)
		return cFailReturn;

	iDeletPoint = 0;

	do{
		if(iDeletPoint < 2)
		{
			iDeletPoint++;
		}
		else if(iDeletPoint == 2)
		{
			strcpy(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
			iDeletPoint++;
		}
		else
		{
			strcat(cFileList, FindData.cFileName);
			strcat(cFileList, "\n");
		}
	}while(FindNextFile(hFind, &FindData));

	std::string sFileList(cFileList);
	std::stringstream strmFileList(sFileList);

	std::string sSingleFileName;

	while(strmFileList.good())
	{
		getline(strmFileList, sSingleFileName,'\n');
		lFileList.push_back(sSingleFileName);	
	}

	list<string>::iterator iterStartList = lFileList.begin();
	list<string>::iterator iterEndList = lFileList.end();

	iListSize  = lFileList.size();

	iIndex = 0;
	while(1)
	{		
		if(iIndex == (iListSize-1))
		{
			break;
		}
		cout<< iIndex << ". " << *iterStartList <<endl;
		iIndex++;
		iterStartList++;
	}
	cout << " 로그파일 선택 : ";
	cin >> sel;

	if(sel > lFileList.size()-2)
		return cFailReturn;

	iterStartList = lFileList.begin();

	for(int i = 0;i != sel;i++)
	{
		iterStartList ++;
	}

	char Temp[257];

	memset(Temp, 0, sizeof(Temp));

	sUnderDir = *iterStartList;

	lFileList.clear();
	FindClose(hFind);

	return sUnderDir;
}

char* GetDataFromAgent_WIN::GetLogData(char cLogFilePath[256], int& iCurrentPointOutput)
{
	char cLogDataOutput[2048];
	int iLoaded = 0;
	DWORD iTempNum = 0;

	memset(cLogDataOutput, 0, sizeof(cLogDataOutput));

	hFile = CreateFile(LPCSTR(cLogFilePath), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		printf("정확한 파일 경로가 아님\n");
	}
	SetFilePointer(hFile, 0, NULL, iCurrentPointOutput);

	//2048은 읽어들일 데이터

	iLoaded = ReadFile(hFile, cLogDataOutput, 2048, &iTempNum, 0);
	//
	iCurrentPointOutput = iCurrentPointOutput+iLoaded;

	return cLogDataOutput;
}