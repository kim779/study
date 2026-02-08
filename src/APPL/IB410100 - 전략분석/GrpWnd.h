#pragma once
// GrpWnd.h : header file
//

#include <Afxtempl.h>
#include "message.h"

#define BOX_NONE	0
#define BOX_LEFT	1
#define BOX_RIGHT	2
#define BOX_TOP		4
#define BOX_BOTTOM	8
#define BOX_ALL		(BOX_LEFT | BOX_RIGHT | BOX_TOP | BOX_BOTTOM)

#define	_KMAX(x,y)	(x >= y) ? x : y	//kangmc
#define	_KMIN(x,y)	(x <= y) ? x : y
        /* 선물이론가 산출      */
#define F_IRGA(k, r, b, t)  ((k / 100. * (1. + r / 100000. * t /*/ 365.*/) - b / 1000000.) * 100.)

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd window

class CGrpWnd : public CWnd
{
// Construction
public:
	CGrpWnd(CWnd* pView, CWnd* parent);

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
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg long OnMsgRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgAlert(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgAddData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg long OnMsgSetParam(WPARAM wparam, LPARAM lparam);
	afx_msg long OnBasicPrc(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	CIB410100App	*m_pApp;
	CWnd*	m_pParent;
	CWnd*	m_pView;
	
	struct _CalData	
	{
		double		xflt{};	//기울기
		double		sang{};	//상수
		double		fstX{};	//x시작
		double		secX{};	//x끝
		CString		cod1; // ??
		CString		cod2; // ??
	};
	CArray <_CalData, _CalData> m_CalData;
	
	CArray <_GraphData*, _GraphData *> m_GraphData;

	struct _Line	{
		double	xval{};
		double	yval{};
	};
	CArray <_Line, _Line> m_gLine[4][5];

	struct _standardX	{
		double	leftX{};
		double	rightX{};
	};
	_standardX	m_Standard[4]{};

	double		m_Maxjjis{};
	double		m_MinJjis{};			//kangmc
	int			m_PayOffStatus{};
	bool		m_MinGamStatus[6]{};

	double		m_yearDate{};
	double		m_cdkr{};
	double		m_k200{};
	double		m_k200Jisu{};			// 2006.12.15 cej
	double		m_Total{};
	
	CRect		m_rcGraph;

	COLORREF	m_gbColor{}, m_gColor{},
				m_gslColor{}, m_gL1Color{},
				m_gfColor{},
				m_gk2Color{}, m_gLzColor{};
	
	COLORREF	m_gMlColor[5]{};

	CFont*		m_ctlFont{};
	int			m_ctlFontSize{};

	struct	_cursor		{
		bool	btnFlag{};
		bool	moveFlag{};
		HCURSOR	cursor{};
		int	kind{};
		int	point{};		// xval 의 X idx
		int	startXpt{};
		int	cidx{};
	};
	_cursor		m_hCursor{};

	struct	_moveLine	{
		int	xpt{};
		int	ypt{};
	};
	CArray<_moveLine, _moveLine> m_moveLine;

	BOOL	m_bTracking{};
	bool	m_chkyetak{};
	bool	m_ironFlag{};

	std::unique_ptr<class CPopupWnd> m_pPopup{};
	int				m_chknum{};
	CString			m_names{};
	CString			m_delta{}, m_gamma{}, m_seta{}, m_vega{};
	bool			m_chkdelta{}, m_chkgamma{}, m_chkseta{}, m_chkvega{};
	std::unique_ptr<class CCrossLine> m_pCrossLine{};
	int			m_MouseValCnt{};

	CString	m_allmax, m_allmin;
	CString	m_Maxiik, m_Miniik;
	CString	m_bunkiValue[2][4];
	int	m_bunkiCount[2]{};

	double	m_jisuStart{}, m_jisuEnd{};
	CString m_rtnStr;
	double	m_jjilMove{};	// 잔존일수 조정
	BOOL	m_bMoveStg{};	// Mouse 전략이동 허용유무
	HWND	m_hTopHwnd{};	//2014.12.22 KSJ 화면탑이 다른창인지 

public:
	void	CalMangi(int flag, int mingam);
	void	CalIron(int flag, int mingam);
	void	DrawgLine(CDC* pDC, COLORREF gLColor, int flag, int mingam, double minY, double maxY);
	void	GetSonik(int flag, int mingam, int xpt);
	void	GetSonik2(int flag, int mingam, int xpt);
	bool	GetMinMaxY(int flag, int mingam, double &MinY, double &MaxY, int type);
	void	CalMinGam(int flag, int mingam);
	double	GetMinGamCal(int flag, int mingam, double ii);

	void	DrawYLine(CDC* pDC, int flag);
	CString	StringModify(double data, int mingam = 5);
	double	GetIronCal(int flag, double ii);
	void	DrawGraph(CDC* pDC);
	int	GetYpt(double ypt, double minY, double maxY);
	int	GetXpt(double xpt, double leftX, double rightX);

	double	SizeCompare(double data, double min, double max);
	void	GetCalMangi(int nSize, int ii, int jj);
	
	void	RMouseUpMsg(int xpt, int ypt);
	int	RoundDouble(double data);
	CString	GetMoveCode(int point);
	void	SetTrackMouse();
	void	BoldDrawLine(int xPoint, int kind);
	void	DrawMoveLine(int xPoint, int kind);
	void	LineCapture(int xPoint, int kind);
	int	GetLinePoint(int xpt);
	CString	GetMoveGab(int spt, int ept);
	bool	CheckMove(int xpt);
	void	LineCheck(int xpt, int ypt);
	void	SetMouseCursor(int kind);
	int	SetgRSpace(CDC* pDC, int mingam, double minY, double maxY);
	void	DrawLineInfo(CDC* pDC, int flag);
	void	SetxValue();
	void	DeleteGraphData(int nItem, BOOL allDelete = FALSE);
	double	GetHsGa(CString Code);
	int	GetCodeType(CString Code, int mmgb);
	void	SortHsga();
	void	AddGraphData(_GraphData GData);
	double	GetNu(double xvalue);
	double	GetNx(double xvalue);
	double	GetD2(double d1, double HV, double T);
	double	GetD1(double mValue, double hValue, double r, double HV, double T);
	void	DrawXLine(CDC* pDC, int mingam, double minY, double maxY);
	void	DrawK200(CDC* pDC);
	void	DrawZero(CDC* pDC, double minY, double maxY);
	void	initProperties();
	void	RemoveGLineArray(int flag = -1);
	double	GetEditHV(_GraphData *pData);
	//double	GetEditFee(bool chk, int type);
	void	SetText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt = DT_CENTER);
	void	DrawBox(CDC* pDC, COLORREF nOutLine, COLORREF nBkColor, CRect rect = CRect(-1, -1, -1, -1), UINT nLine = BOX_ALL, BOOL bFill = TRUE);
	double	MingamBuho(CString jcode, int mmgb, int mingam);
	double	CalFee(bool chk, int code, double price);
	double	GetFiveTen(double data);

	bool	calculateDisplay(double &div, double Max, double Min, double& drawMinValue, int dispTICK, double* dispValue, int& dispCount);
	int	GetDrawTickValue(CDC* pDC, double* pValue, double Max, double Min);
	int	GetTickCount(CDC* pDC);
	double	MinGrphsga(int cidx);
	double	MaxGrphsga(int cidx);

	void	GuganValue();
	void	BungiGap(int flag, int mingam);
	void	BungiCal(int flag, int cnt, double leftX, double rightX);
	double	GetJJis(double jjis);
	int	GetTimeMin(CString sTime);
};

