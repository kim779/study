#pragma once


// CDlg_load 대화 상자

class CDlg_load : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_load)

public:
	CDlg_load(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_load();
	CBitmap* LoadFileBitmap(const char* bmpName);
	CBitmap* m_bmpBg;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
