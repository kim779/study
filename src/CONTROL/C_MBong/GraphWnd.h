#pragma once

#include <afxmt.h>
#include <afxtempl.h>

class CAxToolTipWnd;
class CGraphWnd : public CWnd
{
public:
	CGraphWnd(CWnd* pView, struct _param* cInfo);
	virtual ~CGraphWnd();
	void SetData(char* ptr);
	COLORREF GetColor(int index) { m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index); }

	//{{AFX_VIRTUAL(CGraphWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGraphWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CGraphWnd)
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	
	void updatePalette();
	void drawBitmap(CDC* pDC);
	void loadBitmap(CDC* pDC);
	void calcuArea(int cx = -1, int cy = -1);
	void calcuMinMax();
	void calculate();
	void makeString(CBongData* pData, int nIndex, CStringArray& arString);
	void setReal(char* ptr);
	void inputData(char* lParam);
	void drawGraph(CDC* pDC);
	void drawBong(CDC* pDC);
	void drawMA(CDC* pDC);
	void drawPlusMinus(CDC* pDC);
	void drawVolumn(CDC* pDC);
	void DrawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = true);
	void parseParam(char* ptr);
	void calcuMA(double* target, int nDay);
	double calcuMA(int nDay, int nIndex);
	CString	makeOption();
	BOOL IsInner();
	CRect MakeAlign();
	CSize CalcuWndSize(CString strExam);
	void clearAll();	

	int getXPosL(int nIndex);
	int getXPosM(int nIndex);
	int getXPosR(int nIndex);
	int getYPos(double val, double maxValue, double minValue, CRect rect);
	int getDataIndex(CPoint pt);

	CString	GetTipStr(CPoint pt);
	void	MouseMove(CPoint pt);
		
	CBitmap*	m_pBitmap;
	CWnd*		m_pView;
	CRect		m_rcBong;
	CRect		m_rcVolume;
	CRect		m_rcGraph;
	CRect		m_rcBase;
	CRect		m_rcPlus;
	CRect		m_rcMinus;
	int		m_nTitle;
	CString		m_szDigit;
	double		m_fUnit;
	bool		m_bVolume;
	bool		m_bToolTip;
	bool		m_bPrice;
	bool		m_bLine;

	int		m_nCandle;	
	int		m_nDataType;
	int		m_nGab;
	double		m_fBMax;
	double		m_fBMin;
	double		m_fVMax;
	double		m_fVMin;

	int		m_nDataSize;
	struct	jbong30	m_jbong30;
	double**	m_MAData;
	CString		m_output;
	CString		m_option;
	int		m_nDigit;
	bool		m_bTimer;
	UINT		m_tidOutCheck;
	CAxToolTipWnd*	m_tooltip;
	int		m_nIndex;
	CString		m_szPlus;
	CString		m_szMinus;	
	CArray<int, int>	m_arMADay;
	CIHArray<class CBongData*, class CBongData*>	m_arData;
	int		m_nFontSize;
	CString		m_szFontName;
	class CC_MBongApp*	m_pApp;

	CStringArray	m_arString;
	CString		m_PosData;

	COLORREF	m_clrUp;
	COLORREF	m_clrDown;
};