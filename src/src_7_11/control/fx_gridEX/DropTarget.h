#if !defined(AFX_DROPTARGET_H__D8D4A7A6_7E02_42FE_AACC_24021F7824DF__INCLUDED_)
#define AFX_DROPTARGET_H__D8D4A7A6_7E02_42FE_AACC_24021F7824DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DropTarget.h : header file
//

#include <afxole.h>
class CfxGrid;
/////////////////////////////////////////////////////////////////////////////
// CDropTarget document

class CDropTarget : public COleDropTarget
{
public:
	CDropTarget();           // protected constructor used by dynamic creation
	virtual ~CDropTarget();

// Attributes
public:
	CfxGrid*	m_fxGrid;
	BOOL		m_registered;

// Operations
public:
	BOOL	Register(CfxGrid *pfxGrid);
	virtual void Revoke();

	BOOL        OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	DROPEFFECT  OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	void        OnDragLeave(CWnd* pWnd);
	DROPEFFECT  OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	DROPEFFECT  OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropTarget)
	//}}AFX_VIRTUAL

// Implementation
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDropTarget)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPTARGET_H__D8D4A7A6_7E02_42FE_AACC_24021F7824DF__INCLUDED_)
