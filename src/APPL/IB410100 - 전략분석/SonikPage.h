#pragma once

#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxSpin.h"
#include "../../control/fx_misc/fxImgButton.h"

// SonikPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSonikPage dialog

const int vMZeroSonik = 0, vTZeroSonik = 1, vMMax = 2, vMMin = 3, vTMax = 4, vTMin = 5, vCnt = 6;

class CSonikPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSonikPage)

// Construction
public:
	CSonikPage(CWnd* pView = NULL, CWnd* pParent = NULL);
	~CSonikPage();

public:
	CIB410100App*	m_pApp;
	CWnd		*m_pParent;
	CWnd		*m_pView;
	std::unique_ptr<class CGridWnd> m_pGridWnd;
	std::unique_ptr<class CGrpWnd>	m_pGrpWnd;
	std::unique_ptr<class CfxTab>	m_pTab;
	COLORREF	m_bkColor;
	CBrush		m_bkBr;
	CFont		*m_pFont;

	int		m_nKind;		// 만기지수 0, 잔존일수 1, 변동성 2
	int		m_nInfoKind;		// 챠트Info 0, 민감설정 1

	CString		m_sHval[4];		// 21,30,60,90 역사적 변동성
	CString		m_sIVVal[2], m_sVal;
	double		m_dVal[vCnt];

	CString		m_sSonikTbl[3][3];
	CString		m_sSonikBunKiTbl[3][2];
	CString		m_sDepositTbl[2][2];	// 주문체결후 증거금
	CString		m_sDepGuessTbl[2][2];	// 주문체결 후 익일 추정증거금
	// User Control Value
	CString		m_rtnStr;

public:
	void	CheckMinGam();
	void	InitData(struct _bdat *bdat);
	void	DrawInfo(CDC *dc);
	void	SetSonikPage();
	void	ReadGrpParam();
	void	DrawItemText(CDC* dc, long dlgItem, bool left, CRect* rcLimit, CString text, int gap = 10);
	void	WriteCfgCallPutIV();
	void	SetApplyMode();
	void	SetBasicJisu(double basicprc);		// cej

	
// Dialog Data
	//{{AFX_DATA(CSonikPage)
	enum { IDD = IDD_TAB_SONIK };
	CButton	m_chkRange;
	CfxImgButton	m_btnApply;
	CButton	m_rdoConfigVol;
	CfxSpin	m_spnItemPutChg;
	CfxSpin	m_spnItemCallChg;
	CfxSpin	m_spnCfgPutChg;
	CfxSpin	m_spnCfgCallChg;
	CfxSpin	m_spnRemainDay;
	CfxSpin	m_spnJisuRange2;
	CfxSpin	m_spnJisuRange1;
	CButton	m_ChkSonik;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSonikPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgMouseValue(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgGetHV(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgGetFee(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgInitParam(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgDepositSel(WPARAM wParam, LPARAM lParam);

	//{{AFX_MSG(CSonikPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMgchk1();
	afx_msg void OnMgchk2();
	afx_msg void OnMgchk3();
	afx_msg void OnMgchk4();
	afx_msg void OnMgchk5();
	afx_msg void OnMgchk6();
	afx_msg void OnMgradiox1();
	afx_msg void OnMgradiox2();
	afx_msg void OnMgradiox3();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnApply();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChkApply();
	afx_msg LRESULT OnMsgClear(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChkrange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void	oubDeposit(char *buf, int len);
	void	oubDepGuess(char *buf, int len);
	CString	parse(CString &srcstr, CString substr);
	CString	variant(int cmd, CString data);
	CString	setComma(CString src);
	void	searchDeposit(bool bAll) ;
	CString	m_gubn;

	void	loadSonikRange();
	void	saveSonikRange();
	CString	m_pathRange;
};

