#include "StdAfx.h"
#include "TreeviewData.h"

TreeviewData::TreeviewData()
{
	
}

TreeviewData::~TreeviewData(void)
{

}

int iListIndex;
CString csFileName;
CString csFullDirectory;

void TreeviewData::setFullDirectory(CString full)
{
	csFullDirectory = full;
}
CString TreeviewData::getFullDirectory()
{
	return csFullDirectory;
}

void TreeviewData::setFileName(CString name)
{
	csFileName = name;
}
CString TreeviewData::getFileName()
{
	return csFileName;
}

void TreeviewData::setListIndex(int index)
{
	iListIndex = index;
}

int TreeviewData::getListIndex()
{
	return iListIndex;
}

