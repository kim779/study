#pragma once


// CSockDlg 대화 상자

class CSockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSockDlg)

public:
	CSockDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSockDlg();

	std::unique_ptr<class Csocket> m_sock;


	void SockWrite_Something();
	void SockWrite_piboac10();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SOCK};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg LONG OnSockManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnTestsend();
};
