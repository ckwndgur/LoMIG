//#include "EditTreeCtrlEx.h"

#pragma once
#include "EditTreeCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTree â�Դϴ�.

class CViewTree : public CEditTreeCtrlEx
{
// �����Դϴ�.
public:
	CViewTree();

// �������Դϴ�.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// �����Դϴ�.
public:
	virtual ~CViewTree();

	void RefreshData();

protected:
	DECLARE_MESSAGE_MAP()
};
