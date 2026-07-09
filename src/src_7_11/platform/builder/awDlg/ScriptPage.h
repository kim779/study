#if !defined(AFX_SCRIPTPAGE_H__64821567_DCC7_4651_B00E_08DF206809A4__INCLUDED_)
#define AFX_SCRIPTPAGE_H__64821567_DCC7_4651_B00E_08DF206809A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptPage.h : header file
//
#include "resource.h"
#include "../Components/fxColorCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CScriptPage dialog

class CScriptPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CScriptPage)

// Construction
public:
	void InvalidateSample();
	CScriptPage();
	~CScriptPage();

	CBrush		m_brushCLR;
	COLORREF	m_clBkGnd, m_clText;
	COLORREF	m_clSymbol;
	COLORREF	m_clComment;
	COLORREF	m_clConstant;
	COLORREF	m_clKeyword;
	COLORREF	m_clString;
	COLORREF	*m_refColor;

// Dialog Data
	//{{AFX_DATA(CScriptPage)
	enum { IDD = IDD_PROPPAGE_SCRIPT };
	CComboBox	m_colorItemCBO;
	CfxColorCombo	m_colorCBO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScriptPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScriptPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelendokColorlistCbo();
	afx_msg void OnSelendokSetcolorCbo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDefaultButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTPAGE_H__64821567_DCC7_4651_B00E_08DF206809A4__INCLUDED_)
