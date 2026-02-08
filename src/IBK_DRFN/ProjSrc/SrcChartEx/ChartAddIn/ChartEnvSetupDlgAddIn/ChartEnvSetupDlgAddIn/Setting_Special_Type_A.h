#if !defined(AFX_SETTING_SPECIAL_TYPE_A_H__2D659FC0_1A4C_4BD9_9CD7_4EADC4933C37__INCLUDED_)
#define AFX_SETTING_SPECIAL_TYPE_A_H__2D659FC0_1A4C_4BD9_9CD7_4EADC4933C37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Special_Type_A.h : header file
//

#include "BaseDialog.h"
#include "ColorButton.h"


/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_A dialog
//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CSetting_Special_Type_A : public CBaseDialog
{
// Construction
public:
	CSetting_Special_Type_A( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_Special_Type_A();

	virtual void Initial(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo* p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo* p_pIGraphInfo);
	// 20081014 박동관 <<

// Dialog Data
	//{{AFX_DATA(CSetting_Special_Type_A)
	enum { IDD = IDD_SPECIAL_TYPE_A };
	CButton	m_btnFillUp;
	CButton	m_btnFillDown;
	CBitmapItemComboBox	m_cmbThickness;
	CColorButton	m_btnUp;
	CColorButton	m_btnDown;	
	CString	m_strBlockChange;
	CString	m_strBlockSize;
	//}}AFX_DATA

// Attribute
private:
	int m_nSubGraphIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Special_Type_A)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void SetLineColor( void );				// 선색깔 세팅.
	
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Special_Type_A)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_SPECIAL_TYPE_A_H__2D659FC0_1A4C_4BD9_9CD7_4EADC4933C37__INCLUDED_)
