#include "StdAfx.h"
#include "TextManager.h"
#include <fstream>
using namespace std;
using std::ifstream;
using std::ofstream;

TextManager::TextManager(void)
{
}

TextManager::~TextManager(void)
{
}

void TextManager::WriteText(char* filedirectory, char* filename, string data, bool lastflag)
{
	ofstream ofile;
	char cTemp[256];

	strcpy(cTemp, mStringManager.AppendChar(filedirectory, filename));

	//cTemp[strlen(cTemp) - 1] = NULL;
	if (!ofile.is_open())
	{
		ofile.open(cTemp, ios_base::out | ios_base::app);
	} 

	ofile<< data;

	if (lastflag)
	{
		ofile.close();
	}
}

void TextManager::WriteText(char* filedirectory, char* filename, string data)
{
	//ofstream ofile;.
	char ctemp[256] ;

	strcpy(ctemp, mStringManager.AppendChar(filedirectory, filename));

	//ctemp[strlen(ctemp) - 1] = NULL;

	ofstream ofile(ctemp, ios_base::out | ios_base::app);
	ofile<< data <<endl;
	ofile.close();
}


void TextManager::MakeDirectory(char* filedirectory, char* filename)
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

string TextManager::ReadText(char* filedirectory, char* filename)
{
	ifstream infile;
	char input[2048];
	string strOutput = "";

	infile.open(mStringManager.AppendChar(filedirectory, filename));

	while(!infile.eof()){
		infile.getline(input, 2048);
		strOutput.append(input);
		strOutput.append("\n");
	}

	infile.close();

	return strOutput;
}

string TextManager::ReadText(char* fulldirectory)
{
	ifstream infile;
	char input[2048];
	string strOutput = "";

	infile.open(fulldirectory);

	while(!infile.eof()){
		infile.getline(input, 2048);
		strOutput.append(input);
		strOutput.append("\n");
	}

	infile.close();

	return strOutput;
}

list<CString> TextManager::ReadTextList(char* fulldirectory)
{
	ifstream infile;
	char input[2048];
	list<CString> strOutput;
	CString buf_input;

	infile.open(fulldirectory);

	while(!infile.eof()){
		infile.getline(input, 2048);
		buf_input = input;
		strOutput.push_front(buf_input);
		
	}
	infile.close();

	return strOutput;
}

int TextManager::GetMaxLineSize(char* fulldirectory)
{
	ifstream infile;
	char input[2048];
	CString buf_input;
	int linesize = 0;
	int maxlinesize = 0;

	infile.open(fulldirectory);

	while(!infile.eof()){
		infile.getline(input, 2048);
		buf_input = input;
		linesize = buf_input.GetLength();

		if (maxlinesize < linesize)
		{
			maxlinesize = linesize;
		}
		
	}

	infile.close();
	return maxlinesize;
}

int TextManager::GetLinelength(char* fulldirectory)
{
	ifstream infile;
	char input[2048];
	int linelength = 0;

	infile.open(fulldirectory);

	while(!infile.eof()){
		infile.getline(input, 2048);
		linelength++;
	}

	infile.close();
	return linelength;
}

int TextManager::GetFileSize(char* filedirectory, char* filename){
	FILE *fpMe;
	long len;

	if((fpMe = fopen(mStringManager.AppendChar(filedirectory, filename),"rb")) == NULL)
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