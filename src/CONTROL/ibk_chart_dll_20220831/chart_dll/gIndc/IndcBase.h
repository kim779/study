// IndcBase.h: interface for the CIndcBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDCBASE_H__1EEF3B5D_F940_48A3_AF0F_52814B5B97E2__INCLUDED_)
#define AFX_INDCBASE_H__1EEF3B5D_F940_48A3_AF0F_52814B5B97E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../h/axisgenv.h"
#include "../../h/axisgobj.h"
#include "../gData/GrpBasic.h"

#include <float.h>

#define	_L_MAGENTA	RGB(192,   0, 192)
#define	_L_WHITE	RGB(255, 255, 255)
#define	_UPCOLOR	RGB(255,   0,   0)
#define	_DNCOLOR	RGB(0  ,   0, 255)

const int minDisplayDay = 20;
const int gapTICK  = 1;
const int lenTICK  = 8;
const int dispUNIT = 1000000;		// display Tick (원, 천원)
const int dispMUNIT = 1000000000;	// dosplay Tick (원, 백만원)

//const int dispLINEMAX = 30;
#define	DISP_LINEMAX	30
const int rgnLINEGAP = 3;
const int rgnMARKGAP = 4;
//const int distanceGAP = 1;		// 봉간 Gap (left, right)
#define	DISTANCE_GAP	1		// 봉간 Gap (left, right)

struct _rgnInfo				// Mouse operation Rgn Infomation
{
	int	iCount;			// line Rgn Count, init:0
	int	iSelect;		// current select index, init:-1
	struct _Rgn
	{
		CRgn*	pRgn;		// graph line rgn
		int	iIndex;		// struct _line index[배열상의 색인]
		CRect*	pRect;		// graph legend Rect
	} Rgn[DISP_LINEMAX];
};
#define	SZ_RGNINFO	sizeof(struct _rgnInfo)

struct _tkInfo				// Tick Information
{
	CRect	tkRect[3];		// Drawing Rect
	int	tkHeight[3];		// Real font Height
};
#define	SZ_TKINFO	sizeof(struct _tkInfo)

struct _dispInfo {
	char	dispStr[10];
	CRect	dispRC;
	CPoint	dispPt;
};
enum { dispNONE, dispONEHOUR, dispTHREEHOUR, dispONEDAY, dispTHREEDAY, dispSIXDAY};

class AFX_EXT_CLASS CIndcBase  
{
public:
	CIndcBase();
	CIndcBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay);
	virtual ~CIndcBase();

public:

private:

protected:
	bool		m_bInit;

	int		m_iDispDtCnt;		// Drawing Data Count
	int		m_iDispSPos;		// Drawing Start Pos (data array)
	int		m_iDispEPos;		// Drawing End Pos (data array)
	int		m_iTtlDtCnt;		// Total Data Count

//	int		m_TickNum;
	int		m_iUTickNum;
	int		m_iValidDigit;		// 소수점 유효자릿수 ==> unit 무시

	int		m_iUnit;		// data unit
	int		m_iGrpKind;		// graph kind
	int		m_iDataKey;		// data Key

	double		m_dYMax;			// Y Axis MaxValue
	double		m_dYMin;			// Y Axis MinValue
	double		m_dXMax;			// X Axis MaxValue
	double		m_dXMin;			// X Axis MinValue

	WORD		m_wEnvOpt;		// global environment option
	DWORD		m_dwDspOpt;		// global display option

	COLORREF	m_crBG;			// global background
	COLORREF	m_crFG;			// global foreground
	COLORREF	m_crText;

	COLORREF	m_crUpSise;			// 
	COLORREF	m_crDnSise;			// 
	COLORREF	m_crEqSise;			// 


	CWnd*		m_pwndView;
	CWnd*		m_pwndParent;		// Parent CWnd

	CRect		m_rectObj;		// PnChart 전체				// Entire Object Rect
	CRect		m_rectChart;		// Tick 을 제외한 PnChart 전체		// Entire Graph Rect
	CRect		m_rectOrgGrp;		// 각각의 원래 Graph 영역		//m_rectOrgDraw;		// Graph Original Drawing Rect
	CRect		m_rectGrp;		// 왼쪽, 오른쪽 gap을 제외한 graph 영역 //m_rectDraw;		// Graph Drawing Rect

	CString		m_strLegend;		// 범례@@단위

	class CDataMgr*	m_pDataMgr;		// data Manager -> access by DataKey

	struct _graph*	m_pgraph;		// graph info
	struct _CoDraw*	m_pCoDraw;

	struct _line	m_lineH;		// horizon line
	struct _line	m_lineV;		// vertical line
	struct _line	m_lineDate;		// 일자변경선
	struct _tick	m_atick[3];		// x-tick info, bottom/left/right
	
	struct _tkInfo	m_TkInfo;
	struct _rgnInfo	m_RgnInfo;
public:
	void	ResizeGraph(CRect ObjRect, CRect GrpRect, CRect DrawRect, CRect TickRect[3], int lgap, int rgap, int BalanceGap = 0);
	void	ResizeGraph(CRect ObjRect, CRect GrpRect, CRect DrawRect, CRect TIckRect[3], int lgap, int rgap, int tgap, int bgap, int BalanceGap = 0);
	void	DrawCurPrice(CDC *pDC);
	void	DrawPrePrice(CDC *pDC);
	void	DrawExtPrice(CDC *pDC, bool bOrderMode);
	int	DrawTickIndex(CDC* pDC, CString dispText, bool bLeft);
	int	DrawTickIndex(CDC* pDC, CString strDispText, COLORREF rgbText, bool bColor, bool bIndex = false, CPoint point = CPoint(-1,-1));
	void	GetMinMaxValue(double& dMin, double& dMax) { dMin = m_dYMin; dMax = m_dYMax; }
	int	GetDataKey() { return m_iDataKey; }
	int	GetTotalDataCount() { return m_iTtlDtCnt; }
	int	GetDataIndex();
	int	GetDataUnit()	{ return m_iUnit; }
	int	GetGraphID();
	int	GetGraphType();
	int	GetGraphKind()	{ return m_iGrpKind; }
	int	GetGraphRegionID();
	CRect	GetDrawRect() { return m_rectGrp; }
	CRect	GetOrgDrawRect() { return m_rectOrgGrp; }
	int	GetDisplayDay() { return m_iDispDtCnt; }
	int	GetDisplayPos() { return m_iDispSPos; }
	int	GetDisplayEnd() { return m_iDispEPos; }
	void	SetDisplayDay(int day) { m_iDispDtCnt = day; }
	void	SetDisplayPos(int pos) { m_iDispSPos = pos; }
	void	SetDisplayEnd(int end) { m_iDispEPos = end; }
	bool	SetDisplay(int iDisDay);
	int	SetDisplayScale(int& scaleUpDn);
	bool	SetDisplayShift(int& shiftUpDn);
	void	SetGraphRegionID(int id);
	void	ChangeDisplayPos(int iDispPos, int iDispEnd);
	char*	GetGrpInfo() { return (char *)m_pgraph; }
	CString	GetExtraInfo(DWORD dwCheck, int val, bool bGrid = false);
	void	DisplayCheckBits(CDC* pDC, CPoint point, DWORD dwCheck, int val);
	bool	IsTickDraw(int which)	{ return (m_pgraph->dwDraOption & which)? true:false; } 
	CString GetIndexValueStr(CPoint pt, int &fpr, int &hpr, int &lpr, int &epr);
	void	ReviseMinMax(double& Max, double& Min);
	int	GetTickCount(int dir = 0);
	bool	IsPointInGrpRect(CPoint pt);			// check pt is in Graph Area
	int	IsPointInGrpRgn(CPoint pt, bool &bLabel, bool bSet = true);	// check pt is on Graph Rgn
	bool	IsRgnSelect();
	void	ResetSelRgn();
	void	GetValueToStr(double value, CString &str);
	void	GetValueToYPoint(CPoint &pt, double yValue);
	bool	IsUnion()	{ return ((m_pgraph->dwDraOption & GO_UNION) ? true : false); };	// 합성여부
	virtual void	DrawLegend(CDC* pDC) {}	// Legend (범례) 그리기
	bool	IsPointInTickRect(CPoint point);	////kangmc추세선변경
	int	IncreaseCount();
	CString	GetDisplayPosDateTime(CPoint pt); // 2006.11.20 lsh
	

	virtual int	GetDisplayPosIndex(CPoint pt) { return -1; }
	virtual CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true) { return _T(""); }
	virtual CString	GetDisplayPosData(CPoint pt, bool bGrid = true) { return _T(""); }
	virtual	CString	GetYposValue(CPoint pt, bool bGrid) { return _T(""); }
	virtual	CString	GetXposValue(CPoint pt) { return _T(""); }
	virtual	CPoint	GetAdjustPoint(CPoint pt, CString &sPosVal) {return CPoint(-1, -1); }
	virtual	CPoint	GetOrderPoint(CPoint pt, CString &sPosVal) {return CPoint(-1, -1); }
	virtual bool	CalculateMinMax(bool bForce = false) { return false; }
	virtual bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false) { return false; }
	virtual void	DrawGraph(CDC* pDC) { }
	virtual	void	DrawTickBox(CDC* pDC, COLORREF ef, COLORREF eb, COLORREF gf, COLORREF gb);
	virtual void	DrawTick(CDC* pDC) { };
	virtual void	DrawTickVertical(CDC *pDC);
	virtual CString GetTickUnit(double Max);
	virtual void SetGrpInfo(struct _graph *pGrp, struct _envInfo *pInfo, struct _CoDraw *pCoDraw);
	virtual	CString	ExcelData(int idx) { return ""; }
	virtual	int	GetValue() { return 0; }
	virtual void	FormatNormalComma(CString &dispTxt, double dVal, bool bComma, int idigit = -1);	// comma format widthout condition
	virtual void	FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit=10); // variable comma format 
////kangmc추세선변경
	virtual	void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	virtual	void	GetPtToPosValue(CPoint pt, double &pos, double &value);
	virtual	void	GetValueToPoint(CPoint& pt, double dXValue, double dYValue);
	virtual	CString GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr);
	virtual double	GetOneDataSize() { return 0;}
	virtual int	GetValueToXIdx(double dXValue) { return 0;}
	virtual int	IsNoTime() { return 0;}
	virtual void SetUserDefined(CString sVal) {}
	virtual void GetUsedValue(CString &sVal) {}
protected:
	void	EditMemberValue();
	void	CalculateTickInfo();
	bool	IsLogChart();
	bool	IsReverseChart();
	double	GetLogData(double val, bool bLog = true);
	CRect	DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt);

	CFont*	GetFont(int tkID, int weight = FW_NORMAL);
	CPen*	GetPen(int style, int width, COLORREF clr);
	CBrush*	GetBrush(COLORREF rColor);
	COLORREF GetColor(int index);
public:
	CRect	GetChartRect() const { return m_rectChart; }
};

#endif // !defined(AFX_INDCBASE_H__1EEF3B5D_F940_48A3_AF0F_52814B5B97E2__INCLUDED_)
