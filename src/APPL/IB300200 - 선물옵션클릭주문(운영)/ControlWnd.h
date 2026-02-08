#pragma once

#include "MapWnd.h"

struct _button
{
	CRect	rcChk;
	CRect	rcText;
	CString data;
	bool	chk{};
	bool	bRadio{};
};

const int rdCnt = 2;
enum { RD_NOT = 0, RD_IOC, RD_FOK };
/////////////////////////////////////////////////////////////////////////////
class CControlWnd : public CWnd
{
public:
	CControlWnd(CWnd* pParent, CWnd* pView);
	virtual ~CControlWnd();

	//{{AFX_VIRTUAL(CControlWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonAccount();
	afx_msg void OnButtonChart();
	afx_msg void OnButtonReval();
	afx_msg void OnButtonCan();
	afx_msg void OnButtonJmcn();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnContainerAmount(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CMapWnd* m_pParent{};
	CWnd* m_pView{};
	std::unique_ptr<class CfxImgButton> m_pBtnAccount{};
	std::unique_ptr<class CfxImgButton> m_pBtnChart{};
	std::unique_ptr<class CfxImgButton> m_pBtnRevaluate{};
	std::unique_ptr<class CfxImgButton> m_pBtnCan{};
	std::unique_ptr<class CfxImgButton> m_pBtnJmcn{};
	std::unique_ptr<class CfxImgButton> m_pBtnRsCancel{};
	std::unique_ptr<class CfxImgButton> m_pBtnJmCancel{};

	std::unique_ptr<class CfxEdit> m_pEditJmdg{};
	std::unique_ptr<class C3DEdit> m_pEditJmcn{};
	std::unique_ptr<class C3DEdit> m_pEditTick{};
	std::unique_ptr<class CSpinCtrl> m_pSpinJmcn{};
	std::unique_ptr<class CSpinCtrl> m_pSpinTick{};
	std::unique_ptr<class CContainerWnd> m_pcontwndAmount{};
public:
	std::unique_ptr<class CJangoWnd> m_pJangoWnd{};
	bool	m_bMarketConfirm{};
protected:
	std::unique_ptr<class CContainerWnd> m_pAmount{};

	CRect		m_sRc1, m_sRc2, m_sRc3, m_sRc4;		// 테투리 : 주문가능, 예약주문, 원클릭, 잔고
	
	struct	_jmsr	{
		CString		text;
		CRect		rect;
		COLORREF	color{};
	} m_jmsr[3][3]{};						// 주문가능수량 0:row 1:col

	enum { R_TITLE = 0, R_MD, R_MS };			// row
	enum { C_TITLE = 0, C_NEW, C_CLEAR };			// col
	CRect		m_jmsrRc;				// 주문가능 수량 invalidate영역
	struct _button	m_chkJmsr;				// 주문가능수량자동적용 check

	CRect		m_tRc3;		// 주문 가격
	CRect		m_tRc4;		// 주문수량
	CRect		m_tRc5;		// 예약매매 주문틱 설정

	CString		m_text3;	// 주문가격
	CString		m_text4;	// 주문수량
	CString		m_text5;	// 예약주문틱

	// 잔고 title
	CString		m_tgubn;	// 구분
	CString		m_tjgsu;	// 잔고수량
	CString		m_tjmga;	// 주문가능
	CString		m_tpgga;	// 평균단가
	CString		m_tpgsk;	// 평가손익

	// 잔고 title rect
	CRect		m_rgubn;	// 구분
	CRect		m_rjgsu;	// 잔고수량
	CRect		m_rjmga;	// 주문가능
	CRect		m_rpgga;	// 평균단가
	CRect		m_rpgsk;	// 평가손익

	// 잔고 data
	CString		m_ogubn;	// 구분
	CString		m_ojgsu;	// 잔고수량
	CString		m_ojmga;	// 주문가능
	CString		m_opgga;	// 평균단가
	CString		m_opgsk;	// 평가손익

	// 잔고 data rect
	CRect		m_rogubn;	// 구분
	CRect		m_rojgsu;	// 잔고수량
	CRect		m_rojmga;	// 주문가능
	CRect		m_ropgga;	// 평균단가
	CRect		m_ropgsk;	// 평가손익

	CRect		m_oRc3;		
	CString		m_output3;
	CString		m_output4;	// 보유수량
	CString		m_output5;	// 평균단가
	CString		m_output6;	// 평가손익

	CString		m_curr;
	CString		m_cmsg;		// 총매입금액
	CString		m_ssly;		// 수수료율
	CString		m_maip;		// 총매입금액
	CString		m_jango;	// 잔고
	CString		m_mouseDLL;

	struct _button	m_multi;
	struct _button	m_oneclick;
	struct _button	m_clickmod;
	struct _button	m_trackmod;
	struct _button  m_market;
	CComboBox	m_cbMulti;

	struct	_button	m_rdNOT {};
	struct	_button	m_rdIOC {};
	struct	_button	m_rdFOK {};
	int		m_rdSel{};

	struct _button	m_chkHold {};

	CString		m_home;
	CString		m_user;

	CFont*		m_pFont;
	CFont*		m_pBFont;

	int		m_mmgb{};
	bool		m_bInit{};

	std::unique_ptr <CToolTipCtrl> m_pTipCtrl{};

private:
	HBITMAP		m_hBitmap2{}, m_hBitmap2_dn{}, m_hBitmap2_hv{};
	HBITMAP		m_hBitmap1{}, m_hBitmap1_dn{}, m_hBitmap1_hv{};
	HBITMAP		m_hBitmapJMCN{}, m_hBitmapJMCN_dn{}, m_hBitmapJMCN_hv{};

public:
	void		Init();
	void		DispatchData(char* pData, int len);
	void		DispatchMicheg(char *pData, int len, bool bErase=true);
	void		NoticeParse(CString dat, CMapStringToString& ary);
	int		GetJumunCount();
	int		GetTickCount();
	int		GetPGDG();
	void		SetMicheg(CString code, int cnt);
	void		SetPrice(int mmgb, int price);
	void		SetJmdg(CString jmdg);
	void		SetJmcn(CString code, CString jmcn);
//	void		SetMaxJmcn(CString oldj, CString newj, int mmgb);
	void		SetCanClearS(CString str);
	void		SetCanClearB(CString str);
	int		GetMultiCount();
	void		SetSize(int depth);
	void		SetJango(CString code, CString jgvl, CString pgdg, CString pgga, CString jmgr, CString pgsk, bool bAll);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, COLORREF tcolor=0);
	void		WriteFile(char* pBytes, int nBytes);
	void		ClearJango();
	void		ClearMaip();
	bool		GetTrackMode()  { return m_trackmod.chk; }
	bool		GetOneClick()	{ return m_oneclick.chk; }
	bool		GetClickMod()	{ return m_clickmod.chk; }
	bool		GetChkHold()	{ return m_chkHold.chk; }
	bool		GetMarket()	{ return m_market.chk; }
	CString		GetMaip()	{ return m_maip; }
	CString		GetJango()	{ return m_jango; }
	CString		GetJmgb();
	bool		GetChkJmsr()	{ return m_chkJmsr.chk; }
	CString		Parser(CString &srcstr, CString substr);
	void		SearchCan();
	BOOL GetWaitMode();
	void SetWaitMode(BOOL bWait);
	void		ResizeControl(int cx, int cy);
	void		ChangeFont(double* lRate, CFont* pfont, CFont* boldFont);



protected:
	void		setButtonBitmaps();
	int		calculateCommission(int money);
};

