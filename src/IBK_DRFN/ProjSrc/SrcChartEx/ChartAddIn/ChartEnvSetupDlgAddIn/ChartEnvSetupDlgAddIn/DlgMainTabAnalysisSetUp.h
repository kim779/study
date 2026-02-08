#if !defined(AFX_DLGMAINTABANALYSISSETUP_H__30DEB6EA_4F72_4F3C_878E_A8789FCC1EFD__INCLUDED_)
#define AFX_DLGMAINTABANALYSISSETUP_H__30DEB6EA_4F72_4F3C_878E_A8789FCC1EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMainTabAnalysisSetUp.h : header file
//

#include "Include_AddIn.h"		// for IChartOCX

#include "BaseTabDlg.h"
#include "EnvironmentData.h"				// data
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMainTabAnalysisSetUp dialog

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 3
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CDlgMainTabAnalysisSetUp : public CBaseTabDlg
{
	DECLARE_DYNAMIC( CDlgMainTabAnalysisSetUp)

// Construction
public:
	CDlgMainTabAnalysisSetUp( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL);   // standard constructor
	~CDlgMainTabAnalysisSetUp();   // standard destructor
	
// Attribute
public:
	BOOL m_bPreview;

// Implementation
public:
	void Save( void );
	void Recover( void );
	void PreView( void )	{ OnBtnPreview(); };

public:
	virtual CBaseTabDlg::DLG_TYPE GetDialogType() const;

// Dialog Data
	//{{AFX_DATA(CDlgMainTabAnalysisSetUp)
	enum { IDD = IDD_MAINTAB_PAGE_ANALYSIS };
	CColorButton	m_btnTrendLineColor;
	BOOL	m_bShowTrendLine;
	BOOL	m_bShowVertical;
	BOOL	m_bShowThree;
	BOOL	m_bShowHorizonLeft;
	BOOL	m_bShowHorizonRight;
	BOOL	m_bShowFour;
	BOOL	m_bShowFibo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMainTabAnalysisSetUp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMainTabAnalysisSetUp)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowTrend();
	afx_msg void OnShowVertical();
	afx_msg void OnShowHorizon();
	afx_msg void OnShowThree();
	afx_msg void OnShowFour();
	afx_msg void OnShowFibo();
	afx_msg void OnShowHorizonRight();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnRecover();
	afx_msg void OnBtnPreview();
	afx_msg void OnTrendlineColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	IChartOCX *m_pIChartOCX;
	
	CAnalysisToolData m_AnalysisToolData;
	CAnalysisToolData m_OrgAnalysisToolData;
	
	COLORREF m_clrTrendLineColor;
	COLORREF m_clrOrgTrendLineColor;


	void Initial();

// (2006/8/16 - Seung-Won, Bae) Info Get/Set Library Functions
protected:
	void	GetAnalysisToolData( CAnalysisToolData& analysisToolData);
	void	SetAnalysisToolColor( COLORREF p_clrColor);

// (2006/8/22 - Seung-Won, Bae) Shere the Analysis Tool Color within Analysis and Style Setting.
public:
	void	UpdateAnalysisColor( COLORREF p_clrAnalysis);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTABANALYSISSETUP_H__30DEB6EA_4F72_4F3C_878E_A8789FCC1EFD__INCLUDED_)
