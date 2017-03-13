// AgentDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SWLogDebuggingTool(W).h"
#include "AgentDirDlg.h"


// CAgentDirDlg dialog

IMPLEMENT_DYNAMIC(CAgentDirDlg, CDialog)

CAgentDirDlg::CAgentDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgentDirDlg::IDD, pParent)
{

}

CAgentDirDlg::~CAgentDirDlg()
{
}

void CAgentDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAgentDirDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAgentDirDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAgentDirDlg message handlers

void CAgentDirDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	string sDest = sAgentIP;

	TCPCommunication mTCPCommunication;
	XMLManager mXMLManager;
	UserConfig mUserConfig;
	string sFileDIrChk = "";
	CString csNewDir = "";

	int iTCPSocket;

	GetDlgItemText(IDC_DIRINPUT, csNewDir);

	std::string sNewDir((LPCTSTR)csNewDir);

	sFileDIrChk = mUserConfig.GetExeDirectory()+"AgtInfo\\" + sAgentIP +".xml";
	mXMLManager.Target_EditElementXML(sFileDIrChk, "AgentInfo", "AgentLogFileDirectory", sNewDir);

	OnOK();
}
