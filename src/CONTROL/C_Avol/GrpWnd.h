#pragma once
// GrpWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd window

#include <afxtempl.h>
#include <afxmt.h>

#define	maxCnt	8

struct _tick
{
	bool	bDraw{};
	int	lcolor{};
	int	lstyle{};
	int	lwidth{};
	CRect	tkRect;
};

struct _RgnInfo
{
	int	tkHeight{};

	_tick	tick[3]{};	// left/right/bottom
	CRect	gpRect;		// graph rect
};

#define PT_2D			2
#define PT_3D			3


class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd *pMain, CWnd *pParent, struct _param *pInfo);

	//{{AFX_VIRTUAL(CGrpWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGrpWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	Resize();
	struct _MinMaxChk	*GetMinMaxChk(int rKey);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	SetProperty(CString str);
	void	Clear();

public:
	std::array<std::unique_ptr<struct _RgnInfo>, maxCnt> m_pRgnInfo;
	std::array<std::unique_ptr<class CGrp_Data>, maxCnt> m_pDataInfo;
	std::array<std::unique_ptr<class CGrp_Base>, maxCnt> m_pGrpInfo;
	int		m_RgnCnt; 
	int		m_DataCnt;
	int		m_GrpCnt;
	CString		m_sParam;

	std::array<std::unique_ptr<struct _MinMaxChk>, maxCnt>	     m_pMinMaxChk;
	int	m_MinMaxCnt;

	int		m_totalDay;
	int		m_dispPos;
	int		m_dispEnd;
	int		m_dispDay;

	int		m_digit;
	int		m_dKind;	// param option
	int		m_dIndex;	// param option
	int		m_dUnit;	// param option
	int		m_dDay;		// param option
	int		m_dCount;	// param option
	int		m_jChart;	// param option
	bool		m_bOutLine;	// param option
	
	CFont		*m_pFont;
	COLORREF	m_ltColor;	// left tick color
	COLORREF	m_rtColor;	// right tick color
	COLORREF	m_btColor;	// bottom tick color
	
	CString		m_siga;
	CString		m_koga;
	CString		m_jega;
	CString		m_jgga;
	CString		m_gvol;		// 거래량, 체결량
	CString		m_ctim;
	int		m_poleType;
	char		m_szDate[8];
	int		m_nPoll;
	bool		m_bCtrl;
	
	CRect	m_ObjRect;
	int	m_fPoint;
	CString	m_fName;
	
private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(CString sRTM);
	void	RealTimeECN(CString sRTM);
	bool	AssignRegion();
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
	CWnd	*m_pView;
	CWnd	*m_pParent;
	std::unique_ptr<class CToolTip>	m_pToolTip;
	std::unique_ptr<class CCrossLine> m_pCrossLine;
	
	CString	m_sHeader;

	
	CRect	m_GrpRect;
	int	m_TickView;

	CString	m_PosData;
	UINT	m_timerID;

	COLORREF	m_fRGB;	// Fixed Graph Tick Color
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	CCriticalSection	m_cs;
	CC_AVolApp*		m_pApp;
};
