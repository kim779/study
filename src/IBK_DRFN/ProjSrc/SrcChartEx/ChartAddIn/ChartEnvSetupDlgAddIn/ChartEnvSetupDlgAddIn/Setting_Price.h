#if !defined(AFX_SETTING_PRICE_H__9288FAD6_F5DC_4C25_B8DF_1778381123F4__INCLUDED_)
#define AFX_SETTING_PRICE_H__9288FAD6_F5DC_4C25_B8DF_1778381123F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Price.h : header file
//

#include "BaseDialog.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CSetting_Price dialog
//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 9 / 8
// Explain		: 
// Using way	: 가격차트
//-----------------------------------------------------------------------------
class CSetting_Price : public CBaseDialog
{
// Construction
public:
	CSetting_Price( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_Price();

// Dialog Data
	//{{AFX_DATA(CSetting_Price)
	enum { IDD = IDD_PRICE };
	CColorButton	m_btnDown;
	CColorButton	m_btnSteady;
	CColorButton	m_btnUp;
	CStatic	m_Thickness_Static;
	CStatic	m_DrawStyle_Static;
	CButton	m_FillDown_Bt;
	CButton	m_FillUp_Bt;
	CButton	m_InvertChart_Bt;
	CButton	m_Log_Bt;
	CBitmapItemComboBox	m_Thickness_Combo;
	CBitmapItemComboBox	m_DrawStyle_Combo;
	int		m_nSteadyType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Price)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Price)
	virtual BOOL OnInitDialog();
	afx_msg void OnEquiVolumeChart();	
	afx_msg void OnCandleVolumeChart();	
	afx_msg void OnFlowChart();
	afx_msg void OnNewBarChart();
	afx_msg void OnBarChart();
	afx_msg void OnBongChart();
	afx_msg void OnLineChart();
	afx_msg void OnSteadyNormal();
	afx_msg void OnSteadyBeforeclose();
	afx_msg void OnSteadySameclose();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnSteady();
	afx_msg void OnSelchangeThickness();
	afx_msg void OnSelchangeDrawstyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void Initial( IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply( IGraphInfo *p_pIGraphInfo);
	//virtual void OnApply( IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 <<

protected:
	BOOL m_bOrgLog;
	BOOL m_bOrgInvertChart;
	BOOL m_bUseBasePrice;

protected:
	void OnApply_ColorButton( void);
	void ShowHide_FillUpDownNCombo(int nState);
	void ShowHide_SteadyRadio(BOOL bShow);
	void Initial_ColorButton( void);
	void Initial_ColorBtn( int nCount);
	void Set_DrawStyle();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_PRICE_H__9288FAD6_F5DC_4C25_B8DF_1778381123F4__INCLUDED_)
