#pragma once
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

