#if !defined(AFX_SKINHEADERCTRL_H__8B0847B1_B4E6_4372_A62D_038582FFEA5C__INCLUDED_)
#define AFX_SKINHEADERCTRL_H__8B0847B1_B4E6_4372_A62D_038582FFEA5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinHeaderCtrl.h : header file
//

#include "sfont.h"

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl window

class CSkinHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CSkinHeaderCtrl();

// Attributes
public:

// Operations
public:
	CSFont	m_Font;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CSkinHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinHeaderCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

//	CMyBitmap m_bitmapSpan;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINHEADERCTRL_H__8B0847B1_B4E6_4372_A62D_038582FFEA5C__INCLUDED_)
