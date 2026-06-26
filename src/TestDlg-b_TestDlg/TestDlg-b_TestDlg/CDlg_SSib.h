#pragma once


// CDlg_SSib 대화 상자

class CDlg_SSib : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_SSib)

public:
	CDlg_SSib(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_SSib();

	template <typename T>
	void ShowVec(std::vector<T> strvec);
	CString m_slog;

	void funcpt();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSSIB};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnMuitable();
	afx_msg void OnBnClickedBtnStatic();
	afx_msg void OnBnClickedBtnRef();
	afx_msg void OnBnClickedBtnFriend();
	afx_msg void OnBnClickedBtnOver();
	afx_msg void OnBnClickedBtnLamda();
	afx_msg void OnBnClickedBtnMangling();
	afx_msg void OnBnClickedBtnStfunc();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnTemplete();
	afx_msg void OnBnClickedBtnTstatic();
	afx_msg void OnBnClickedBtnBbsort();
	afx_msg void OnBnClickedBtnInheri();
	afx_msg void OnBnClickedBtnStdmap();
};
