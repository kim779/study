#pragma once


// CDlg_MSGBOX 대화 상자

class CDlg_MSGBOX : public CDialog
{
	DECLARE_DYNAMIC(CDlg_MSGBOX)

public:
	CDlg_MSGBOX(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_MSGBOX();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MSGBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
