#pragma once
// MapWnd.h : header file
//
//#include "symbol.h"
#include "SendOrder.h"
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

struct	_jinfo {
	char    codx[12];		// 종목코드
	char    hnam[32];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '5':ETF
	char    size[12];		// 기업규모
	char    k2gb[20];		// 소구분 전기,전자
	char    usun[10];		// 보통주/우선주
	char    ksmm[2];		// 결산월
	char    amga[20];		// 액면가 8->20 단위포함
	char    jbkm[20];		// 자본금(단위:억) 12->20 단위포함
	char    siga[12];		// 전일시가총액(단위:억)
	char    sjjs[12];		// 상장주식수(단위:천주)
	char	jrat[3];		// 증거금율
	char	nrat[3];		// 신용증거금율 20070718 
	char    krgb[12];		// "거래정지", "거래중단", "관리종목", "이상급등", "투자유의", "정상"
	char    jggb[12];		// 장운영 구분
	char    lock[12];		// 락구분
	char    dyga[8];		// 대용가
	char    ujgb[12];		// 업종구분
	char    ujcd[6];		// RTS 업종코드
	char    jisu[8];		// 업종지수
	char    diff[8];		// 업종전일대비
	char    rate[8];		// 업종등략율
	char    gvol[12];		// 업종거래량(단위:천주)
	char    unio[10];		// KRX 100 (0:일반 1:통합지수)
	
};
#define L_jinfo		sizeof(struct _jinfo)


class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* pwndParent);

// Attributes
public:

	COLORREF	m_CmdBgColor{}, m_ContentsBgColor{}, m_GuideBgColor{};
	COLORREF	m_ContentsBorderColor{}, m_GuideTxtColor{};
protected:
	BOOL		m_bWait, m_bInit, m_bChangeFont, m_bHogaSize;
	bool		m_bWarning{};
	int			m_iLenLedger{};
	struct		_param	symbolparam {};
	
	CWnd*		m_pwndParent{};
	CWnd*		m_pwndSymbol{};

	HINSTANCE	m_hiSymbol{};

	CFont*		m_pfont{};
	CFont*		m_pfontB{};

	CPen*		m_ppenLine{};

	COLORREF	m_clrBG{};	// Back Ground
	COLORREF	m_clrTH{};	// Table Head
	COLORREF	m_clrGH{};	// Grid Head
	COLORREF	m_clrDT{};	// Data
	COLORREF	m_clrTFG{}, m_clrGridHdrTxt{};	// Text ForeGround

	CRect		m_rectAccName;		// 계좌명
	CRect		m_rectCurrD;		// 현재가
	CRect		m_rectDiffD;		// 대비
	CRect		m_rectRateD;		// 등락율 
	CRect		m_rectGvolD;		// 거래량

	CRect		m_rectCurrS;		
	CRect		m_rectDiffS;		
	CRect		m_rectRateS;		
	CRect		m_rectGvolS;		

	CString		m_strAccName;	// 계좌명
	CString		m_strCurr;	
	CString		m_strDiff;	
	CString		m_strRate;	
	CString		m_strGvol;
	
	CRect		m_rcGuide;
	CString		m_strGuide;

	CString		m_strAcc;		// 계좌번호
	CString		m_strPswd;		// 계좌비번
	CString		m_strJCOD;		// 종목코드
	CString		m_strJName;		// 종목명
	CString		m_strCODX;		// RTS code


	CSendOrder	m_ledger;

	std::unique_ptr<class CHogaWnd> m_pHogaWnd{};
	std::unique_ptr<class CControlWnd> m_pCtrlWnd{};
	std::unique_ptr<class CAccountCtrl> m_pAccount{};

	std::unique_ptr<class CfxEdit> m_pPass{};
	std::unique_ptr<class CfxCodeCtrl> m_pCodeCtrl{};
	std::unique_ptr<class CfxImgButton>	m_pBtnSend{};
	std::unique_ptr<class CfxImgButton>	m_pBtnReserve{};
	std::unique_ptr<class CfxImgButton>	m_pBtnConfig{};
	std::unique_ptr<class CfxImgButton>	m_pBtnVSize{};
	std::unique_ptr<class CfxImgButton>	m_pBtnCode{};
	std::unique_ptr<class CNotify>		m_pNotify{};

	std::unique_ptr<class CTool>		m_pTool{};

	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_eDepth;	// 5/10호가 구분


	int					m_iOldFont{}, m_iDefaultFont{}, m_iDefaultSize[2]{}, m_iOldSize[2]{}, m_iHogaType{};
	double				m_lSizeRate[2];

	HBITMAP				m_hRoundBitmap[4]{}, m_hGuideBitmap{};
	int					m_iHogaRowH{};
public:
	// guide
	void		SetGuide(CString strGuide);

	void		LoadBuffer();
	void		SaveBuffer(CString sKey, CString sData);
	CString		GetBuffer(CString sKey);

	void		DrawCornerRoundBitmap(CDC* pDC, CRect rc);
	

protected:
	// Init
	int		Init();
	void		InitSymbol(CRect rect);
	void		SetButtonBitmaps();

	// TR
	void		Transaction();
	void		SendJumun(char cJflg, int iPrice, int iJuno = 0, int iJqty = 0);
	void		SendMicheg(CString strLedger = "");
	void		SendMaxJmcn(CString strJmdg, int iMmgb);
	void		SendSymbol();
	void		ReceiveSise(char* pcData, int iLen);
	void		Alert(char* pcData);

	// draw
	void		DrawSiseText(CDC* pDC, CString strData, CRect rect, UINT nFormat = 0, UINT type = 0, COLORREF color=0xFFFFFF);

	// etc
	BOOL		IsChildWindow(HWND hParentWnd, HWND hChild);
	CString		GetCode()	{ return m_strJCOD; }
	CString		GetAccount()	{ return m_strAcc;	 }
	CString		GetName()	{ return m_strAccName; }
	CString		GetCodeName()	{ return m_strJName; }
	CString		GetPswd();
	void		SetCode(CString strJCOD);
	void		Trigger(CString strTrigger);
	void		Domino(CString strDomino);
	void		Palette();

	void		SetChildWait(BOOL bWait);
	void		OnDLLSetFont(int point, CString sFont);
	void		Resize(int cx, int cy);

	void		LoadRoundBitmap();
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	






//	void		SetTabFocus();


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString SplitString(CString &strData, CString strToken);
	CString m_strListedQuantity;
	BOOL m_bChecking;
	void ParsingCheckPswd(CString strData, int nLen);
	CString m_strLedger;
	int m_iMmgb;
	CString m_strJmdg;
	int m_nWaitTrType;
	void CheckPswd(CString strAccount, CString strPassword);
	virtual ~CMapWnd();

	inline BOOL GetWaitMode() { return m_bWait; };
	void SetWaitMode(BOOL bWait);

	CFont*	getAxFont(CString fName = _T("굴림체"), int point = 9, bool bBold = false, bool bItalic = false);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCode();
	afx_msg void OnButtonSend();
	afx_msg void OnButtonVSize();
	afx_msg void OnButtonConfig();
	afx_msg void OnButtonReserve();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMessage2(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnHistoryCode();
	
	DECLARE_MESSAGE_MAP()
private:
	bool IsNumber(CString str);
};

