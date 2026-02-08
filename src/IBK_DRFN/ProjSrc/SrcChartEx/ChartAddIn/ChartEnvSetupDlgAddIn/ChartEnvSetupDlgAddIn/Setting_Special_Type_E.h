#if !defined(AFX_SETTING_SPECIAL_TYPE_E_H__789C2535_4882_455C_98E8_C148488BD2BD__INCLUDED_)
#define AFX_SETTING_SPECIAL_TYPE_E_H__789C2535_4882_455C_98E8_C148488BD2BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Special_Type_E.h : header file
//

#include "BaseDialog.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_E dialog

class CSetting_Special_Type_E : public CBaseDialog
{
// Construction
public:
	CSetting_Special_Type_E( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting_Special_Type_E)
	enum { IDD = IDD_SPECIAL_TYPE_E };
	CColorButton	m_btnColor6;
	CColorButton	m_btnColor5;
	CColorButton	m_btnColor4;
	CColorButton	m_btnColor3;
	CColorButton	m_btnColor2;
	CColorButton	m_btnColor1;
	CBitmapItemComboBox	m_cmbLineType;
	int	m_nTextType;
	CNumSpinCtrl	m_spnCond1;
	CNumEdit	m_edtCond1;
	int	m_nHorizontalBarType;
	BOOL	m_bIsIndividually;
	//}}AFX_DATA

// Attribute
private:
	int m_nSubGraphIndex;

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Special_Type_E)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void Init_ColorButton( void );

public:
	virtual void Initial(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo *p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 <<

protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Special_Type_E)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColor1();
	afx_msg void OnBtnColor2();
	afx_msg void OnBtnColor3();
	afx_msg void OnBtnColor4();
	afx_msg void OnBtnColor5();
	afx_msg void OnBtnColor6();
	afx_msg void OnGuganRadio();
	afx_msg void OnHogaRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_SPECIAL_TYPE_E_H__789C2535_4882_455C_98E8_C148488BD2BD__INCLUDED_)
