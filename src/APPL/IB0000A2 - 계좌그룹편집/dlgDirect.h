#pragma once

#include "accEdit.h"
#include "libDefine.h"


class CDlgDirect : public CDialog
{
public:
	CDlgDirect(CWnd* pParent = NULL);
	//{{AFX_DATA(CDlgDirect)
	enum { IDD = IDD_DLGDIRECT };
	CAccEdit	m_EditAccNum;
	CEdit		m_EditAccName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgDirect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgDirect)
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnAccNameSearch(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	GetAccInfo(CString &accNum, CString &accName, CString &accPass);
	void	SetAccName(CString accName);
	CString	GetAccNum();
private:
	CWnd*	m_pParent;
	CString	m_accName;
	CString	m_accNum;
	CString m_accPass;
};

