#if !defined(AFX_DLGYSCALESETUP_H__ECCF4646_4CAA_4D8E_B53B_D195821AF3BE__INCLUDED_)
#define AFX_DLGYSCALESETUP_H__ECCF4646_4CAA_4D8E_B53B_D195821AF3BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgYScaleSetUp.h : header file
//

#include "YScaleSetUpAddInImp.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgYScaleSetUp dialog

class CDlgYScaleSetUp : public CDialog
{
// Construction
public:
	CDlgYScaleSetUp( CYScaleSetUpAddInImp *pAddIn, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode, UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgYScaleSetUp)
	enum { IDD = IDD_YSCALE_SETUP_HTS };
	int m_nYScaleMaxMinSetupType;
	int m_nSelectPercent;
	CString	m_strYesterdayClosePrice;
	CString	m_strMaxPrice;
	CString	m_strMinPrice;
	//}}AFX_DATA
	CString	m_strUpperBound;
	CString	m_strLowerBound;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgYScaleSetUp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attribute
protected:
	CYScaleSetUpAddInImp *m_pAddIn;
	
	// 설정값 / 비율 선택에 따라 Control들이 이동해야 되는 높이
	int m_nHeightToMoving;


	BOOL m_bShow;

	double m_dYesterdayClosePrice;
	double	m_dUpperBound;
	double	m_dLowerBound;

	// 설정값 - 최대 / 최소값
	double m_dYScaleMaxValue;
	double m_dYScaleMinValue;
		
	// 비율 - 최대 / 최소값
	double m_dYScaleMaxPercentValue;
	double m_dYScaleMinPercentValue;
	
	CString m_strValueFormat;


// Implementation
public:
	void SetValueFormat( CString strFormat );

	void MovingAllControls( void );
	void MoveControlPos( int nID, BOOL bSign );
	
	int GetYScaleMaxMinSetupType( void );
	void SetYScaleMaxMinSetupType( int nYScaleMinMaxSetupType );

	double GetYScaleMax( void );
	double GetYScaleMin( void );

	void SetYScaleMaxMin( double dMax, double dMin );
	void SetYScaleMaxMinPercent( double dMax, double dMin );
	void SetStockPriceInfo( double dYesterpayClosePrice, double dUpperBound, double dLowerBound );

	// 20081016 이주표 >>
	CString m_strPacketType;
	void SetPacketType(CString strType)
	{
		m_strPacketType = strType;
	}
	//	<<
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgYScaleSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnUseSetupValue();
	afx_msg void OnUsePercent();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void On30Percent();
	afx_msg void On50Percent();
	afx_msg void On70Percent();
	afx_msg void On100Percent();
	afx_msg void OnChangeMaxPriceEdit();
	afx_msg void OnChangeMinPriceEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2007/5/2 - Seung-Won, Bae) Check Market Type for Unit.
public:
	CPacketType::COMMODITY_BOND_NAME	m_eMarketType;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;
// (2009/9/14 - Seung-Won, Bae) for World-On
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGYSCALESETUP_H__ECCF4646_4CAA_4D8E_B53B_D195821AF3BE__INCLUDED_)
