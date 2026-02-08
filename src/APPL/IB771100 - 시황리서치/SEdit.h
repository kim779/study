#pragma once
// SEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSEdit window

class CSEdit : public CEdit
{
// Construction
public:
	CSEdit(CMapDlg *mapdlg);

// Attributes
public:
	CMapDlg* m_pMapDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

