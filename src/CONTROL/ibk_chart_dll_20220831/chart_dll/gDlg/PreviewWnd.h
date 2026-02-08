#if !defined(AFX_PREVIEWWND_H__9593D867_FA8B_43A9_AE13_FE4EAE99F286__INCLUDED_)
#define AFX_PREVIEWWND_H__9593D867_FA8B_43A9_AE13_FE4EAE99F286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _bongdata
{
        char    date[5];	/* 시간 (2) 분(2)       */
        double  siga;		/* 시가                 */
        double	koga;		/* 고가                 */
        double	jega;		/* 저가                 */
        double	curr;		/* 종가(현재가)         */
	double	vol;
};

#define BOX_NONE	0
#define BOX_LEFT	1
#define BOX_RIGHT	2
#define BOX_TOP		4
#define BOX_BOTTOM	8
#define BOX_ALL		(BOX_LEFT | BOX_RIGHT | BOX_TOP | BOX_BOTTOM)

class CPreviewWnd : public CWnd
{
public:
	CPreviewWnd(CWnd* pParent = NULL);
	virtual ~CPreviewWnd();

public:

private:
	int		m_iDataCount;
	int		m_iSubKind;
	double		m_dMinValue;
	double		m_dMaxValue;
	double		m_dUnit;

	WORD		m_wUnionKind;

	CBitmap*	m_pBitmap;	// 화면 BMP	
	CPen		m_pen;
	CBrush		m_brush;	
	CRect		m_rectTotal;
	CRect		m_rectRegion;
	CRect		m_rectGraph;
	CRect		m_rectText;
	CRect		m_arectTick[3];	// 0:left 1: right 2: bottom
	
	CString		m_strName;
	COLORREF	m_rgbTick;

	struct	_graph*		m_pGraph;
	struct	_graph*		m_pBong;
	struct	_envInfo*	m_pEnv;

public:
	void	ShowGraph(struct _graph* pBong, COLORREF tkCOLOR, struct _envInfo* envinfo, struct _graph* pGraph, int nSubKind);
	void	SetUnion(WORD wUnion);
		
private:
// resource library
	CPen* selectPen(CDC* pDC, struct _line* lineinfo);
	CPen* selectPen(CDC* pDC, COLORREF clr, int nWidth = 1, int nType = PS_SOLID);

	CBrush*	selectBrush(CDC* pDC, COLORREF color);

// drawing library
	void drawWidthLine(CDC* pDC, CPoint pt[2], struct _line* lineinfo);	
	void drawBack(CDC* pDC);
	void setText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt = DT_CENTER);
	void setText(CDC* pDC, CPoint pt, CString strInput, COLORREF txColor);	
	void setText(CDC* pDC, CPoint pt, CString strInput, COLORREF txColor, COLORREF bkColor);	
	void drawBox(CDC* pDC, CRect rect, COLORREF clrLine, COLORREF clrBack, UINT nFlag = BOX_ALL);
	int getXPosL(int nIndex);
	int getXPosM(int nIndex);
	int getXPosR(int nIndex);
	int getYPos(double val);
	int getmarket(double val, CRect rect, double nMax);
	int getClock(double val, double nMax);


// drawing function
	void DrawGraph(CDC* pDC);
	
	
	void drawJPN(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void drawBAR(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void drawLIN(CDC* pDC, struct _line* lineinfo);
	void drawPOLE(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	
	void drawCLOCK(CDC* pDC);
	void drawPV(CDC* pDC);
	void drawCQV(CDC* pDC);
	void drawEQV(CDC* pDC);
	void drawSALE(CDC* pDC);
	void drawMaemul(CDC* pDC, CRect rect);
	void drawPNF(CDC* pDC);
	void drawTHREE(CDC* pDC);
	void drawBalance(CDC* pDC);
	void drawRenko(CDC* pDC);
	void drawKagi(CDC* pDC);

	//void	DrawUnionChart(CDC* pDC);
	//bool	CreateData();

	void	DrawUnionKagi(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionPnF(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionSwing(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionSamsun(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionRenko(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionGukki(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void	DrawUnionAVol(CDC* pDC, COLORREF clrUp, COLORREF clrDown);
	void    DrowRect(CDC* pDC, int x1,int x2,int y1,int y2);
		
	
	void drawTICK(CDC* pDC, bool bVol = false);	

	double getDefVal();
// logical function
	CSize calcuWndSize(CDC* pDC, CString strExam);
	void calcuArea(CRect rect = NULL);
	void calcuVolWidth(double* width);

// normal member function
	void drawBitmap(CDC* pDC = NULL);
	void loadBitmap(CDC *pDC = NULL);
	

	
	//{{AFX_VIRTUAL(CPreviewWnd)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPreviewWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_PREVIEWWND_H__9593D867_FA8B_43A9_AE13_FE4EAE99F286__INCLUDED_)

