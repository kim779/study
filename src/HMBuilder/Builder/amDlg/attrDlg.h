#pragma once

#include "resource.h"

// CAttrDlg 대화 상자입니다.

class AFX_EXT_CLASS CAttrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttrDlg)

public:
	CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attr, DWORD attr2, int upRGB, int dnRGB);   // 표준 생성자입니다.
	virtual ~CAttrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR };
	BOOL	m_at01;		// COMMA
	BOOL	m_at02;		// PASSWD
	BOOL	m_at03;		// ZEROSUP
	BOOL	m_at04;		// PROTECT
	BOOL	m_at05;		// WRAP
	BOOL	m_at06;		// SKIP
	BOOL	m_at08;		// DOMINO
	BOOL	m_at09;		// UPPER
	BOOL	m_at10;		// NOR
	BOOL	m_at12;		// FLOAT
	BOOL	m_at13;		// FLOATx
	BOOL	m_at14;		// PASS

public:
	DWORD	m_attr;
	DWORD	m_attr2;

	int	m_upRGB;
	int	m_dnRGB;

	CString	m_downE;
	CString	m_upE;

	CComboBox m_corgb;
	CComboBox m_corgb2;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	CBrush		m_ctlBrushUp, m_ctlBrushDn;
	COLORREF	m_ctlClrUp, m_ctlClrDn;

public:
	BYTE	m_kind;
	BYTE	m_type;
	BYTE	m_iok;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckZerosup();
	virtual void OnOK();
	afx_msg	void OnZeroSup(UINT cmdID);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
