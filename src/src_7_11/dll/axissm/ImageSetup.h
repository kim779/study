#if !defined(AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_)
#define AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageSetup.h : header file
//
#include "../ctrl/colorcombo.h"
#include "../ctrl/themecombo.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CImageSetup dialog

class AFX_EXT_CLASS CImageSetup : public CDialog
{
// Construction
public:
	CImageSetup(CString home, CString user, CWnd* pParent = NULL);   // standard constructor

	void		ApplySetup();
	void		DefaultSetup();

protected:
	CString		m_home;
	CString		m_user;

	CString		m_bmpFile;
	HBITMAP		m_bitmap;

	void		loadSetup();

public:
// Dialog Data
	//{{AFX_DATA(CImageSetup)
	enum { IDD = IDD_IMAGESETUP };
	CthemeCombo	m_cbTheme;
	CcolorCombo	m_cbColor;
	CStatic	m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnFilefind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_)
