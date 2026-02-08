#pragma once

#include <afxmt.h>
#include <afxtempl.h>
#include "grpBase.h"
#include "../../h/graph.h"
#include "../../h/common.h"

const int dispDAY = 140;		// basic display day
const int nTICK = 3;

const int chkMAGIC = 0;
const int maxMAGIC = 1;
const int minMAGIC = 2;

const int keyAPPEND = maxGRAPH * 2;
const int unusedKEY = -999;



const int splitHEIGHT = 4;

enum	pointWHO {pointNONE = 0, pointGRAPH, pointTICK};

#define	GOS_MSLBTNDN		0x0001
#define	GOS_MSRBTNDN		0x0002
#define GOS_MSMOVE		0x0004

#define	GOS_MODIFY		(GOS_TCREATE|GOS_TMOVE|GOS_TCHANGE)
#define	GOS_TCREATE		0x0010		// tool create
#define	GOS_TMOVE		0x0020		// tool move
#define	GOS_TCHANGE		0x0040		// tool change

#define	GOS_DURATION		(GOS_TENDENCY|GOS_USRINPUT)
#define	GOS_TENDENCY		0x0100		// tendency draw mode
#define	GOS_CROSSLINE		0x0200		// cross line
#define	GOS_USRINPUT		0x0400		// user input dialog

#define	GOS_SPLITOVER		0x1000		// mouse split over
#define	GOS_SPLITMOVE		0x2000		// mouse split drag
#define	GOS_CAPTURE		0x4000		// mouse capture
#define	GOS_AUTOSCROLL		0x8000		// auto scroll

#define	GOL_LINEOVER		0x0001		// mouse line over

#define	FOR_NO			0x00
#define	FOR_POS			0x01		// foreign possession
#define	FOR_TRD			0x02		// foreign trade

struct	_xTick {
	CRect	xRect;				// current rect
	CRect	sRect;				// save rect
	unsigned char xScale;
	DWORD	xColor;
};

struct	_xRegion {
	int	orgRgnKey;			// orginal region key
	CRect	xRect;				// current rect
	CRect	sRect;				// save rect
};

const	int	szHOLDER = 5;
const	int	szREGION = 8;

enum	tkWHAT {tkNONE = 0,
		tkTENDENCY, tkCROSSLINE, tkUSRINPUT, 
		tkFIBOLINE, tkFIBOFAN, tkFIBOARC, tkFIBORET,
		tkGANNLINE, tkGANNFAN,
		tkACCELFAN, tkACCELARC,
		tkTIRONE,   tkQUADRANT, tkCROSS,
		tkGRPCONFIG};

enum	chgUPDN {chgNO = 0, chgUP, chgDN};


class CGrpObject  
{
public:
	CGrpObject(class CGraphMgr* mgr, char* info);
	virtual ~CGrpObject();

public:
	char*	GetGrpHeader(int& hLen);
	bool	DispatchGraphData(char* gData, int& gDataL);

	void	RemoveGrpComponent(bool bClearAll = true);
	void	ObjectClear();

	char*	GetGrpHeader(int *hLen);
	bool	DispatchDup(char* gData, int& gDataL);
	void	RecalculateMagic();
	void	RecalculateRTMMagic(class CGrpBase *pGrpBase);
	void	SetSplitRgn();
	void	SetGraphError(int no);

	void	AddGraphRegion(char* data);
	void	RemoveGraphRegion();
	bool	RemoveGraphRegion(int RgnKey);

	bool	AddGraphData(int index, int ndat, char* gData, int& gDataL);
	void	RemoveGraphData();

	class CGrpBase*	AddGraphQue(char* data, bool bAppend = false, bool bScrollRgn = false);
	void	RemoveGraphQue();
	bool	RemoveGraphQue(class CGrpBase* pBase);
	bool	RemoveGraphQue(int rgnKey);
	
	class CToolBase* AddToolQue(int toolKind, class CGrpBase *pGrpBase);
	void	RemoveToolQue();
	void	RemoveLastToolQue();
	

	int	GetCountAddGraph(int rgnKey);
	int	GetCountRgnGraph(int rgnKey);
	int	GetCountRgnGraph(int rgnKey, int& idx);
	
	int	GetDataCount(int key);
	bool	GetGrpTick(int id, CRect& rect, int& scale, int& color);
	bool	GetGrpData(int key, class CGrpData* &pGrpData);

	bool	GetBongDataKey(int& key);
	bool	GetGrpCommonData(class CGrpData* &pGrpData);
	bool	GetGrpVidxData(class CGrpData* &pGrpData);
	bool	GetGrpRegion(int key, CRect& rect);
	bool	GetGrpRegion(int key, struct _xRegion*& xRegion);
	int	GetGrpUnit() { return m_ObjUnit; }
	int	GetGrpIndex() { return m_ObjIndex; }

	bool	AppendGraphChart(class CGrpBase* pBase, int OrgRgnKey, int aType, int aKind);
	//bool	OverlapGraphChart(class CGrpBase* pBase, int aType, int aKind);
	bool	DeleteGraphChart(class CGrpBase* pBase);
	void	ModifyColor(class CGrpBase *pGrpBase);
	int	GetGraphKind();
	void	ChartChange(int kind);

	
	void	GraphConfig();
	void	GraphAppend();
	void	graphDup(CGrpBase* pBase, CPoint pt);
	void	ChartConfig(CGrpBase *pGrpBase);
	void	ChartDate(CGrpBase *pGrpBase);
	bool	ChangeDisplayScale(int& scale);
	bool	ChangeDisplayShift(int& shift);
	bool	ChangeDisplayPos(int dispPos, int dispEnd);
	bool	ChangeDisplay(int dispDay);

	//void	DispatchGrpToolMenu(int direct = 0);
	CWnd*	GetGraphWnd();
	int	GetCountDupBong(int rgn = 0);

	bool	IsPointInRect(CPoint pt);
	void	LButtonDownAction(CPoint pt);
	void	LButtonUpAction(CPoint pt);
	void	RButtonDownAction(CPoint pt);
	void	RButtonUpAction(CPoint pt);
	void	MouseMoveAction(CPoint pt);
	BOOL	WheelAction(CPoint pt, bool bLeft);

	CPoint	SetDisplayPosData(CPoint pt, int mEvent = 0);

	bool	IsAcceptRTM() { return m_bUpdateREAL; }
	void	RealTimeExMessage(CString rtm);
	bool	ParseRealtimeEx(CString rtm, struct _rtmEx *rtmEx);
	bool	ParseRealtimeECN(CString rtm, struct _rtmEx *rtmEx);
	bool	IsAlreadyGet(CString symbol, struct _rtmEx *rtmEx);
	void	FocusAction(bool bfocus);

	CSize	GetSizeAdjust();

	void	MoveObject(CRect rc);
	void	ResizeGraphObject(float hRatio, float vRatio, bool bUpdate, bool bForce = false, bool bSplit = false);
	void	DrawGraphObject(CDC *pDC);
	void	DrawGraphObject(CDC *pDc, CRect rc);
	void	DrawEmpty(CDC* pDC, CRect rc);
	void	ReDrawGraphObject();
	void	ReDrawRegion(CRect& rect);
	void	ReDrawRTMRegion(bool bForceErase=false);
	
	COLORREF GetPaletteRGB(int index);

	CRect	CalculateRect(CRect rect, float hRatio, float vRatio);

	int	ReadUserData(CString& data);
	int	GetGrpCount() { return m_GraphQue.GetUpperBound(); }
	void	ReAssignRgn(class CGrpBase *pDnBase, class CGrpBase *pUpBase, int vShift);
	void	SaveRegion();
	class CGrpBase*	GetBottomAdjustGrp(struct _xRegion* pPivotRegion);

	void	SetGrpEnv(bool bReset = false);
	bool	GetObjMod()	{ return (m_ObjEnv & ENV_MOD)? true:false; }
	WORD	GetObjEnv() { return m_ObjEnv; }
	void	SetObjMod(bool bSet);
	bool	SetObjBong(bool bBong);
	void	SetObjLog(bool bSet);
	void	SetObjDup();
	bool	SetObjType(int  type);
	void	SetLineType();
	void	SetNewData(char *NewData);
	bool	IsObjEditable();
	bool	IsCrossVisible();
	bool	IsDrawFrameLine()	{ return !(m_DevOption & DF_NOFRAME); }
	bool	IsDrawMinMax()		{ return !(m_DevOption & DF_NOMINMAX); }
	COLORREF	GetUserInputColor();

	void	OnKey(int key);
	
	CGrpBase*	GetGrpBase();
	int	GetDisplayDay();
	int	GetTotalDataCount();
	void	ChangeFGraph(char* buf, bool bClear = false);
	void	ToolSelect(int select);
	bool	IsAddable();
	void	ViewTip(BOOL bShow, CString tipS = _T(""));

	void	GetDataGap();
	void	SetUserDefined(CGrpBase *pGrpBase);
	void	GetBongMinMax(int regionID, double& dMin, double& dMax);

private:
	bool	IsValidGraphHeader(int key, int region);
	void	CopyAndSpaceFilter(char* des, char* src, char srcL, bool bFilter = false);
	void	AddGraphTick(char* data, int bPos);
	void	SetMagicValue(int mapKey, class CGrpData* pGrpData, int gkind);
	pointWHO	SearchPointInRegion(CPoint pt);
	class CGrpBase*	SearchPointInGraph(CPoint pt);
	class CGrpBase*	IsPtOverLine(CPoint pt, bool bSet);
	int	DisplayAddToolMenu();
	int	DisplayModifyToolMenu();
	bool	CheckGraphTypeAndKind(CPoint pt, int &OrgRgnKey);
	void	SyncGraphDisplay(class CGrpBase* graphS, class CGrpBase* graphD);
	bool	SearchPointInTool(CPoint pt);
	int	DisplayAddChartMenu(class CGrpBase *pGrpBase, int& aType, int& aKind, CPoint pt);

	class	CGrpBase*	GetMainBaseGraph();
	class	CGrpBase*	ExistBongTypeGraph();
	class	CRgnSplit*	IsSplitRgn(CPoint pt);

	bool	DurationCursorModify(bool bArrow = false);
	bool	OnetimeCursorModify(bool bArrow = false);
	void	SplitCursor(CRect *pRc = NULL, bool bLineOver = false, bool bLineDrag = false);
	bool	ResetCursorModify();
	bool	ExistDupBong(int rgn = 0);
	
	class CGrpBase*	GetTopAdjustGrp(struct _xRegion* pPivotRegion);
	CPoint	GetAdjustPoint(CPoint pt);

	bool	IsJTrace ();
	bool	IsDataVisible();
	DWORD	GetOperation();
	void	SaveMouseOption();
	void	SetAppendMode(bool bAppend);
	void	SetReverseMode();
	void	SetJTrace();
	void	SetDataVisible();
	void	SetCrossView();
	void	SetGradient();
	void	SetMinMax();
	
	int	GetUnusedRegionKey();
	void	SetBongOption(DWORD UpColor, DWORD DnColor, DWORD option);

	void	mouseResizeArea(CPoint pt, bool bEnd = false);
	void	drawMouseArea(CPoint pt, bool bMid = false);
	int	getPosition(int Pos, CPoint pt = CPoint(-1,-1), bool bBalance = false);

	void	setDisplayGraph(int start, int end);
	void	saveCurrentPos(int start = -1, int end = -1);
	bool	popDisplayStack(int &start, int &end);
	void	pushDisplayStack(int start, int end);
	void	removeDisplayStack();

	void	toolHideHolder();
	void	copyGraphChart(CPoint pt);
	void	changeObject(struct _fgraph fgraph, bool bChange = true);

	void	deleteDupChart(int rgnKey, bool bAllClear = false);
	void	dupChartList(int delRgnKey, char* buf);
	void	setDupGraph();
	void	getCrossValue(CToolBase* pToolBase, CPoint pt);
	bool	checkCmpGraph(struct _dupInfo orgInfo, CGrpBase* cmpBase);
public:
	class	CDataFormat*	m_DataFormat;
	class	CAxisGraphApp*	m_pGrpApp;
	class	CGrpGrid*	m_pGrid;
	class	CGraphMgr*	m_pMgr;

	CWnd*	m_pGraph;

	int	m_dayDisp;			// Display day
	int	m_inputDisp;			// inputDisp in Config Dialog
	int	m_tkGap;			// tick gap

	CString	m_fName;			// Font Name
	float	m_fWidth;			// Font Width
	float	m_fHeight;			// Font Height
	float	m_fPoint;			// Font Point
	int	m_fStyle;			// Font Style

	bool		m_transparent;		// Transparent background
	COLORREF	m_tRGB;			// Text RGB(text)
	COLORREF	m_pRGB;			// Paint RGB(background)
	COLORREF	m_aRGB;			// Grid RGB(grid)

	float	m_hRatio;			// Resize ratio
	float	m_vRatio;

	CString	m_ObjName;			// Object Name
	CRect	m_ObjRect;			// Object Rect (Map Rect)
	CRect	m_ObjRegion;			// Object Draw Region
	CRect	m_GrpRect;			// Graph Rect 
	CRect	m_GrpRegion;			// Draw Region

	static CRect m_saveRIGHT;		// legend rect
	static CRect m_saveLEFT;	

	double	m_magicVAL[magicMAX][3];	// validity, max,min, value : DK_VAL, DK_VIDX

	class CCrossLine* m_pCrossLine;		// Cross Line
	class CGrpBase*	m_pSelLineGrp;		// Selected line GrpBase

	WORD	m_ObjStatus;			// object status
	unsigned char	m_DevOption;		// developer option
	bool	m_bNoMargin;			// top, bottom no margin (only bong)
	bool	m_bEditable;			// User Editable graph
	bool	m_bReverseMode;			// reverse mode
	bool	m_bNoSplit;
	unsigned char	m_bForeign;

	bool	m_bGradient;			// gradient effect
	bool	m_bUpFill;
	bool	m_bDnFill;
	int	m_UpColor;
	int	m_DnColor;
	bool	m_bSolidAidLine;

	bool	m_bEveryUpdate;			// everytime tick update
	int	m_rGap;
	int	m_lGap;
	bool	m_bTickChart;
	bool	m_bViewCurr;
	int	m_lMargin;
	int	m_rMargin;

	int	m_nTotalCnt;
	double	*m_pVal[maxAVLINE];
	bool	m_bStack;
	bool	m_bDrawTool;
	bool	m_bNMLine;

	CString	m_mgjySave;

private:
	bool	m_bFirst;
	CPoint	m_linePoint;
	bool	m_bToolChange;

	CMap	<int, int, struct _xRegion*, struct _xRegion*> m_MapRegion;	// key is _region.key
	CMap	<int, int, class CGrpData*, class CGrpData*> m_MapData;		// key is _dataH.key

	CArray	<class CGrpBase *, class CGrpBase *> m_GraphQue;	// Graph Array
	CArray	<class CToolBase *, class CToolBase *> m_ToolQue;	// Tool Array
	CArray	<class CToolBaseEx *, class CToolBaseEx *> m_ToolQueEx;	// Tool Array

	CCriticalSection	m_cs;
	bool	m_bAutoTran;
	CRect	m_mapRECT;			// map rect
	int	m_ObjUnit;			// Graph Unit
	int	m_ObjIndex;			// Graph Index

	enum {xBOTTOM = 0, xLEFT = 1, xRIGHT = 2};
	struct	_xTick	m_xTick[nTICK];		// xtick, ytick, ytick2
	bool	m_bUpdateREAL;			// RealData Update
	bool	m_bRefresh;			// forcely refresh
	bool	m_bECNData;			// ECN Data
	
	bool	m_bAppendMode;			// 추가 모드
	pointWHO m_mouseOWNER;			// last mouse action owner

	tkWHAT	m_tkCHOICED;			// select tool kind
	int	m_tkSELECT;			// save select toolbase
	int	m_tkSAVE;			// last select toolbase

	class CRgnSplit	*m_DragRgnSplit;
	int	m_BongDataKey;
	WORD	m_ObjEnv;			// external environment
	
	
	WORD	m_ObjLine;			// current line over
	CString	m_timeRTM;			// 체결시간 or 호가시간
	CString	m_currRTM;			// 종가
	CString	m_gvolRTM;			// 거래량
	CString	m_gamtRTM;			// 거래대금
	CString	m_mgjyRTM;			// 미결제약정
	

	CPoint	m_lastPt;			// 드래그시 마지막 마우스좌표
	CRect	m_RealGrpRgn;			// original drawing region
	bool	m_bJTrace;			// 종가 crossline view
	bool	m_bDataVisible;			// user data view option
	bool	m_bCrossVisable;		// Crossline View
	bool	m_bUsableGrid;			// usable data grid
	bool	m_bDrawMinMax;			// display min/max
	bool	m_bDataVisable;			// user data view option
	
	
	CPoint	m_SSizePt;
	CPoint	m_ESizePt;

	struct _xDisplay {
		int	start;
		int	end;
	};

	CArray	<struct _xDisplay *, struct _xDisplay *> m_DisplayQue;	// mouse display que

	bool	m_bLineSelect;

	struct _dup {
		int	kind;
		int	type;
	};

public:
	BOOL	m_bMouseCaptureTCreate;

};


