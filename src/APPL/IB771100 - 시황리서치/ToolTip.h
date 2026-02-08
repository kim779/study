#pragma once
// ToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTip window

class CToolTip : public CToolTipCtrl
{
// Construction
public:
	CToolTip();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTip)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTip();

	// Generated message map functions

	

protected:
	//{{AFX_MSG(CToolTip)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

