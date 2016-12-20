
#include "stdafx.h"

#include "FilterView.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W).h"
#include "Filter.h"

#define ERRORSTR	0
#define DATESTR		1
#define PATHSTR		2
#define LINESTR		3
#define DESSTR		4
#define TOTALSTR	5

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CFilterView

CFilterWnd::CFilterWnd()
{
}

CFilterWnd::~CFilterWnd()
{
}

BEGIN_MESSAGE_MAP(CFilterWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILTERING, ProcessFilter)
	
	/*
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllFilter)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllFilter)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortFilter)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortFilter)
	ON_COMMAND(ID_PROPERTIES1, OnFilter1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateFilter1)
	ON_COMMAND(ID_PROPERTIES2, OnFilter2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateFilter2)
	*/
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	END_MESSAGE_MAP()

void CFilterWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFilterList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CFilterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	CRect rect1(0, 0, 10, 10);
	

	// 콤보 상자를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rect1, this, 1))
	{
		TRACE0("사용자정의 필터를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndObjectCombo.AddString(_T("정형로그필터"));
	m_wndObjectCombo.AddString(_T("비정형로그필터"));
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndFilterList.Create(WS_VISIBLE | WS_CHILD, rect1, this, 2))
	{
		TRACE0("필터 뷰를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	InitFilterList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_FILTER);
	m_wndToolBar.LoadToolBar(IDR_FILTER, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_FILTER, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}


void CFilterWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFilterWnd::OnExpandAllFilter()
{
	m_wndFilterList.ExpandAll();
}

void CFilterWnd::OnUpdateExpandAllFilter(CCmdUI* pCmdUI)
{
}

void CFilterWnd::OnSortFilter()
{
	m_wndFilterList.SetAlphabeticMode(!m_wndFilterList.IsAlphabeticMode());
}

void CFilterWnd::OnUpdateSortFilter(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndFilterList.IsAlphabeticMode());
}

void CFilterWnd::OnFilter1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFilterWnd::OnUpdateFilter1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	m_wndFilterList.EnableHeaderCtrl(FALSE);
	m_wndFilterList.EnableDescriptionArea();
	m_wndFilterList.SetVSDotNetLook();
	m_wndFilterList.MarkModifiedProperties();

	int startno;
	bool rowflag;
	CString rowname;

	startno = 0;
	rowflag  =  true;
	rowname = "";

	Filter *pfilter = new Filter;
	rowname = pfilter->LogColumn;
	CMFCPropertyGridProperty* pGroupfilter = new CMFCPropertyGridProperty(_T("Filter Property and String"));

	while(rowflag)
	{
		CMFCPropertyGridProperty* pDateProp = new CMFCPropertyGridProperty(_T("from"), (_variant_t) _T(""), _T("YYYY-MM-DD_hh:mm:ss"));
		pGroupfilter->AddSubItem(pDateProp);
	}

	m_wndFilterList.AddProperty(pGroupfilter);
	
}

void CFilterWnd::OnFilter2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

}

void CFilterWnd::OnUpdateFilter2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

//필터링 버튼 클릭 이벤트 시 실행
void CFilterWnd::ProcessFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	//필터 윈도우에 입력된 문자열을 받아옵니다.
	CString errorstr;
	errorstr = GetSearchString(ERRORSTR);//에러레벨 필터링 값

	CString datestr;
	datestr  = GetSearchString(DATESTR);//날짜 필터링 값

	CString pathstr;
	pathstr = GetSearchString(PATHSTR);//경로 필터링 값

	CString linestr;
	linestr = GetSearchString(LINESTR);//라인 필터링 값

	CString descriptionstr;
	descriptionstr = GetSearchString(DESSTR);//설명 필터링 값

	CString totalstr;
	totalstr = GetSearchString(TOTALSTR);//문자열 필터링 값
}

CString CFilterWnd::GetSearchString(/*필터 카테고리*/int search_type)
{
	//필터링 문자열 값을 얻어옵니다.
	CMFCPropertyGridProperty *filterdata_buf = m_wndFilterList.GetProperty(search_type);
	CMFCPropertyGridProperty *filterdata;
	filterdata = filterdata_buf->GetSubItem(0);
	COleVariant vars= filterdata->GetValue();
	wchar_t* strs = vars.bstrVal;
	CString searchstr;
	searchstr =  CString(strs);

	return searchstr;
}


void CFilterWnd::InitFilterList()
{
	//출력 될 필터 코드를 추가합니다.

	SetFilterListFont();

	m_wndFilterList.EnableHeaderCtrl(FALSE);
	m_wndFilterList.EnableDescriptionArea();
	m_wndFilterList.SetVSDotNetLook();
	m_wndFilterList.MarkModifiedProperties();

	
/*
	//에러레벨 검색
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Error Level"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Error"), (_variant_t)_T(""), _T("ERROR")));
	m_wndFilterList.AddProperty(pGroup1);

	//날짜 검색
	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Date"));
	//pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("from"), (_variant_t)_T(""), _T("YYYY-MM-DD_hh:mm:ss")));
	//pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("to"), (_variant_t)_T(""), _T("YYYY-MM-DD_hh:mm:ss")));
	
	CMFCPropertyGridProperty* pDateProp1 = new CMFCPropertyGridProperty(_T("from"), (_variant_t) _T(""), _T("YYYY-MM-DD_hh:mm:ss"));
	
	CMFCPropertyGridProperty* pDateProp2 = new CMFCPropertyGridProperty(_T("to"), (_variant_t) _T(""), _T("YYYY-MM-DD_hh:mm:ss"));
	
	pGroup2->AddSubItem(pDateProp1);
	pGroup2->AddSubItem(pDateProp2);
	m_wndFilterList.AddProperty(pGroup2);

	//파일경로 검색
	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("File Path"));
	pGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("Path"), (_variant_t)_T(""), _T("c:\\")));
	m_wndFilterList.AddProperty(pGroup3);

	//라인넘버검색
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Line Number"));
	pGroup4->AddSubItem(new CMFCPropertyGridProperty(_T("No."), (_variant_t)_T(""), _T("1988")));
	m_wndFilterList.AddProperty(pGroup4);

	//주석부분 검색
	CMFCPropertyGridProperty* pGroup5 = new CMFCPropertyGridProperty(_T("Description"));
	pGroup5->AddSubItem(new CMFCPropertyGridProperty(_T("Des"), (_variant_t)_T(""), _T("This eorror from ")));
	m_wndFilterList.AddProperty(pGroup5);

	//스트링기반 검색
	CMFCPropertyGridProperty* pGroup6 = new CMFCPropertyGridProperty(_T("Total"));
	pGroup6->AddSubItem(new CMFCPropertyGridProperty(_T("String"), (_variant_t)_T(""), _T("&와 | 연산을 사용가능합니다.")));
	m_wndFilterList.AddProperty(pGroup6);

	CMFCPropertyGridProperty* filterbtn = new CMFCPropertyGridProperty(_T("Filtering"));
	
*/
}

void CFilterWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndFilterList.SetFocus();
}

void CFilterWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetFilterListFont();
}

void CFilterWnd::SetFilterListFont()
{
	::DeleteObject(m_fntFilterList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntFilterList.CreateFontIndirect(&lf);

	m_wndFilterList.SetFont(&m_fntFilterList);
}