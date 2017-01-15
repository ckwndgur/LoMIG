

#include "stdafx.h"
#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W).h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	bCheckInfo = 0;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

void CPropertiesWnd::AdjustLayout()
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
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 콤보 상자를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("속성 콤보 상자를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndObjectCombo.AddString(_T("응용 프로그램"));
	m_wndObjectCombo.AddString(_T("속성 창"));
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	
	RefreshAW();
	//SetAgentP(false);
	//SetWatcherP(false);

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}



void CPropertiesWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDockablePane::OnPaint() for painting messages

// 	if (bAgentInfo)
// 	{
// 		SetAgentP();
// 	} 
// 	if (bWatcherInfo)
// 	{
// 		SetWatcherP();
// 	}
// 	bAgentInfo = false;
// 	bWatcherInfo = false;
	
	switch(bCheckInfo)
	{
	case 1 :
		RefreshAW();
		break;
	case 2 :
		SetAgentP();
		break;
	case 3 :
		SetWatcherP();
		break;
	}
	bCheckInfo = 0;
}

void CPropertiesWnd::SetAgentP()
{
	m_wndPropList.DeleteProperty(pGroup1);
	pGroup1 = new CMFCPropertyGridProperty(_T("Agent"));
	pAgentProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) csAgentName, _T("Agent의 Name입니다."));
	pAgentProp2 = new CMFCPropertyGridProperty(_T("IP"), (_variant_t) csAgentIP, _T("Agent의 IP입니다."));
	pAgentProp3 = new CMFCPropertyGridProperty(_T("CPU"), (_variant_t) csAgentCPU, _T("Agent의 CPU 사용량입니다."));
	pAgentProp4 = new CMFCPropertyGridProperty(_T("MEM"), (_variant_t) csAgentMEM, _T("Agent의 Memory 사용량입니다."));
	pAgentProp5 = new CMFCPropertyGridProperty(_T("DISK"), (_variant_t) csAgentDISK, _T("Agent의 Disk 사용량입니다."));
	pGroup1->AddSubItem(pAgentProp1);
	pGroup1->AddSubItem(pAgentProp2);
	pGroup1->AddSubItem(pAgentProp3);
	pGroup1->AddSubItem(pAgentProp4);
	pGroup1->AddSubItem(pAgentProp5);
	m_wndPropList.AddProperty(pGroup1);

	m_wndPropList.DeleteProperty(pGroup2);
	pGroup2 = new CMFCPropertyGridProperty(_T("Watcher Log File"));
	pLogProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) _T(""), _T("Log File의 이름입니다."));
	pLogProp2 = new CMFCPropertyGridProperty(_T("Path"), (_variant_t) _T(""), _T("Log File의 경로입니다."));
	pLogProp3 = new CMFCPropertyGridProperty(_T("Size"), (_variant_t) _T(""), _T("Log File의 크기입니다."));
	pGroup2->AddSubItem(pLogProp1);
	pGroup2->AddSubItem(pLogProp2);
	pGroup2->AddSubItem(pLogProp3);
	m_wndPropList.AddProperty(pGroup2);
}

void CPropertiesWnd::SetWatcherP()
{
	m_wndPropList.DeleteProperty(pGroup1);
	pGroup1 = new CMFCPropertyGridProperty(_T("Agent"));
	pAgentProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) _T(""), _T("Agent의 Name입니다."));
	pAgentProp2 = new CMFCPropertyGridProperty(_T("IP"), (_variant_t) _T(""), _T("Agent의 IP입니다."));
	pAgentProp3 = new CMFCPropertyGridProperty(_T("CPU"), (_variant_t) _T(""), _T("Agent의 CPU 사용량입니다."));
	pAgentProp4 = new CMFCPropertyGridProperty(_T("MEM"), (_variant_t) _T(""), _T("Agent의 Memory 사용량입니다."));
	pAgentProp5 = new CMFCPropertyGridProperty(_T("DISK"), (_variant_t) _T(""), _T("Agent의 Disk 사용량입니다. \n(총 크기/사용 가능한 크기)"));
	pGroup1->AddSubItem(pAgentProp1);
	pGroup1->AddSubItem(pAgentProp2);
	pGroup1->AddSubItem(pAgentProp3);
	pGroup1->AddSubItem(pAgentProp4);
	pGroup1->AddSubItem(pAgentProp5);
	m_wndPropList.AddProperty(pGroup1);

	m_wndPropList.DeleteProperty(pGroup2);
	pGroup2 = new CMFCPropertyGridProperty(_T("Watcher Log File"));
	pLogProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) csWatcherFileName, _T("Log File의 이름입니다."));
	pLogProp2 = new CMFCPropertyGridProperty(_T("Path"), (_variant_t) csWatcherFileDirectory, _T("Log File의 경로입니다."));
	pLogProp3 = new CMFCPropertyGridProperty(_T("Size"), (_variant_t) csWatcherFileSize, _T("Log File의 크기입니다."));
	pGroup2->AddSubItem(pLogProp1);
	pGroup2->AddSubItem(pLogProp2);
	pGroup2->AddSubItem(pLogProp3);
	m_wndPropList.AddProperty(pGroup2);
}

void CPropertiesWnd::RefreshAW()
{
	m_wndPropList.DeleteProperty(pGroup1);
	pGroup1 = new CMFCPropertyGridProperty(_T("Agent"));
	pAgentProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) _T(""), _T("Agent의 Name입니다."));
	pAgentProp2 = new CMFCPropertyGridProperty(_T("IP"), (_variant_t) _T(""), _T("Agent의 IP입니다."));
	pAgentProp3 = new CMFCPropertyGridProperty(_T("CPU"), (_variant_t) _T(""), _T("Agent의 CPU 사용량입니다."));
	pAgentProp4 = new CMFCPropertyGridProperty(_T("MEM"), (_variant_t) _T(""), _T("Agent의 Memory 사용량입니다."));
	pAgentProp5 = new CMFCPropertyGridProperty(_T("DISK"), (_variant_t) _T(""), _T("Agent의 Disk 사용량입니다. \n(총 크기/사용 가능한 크기)"));
	pGroup1->AddSubItem(pAgentProp1);
	pGroup1->AddSubItem(pAgentProp2);
	pGroup1->AddSubItem(pAgentProp3);
	pGroup1->AddSubItem(pAgentProp4);
	pGroup1->AddSubItem(pAgentProp5);
	m_wndPropList.AddProperty(pGroup1);

	m_wndPropList.DeleteProperty(pGroup2);
	pGroup2 = new CMFCPropertyGridProperty(_T("Watcher Log File"));
	pLogProp1 = new CMFCPropertyGridProperty(_T("Name"), (_variant_t) _T(""), _T("Log File의 이름입니다."));
	pLogProp2 = new CMFCPropertyGridProperty(_T("Path"), (_variant_t) _T(""), _T("Log File의 경로입니다."));
	pLogProp3 = new CMFCPropertyGridProperty(_T("Size"), (_variant_t) _T(""), _T("Log File의 크기입니다."));
	pGroup2->AddSubItem(pLogProp1);
	pGroup2->AddSubItem(pLogProp2);
	pGroup2->AddSubItem(pLogProp3);
	m_wndPropList.AddProperty(pGroup2);

	bWatcherInfo = false;
	bAgentInfo = false;
}