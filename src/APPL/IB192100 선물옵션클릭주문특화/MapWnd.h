#pragma once

#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"

#include "Common.h"
#include "dataio.h"
#include "AxisExt.h"
#include "AccountCtrl.h"
#include "CodeCtrl.h"
#include "NotifyCtrl.h"
#include "FOHogaWnd.h"
#include "ConfirmDlg.h"
#include "ConfigDlg.h"

#define ADDPV(x)	(x+PADDING_VALUE)
#define ADDTC(x)	(x+2)
#define ADDTC1(x)	(x+1)
#define IDC_ACNO_CTRL		(1001)
#define IDC_PSWD_EDIT		(1002)
#define IDC_ACNM_STATIC		(1003)
#define IDC_SEARCH_BTN		(1004)
#define IDC_HOGA_WND		(1005)
#define IDC_JANGO_GRID		(1006)
#define IDC_JQTY_GRID		(1007)
#define IDC_SEND_BTN		(1008)
#define IDC_CHEG_BTN		(1009)
#define IDC_JNGO_BTN		(1010)
#define IDC_CONF_BTN		(1011)
#define IDC_ALIGN_BTN		(1012)
#define IDC_CANCEL_BTN		(1013)

typedef map<string, CNotifyCtrl*> NotifyMap;
typedef map<UINT, vector<CString> > OrderAcnoMap;

class CMapWnd : public CWnd, CAxisExt
{
public:
	CMapWnd(CWnd *pParent);
	void Init();
	void ChangeAccount(LPCSTR data);
	void ClearAccount();
	void ChangeCode(LPCSTR code, BOOL bTrigger = TRUE);
	CODE_TYPE GetCodeType(LPCSTR code);
	void Send();

protected:
	void AddAccount(LPCSTR acno, LPCSTR pswd, LPCSTR acnm = "", int mult = 1);
	void DelAccount(LPCSTR acno);
	void RefreshRsvd();

	LPCSTR GetPassword(LPCSTR acno);
	void SetGuide(LPCSTR msg);
	void SetJangoGrid(BOOL bOnlyHeader=FALSE);
	void SetJqtyGrid();

protected:
	void LoadConfig();
	void SaveConfig();
	void LoadImage();
	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void MakeBackground();
	void CalcSize();
	void Draw(CDC *pdc);
	void MoveWindow(CWnd *pWnd, int pos_x, int pos_y, int width, int height);
	void ChangeTheme();
	std::shared_ptr<CfxImgButton>CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);

	void RefreshJango();
	void RefreshMicheg();
	void CheckRsvd();
	BOOL CheckAccount();

	int GetCdgb() { return 0; }
	int GetRsvd() { return 0; }
	int GetJcnt();
	int GetMulti(){ return 0; }

protected:
	void proc_trigger(LPCSTR data, int dlen);
	void proc_alert(LPCSTR data, int dlen);
	void proc_alertx(struct _alertR* alertR);
	void proc_guide(LPCSTR msg);
	void proc_notice(LPCSTR data, int dlen);
	void proc_jango(Jango *j);
	void proc_domino(LPCSTR data, int size);

	void calc_micheg(LPCSTR acno);
	void _calc_micheg(Account *pAcnt, BOOL bNotify = TRUE);
	void _calc_micheg(Account *pAcnt, string code, BOOL bNotify = TRUE);

	void rsvd_order(ReserveData *prd);
	void modify_rsvd(ModifyData *pmd);
	void cancel_rsvd(CancelData *pcd);
	void modify_order(ModifyData *pmd);
	void cancel_order(CancelData *pcd);
	void cancel_order_all(int odgb);
	void cancel_rsvd_all(int odgb);
	void click_order(Order *pOdr);
	void make_fodr(vector<char> &buff, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR code, int jqty, int jprc, int cdgb);
	void merge_order(vector<char> &buff, vector< vector<char> > &orders);
	void reg_order( struct _ledgerH *ledger, vector< vector<char> > &orders );

protected:
	void req_pibo3002(LPCSTR code);
	void res_pibo3002(LPCSTR data, int size);
	void req_SACMT238(LPCSTR acno, LPCSTR pswd);
	void res_SACMT238(LPCSTR data, int size);
	void req_pibofchg(LPCSTR acno, LPCSTR pswd, BOOL bNext=FALSE, LPCSTR nkey=NULL);
	void res_pibofchg(LPCSTR data, int size);
	void res_pibofodr(LPCSTR data, int size);

protected:
	int m_JqtyIndex;
	BOOL m_bShowConfirm, m_bHogaReverse;
	CString m_home, m_user, m_name, m_profile;
	CBitmap *m_hRndBmp[4], *m_hGuide, m_hBackground, *m_hBtn2[3], *m_hBtn4[3], *m_hSearch[3];
	CRect m_rcBody, m_rcGuide;

protected:
	SiseData m_SiseData;
	AccountMap m_AcntData;
	vector<int> m_NotifyKeys;
	CString m_RsvdCode;
	RsvdDataArray m_RsvdData;
	OrderAcnoMap m_OdrAcnoMap;

protected:
	std::unique_ptr<CAccountCtrl>m_pAcno;
	std::unique_ptr <CCodeCtrl>m_pCode;
	NotifyMap m_NotifyMap;
	std::unique_ptr <CFOHogaWnd>m_pHogaWnd;
	
	std::unique_ptr <CGridCtrl>m_pJango, m_pJqty;
	std::unique_ptr <CfxStatic>m_pStAccn, m_pStCode, m_pStGuide;
	std::unique_ptr <CfxStaticEx>m_pStAcnm, m_pStCodeNm, m_pStJango;
	std::unique_ptr <CfxEditEx>m_pPswd;
	std::shared_ptr <CfxImgButton>m_pBtnSearch, m_pBtnSend, m_pBtnCheg, m_pBtnJngo, m_pBtnConf, m_pBtnAlign, m_pBtnCancel;
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	
public:
	virtual ~CMapWnd();

protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAppSignal(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCodeSearch();
	afx_msg void OnSendBtn();
	afx_msg void OnChegBtn();
	afx_msg void OnJngoBtn();
	afx_msg void OnConfBtn();
	afx_msg void OnChartBtn();
	afx_msg void OnAlignBtn();
	afx_msg void OnCancelBtn();
};


