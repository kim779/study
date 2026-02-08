#pragma once
// setup.h : header file
//

//
//	config
//
#define CNT_TREE 15
#define CNT_CHECK 10
class	Cconfig
{
public:
	int	m_vol;			// 체결량 범위   (vol * 1,000)
	int	m_amt;			// 체결금액 범위 (amt * 10,000,000)
	bool	m_and;			// and / or

	bool	m_price;		// 가격설정 flag
	int	m_sprc;			// 가격설정 시작값
	int	m_eprc;			// 가격설정 끝값

	bool	m_ulimit;		// 상한
	bool	m_up;			// 상승
	bool	m_flat;			// 보합
	bool	m_dlimit;		// 하한
	bool	m_down;			// 하락

	bool	m_color;		// 색상표시
	WORD	m_field;		// 필드표시

	CString	m_tree[CNT_TREE];			/* 0: 전체                        */
										/* 1: 거래소,      2: 코스닥      */
										/* 3: KP200,       4: KRX100      */
										/* 5: 프리보드,    6: ETF         */
										/* 7: ELW,         8: ELW기초자산 */
};

//
//	field
//
#define	F_CODE		0x0001		// 코드
#define	F_RATE		0x0002		// 등락율
#define	F_VOL		0x0004		// 거래량
#define	F_AMT		0x0008		// 순간체결금액


/////////////////////////////////////////////////////////////////////////////
// Csetup dialog

class Csetup : public CDialog
{
// Construction
public:
	Csetup(CWnd* pParent = NULL, Cconfig* config = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Csetup)
	enum { IDD = IDD_SETUP };
	CButton	m_chkKrx100;
	CButton	m_chkKospi200;
	CButton	m_chkKospi;
	CButton	m_chkKosdaq;
	CButton	m_chkFreeboard;
	CButton	m_chkEtf;
	CButton	m_chkElwbase;
	CButton	m_chkElw;
	CButton	m_chkAll;
	//}}AFX_DATA

public:
	void	GetConfig(Cconfig* config);
	void	SetProceedCheck(int index);
	void	Check_AllCheck();
protected:
	Cconfig* m_config;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Csetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Csetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPrice();
	afx_msg void OnAll();
	afx_msg void OnElw() { UpdateData(); };
	afx_msg void OnElwbase() { UpdateData(); };
	afx_msg void OnEtf() { UpdateData(); };
	afx_msg void OnFreeboard() { UpdateData(); };
	afx_msg void OnKosdaq() { UpdateData(); };
	afx_msg void OnKospi() { UpdateData(); };
	afx_msg void OnKospi200() { UpdateData(); };
	afx_msg void OnKrx100() { UpdateData(); };
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

