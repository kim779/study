// ObjMgr.h: interface for the CObjMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJMGR_H__ECB3AE34_6673_4BA1_BF69_9B69D5103D9D__INCLUDED_)
#define AFX_OBJMGR_H__ECB3AE34_6673_4BA1_BF69_9B69D5103D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <afxtempl.h>
#include "SMemDC.h"

#include "ToolTip.h"

#define	TIMER_GRID	10000
#define	TIMER_IMG	10001
#define	TIMER_HIDE	10002
#define	TIMER_HGAP	250
// tooltip
#define	TIMER_PID	10003
#define	TIMER_AID	10004
#define	TIMER_PGAP	500
#define	TIMER_AGAP	5000

//////////////////////////////

#define	GWE_MSLBDN	1
#define	GWE_MSLBUP	2
#define	GWE_MSRBDN	3
#define	GWE_MSRBUP	4
#define	GWE_MSMOVE	5
#define	GWE_MSDBLC	6
#define	GWE_MSWHEEL	7
#define	GWE_KEYBOARD	8
#define	GWE_FOCUS	9

struct	_sRegion{	// rect basic structure
	int	left;
	int	top;
	int	right;
	int	bottom;
};

struct	_mRegion{	// delete mark info
	int		rgnKey;
	_sRegion	org;
	_sRegion	left;
	_sRegion	right;
};

struct	_xRegion {	// drawing region info
	CRect		xRect;				// current rect
	_sRegion	pRect;				// 백분율 - x100
};


class CPnChart;

class CObjMgr
{
public:
	CObjMgr(CWnd* pwndView, CPnChart* pwndPnChart, char* pcEnvInfo, COLORREF clrBack, CFont* pfont);
	virtual ~CObjMgr();

public:
	CWnd*	m_pwndView;
	CPnChart*	m_pwndPnChart;

	CRect	m_rectObj;
	CRect	m_rectGrp;

	CString	m_strData;

	class	CAxisGMainApp*	m_pGMainApp;
	class	CDataFormat*	m_pDataFormat;
	class	CGrpGrid*	m_pGrpGrid;
	class	CCrossLine*	m_pCrossLine;
	class	CObjEvent*	m_pObjEvent;

//	CAxisGMainApp	*m_pApp;

	CMap	<int, int, struct _xRegion*, struct _xRegion*> m_mapRegion;
	CArray	<class CIndcBase *, class CIndcBase *> m_arGraphQue;	// Graph Array


private:
	bool	m_bDrawAll;
//	bool	m_bDrawMouse;
//	bool	m_bRefresh;			// forcely refresh
	bool	m_bTableMode;
	bool	m_bOrderMode;

	int	m_iCoDrawCnt;
//	int	m_dUnit;
	int	m_iDtIndex;

	CFont*	m_pFont;
	SMemDC*	m_psdcBitmap;

	CRect	m_rcCrossX;
	CRect	m_rcCrossY;
	CString	m_strCrossX;
	CString	m_strCrossY;

	CSize	m_aszTick[3];			// left tick Size
	CRect	m_arectXTick[3];

	COLORREF	m_crBG;

	class CDataMgr*	m_pDataMgr;
	struct _CoDraw*	m_pCoDraw;

	CArray	<class CIndcBase *, class CIndcBase *> m_arGraphQueToDraw;	// 그릴거

	CCriticalSection	m_cs;


public:
	char*	GetGrpHeader(int* piLen);
	char*	GetGrpHeader2(int* piLen);
	char*	GetGrpHeaderS(int* piLen);
	struct _envInfo*	GetEnvInfo();
	char*	GetMapInfo();
	char*	GetMapInter();

	int	DispatchGraphData(struct _trData* pTD);
	int	DispatchGraphDataS(struct _trData* pTD);
	bool	RealtimeGraphData(char* pcRTData);
	bool	RealtimeProc(CString sCode, CString sData);
	void	ObjectClear();
	bool	ResizeGraphObject(bool bForceResize = true);
	void	DrawGraphObject();
	void	ReDrawGraphObject(bool bAll = true, bool bMouse = false);
	bool	RemoveGraphRegion(int iRgnKey);
	void	SetDisplayGraph(int start, int end);
	void	SaveRegion();
	int	GetDisplayDay();
	void	SetReverseMode();
	void	SetCrossMode();
	void	SetJTraceMode();
	void	SetOrderMode(bool bOrderMode);
	void	SetTipMode(bool bMsgSend = true);
	void	SetGridMode(bool bMsgSend = true);
	void	SetTableMode(bool bView, bool bMsgSend = true);
	void	SetLogMode();
	bool	IsReverseMode();
	bool	IsCrossMode();
	bool	IsJTraceMode();
	bool	IsOrderMode()	{ return m_bOrderMode; }
	bool	IsGridMode();
	bool	IsLogMode();
	bool	IsAutoScr();
	void	SetUserDefined(CIndcBase *pIndcBase);
	void	InitGraph();
	bool	DeleteGraph(class CIndcBase* pindcDelete);
	void	MapConfig();
	void	ChartConfig(CIndcBase *pIndcBase);
	void	ToolConfig(class CToolBase *pToolBase);
	bool	ChangeMainGraph(int iKind);
	void	ChangeMap(char *info);
	void	ChangeMenuMKind(int kind);
	void	ChangeMenuAKind(int kind);
	bool	ChangeAidGraph(bool bAdd, struct _graph *pGrp);
	void	ChangeEnvInfo();
	void	ChangeIndex(int index, int gap);
	void	ChangeUnit(int unit);
	void	ChangeTotalDay(int total);
	int	ChangeDisplayScale(int scale);
	bool	ChangeDisplayShift(int shift);
	bool	ChangeDisplayPos(int dispPos, int dispEnd);
	bool	ChangeDisplay(int iDispDay);
	bool	ChangeTool(int select);
	void	ChangeToolColor(int rgb);
	void	ChangeToolWidth(int width);
	int	GetScrollInfo(char *pBuf);
//	int	SetScrollInfo(char *pBuf);	// none body?
	void	SetAutoScroll(bool bSet);
	void	ReloadGrp(char *pData, bool bTableReset = true);

	class CIndcBase* GetMainGraph();
	int	GetMainKind();
	bool	GetGrpRegion(int key, CRect& rect);
	int	GetRegionAdjustBottom(int bottom, int& bottomKey);
	int	GetCountGraphinRgn(int rgnKey);
	
	void	ReviseTick();
	void	Excel();
	bool	GetExcelData(CString &data);
	char*	GetTableData();
	char*	GetPrintString();
	char*	GetSaveString();
	void	SaveImage();
	void	Print();
	void	MouseEvent(int event, CPoint pt, int option);
	void	HideGrid();
	void	CreateGrid(bool bCreate);
	int	GetUnusedRegionKey();
	void	GetRTSCode(CString &code);
	bool	GetCrossValue(CPoint pt, CIndcBase *pIndcBase, bool bForce, CString sYPosVal = "");
	void	ShowCrossLine(CPoint pt, CString sPosVal);
	void	HideCrossLine();
	bool	GetTableMode()	{ return m_bTableMode; };
	void	SetCrBG(COLORREF crBG)	{ m_crBG = crBG; };

	//** 2005.12.07 macho
	BOOL GetXY(LPCTSTR time, LPCTSTR price, int &x, int &y);

	void	YAxisConfig(CIndcBase* pIndcBase);
	bool	YShowConfigByMove(CIndcBase* pindcMove, int iMovRgn);
	bool	YShowConfigByMenu(CIndcBase* pindcConfig);

private:
	int	CharToInt(const char* cnvB, int cnvL);
	bool	IsValidGraphHeader(int iKey, int iRegion);
	void	CalculateTk(int iWhichTK);
	void	SyncGraphDisplay(class CIndcBase* pindcSrc, class CIndcBase* pindcDst);
	bool	AddGraphData(struct _trData* pTD, int& riValidDataCount);
	bool	AddGraphDataS(struct _trData* pTD);
	void	AddGraphRegion(int iRgnCnt, char* pcData);
	void	DrawChart(CDC *pDC);
	void	DrawCrossStr(CDC *pDC);
	void	DrawEmpty(CDC* pDC, CRect rc);
	void	DrawEtc(CDC *pDC);
	int	GetGraphCount(int iRgnKey, int iGKind);
	int	GetUnusedGraphKey();
	void	RealTableData(bool bShift);
	void	RemoveGraphData();
	void	RemoveGraphQue();
	bool	RemoveGraphQue(class CIndcBase* pBase);
	bool	RemoveGraphQue(int rgnKey);
	void	RemoveGraphRegion();
	void	RemoveGrpComponent(bool bClearAll = true);
	void	ReviseMinMax();
	void	UnionGraph(CIndcBase* pIndcBase, WORD wGKind, WORD wUnionGKindNew, WORD wUnionGKindOld);

	struct _CoDraw* GetCoDrawInfo(int iRgnKey, bool bTick, int iKind);
	class CIndcBase* GetVolGraph(int iGKind);
	class CIndcBase*	GetAmtGraph(int gkind);
	class CIndcBase*	AddGraphQue(int iGrpCnt, char* pcData, bool bAppend = false, bool bUnion = false);
public:
	BOOL	IsExistGraph(int iKind);
	BOOL	DrawCntIsZero(); // true: data empty

protected:
	class   CAxPattern* m_pPattern;
	BOOL	m_patternUP, m_patternDN;
	HBITMAP m_bmpPatternBearUp, m_bmpPatternBearDn, m_bmpPatternBullUp, m_bmpPatternBullDn;
	int		m_bmWidth, m_bmHeight;
	CToolTip* m_patternToolTip;
	CString m_prevPatternDate;

	void	SetPattern(BOOL patternStatus, bool bUP);
	void	DrawPattern(CDC* dc, CIndcBase* indc);
	void	ClearPattern();
	void	MakePattern(CIndcBase* indc);
	void	DeletePatternToolTip();
	void	OnMouseMoveForPattern(CPoint pt);
	void	PatternReset();

	int		m_analStart, m_analEnd;
	BOOL	m_analMode, m_mouseDown, m_bmpMode;

	CWnd*	m_analNotifyMessageHandler;
	UINT	m_analNotifyMessageID;

	void	OnLButtonDownAnal(CPoint pt);
	void	OnLButtonUpAnal(CPoint pt);
	void	OnMouseMoveAnal(CPoint pt);
	void	RemoveAnal();
	void	CreateAnal();
	BOOL	DrawAnalArea(CDC* dc);
public:
	LRESULT AnalMessageProc(WORD msgKind, LPARAM lParam);
	friend class CPnChart;
};

#endif // !defined(AFX_OBJMGR_H__ECB3AE34_6673_4BA1_BF69_9B69D5103D9D__INCLUDED_)
