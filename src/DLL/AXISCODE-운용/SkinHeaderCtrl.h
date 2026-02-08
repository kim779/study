#pragma once
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
	//}}AFX_MSG

//	CMyBitmap m_bitmapSpan;

	DECLARE_MESSAGE_MAP()
};

