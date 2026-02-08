
#include "CDX_OptionSens.h"	// Added by ClassView

#pragma once

// Component.h : header file
//
#include "CDX_OptionSens.h"


/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation

// Attributes
public:
	CString		m_sRoot;
	CString		m_sJCode, m_sCD, m_sBdjs, m_sCurr, 
			m_sRemain, m_sUnder, m_sVola, m_sStrike;

	CDX_OptionSens	m_optionCalc;
// Operations
public:
	OPTION getStOption();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponent)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComponent();

	// Generated message map functions
	//{{AFX_MSG(CComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponent)
	afx_msg BSTR GetJCode();
	afx_msg void SetJCode(LPCTSTR lpszNewValue);
	afx_msg BSTR GetBedangJisu();
	afx_msg void SetBedangJisu(LPCTSTR lpszNewValue);
	afx_msg BSTR GetStrike();
	afx_msg void SetStrike(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUnderlying();
	afx_msg void SetUnderlying(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCurrValue();
	afx_msg void SetCurrValue(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCD();
	afx_msg void SetCD(LPCTSTR lpszNewValue);
	afx_msg BSTR GetRemainDays();
	afx_msg void SetRemainDays(LPCTSTR lpszNewValue);
	afx_msg BSTR GetDelta();
	afx_msg BSTR GetGamma();
	afx_msg BSTR GetTheta();
	afx_msg BSTR GetRho();
	afx_msg BSTR GetVegga();
	afx_msg BSTR GetTheoryValue();
	afx_msg BSTR GetIV();
	afx_msg BSTR GetVolatility();
	afx_msg void SetVolatility(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};