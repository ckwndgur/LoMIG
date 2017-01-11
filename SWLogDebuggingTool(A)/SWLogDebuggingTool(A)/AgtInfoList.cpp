#include "AgtInfoList.h"

AgtInfoList::AgtInfoList(void)
{
}

AgtInfoList::~AgtInfoList(void)
{
}

void AgtInfoList::PrintList(list<char*> TargetList)
{
	for (list<char*>::iterator iterPos = TargetList.begin(); iterPos != TargetList.end(); ++iterPos)
	{
		//cout<< *iterPos <<endl;
	}
}