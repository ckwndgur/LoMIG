#include "TextManager.h"

using namespace std;


TextManager::TextManager(void)
{
}
TextManager::~TextManager(void)
{
}

void TextManager::WriteText(char* filedirectory, char* filename, string data)
{
	ofstream ofile;
	char cTemp[256] ;

	strcpy(cTemp, AppendChar(filedirectory, filename));

	cTemp[strlen(cTemp) - 1] = NULL;

	ofile.open(cTemp, ios_base::out | ios_base::app);
	ofile<< data <<endl;
	ofile.close();
}

char* TextManager::AppendChar(char* arg1, char* arg2)
{

	char *concat = (char *)malloc(strlen(arg1) + strlen(arg2) + 1);
	strcpy(concat, arg1);
	strcat(concat, arg2);

	return concat;
}


void TextManager::MakeDirectory(char* filedirectory, char* filename)
{
	char cTemp[256], *sp;
	strcpy(cTemp, AppendChar(filedirectory, filename));
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

string TextManager::ReadText(char* filedirectory, char* filename, int &iRdLen)
{
	ifstream infile;
	char input[2048];
	string strOutput = "";
	
	infile.open(AppendChar(filedirectory, filename));

	while(!infile.eof())
	{
		infile.getline(input, 2048);
		strOutput.append(input);
		strOutput.append("\n");
	}

	iRdLen = strlen(strOutput.c_str());

	infile.close();

	return strOutput;
}

int TextManager::GetFileSize(char* filedirectory, char* filename){
	FILE *fpMe;
	long len;

	if((fpMe = fopen(AppendChar(filedirectory, filename),"rb")) == NULL)
	{
		printf("");
	}
	else
	{
		printf("s");
	}

	fseek(fpMe, 0L, SEEK_END);

	len = ftell(fpMe);
	printf("%d byte\n",len);
	fclose(fpMe);
	
	return (int)len;
}

