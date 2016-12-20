// LogFileView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "stdlib.h"
#include "string.h"
#include "DFilterView.h"


#define ERRORITEM	1
#define DATEITEM	2
#define PATHITEM	3
#define LINEITEM	4
#define DESITEM		5


// LogFileView

IMPLEMENT_DYNCREATE(LogFileView, CScrollView)

LogFileView::LogFileView()
{

}

LogFileView::~LogFileView()
{
}

BEGIN_MESSAGE_MAP(LogFileView, CScrollView)
	ON_WM_SIZE()
	ON_COMMAND(ID_Log_Req, &CClassView::OnLogReq)
	ON_COMMAND(ID_Info_Req, &CClassView::OnInfoReq)
	ON_COMMAND(ID_Info_Load, &CClassView::OnInfoLoad)
END_MESSAGE_MAP()

// LogFileView drawing

void LogFileView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//ListView추가
	m_OriginLoglist.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
						   CRect(0, 0, 1000, 100), this, 1234);
    m_OriginLoglist.ShowScrollBar(SB_BOTH);

	/*
	//컬럼 추가
	m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
	m_OriginLoglist.InsertColumn(1, "Error Level", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(2, "Date", LVCFMT_LEFT, 300);
	m_OriginLoglist.InsertColumn(3, "Path", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(4, "Line number", LVCFMT_LEFT, 100);
	m_OriginLoglist.InsertColumn(5, "Description", LVCFMT_LEFT, 200);
*/
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	
	SetScrollView(0, 0);
}

void LogFileView::SetScrollView(int x, int y)
{
	if ((x==0)&&(y==0))
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;
		sizeViewPage.cx = rc.right-rc.left;
		sizeViewPage.cy = rc.bottom-rc.top;
		SetScrollSizes(MM_TEXT, sizeViewPage);
	} 
	else
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;

		int check_x = rc.right - rc.left;
		if (x < check_x)
		{
			sizeViewPage.cx = check_x;
		}
		else
		{
			sizeViewPage.cx = x;
		}

		int check_y = rc.bottom - rc.top;
		if (y < check_y)
		{
			sizeViewPage.cy = check_y;
		}
		else
		{
			sizeViewPage.cy = y;
		}


		SetScrollSizes(MM_TEXT, sizeViewPage);
	}

}

void LogFileView::OnDraw(CDC* pDC)
{
	/*
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if (m_strView.size() >0)
	{
		//int C = 40;
		int i = 0;

		SetScrollView(m_textsize.cx * 8, m_textsize.cy*20);
		//text size * 8 -> wnd x size
		//first line height + total line number * each line height
		for (list<CString>::iterator iterPos = m_strView.begin(); iterPos != m_strView.end(); ++iterPos, ++i)
		{

			pDC->TextOut(0, i*20, *iterPos);
		}
	}
	*/

	if(m_openflag == TRUE)
	{
		LogtoList(openfilepath);
		//DLogtoList(openfilepath);
	}

	m_openflag = FALSE;
}

BOOL LogFileView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
// 	if(wParam == 1233){
// 
// 		int Category = m_ComboBox.GetCurSel() + 1;
// 		CString WantedLog1;
// 		GetDlgItemTextA(1232,WantedLog1);
// 		CT2CA pszConvertedAnsiString(WantedLog1);
// 		string WantedLog(pszConvertedAnsiString);
// 
// 		string Title=mFilter.CreatingTime(WantedLog);
// 		m_strFilteredData = mFilter.DoFilter(Category, WantedLog, Title, m_strViewPath);
// 
// 	}
	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지

	return CView::OnCommand(wParam, lParam);
}

//윈도우의 리스트뷰에 로그파일 오픈
void LogFileView::DLogtoList(CString filepath)
{
	ifstream originfile(filepath); //지정된 경로의 로그파일을 열어 저장합니다.
	string alinelog, alinelog_buf;
	alinelog = alinelog_buf = "";
	CString lineno;
	int valstart, valend, listno, strlength, columnno;
	valstart = 1;
	valend = listno = strlength = columnno = 0;
	string liststr_buf;
	CString liststr;//리스트뷰 서브아이템에 들어갈 값입니다.
	liststr = "";
	bool inputflag;
	inputflag = true;

	Filter *pfilter = new Filter;
	pfilter->LogColumn  = "";
	//DFilterView *pDfilter  = new DFilterView;
	//pDfilter->filterform = "";

	while(!originfile.eof())
	{
		getline(originfile, alinelog);//1개 라인을 스트링으로 읽어옵니다.
		alinelog_buf = alinelog;
		inputflag = true;
		columnno = 0;

		//리스트뷰의 컬럼과 첫번째 아이템을 생성합니다.
		if(listno == 0)
		{
			m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
			lineno.Format(_T("%d"), listno+1);
			m_OriginLoglist.InsertItem(listno, lineno, 0);

			while(inputflag)
			{
				valstart = alinelog_buf.find("《");
				valend = alinelog_buf.find("》");
				strlength = valend - valstart;
				liststr_buf = alinelog_buf.substr(valstart+2, strlength-2);
				liststr = liststr_buf.c_str();
				m_OriginLoglist.InsertColumn(columnno+1, liststr, LVCFMT_LEFT, 150);

				pfilter->LogColumn  += "*";
				pfilter->LogColumn  += liststr;

				//pDfilter->filterform += "*";
				//pDfilter->filterform += liststr;

				alinelog_buf.erase(valstart,strlength+2);
				valstart = alinelog_buf.find("《");

				liststr_buf = alinelog_buf.substr(0, valstart);
				liststr = liststr_buf.c_str();
				
				m_OriginLoglist.SetItemText(listno, columnno+1, liststr);//서브아이템을 추가합니다.				
				alinelog_buf.erase(0,valstart);
				
				valstart = alinelog_buf.find("《");

				if(valstart == -1)
				{
					inputflag = false;
				}

				else
				{
				}

				columnno++;
			}

			listno++;

		}
		
		//리스트뷰의 나머지 아이템을 추가합니다.
		else
		{
			//m_OriginLoglist.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
			lineno.Format(_T("%d"), listno+1);
			m_OriginLoglist.InsertItem(listno, lineno, 0);

			while(inputflag)
			{
				valstart = alinelog_buf.find("《");
				valend = alinelog_buf.find("》");
				strlength = valend - valstart;
				liststr_buf = alinelog_buf.substr(valstart+2, strlength-2);
				liststr = liststr_buf.c_str();
				//m_OriginLoglist.InsertColumn(columnno+1, liststr, LVCFMT_LEFT, 150);

				alinelog_buf.erase(valstart,strlength+2);
				valstart = alinelog_buf.find("《");

				liststr_buf = alinelog_buf.substr(0, valstart);
				liststr = liststr_buf.c_str();
				
				m_OriginLoglist.SetItemText(listno, columnno+1, liststr);//서브아이템을 추가합니다.
				alinelog_buf.erase(0,valstart);
				
				valstart = alinelog_buf.find("《");

				if(valstart == -1)
				{
					inputflag = false;
				}

				else
				{
				}

				columnno++;
			}

			listno++;
		}
	}

	listno = 0;
}


//로그형태
/*
(TEST)
[150611_11h43m23s154ms]_
_c:\stc_dds\include\csds\csds.h_
734_
[ RTDBDomain ] TRACKDB_ALL_E on_sample_lost and total count =7
*/
void LogFileView::LogtoList(CString filepath)
{
	ifstream originfile(filepath); //지정된 경로의 로그파일을 열어 저장합니다.
	string alinelog;
	alinelog = "";
	CString lineno;
	int valstart, valend, listno, strlength;
	valstart = 1;
	valend = listno = strlength = 0;
	string liststr_buf;
	CString liststr;//리스트뷰 서브아이템에 들어갈 값입니다.
	liststr = "";

	m_OriginLoglist.InsertColumn(0, "No.",  LVCFMT_LEFT, 50);
	m_OriginLoglist.InsertColumn(1, "ERROR",  LVCFMT_LEFT, 70);
	m_OriginLoglist.InsertColumn(2, "DATE",  LVCFMT_LEFT, 170);
	m_OriginLoglist.InsertColumn(3, "PATH",  LVCFMT_LEFT, 200);
	m_OriginLoglist.InsertColumn(4, "LINE",  LVCFMT_LEFT, 50);
	m_OriginLoglist.InsertColumn(5, "DESCRIPTION",  LVCFMT_LEFT, 250);

	while(!originfile.eof())
	{
		lineno.Format(_T("%d"), listno+1);
		m_OriginLoglist.InsertItem(listno, lineno, 0);//리스트뷰 아이템 추가합니다.

		getline(originfile, alinelog);//1개 라인을 스트링으로 읽어옵니다.

		valend = alinelog.find(")");
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);		
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, ERRORITEM, liststr); //에러레벨을 추가합니다.
		alinelog.erase(0,strlength+2);

		valstart = alinelog.find("[") + 1;
		valend = alinelog.find("]");
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, DATEITEM, liststr); //날짜를 추가합니다.
		alinelog.erase(0,strlength+4);

		valstart = 0;
		valend = alinelog.find(".");
		valend = valend  + 2;
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, PATHITEM, liststr); //경로를 추가합니다.
		alinelog.erase(0,strlength+1);

		valstart = 0; //alinelog.find("]__") + 1;
		valend = alinelog.find("_");
		strlength = valend - valstart;
		liststr_buf = alinelog.substr(valstart, strlength);
		liststr = liststr_buf.c_str();
		m_OriginLoglist.SetItemText(listno, LINEITEM, liststr); //라인을 추가합니다.
		alinelog.erase(0,strlength+1);

		liststr = alinelog.c_str();
		m_OriginLoglist.SetItemText(listno, DESITEM, liststr); //설명을 추가합니다.
		
		alinelog = "";
		valstart = 1;
		valend = 0;
		listno++;
	}

	listno = 0;
}

//리스트뷰의 아이템을 받아옵니다.
void ListtoLog()
{
	
}



// LogFileView diagnostics

#ifdef _DEBUG
void LogFileView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void LogFileView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogFileView message handlers

void LogFileView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_OriginLoglist)
	{
		CRect rc;
		GetClientRect(&rc);
		m_OriginLoglist.SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
	}
	// TODO: Add your message handler code here
}
