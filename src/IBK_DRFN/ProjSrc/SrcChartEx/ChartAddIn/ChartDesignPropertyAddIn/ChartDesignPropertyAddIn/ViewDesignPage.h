#if !defined(AFX_VIEWDESIGNPAGE_H__8AC44F9D_0C74_4E8C_8C30_C551D940FAE1__INCLUDED_)
#define AFX_VIEWDESIGNPAGE_H__8AC44F9D_0C74_4E8C_8C30_C551D940FAE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDesignPage.h : header file
//

#include "resource.h"
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage dialog

interface IPropertyMap;
class CViewDesignPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CViewDesignPage)

// Construction
public:
	CViewDesignPage();
	~CViewDesignPage();

	//Set Received Value From ocx
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// Dialog Data
	//{{AFX_DATA(CViewDesignPage)
	enum { IDD = IDD_PROPPAGE_VIEWDESIGN };
	CNoSystemMenuEdit	m_CtrlDataCountPerOnePage;
	int		m_nBorderShape;
	BOOL	m_bDisplayTotalData;
	int		m_nDataCountPerOnePage;
	int		m_nLeftSpace;
	int		m_nRightSpace;
	int		m_nTopSpace;
	int		m_nBottomSpace;
	int		m_nMarginBorderType;
	BOOL	m_bUseConditiontoTitle;
	BOOL	m_bUseDatatoTitle;
	int		m_nVertGap;
	int		m_nHorzGap;
	int		m_nMinDataCountOnOnePage;
	int		m_nBongCountMargin;
	BOOL	m_bDisplayTRdata;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewDesignPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewDesignPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDisplaytotaldata();
	afx_msg void OnChangeMindatacount();
	afx_msg void OnBtnGridshapeLine();
	afx_msg void OnBtnGridshapeCuttedline();
	afx_msg void OnBtnGridshapeDotline();
	afx_msg void OnBtnGridshapeNone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//Create tooltip object and Add text to StringArray 
	void CreateToolTip();
	void EnableWindow(const int nID, const bool bEnable);

private:
	//tooltip
	CDlgToolTip		m_toolTip;

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
protected:
	void	SetAllGridType( const int p_nScaleGridType);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDESIGNPAGE_H__8AC44F9D_0C74_4E8C_8C30_C551D940FAE1__INCLUDED_)
