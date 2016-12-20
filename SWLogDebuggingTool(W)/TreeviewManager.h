#pragma once
#include "TreeviewData.h"
#include <list>
#include <string>
#include <iostream>
#include <afxwin.h>
#include <Windows.h>

using namespace std;

class TreeviewManager
{
public:
	TreeviewManager(void);
	~TreeviewManager(void);

	list<CString> FindDirectory(LPCTSTR pstr);
	list<TreeviewData> FindLogFile(LPCTSTR pstr);
	list<CString> FullDirectory;
	/*list<CString> OrderDirectory(int order);*/

	//int GetListSize(list<TreeviewData> input);
	void SetupListIndex(list<TreeviewData> input);
	/*list<TreeviewData> SortingList(list<TreeviewData> input);*/


	int GetCharNumber(CString str, char pstr);

	list<TreeviewData> TreeviewList;
};
