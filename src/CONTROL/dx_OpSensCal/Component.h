#include "CDX_OpSensCal.h"	// Added by ClassView

#pragma once

// Component.h : header file
//
#include "CDX_OpSensCal.h"


/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation

// Attributes
public:
	CString		m_sRoot, m_sUserName;
	CString		m_sJCode, m_sCD, m_sBdjs, m_sCurr, 
			m_sVola, m_sStrike, m_sTime;
	CString		m_sBzRemain, m_sCalRemain; // øµæ˜¿‹¡∏¿œ, ¥ﬁ∑¬¿‹¡∏¿œ
	CString		m_sK200Cur, m_sFutureCur;

	CString		m_sIV, m_sDelta, m_sGamma, m_sTheta, m_sVegga, m_sRho;
	CString		m_symIV, m_symDelta, m_symGamma, m_symTheta, m_symVegga, m_symRho;

	CString		m_sBPType, m_sVolType, m_sRDType, m_sK200Rate, m_sUserCallIV, m_sUserPutIV, 
			m_sTimeValue, m_sTimeStart, m_sTimeEnd, m_sHVDay;
	double		m_dHV90, m_dHV60, m_dHV30, m_dHV21;

	CDX_OpSensCal	m_optionCalc;
// Operations
public:
	OPTION	getStOption(bool theoryType = false);
	int	GetTimeMin(CString sTime);
	//void	SetData(CString sym, double value, bool dot2 = false);
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
	afx_msg BSTR GetCurrValue();
	afx_msg void SetCurrValue(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCD();
	afx_msg void SetCD(LPCTSTR lpszNewValue);
	afx_msg BSTR GetDelta();
	afx_msg BSTR GetGamma();
	afx_msg BSTR GetTheta();
	afx_msg BSTR GetRho();
	afx_msg BSTR GetVegga();
	afx_msg BSTR GetTheoryValue();
	afx_msg BSTR GetIV();
	afx_msg BSTR GetBzRemainDays();
	afx_msg void SetBzRemainDays(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCalRemainDays();
	afx_msg void SetCalRemainDays(LPCTSTR lpszNewValue);
	afx_msg BSTR GetK200Cur();
	afx_msg void SetK200Cur(LPCTSTR lpszNewValue);
	afx_msg BSTR GetFutureCur();
	afx_msg void SetFutureCur(LPCTSTR lpszNewValue);
	afx_msg BSTR GetTime();
	afx_msg void SetTime(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUserName();
	afx_msg void SetUserName(LPCTSTR lpszNewValue);
	afx_msg void Calc(long CalcItem);
	afx_msg void ReadCfg();
	afx_msg void SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21);
	afx_msg void symIV(LPCTSTR sym);
	afx_msg void symDELTA(LPCTSTR sym);
	afx_msg void symGAMMA(LPCTSTR sym);
	afx_msg void symTHETA(LPCTSTR sym);
	afx_msg void symVEGGA(LPCTSTR sym);
	afx_msg void symRHO(LPCTSTR sym);
	afx_msg void CalcEx(LPCTSTR cur, LPCTSTR k200cur, LPCTSTR fcur, LPCTSTR time);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};