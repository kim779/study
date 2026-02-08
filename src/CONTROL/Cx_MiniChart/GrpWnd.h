#pragma once
// GrpWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd window
#include "MainWnd.h"

#include <afxmt.h>

struct _tkinfo
{
	CRect		tkRect[3];
	COLORREF	tkColor[3]{};
};



class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd* pView, CWnd* pMainWnd, char* mj, CParam* pParam);
//	CGrpWnd(CWnd *pParent, char *mj);
	void	TipMouseMove(CPoint point);
	void	MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly = false);
	
	void	SetParam(CParam *pParam);
	void	init();

	void	Send() { RequestData(m_TRIndex); }
// Attributes
	CString		m_title;
	CParam		m_param;

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
	long OnUser(WPARAM wParam, LPARAM lParam);



	// Generated message map functions
protected:
	//{{AFX_MSG(CGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGrpWnd)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()



private:
	int	DispatchData();
	void	RealTimeData(struct _alertR* data);
	bool	AssignGraph();
	void	RemoveComponent();

	void	DrawGraph(CDC *pDC);
	void	DrawEmpty(CDC *pDC);
	void	Resize();
	int	GetPalette(int index);
	long	RequestData(int index);
	int	GetHeaderInfo(int index, char *Header);
	int	GetDataInfo(int index, char *Header);
	int	GetUnit(char *code);
	void	MakeDataSymbol(int unit, CString &dataSymbol, CString &outSymbol);

	BOOL	IsPtInGrp();
	CString	GetTipStr(CPoint point);
	void	Config(CPoint point);
	


public:
	std::unique_ptr<class CGrpData> m_pGrpData;
	std::array<std::unique_ptr<class CGrpLine>, MAX_JISU>	m_pGrpInfo;
	int	m_GrpCnt;
	struct _minijisu	m_mj;

	//char* m_pRcv[MAX_JISU]{};
	std::array<std::unique_ptr<char[]>, MAX_JISU> m_pRcv;
	int   m_szRcv[MAX_JISU]{};
	int   m_pIndex[MAX_JISU]{};

	CFont* m_pFont{};
	int		m_fPoint{};
	CString		m_fName;
	CRect		m_ObjRect;
	CRect		m_GrpRect;
	_tkinfo		m_tkInfo;
	
	int		m_TickView{};

	double		m_Max{};
	double		m_Min{};
	CPoint		m_tkPoint;

	CString    m_strMarket = "KRX";
	void SetTrKey(int ival) { m_TRIndex = ival; }
private:
	class CCx_MiniChartApp* m_pApp{};
	CWnd* m_pView{};
	CWnd* m_pMainWnd{};

	std::unique_ptr<class CToolTip>   m_pToolTip;
	std::unique_ptr<class CCrossLine> m_pCrossLine;
	CString		m_PosData;
	int		m_timerID{};

	COLORREF	m_fRGB{};	// Fixed Graph Tick Color
	COLORREF	m_tRGB{};
	COLORREF	m_pRGB{};


	int		m_TRIndex{};
	CRect		m_btnRect;
	CBitmap* m_pBitmap{};
};