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

class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd* pView, struct _param* pInfo);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpWnd)
	public:
	virtual void OnFinalRelease();
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGrpWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void _SetTitle(LPCTSTR title);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	void	DrawGraph(CDC *pDC);
	void	DrawTitle(CDC *pDC);
	void	Resize();
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
public:
	std::unique_ptr<struct _RgnInfo> m_pGrpRgn{};
	std::unique_ptr<class CGrp_Data> m_pGrpData{};
	std::unique_ptr<class CGrp_Tick> m_pGrpTick{};

	int		m_totalDay{};
	int		m_dispPos{};
	int		m_dispEnd{};
	int		m_dispDay{};

	int		m_digit{};
	int		m_dKind{};	// param option
	int		m_dIndex{};	// param option
	int		m_dUnit{};	// param option
	int		m_dDay{};		// param option
	int		m_dCount{};	// param option
	bool		m_bExpand{};	// param option
	bool		m_bOutLine{};	// param option
	
	CFont		*m_pFont{};
	COLORREF	m_ltColor{};	// left tick color
	COLORREF	m_rtColor{};	// right tick color
	COLORREF	m_btColor{};	// bottom tick color
	
	CString		m_jgga;
	CString		m_cvol;		// 체결량
	CString		m_diff;
	CString		m_udyl;
	CString		m_ctim;
	
	int		m_pJgga{};	// 전일가
	int		m_uJgga{};	// 상한가
	int		m_dJgga{};	// 하한가

	CString		m_title;

private:
	int	DispatchData(WPARAM wParam, LPARAM lParam);
	void	RealTimeData(LPARAM lParam);
	bool	Alert(DWORD* data);
	bool	AssignRegion();
	bool	AssignGraph();
	void	DrawEmpty(CDC *pDC, bool bInit);
	void	ParseParam(char *param);
	int	RequestHead();
	void	RemoveComponent();
	int	GetPalette(int index);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);
	
private:
	CWnd		*m_pView{};
	CString		m_sParam;
	std::unique_ptr<class CToolTip>		m_pToolTip;
	std::unique_ptr<class CCrossLine>	m_pCrossLine;
	int	m_fPoint{};
	CString	m_fName;
	CString	m_sHeader;

	CRect	m_ObjRect;
	CRect	m_GrpRect;
	int	m_TickView{};

	CString	m_PosData;
	UINT	m_timerID{};

	COLORREF	m_fRGB{};	// Fixed Graph Tick Color
	COLORREF	m_tRGB{};
	COLORREF	m_pRGB{};
	
	CC_TickApp		*m_pApp{};
};
