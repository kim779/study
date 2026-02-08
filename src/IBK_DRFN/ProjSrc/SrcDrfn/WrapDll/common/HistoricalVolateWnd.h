#if !defined(AFX_HISTORICALVOLATEWND_H__6C842013_6A3C_48F4_8731_0F9783A68B20__INCLUDED_)
#define AFX_HISTORICALVOLATEWND_H__6C842013_6A3C_48F4_8731_0F9783A68B20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoricalVolateWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoricalVolateWnd window

#include "FONetworkModule.h"
#include "strategyCalc.h"
//#include "Dataset.h"

class CHistoricalVolateWnd : public CWnd
{
// Construction
public:
	CHistoricalVolateWnd();

	//20080304 이문수 >>
	CHistoricalVolateWnd(CWnd * pNetworkWnd);
	//20080304 이문수 <<
// Attributes
public:

	//20080304 이문수 >>
	//tr통신할 포인터
	CWnd *					m_pNetworkWnd;
	//20080304 이문수 <<

	CStrategyCalc			m_StrategyCalc;

	CFONetworkModule *		m_pFONetwork;

	BOOL					m_bSuccess;

	//20080304 이문수 >>
	//CPriceProgressData *	m_pPastDataOfUnderlyingAssets;
	CIV4ItemArray			m_IVItemArray;
	//20080304 이문수 >>
// Operations
public:

	double CalcHisVolate(CString strUnderlyingAssetsCode, int nCount);

	BOOL RegisterWndClass(HINSTANCE hInstance);

	BOOL CreateWnd(int nID, CWnd * pParentWnd);

	//20080304 이문수 >>
	void SetNetworkWnd(CWnd * pNetworkWnd);
	//20080304 이문수 <<

	//20080317 이문수 >>
	BOOL GetCloseData(CString strUnderlyingAssetsCode, CArray < double, double > * pCloseDataArray);
	//20080317 이문수 <<
// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoricalVolateWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHistoricalVolateWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoricalVolateWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnReceiveBaseData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnFOReceiveData(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);

	//20080306 이문수 >>
	long OnGetThisWndPtr(WPARAM wParam, LPARAM lParam);
	//20080306 이문수 <<
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORICALVOLATEWND_H__6C842013_6A3C_48F4_8731_0F9783A68B20__INCLUDED_)
