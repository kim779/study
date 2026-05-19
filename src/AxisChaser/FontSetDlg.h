#if !defined(AFX_FONTSETDLG_H__E6EF3805_CAB7_4196_BD14_74C785001F94__INCLUDED_)
#define AFX_FONTSETDLG_H__E6EF3805_CAB7_4196_BD14_74C785001F94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontSetDlg dialog

class CFontSetDlg : public CDialog
{
// Construction
public:
	CFontSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFontSetDlg)
	enum { IDD = IDD_FONTSETTING };
	//}}AFX_DATA

	CString	GetTFontName(int idx)	{ return m_fName[idx]; }
	int	GetTFontSize(int idx)	{ return m_fSize[idx]; }
	BOOL	IsBold(int idx)		{ return m_bBold[idx]; }
	BOOL	IsItalic(int idx)	{ return m_bItalic[idx]; }
	BOOL	IsUline(int idx)	{ return m_bUline[idx]; }
	BOOL	IsStrike(int idx)	{ return m_bStrike[idx]; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	m_fName[5];
	int	m_fSize[5];
	BOOL	m_bBold[5];
	BOOL	m_bItalic[5];
	BOOL	m_bUline[5];
	BOOL	m_bStrike[5];
	
	void	loadInfo();
	void	saveInfo();
	void	ChangeFont(int kind);
	// Generated message map functions
	//{{AFX_MSG(CFontSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetallfont();
	afx_msg void OnSetsnrfont();
	afx_msg void OnSetrtmfont();
	afx_msg void OnSetsysfont();
	afx_msg void OnSetrptfont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSETDLG_H__E6EF3805_CAB7_4196_BD14_74C785001F94__INCLUDED_)
