#pragma once

#include "TextManager.h"
#include "Filter.h"

// LogFileView view

class LogFileView : public CScrollView
{
	DECLARE_DYNCREATE(LogFileView)

protected:
	LogFileView();           // protected constructor used by dynamic creation
	virtual ~LogFileView();

public:
//	Filter mFilter;
// 	CWnd m_EditSearch;
// 	CWnd m_ButtonSearch;
// 	CComboBox m_ComboBox;

	list<CString> m_strView;
//	CString m_strViewPath;
	CSize m_textsize;

//	list<CString> m_strFilteredData;


	BOOL m_bView;

	CString openfilepath;
	BOOL m_openflag;
	CListCtrl m_OriginLoglist;
	

private:
	void SetScrollView(int x, int y);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	void LogtoList(CString filepath);
	void DLogtoList(CString filepath);
	void ListtoLog();

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


