#pragma once
// ItemConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemConfigDlg dialog

class CItemConfigDlg : public CDialog
{
// Construction
public:
	CItemConfigDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd*								m_pMain;
	CfxGrid*							m_pRemainGrid;

	void								CreateGrid();
	int									insertGridRow(int row, bool bheader=false);


// Dialog Data
	//{{AFX_DATA(CItemConfigDlg)
	enum { IDD = IDD_ITEMCONFIGDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

