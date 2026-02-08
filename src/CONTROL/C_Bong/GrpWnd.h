#if !defined(AFX_GRPWND_H__CE1648E7_7460_4979_A6AA_DC9778CEA1F3__INCLUDED_)
#define AFX_GRPWND_H__CE1648E7_7460_4979_A6AA_DC9778CEA1F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrpWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd window

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
};


class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
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
	void	StartOrderSignal(CString sPrice);
	void	EndOrderSignal();
public:
	struct _RgnInfo	*m_pRgnInfo[maxCnt];
	class CGrp_Data	*m_pDataInfo[maxCnt];
	class CGrp_Base	*m_pGrpInfo[maxCnt];
	int		m_RgnCnt; 
	int		m_DataCnt;
	int		m_GrpCnt;
	CString		m_sParam;

	struct _MinMaxChk	*m_pMinMaxChk;
	int	m_MinMaxCnt;

	int		m_PMA[4];
	int		m_PMACnt;
	int		m_VMA[4];
	int		m_VMACnt;

	int		m_totalDay;
	int		m_dispPos;
	int		m_dispEnd;
	int		m_dispDay;

	int		m_pivot;
	int		m_digit;
	double		m_power;
	int		m_dKind;	// param option
	int		m_dIndex;	// param option
	int		m_dUnit;	// param option
	int		m_dDay;		// param option
	int		m_dCount;	// param option
	int		m_jChart;	// param option
	bool		m_bVol;		// param option
	bool		m_bOutLine;	// param option
	bool		m_bSameRegion;	// param option
	int		m_VolColor;	// param option

	CFont		*m_pFont;
	int		m_fPoint;
	CString		m_fName;
	CRect		m_ObjRect;
	CRect		m_GrpRect;

	COLORREF	m_ltColor;	// left tick color
	COLORREF	m_rtColor;	// right tick color
	COLORREF	m_btColor;	// bottom tick color
	
	CString		m_siga;
	CString		m_koga;
	CString		m_jega;
	CString		m_jgga;
	CString		m_gvol;		// 거래량, 체결량
	CString		m_ctim;
	
private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(CString sRTM);
	bool	Alert(CString sRTM);
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
	class CToolTip	*m_pToolTip;
	class CCrossLine	*m_pCrossLine;
	CString	m_sHeader;

	int	m_TickView;

	CString	m_PosData;
	UINT	m_timerID;

	COLORREF	m_fRGB;	// Fixed Graph Tick Color
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	CCriticalSection	m_cs;
	bool			m_bCtrl;

	CC_BongApp		*m_pApp;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRPWND_H__CE1648E7_7460_4979_A6AA_DC9778CEA1F3__INCLUDED_)
