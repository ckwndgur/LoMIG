#pragma once

#include <Windows.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <list>

using namespace std;

class Filter
{
public:
	Filter();
	~Filter();
	list<CString> DoFilter(int Category,string WantedLog,string Title, CString filepath, CString Rfilepath, bool multiflag);
	list<CString> MultiFilter(list<string> keywords, string Title, CString originpath, CString newpath, bool multifalg);
	string LogDivider(int category, string alinelog);
	string CreatingTime(string WantedLog);

	string WantedLog;
	CString LogColumn;
	CString ResultPath;


	list<CString> cslstFilteredData;
};