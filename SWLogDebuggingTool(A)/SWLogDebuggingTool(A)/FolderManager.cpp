#include "FolderManager.h"


FolderManager::FolderManager(void)
{
}

FolderManager::~FolderManager(void)
{
}

void FolderManager::MakeDirectory(char* filedirectory, char* filename)
{
	char cTemp[256], *sp;
	strcpy(cTemp, mStringManager.AppendChar(filedirectory, filename));
	cTemp[strlen(cTemp) - 1] = NULL;
	sp = cTemp;

	while((sp = strchr(sp, '\\')))
	{
		if(sp> cTemp && *(sp - 1) != ':')
		{
			*sp = '\0';
			::CreateDirectoryA(cTemp, NULL);
			*sp = '\\';
		}
		sp++;
	}
}

void FolderManager::MakeDirectory(char* filedirectory)
{
	char cTemp[256], *sp;
	strcpy(cTemp, filedirectory);
	cTemp[strlen(cTemp) - 1] = NULL;
	sp = cTemp;

	while((sp = strchr(sp, '\\')))
	{
		if(sp> cTemp && *(sp - 1) != ':')
		{
			*sp = '\0';
			::CreateDirectoryA(cTemp, NULL);
			*sp = '\\';
		}
		sp++;
	}
}