#if !defined(AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_)
#define AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageSetup.h : header file
//
#include "1Picture.h"
#include "../ctrl/colorcombo.h"
#include "../ctrl/themecombo.h"
#include "SetupDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CImageSetup dialog

class AFX_EXT_CLASS CImageSetup : public CSetupDialog
{
// Construction
public:
	CImageSetup(CWnd* pParent = NULL);   // standard constructor

	virtual void		ApplySetup();
	virtual void		DefaultSetup();

protected:
	CString		m_bmpFile;
	HBITMAP		m_bitmap;

	CPicture	m_picture;

	void		loadSetup();

public:
	CBitmap* m_bmpCombo;
	CBitmap* m_bmpCombo_N;
	void draw_groupBox(CPaintDC *pdc, int ids);
	CFont m_font;
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
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCbBkg();
	afx_msg void OnSelchangeCbPos();
	afx_msg void OnChkColor();
	afx_msg void OnChkDisplay();
	afx_msg void OnThemepreview();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Load(const char* file);
	void Save(const char* file);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGESETUP_H__4F8F8785_F55D_413D_82C4_CC78F734340E__INCLUDED_)
