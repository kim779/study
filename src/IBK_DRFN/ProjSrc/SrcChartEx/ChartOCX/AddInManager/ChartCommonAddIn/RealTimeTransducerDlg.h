#if !defined(AFX_REALTIMETRANSDUCERDLG_H__72883D50_06D3_49DD_B265_7A6F321BBB12__INCLUDED_)
#define AFX_REALTIMETRANSDUCERDLG_H__72883D50_06D3_49DD_B265_7A6F321BBB12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealTimeTransducerDlg.h : header file
//

#include "../../Include_Chart/IRealReceiver.h"				// for IRealReceiver
#include "../../Include_Chart/TimeTypeConvert.h"			// for CTimeTypeConvert

#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr

#include "EditableListCtrl.h"								// for CEditableListCtrl


/////////////////////////////////////////////////////////////////////////////
// CRealTimeTransducerDlg dialog

class CRealTimeTransducerDlg : public CDialog
{
// Construction
public:
	CRealTimeTransducerDlg( __int64 &p_nRealTime, __int64 &p_nAddTime, __int64 &p_nFakeTime, CTimeTypeConvert &p_tcTR, CTimeTypeConvert &p_tcReal, CWnd* pParent = NULL);   // standard constructor
	~CRealTimeTransducerDlg();

// Dialog Data
	//{{AFX_DATA(CRealTimeTransducerDlg)
	enum { IDD = IDD_REAL_TIME_TRANSDUCER };
	CButton	m_btnRealOnly;
	CStatic	m_stcNMTRTime;
	CComboBox	m_cmbClockAccelerator;
	CStatic	m_stcNMTRClock;
	CButton	m_btnClockOnly;
	CComboBox	m_cbPriceType;
	CComboBox	m_cbRealSpeed;
	CButton	m_btnOnFakeReal;
	CEdit	m_edtFixedRealTime;
	CEditableListCtrl	m_lcRealState;
	CStatic	m_stcFakeTime;
	CStatic	m_stcAddTime;
	CStatic	m_stcRealTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealTimeTransducerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealTimeTransducerDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtnSecond();
	afx_msg void OnBtnMinute();
	afx_msg void OnBtnHour();
	afx_msg void OnBtnDay();
	afx_msg void OnBtnMonth();
	afx_msg void OnBtnYear();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkFakeRealOn();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnFireFixedRealtime();
	afx_msg void OnSelendokCmbSpeed();
	afx_msg void OnSelendokCmbPricetype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// (2009/10/23 - Seung-Won, Bae) Manage Time Value
protected:
	__int64 &			m_nRealTime;
	__int64 &			m_nAddTime;
	__int64 &			m_nFakeTime;
	IPacketManager *	m_pIPacketManager;
	CTimeTypeConvert &		m_tcTR;
	CTimeTypeConvert &		m_tcReal;
	IRealReceiver *		m_pIRealReceiver;
protected:
	CString				GetTimeString( __int64 p_nTime);
	double				GetLastPacketValue( const char *p_szPacketName, BOOL p_bPrev = FALSE);
	void				FireFakeReal( BOOL p_bWithFixedTime = FALSE);
public:
	void				UpdateTime( IPacketManager *p_pIPacketManager = NULL, IRealReceiver *p_pIRealReceiver = NULL);
	void				UpdateReal( void);
	BOOL				IsClockTimeOnly( void);
	BOOL				IsNoClockTime( __int64 &p_nRealClockSecs);		// (2009/11/12 - Seung-Won, Bae) for Clock Monitor
	void				UpdateClockTime( __int64 &p_nRealClockSecs);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMETRANSDUCERDLG_H__72883D50_06D3_49DD_B265_7A6F321BBB12__INCLUDED_)
