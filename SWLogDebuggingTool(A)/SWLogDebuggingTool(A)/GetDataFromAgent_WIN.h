#include <stdio.h>
#include <windows.h>
#include "AgtInfoList.h"
#include <string>


#pragma once

class GetDataFromAgent_WIN : public AgtInfoList
{
public:
	GetDataFromAgent_WIN(void);
	~GetDataFromAgent_WIN(void);

	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	HANDLE hFile;

	char* GetAgentName(int& iAgtNameSize);
	char* GetAgentIP();
	string GetAgentFileInfo(char* cAgtLogPathBuf);
	char* GetLogData(char cLogFilePath[256], int& iCurrentPointOutput);
	
	//list<char*> GetAgentFileInfo_List(char* cAgtLogPathBuf);
};
