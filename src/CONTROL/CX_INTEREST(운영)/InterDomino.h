#pragma once
// InterDomino.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInterDomino dialog


class CInterDomino : public CDialog
{
// Construction
public:
	CInterDomino(CWnd* pParent, int rowCount, CString data);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInterDomino)
	enum { IDD = IDD_DLG_DOMINO };
	CCheckListBox	m_ListBox;
	CString	m_staticCount;
	//}}AFX_DATA

	CWnd*	m_pParent;
	CWnd*	m_pView;

	int		m_irowCount;
	CString m_dominoData;
	CString m_strCode[110][4];
	int		m_iTempRow;

	int		m_maxCount;

	CString GetCodeName(CString code);
	int	GetCodeType(CString code);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterDomino)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInterDomino)
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnChkall();
	afx_msg void OnBtnAllcancel();
	afx_msg void OnBtnChkkospi();
	afx_msg void OnBtnChkkosdaq();
	afx_msg void OnSelchangeListjm();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CInterDomino)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};