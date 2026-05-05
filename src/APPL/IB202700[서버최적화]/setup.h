#include "resource.h"

#pragma once
// setup.h : header file
//

//
//	config
//
class Cconfig
{
public:
	BOOL m_bApply{};
	int m_vol{};		// 체결량 범위   (vol * 1,000)
	int m_amt{};		// 체결금액 범위 (amt * 10,000,000)
	BOOL m_and{};		// and / or

	BOOL m_price{};		// 가격설정 flag
	int m_sprc{};		// 가격설정 시작값
	int m_eprc{};		// 가격설정 끝값

	BOOL m_ulimit{};	// 상한
	BOOL m_up{};		// 상승
	BOOL m_flat{};		// 보합
	BOOL m_dlimit{};	// 하한
	BOOL m_down{};		// 하락

	Cconfig()
	{
		m_bApply = FALSE;
	}
};

//
//	field
//
#define F_CODE 0x0001 // 코드
#define F_RATE 0x0002 // 등락율
#define F_VOL 0x0004  // 거래량
#define F_AMT 0x0008  // 순간체결금액

/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

class Csetup : public CDialog
{
	// Construction
public:
	Csetup(CWnd *pParent = nullptr, Cconfig *config = nullptr); // standard constructor

	// Dialog Data
	//{{AFX_DATA(Csetup)
	enum
	{
		IDD = IDD_TICKER_SETUP
	};
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	void GetConfig(Cconfig *config);
	void EnableControl(BOOL bEnable);

protected:
	Cconfig *m_config;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csetup)
protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
							 //}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(Csetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPrice();
	afx_msg void OnCheckApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
