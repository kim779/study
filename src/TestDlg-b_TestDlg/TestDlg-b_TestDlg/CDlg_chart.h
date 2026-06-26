#pragma once


// CDlg_chart 대화 상자
#include "header.h"
class CDlg_chart : public CDialog
{
	DECLARE_DYNAMIC(CDlg_chart)

public:
	CDlg_chart(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_chart();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CMapStringToOb	m_penObs{};
	CMapStringToOb	m_pointFontObs{};
	CPen* getpen(int style, int width, COLORREF clr);
	CFont* getfont(CString fname, int ipoint, bool bBold);
	afx_msg void OnBnClickedButton1();
};
