#include "resource.h"

#include <string>
#include <sstream>
#include <iostream>

#include "TCPCommunication.h"
#include "XMLManager.h"
#include "UserConfig.h"
#include "StringManager.h"

#define MY_TCP_PORT 18840

#pragma once


// CAgentDirDlg dialog

class CAgentDirDlg : public CDialog
{
	DECLARE_DYNAMIC(CAgentDirDlg)

public:
	CAgentDirDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAgentDirDlg();
	
	string sAgentDir;
	string sAgentIP;

// Dialog Data
	enum { IDD = IDD_AGENTDIRDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};