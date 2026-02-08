#if !defined(AFX_DLGSAVESCREEN_H__25FDDF69_FBBC_4D68_BB41_5992C933726E__INCLUDED_)
#define AFX_DLGSAVESCREEN_H__25FDDF69_FBBC_4D68_BB41_5992C933726E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSaveScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveScreen dialog

class CDlgSaveScreen : public CDialog
{
// Construction
public:
	int GetFileType();
	void SetFilePath( const char *p_szFilePath);
	CDlgSaveScreen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSaveScreen)
	enum { IDD = IDD_SAVE_SCREEN };
	int		m_nWidth;
	int		m_nHeight;
	CString	m_strFileName;
	CString	m_strFilePath;
	int		m_nFileType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSaveScreen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSaveScreen)
	afx_msg void OnFiledialog();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSelchangeFiletype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVESCREEN_H__25FDDF69_FBBC_4D68_BB41_5992C933726E__INCLUDED_)
