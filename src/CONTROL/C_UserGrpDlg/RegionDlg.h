#pragma once
// RegionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegionDlg dialog

class CRegionDlg : public CDialog
{
// Construction
public:
	CRegionDlg(int count, CString CRregion, CWnd* pParent = NULL);   // standard constructor

	int		m_Count;
	CString	m_sRegion;
// Dialog Data
	//{{AFX_DATA(CRegionDlg)
	enum { IDD = IDD_REGION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void	Init();

	// Generated message map functions
	//{{AFX_MSG(CRegionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
