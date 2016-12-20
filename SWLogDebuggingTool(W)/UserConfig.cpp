#include "StdAfx.h"
#include "UserConfig.h"
#define ConfigFileName "configfile"

TextManager mTManager;


UserConfig::UserConfig(void)
{
}

UserConfig::~UserConfig(void)
{
}

string UserConfig::GetExeDirectory()
{
	char buffer[MAX_PATH];
	string buf;

	//GetModuleFileName(NULL, (LPWCH)buffer, MAX_PATH);
	GetModuleFileName(NULL, (LPCH)buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	buf = string(buffer).substr(0, pos);

	buf = buf + "\\";

	return buf;
}

void UserConfig::InitConfig()
{
	// 	ConfigFileDirectory = GetExeDirectory();
	// 
	// 	string initpath = ""; 
	// 	initpath = "Agent: \n";
	// 	initpath.append("c:\\Temp\\CoreDebug\\\n");
	// 	initpath.append("Watcher:\n");
	// 	initpath.append("C:\LogDebugging\\");
	// 
	// 	mTManager.WriteText(ConfigFileDirectory, ConfigFileName, initpath);
}

char* UserConfig::GetAgtDirectory()
{

	return SelAorWDirectory(false);
}

char* UserConfig::GetWchDirectory()
{
	return SelAorWDirectory(true);
}

char* UserConfig::SelAorWDirectory(bool bSel)
{
	string buffer[4];

	//ifstream infile;
	//char input[2048];
	//int cnt = 0;

	////infile.open(mSManager.AppendChar(ConfigFileDirectory, ConfigFileName));

	//while(!infile.eof()){
	//	infile.getline(input, 2048);
	//	buffer[cnt].append(input);

	//	if(cnt<4)
	//		cnt++;
	//}

	//infile.close();


	if(bSel = false)
		return (char*)buffer[1].c_str();
	else
		return (char*)buffer[3].c_str();
}
