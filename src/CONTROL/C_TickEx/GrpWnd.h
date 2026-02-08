#pragma once
// GrpWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd window
#include <afxmt.h>
#include <afxtempl.h>
#define	maxCnt		2

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

class CSignal : public CObject
{
public:
	int m_nSig{};
	UINT m_nState{};
};

class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd* pView, CWnd* pParent, struct _param* pInfo);

	enum	{ sp_NO, sp_DN, sp_EQ, sp_UP};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGrpWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	Resize();
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	void	StartOrderSignal(CString sPrice, CString strKey);
	void	EndOrderSignal(CString strKey);
public:
	//struct _RgnInfo	*m_pGrpRgn[2];	
	std::array<std::unique_ptr<struct _RgnInfo>, 2>		m_pGrpRgn;
	std::array<std::unique_ptr<class CGrp_Tick>, 2>		m_pGrpTick;
	std::unique_ptr<class CGrp_Data>			m_pGrpData;

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
	bool		m_bExpand;	// param option
	bool		m_bMygl;	// param option
	
	CFont		*m_pFont;
	COLORREF	m_ltColor;	// left tick color
	COLORREF	m_rtColor;	// right tick color
	COLORREF	m_btColor;	// bottom tick color
	
	CString		m_jgga;
	CString		m_cvol;
	CString		m_ctim;

	std::map<CString, std::unique_ptr<CSignal>> _mapSig;

	
	int		m_pJgga;	// 전일가
	int		m_uJgga;	// 상한가
	int		m_dJgga;	// 하한가
	int		m_sJgga;	// signal 기준가
	int		m_sState;	// signal Position

private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(CString sRTM);
	void    RealTimeData(LPARAM lParam);
	bool	Alert(CString sCode, DWORD *data);	
	bool	AssignRegion();
	bool	AssignGraph();
	void	DrawEmpty(CDC *pDC, bool bInit);
	void	DrawFrameRect(CDC *pDC, CRect rc, COLORREF rgb);
	void	ParseParam(char *param);
	int	RequestHead();
	void	RemoveComponent();
	int	GetPalette(int index);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);

	bool	ChangeDisplayScale(int scale);
	bool	ChangeDisplay(int dispDay);
	bool	ChangeDisplayShift(int shift);
	void	RecalculateMinMax();
	void	ExcelProc();
	void	CheckFile();
	BOOL	userConfig();
	
private:
	CWnd		*m_pView;
	CWnd		*m_pParent;
	int		m_ParamKey;
	CString		m_sParam;
	CString		m_sParamName;
	std::unique_ptr <class CToolTip>	m_pToolTip;
	std::unique_ptr <class CCrossLine>	m_pCrossLine;
	int	m_fPoint;
	CString	m_fName;
	CString	m_sHeader;
	CString	m_FileName;

	CRect	m_ObjRect;
	CRect	m_GrpRect;
	int	m_TickView;

	CString	m_PosData;
	UINT	m_timerID;

	COLORREF	m_fRGB;	// Fixed Graph Tick Color
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;
	
	CCriticalSection	m_cs;
	CC_TickExApp		*m_pApp;
	struct _tickinfo	m_tickinfo;
};
