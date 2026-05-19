#pragma once


// CFormLoadDlg 대화 상자입니다.

class CFormLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFormLoadDlg)

public:
	CFormLoadDlg(CWnd* pParent, CString strID, CString strName);   // 표준 생성자입니다.
	virtual ~CFormLoadDlg();

public:
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FORM_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CString m_strID;
	CString m_strName;
	CString m_sRoot;
	BOOL	m_bFlag;
	int		m_iMode;

public:
	void SetRoot(CString sRoot){ m_sRoot = sRoot; }
	int  GetMode()             { return m_iMode; }

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRaReadonly();
	afx_msg LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	
};
