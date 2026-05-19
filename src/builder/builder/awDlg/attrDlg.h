#pragma once

#include "resource.h"

// CAttrDlg 대화 상자입니다.

class AFX_EXT_CLASS CAttrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttrDlg)

public:
	CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attributes, DWORD auxAttributes);   // 표준 생성자입니다.
	virtual ~CAttrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ATTR };
	BOOL	m_at01;
	BOOL	m_at02;
	BOOL	m_at03;
	BOOL	m_at04;
	BOOL	m_at05;
	BOOL	m_at06;
	BOOL	m_at07;
	BOOL	m_at08;
	BOOL	m_at09;
	BOOL	m_at10;
	BOOL	m_at11;
	BOOL	m_at12;
	BOOL	m_at13;
//	BOOL	m_at14;
	BOOL	m_at15;
	BOOL	m_at16;
	BOOL	m_at17;
	BOOL	m_at18;
	BOOL	m_at19;
	BOOL	m_at20;

	DWORD	m_attributes;
	DWORD	m_auxAttributes;

protected:
	BYTE	m_kind;
	BYTE	m_type;
	BYTE	m_iok;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckComma();
	afx_msg void OnCheckRight();
	afx_msg void OnCheckZerosup();
	afx_msg void OnCheckSend();
	afx_msg	void OnCheckCoRgb();
	afx_msg void OnCheckContrast();

	afx_msg void OnContrast(UINT cmdID);
	afx_msg	void OnZeroSup(UINT cmdID);
	afx_msg	void OnCoRgb(UINT cmdID);
	afx_msg	void OnSend(UINT cmdID);
};
