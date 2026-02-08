#pragma once
// HogaWnd.h : header file
//
#include "MapWnd.h"
#include "AxScrollBar.h"
#include "ControlWnd.h"
//#include <afxtempl.h>
//#include <afxmt.h>

#include <map>
#include <vector>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
using namespace std;

/*
#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12

#define MAX_COL		7
#define MAX_ROW		23
*/
#define	D10SHGA		0
#define D10HHGA		21
#define D10CHVL		22
#define D10BTN		23

#define	D5SHGA		0
#define D5HHGA		11
#define D5CHVL		12
#define D5BTN		13

#define MAX_COL		7
#define MAX_ROW		24


/////////////////////////////////////////////////////////////////////////////
// CHogaWnd window
class CHoga
{
public:
	CString mdga;		// 매도호가           
	CString msga;		// 매수호가           
	CString dvol;		// 매도호가수량       
	CString svol;		// 매수호가수량       
	CString dcnt;		// 직전매도호가수량            
	CString scnt;		// 직전매수호가수량
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
	int	mmgb{};		// 매도/수 구분		1:매도 2:매수
	double	gprc{};		// 감시 기준가격
	double	jprc{};		// 주문가격
	double  ojprc{};		// 원주문 주문가격
	int	juno{};		// 원주문번호
	int	jqty{};		// 주문수량
	int	gubn{};		// 기준가격 구분 (현재가보다 위/아래)
	int	tick{};		// 틱
	int index{};      // 인덱스
	bool market{};	//시장가주문 여부
};

class CArrow
{
public:
	int	ojno{};		// 원주문번호
	int	jflg{};		// 주문 구분
	int	mmgb{};		// 매도/수 구분		1:매도 2:매수
	double	oprc{};		// 원 가격
	double	mprc{};		// 수정 가격
};

class CHogaWnd : public CWnd
{
// Construction
public:
	CHogaWnd(CWnd* pParent, CWnd* pView);

public:
	void DrawScreen();

public:
	CMapWnd* m_pParent{};
	CWnd* m_pView{};
	CControlWnd* m_pCtrlWnd{};

	std::unique_ptr <CExButton> m_mdStopBtn{};
	std::unique_ptr <CExButton> m_mdJmBtn{};
	std::unique_ptr <CExButton> m_msJmBtn{};
	std::unique_ptr <CExButton> m_msStopBtn{};

	std::unique_ptr <class CRectWnd> m_pTrackingWnd{};

	CFont* m_pFont{};
	CFont* m_pBFont{};

	CString		m_home;
	CString		m_user;
	CString		m_file;
	CString		m_code;
	CString		m_mouseDLL;

	struct _config	m_config {};

	enum { FUTURE = 0, OPTION }		m_market;	// 선물/옵션 구분
	enum { hgDEPTH10 = 0, hgDEPTH5 }	m_depth;	// 5/10호가 구분
	enum { hgFOLD = 0, hgUNFOLD  }		m_fdgb;		// 일반/재매매 구분


	int		m_posSHGA{};
	int		m_posHHGA{};
	int		m_posCHVL{};
	int		m_posBTN{};
	int		m_rows{};
	double		m_width{};	// cell width
	double		m_height{};	// cell height

	int		m_col[MAX_COL]{};

	CHoga		m_hoga[5]{};

	CString		m_shga;		// 상한가
	CString		m_hhga;		// 하한가

        CString		m_curr;	        // 현재가
	CString		m_jijg;		// 전일종가
	CString		m_pivot;	// 기준가
	CString		m_siga;		// 시가
	CString		m_koga;		// 고가
	CString		m_jega;		// 저가 

   	CString		m_gsm4;		// 4일 종가합
	CString		m_gsm9;		// 9일 종가합

	CString		m_diff;		// 전일대비

	CString		m_jggy;		// 증거금율
	CString		m_res2;		// 2차저항
	CString		m_res1;		// 1차저항
	CString		m_pivt;		// 피봇값
	CString		m_spp1;		// 1차지지
	CString		m_spp2;		// 2차지지 
	CString		m_resd;		// D저항
	CString		m_sppd;		// D지지   

	CString		m_dvol;         // 매도호가총수량
	CString		m_svol;         // 매수호가총수량
    CString		m_dcnt;         // 매도호가총건수
    CString		m_scnt;         // 매수호가총건수
    CString		m_dscha;	// 잔량차
        
	double		m_hogaStart{};	// 정정취소 시작 가격
	CPoint		m_ptStart{};	// LButton down Point
	int		m_mmgb{};		// 주문시 매도/수 구분 1, 2
	int		m_focusMmgb{};	// 포커스 매돗/수 구부 1, 2
	int		m_focusIndex{};	// 포커스 인덱스
	bool		m_bDragJumun{};	// 주문 드래그 
	bool		m_bDragResv{};	// 예약 드래그
	bool		m_bOneClick{};	// 원클릭 주문
	bool		m_bClickMod{};	// 원클릭 정정
	BOOL		m_bTracking{};	// 마우스 TRACCK

	CStringArray	m_arHoga;
	CMap<int, int, class COrder *, class COrder *>	m_mapOrder;
	CArray<class CReserve *, class CReserve *>	m_arReserve;
	CArray<class CArrow *, class CArrow *>		m_arArrow;

	CCriticalSection	m_cs;		// synchronize m_mapOrder
//	CCriticalSection	m_csResv;	// synchronize Reserve Order

	CAxScrollBar	m_vbar;
	std::unique_ptr <CToolTipCtrl> m_tooltip{};
	int		m_indexTip{};
/*
	CRect		m_rcHoga[23];	// 호가 영역 0:상한 21:하한 22:총잔량
	CRect		m_rcDvol[23];	// 매도잔량
	CRect		m_rcSvol[23];	// 매수잔량
	CRect		m_rcMDJumun[23];// 매도주문
	CRect		m_rcMSJumun[23];// 매수주문
	CRect		m_rcMDResv[23];	// 매도예약
	CRect		m_rcMSResv[23];	// 매수예약
*/
	CRect		m_rcHoga[24];	// 호가 영역 0:상한 21:하한 22:총잔량
	CRect		m_rcDvol[24];	// 매도잔량
	CRect		m_rcSvol[24];	// 매수잔량
	CRect		m_rcMDJumun[24];// 매도주문
	CRect		m_rcMSJumun[24];// 매수주문
	CRect		m_rcMDResv[24];	// 매도예약
	CRect		m_rcMSResv[24];	// 매수예약
	CRect		m_rcHeadHoga;	// 헤드 호가
	CRect		m_rcFocus;	// 포커스 

	bool		m_bReal{};

	CRect		m_rcWnd;
	/*
	struct	_reserve	{
		CString	code;
		CString	curr;
	};
	CArray <_reserve, _reserve>	m_rtsReserve;
	*/

	
public:
	/* callback-function Timer를 이용하기 위함. event->function */
	CString		m_rsvCode;
	CString		m_rsvCurr;

public:
	bool		m_bHold;
public:
	bool bAlertClose;
	CCriticalSection m_alertcs;
	HANDLE m_alertevt, m_alertth;
	vector<int> m_alertmsg;

public:
	void		drawHead(CDC* pDC);
	void		drawHogaFold(CDC* pDC);
	void		drawHogaUnfold(CDC* pDC);
	void		drawReserve(CDC* pDC);
	void		drawArrow(CDC *pDC);
	void		drawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder = true);
	void		drawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder);
	void		drawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawStar(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb = 3);
	void		DrawColorBox(CDC* pDC, CRect rc, BOOL l = TRUE, BOOL t = TRUE, BOOL r = TRUE, BOOL b = TRUE);

	double		getPriceUnit(double price, bool bFuture, bool bPlus);
	double		getTickUnit(double price, bool bFuture, int mmgb, int count = 0);
	void		createHogaSet();
	double		str2double(CString string, bool bABS = true);
	int		str2int(CString string, bool bABS = true);
	void		setHogaRect();
	void		createScroll();
	void		setScroll(int nHoga);
	int		getKey(double value);
	CString		getHogaData(CString hoga, int dsgb);
	double		getIndexToHoga(int index);
	CString		getHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	CString		getReserveData(CString hoga, int mmgb, CRect rc, bool bTotal = false);
	int		getPointToIndex(CPoint point, int cell, int& mmgb);
	int		getHogaToIndex(double hoga);
	void		removeJumun(int index);
	void		removeAllJumun();
	void		removeAllReserve();
	void		removeAllArrow();
	bool		getConfirmJumun();
	bool		checkJumunAvailable(int mmgb = 1);
	int		getJumunCount(CString junos, int mmgb);
	int		getMichegCount();
	CString		calculatePercent(CString hoga);
	CString		calculateDiffTick(CString hoga);
	void		getPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd);
	void		getPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd);
public:
	/* callback-function timer 이용을 위해 public메소드로 변경함 */
	void		checkReserveJumun(CString code, CString curr);
public:
	void		SetModArrow(int jflg, double price, int juno);
	void		OneClickJumun(CPoint point);
	void		ClickMod(CPoint point);

	void		invalidateRC();
	void		onBtn(int mmgb, bool bReserve);

public:
	void		DispatchData(char* pData, int len);
	void		DispatchMicheg(char *pData, int len, bool bErase=true);
	void		DispatchAlertX(struct _alertR* alertR);
	void		DispatchNotice(CString Notice);
	void		DispatchJango(CString Jango);
	void		NoticeParse(CString dat, CMapStringToString& ary);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0, int clrIndex = COLOR_TEXT);
	COLORREF	GetIndexColor(int index);
	CBrush*		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	CFont*		GetAxFont(CString fName = "굴림", int point = 9, bool bItalic = false, int nBold = 0);
	CString		SetComma(CString src);
	void		Init(CControlWnd* pCtrlWnd);
	void		SetHogaDepth(int depth);
	void		DoModalConfigDlg();
	int		SetHogaReserve();
	void		ClearJumun();
	CPoint		GetRect2HogaPoint(CPoint point);

	
	void		SetHold(bool bHold);
	int		GetRowheight(){return (int)m_height;};
	void		ChangeFont(CFont* pFont, CFont* pBFont);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHogaWnd();

	// Generated message map functions
public:
	//{{AFX_MSG(CHogaWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonMdStop();
	afx_msg void OnButtonMdJm();
	afx_msg void OnButtonMsJm();
	afx_msg void OnButtonMsStop();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CPoint trackpoint;
	CString SplitString(CString& strData, CString strToken);
};

