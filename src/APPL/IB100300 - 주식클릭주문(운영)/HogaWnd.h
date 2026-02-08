#pragma once
// HogaWnd.h : header file
//

#include <map>
#include <set>
using namespace std;

#include "AxScrollBar.h"
#include "UserDefine.h"
#include <afxtempl.h>
#include <afxmt.h>
#include "../../control/fx_misc/fxButton.h"

#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22
#define D10BBTN		23	//2012.10.30 KSJ 일괄취소 버튼

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12
#define D5BBTN		13	//2012.10.30 KSJ 일괄취소 버튼

#define MAX_COL		7
#define MAX_ROW		24

#define IDC_BTN_MADO_RSVD		(1003)
#define IDC_BTN_MADO_JUMN		(1004)
#define IDC_BTN_MADO_MAKET		(1005)
#define IDC_BTN_MASU_RSVD		(1006)
#define IDC_BTN_MASU_JUMN		(1007)
#define IDC_BTN_MASU_MAKET		(1008)
#define IDC_BTN_HOGA_FIX        (1009)

#define	bsTEXT		0x00000001
#define	bsCOLOR		0x00000002
#define	bsIMAGE		0x00000004	// 1 IMAGE, size DeflateRect(2, 2)
#define	bsGRAD		0x00000008	// gradiation

#define	bsPUSH		0x00000010	// push button
#define	bsFLATE		0x00000020	// flate button
#define	bsTOGGLE	0x00000040	// toggle button
#define	bsRECT		0x00000080	// draw rect only
#define	bsGENERAL	0x00000100	// general button

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd window
class CHoga
{
public:
	CString mdga;		// 매도호가           
	CString msga;		// 매수호가           
	CString dvol;		// 매도호가수량       
	CString svol;		// 매수호가수량       
	CString pdvl;		// 직전매도호가수량            
	CString psvl;		// 직전매수호가수량
};

class COrder
{
public:
	CString code;		// 종목코드
	CString juno;		// 원주문 번호
	CString mmgb;		// 매도/수 구분		1:매도 2:매수
	CString jprc;		// 주문가격
	CString jqty;		// 주문수량
	CString mcvl;		// 미체결 수량
};

class CReserve
{
public:
	CString code;		// 종목코드
	CString name;		// 종목명
	char	mmgb{};		// 매도/수 구분		1:매도 2:매수 
	int	gprc{};		// 감시 기준가격
	int	jprc{};		// 주문가격
	int	ojprc{};		// 원주문 주문가격
	int	jqty{};		// 주문수량
	int	juno{};		// 원주문번호
	int	gubn{};		// 기준가격 구분 (현재가보다 위/아래)
	char sijang{};	//KSJ 시장가 추가 시장가이면 1 아니면 0
};

class CArrow
{
public:
	int	ojno{};		// 원주문번호
	int	jflg{};		// 주문 구분
	int	mmgb{};		// 매도/수 구분		1:매도 2:매수
	int	oprc{};		// 원 가격
	int	mprc{};		// 수정 가격
};

class CHogaWnd : public CWnd
{
// Construction
public:
	CHogaWnd(class CTool* pTool, class CControlWnd* pCtrlWnd, CWnd* parent);


public:

protected:

	bool		m_bDragJumun{};	// 주문 드래그 
	bool		m_bDragResv{};	// 예약 드래그
	BOOL		m_bTracking{};	// 마우스 TRACCK

	int		m_iPosSHGA{};
	int		m_iPosHHGA{};
	int		m_iPosCHVL{};
	int		m_iPosBBTN{};	//2012.10.30 KSJ 일괄취소버튼
	int		m_iRows{};
	int		m_iMmgb{};		// 주문시 매도/수 구분 1, 2
	int		m_iFocusMmgb{};	// 포커스 매돗/수 구부 1, 2
	int		m_iFocusIndex{};	// 포커스 인덱스
	int		m_aiCol[MAX_COL]{};
	int		m_iIndexTip{};
	int		m_iHogaStart{};	// 정정취소 시작 가격

	double		m_width{};	// cell width
	double		m_height{};	// cell height
	
	CFont*		m_pfont{};
	CFont*		m_pfontB{};

	CPen*		m_ppenLine{};
	CPen*		m_ppenTable{};

	COLORREF	m_clrBG{};	// BackGround
	COLORREF	m_clrTH{};	// Table Head
	COLORREF	m_clrGH{};	// Grid Head
	COLORREF	m_clrSBG{};	// Sell BackGround
	COLORREF	m_clrBBG{};	// Buy BackGround
	COLORREF	m_clrLGFG{};	// Light Gray ForeGround
	COLORREF	m_clrUp{};	// Up, SangHan
	COLORREF	m_clrDown{};	// Down, HaHan
	COLORREF	m_clrGridHdrTxt{};	// 
	COLORREF	m_clrYellowBG{};	//노랑색
	COLORREF	m_clrNomalBG{};	//흰색


//	CString		m_strCODX;	// RTS 종목코드
public:
	CString		m_strShga;	// 상한가
	CString		m_strHhga;	// 하한가
    CString		m_strCurr;	// 현재가
	CString		m_strJijg;	// 전일종가
	CString		m_strGijun;	// 기준가
	CString		m_strSiga;	// 시가
	CString		m_strKoga;	// 고가
	CString		m_strJega;	// 저가 
	CString		m_strDiff;	// 전일대비
	CString		m_strKrgb;   //
	CString		m_strMkms;   //시장조치
protected:
	CString		m_strDvol;	// 매도호가총수량
	CString		m_strSvol;	// 매수호가총수량
    CString		m_strDcnt;	// 매도호가총건수
    CString		m_strScnt;	// 매수호가총건수
    CString		m_strDSCha;	// 잔량차

	CRect		m_rcHoga[MAX_ROW];	// 호가 영역 0:상한 21:하한 22:총잔량 ADD 23:일괄취소버튼
	CRect		m_rcDvol[MAX_ROW];	// 매도잔량
	CRect		m_rcSvol[MAX_ROW];	// 매수잔량
	CRect		m_rcMDJumun[MAX_ROW];// 매도주문
	CRect		m_rcMSJumun[MAX_ROW];// 매수주문
	CRect		m_rcMDResv[MAX_ROW];	// 매도예약
	CRect		m_rcMSResv[MAX_ROW];	// 매수예약
	CRect		m_rcHeadHoga;	// 헤드 호가
	CRect		m_rcFocus;	// new 포커스 


	CPoint		m_ptStart{};	// LButton down Point

	struct _config	m_config {};

	CHoga		m_hoga[10];
        
	CAxScrollBar	m_vbar;
	CToolTipCtrl*	m_pTooltip;
	std::unique_ptr<class CRectWnd>	m_pTrackingWnd;
	class CControlWnd*	m_pCtrlWnd;
	class CTool*		m_pTool;

	enum { KOSPI = 0, KOSDAQ, ELW, ETF }		m_eMarket;	// 종목 구분 
	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_eDepth;	// 5/10호가 구분
	enum { hgFOLD = 0, hgUNFOLD  }		m_eFdgb;		// 일반/예약 구분


	CStringArray	m_arHoga;
	CMap<int, int, class COrder*, class COrder*>	m_mapOrder;
	std::map<int, std::unique_ptr<COrder>> m_Order;

	CArray<class CReserve*, class CReserve*>	m_arReserve;
//	CArray<class CArrow*, class CArrow*>		m_arArrow;

	CArray<std::shared_ptr<class CArrow>, std::shared_ptr<class CArrow>> m_arArrow;

	std::vector<std::unique_ptr<COrder>> _vOrder;

	CCriticalSection	m_cs;	// Synchronize m_mapOrder

	CString			m_sendS;
	CWnd*			m_pParent;

	bool		m_bNewHOGA = false; // 2023 호가 단위 변경 구분값
	std::unique_ptr<CfxButton> m_pMadoRsvd, m_pMadoJumn, m_pMadoMarket, m_pMasuRsvd, m_pMasuJumn, m_pMasuMarket, m_pHogaFix;
	set<HWND> m_HandCursorWnd;
	bool m_bHogaFix;
	bool m_bClearAll;

public:
	void		ReceiveSise(char* pcData, int iLen);
	void		ReceiveMiche(char* pcData, int iLen);
	void		ReceiveAlert(char* pcData);
	void		ReceiveAlert(struct _alertR* alertR);
	void		ReceiveNotice(char* pcData);

	void		Palette();
	
	
//	CString		SetComma(CString src);
//	void		Init(CControlWnd* pCtrlWnd);
	void		SetHogaDepth(int depth);
	void		DoModalConfigDlg();
	int		SetHogaReserve();
	void		ClearJumun();
	char		GetMarket();
	CPoint		GetRect2HogaPoint(CPoint point);
	void		ChangeFont(double* lRate, CFont* pfont, CFont* boldFont=NULL);
	int			GetRowheight(){return (int)m_height;};
	void		Click_Order(CPoint point);


	CString		m_sVIUp;      //정적VI상한 
	CString		m_sVIDown;    //정적VI하한
protected:
	// Init
	int		Initialize();

	// Draw
	//void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, int clrIndex = COLOR_TEXT);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT nType = 0, COLORREF clrText = RGB(0, 0, 0));

	// To MapWnd
	CString		GetCODX();
	CString		GetAccount();
	CString		GetCondition();
	CString		GetPassword();
	CString		GetName();
	CString		GetCodeName();
	CString		GetListedQuantity(); //2014.06.23 KSJ 상장주식수 가져오기
	void		SetGuide(CString strMsg);
	void		SendJumun(char cMmgb, int iPrice, int iJuno = 0, int iJqty = 0, int bMarketPrice = 0);	//2012.11.07 KSJ 시장가일때는 1로 세팅해줌
	int			GetPrgbType();

	// Calc
	CString		MakeSangHan(int iCurr);	// elw 상한가를 위한 펑션
	CString		MakeHaHan(int iCurr);
	CString		CalculateDiffTick(CString hoga);
	CString		CalculatePercent(CString hoga);
	int		GetPriceUnit(int iPrice, bool bPlus);
	int		GetTickUnit(int price, int mmgb, int count = 0);

	// Draw
	void		DrawHead(CDC* pDC);
	void		DrawHogaFold(CDC* pDC);
	void		DrawHogaUnfold(CDC* pDC);
	void		DrawReserve(CDC* pDC);
	void		DrawArrow(CDC *pDC);
	void		drawPgdg(CDC *pDC);	

	void		DrawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder = true);
	void		DrawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder);
	void		DrawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawColorBox(CDC* pDC, CRect rc, BOOL l = TRUE, BOOL t = TRUE, BOOL r = TRUE, BOOL b = TRUE);
	void		DrawArrowBox(CDC *pDC, CPoint StartPoint);	//2012.11.15 KSJ 시장가 일때 그리기

	//  etc
	void		CreateHogaSet();
	double		Str2double(CString string, bool bABS = true);
	int		Str2int(CString string, bool bABS = true);
	void		SetHogaRect();
	void		CreateScroll();
	void		SetScroll(int nHoga);
	int		GetKey(int value);
	CString		GetHogaData(CString hoga, int dsgb);
	int		GetIndexToHoga(int index);
	CString		GetHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	CString		GetReserveData(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	int		GetPointToIndex(CPoint point, int cell, int& mmgb);
	void		RemoveJumun(int index);
	void		RemoveAllJumun();
	void		RemoveAllReserve();
	void		RemoveAllArrow();
	bool		GetConfirmJumun();
	bool		CheckJumunAvailable(bool bCheckJmcn = true);
	int		GetJumunCount(CString junos, int mmgb);
	
	
	void		GetPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd);
	void		GetPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd);
	void		CheckReserveJumun(CString code, CString curr);
	void		SetModArrow(int jflg, int price, int juno);

	// 2012.11.05 KSJ 추가
	void AllJumnCancel(int nMmgb);	//0 지정안함(기존로직), 1 매도, 2, 매수
	void AllRsvdCancel(int nMmgb);	//0 지정안함(기존로직), 1 매도, 2, 매수
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString SplitString(CString &strData, CString strToken);
	virtual ~CHogaWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHogaWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnHogaMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllMadoCancel();
	afx_msg void OnAllMasuCancel();
	afx_msg void OnAllMadoRsvdCancel();
	afx_msg void OnAllMasuRsvdCancel();
	afx_msg void OnHogaFix();
	afx_msg void OnMarketMado();
	afx_msg void OnMarketMasu();
	DECLARE_MESSAGE_MAP()


	bool IsValidHoga(int price);
};

