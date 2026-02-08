#if !defined(AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_)
#define AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STDlgConfirm.h : header file
//
#include "Resource.h"
#include "../../inc/IAuTrCommMng.h"
#include "../CommonTR/SS_OrderDef.h"

//#include "../chart_common/Grid/TR_drds/D49890.h"
#include "../../inc/RealUpdate.h"
#include "Define/ChartMsgDef.h"
#include "../Common_ST/STControllerDef.h"


///////////////////////////////////////////////////////////////////////////////
// CConfirmWnd

DECLARE_INTERFACE(ISTDlgConfirmSendJumun)
{
	STDMETHOD_(CWnd*, GetParent)() PURE;
	STDMETHOD_(HWND, GetHwndScreenView)() PURE;
//	STDMETHOD_(LRESULT, SendJumun)(WPARAM wp, LPARAM lp) PURE;
};

class CConfirmWnd : public CWnd
{
// Construction
public:
	CConfirmWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmWnd)
	//}}AFX_VIRTUAL

	CSTOrderRData_EG		m_InData;

	CString m_szHogaData[10];	//현재가,매수1~3,매도1~3,시장가,상한가,하한가.
	enum {
		hogaIdx_current=0,
		hogaIdx_buy1=1,
		hogaIdx_sell1=4,
		hogaIdx_market=7,
		hogaIdx_hprice=8,
		hogaIdx_lprice=9,
	};
	
	ISTDlgConfirmSendJumun* m_pSTDlgConfirmSendJumun;
	
// Implementation
public:
	virtual ~CConfirmWnd();
	BOOL	OpenWindow();

	int		SendJumun(int nType);

	CString m_strOut;
	CString m_strCurPrice;			// 현재가
	CString m_strUpLimitPrice;		// 상한가
	CString m_strDownLimitPrice;	// 하한가
	CString m_strBuy1Hoga;			// 매수1호가
	CString m_strBuy2Hoga;			// 매수2호가
	CString m_strBuy3Hoga;			// 매수3호가
	CString m_strSell1Hoga;			// 매도1호가
	CString m_strSell2Hoga;			// 매도2호가
	CString m_strSell3Hoga;			// 매도3호가

	// Generated message map functions
protected:
	//{{AFX_MSG(CConfirmWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnRmsgReceiveDirect(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDLGCONFIRM_H__DF0EEA73_3992_4279_9F24_928A46825D43__INCLUDED_)
