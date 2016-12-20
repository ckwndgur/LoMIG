#pragma once

#include "TextManager.h"
#include "Filter.h"
#include "LogFtView.h"


// DFilterView view

class DFilterView : public CScrollView
{
	DECLARE_DYNCREATE(DFilterView)

protected:
	DFilterView();           // protected constructor used by dynamic creation
	virtual ~DFilterView();

public:


	CString m_filePath;

	Filter mFilter;

	CStatic m_StaticERROR;
	CStatic m_StaticDATE;
	CStatic m_StaticPATH;
	CStatic m_StaticLINE;
	CStatic m_StaticDES;
	CStatic m_StaticTOTAL;

	CWnd m_EditERROR;
	CWnd m_EditDATE;
	CWnd m_EditPATH;
	CWnd m_EditLINE;
	CWnd m_EditDES;
	CWnd m_EditTOTAL;

	CWnd m_EditSearch;
	CWnd m_ButtonSearch;

	list<CString> m_strFilteredData;
	

	CString filterform;

	void MakeFilterForm(CString columnlist);
	void PreFiltering(list<string> WantedLogString);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);


#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	

	DECLARE_MESSAGE_MAP()
};


