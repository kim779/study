#pragma once


// CDlg 대화 상자

class CDlg : public CDialog
{
	DECLARE_DYNAMIC(CDlg)

public:
	CDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg();
	void AddTextEdit(CString sdata);
	CWnd* m_pWizrd;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnGpsearch();
	afx_msg void OnBnClickedBtnGpcode();
	afx_msg void OnBnClickedBtnDb();
	afx_msg void OnBnClickedBtnNewgroup();
	afx_msg void OnBnClickedBtnGroupsave();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnCodename();
	afx_msg void OnBnClickedBtnInteradd();
};
