// XTStatusBar.h : header file
//
// This file is a part of the XTREME TOOLKIT MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#ifndef __XTSTATUSBAR_H__
#define __XTSTATUSBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Summary: XT_STATUSPANE is a stand alone structure class. It is used to create
//			an XT_STATUSPANE structure.
struct XT_STATUSPANE
{
	UINT	nID;		// IDC of indicator: 0 => normal text area.
	int		cxText;		// Width of the string area in pixels. On both sides there is a 3 pixel gap and a 1 pixel border, making a pane 6 pixels wider.
	UINT	nStyle;		// Style flags (SBPS_*).
	UINT	nFlags;		// State flags (SBPF_*).
	CString strText;	// Text in the pane.
};

//////////////////////////////////////////////////////////////////////
// Summary: XT_STATUSPANE_CTRL is a stand alone helper structure class. It is used
//			by the CXTStatusBar class.
struct XT_STATUSPANE_CTRL
{
	CWnd* pWnd;			// A pointer to a valid CWnd object.
	UINT  nID;			// ID of the indicator pane.
	BOOL  bAutoDelete;  // TRUE if the control is to be deleted when destroyed.
};

//////////////////////////////////////////////////////////////////////
// Summary: CXTStatusBar is a CStatusBar derived class. It allows you to easily
//			insert controls into your status bar and remove controls from your
//			status bar.
class _XT_EXT_CLASS CXTStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CXTStatusBar)

public:

    // Summary: Constructs a CXTStatusBar object
	CXTStatusBar();

	// Summary: Destroys a CXTStatusBar object, handles cleanup and de-allocation
    virtual ~CXTStatusBar();

// Attributes
protected:

	CArray <XT_STATUSPANE_CTRL*,XT_STATUSPANE_CTRL*> m_arControls;  // Array of pane controls

// Operations
public:

	// Summary: This member function is called by the status bar to reposition pane
	//			item controls.
	void PositionControls();

	// Input:	nID - ID of the indicator pane.
	// Returns: A pointer to an XT_STATUSPANE_CTRL struct.
	// Summary:	Call this member function to retrieve information for the specified
	//			indicator pane. 
	XT_STATUSPANE_CTRL* GetPaneControl(UINT nID);

	// Input:	pWnd - Points to a control window.
	//			nID - ID of the indicator pane.
	//			bAutoDelete - TRUE if the control is to be deleted when destroyed.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary:	Call this member function to add a control to an indicator pane. 
	BOOL AddControl(CWnd* pWnd,UINT nID,BOOL bAutoDelete=TRUE);

	// Input:	nIndex - Index of the indicator pane.
	//			cxText - New width for the indicator pane.
	// Summary:	Call this member function to set the width for an indicator pane.
	void SetPaneWidth(int nIndex,int cxText);

	// Input:	nID - ID of the indicator pane.
	//			nIndex - Index of the indicator pane.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary:	Call this member function to add an indicator pane to the status bar.
	BOOL AddIndicator(UINT nID,int nIndex);

	// Input:	nID - ID of the indicator pane.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary:	Call this member function to remove an indicator pane from the status bar. 
	BOOL RemoveIndicator(UINT nID);

	// Input:	nIndex - Index of the indicator pane.
	//			pSBP - Address of an XT_STATUSPANE struct.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary:	Call this member function to retrieve information for an indicator pane. 
	BOOL GetPaneInfoEx(int nIndex,XT_STATUSPANE* pSBP);

	// Input:	nIndex - Index of the indicator pane.
	//			pSBP - Address of an XT_STATUSPANE struct.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary:	Call this member function to set the information for an indicator pane.
	BOOL SetPaneInfoEx(int nIndex,XT_STATUSPANE* pSBP);

	// Input:	nIndex - Index of the indicator pane.
	// Returns: A pointer to an XT_STATUSPANE struct.
	// Summary:	Call this member function to get the pane information for the given index.
	XT_STATUSPANE* GetPanePtr(int nIndex) const;

	// Returns: An integer value that represents the number of panes for the status bar. 
	// Summary:	Call this member function to return the number of panes that are created
	//			for the status bar. 
	int GetPaneCount() const;

	//{{AFX_VIRTUAL(CXTStatusBar)
	//}}AFX_VIRTUAL

protected:

    //:Ignore
	//{{AFX_MSG(CXTStatusBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTStatusBar::GetPaneCount() const {
	return m_nCount;
}

//:Ignore
//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __XTSTATUSBAR_H__
//:End Ignore