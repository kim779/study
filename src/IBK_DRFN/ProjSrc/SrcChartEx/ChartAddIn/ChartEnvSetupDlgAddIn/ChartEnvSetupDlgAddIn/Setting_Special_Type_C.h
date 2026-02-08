#if !defined(AFX_SETTING_SPECIAL_TYPE_C_H__B9ADBA1D_6BBB_436B_94E5_A06E729FEF6A__INCLUDED_)
#define AFX_SETTING_SPECIAL_TYPE_C_H__B9ADBA1D_6BBB_436B_94E5_A06E729FEF6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Special_Type_C.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_C dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CSetting_Special_Type_C : public CBaseDialog
{
// Construction
public:
	CSetting_Special_Type_C( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting_Special_Type_C)
	enum { IDD = IDD_SPECIAL_TYPE_C };
	CColorButton	m_btnUp;
	CColorButton	m_btnDown;
	CNumSpinCtrl	m_spnCond1;
	CNumEdit	m_edtCond1;
	CString	m_strBlockChange;
	//}}AFX_DATA
//	CBitmapItemComboBox	m_cmbThickness;


// Attribute
private:
	int m_nSubGraphIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Special_Type_C)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<
	
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Special_Type_C)
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_SPECIAL_TYPE_C_H__B9ADBA1D_6BBB_436B_94E5_A06E729FEF6A__INCLUDED_)
