#pragma once
#include <string>
#include <direct.h>
#include <Windows.h>
#include <iostream>
#include "TextManager.h"
#include "StringManager.h"


using namespace std;

class UserConfig
{
public:
	UserConfig(void);
	~UserConfig(void);

	string GetExeDirectory();
	void SetAgtDirectory(char* IP, char* AgtDirectory);
	void SetWchDirectory(char* IP, char* AgtDirectory);
	char* GetAgtDirectory();
	char* GetWchDirectory();

	void InitConfig();

private:
	string ConfigFileDirectory;
	StringManager mSManager;
	char* SelAorWDirectory(bool bSel);
};
