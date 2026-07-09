#if !defined(AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_)
#define AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManageInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManageInfo dialog

class CManageInfo : public CDialog
{
// Construction
public:
	CManageInfo(CString home, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManageInfo)
	enum { IDD = IDD_MNGINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManageInfo)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int	m_width;
	int	m_height;
	int	m_pos;
	class CMTable*	m_table;
protected:
	CRect	m_baseRc;
	CString	m_home;
public:
	void	Init();
	void	ChangePalette();
	void	ChangeData();
	void	ShowSlide(CRect, int itime = 3000);
	void	HideSlide();
	void	SetData(CString dat, int kind, int pos = 0);
	void	StopSlide();	
	void    SetContent(CString strdata);
protected:
	void	Slide();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_)
