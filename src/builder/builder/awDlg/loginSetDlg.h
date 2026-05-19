#pragma once

#include "resource.h"

// CLoginSetDlg 대화 상자입니다.

class AFX_EXT_CLASS CLoginSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginSetDlg)

public:
	CLoginSetDlg(CString path, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoginSetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGINSETDLG };
	CIPAddressCtrl	m_addr;
	CString	m_sProjectName;
	int	m_nPort;
	BOOL	m_bFloat;

protected:
	CString	m_ips;
	CString	m_path;

	void	getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
