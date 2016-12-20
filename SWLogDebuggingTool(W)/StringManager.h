#pragma once
#include <string>
using namespace std;

class StringManager
{
public:
	StringManager(void);
	~StringManager(void);

	char* AppendChar(char* arg1, char* arg2);
	char* StringToChar(string arg);
};
