#pragma once
// MapWnd.h : header file
//
#include "SendOrder.h"
#include "afxtempl.h"
#include <afxmt.h>
#include "exButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd *parent);
public:
	// thread
	CArray<char *, char *>	m_arBytes;
	CCriticalSection	m_Section;
	CWinThread*		m_pThread;
	bool			m_bThread;
	HANDLE			m_hEvent;

	int				m_iWndWidth, m_iWndHeight, m_iContentHeight;
	COLORREF		m_CmdBgColor, m_ContentsBgColor, m_GuideBgColor;
	COLORREF		m_ContentsBorderColor, m_GuideTxtColor, m_GridHdrTxtColor, m_txtcolor;

protected:
	CWnd*		m_pParent;
	BOOL		m_bWait;

	CRect		m_oRc1;		// 계좌명
	CRect		m_oRc2;		// 현재가
	CRect		m_oRc3;		// 대비
	CRect		m_oRc4;		// 등락율 
	CRect		m_oRc5;		// 거래량
	CRect		m_oRc6;		// 종목명
	CRect		m_oRc7;		// 시가
	CRect		m_oRc8;		// 고가
	CRect		m_oRc9;		// 저가
	CRect		m_oRc10;	// 미결제

	CRect		m_tRc2;		
	CRect		m_tRc3;		
	CRect		m_tRc4;		
	CRect		m_tRc5;		
	CRect		m_tRc7;		
	CRect		m_tRc8;		
	CRect		m_tRc9;		
	CRect		m_tRc10;		

	CString		m_output1;	// 계좌명
	CString		m_output2;	
	CString		m_output3;	
	CString		m_output4;	
	CString		m_output5;
	CString		m_output6;
	CString		m_output7;	
	CString		m_output8;	
	CString		m_output9;
	CString		m_output10;
		
	CString		m_text2;
	CString		m_text3;
	CString		m_text4;
	CString		m_text5;
	CString		m_text6;

	CRect		m_rcGuide;
	CString		m_guide;

	CString		m_acc;		// 계좌번호
	CString		m_pswd;		// 계좌비번
	CString		m_code;		// 종목코드
	CString		m_codeName;	// 종목명
	CString		m_mouseDLL;

	std::unique_ptr<class CHogaWnd>		m_pHogaWnd;
public:
	std::unique_ptr<class CControlWnd>	m_pCtrlWnd;
protected:
	std::unique_ptr<class CAccountCtrl>	m_pAccount;
	std::unique_ptr<class COptionEx>	m_pOptionEx;	//2013.01.29 KSJ 

	std::unique_ptr<class CfxEdit>		m_pPass;
	std::unique_ptr<class CfxCodeCtrl>	m_pCodeCtrl;
	std::unique_ptr<class CfxImgButton>	m_pBtnSend;
	std::unique_ptr<class CfxImgButton>	m_pBtnReserve;
	std::unique_ptr<class CfxImgButton>	m_pBtnConfig;
	std::unique_ptr<class CfxImgButton>	m_pBtnVSize;
	std::unique_ptr<class CfxImgButton>	m_pBtnCode;
	std::unique_ptr<class CfxStatic>	m_pStatic;
	std::unique_ptr<class CNotify>		m_pNotify;
	std::unique_ptr<CWnd>				m_pMapWnd;
	BOOL			m_bSendFchgTimer;
	BOOL			m_bCanSend;

	int				m_nFchgCount;	

	CString		m_home;
	CString		m_user;

	CFont*		m_pFont;
	CFont*		m_pBFont;

	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_depth;	// 5/10호가 구분
	enum { szNORMAL = 0, szEXPAND }		m_size;		// 가로 크기

	int		m_mapKey;
	bool		m_bWarning, m_bFirst;

	CSendOrder	m_ledger;
	int		m_lenLedger;

	int m_nWaitTrType, m_iCurFont, m_iOldSize[2];
	BOOL	m_bInit, m_bhogaResizeClick;
	double	m_lSizeRate[2];
	HBITMAP				m_hRoundBitmap[4], m_hGuideBitmap;

	bool m_bMicheContinue;
private:
	bool IsNumber(CString str);
	HBITMAP		m_hBitmap1, m_hBitmap1_dn, m_hBitmap1_hv;
	HBITMAP		m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv;
	HBITMAP		m_hBitmap3, m_hBitmap3_dn, m_hBitmap3_hv;
	HBITMAP		m_hBitmapUP, m_hBitmapUP_dn, m_hBitmapUP_hv;
	HBITMAP		m_hBitmapLR, m_hBitmapLR_dn, m_hBitmapLR_hv;
	HBITMAP		m_hBitmapCODE, m_hBitmapCODE_dn, m_hBitmapCODE_hv;
	HBITMAP		m_hBitmapHOGA1, m_hBitmapHOGA1_dn, m_hBitmapHOGA1_hv;
	HBITMAP		m_hBitmapHOGA3, m_hBitmapHOGA3_dn, m_hBitmapHOGA3_hv;
	HINSTANCE	m_hiSha256;	//2013.13.23 KSJ 일방향암호화

// Attributes
public:
	void		Init();

	COLORREF	GetIndexColor(int index);
	CBitmap*	GetAxBitmap(CString path);
	HBITMAP		GetAxHBitmap(CString stPath);

	CFont*		GetAxFont(CString fName = "굴림", int point = 9, bool bItalic = false, int nBold = 0);
	CBrush*		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	int		Variant(int cmd, int data = 0);
	CString		Variant(int cmd, CString data = "");
	void		SetView(int type, CString data);
	CString		SetComma(CString src);
	CString		Parser(CString &srcstr, CString substr);
	BOOL		SendTR(CString name, BYTE type = NULL, CString data = "", int key = 0);
	BOOL		SendTR2(LPCSTR name, char type, LPCSTR data, int dlen, int key);

	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, COLORREF txtcolor=0xFF00FF);
	void		DrawColorBox(CDC* pDC, CRect rc, BOOL l = TRUE, BOOL t = TRUE, BOOL r = TRUE, BOOL b = TRUE);
	void		SendJumun(CString jflg, double hoga, int juno = 0, int jqty = 0, bool market = false);
	void		SendJangoData();
	void		SendMicheg(CString strLedger = "");
	void		SendMaxJmcn(CString jmdg);
	CString		GetCode()	{ return m_code; }
	CString		GetAccount()	{ return m_acc;	 }
	CString		GetName()	{ return m_output1; }
	CString		GetCodeName()	{ return m_codeName; }
	CString		GetPswd();
	void		SetCode(CString code);
	void		SetGuide(CString guide);

	void		AttatchMap();
	void		DetachMap();
	void		LoadMap(CString param);

	void		CreateMap(int iLeft, int iTop, int iRight, int iBottom);
	void		DeleteMap();

	void		DoParse(CString datB);
	void		DoParsex(struct _alertR* alertR);
	bool		WaitRTM();

	void		SetHold(bool bHold);
	void		DrawCornerRoundBitmap(CDC* pDC, CRect rc);

	bool		GetMarketEnable();

protected:
	void		transaction();
	void		dispatchData(char* pData, int len);
	void		dispatchNotice(CString notice);
	void		dispatchAlert(CString alert);
	void		dispatchAlertX(struct _alertR * alertR);
	void		setButtonBitmaps();
	CString		getEncPass(CString sPass, CString sAcc);
	void		setTabFocus();
	BOOL		IsChildWindow(HWND hParentWnd, HWND hChild);

	void		alert(char* buf);
	void		initRTM();
	void		stopRTM();
	void		removeRTM();
	void		SetChildWait(BOOL bWait);
	void		ChangeFont(int ifontsixze, LPCTSTR lszFont);
	void		ResizeControl(int cx, int cy);
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	void		ChangePallete();
	void		LoadRoundBitmap();
	CString		GetEncPassword(CString sPswd);	//2013.13.23 KSJ 일방향암호화

public:
	bool bChecking;
	bool bPasswordChecked;
	bool IsPasswordChecked();


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ParsingCheckPswd(CString strData);
	bool CheckPswd(CString strAccount, CString strPassword);
	struct _ledgerH* GetLedger(struct _ledgerH* ledger);
	virtual ~CMapWnd();
	inline BOOL GetWaitMode() { return m_bWait; };
	void SetWaitMode(BOOL bWait);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnButtonCode();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonVSize();
	afx_msg void OnButtonHoga1();
	afx_msg void OnButtonHoga3();
	afx_msg void OnButtonConfig();
	afx_msg void OnButtonReserve();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnHistoryCode();
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);
};

