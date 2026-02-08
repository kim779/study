#if !defined(AFX_WNDTREECTRLOWNER_H__B911BF15_7DF4_4BAE_B9E0_8CEBF6BD075B__INCLUDED_)
#define AFX_WNDTREECTRLOWNER_H__B911BF15_7DF4_4BAE_B9E0_8CEBF6BD075B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndTreeCtrlOwner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndTreeCtrlOwner window
#include "../common_ST/DropTarget.h"

class CTreeCtrlSysTrading;
class CWndTreeCtrlOwner : public CWnd
{
// Construction
public:
	CWndTreeCtrlOwner(UINT nTreeType);

// Attributes
public:

// Operations
public:
	CWnd*	GetTreeCtrl();
	void	InitTreeCtrl(CImageList* pImageList);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndTreeCtrlOwner)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndTreeCtrlOwner();

private:
	void	ListupUserFunction();
	void	ListupStrategy();

	UINT						m_nTreeType;
	CTreeCtrlSysTrading*		m_pTree;

	CDropTarget m_DropTarget;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CWndTreeCtrlOwner)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDTREECTRLOWNER_H__B911BF15_7DF4_4BAE_B9E0_8CEBF6BD075B__INCLUDED_)
