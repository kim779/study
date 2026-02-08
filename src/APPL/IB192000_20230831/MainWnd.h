#pragma once
// MainWnd.h : header file
//
#include "ExcelGrid.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_grid/fxGrid.h"



struct	_gridHdr {
	UINT	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	
	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	
	UINT	fidx;		// field index
};

struct _mid {
    char    fcod    [ 2][ 8];   /* 선물 종목코드 리스트(AAAAAAAA이면 default)   */
    char    scod    [40][ 6];   /* 주식 종목코드 리스트 */
};

#define MAX_CNT     2000
#define MAX_ST_R    40
#define MAX_FU_R    2


static  struct  futitem {
    char    fcod        [ 8];   /* 선물 종목코드    */
    char    fnam        [30];   /* 선물 종목명      */
    char    fprc        [ 8];   /* 선물 현재가      */
    char    fdif        [ 8];   /* 선물 전일대비    */
    char    fmba        [ 7];   /* 선물 베이시스    */
	char    fopv        [ 9];   /* 선물 미결제약정  */         
} __futitem;

static  struct  griditem {
    char    code        [ 6];   /* 종목코드             */
    char    hnam        [20];   /* 종목명               */
    char    curr        [ 9];   /* 현재가               */
    char    diff        [ 8];   /* 전일대비             */
    char    drat        [ 7];   /* 등락률               */
    char    ask1        [ 9];   /* 매도호가1            */
    char    bid1        [ 9];   /* 매수호가1            */
    char    bvol        [10];   /* 전일거래량           */
    char    gvol        [10];   /* 누적거래량           */
    char    higp        [ 9];   /* 고가                 */
    char    lowp        [ 9];   /* 저가                 */
    char    stdp        [ 9];   /* 기준가               */
} __griditem;


static  struct _mod {
    struct  futitem f1item  ;   /* 선물 최근월물 데이타         */
    char    f1av        [ 8];   /* 선물 최근월물 매도총잔량     */
    char    f1bv        [ 8];   /* 선물 최근월물 매수총잔량     */
    char    f1ac        [ 8];   /* 선물 최근월물 매도총건수     */
    char    f1bc        [ 8];   /* 선물 최근월물 매수총건수     */
    char    f1pv        [ 8];   /* 선물 최근월물 순매수총잔량   */
    char    f1pa        [ 8];   /* 선물 최근월물 순매수총건수   */
	
    struct  futitem f2item  ;   /* 선물 차기월물 데이타     */
	
    char    spr1        [ 8];   /* 선물 스프레드            */
	
    char    nrec        [ 4];   /* 레코드 카운트            */
    struct  griditem    grid[MAX_ST_R];
} __mod;

#define USE_DLL_ALERTx
/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
class ChartPoint
{
public:
	int idx{};
	int xpos{};
	double data{};
	CString sdata;
	CString stime;
	bool	bhoga{};
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent);

// Attributes
public:
	CFont* m_pFont;
	CPen* m_pLine1Pen;
	CPen* m_pLine2Pen;
	CPen* m_pDeliPen;
	CPen* m_pGridLine;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_choi_code;	//최근월물 코드
	CString m_cha_code;		//차월물 코드
	CString m_choi_curr;	//최근월 현재가
	CString m_choi_diff;	//최근월 전일대비
	CString m_medo_jan;		//매도총잔량
	CString m_mesu_jan;		//매수총잔량
	CString	m_medo_geon;	//매도총건수
	CString m_mesu_geon;	//매수총건수
	CString m_meme_jan;		//매매 순잔량
	CString m_meme_geon;	//매매 순건수
	CString m_cha_curr;		//차근월 현재가
	CString m_cha_diff;		//차근월 전일대비
	CString m_basis;		//베이시스
	CString m_migyul;		//미결제
	CString m_sum_up1;		//가격상승수
	CString m_sum_up2;		//거래량상승수
	
	CRect m_rc_choi_curr;	//최근월 현재가
	CRect m_rc_choi_diff;	//최근월 전일대비
	CRect m_rc_medo_jan;	//매도총잔량
	CRect m_rc_mesu_jan;	//매수총잔량
	CRect m_rc_medo_geon;	//매도총건수
	CRect m_rc_mesu_geon;	//매수총건수
	CRect m_rc_meme_jan;	//매매 순잔량
	CRect m_rc_meme_geon;	//매매 순건수
	CRect m_rc_cha_curr;	//차근월 현재가
	CRect m_rc_cha_diff;	//차근월 전일대비
	CRect m_rc_cha_migyul;	//미결제수량
	CRect m_rc_basis;		//베이시스
	CRect m_rc_sum_up1;		//가격상승수
	CRect m_rc_sum_up2;		//거래량상승수
	CRect m_rc_dataRect;    //데이터 영역
	CRect m_rc_title;		//제목영역

	double m_maxyval, m_minyval;
	double m_maxy2val, m_miny2val;
	CRect chartRect, drawChartRect;
	CRect m_rc_Check1, m_rc_Check2;
	bool  m_bCheck1, m_bCheck2;
	double m_lastIChg;
	int	   m_lastIVol;

//	CArray<class ChartPoint*, class ChartPoint*> m_arCPoint;
	CArray<std::shared_ptr<ChartPoint>, std::shared_ptr<ChartPoint>&> m_arCPoint;

	CString idTOstring(UINT id);
	int m_rowC;
	int m_colC;
	int		m_gridW;
	CCriticalSection m_sync;
	CFont* m_fontB;
	CFont* m_font;
	enum _viewType {viewTWO = 2, viewTHREE = 3, viewFOUR = 4 } m_viewType;
	CArray < _gridHdr, _gridHdr > m_gridHdrX;
	void initialGrid();
	CString SplitString(CString &strData, CString strToken);
	LRESULT SendTR(CString strCode, CString strData, int iKey, int iStat = NULL);
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	CString Variant(int nComm, CString strData = _T("") );
	COLORREF GetIndexColor(UINT nIndex);
	void Init();
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnSend();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	int m_lastCheg;
	int m_lastHVOL;
	bool m_bViewChart;
	void loadFieldInfo();
	int m_fontsize;
	void ParseAlert(CString pData);
	COLORREF m_BackColor;
	COLORREF m_Line1Color;
	COLORREF m_Line2Color;
	std::unique_ptr<class CfxGrid>	m_grid;
	std::unique_ptr<class CfxImgButton>	m_pBtnSel;
	std::unique_ptr<class CfxImgButton>	m_pBtnSend;
	CString m_stime;
	CRect	m_ChartRect;
	
	
	CWnd* m_pWizard;

public:		// RTM
	
	void DrawUpData(CDC *pdc);
	void CalcRow(int row);
	void CalcRowDiff(int row);
	CString m_strPal;
	int m_nOldRow;
	CArray<char *, char *>	m_arBytesRTM;
	CCriticalSection	m_syncRTM;
	CWinThread*		m_pThreadRTM;
	bool			m_bThreadRTM;
	CArray<struct _alertR *, struct _alertR *>	m_arBytesXRTM;
	
	
	void	doRTM(char *pBytes);
	void	doXRTM(struct _alertR* alertR);
	bool	WaitRTM();
private:	// RTM
	CString AddCommai(int iSrc);
	CString AddComma(CString strSrc);
	CBitmap* RequestBitmap(CString stPath);
	bool m_bChartChange;
	bool m_bDataChange;
	void RemoveCPosAll();
	CPen* RequestPen(COLORREF rgbLine, int iWidth, int iStyle);
	CPen* penLine2;
	CPen* penLine1;
	bool m_bfchg;
	bool m_bfhog;
	void AddChartData(CString time, CString sval, double yval,  int idx, bool bhoga = false);
	int m_maxidx;
	COLORREF m_clrChartBack;
	void DrawChart(CDC *pDC, CRect rc);
	COLORREF clrText;
	void initArea();
	void QueryExcelSise();
	double TOfabs(CString string);
	double TOf(CString string);
	int checkRealTimeCode(CString code, int &xrow);
	void parsingAlert(CString datB);
	void parsingAlert(struct _alertR* alertR);
	void SplitStringArray(CString &strData, CString strToken,CStringArray &ar);
	void DrawGrid();
	void DrawChart();

	HANDLE			m_hEventRTM;
	
	void	initRTM();
	void	stopRTM();
	void	removeRTM();
};

