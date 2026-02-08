/*****************************************************************************
  File: DDEdit.h

  Purpose: Header file which contains declarations for the COleEditDropTarget 
           class, and the CDDEdit class.

  Development Team: Steve Horne

  Written by Microsoft Product Support Services, Languages Developer Support
  Copyright (c) 1993 Microsoft Corporation. All rights reserved.
\****************************************************************************/

#include "afxole.h"

/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget
class COleEditDropTarget : public COleDropTarget
{
// Construction
public:
	COleEditDropTarget();

// Implementation
public:
	CString Parser(CString &srcstr, CString substr);
	virtual ~COleEditDropTarget();  
    
	//
	// These members MUST be overridden for an OLE drop target
	// See DRAG and DROP section of OLE classes reference
	//
	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	void	OnDragLeave(CWnd* pWnd);
	BOOL	OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );    
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CDDEdit window

class CDDEdit : public CEdit
{
	DECLARE_DYNCREATE(CDDEdit)

// Attributes
private:
	// Needed to make this control an OLE data SOURCE (see OnLButtonDown)
	COleDataSource m_COleDataSource;

// Operations
public:    
	//*****   
	// Initialization method REQUIRED for controls created by a dialog 
	// template dynamically created controls are initialized in the 
	// OnCreate. OnCreate is not called for controls created by a 
	// template since the Windows controls are created before the CWnd 
	// objects
	//*****
	void	Initialize();    

	//{{AFX_VIRTUAL(CDDEdit)
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual void OnEditCopy();
	virtual void OnEditPaste();

// Implementation
public:    
	//*****
	// COleEditDropTarget, derived from COleDropTarget gives us the 
	// functionality to be an OLE drop TARGET.
	// COleEditDropTarget.Register() must be called to register the CWnd 
	// as an OLE drop target. 
	//*****
	COleEditDropTarget m_DropTarget;
	HCURSOR	m_hCursor = nullptr;
	BOOL	m_bNoDrop = FALSE;
	BOOL	m_bSetFocusSel = FALSE;
	BOOL	m_bTracking = FALSE;

public:
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// Generated message map functions
	//{{AFX_MSG(CDDEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
