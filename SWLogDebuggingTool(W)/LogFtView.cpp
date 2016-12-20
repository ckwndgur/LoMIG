// LogFtView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFtView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"


// LogFtView

IMPLEMENT_DYNCREATE(LogFtView, CScrollView)

LogFtView::LogFtView()
{
	bCListCnt = false;	
	m_textsize.cx = 0;
	m_textsize.cy = 0;
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
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	FillFtView(m_strFilteredData, pDC);
	if (!bCListCnt)
	{
		initClistCnt();
	}

	
}

void LogFtView::initClistCnt()
{
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
/*
	// 항목 추가
	m_list.InsertItem(0, "1", 0);
	m_list.InsertItem(1, "2", 0);

	// 하위 항목 추가
	m_list.SetItemText(0, 1, "ERROR");
	m_list.SetItemText(0, 2, "20161114");
	m_list.SetItemText(0, 3, "c:\\");
	m_list.SetItemText(0, 4, "1234");
	m_list.SetItemText(0, 5, "test test");
	*/
	m_list.ShowScrollBar(SB_VERT, 1);
	m_list.ShowScrollBar(SB_HORZ, 1);

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
