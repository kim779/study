#pragma once


// CGroupNameDlg 대화 상자입니다.

class CGroupNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNameDlg)

public:
	CGroupNameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGroupNameDlg();

	void SetGroupName(LPCTSTR lpszGroupName);
	LPCTSTR GetGroupName();
	CString	m_strGroupName;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_GROUPNAMEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
