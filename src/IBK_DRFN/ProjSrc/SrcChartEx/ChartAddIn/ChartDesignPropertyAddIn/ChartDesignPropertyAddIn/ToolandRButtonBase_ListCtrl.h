////////////////////////////////////////////////////////////////////
// CToolandRButtonBase_ListCtrl -- OLE drap & drop
//               리스트간의 Drag&Drop --> 이동(false)&복사(true)가능
//								
//
// Author: Jean-Claude DAUPHIN, CI/INF UNESCO
//

#if !defined(AFX_TOOLANDRBUTTONBASE_LISTCTRL_H__3CAF3FBB_110F_4C17_941F_CAD50E67FC71__INCLUDED_)
#define AFX_TOOLANDRBUTTONBASE_LISTCTRL_H__3CAF3FBB_110F_4C17_941F_CAD50E67FC71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "message.h"

typedef struct
{
	DWORD	hItem;
	char	ChartName[64];
} ToolItem;

/////////////////////////////////////////////////////////////////////////////
// COleListCtrlDropTarget window

class COleListCtrlDropTarget : public COleDropTarget
{
// Construction
public:
    COleListCtrlDropTarget();

// Implementation
public:
    virtual ~COleListCtrlDropTarget();   
    
   /**
    * These members MUST be overridden for an OLE drop target
    * See DRAG and DROP section of OLE classes reference
    **/
   DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                                dwKeyState, CPoint point );
   DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD 
                                               dwKeyState, CPoint point );
   void OnDragLeave(CWnd* pWnd);               
   
   BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT 
                                          dropEffect, CPoint point );
   
   void SetCurrentWindow(CWnd* pWnd);   

private:
	CWnd* m_pWnd;
};

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonBase_ListCtrl window

class CToolandRButtonBase_ListCtrl : public CListCtrl
{

// Construction
public:
	CToolandRButtonBase_ListCtrl();
	virtual ~CToolandRButtonBase_ListCtrl();

// Attributes
private: 
	int SelectedItemCount();
	 CString SelectedItemNames();
	//draganddrop을 위한 변수이다.
	 COleDataSource m_COleDataSource;
	 COleListCtrlDropTarget m_DropTarget;

public:
   //draganddrop을 위해서 초기화한다.
   void InitDropTarget();

// Operations
public:
    /*****   
     * Initialization method REQUIRED for controls created by
     * a dialog template. Dynamically created controls are  
     * initialized in the OnCreate. OnCreate is not called for 
	 * controls created by a template since the Windows controls
	 * are created before the CWnd objects.
     *****/
 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolandRButtonBase_ListCtrl)
	//}}AFX_VIRTUAL


// Implementation
public:
    /*****
     * COleListCtrlDropTarget, derived from COleDropTarget gives us
     * the functionality to be an OLE drop target.
     * COleListCtrlDropTarget.Register() must be called to register 
     * the CWnd as an OLE drop target. 
     *****/
	// Generated message map functions
protected:

	//{{AFX_MSG(CToolandRButtonBase_ListCtrl)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLANDRBUTTONBASE_LISTCTRL_H__3CAF3FBB_110F_4C17_941F_CAD50E67FC71__INCLUDED_)
