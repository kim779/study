#pragma once
// MGrpWnd.h : header file
//

struct _MChartCode{
	int		ctype;	// 선물 1, 콜 2, 풋 3
	int		atmgap;	// atm과의 차이
	int		Num;	// 수량
	int		mmgb;	// 매매구분 (-1. 매도, +1. 매수)


	double		ySum;	// 가격지점의 종목들의 y값 합계
};

/////////////////////////////////////////////////////////////////////////////
// CMGrpWnd window

class CMGrpWnd : public CWnd
{
// Construction
public:
	CMGrpWnd(CWnd* pView, CWnd* parent);

// Attributes
public:
	CIB410100App*	m_pApp{};
	CWnd*			m_pParent{};
	CWnd*			m_pView{};
	COLORREF		m_bkColor{};

	std::unique_ptr<CBitmap>	m_bmpChart{};
	int	m_nStgCode{};
	//CArray <struct _MChartCode, struct _MChartCode>		m_chartCode;

	bool		m_bRedraw{};

// Operations
public:
	void	drawChart(CDC *dc, CRect rcClient);
	void	Redraw();
	int	GetPerentX(CRect rcDraw, double perX);
	int	GetPerentY(CRect rcDraw, double perY);
	void	DrawZero(CDC *dc, CRect rcDraw, int zeroY, int x, bool upText = true, CString text = "ATM");


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMGrpWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMGrpWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMGrpWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

