#pragma once

#include "TextManager.h"
#include "Filter.h"



// LogFtView view

class LogFtView : public CScrollView
{
	DECLARE_DYNCREATE(LogFtView)

	
protected:
	LogFtView();           // protected constructor used by dynamic creation
	virtual ~LogFtView();

public:
	//view


	//class
	CString m_strViewPath;
	CSize m_textsize;
	CListCtrl m_list;
	//var

	//func
private:
		CWnd m_EditSearch;
		CWnd m_ButtonSearch;
		CComboBox m_ComboBox;
	//view
		Filter mFilter;
	//class
		list<CString> m_strFilteredData;
		bool bCListCnt;
	//var
		void FillFtView(list<CString> input, CDC* pdc);
		void SetScrollView(int x, int y);
		CSize CalViewSize(int x, int y);
		void initClistCnt();
	//func

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


