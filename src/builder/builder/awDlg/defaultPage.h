#pragma once

#include "resource.h"

// CDefaultPage 대화 상자입니다.

class CDefaultPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDefaultPage)

public:
	CDefaultPage();
	virtual ~CDefaultPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_DEFAULT };
	int	m_clrTabPos;
	int	m_dnRGB;
	CSpinButtonCtrl m_dnSpin;
	int	m_upRGB;
	CSpinButtonCtrl m_upSpin;
	CString	m_fontName;
	CString	m_fontPoint;

protected:
	CBrush	m_ctlBrushDn;
	CBrush	m_ctlBrushUp;

	COLORREF	m_ctlClrUp;
	COLORREF	m_ctlClrDn;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFont();
};
