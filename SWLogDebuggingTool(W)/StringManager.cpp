#include "StdAfx.h"
#include "StringManager.h"

StringManager::StringManager(void)
{
}

StringManager::~StringManager(void)
{
}


char* StringManager::AppendChar(char* arg1, char* arg2)
{

	char *concat = (char *)malloc(strlen(arg1) + strlen(arg2) + 1);
	strcpy(concat, arg1);
	strcat(concat, arg2);

	return concat;
}

char* StringManager::StringToChar(string arg)
{
	return (char *)arg.c_str();
}

// CString StringManager::FindCString(CString contents, char strTok, int index)
// {
// 	CString cstr;
// 	AfxExtractSubString(cstr, contents, index, strTok);
// 	return cstr;
// }