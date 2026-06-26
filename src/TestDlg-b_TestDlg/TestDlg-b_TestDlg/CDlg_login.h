#pragma once


// CDlg_login 대화 상자

class CDlg_login : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_login)

public:
	CDlg_login(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_login();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOGIN
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSignr();
	afx_msg void OnBnClickedBtnCheck();
};
