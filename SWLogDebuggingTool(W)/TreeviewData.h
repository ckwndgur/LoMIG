#pragma once
#include <list>
#include <string>
using namespace std;

class TreeviewData
{
public:
	TreeviewData();
	~TreeviewData(void);

	void setFullDirectory(CString full);
	CString getFullDirectory();
	void setFileName(CString filename);
	CString getFileName();

	void setListIndex(int index);
	int getListIndex();

public:
	
};