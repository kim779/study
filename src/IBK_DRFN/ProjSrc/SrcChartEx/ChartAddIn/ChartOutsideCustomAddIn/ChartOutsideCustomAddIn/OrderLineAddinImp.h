// OrderLineAddinImp.h: interface for the COrderLineAddinImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERLINEADDINIMP_H__75976A96_2C60_4545_B9E6_6C9E691AD24F__INCLUDED_)
#define AFX_ORDERLINEADDINIMP_H__75976A96_2C60_4545_B9E6_6C9E691AD24F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I003531/_ChartAddInBase.h"				// for CChartAddInBase
#include "SM_CheJanStruct.h"
#include "CommentObj.h"


#define COLOR_ORDERLINE RGB(195, 195, 195)

#define COLOR_ORDERLINE_GRAY RGB(217, 217, 217)
#define COLOR_ORDERLINE_STOP2  RGB(254, 163, 152)  //Stop  다른 주문선 over시
#define COLOR_ORDERLINE_LIMIT2 RGB(164, 200, 255)  //Limit 다른 주문선 over시


#define CLICKEDPOS_NONE	0
#define CLICKEDPOS_ORDERLINE 1
#define CLICKEDPOS_TABLE 2

#define TRIANGLE_SIDE 10
#define TRIANGLE_HEIGHT 12

#define LINE_BASE  0
#define LINE_STOP  1
#define LINE_LIMIT 2

typedef enum ORDERLINE_POSITION
{
	LINE_POS_IN,			//그래프 영역 안
	LINE_POS_OUT_UP,		//그래프 영역 벗어난 위쪽
	LINE_POS_OUT_DOWN		//그래프 영역 벗어난 아래쪽
};

typedef struct _StopLine
{

}StStopLine;
typedef struct _LimitLine
{

}StLimitLine;

class COrderLine 
{
public:
	COrderLine();
	COrderLine(CPoint ptPrev, CPoint ptEnd, double dOrderPrice);
	virtual ~COrderLine();

public:
	
	int			m_nIndex;
	CPoint				m_ptEscape_Start;   //영역 에서 벗어날시 삼각형 시작 좌표 위치.
	ORDERLINE_POSITION  m_eLinePosition;    //라인 영역 
	ORDERLINE_POSITION  m_eStopLinePosition;     //Stop라인 영역 
	ORDERLINE_POSITION  m_eLimitLinePosition;    //Limit라인 영역 

	BOOL		m_bSelected;
	BOOL		m_bShow;
	BOOL		m_bMouseOver;
	int			m_nSymbol;
	int			m_nFlagSellorBuy;	
	int			m_nOrderType;
	double		m_dOrgOrderAmount;
	double		m_dAmount;
	double		m_dRate;
	double		m_dOrderPrice;
	CString		m_strOpenTime;
	CString		m_strExeTime;

	CPoint		m_ptStart, m_ptEnd;
	CPoint		m_ptStartChgLine, m_ptEndChgLine;	


	//stop, limit
	CRect		m_rctStopLine;
	CRect		m_rctLimitLine;
	CPoint		m_ptStartStop, m_ptEndStop;
	CPoint		m_ptStartChgStopLine, m_ptEndChgStopLine;	
	CPoint		m_ptStartLimit, m_ptEndLimit;
	CPoint		m_ptStartChgLimitLine, m_ptEndChgLimitLine;
	CPoint		m_ptStopTableOffset, m_ptLimitTableOffset;
	CRect		m_rctStopTable, m_rctLimitTable;
	CRect		m_rctStopText, m_rctLimitText;
	//

	CRect		m_rct;
	CRect		m_rctLine;
	CRect		m_rctTable;
	CRect		m_rctText;
	CRect		m_rctBackup;

	

	CPoint		m_ptTableOffset;
	CPoint		m_ptLBtnDown;
	CPoint		m_ptBackupTalbeOffset;
	CPoint		m_ptLineOffset;
	
	CString		m_strOrderID;			// 원주문번호
	CString		m_strPositionID;		// 포지션번호
	CString		m_strRate;				// 체결, 미체결 가격
	CString		m_strStop;
	CString		m_strLimit;

	int			m_nPanOffset;
	BOOL		m_bCon;					//체결, 미체결 플래그
	BOOL		m_IsStop;
	BOOL		m_IsLimit;

	CString		m_strCustItem1;
	CString		m_strCustItem2;
	CString		m_strCustItem3;
	CString     m_strSide; //매매구분
	
public:
	POSITION	m_pos;
	int			m_nPenStyle;
	int			m_nPenThickness;
	COLORREF	m_nPenClr;
	int			m_nCurClickedPos;

	COLORREF	m_clrCurBrush;
	COLORREF	m_clrOpen;
	COLORREF	m_clrPend;
	COLORREF	m_clrStop;
	COLORREF	m_clrLimit;
public:
	CPoint	GetPrevPoint();
	CPoint	GetEndPoint();
	void	ShowOrderLine(CDC* pDC);
	void	HideOrderLine(CDC* pDC);

public:
	BOOL	IsInRegion(CPoint pt);  
	BOOL	IsOnLine(CPoint pt);   
	BOOL	IsOnTable(CPoint pt);  

	void    GetRectTypeStop();
	void    GetRectTypeLimit();
	
	CRect	GetRect();
	CPoint	GetPostion();
	void	SetPosition(CPoint pt, int nType = 0);
	void	MouseLButtonDown(const CPoint &pt);
	void	MouseLButtonUp(const CPoint &pt);	
	void	MouseRButtonUp(const CPoint &pt);
	
public:
	void	Draw(CDC* pDC);
	void	DrawOrderLine(CDC* pDC);	
	void	DrawChgLine(CDC* pDC);	
	void	DrawOrderLineOnMouseOver(CDC* pDC);
	void	DrawOrderText(CDC* pDC);
	void	DrawOrderTable(CDC* pDC);
	void    DrawEscapeTriangle(CDC* pDC);	
	CString GetToolTipText();

	void	SetOpenPositionData(FX_CHE_STRUCT* pStOpenPosition);
	void	SetPendingOrderData(FX_CHE_STRUCT* pStOpenPosition);
};

class COrderLineAddInImp : public CChartAddInBase
{
public:
	COrderLineAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~COrderLineAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

public:
	static CString		m_strAddInItemName;
	COrderLine* m_pSelectedOrderLine;
	CObList		m_OrderLineList;
	CObList     m_OrderLineStopList;
	CObList     m_OrderLineLimitList;


public:
	BOOL		m_bLog;
	BOOL		m_bReverse;
	BOOL		m_bRun;
	BOOL		m_bComment;
	BOOL		m_bIsLock;
	BOOL		m_bOnSetCaprture;
	BOOL		m_bSplit;

	CRect		m_rctGraphRegion, m_rctGraphDrawingRegion;
	CRect		m_rctLVScaleRegion, m_rctRVScaleRegion;	
	HCURSOR		m_hAPCursor;

	double		m_dViewMin, m_dViewMax;
	int			m_nSelecteYValue;
	CToolTipCtrl m_ToolTip;
	CString		 m_strToolTipText;

public:
	int	   m_nCount_EscapeUp;
	int	   m_nCount_EscapeDown;
	//PAN
	double m_dwMouseActionID;	
	double m_dPanStart;
	double m_dBarWidth;
	int	   m_nStartIndex;
	int	   m_nEndIndex;;
	BOOL   m_bLButtonDown;
	CPoint m_ptPanStart;
	CPoint m_ptPrevPoint;

	//YZOOM
	int		m_nRow;
	int		m_nColumn;
	CPoint  m_ptYZoomStart;
	int		m_nVScaleIndex;
	int		m_nScaleHeight;

	int		m_nMMGb;

	CArray<CCommentItem*, CCommentItem*>	m_arrCommentList;
	CFont		m_boldFont;

public:
	void SetMouseCursor(int nType);
	void SetOpenPosition(FX_CHE_STRUCT* pStOpenPosition); //체결
	void SetPendingPosition(FX_CHE_STRUCT*	pStPendingOrder); //미체결

	BOOL CheckTypeAndSet(FX_CHE_STRUCT* pDataPosition, int nLineType);
	void AddOpenPosition(FX_CHE_STRUCT* pStOpenPosition, int nLineType); //체결
	void AddPendingPosition(FX_CHE_STRUCT* pStOpenPosition, int nLineType); //미체결

	void CheckAllTypeLineAndDoFunc(CObList* pOrderLineList, CDC* pDC);
	void CheckIsLineInScreen(COrderLine* pOrderLine);
	void SetData(COrderLine* pOrderLine, FX_CHE_STRUCT* pDataPosition, int nType, int nLineType);
	void SetOrderInfomation(CPoint point);
	void DrawVertText(HDC p_hDC, CPoint pt);
	void DrawHorzText(HDC p_hDC, CPoint pt);
	void DrawCrossLine(HDC p_hDC, CPoint pt);
	BOOL FindandSet(FX_CHE_STRUCT* pStOpenPosition, int nLineType, CString& strData);
	BOOL IsPointInLine(CObList* obOrderLineList, CPoint point);
	BOOL SetLineStatusOnMouseMove(CPoint point);
	void SetOrderLineShowAndMouseOver(POSITION pos, CObList* obOrderLineList, BOOL bMouseOver);

	void SetCommentData(FX_CHE_STRUCT* pDataPosition);
	BOOL SetCommentStatusOnMouseMove(CPoint point);
	void SetRemove_CommentData();
	int	 GetIndexFromValue(CString strTime, long& lDate);

protected:
	BOOL GetGraphBlockRegion( void );
	BOOL GetGraphVScaleRegion( void );
	void SetRemove_OrderLine();
	BOOL FindLineAndSelect(CObList* obOrderLineLiset, CPoint point);

	void InitToolTip();
	void AddToolInTooltip(const CString& strText, const bool bActivate);
	void SetToolTipText(const CString& strText);
	void SetToolTipText(const CString& strText, const bool bActivate);

	BOOL	OnLButtonDownForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	BOOL	OnMouseMoveForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	BOOL	OnLButtonUpForYZoom(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);

protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual void	OnResetChart( void);
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	virtual RECT	DrawCursor( HDC p_hDC);
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption );
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ);
//	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate);
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);	

};

#endif // !defined(AFX_ORDERLINEADDINIMP_H__75976A96_2C60_4545_B9E6_6C9E691AD24F__INCLUDED_)
