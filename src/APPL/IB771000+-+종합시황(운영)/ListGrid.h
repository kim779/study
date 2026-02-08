#pragma once
// ListGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListGrid window

class CListGrid : public CListCtrl
{
// Construction
public:
	CListGrid(CWnd* pParent = NULL);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListGrid)
	enum { IDD = IDD_MAIN };
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

