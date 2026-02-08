#pragma once
#include "afxwin.h"
#include "../../inc/ExStatic.h"

#define IDD_DLG_WAIT 3066

// CDlgWait 대화 상자입니다.

class CDlgWait : public CDialog
{
	DECLARE_DYNAMIC(CDlgWait)

public:
	CDlgWait(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgWait();

	COLORREF m_crBk;
	CBrush	m_brBkColor;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_WAIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CExStatic m_ExStaticTitle;
};
