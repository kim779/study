
// NaonStarterDlg.h: 헤더 파일
//

#pragma once


// CNaonStarterDlg 대화 상자
class CNaonStarterDlg : public CDialogEx
{
// 생성입니다.
public:
	CNaonStarterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAONSTARTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
	int m_itry{};
	int m_icnt{};
	int m_port{};
	CString m_serverip{};
	CString m_strPath{};
	CString m_slog;
	BOOL ProcessFind(char* strProcessName, DWORD processID = 0);
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
