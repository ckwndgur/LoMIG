
#pragma once
#include <string>
#include "stdafx.h"
#include "afxcmn.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();

	void AdjustLayout();

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

// 구현입니다.
public:
	virtual ~CPropertiesWnd();

public:
	bool bAgentInfo;
	CString csAgentName;
	CString csAgentIP;
	CString csAgentCPU;
	CString csAgentMEM;
	CString csAgentDISK;

	bool bWatcherInfo;
	CString csWatcherFileName;
	CString csWatcherFileDirectory;
	CString csWatcherFileSize;

private:
	CMFCPropertyGridProperty* pGroup1 ;
	CMFCPropertyGridProperty* pAgentProp1;
	CMFCPropertyGridProperty* pAgentProp2;
	CMFCPropertyGridProperty* pAgentProp3;
	CMFCPropertyGridProperty* pAgentProp4;
	CMFCPropertyGridProperty* pAgentProp5;
	CMFCPropertyGridProperty* pGroup2;
	CMFCPropertyGridProperty* pLogProp1;
	CMFCPropertyGridProperty* pLogProp2;
	CMFCPropertyGridProperty* pLogProp3;

	void SetAgentP();
	void SetWatcherP();
	void RefreshAW();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

public:
	afx_msg void OnPaint();
	
};

