
#pragma once

class CFilterToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFilterWnd : public CDockablePane
{
// 생성입니다.
public:
	CFilterWnd();

	void AdjustLayout();

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndFilterList.SetVSDotNetLook(bSet);
		m_wndFilterList.SetGroupNameFullWidth(bSet);
	}

	CFont m_fntFilterList;
	CComboBox m_wndObjectCombo;
	CFilterToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndFilterList;
	

protected:
	/*
	CFont m_fntFilterList;
	CComboBox m_wndObjectCombo;
	CFilterToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndFilterList;
	*/

// 구현입니다.
public:
	virtual ~CFilterWnd();
	void ProcessFilter();
	CString GetSearchString(int search_type);
	void InitFilterList();


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllFilter();
	afx_msg void OnUpdateExpandAllFilter(CCmdUI* pCmdUI);
	afx_msg void OnSortFilter();
	afx_msg void OnUpdateSortFilter(CCmdUI* pCmdUI);
	afx_msg void OnFilter1();
	afx_msg void OnUpdateFilter1(CCmdUI* pCmdUI);
	afx_msg void OnFilter2();
	afx_msg void OnUpdateFilter2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	//void InitFilterList();
	void SetFilterListFont();
};

