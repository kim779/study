#if !defined(AFX_PREVIEW_H__16755945_28D7_4211_8948_A5A0F2200EEA__INCLUDED_)
#define AFX_PREVIEW_H__16755945_28D7_4211_8948_A5A0F2200EEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Preview.h : header file
//
#include "../../dll/lib/axislib.h"
#include "../awObject/show.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPreview dialog

class AFX_EXT_CLASS CPreview : public CDialog
{
// Construction
public:
	CPreview(CWnd* pParent = NULL);   // standard constructor
	void setShow(CAxisPalette* palette, CAxisDraw* draw, CRect rect, CString root, CString fileN, CString caption, CString backBrush);
public:
	CShow*		m_pShow;
	CRect		m_rect;
	CString		m_filename, m_root, m_caption;
	CAxisPalette*	m_palette;
	CAxisDraw*	m_draw;
	CBrush		*m_pBackBrush;

// Dialog Data
	//{{AFX_DATA(CPreview)
	enum { IDD = IDD_PREVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreview)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEW_H__16755945_28D7_4211_8948_A5A0F2200EEA__INCLUDED_)
