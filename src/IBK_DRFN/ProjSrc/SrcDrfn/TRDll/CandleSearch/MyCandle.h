#if !defined(AFX_MYCANDLE_H__53F36C8C_C4DA_416F_A010_614D863F99DE__INCLUDED_)
#define AFX_MYCANDLE_H__53F36C8C_C4DA_416F_A010_614D863F99DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCandle.h : header file
//
#include "MemDC.h"
#include "resource.h"

#define WM_NOTIFY_CANDLEPATTEN	WM_USER + 245

// 캔틀 하나의 데이터 구조체
typedef struct tagCandleData
{
	BOOL bEnable;

	int	nOpen;     // 시가
	int	nHigh;     // 고가
	int	nLow;      // 저가
	int	nClose;    // 종가
} CANDLE_DATA;

// 캔들 셋트, 최대 5개임
#define MAX_CANDLE			 5
typedef struct tagCandleSet
{
	CHAR szCandleName[256];
	CANDLE_DATA cd[MAX_CANDLE];
	int nIgnoreTail;// 꼬리옵션
	int nIgnoreColor; // 음양봉구분
	int nPreTrend;	// 이전추세
	int nApplyBodyLength; // 몸통길이 고려
	int nBodyRate[5]; // 몸통길이고려시 비율
	int nBodyRateDir[5]; // 몸통길이비율 방향 : 0:아하, 1:이상
} CANDLE_SET;

typedef CArray<CANDLE_SET*, CANDLE_SET*> CANDLESET_ARRAY;
/////////////////////////////////////////////////////////////////////////////
// CMyCandle window
class CCandleManager;

class CMyCandle : public CWnd
{
// Construction
public:
	CMyCandle();

// Attributes
public:
	int		m_nCandleIndex;
	BOOL	m_bEnable;
	int		m_nHigh;
	int		m_nLow;
	int		m_nOpen;
	int		m_nClose;
	BOOL	m_bReverse;// 시가>종가 (하락시)

	BOOL	m_bYScale;		// y축 표시
	int 	m_nYScaleWidth; // y축 표시 영역 너비

	int		m_nLRSpace;

	double	m_dHeight;

	CRect	m_rcDrawingArea;

	CButton	m_chkEnable;
	CCandleManager* m_pCandleMgr;

private:
	CBitmap	m_bitmapHigh;
	CBitmap	m_bitmapLow;
	CBitmap	m_bitmapOpen;
	CBitmap	m_bitmapClose;
	CBitmap	m_bitmapCheck;
	CImageList m_ImageList;
	CImageList m_CheckImageList;
	CPoint m_ptCheck;

	int		m_nHighT;
	int		m_nLowT;
	int		m_nOpenT;
	int		m_nCloseT;

	COLORREF m_clrBackGround;
	HCURSOR	m_hCursor;
	HCURSOR	m_hCursorCross;
	HCURSOR	m_hCursorCrossR;

	BOOL	m_bFocus;
	CFont*	m_pFont;

	BOOL	m_bTopCheckBox; // 상단에 체크박스 표시

// Operations
private:
	void	CalcRect();

	CRect	GetRect_Candle();
	CPoint	GetPoint_High();
	CPoint	GetPoint_Low();

	void	DrawBackground(CDC* pDC);
	void	DrawMark(CDC* pDC);
	void	DrawGradientRect(CDC *pDC, CRect rect, BOOL bTopDown = FALSE);

	CRect	m_rcCandle;
	CRect	m_rcHigh, m_rcLow;
	CRect	m_rcOpen, m_rcClose;
	int		HitTest(CPoint pt);

	BOOL	m_bDragMode;
	BOOL	m_bRButtonClick;
	CPoint	m_ptClick;
	int		m_nHitTestRes;
	CRect	m_rcCandleOrign;

	int		m_nParkingMargin;
	int		m_nCheckMargin;

public:
	void	SetCandleManager(CCandleManager* pManager){ m_pCandleMgr = pManager; }
	void	SetEnable(BOOL bEnable=TRUE);
	BOOL	GetEnable(){ return m_bEnable; }
	void	SetYScale(BOOL bShow, int nWidth);
	void	MoveWindow(int nX, int nY, int nWidth, int nHeight);
	void	KillFocusCandle();
	
	void	SetCandleData(int nIndex, CANDLE_DATA cd);
	void	SetCandleData(int nIndex, BOOL bCheck, int nHigh, int nLow, int nOpen, int nClose);
	void	SetFont(CFont* pFont){ m_pFont = pFont; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCandle)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyCandle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCandle)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
#if (_MFC_VER >= 0x0421)
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
	DECLARE_MESSAGE_MAP()
};

class CCandleManager
{
	CMyCandle m_CandleWnd[5];
	CString m_strRootPath;
	CANDLE_SET	m_CandleSet;
	int		m_nID;
	int		m_nFocusIndex;
	
	BOOL	m_bYScale;
	int 	m_nYScaleWidth;
	CFont*	m_pFont;


public:
	BOOL m_bCreated;
	CCandleManager()
	{
		m_nFocusIndex = 0;
		m_nID = 0;
		m_bCreated = FALSE;
		m_bYScale = FALSE;
		m_nYScaleWidth = 0;
	}
	~CCandleManager()
	{
	}
	
	int		GetFirstEnableCandle();
	int		GetLastEnableCandle();
	int		GetEnableCandleCount();

	void DrawGradientRect(CDC *pDC, CRect rect, COLORREF Start, COLORREF End, BOOL bTopDown = TRUE);
	void Create(int nID, CPoint ptWnd, CSize szCandle, CWnd* pParent, CString strRootPath);
	void DestroyCandle();
	void SetFocusCandle(int nIndex){ m_nFocusIndex = nIndex; SetFocusCandle(&m_CandleWnd[nIndex]); }
	void SetFocusCandle(CWnd* pFocusWnd);
	void SetCandleSet(CANDLE_SET* pSet);
	void MoveWindow(int nX, int nY, int nWidth, int nHeight);
	void ShowWindow(int nCmdShow);
	void EnableWindow(BOOL bEnable);
	int GetRange();
	void GetCandlePacketData(CANDLE_IN_DATA* stIn);
	void GetCandleSet(CANDLE_SET* pSet);
	void SetCheck(int nIndex, BOOL bCheck);
	void SetYScale(BOOL bShow, int nWidth);
	void SetFont(CFont* pFont);
	void Reset();
	BOOL GetWindowRect(CRect& rcWnd, int nIndex);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCANDLE_H__53F36C8C_C4DA_416F_A010_614D863F99DE__INCLUDED_)
