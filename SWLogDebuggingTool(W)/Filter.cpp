//////////////////////////////////////////////////////////////////////////////////////////////싹다 복붙
#include "StdAfx.h"
#include "Filter.h"
#include "FolderManager.h"
#include "stdlib.h"
#include "string.h"

Filter::Filter()
{
}

Filter::~Filter()
{
}



string Filter::CreatingTime(string WantedLog)
{
	SYSTEMTIME lst;
	GetLocalTime(&lst);
	char buffer[40];
	sprintf_s(buffer,sizeof(buffer),"%04d-%02d-%02d %02dhour%02dmin%02dsec.txt", lst.wYear, lst.wMonth, lst.wDay, lst.wHour, lst.wMinute, lst.wSecond);
	string Title1;
	Title1 = buffer;
	Title1 = WantedLog + Title1;
	//Title1 = "["+WantedLog + "] "+ Title1;
	char* Title = new char[Title1.size() +1];
	strcpy_s(Title, Title1.size() + 1, Title1.c_str());

	return Title;
}
//list<string> keywords = [1] category [2] keyword
//string Title = [keyword]
//CString originpath = 검색 실행 할 원본로그주소
//CString newpath = 검색 결과 저장 할 주소
//bool multiflag = 다중필터링 확인
//list<CString> Filter::MultiFilter(list<string> keywords, string Title, CString originpath, CString newpath, bool multiflag)
list<CString> Filter::MultiFilter(int cate, string keyword, string Title, CString originpath, CString newpath, bool multiflag)
{
	//ifstream originfile(originpath); //원본로그파일을 열어 저장합니다.
	ifstream originfile;
	originfile.open(originpath);//원본로그파일을 열어 저장합니다.


	ofstream resultfile(newpath, ios::out);

	string alinelog, cateBuf, keywordBuf, resultlog, devidedlog;
	alinelog = cateBuf =  keywordBuf = resultlog = devidedlog = "";
	CString linenumberBuf;
	linenumberBuf = "";
	int category, valStart, valEnd, linenumber;
	category = valStart = valEnd = linenumber = 0;

	list<CString> FilteredLog;

//	else
//	{

		while (!originfile.eof())
		{
			getline(originfile, alinelog); //원본로그파일을 한줄 씩 받아옵니다.
			
			category = cate;
			keywordBuf = keyword;
			if(alinelog == "")
			{
				linenumber = linenumber + 1; //로그의 라인 넘버를 증가시킵니다.
			}
			/*
			cateBuf = keywords.front();
			category = atoi(cateBuf.c_str()); //카테고리를 받아옵니다.
			keywords.pop_front();
			keywordBuf = keywords.front(); //키워드를 받아옵니다.
			*/
			
			else
			{
				//카테고리 기반으로 로그를 분리합니다.
				if (multiflag == false)//다중 필터링이 아닐경우
				{
					linenumber = linenumber + 1; //로그의 라인 넘버를 증가시킵니다.

					linenumberBuf.Format(_T("%d"), linenumber); //int -> CString

					CT2CA AnsiStringlinenumber(linenumberBuf); //CString -> string
					string resultlogbuf(AnsiStringlinenumber);
					resultlog = "";
					resultlog = resultlogbuf + ": "; //결과로그의 각 줄 첫부분에 원본로그의 라인넘버를 입력합니다.

					devidedlog = LogDivider(category, alinelog);
					
					alinelog = resultlog + alinelog;
				} 

				else//다중 필터링일 경우
				{
					string buflog =  "";
					buflog = alinelog;
					valEnd  = buflog.find(": ");
					resultlog = buflog.substr(0, valEnd);
					buflog.erase(0,valEnd+2);

					devidedlog = LogDivider(category, buflog);
				}

				if (category == 6) //카테고리가 Total일 경우
				{
					if (keywordBuf.find("&") != -1) //&연산자 이용 검색
					{
						string firstand, secondand;
						firstand = secondand = "";
						int andcnt = 0;

						andcnt = keywordBuf.find("&");
						firstand = keywordBuf.substr(0, andcnt);
						keywordBuf.erase(0, andcnt +1);
						secondand =  keywordBuf;

						if (devidedlog.find(firstand) != -1) //첫번째 검색인자 확인
						{
							if (devidedlog.find(secondand) != -1) //두번째 검색인자 확인
							{
								resultlog = alinelog; //결과에 해당 로그를 저장합니다.
								FilteredLog.push_back(resultlog.c_str());
								resultfile << resultlog << endl;
							}
							else
							{
							
							}
						}
						else
						{
						}
					}
					else if (keywordBuf.find("|") != -1) //|연산자 이용 검색
					{
						string firstand, secondand;
						firstand = secondand = "";
						int andcnt = 0;

						andcnt = keywordBuf.find("|");
						firstand = keywordBuf.substr(0, andcnt);
						keywordBuf.erase(0, andcnt +1);
						secondand =  keywordBuf;

						if (devidedlog.find(firstand) != -1) //첫번째 검색인자 확인
						{
							resultlog = alinelog; //결과에 해당 로그를 저장합니다.
							FilteredLog.push_back(resultlog.c_str());
							resultfile << resultlog << endl;	
						}
						else if (devidedlog.find(secondand) != -1) //두번째 검색인자 확인
						{
							resultlog = alinelog; //결과에 해당 로그를 저장합니다.
							FilteredLog.push_back(resultlog.c_str());
							resultfile << resultlog << endl;
						}
						else
						{

						}
					}
				}
				else
				{

					//키워드가 해당 로그에 존재하는지 검사합니다.
					if (devidedlog.find(keywordBuf) != -1)
					{
						resultlog = alinelog; //결과에 해당 로그를 저장합니다.
						FilteredLog.push_back(resultlog.c_str());
						resultfile << resultlog << endl;
					} 
					else
					{

					}

				}
			}
			
		//}
	}

	return FilteredLog;
}

string Filter::LogDivider(int category, string alinelog)
{
	int valstart, valend, vallength;
	valstart = valend = vallength = 0;
	string valrtnLog;
	
	switch(category)
	{
	case 1 : //Error Level
		valend = alinelog.find(")");
		valrtnLog = alinelog.substr(1,valend);
		break;

	case 2 : //Date
		valstart = alinelog.find(")[");
		valend = alinelog.find("]__");
		vallength = valend - valstart;
		valrtnLog = alinelog.substr(valstart, vallength);
		break;

	case 3 : //Path
		valstart = alinelog.find("]__") + 3;

		if (alinelog.find(".cpp") != -1)
		{
			valend = alinelog.find(".cpp") + 4;
			vallength = valend - valstart;
			valrtnLog = alinelog.substr(valstart, vallength);
		}

		else if (alinelog.find(".h") != -1)
		{
			valend = alinelog.find(".h") + 2;
			vallength = valend - valstart;
			valrtnLog = alinelog.substr(valstart, vallength);
		}

		else if (alinelog.find(".cxx") != -1)
		{
			valend = alinelog.find(".cxx") + 4;
			vallength = valend - valstart;
			valrtnLog = alinelog.substr(valstart, vallength);
		}

		else if (alinelog.find(".c") != -1)
		{
			valend = alinelog.find(".c") + 2;
			vallength = valend - valstart;
			valrtnLog = alinelog.substr(valstart, vallength);
		}

		else
		{
			valend = alinelog.find(".") + 3;
			valend = valend + 1;
			vallength = valend - valstart;
			valrtnLog = alinelog.substr(valstart, vallength);
		}
		
		break;

	case 4 : //Line
		valstart = alinelog.find("]__");
		alinelog.erase(0, valstart);
		
		if (alinelog.find(".cpp") != -1)
		{
			valstart = alinelog.find(".cpp") + 5;			
			alinelog.erase(0,valstart);
			valend = alinelog.find("_");
			vallength = valend;
			valrtnLog = alinelog.substr(0, valend);
		}

		else if (alinelog.find(".h") != -1)
		{
			valstart = alinelog.find(".h") + 3;			
			alinelog.erase(0,valstart);
			valend = alinelog.find("_");
			vallength = valend;
			valrtnLog = alinelog.substr(0, valend);
		}

		else if (alinelog.find(".cxx") != -1)
		{
			valstart = alinelog.find(".cxx") + 5;			
			alinelog.erase(0,valstart);
			valend = alinelog.find("_");
			vallength = valend;
			valrtnLog = alinelog.substr(0, valend);
		}

		else if (alinelog.find(".c") != -1)
		{
			valstart = alinelog.find(".c") + 3;	
			alinelog.erase(0,valstart);
			valend = alinelog.find("_");
			vallength = valend;
			valrtnLog = alinelog.substr(0, valend);
		}

		else
		{
			valend = alinelog.find(".") + 4;
			alinelog.erase(0,valstart);
			valend = alinelog.find("_");
			vallength = valend;
			valrtnLog = alinelog.substr(0, valend);
		}

		break;

	case 5 : //Description
		valstart = alinelog.find("]__");
		alinelog.erase(0, valstart);

		if (alinelog.find(".cpp") != -1)
		{
			valstart = alinelog.find(".cpp") + 1;			
			alinelog.erase(0,valstart);
			valstart = alinelog.find("_");
			alinelog.erase(0,valstart);
			valrtnLog = alinelog;
		}

		else if (alinelog.find(".h") != -1)
		{
			valstart = alinelog.find(".h") + 1;			
			alinelog.erase(0,valstart);
			valstart = alinelog.find("_");
			alinelog.erase(0,valstart);
			valrtnLog = alinelog;
		}

		else if (alinelog.find(".cxx") != -1)
		{
			valstart = alinelog.find(".cxx") + 1;			
			alinelog.erase(0,valstart);
			valstart = alinelog.find("_");
			alinelog.erase(0,valstart);
			valrtnLog = alinelog;
		}

		else if (alinelog.find(".c") != -1)
		{
			valstart = alinelog.find(".c") + 1;	
			alinelog.erase(0,valstart);
			valstart = alinelog.find("_");
			alinelog.erase(0,valstart);
			valrtnLog = alinelog;
		}

		else
		{
			valend = alinelog.find(".") + 2;
			alinelog.erase(0,valstart);
			valstart = alinelog.find("_");
			alinelog.erase(0,valstart);
			valrtnLog = alinelog;
		}
		break;

	case 6 : //Total
		valrtnLog =  alinelog;
		break;

	default : AfxMessageBox(TEXT("카테고리 범주가 아닙니다."));
		break;
	}
	return valrtnLog;
}

list<CString> Filter::DoFilter(int Category, string WantedLog, string Title, CString filepath, CString Rfilepath, bool multiflag)
{
	string Log;
	string FilteredLog;
	string TargetPart;
	string WantedLogPart[2];
	char divider;
	if(WantedLog.find("&"))
	{
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("&"));
		WantedLogPart[1].assign(WantedLog,WantedLog.find("&") + 1, WantedLog.length());
		divider = '&';
	}
	if(WantedLog.find("|"))
	{
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("|"));
		WantedLogPart[1].assign(WantedLog,WantedLog.find("|") + 1, WantedLog.length());
		divider = '|';
	}

	CString csfilepath, temp = ""; 
	csfilepath = Rfilepath;
/*
	if(multiflag == false)
	{
		
		for (int i = 0; i<4; i++)
		{
			AfxExtractSubString(temp, filepath, i, '\\');
			csfilepath += temp + '\\';
		}
		AfxExtractSubString(temp, filepath, 4, '\\');
		AfxExtractSubString(temp, temp, 0, '.');

		csfilepath = csfilepath + "Debug" + '\\' + "(" + temp + ")" + Title.c_str();
		ResultPath = csfilepath;		
		FolderManager mFolderManager;
		mFolderManager.MakeDirectory((LPSTR)((LPCTSTR)csfilepath));

		ifstream input((CStringA)filepath, ios::in); // 
		if(input.fail()){
			cout << "파일을 여는 데 실패했습니다."<< endl;
		}

		ofstream output(csfilepath, ios::out); 
		if(output.fail()){
			cout << "파일을 쓰는 데 실패했습니다.1"<< endl;
		}
	}
	else
	{
		csfilepath = Rfilepath;
	}
*/
	ifstream input((CStringA)filepath, ios::in); // 
	if(input.fail()){
		cout << "파일을 여는 데 실패했습니다."<< endl;
	}

	ofstream output(csfilepath, ios::out); 
	if(output.fail()){
		cout << "파일을 쓰는 데 실패했습니다."<< endl;
	}
	int LogPartStartPoint= 0, LogPartEndPoint =0;
	
	if((Category == 2)){
		WantedLogPart[0].assign(WantedLog,0, WantedLog.find("~"));
		WantedLogPart[1].assign(WantedLog, WantedLog.find("~")+1, WantedLog.length());
	}
	while(!input.eof())
	{					
		getline(input, Log);
		switch(Category)
		{
		case 1 :
			LogPartStartPoint = 1;
			LogPartEndPoint = Log.find(")");
			break;
		case 2 :
			LogPartStartPoint = Log.find("[")+1;
			LogPartEndPoint = Log.find("_");
			break;
		case 3 :
			LogPartStartPoint = Log.find("]__") +3;
			LogPartEndPoint = Log.find('.',LogPartStartPoint);
			do
			{
				LogPartEndPoint++;
			}while(Log.at(LogPartEndPoint) != '_');
			break;
		case 4 :
			LogPartStartPoint = Log.find('.');
			do{
				do{
					LogPartStartPoint++;
				}while(Log.at(LogPartStartPoint) != '_');
				LogPartEndPoint = LogPartStartPoint;
				do{
					LogPartEndPoint++;
				}while(Log.at(LogPartEndPoint) != '_');
			}while( !((LogPartEndPoint - LogPartStartPoint >= 1) & (LogPartEndPoint - LogPartStartPoint <= 8)));
			break;
		case 5 :
			LogPartStartPoint = Log.find('.');
			do{
				do{
					LogPartStartPoint++;
				}while(Log.at(LogPartStartPoint) != '_');
				LogPartEndPoint = LogPartStartPoint;
				do{
					LogPartEndPoint++;
				}while(Log.at(LogPartEndPoint) != '_');
			}while( !((LogPartEndPoint - LogPartStartPoint >= 1) & (LogPartEndPoint - LogPartStartPoint <= 8)));
			LogPartStartPoint = LogPartEndPoint+1;
			LogPartEndPoint = Log.length();
			break;
		case 6 :
			LogPartStartPoint = 0;
			LogPartEndPoint = Log.length();
			break;
		default : AfxMessageBox(TEXT("카테고리 범주가 아닙니다."));
			break;
		}
		TargetPart.assign(Log, LogPartStartPoint, LogPartEndPoint-LogPartStartPoint);
		if(Category == 2){
			if((TargetPart>=WantedLogPart[0]) & (TargetPart<=WantedLogPart[1])){
				FilteredLog = Log;
				output << FilteredLog << endl;
				cslstFilteredData.push_front(FilteredLog.c_str());
			}
		}
		else{
			if(divider == '&')
			{
				if(((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())) & ((TargetPart.find(WantedLogPart[1]) >=0) & (TargetPart.find(WantedLogPart[1]) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
			else if(divider == '|')
			{
				if(((TargetPart.find(WantedLogPart[0]) >=0) & (TargetPart.find(WantedLogPart[0]) < TargetPart.length())) | ((TargetPart.find(WantedLogPart[1]) >=0) & (TargetPart.find(WantedLogPart[1]) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
			else
			{
				if(((TargetPart.find(WantedLog) >=0) & (TargetPart.find(WantedLog) < TargetPart.length())))
				{
					FilteredLog = Log;
					output << FilteredLog << endl;
					cslstFilteredData.push_front(FilteredLog.c_str());
				}
			}
		}
	}
	char devider = '0';
	input.close();
	output.close();

	return cslstFilteredData;
}
//(입력방법 : YYMMDD_hhmmssmsmsms~YYMMDD_hhmmssmsmsms"