// DFilterView.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "DFilterView.h"
#include "MainFrm.h"
#include "SWLogDebuggingTool(W)Doc.h"
#include "stdlib.h"
#include "string.h"
#include "FileView.h"
#include "FolderManager.h"


// DFilterView

IMPLEMENT_DYNCREATE(DFilterView, CScrollView)

DFilterView::DFilterView()
{

}

DFilterView::~DFilterView()
{
}


BEGIN_MESSAGE_MAP(DFilterView, CScrollView)
END_MESSAGE_MAP()


// DFilterView drawing

void DFilterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	Filter *pfilter =  new Filter;
	//filterform = "";
	//filterform = pfilter->LogColumn;

	//MakeFilterForm(filterform);

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 200;

	m_EditERROR.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,0,250,20), this, WantedERROR);
	m_EditDATE.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,30,250,50), this, WantedDATE);
	m_EditPATH.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,60,250,80), this, WantedPATH);
	m_EditLINE.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,90,250,110), this, WantedLINE);
	m_EditDES.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,120,250,140), this, WantedDESCRIPTION);
	m_EditTOTAL.Create(TEXT("EDIT"),TEXT(""), WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL, CRect(150,150,250,170), this, WantedTOTAL);

	m_StaticERROR.Create(_T("ERROR"),WS_CHILD|WS_VISIBLE, CRect(20,0,120,20), this, 327895);
	m_StaticDATE.Create(_T("DATE"),WS_CHILD|WS_VISIBLE, CRect(20,30,120,50), this, 327896);
	m_StaticPATH.Create(_T("PATH"),WS_CHILD|WS_VISIBLE, CRect(20,60,120,80), this, 327897);
	m_StaticLINE.Create(_T("LINE"),WS_CHILD|WS_VISIBLE, CRect(20,90,120,110), this, 327898);
	m_StaticDES.Create(_T("DESCRIPTION"),WS_CHILD|WS_VISIBLE, CRect(20,120,120,140), this, 327899);
	m_StaticTOTAL.Create(_T("TOTAL"),WS_CHILD|WS_VISIBLE, CRect(20,150,120,170), this, 327900);

	m_ButtonSearch.Create(TEXT("BUTTON"), TEXT("Search"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(50,180,200,200), this, 327901);


	SetScrollSizes(MM_TEXT, sizeTotal);

}

void DFilterView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here


}

void DFilterView::MakeFilterForm(CString columnlist)
{
	int starptr;
	starptr = 0;

	CString columnlist_buf;
	columnlist_buf = "";

	
}

BOOL DFilterView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(wParam == 221)
	{

		CString Errorstr;
		CString Datestr;
		CString Pathstr;
		CString Linestr;
		CString Descriptionstr;
		CString Totalstr;
		CString Newfilepath;
		Errorstr = Datestr  = Pathstr = Linestr = Descriptionstr = Totalstr =  Newfilepath = "";

		list<string> WantedString;

		//editbox의 값을 읽어옵니다.
		GetDlgItemTextA(WantedERROR,Errorstr);
		CT2CA AnsiStringerror(Errorstr);
		string Wantederror(AnsiStringerror);

		GetDlgItemTextA(WantedDATE,Datestr);
		CT2CA AnsiStringdate(Datestr);
		string Wanteddate(AnsiStringdate);

		GetDlgItemTextA(WantedPATH,Pathstr);
		CT2CA AnsiStringpath(Pathstr);
		string Wantedpath(AnsiStringpath);

		GetDlgItemTextA(WantedLINE,Linestr);
		CT2CA AnsiStringline(Linestr);
		string Wantedline(AnsiStringline);

		GetDlgItemTextA(WantedDESCRIPTION,Descriptionstr);
		CT2CA AnsiStringdes(Descriptionstr);
		string Wanteddes(AnsiStringdes);

		GetDlgItemTextA(WantedTOTAL,Totalstr);
		CT2CA AnsiStringtotal(Totalstr);
		string Wantedtotal(AnsiStringtotal);

		WantedString.push_back(Wantederror);
		WantedString.push_back(Wanteddate);
		WantedString.push_back(Wantedpath);
		WantedString.push_back(Wantedline);
		WantedString.push_back(Wanteddes);
		WantedString.push_back(Wantedtotal);

		PreFiltering(WantedString);
/*
		string Title= mFilter.CreatingTime(Wantederror);
		m_strFilteredData.clear();
		m_strFilteredData = mFilter.DoFilter(1, Wantederror, Title, m_filePath);
*/
		this->Invalidate(TRUE);

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////여기까지

	return CView::OnCommand(wParam, lParam);
}

void DFilterView::PreFiltering(list<string> WantedLogString)
{
	list<string> keywords;
	list<int> catenumber;
	string TitleMana;
	string keywordBuf;
	TitleMana = keywordBuf = "";
	int cate = 0;
	int catenumberBuf = 0;
	bool multiflag = false;
	bool filterproc = true;
	CString filterkeyword;
	CString csfilepath, newpath, originfile, temp = ""; 

	//for(cate = 0; cate <= 5; cate++)
	while(filterproc)
	{
		//catenumberBuf = cate + 1;
		catenumberBuf = catenumberBuf + 1;

		if(WantedLogString.front() == "")
		{

			WantedLogString.pop_front();
		}

		else
		{
			//catenumberBuf = cate + 1;
			catenumber.push_back(catenumberBuf);
			keywordBuf = WantedLogString.front();
			keywords.push_back(keywordBuf);
			TitleMana = TitleMana + "[" + WantedLogString.front() + "]";
			WantedLogString.pop_front();

			string Title= mFilter.CreatingTime(TitleMana);

			if(multiflag == false)
			{
				
				for (int i = 0; i<4; i++)
				{
					AfxExtractSubString(temp, m_filePath, i, '\\');
					csfilepath += temp + '\\';
				}
				AfxExtractSubString(temp, m_filePath, 4, '\\');
				AfxExtractSubString(temp, temp, 0, '.');

				csfilepath = csfilepath + "Debug" + '\\' + "(" + temp + ")" + Title.c_str();
				originfile = temp;
				FolderManager mFolderManager;
				mFolderManager.MakeDirectory((LPSTR)((LPCTSTR)csfilepath));

				newpath = csfilepath;

				m_strFilteredData.clear();
				m_strFilteredData = mFilter.DoFilter(catenumberBuf, keywordBuf, Title, m_filePath, csfilepath, multiflag);

				multiflag  =  true;
			}
			else
			{
				char oldpath [10240] = {0,};
				csfilepath = temp = "";

				for (int i = 0; i<4; i++)
				{
					AfxExtractSubString(temp, m_filePath, i, '\\');
					csfilepath += temp + '\\';
				}
				AfxExtractSubString(temp, m_filePath, 4, '\\');
				AfxExtractSubString(temp, temp, 0, '.');

				csfilepath = csfilepath + "Debug" + '\\' + "(" + temp + ")" + Title.c_str();

				FolderManager mFolderManager;
				mFolderManager.MakeDirectory((LPSTR)((LPCTSTR)csfilepath));

				m_strFilteredData.clear();
				m_strFilteredData = mFilter.DoFilter(catenumberBuf, keywordBuf, Title, newpath, csfilepath, multiflag);

				memcpy(oldpath, (LPSTR)(LPCSTR)newpath, newpath.GetLength());
				remove(oldpath);
				newpath = csfilepath;
				multiflag  =  true;
			}
		}

		if(catenumberBuf == 6)
		{
			filterproc = false;
		}
		else
		{
		
		}
	}

}


// DFilterView diagnostics

#ifdef _DEBUG
void DFilterView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void DFilterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// DFilterView message handlers
