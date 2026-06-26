#pragma once


// CScrollDlg 대화 상자
//#include "CScrollWindow.h"
//#include "CTestScroll.h"
#include "CControlWnd.h"

class CScrollDlg : public CDialog
{
	DECLARE_DYNAMIC(CScrollDlg)

public:
	CScrollDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScrollDlg();
	CControlWnd scrollControl;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
};
