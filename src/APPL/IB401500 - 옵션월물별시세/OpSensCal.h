
#include "dxopsenscal.h"

using namespace std;


#pragma once
// OpSensCal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpSensCal window

class COpSensCal : public CWnd
{
// Construction
public:
//	COpSensCal();

// Attributes
public:
	CWnd* m_parent{};
	CCmdTarget* m_COpSensCal{};
	std::unique_ptr< IOpSensCal > m_IOpSensCal;
	CString		m_root;
	HINSTANCE	m_hInst{};
	CCmdTarget* (APIENTRY* axCreate)(char*) {};
// Operations
public:
	BOOL	CreateOpSensCal(CWnd* parent, CString root);

	CString GetJCode();
	void SetJCode(LPCTSTR jCode);
	CString GetBedangJisu();
	void SetBedangJisu(LPCTSTR bdjs);
	CString GetStrike();
	void SetStrike(LPCTSTR strike);
	CString GetCurrValue();
	void SetCurrValue(LPCTSTR cur);
	CString GetCd();
	void SetCd(LPCTSTR cd);
	CString GetDelta();
	CString GetGamma();
	CString GetTheta();
	CString GetRho();
	CString GetVegga();
	CString GetTheoryValue();
	CString GetIv();
	CString GetBzRemainDays();
	void SetBzRemainDays(LPCTSTR bzRemainDay);
	CString GetCalRemainDays();
	void SetCalRemainDays(LPCTSTR calRemainDay);
	CString GetK200Cur();
	void SetK200Cur(LPCTSTR K200Cur);
	CString GetFutureCur();
	void SetFutureCur(LPCTSTR FutureCur);
	CString GetTime();
	void SetTime(LPCTSTR time);
	CString GetUserName();
	void SetUserName(LPCTSTR userName);

	void Calc(long CalcItem);
	void ReadCfg();
	void SetHv(LPCTSTR Hv90, LPCTSTR Hv60, LPCTSTR Hv30, LPCTSTR Hv21);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpSensCal)
	//}}AFX_VIRTUAL

// Implementation
public:
//	virtual ~COpSensCal();

	// Generated message map functions
protected:
	//{{AFX_MSG(COpSensCal)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

