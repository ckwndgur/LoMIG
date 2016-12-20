
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "LogFileView.h"
#include "LogFtView.h"
#include "DFilterView.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
	{
		return FALSE;
	}
	if (!m_wndSplitterDown.CreateStatic(&m_wndSplitter, 1, 2, WS_CHILD | WS_VISIBLE | WS_BORDER,m_wndSplitter.IdFromRowCol(1, 0)))
	{
		return FALSE;
	}

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(LogFileView), CSize(300, 300), pContext);
	m_wndSplitterDown.CreateView(0, 0, RUNTIME_CLASS(LogFtView), CSize(650, 300), pContext);
	m_wndSplitterDown.CreateView(0, 1, RUNTIME_CLASS(DFilterView), CSize(100, 300), pContext);


	return TRUE;
}

CScrollView* CChildFrame::GetFtViewPane()
{
	CWnd* pWnd = m_wndSplitterDown.GetPane(0, 0);
	CScrollView* pView = DYNAMIC_DOWNCAST(CScrollView, pWnd);
	return pView;
}
CScrollView* CChildFrame::GetDFilterViewPane()
{
	CWnd* pWnd = m_wndSplitterDown.GetPane(0, 1);
	CScrollView* pView = DYNAMIC_DOWNCAST(CScrollView, pWnd);
	return pView;
}

CScrollView* CChildFrame::GetFileViewPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CScrollView* pView = DYNAMIC_DOWNCAST(CScrollView, pWnd);
	return pView;
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기


