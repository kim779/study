#pragma once
// CodeCell.h : header file
//

#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeCell window

class CCodeCell : public CWnd
{
// Construction
public:
	CCodeCell(CString strTabPath);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCell)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCell();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCell)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CfxCodeCtrl* m_pCodeCtrl;
	CfxImgButton* m_pCodeButton;
private:
	CString m_strTabPath;

};

