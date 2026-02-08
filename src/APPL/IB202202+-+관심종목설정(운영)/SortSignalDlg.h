#pragma once
// SrotSignalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSrotSignalDlg dialog

class CSortSignalDlg : public CDialog
{
// Construction
public:
	CSortSignalDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSortSignalDlg)
	enum { IDD = IDD_SORT_SIGNAL };
	CStatic	m_stcUpDown;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortSignalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
private:
	CString m_strUp;
	CString m_strDown;

// Operators
public:
	void SetSortSignal(int nCol, BOOL bUpDown);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSortSignalDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

