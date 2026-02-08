#if !defined(AFX_DLGPRINTCFG_H__61FFEAB2_67C0_41A7_88A6_3ECF2B252D57__INCLUDED_)
#define AFX_DLGPRINTCFG_H__61FFEAB2_67C0_41A7_88A6_3ECF2B252D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrintCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPrintCfg dialog

class CDlgPrintCfg : public CDialog
{
// Construction
public:
	HDC GetHDC();
	BOOL PrintAllByOnePageDataCount();
	BOOL GetPrintColor();
	BOOL GetPrintRange();
	CDlgPrintCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPrintCfg)
	enum { IDD = IDD_PRINT };
	CButton	m_PrintAllByOnePageCount_Bt;
	int		m_nPrintColor;
	int		m_nPrintRange;
	BOOL	m_nPrintAllbyOnePageDataCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrintCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrintCfg)
	virtual void OnOK();
	afx_msg void OnButtonPrintsetting();
	afx_msg void OnAll();
	afx_msg void OnSelectionrange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EnableCheckButton();
	HDC m_HDC;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRINTCFG_H__61FFEAB2_67C0_41A7_88A6_3ECF2B252D57__INCLUDED_)
