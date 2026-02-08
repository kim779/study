#pragma once
// MBongWnd.h : header file
//
#include <afxmt.h>
#include <afxtempl.h>
#include "MBongLib.h"
/////////////////////////////////////////////////////////////////////////////
// CMBongWnd window

class CMBongWnd : public CWnd
{
// Construction
public:
	CMBongWnd(CWnd *pWizard, CWnd *pParent);
	void	Init(bool bPrice, bool bVolume, bool bLine = true);
	void	Clear();
	void	Dispatch(char *pData, CString sUpper, CString sLower, int Digit);
	void	Alert(CString sRts);
	void	Alert(struct _alertR* alertR);
	void	Font(CFont *pFont);
	void	Palette();

// Attributes
public:
	CWnd* m_pWizard;
	CWnd* m_pParent;

private:
	CFont	*m_pFont;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMBongWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMBongWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMBongWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	calcuArea(int cx = -1, int cy = -1);
	void	calcuMinMax();
	void	calculate();
	void	makeString(class CBongData* pData, int nIndex, CStringArray& arString);
	void	drawGraph(CDC* pDC);
	void	drawBong(CDC* pDC);
	void	drawMA(CDC* pDC);
	void	drawPlusMinus(CDC* pDC);
	void	drawVolume(CDC* pDC);
	void	DrawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = true);
	void	calcuMA(double* target, int nDay);
	double	calcuMA(int nDay, int nIndex);
	BOOL	IsInner();
	CSize	CalcuWndSize(CString strExam);

	int	getXPosL(int nIndex);
	int	getXPosM(int nIndex);
	int	getXPosR(int nIndex);
	int	getYPos(double val, double maxValue, double minValue, CRect rect);
	int	getDataIndex(CPoint pt);

	CString	GetTipStr(CPoint pt);
	void	MouseMove(CPoint pt);
	COLORREF GetGradColor(COLORREF color, bool bDark);
		
	CRect		m_rcBong;
	CRect		m_rcVolume;
	CRect		m_rcGraph;
	CRect		m_rcBase;
	CRect		m_rcPlus;
	CRect		m_rcMinus;
	int		m_nTitle;
	double		m_fUnit;
	bool		m_bVolume;
	bool		m_bToolTip;
	bool		m_bPrice;
	bool		m_bLine;

	int		m_nCandle;	
	double		m_fBMax;
	double		m_fBMin;
	double		m_fVMax;
	double		m_fVMin;

//	double**	m_MAData;
	std::vector<std::unique_ptr<double[]>>	m_MAData;

	int		m_nDigit;
	double		m_dPower;
	std::unique_ptr<class CMBongToolTip> m_tooltip;
	CString		m_szPlus;
	CString		m_szMinus;	
	CArray<int, int>	m_arMADay;
//	CIHArray<class CBongData*, class CBongData*>	m_arData;
	CArray<std::shared_ptr<class CBongData>, std::shared_ptr<class CBongData>>	m_arData;
	class CCX_FODepthApp*	m_pApp;

	CStringArray	m_arString;
	CString		m_PosData;

	COLORREF	m_clrUp;
	COLORREF	m_clrDown;
	COLORREF	m_clrLine;
	COLORREF	m_clrTable;
};