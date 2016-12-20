#pragma once
#include <string>
#include <windows.h>
#include "StringManager.h"
#include <list>

using namespace std;

class TextManager
{
public:
	TextManager(void);
	~TextManager(void);

	void WriteText(char* filedirectory, char* filename, string data, bool lastflag);
	void WriteText(char* filedirectory, char* filename, string data);

	int GetFileSize(char* filedirectory, char* filename);

	int GetMaxLineSize(char* fulldirectory);
	int GetLinelength(char* fulldirectory);

	void MakeDirectory(char* filedirectory, char* filename);

	string ReadText(char* filedirectory, char* filename);
	string ReadText(char* fulldirectory);
	list<CString> ReadTextList(char* fulldirectory);

private:
	StringManager mStringManager;
};
