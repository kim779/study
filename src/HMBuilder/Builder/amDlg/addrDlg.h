#pragma once

#include "resource.h"


// CAddrDlg 대화 상자입니다.

class AFX_EXT_CLASS CAddrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrDlg)

public:
	CAddrDlg(CString path = _T(""), CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDR };
	CIPAddressCtrl m_addr;


	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
	CString	m_ips;
	CString m_path;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void	getAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3);

	DECLARE_MESSAGE_MAP()
};
