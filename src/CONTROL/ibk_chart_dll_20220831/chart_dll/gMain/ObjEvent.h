// ObjEvent.h: interface for the CObjEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJEVENT_H__D774D7DF_912F_4787_9FFD_6C6C0DA2FC88__INCLUDED_)
#define AFX_OBJEVENT_H__D774D7DF_912F_4787_9FFD_6C6C0DA2FC88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum class enumPointWhat {pointNONE = 0, pointGRAPH, pointTICK};

#define	GOS_msLbtnDN		0x00000001
#define GOS_msLbtnMV		0x00000002
#define	GOS_msRbtnDN		0x00000004

#define	GOS_toolState		(GOS_toolCreate|GOS_toolMove|GOS_toolChange)
#define	GOS_toolCreate		0x00000010		// tool create
#define	GOS_toolMove		0x00000020		// tool move
#define	GOS_toolChange		0x00000040		// tool change

#define	GOS_dragState		(GOS_dragObject|GOS_dragSplit|GOS_dragRegion)
#define	GOS_dragObject		0x00000100		// drag graph
#define	GOS_dragSplit		0x00000200		// drag split
#define	GOS_dragRegion		0x00000400		// drag region

#define	GOS_capture		0x00001000		// mouse capture
#define	GOS_autoScroll		0x00002000		// auto scroll

#define	GOM_overState		(GOM_overObject|GOM_overSplit|GOM_overToolH|GOM_overToolB|GOM_overRegion|GOM_overDelete)
#define	GOM_overObject		0x00010000		// mouse Object(tool, graph) over
#define	GOM_overSplit		0x00020000		// mouse split over
#define	GOM_overToolH		0x00040000		// mouse tool Hold over
#define	GOM_overToolB		0x00080000		// mouse tool Body over
#define	GOM_overRegion		0x00100000		// region move over
#define	GOM_overDelete		0x00200000		// region delete over

class CObjEvent  
{
public:
	CObjEvent(CWnd *pWnd, class CObjMgr *pObjMgr);
	virtual ~CObjEvent();


public:
	bool	m_bMouseCaptureTCreate;
	bool	m_bMapChart;
	bool	m_bSelctEtc;
	
	DWORD	m_dwObjStatus;			// object status

	UINT	m_nIimerID;

	CString	m_strPosData;
	// 2006.11.20 lsh (start)
	CString	m_strClickDateTime;
	CString	m_strClickPrice;
	bool	m_bOrderMode;			// SE801200
	// 2006.11.20 lsh (end)
	
	class CIndcBase*	m_pindcSelect;		// selected indicator
	class CToolTip*		m_pToolTip;
	
	struct _xDisplay 
	{
		int	start;
		int	end;
	};

	CArray	<class CToolBase*, class CToolBase*> m_arToolQue;
private:
	bool	m_bMouseRelease;
	bool	m_bShowTip;
	bool	m_bGrpSelect;
	

	char	m_acBookMark[MAX_UGRP];

	int	m_iTkChoiced;			// select tool kind - tkWHAT
	int	m_iToolSelectIndex;
	int	m_iLeftGap;
	int	m_iRightGap;
	int	m_iSplitTopID;
	int	m_iSplitBtmID;
	int	m_iCaptureSelectTool;
	int	m_iBMCnt;
	int	m_iDragRgnKey;

	DWORD	m_dwObjOpr;			// current Object operation


	CWnd*	m_pParent;	
	
	CRect	m_rectSplitCur;
	CRect	m_rectSplitPre;
	CRect	m_rectDrag;

	CPoint	m_pointStartSize;		// mouse drag, start
	CPoint	m_pointEndSize;			// mouse drag, end
	CPoint	m_pointDown;			// 추세선에서 사용

	CMenu	m_menuType;
	CMenu	m_menuKind;
	CMenu	m_amenuAid[8];
	
	class CObjMgr	*m_pObjMgr;
	class CIndcBase*	m_pTipIndc;

	CArray	<struct _xDisplay *, struct _xDisplay *> m_arDspQue;	// mouse display que
	CMap	<int, int, struct _sRegion *, struct _sRegion *> m_mapSplit;	// split Array
	CMap	<int, int, struct _mRegion *, struct _mRegion *> m_mapMark;	// delete mark Array

public:
	CPoint	GetStartPoint() { return m_pointStartSize; };
	void	Init();
	void	LButtonDownEvent(CPoint point);
	void	LButtonUpEvent(CPoint pt);
	void	RButtonDownEvent(CPoint pt);
	void	RButtonUpEvent(CPoint point);
	void	MouseMoveEvent(CPoint point, int iValid);
	void	DoubleClickEvent(CPoint point);
	void	MouseWheelEvent(bool bLeft);
	void	KeyboardEvent(int key);
	void	FocusEvent(bool bfocus);
	bool	ToolSelect(int select);
	void	ToolColor(int rgb);
	void	ToolWidth(BYTE btWidth);
	int	SearchPointInDragRegion(CPoint pt, CRect &DragRCm, bool bMouseUP = false);
	bool	SetStateCursor(CRect *pRect = NULL);
	void	DrawMouseArea(CPoint point, bool bMid = false, CDC* pDC = NULL);
	void	DrawSelectArea(CDC* pDC);
	void	ToolHideHolder();
	void	AppendChartMenu(CMenu *baseMENU, bool bInner = true );
	
	void	SetBookMark(char *ulist);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	ViewGrid(bool bShow = true, CPoint pt = CPoint(-1,-1));
	CString	GetTipStr(CPoint pt);
	bool	IsMapChart()	{ return m_bMapChart; }
	void	SetViewTooltip();
	void	SetViewTooltip(bool bSet);

	// SplitMap
	void	AddSplitMap();
	void	DrawSplitLine(CDC *pDC);
	

	// MarkMap
	void	AddMarkMap();
	void	DrawDeleteMark(CDC *pDC);

	void	DrawTopLine(CDC *pDC);
	void	RemoveAndrewPichfork();

	// Menu
	void	ProcessChartMenu(int menuID, int aKind);
	bool	ProcessSmallMenu(CPoint point);

	bool	YShowGraph(CPoint point);


private:
	// SplitMap
	void	DrawSplit();
	void	GetCrossValue(CToolBase* pToolBase, CPoint pt);
	int	GetPosition(int Pos, CPoint pt = CPoint(-1,-1), bool bBalance = false);
	CRect	GetSplitMoveRect(int rKey);
	int	IsSplitPos(CPoint pt, CRect& rRC);
	void	RemoveSplitMap();

	// MarkMap
	int	IsMarkPos(CPoint point);
	void	RemoveMarkMap();
	void	RemoveMarkRegion(int iDelKey);

	// Drag Chart
	void	MoveGraph(CPoint point);
	void	MoveGraphToNewRegion(CPoint point);

	void	SwapRegion(int RgnKey1, int RgnKey2);
	int	DisplayModifyToolMenu();
	int	DisplayChartMenu(int& aKind, CPoint pt);
	void	SaveCurrentPos(int start = -1, int end = -1);
	bool	ResetStateCursor();
	void	RemoveToolQue();
	void	RemoveLastToolQue();
	void	RemoveDisplayStack();
	void	ReAssignRegion(CPoint pt);
	void	PushDisplayStack(int start, int end);
	
	int	IsPtOverTool(CPoint pt);
	void	MouseResizeArea(CPoint pt, bool bEnd = false);
	bool	PopDisplayStack(int &start, int &end);
	bool	SearchPtInTool(CPoint pt);
	void	SelectTable(CPoint pt);
	CPoint	SetDisplayPosData(CPoint point, bool bOnlyCross = false);
	bool	SetOverCursor();
	void	ToolBackGround(int rgb);
//	void	DrawCrossLine(CPoint point, CDC* pDC = NULL);

	enumPointWhat	SearchPtInRegion(CPoint point);	// enumPointWhat

	class CIndcBase* SearchPtInGraph(CPoint point);
	class CToolBase* AddToolQue(int toolKind, class CIndcBase *pIndcBase);
	class CIndcBase* IsPtOverLine(CPoint pt, bool bSet, bool &bLabel);
	class CIndcBase* SearchPtInTick(CPoint point);

//** macho add
private:
	BOOL	IsPanelShow(UINT pnKind);
};

#endif // !defined(AFX_OBJEVENT_H__D774D7DF_912F_4787_9FFD_6C6C0DA2FC88__INCLUDED_)
