#if !defined(AFX_PROPERTIES_H__D5A87B6B_8F52_4CA9_9E41_F6A9A200916D__INCLUDED_)
#define AFX_PROPERTIES_H__D5A87B6B_8F52_4CA9_9E41_F6A9A200916D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Properties.h : header file
//
#include "SymbolBaseData.h"

#include "resource.h"
#include "PreView.h"
#include "ColorButton.h"
/////////////////////////////////////////////////////////////////////////////
// CProperties dialog

class CPropertiesData_text;
class CProperties : public CDialog
{
// Construction
public:
	CProperties(CPropertiesData_text* pProperties, const CSymbolBaseData::TOOLTYPE SymbolTool, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProperties)
	enum { IDD = IDD_PROPERTIES };
	CColorButton	m_Color_Bt;
	CPreView	m_PreView_Static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProperties)
	afx_msg void OnColor();
	afx_msg void OnFont();
	afx_msg void OnSelchangeSymbolSize();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetBackgroundColorInViewer(const COLORREF& color);

private:
	CPropertiesData_text* m_pProperties;
	CSymbolBaseData::TOOLTYPE m_eSymbolTool;

	void EnableWindow_FontBt();
	bool DoesUseFontSymbol() const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTIES_H__D5A87B6B_8F52_4CA9_9E41_F6A9A200916D__INCLUDED_)
