#pragma once
#include <list>

using namespace std;

class AgtInfoList
{
public:
	AgtInfoList(void);
	~AgtInfoList(void);

	list<char*> AgtIPList;
	list<string> AgtFileList;

	void PrintList(list<char*> TargetList);

};