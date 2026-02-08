#if !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
#define AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RscDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRscDlg dialog

class CRscDlg : public CDialog
{
// Construction
public:
	CRscDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL ); // standard constructor

	CBrush	m_brBkColor;
	COLORREF m_crBk;

	// ½ºÅ²
	virtual void ChangeSkinColor(COLORREF clrBackground);
	virtual void SetBkColor(COLORREF crBk) {m_crBk=crBk;}

// Dialog Data
	//{{AFX_DATA(CRscDlg)
	enum { IDD = NULL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRscDlg)
	public:
	virtual BOOL Create(UINT nID,CWnd* pParentWnd);
	virtual int DoModal();
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRscDlg)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSCDLG_H__6000F270_B94B_4D29_BF0C_B65C7348330A__INCLUDED_)
