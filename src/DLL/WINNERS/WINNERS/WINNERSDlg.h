
// WINNERSDlg.h: 헤더 파일
//

#pragma once

//#include "CWebDlg.h"
//#include "CMainWnd.h"
// CWINNERSDlg 대화 상자

class CWINNERSDlg : public CDialogEx
{
// 생성입니다.
public:
	CWINNERSDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CString m_slog;
	CString m_root;
	//CWebDlg* m_pdlg;
	//CMainWnd* m_pMain;

	void CreateIE(CString strurl);
	void PopWinner();
	void CreateIEPorcess(CString strurl, CString strRoot);
	void CreateShellExcute(CString strurl, CString strRoot);
	CString Parser(CString& srcstr, CString substr);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINNERS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
