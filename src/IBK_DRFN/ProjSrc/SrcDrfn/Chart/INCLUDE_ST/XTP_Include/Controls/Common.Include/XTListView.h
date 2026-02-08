// XTListView.h interface for the CXTListView class.
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
#if !defined(__XTLISTVIEW_H__)
#define __XTLISTVIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Summary:  CXTListView is a multiple inheritance class derived from CListView and
//			 CXTListCtrlBase. This class implements flat header and generic sorting
//			 routines.  
// See Also: CXTListCtrlBase
class _XT_EXT_CLASS CXTListView : public CListView, public CXTListCtrlBase
{
    DECLARE_DYNCREATE(CXTListView)

public:
    
    // Summary: Constructs a CXTListView object
    CXTListView();

    // Summary: Destroys a CXTListView object, handles cleanup and de-allocation
    virtual ~CXTListView();

	// Input:	dwExStyle  - DWORD value that specifies the extended list-view control style. This
	//			parameter can be a combination of Extended List-View Styles.
    // Summary:	This member function is called to set extended styles for the list
	//			control, ie: LVS_EX_FULLROWSELECT, LVS_EX_GRIDLINES, etc. See MSDN
	//			documentation for a complete list of available styles.
    virtual void SetExtendedStyle(DWORD dwExStyle );

	// Returns: A DWORD value representing the extended style of the list control.
    // Summary:	This member function is called to return the list control extended
	//			style. 
    DWORD GetExtendedStyle();

protected:

public:
    //:Ignore
	//{{AFX_VIRTUAL(CXTListView)
    public:
    virtual void OnInitialUpdate();
    protected:
    virtual void OnDraw(CDC* pDC);      
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL
	//:End Ignore

    //:Ignore
#ifdef _DEBUG
    
	virtual void AssertValid() const;
    
	virtual void Dump(
		CDumpContext& dc) const;

#endif
	//:End Ignore

    //:Ignore
	//{{AFX_MSG(CXTListView)
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    //}}AFX_MSG
	//:End Ignore

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE DWORD CXTListView::GetExtendedStyle() {
	return CXTListCtrlBase::GetExtendedStyle();
}
AFX_INLINE void CXTListView::SetExtendedStyle(DWORD dwExStyle){
	CXTListCtrlBase::SetExtendedStyle(dwExStyle);
}

//:Ignore
//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTLISTVIEW_H__)
//:End Ignore