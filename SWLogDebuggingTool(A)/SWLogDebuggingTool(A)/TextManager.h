#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

class TextManager
{
public:
	TextManager(void);
	~TextManager(void);

	void WriteText(char* filedirectory, char* filename, string data);
	int GetFileSize(char* filedirectory, char* filename);
	void MakeDirectory(char* filedirectory, char* filename);
	
	string ReadText(char* filedirectory, char* filename, int &iRdLen);
	char* AppendChar(char* arg1, char* arg2);
	
};
