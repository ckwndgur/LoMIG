#pragma once
#include "StringManager.h"
#include <list>
#include <string>
#include <iostream>
#include <afxwin.h>
#include <Windows.h>

using namespace std;


class FolderManager
{
public:
	FolderManager(void);
	~FolderManager(void);

	void MakeDirectory(char* filedirectory, char* filename);
	void MakeDirectory(char* filedirectory);

private:
	StringManager mStringManager;
	
	//#define BASICDIRECTORY "C:\\LogDebugging\\*.*";
};
