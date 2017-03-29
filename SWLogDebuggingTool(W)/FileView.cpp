
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SWLogDebuggingTool(W).h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "LogFileView.h"
#include "LogFtView.h"
#include "DFilterView.h"
#include "PropertiesWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
	m_hItemFirstSel = NULL;
	iMultiCnt = 0;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 메시지 처리기

list<CString> cslistFileNames;
list<CString> cslistFilePaths;
list<CString> cslistFileSizes;
list<HTREEITEM> cslistItems;

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("파일 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 뷰 이미지를 로드합니다.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /*잠금*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
	FillFileView();
	AdjustLayout();
	

	//m_wndFileView.EnableMultiSelect(true);
	
	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::RefreshFileView()
{
	m_wndFileView.DeleteAllItems();
	MakeTreeview("C:\\LogDebugging");
	m_wndFileView.Invalidate();
	m_wndFileView.UpdateWindow();
	
	cslistFilePaths.clear();
	cslistItems.clear();
	cslistFileNames.clear();
	cslistFileSizes.clear();

}
void CFileView::MakeTreeview(CString pstr) // Folder searching and make tree view + going to use SetItemData for saving information
{
	CFileFind finder;
	CString strWildcard(pstr);
	int treeindex;

	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory()) 
		{
			CString str = finder.GetFilePath();
			treeindex = m_TreeviewManager.GetCharNumber(str, '\\') - 2; //ex: C:\LogDebugging\20160417\202.31.137.116\dsa -> defaujlt: 2*/
			
			CString temp;
			AfxExtractSubString(temp, str, treeindex+2, '\\');

			if (treeindex == 0)
			{
				TreeviewData *mTreeviewData = new TreeviewData();
				hSrc = m_wndFileView.InsertItem(temp, 0, 0, hRoot);
				mTreeviewData->setFileName(temp);
				mTreeviewData->setFullDirectory(finder.GetFilePath());
				m_wndFileView.SetItemData(hSrc, (DWORD)mTreeviewData);
			} 
			else if (treeindex == 1)
			{
				TreeviewData *mTreeviewData = new TreeviewData();
				hInc = m_wndFileView.InsertItem(temp, 0, 0, hSrc);
				mTreeviewData->setFileName(temp);
				mTreeviewData->setFullDirectory(finder.GetFilePath());
				m_wndFileView.SetItemData(hInc, (DWORD)mTreeviewData);
			}

			MakeTreeview(str);
		}
		else
		{
			TreeviewData *mTreeviewData = new TreeviewData();
			m_wndFileView.InsertItem(finder.GetFileTitle(), 1, 1, hInc);
			mTreeviewData->setFileName(finder.GetFileName());
			mTreeviewData->setFullDirectory(finder.GetFilePath());
			m_wndFileView.SetItemData(hInc, (DWORD)mTreeviewData);
		}
	}
	finder.Close();
}

void CFileView::FillFileView()
{
	hRoot = m_wndFileView.InsertItem(_T("LogDebugging"), 0, 0);
	
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
 
	MakeTreeview("C:\\LogDebugging");

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CEditTreeCtrlEx* pWndTree = (CEditTreeCtrlEx*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		checkMulti = m_wndFileView.GetSelectedCount();
		if ( checkMulti == 1 )
		{
			CPoint ptTree = point;
			pWndTree->ScreenToClient(&ptTree);

			UINT flags = 0;
			HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
			csTVDataFileName = pWndTree->GetItemText(hTreeItem);
			HTREEITEM hTreeItem_buf = hTreeItem;

			csTVDataFilePath = GetFilePathAtFile(hTreeItem);
		}
		else if ( checkMulti > 1 )
		{
			GetSelectedItems();
			GetSelectedFilePath();
			GetFileSizeForList();
			GetFileNames();
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::GetSelectedItems() {
	for(HTREEITEM hItem = m_wndFileView.GetFirstSelectedItem(); hItem != 0; hItem = m_wndFileView.GetNextSelectedItem(hItem)) {
		cslistItems.push_back(hItem);
	}
}



void CFileView::GetSelectedFilePath()
{
	for(list<HTREEITEM>::iterator it = cslistItems.begin(); it != cslistItems.end(); ++it)
	{
		int cnt = m_wndFileView.GetLevel(*it);
		CString temp;
		HTREEITEM hChildItem;
		switch (cnt)
		{
		case 0 :
			temp = GetFilePathBelowRoot(*it);
			FindFileDirectory(temp);
			/*cslistFilePaths.push_back(temp);*/
			break;
		case 1 :
			temp = GetFilePathBelowDate(*it);
			FindFileDirectory(temp);
			/*cslistFilePaths.push_back(temp);*/
			break;
		case 2 :
			temp = GetFilePathBelowIP(*it);
			FindFileDirectory(temp);
			/*cslistFilePaths.push_back(temp);*/
			break;
		case 3 :
			temp = GetFilePathAtFile(*it);
			cslistFilePaths.push_back(temp);
			break;
		default:
			break;
		}
	}
	cslistFilePaths.sort();
	cslistFilePaths.unique();

}

void CFileView::GetFileNames()
{
	
	for (list<CString>::iterator iterPos = cslistFilePaths.begin(); iterPos != cslistFilePaths.end(); ++iterPos)
	{
		CString bufPath = *iterPos;
		int cnt = m_TreeviewManager.GetCharNumber(bufPath, '\\') - 2;
		CString temp;
		AfxExtractSubString(temp, bufPath, cnt+2, '\\');

		cslistFileNames.push_back(temp);
	}
}

void CFileView::GetFileSizeForList()
{
	for (list<CString>::iterator iterPos = cslistFilePaths.begin(); iterPos != cslistFilePaths.end(); ++iterPos)
		cslistFileSizes.push_back(GetFileSizeForCString(*iterPos));
}

void CFileView::FindFileDirectory(CString pstr)
{
	CFileFind finder;
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);
	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			FindFileDirectory(str);
		}
		else
		{
			cslistFilePaths.push_back(finder.GetFilePath());
		}
	}
	finder.Close();
}


void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("속성...."));

}

void CFileView::OnFileOpen()
{
	

	if ( checkMulti == 1 )
	{
		ifstream originfile;
		originfile.open(csTVDataFilePath);
		
		if (!originfile.fail())
		{
			if (csTVDataFilePath.GetLength() >0 )
			{
				CSWLogDebuggingToolWApp *pApp = (CSWLogDebuggingToolWApp *)AfxGetApp();
				CSWLogDebuggingToolWDoc *pDoc = (CSWLogDebuggingToolWDoc *)pApp->pDocTemplate->OpenDocumentFile(csTVDataFilePath);

				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				CChildFrame *pChild = (CChildFrame *) pFrame->GetActiveFrame();

				LogFileView *pView = (LogFileView *)pChild->GetFileViewPane();
				LogFtView *pFtView = (LogFtView *)pChild->GetFtViewPane();
				DFilterView *pDView = (DFilterView *)pChild->GetDFilterViewPane();

				pView->m_strView = mTextManager.ReadTextList((LPSTR)(LPCTSTR)csTVDataFilePath);
				pView->m_bView = TRUE;
				pView->m_textsize = Cal_scrollview(csTVDataFilePath);

				pView->m_openflag = TRUE;
				pView->openfilepath = csTVDataFilePath;
				pFtView->m_strViewPath = csTVDataFilePath;
				pDView->m_filePath = csTVDataFilePath;
				pFtView->m_textsize = Cal_scrollview(csTVDataFilePath);

				pView->Invalidate(TRUE);

			}
			else
			{
				AfxMessageBox(TEXT("파일열기에 실패했습니다."));
			}
		}
		else
		{
			AfxMessageBox(TEXT("파일열기에 실패했습니다."));
		}
	}
	else if ( checkMulti > 1 )
	{
		CSWLogDebuggingToolWApp *pApp = (CSWLogDebuggingToolWApp *)AfxGetApp();
		CSWLogDebuggingToolWDoc *pDoc = (CSWLogDebuggingToolWDoc *)pApp->pDocTemplate->OpenDocumentFile(NULL);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CChildFrame *pChild = (CChildFrame *) pFrame->GetActiveFrame();

		LogFileView *pView = (LogFileView *)pChild->GetFileViewPane();
		LogFtView *pFtView = (LogFtView *)pChild->GetFtViewPane();
		DFilterView *pDView = (DFilterView *)pChild->GetDFilterViewPane();
		
		pDoc->SetTitle("MultiOpen");
		pView->m_lstcsPaths = cslistFilePaths;
		pView->m_bView = TRUE;
		pView->m_textsize = Cal_scrollview(csTVDataFilePath);
		pView->m_bMultiSelect = TRUE;
		pView->m_lstcsNames = cslistFileNames;
		pView->m_lstcsSizes = cslistFileSizes;
		pDView->multi_filepath = cslistFilePaths;
		pFtView->isMultiSelction = true;

		cslistFilePaths.clear();
		cslistItems.clear();
		cslistFileNames.clear();
		cslistFileSizes.clear();
		pView->Invalidate(TRUE);

	}
	
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

CSize CFileView::Cal_scrollview(CString fulldirectory)
{
	CSize textsize;
	
	if ( checkMulti == 1)
	{
		textsize.cx = mTextManager.GetMaxLineSize((LPSTR)(LPCTSTR)fulldirectory);
		textsize.cy = mTextManager.GetLinelength((LPSTR)(LPCTSTR)fulldirectory);
	} 
	else if ( checkMulti > 1 )
	{
		textsize.cx = 10;
		textsize.cy = 10;
	}
	
	
	return textsize;
	
}

// bool CFileView::Ancestor(HTREEITEM hItem, HTREEITEM hCheck) {
// 	for(HTREEITEM hParent = hCheck; hParent != 0; hParent = m_wndFileView.GetParentItem(hParent))
// 		if(hParent == hItem)
// 			return true;
// 	return false;
// }

CString CFileView::getData()
{
	return csData;
}

void CFileView::OnFileOpenWith()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	hRoot = NULL;
	hInc = NULL;
	hSrc = NULL;
	m_wndFileView.DeleteAllItems();
	m_wndFileView.Invalidate(TRUE);
	FillFileView();
	
}

void CFileView::OnDummyCompile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CWnd::OnLButtonDown(nFlags, point);
}
BOOL CFileView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	
	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
		{
			BOOL bControlKey = (0x8000 == (0x8000 & GetKeyState(VK_CONTROL)));
			BOOL bShiftKey = (0x8000 == (0x8000 & GetKeyState(VK_SHIFT)));
			if(!(bControlKey || bShiftKey))
			{
				TV_HITTESTINFO hit_info;
				::GetCursorPos(&hit_info.pt);
				::ScreenToClient(m_wndFileView.m_hWnd, &hit_info.pt);
				HTREEITEM selitem = m_wndFileView.HitTest(&hit_info);
				
				MessageToPV(hit_info, selitem);
				if (!selitem)
				{
					m_wndFileView.ClearSelection();
				}
			}
			
			break;
		}
	}
	return CDockablePane::PreTranslateMessage(pMsg);
}

CString	CFileView::GetFilePathAtFile(HTREEITEM hItem)
{
	CString path = m_wndFileView.GetItemText(hItem);
	hItem = m_wndFileView.GetNextItem(hItem, TVGN_PARENT);
	CString mid = m_wndFileView.GetItemText(hItem);

	hItem = m_wndFileView.GetNextItem(hItem, TVGN_PARENT);
	CString first = m_wndFileView.GetItemText(hItem);

	path = "C:\\LogDebugging\\" + first + "\\" + mid + "\\" + path +".txt";

	return path;
}

CString CFileView::GetFilePathBelowIP(HTREEITEM hItem)
{
	CString path;
	CString first;
	CString mid;
	HTREEITEM parentItem = m_wndFileView.GetNextItem(hItem, TVGN_PARENT);

	if (m_wndFileView.GetChildItem(hItem))
	{
		first = m_wndFileView.GetItemText(parentItem);
		mid = m_wndFileView.GetItemText(hItem);
		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		path = m_wndFileView.GetItemText(hItem);
	}

	path = "C:\\LogDebugging\\" + first + "\\" + mid;

	return path;
}


CString CFileView::GetFilePathBelowDate(HTREEITEM hItem)
{
	CString path;
	CString first;
	CString mid;

	if (m_wndFileView.GetChildItem(hItem))
	{
		first = m_wndFileView.GetItemText(hItem);
		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		mid = m_wndFileView.GetItemText(hItem);
		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		path = m_wndFileView.GetItemText(hItem);
	}

	path = "C:\\LogDebugging\\" + first;

	return path;
}


CString CFileView::GetFilePathBelowRoot(HTREEITEM hItem)
{
	CString path;
	CString first;
	CString mid;

	if (m_wndFileView.GetChildItem(hItem))
	{
		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		first = m_wndFileView.GetItemText(hItem);

		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		mid = m_wndFileView.GetItemText(hItem);

		hItem = m_wndFileView.GetNextItem(hItem, TVGN_CHILD);
		path = m_wndFileView.GetItemText(hItem);
	}

	path = "C:\\LogDebugging";

	return path;
}

CString CFileView::GetFileSizeForCString(CString directory)
{
	HANDLE hFile;
	hFile = ::CreateFile(directory, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	UINT ifilesize = GetFileSize(hFile, NULL);
	CString csfilesize = "";
	csfilesize.Format(_T("%d"), ifilesize);

	CloseHandle(hFile);

	return csfilesize;
}

void CFileView::MessageToPV(TV_HITTESTINFO hitinfo, HTREEITEM selitem)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CPropertiesWnd *pProWnd = (CPropertiesWnd*)pFrame->GetPropertyViewPT();
	
	HTREEITEM current_item = selitem;
	HTREEITEM selected_DateItem;
	HTREEITEM selected_IPItem;
	HTREEITEM selected_DefaultItem;

	CString csSelectedFileName;
	CString csSelectedIP;
	CString csSelectedDate;
	CString csSelectedDefault;
	CString csSelectedDirectory;

	if(current_item != NULL)
	{
		m_wndFileView.Select(current_item, TVGN_CARET);

		int cnt = m_wndFileView.GetLevel(current_item);
		
		if (cnt == 3)
		{
			csSelectedFileName = m_wndFileView.GetItemText(selitem);
			selected_IPItem = m_wndFileView.GetParentItem(selitem);
			csSelectedIP = m_wndFileView.GetItemText(selected_IPItem);
			selected_DateItem = m_wndFileView.GetParentItem(selected_IPItem);
			csSelectedDate = m_wndFileView.GetItemText(selected_DateItem);
			selected_DefaultItem = m_wndFileView.GetParentItem(selected_DateItem);
			csSelectedDefault = m_wndFileView.GetItemText(selected_DefaultItem);

			csSelectedDirectory = "C:\\" + csSelectedDefault + "\\" + csSelectedDate + "\\" + csSelectedIP + "\\" + csSelectedFileName;

			pProWnd->csWatcherFileName = csSelectedFileName;
			pProWnd->csWatcherFileDirectory = csSelectedDirectory;
			pProWnd->bCheckInfo = 3;
			CString csbuf = "C:\\" + csSelectedDefault + "\\" + csSelectedDate + "\\" + csSelectedIP + "\\";

			csSelectedDirectory += ".txt";
			
			CString csfilesize = GetFileSizeForCString(csSelectedDirectory);
			
			pProWnd->csWatcherFileSize = csfilesize + " Byte";

			pProWnd->Invalidate(FALSE);
		} 
		else
		{
			pProWnd->bCheckInfo = 1;
			pProWnd->csWatcherFileDirectory = "";
			pProWnd->csWatcherFileName = "";
			pProWnd->csWatcherFileSize = "";

			pProWnd->Invalidate(FALSE);
		}

	}
}