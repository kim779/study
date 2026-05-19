#pragma once

#include "resource.h"

// CCoRGB 대화 상자입니다.

class AFX_EXT_CLASS CCoRGB : public CDialogEx
{
	DECLARE_DYNAMIC(CCoRGB)

public:
	CCoRGB(CWnd* pParent, CString name, BYTE whatEdit, int upRGB, int dnRGB);   // 표준 생성자입니다.
	virtual ~CCoRGB();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CORGB };
	CString	m_downE;
	CString	m_upE;
	CString	m_name;

	int	m_upRGB;
	int	m_dnRGB;
	BYTE	m_whatEdit;

protected:
	CBrush		m_ctlBrushUp, m_ctlBrushDn;
	COLORREF	m_ctlClrUp, m_ctlClrDn;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnNoEdit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
};
