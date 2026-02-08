#if !defined(AFX_SETTING_SPECIAL_TYPE_D_H__F7974FCF_CC7E_433D_AF17_B8FA3EAFFB4D__INCLUDED_)
#define AFX_SETTING_SPECIAL_TYPE_D_H__F7974FCF_CC7E_433D_AF17_B8FA3EAFFB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Special_Type_D.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_D dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Comments		: 
// Using way	: 
// See			: CBaseDialog
//-----------------------------------------------------------------------------
class CSetting_Special_Type_D : public CBaseDialog
{
// Construction
public:
	CSetting_Special_Type_D( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetting_Special_Type_D)
	enum { IDD = IDD_SPECIAL_TYPE_D };
	CColorButton	m_btnColorStart;
	CColorButton	m_btnColorEnd;
	CColorButton	m_btnColorTurnUp;
	CColorButton	m_btnColorBuy;
	CColorButton	m_btnColorKeepBuy;
	CColorButton	m_btnColorHoldBuy;
	CColorButton	m_btnColorTurnDown;
	CColorButton	m_btnColorSell;
	CColorButton	m_btnColorKeepSell;
	CColorButton	m_btnColorHoldSell;
	CBitmapItemComboBox	m_cmbThickness;
	CNumSpinCtrl	m_spnCond1;
	CNumEdit	m_edtCond1;
	//}}AFX_DATA

// Attribute
private:
	int m_nSubGraphIndex;

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Special_Type_D)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Initial(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo *p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 <<

protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Special_Type_D)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	void OnBtnColor(UINT nID);	// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_SPECIAL_TYPE_D_H__F7974FCF_CC7E_433D_AF17_B8FA3EAFFB4D__INCLUDED_)
