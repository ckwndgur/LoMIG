
#pragma once

#include "ViewTree.h"
#include "TreeviewManager.h"
#include "TreeviewData.h"
#include <list>
#include <string>
#include <iostream>
#include <afxwin.h>
#include <Windows.h>
#include "TextManager.h"
#include "Filter.h"


class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 생성입니다.
public:
	CFileView();
	


// 특성입니다.
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;


protected:
	void FillFileView();

// 구현입니다.
public:
	virtual ~CFileView();

	

	

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

	HTREEITEM m_hItemFirstSel;

public:
	CString csData;
	
	int checkMulti;

	CString getData();
	void RefreshFileView();
	void AdjustLayout();
	void OnChangeVisualStyle();
// 	void OpenOneItem();
// 	void OpenItems();


private:
	TextManager mTextManager;
	TreeviewData *mSelTVData;
	TreeviewManager m_TreeviewManager;

	HTREEITEM hRoot, hSrc, hInc;
	CString csTVDataFilePath;
	CString csTVDataFileName;
	int scroll_cx; 
	int scroll_cy; 
	int iMultiCnt;

	void GetFileNames();
	void GetFileSizeForList();
	CString GetFileSizeForCString(CString directory);
	void FindFileDirectory(CString pstr);
	CString	GetFilePathAtFile(HTREEITEM hItem);
	CString	GetFilePathBelowIP(HTREEITEM hItem);
	CString	GetFilePathBelowDate(HTREEITEM hItem);
	CString	GetFilePathBelowRoot(HTREEITEM hItem);
	void MakeTreeview(CString pstr);
	CSize Cal_scrollview(CString fulldirectory);
	void MessageToPV(TV_HITTESTINFO hitinfo, HTREEITEM selitem);
	void GetSelectedItems();
	//bool Ancestor(HTREEITEM hItem, HTREEITEM hCheck);
	void GetSelectedFilePath();
	

// 	void MultiSelection_ctl(TV_HITTESTINFO hitinfo, HTREEITEM selitem);
// 	void MultiSelection_shift(TV_HITTESTINFO hitinfo, HTREEITEM selitem);
// 	void ClearSelection();
// 	BOOL SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);
// 	void ShiftKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	
public:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

