#pragma once
#include "afxdialogex.h"


#include "BtnST.h"
#include "CMCButton.h"
// CDlg_Top 대화 상자

class CDlg_Top : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Top)

public:
	CDlg_Top(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Top();

	CBitmap* LoadFileBitmap(const char* bmpName);
    CBitmap* m_bmpBg;

	CString m_path{};
	CString m_slog{};


	void InitBtn();
	CMCButton m_chkTest;
	CMCButton m_pBtnTest;
	CMCButton* m_pbtntest;
	//std::unique_ptr < CMCButton>  m_pcheck;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
};
