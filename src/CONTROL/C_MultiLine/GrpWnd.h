#pragma once

#include <afxtempl.h>
#include <afxmt.h>

#define	maxCnt	8

struct _tick
{
	bool	bDraw;
	int	lcolor;
	int	lstyle;
	int	lwidth;
	CRect	tkRect;
};

struct _RgnInfo
{
	int	tkHeight;
	_tick	tick[3];	// left/right/bottom
	CRect	gpRect;		// graph rect
	int	lcnt;
	int	digit;
	int	start;
	char	rcod[5][6];
};

#define RATE_1_1	0
#define RATE_2_1	1
#define RATE_3_2	2

class CGrpWnd : public CWnd
{
public:
	CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo);
	virtual ~CGrpWnd();
	
	//{{AFX_VIRTUAL(CGrpWnd)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	Resize();
	struct _MinMaxChk*	GetMinMaxChk(int rKey);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	SetProperty(CString str);
	void	Clear();
public:
	class CGrp_Data*	m_pDataInfo;
	struct _RgnInfo*	m_pRgnInfo[maxCnt];	
	class CGrp_Base*	m_pGrpInfo[maxCnt];
	int			m_RgnCnt; 
	int			m_DataCnt;
	int			m_GrpCnt;
	CString			m_sParam;
	struct _MinMaxChk*	m_pMinMaxChk;
	int			m_MinMaxCnt;
	int			m_totalDay;
	int			m_dispPos;
	int			m_dispEnd;
	int			m_dispDay;	
	int			m_lcnt;
	int			m_dKind;	// param option
	int			m_dIndex;	// param option
	int			m_dUnit;	// param option
	int			m_dDay;		// param option
	int			m_dCount;	// param option
	char			m_szDate[8];	// param option
	bool			m_bPivot;	// param option
	CFont*			m_pFont;
	COLORREF		m_ltColor;	// left tick color
	COLORREF		m_rtColor;	// right tick color
	COLORREF		m_btColor;	// bottom tick color	
	CString			m_ctim;
	CStringArray		m_arTitle;
	CStringArray		m_arDegree;
	int			m_rgnRate;	
	int			m_fPoint;
	CString			m_fName;
	CRect			m_ObjRect;
	bool			m_bSign;
	bool			m_bCtrl;
	
	
private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(char* sRTM);
	bool	AssignGraph();
	void	DrawEmpty(CDC *pDC, bool bInit);
	void	ParseParam(char *param);
	int	RequestHead();
	void	RemoveComponent();
	int	GetPalette(int index);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);

	bool	ChangeDisplayScale(int scale);
	bool	ChangeDisplay(int dispDay);
	bool	ChangeDisplayShift(int shift);
	void	ExcelProc();
	void	RecalculateMinMax();
	void	ReviseTick();
	

private:
	CWnd*			m_pView;
	CWnd*			m_pParent;
	class CToolTip*		m_pToolTip;
	class CCrossLine*	m_pCrossLine;
	CString			m_sHeader;
	CRect			m_GrpRect;
	int			m_TickView;
	CString			m_PosData;
	UINT			m_timerID;	
	COLORREF		m_fRGB;	// Fixed Graph Tick Color
	COLORREF		m_tRGB;
	COLORREF		m_pRGB;
	CCriticalSection	m_cs;
	CC_MultiLineApp*	m_pApp;
};
