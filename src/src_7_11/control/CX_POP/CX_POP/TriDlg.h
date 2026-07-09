#pragma once


// CTriDlg 대화 상자입니다.

class CTriDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTriDlg)

public:
	CTriDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTriDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TRI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
