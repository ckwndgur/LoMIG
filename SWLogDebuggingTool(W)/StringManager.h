#pragma once
#include "stdafx.h"
#include <string>
using namespace std;

class StringManager
{
public:
	StringManager(void);
	~StringManager(void);

	char* AppendChar(char* arg1, char* arg2);
	char* StringToChar(string arg);
	bool Str_replace(std::string& str, const std::string& from, const std::string& to);
};
