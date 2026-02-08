#if !defined(AFX_FXFONTDIALOG_H__360A42E5_7643_4466_9EE2_37C858C3D4BB__INCLUDED_)
#define AFX_FXFONTDIALOG_H__360A42E5_7643_4466_9EE2_37C858C3D4BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxFontDialog.h : header file
//

#include "colorCombo.h"
#include "fontNameCombo.h"
#include "fontSizeCombo.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CfxFontDialog dialog

class AFX_EXT_CLASS CfxFontDialog : public CDialog
{
// Construction
public:
	CfxFontDialog(CString home = _T(""), CWnd* pParent = NULL);   // standard constructor
	CfxFontDialog(CString faces, int point, bool italic, bool bold, DWORD fgColor, DWORD bgColor,
			CString home = _T(""), CWnd* pParent = NULL);
	virtual	~CfxFontDialog();

// Dialog Data
	//{{AFX_DATA(CfxFontDialog)
	enum { IDD = IDD_FONTDLG };
	CcolorCombo	m_bgCB;
	CcolorCombo	m_fgCB;
	CComboBox	m_styleCB;
	CfontSizeCombo	m_sizeCB;
	CfontNameCombo	m_nameCB;
	//}}AFX_DATA

	CString	m_home;
	CString	m_faces;
	CString	m_points;
	DWORD	m_style;
	DWORD	m_actual;
	DWORD	m_current;
	DWORD	m_fgColor;
	DWORD	m_bgColor;

// Operation
public:
	BOOL	Create(CWnd* parent = NULL);
	void	SetFont(CString faces, int point, bool italic, bool bold, DWORD fgColor, DWORD bgColor, bool applyNow = false);
	void	GetFont(CString& faces, int& point, bool& italic, bool& bold, DWORD& fgColor, DWORD& bgColor);
	CString	GetFontFaceName();
	int	GetFontSize();
	bool	IsFontItalic();
	bool	IsFontBold();
	DWORD	GetTextColor();
	DWORD	GetBkColor();
	void	Enable(bool eFace = true, bool eSize = true, bool eStyle = true, bool eFG = true, bool eBK = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxFontDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	fillFontNames();
	void	fillFontSizes();

	// Generated message map functions
	//{{AFX_MSG(CfxFontDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFaceNameEditUpdate();
	afx_msg void OnFaceNameSelChange();
	afx_msg void OnPointEditUpdate();
	afx_msg void OnPointSelChange();
	afx_msg void OnStyleEditChange();
	afx_msg void OnStyleSelChange();
	afx_msg void OnFgColorSelChange();
	afx_msg void OnBgColorSelChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXFONTDIALOG_H__360A42E5_7643_4466_9EE2_37C858C3D4BB__INCLUDED_)
