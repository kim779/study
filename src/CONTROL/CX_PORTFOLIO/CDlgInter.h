#pragma once


// CDlgInter 대화 상자

class CDlgInter : public CDialog
{
	DECLARE_DYNAMIC(CDlgInter)

public:
	CDlgInter(CWnd* pParent = nullptr);
	CDlgInter(CWnd* pParent, char* sroot);   // 표준 생성자입니다.
	virtual ~CDlgInter();

	CString m_strRoot, m_strUser;
	CWnd* m_pMainFrame{};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIG_INTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
