#pragma once
// MapWnd.h : header file
//

#include <map>
using namespace std;

#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "Common.h"
#include "dataio.h"
#include "AxisExt.h"
#include "FOHogaWnd.h"
#include "AccountCtrl.h"
#include "SiseWnd.h"
#include "CodeCtrl.h"
#include "NotifyCtrl.h"
#include "ControlWnd.h"
#include "Control2Wnd.h"
#include "ConfigDlg.h"
#include "ConfirmDlg.h"
#include "JngoTicker.h"
#include "OptionEx.h"		//2013.02.07 CX_OptionEx 추가

#define IDC_TEST_BTN		(1000)
#define IDC_GUIDECTRL		(1001)
#define IDC_CB_GROUP		(1002)
#define IDC_EDIT_PSWD		(1003)
#define IDC_BTN_GROUP		(1004)
#define IDC_ST_ACNM			(1005)
#define IDC_BTN_SEND		(1006)
#define IDC_BTN_CONF		(1007)
#define IDC_BTN_SEARCH		(1008)
#define IDC_BTN_LIMIT		(1009)
#define IDC_GRID_JANGO      (1010)
#define IDC_BTN_FOLD1		(1011)

typedef enum _ACCOUNT_MODE { AM_ACCOUNT, AM_GROUP } ACCOUNT_MODE;

typedef map<string, CNotifyCtrl*> NotifyMap;
typedef map<UINT, vector<CString> > OrderAcnoMap;

class CMapWnd : public CWnd, public CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

public:
	void Init();
	void LoadImage();
	void SetSize(int width, int height);
	void Send();
public:
	CRect m_rcAccn, m_rcSise, m_rcHoga, m_rcCtrl, m_rcCtrl2, m_rcGuide, m_rcCheg;
	int m_nWidth,m_nHeight;

public:
	void AddAccount(LPCSTR acno, LPCSTR pswd, LPCSTR acnm = "", int mult = 1);
	void DelAccount(LPCSTR acno);
	void ClearAccount();
	void RefreshRsvd();

	void CalcSize();
	void FoldControl2(BOOL bFold);
	void MakeBackground(int width=-1, int height=-1);
	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	CODE_TYPE GetCodeType(LPCSTR code);
	void ChangeCode(LPCSTR code, BOOL bTrigger = TRUE);
	void RefreshMicheg();
	void RefreshJango();
	void CheckRsvd();
	void SetGuide(LPCSTR msg);
	void ChangeAccount(LPCSTR data);
	void ChangeTheme();
	void LoadConfig();
	void SaveConfig();
	CfxImgButton* CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);
	bool	GetMarket() { return m_pControl->GetMarket(); };
protected:
	BOOL chck_account();
	void req_SACMT238(LPCSTR acno, LPCSTR pswd);
	void res_SACMT238(LPCSTR data, int size);
	void req_SDLDQ003_grp(BOOL bNext=FALSE, LPCSTR nkey=NULL);
	void req_SDLDQ003_acc(LPCSTR gseq, LPCSTR gnam);
	void req_PIBFUTP1(LPCSTR data,int nCount);
	void res_SDLDQ003(LPCSTR data, int size);
	void req_pibo3002(LPCSTR code);
	void res_pibo3002(LPCSTR data, int size);
	void req_pibofchg(LPCSTR acno, LPCSTR pswd, BOOL bNext=FALSE, LPCSTR nkey=NULL);
	void res_pibofchg(LPCSTR data, int size);
	void res_pibofodr(LPCSTR data, int size);
	void proc_trigger(LPCSTR data, int size);
	void proc_alert(LPCSTR data, int size);
	void proc_alert( struct _alertR* alertR );
	void proc_notice(LPCSTR data, int size);
	void SetRealData( DWORD* data );
	void SetRealData( RealData* data );
	void proc_domino(LPCSTR data, int size);
	void proc_jango(Jango *j);
	void proc_guide(LPCSTR msg);
	void calc_micheg(LPCSTR acno);
	void _calc_micheg(Account *pAcnt, BOOL bNotify = TRUE);
	void _calc_micheg(Account *pAcnt, string code, BOOL bNotify = TRUE);
	void SetAverage(Jango* j);
	void Set_Average(Jango* j, bool bSearch = true);
	void SetChegGrid();

	void rsvd_order(ReserveData *prd);
	void modify_rsvd(ModifyData *pmd);
	void cancel_rsvd(CancelData *pcd);
	void modify_order(ModifyData *pmd);
	void cancel_order(CancelData *pcd);
	void cancel_order_all(int odgb);
	void cancel_rsvd_all(int odgb);
	void click_order(Order *pOdr, bool bDragOrder = false);
	void make_fodr(vector<char> &buff, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR code, int jqty, int jprc, int cdgb);
	void merge_order(vector<char> &buff, vector< vector<char> > &orders);
	void reg_order( struct _ledgerH *ledger, vector< vector<char> > &orders );

protected:
	LPCSTR GetPassword(LPCSTR acno);
	CString GetEncPassword(CString sPswd);	//2013.12.23 KSJ 일방향암호화 추가
	void testSaveFile3(CString code, CString datB);

protected:
	ACCOUNT_MODE m_AcntMode;
	SiseData m_SiseData;
	AccountMap m_AcntData;
	vector<int> m_NotifyKeys;
	CString m_RsvdCode;
	RsvdDataArray m_RsvdData;
	OrderAcnoMap m_OdrAcnoMap;
	CString m_LastGroup, m_profile;
	BOOL m_bFoldCtrl2, m_bFoldCtrl1, m_bShowConfirm, m_bShowRate, m_bShowBefore, m_bShowRsvd, m_bShowChg, m_bFocusCheg,m_bShowCurr;
	BOOL m_bSiJangDBClick, m_bJumunReset;
	HBRUSH m_hBrush1,m_hBrush2,m_hBrush3;

protected:
	CString m_home, m_user, m_name;
	CBitmap *m_hRndBmp[4], *m_hGuide, *m_hCombo[2], *m_hBtn2[3], *m_hBtn4[3], *m_hSearch[3];
	CBitmap m_bgBmp;
	BOOL	m_bCreated;
	int m_nMajor, m_nMinor;
	CString m_dept;

protected:
	CfxSpin *m_pSpJcnt;
	CfxEditEx *m_pPswd;
	CfxStatic *m_pStAccn, *m_pStCode, *m_pStJcnt;
	CfxStaticEx *m_pStAcnm, *m_pStCodeNm;
	CComboBox *m_pGroup;
	CfxImgButton *m_pGrpEdit, *m_pSend, *m_pConf, *m_pSearch, *m_pLimit, *m_pFold;
	CCodeCtrl *m_pCode;
	CAccountCtrl *m_pAcno;
	COptionEx *m_pOptionEx;		//2013.02.07 CX_OptionEx 추가
	CFOHogaWnd *m_pHogaWnd;
	CSiseWnd *m_pSise;
	NotifyMap m_NotifyMap;
	CControlWnd *m_pControl;
	CControl2Wnd *m_pControl2;
	CJngoTicker *m_pJngoTick;
	CButton m_btn;
	CStatic m_stGuide;
	CGridCtrl	*m_pChegGrid;
	//CToolTipCtrl m_tooltip;
	int m_otpf;
	int m_utype;
	HINSTANCE m_hiSha256;	//2013.12.23 KSJ 일방향암호화 추가

	int m_iDragOrderAmt;
	CString Parser(CString &srcstr, CString substr);
	void modify_Dragorder( ModifyData *pmd );
	int m_iRowHeight;
	bool binit;
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAppSignal(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOptionEx(WPARAM wParam, LPARAM lParam);		//2013.02.07 CX_OptionEx 추가
	afx_msg void OnTest();
	afx_msg void OnPswdFocus();
	afx_msg void OnGroupEdit();
	afx_msg void OnSendBtn();
	afx_msg void OnConfBtn();
	afx_msg void OnCodeSearch();
	afx_msg void OnGroupChange();
	afx_msg void OnFold();
	afx_msg void OnLimit();
protected:
	void Draw(CDC *pdc);
};

