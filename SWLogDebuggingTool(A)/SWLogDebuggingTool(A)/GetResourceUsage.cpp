#include "GetResourceUsage.h"

GetResourceUsage::GetResourceUsage(void)
{
}

GetResourceUsage::~GetResourceUsage(void)
{
}


string GetResourceUsage::GetDiskFreeSpace_All()
{
	/* do a loop to check each drive */
	int count;
	ULARGE_INTEGER totalbytes, freebytes;
	totalbytes.QuadPart = 0L;
	freebytes.QuadPart = 0L;

	int iTotalBytes, iFreeBytes;

	std::stringstream ssIntToStrBuf;
	std::string sIntToStrBuf;

	string drives[24] = {"C:\\","D:\\","E:\\","F:\\","G:\\","H:\\","I:\\","J:\\","K:\\","L:\\","M:\\","N:\\","O:\\","P:\\","Q:\\","R:\\","S:\\","T:\\","U:\\","V:\\","W:\\","X:\\","Y:\\","Z:\\"};

	for( count = 0 ; count<24 ; count++ )
	{
		/* something goes wrong, continue with next drive */
		if(GetDiskFreeSpaceEx(drives[count].c_str(), NULL, &totalbytes, &freebytes) == 0)
			continue;

		iTotalBytes = (int)(totalbytes.QuadPart>>30);
		iFreeBytes = (int)(freebytes.QuadPart>>30);

		sIntToStrBuf += drives[count];
		sIntToStrBuf +="/";

		ssIntToStrBuf << iTotalBytes;
		sIntToStrBuf += ssIntToStrBuf.str();
		ssIntToStrBuf.str("");
		
		sIntToStrBuf +="/";
		
		ssIntToStrBuf << iFreeBytes;
		sIntToStrBuf += ssIntToStrBuf.str();
		ssIntToStrBuf.str("");

		sIntToStrBuf +="\n";
	}

	return sIntToStrBuf;
}

DWORDLONG GetResourceUsage::GetRAMUsage()
{
	
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);

	return statex.dwMemoryLoad;
}


float GetResourceUsage::GetCPUUsage()
{
	return mCPUUsage.usage_f();
}