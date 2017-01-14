// LogFtView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFtView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"

#define R_ERRORITEM	1
#define R_DATEITEM	2
#define R_PATHITEM	3
#define R_LINEITEM	4
#define R_DESITEM	5

// LogFtView

IMPLEMENT_DYNCREATE(LogFtView, CScrollView)

LogFtView::LogFtView()
{
	bCListCnt = false;	
	m_textsize.cx = 0;
	m_textsize.cy = 0;
	btn_flag = false;
}

LogFtView::~LogFtView()
{
}


BEGIN_MESSAGE_MAP(LogFtView, CScrollView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// LogFtView drawing

void LogFtView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;

	CRect crect;
	GetDesktopWindow()->GetWindowRect(crect);
	CSize siz;

	siz.cx = crect.Width();
	siz.cy = crect.Height();

	m_list.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(0, 0, siz.cx, siz.cy - 10) , this, 1234);
	m_list.ShowScrollBar(SB_BOTH);

	m_list.InsertColumn(0, "No.",  LVCFMT_LEFT, 50);
	m_list.InsertColumn(1, "ERROR",  LVCFMT_LEFT, 70);
	m_list.InsertColumn(2, "DATE",  LVCFMT_LEFT, 170);
	m_list.InsertColumn(3, "PATH",  LVCFMT_LEFT, 200);
	m_list.InsertColumn(4, "LINE",  LVCFMT_LEFT, 50);
	m_list.InsertColumn(5, "DESCRIPTION",  LVCFMT_LEFT, 250);

/*
	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(300,0,450,20),this,1233);
	m_EditSearch.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(150,0,300,20), this, 1232);

	m_ComboBox.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, CRect(0,0,150,140), this, 123);
	m_ComboBox.AddString("1. ErrorLevel");
	m_ComboBox.AddString("2. Time");
	m_ComboBox.AddString("3. Path");
	m_ComboBox.AddString("4. LineNumber");
	m_ComboBox.AddString("5. Description");
	m_ComboBox.AddString("6. Total");
*/
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void LogFtView::OnDraw(CDC* pDC)
{
	if (btn_flag)
	{
		ifstream resultfile;
		//resultfile.open(mFilter.MultiResultPath);
		resultfile.open(ExhibitPath);
		if(!resultfile.fail())
		{
			//ResultExhbView(mFilter.MultiResultPath);
			ResultExhbView(ExhibitPath);			
		}
		else
		{

		}

		btn_flag = false;

		/*
		CDocument* pDoc = GetDocument();
		// TODO: add draw code here

		FillFtView(m_strFilteredData, pDC);
		if (!bCListCnt)
		{
			initClistCnt();
		}
		*/
	}

}

void LogFtView::ResultExhbView(CString resultpath)
{
	btn_flag = false;
	m_list.DeleteAllItems();

	ifstream originfile;
	originfile.open(resultpath);

	string alinelog;
	alinelog = "";
	CString lineno;
	int valstart, valend, listno, strlength;
	valstart = 1;
	valend = listno = strlength = 0;
	string liststr_buf;
	CString liststr;//리스트뷰 서브아이템에 들어갈 값입니다.
	liststr = "";

	while(!originfile.eof())
	{
		lineno.Format(_T("%d"), listno+1);
		//m_list.InsertItem(listno, lineno, 0);//리스트뷰 아이템 추가합니다.

		getline(originfile, alinelog);//1개 라인을 스트링으로 읽어옵니다.

		if (alinelog == "")//비어있는 로그 라인일 경우
		{
			m_list.SetItemText(listno, R_ERRORITEM, ""); //에러레벨을 추가합니다.
			m_list.SetItemText(listno, R_DATEITEM, ""); //날짜를 추가합니다.
			m_list.SetItemText(listno, R_PATHITEM, ""); //경로를 추가합니다.
			m_list.SetItemText(listno, R_LINEITEM, ""); //라인을 추가합니다.
			m_list.SetItemText(listno, R_DESITEM, ""); //설명을 추가합니다.
		}
		else
		{
			valend = alinelog.find(":");
			liststr_buf = alinelog.substr(0, valend);
			liststr = liststr_buf.c_str();
			m_list.InsertItem(listno, liststr, 0);//리스트뷰 아이템 추가합니다.
			alinelog.erase(0, valend + 2);

			valend = alinelog.find(")");
			strlength = valend - valstart;
			liststr_buf = alinelog.substr(valstart, strlength);		
			liststr = liststr_buf.c_str();
			m_list.SetItemText(listno, R_ERRORITEM, liststr); //에러레벨을 추가합니다.
			alinelog.erase(0,strlength+2);

			valstart = alinelog.find("[") + 1;
			valend = alinelog.find("]");
			strlength = valend - valstart;
			liststr_buf = alinelog.substr(valstart, strlength);
			liststr = liststr_buf.c_str();
			m_list.SetItemText(listno, R_DATEITEM, liststr); //날짜를 추가합니다.
			alinelog.erase(0,strlength+4);

			if (alinelog.find(".cpp") != -1)
			{
				valstart = 0;
				valend = alinelog.find(".cpp");
				valend = valend  + 4;
				strlength = valend - valstart;
				liststr_buf = alinelog.substr(valstart, strlength);
				liststr = liststr_buf.c_str();
				m_list.SetItemText(listno, R_PATHITEM, liststr); //경로를 추가합니다.
				alinelog.erase(0,strlength+1);
			}

			else if (alinelog.find(".h") != -1)
			{
				valstart = 0;
				valend = alinelog.find(".h");
				valend = valend  + 2;
				strlength = valend - valstart;
				liststr_buf = alinelog.substr(valstart, strlength);
				liststr = liststr_buf.c_str();
				m_list.SetItemText(listno, R_PATHITEM, liststr); //경로를 추가합니다.
				alinelog.erase(0,strlength+1);
			}

			else if (alinelog.find(".cxx") != -1)
			{
				valstart = 0;
				valend = alinelog.find(".cxx");
				valend = valend  + 4;
				strlength = valend - valstart;
				liststr_buf = alinelog.substr(valstart, strlength);
				liststr = liststr_buf.c_str();
				m_list.SetItemText(listno, R_PATHITEM, liststr); //경로를 추가합니다.
				alinelog.erase(0,strlength+1);
			}

			else if (alinelog.find(".c") != -1)
			{
				valstart = 0;
				valend = alinelog.find(".c");
				valend = valend  + 2;
				strlength = valend - valstart;
				liststr_buf = alinelog.substr(valstart, strlength);
				liststr = liststr_buf.c_str();
				m_list.SetItemText(listno, R_PATHITEM, liststr); //경로를 추가합니다.
				alinelog.erase(0,strlength+1);
			}

			else
			{
			}

			valstart = 0; //alinelog.find("]__") + 1;
			valend = alinelog.find("_");
			strlength = valend - valstart;
			liststr_buf = alinelog.substr(valstart, strlength);
			liststr = liststr_buf.c_str();
			m_list.SetItemText(listno, R_LINEITEM, liststr); //라인을 추가합니다.
			alinelog.erase(0,strlength);

			
			if(alinelog == "")
			{
				m_list.SetItemText(listno, R_DESITEM, "NULL"); //설명을 추가합니다.
			}
			else
			{
				if(alinelog.find("_") != -1)
				{
					alinelog.erase(0,1);

					if (alinelog == "")
					{
						m_list.SetItemText(listno, R_DESITEM, "NULL"); //설명을 추가합니다.
					}
					else if (alinelog == " ")
					{
						m_list.SetItemText(listno, R_DESITEM, "NULL"); //설명을 추가합니다.
					}
					else
					{
						liststr = alinelog.c_str();
						m_list.SetItemText(listno, R_DESITEM, liststr); //설명을 추가합니다.
					}
				}

				else
				{
					m_list.SetItemText(listno, R_DESITEM, "NULL"); //설명을 추가합니다.
				}

			}


			alinelog = "";
			valstart = 1;
			valend = 0;
			listno++;
		}
	}

	m_list.DeleteItem(listno);
	listno = 0;	
}

void LogFtView::initClistCnt()
{
	/*
	CRect crect;
	GetDesktopWindow()->GetWindowRect(crect);
	CSize siz;

	siz.cx = crect.Width();
	siz.cy = crect.Height();

	m_list.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(0, 0, siz.cx, siz.cy - 10) , this, 1234);

	// 컬럼 추가
	m_list.InsertColumn(0, "No.", LVCFMT_LEFT, 40);
	m_list.InsertColumn(1, "Error Level", LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, "Date", LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, "Path", LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, "Line number", LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, "Description", LVCFMT_LEFT, 200);

	// 항목 추가
	m_list.InsertItem(0, "1", 0);
	m_list.InsertItem(1, "2", 0);

	// 하위 항목 추가
	m_list.SetItemText(0, 1, "ERROR");
	m_list.SetItemText(0, 2, "20161114");
	m_list.SetItemText(0, 3, "c:\\");
	m_list.SetItemText(0, 4, "1234");
	m_list.SetItemText(0, 5, "test test");
	
	m_list.ShowScrollBar(SB_VERT, 1);
	m_list.ShowScrollBar(SB_HORZ, 1);
	*/

	bCListCnt = true;
}


void LogFtView::SetScrollView(int x, int y)
{
	CSize sizeViewPage;

	sizeViewPage = CalViewSize(x, y);
	
	SetScrollSizes(MM_TEXT, sizeViewPage);

}

CSize LogFtView::CalViewSize(int x, int y)
{
	if ((x==0)&&(y==0))
	{
		CRect rc;
		GetClientRect(&rc);
		CSize sizeViewPage;
		sizeViewPage.cx = rc.right-rc.left;
		sizeViewPage.cy = rc.bottom-rc.top;
		return sizeViewPage;
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
		return sizeViewPage;
	}
	

}

void LogFtView::FillFtView(list<CString> input, CDC* pdc)
{
	int C = 30;
	int i = 0;
	if (input.size() >0 )
	{
		SetScrollView(m_textsize.cx * 8, C + m_textsize.cy*20);
		for (list<CString>::iterator iterPos = input.begin(); iterPos != input.end(); ++iterPos, ++i)
		{
			pdc->TextOut(0, C + i*20, *iterPos);
		}
	}

}


// LogFtView diagnostics

#ifdef _DEBUG
void LogFtView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void LogFtView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogFtView message handlers

void LogFtView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_list)
	{
		CRect rc;
		GetClientRect(&rc);
		m_list.SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
	}
	
	// TODO: Add your message handler code here

}
