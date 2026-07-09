#if !defined(AFX_SINGLEPROPERTY_H__C2F19D51_B830_40B7_A193_747310108200__INCLUDED_)
#define AFX_SINGLEPROPERTY_H__C2F19D51_B830_40B7_A193_747310108200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SingleProperty.h : header file
//
#include <afxtempl.h>
#include "../BtnCombo.h"
#include "../NFBtn.h"
#include "../ListGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CSingleProperty window

/*class	CProHead
{
public:
	CString		headCaption;
	int		type;
	CString		value1;
	CString		value2;
	CString		formM;
	CString		desc;
};

typedef CArray	<CProHead, CProHead&> CProArr;

class	_GridAttr
{
public:
	int		headInfo;
	CSize		HeadSz, CellSz;
	bool		multiCol;
	COLORREF	lineColor;
	int		rows, cols;
};
*/
class AFX_EXT_CLASS CSingleProperty : public CWnd
{
// Construction
public:
	CSingleProperty();

// Attributes
public:
	bool			m_bInvalidate;
	int			m_index;
	CArray	<CString, CString>	m_writeInfo;
	_GridAttr		m_GridAttr;
	CString			m_sControlName;

protected:
	CPoint			m_StartPnt, m_SelectedPnt, m_InputPnt;
	CBitmap			*m_pBitmap;
	CEdit			*m_pEdit;
	CBtnCombo		*m_pCombo;
	CNFBtn			*m_pButton;
	CRect			m_ClientRt;
	CFont			m_font;

	struct	_mapH*		m_mapH;
	CString			m_field, m_sData;
	int			m_nCursor;
	bool			m_bKeyDown, m_bMove;
	bool			m_bVScroll;

	COLORREF		m_ctlBkColor;
	CBrush			m_ctlBrush;

	CMap	<int, int, CProArr*, CProArr*>	m_proMap;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingleProperty)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString	getData2(int col, int row);
	void	deleteCol(int col);
	int	insertCol(int pos);
	void	SetMulti(bool bMulti);
	CString convertData(CString str);
	CString getAllData();
	CString getValues();
	void	AdjustScroll();
	void	setHeadInfo(int headNum);
	void	setHeadInfo(int idx, CString headStr);
	CString getDesc(int pos);
	void	refreshData();
	void	SetKeyIn(char nChar);
	int	getVisibleCols();
	long	getGridWidth();
	void	DrawBitmap2(int col, int row, CRect rc);
	void	InvalidateRect2(CPoint pnt);
	void	OnBtnClick();
	void	OnSelEndOk();
	void	Initialize(CString sData);
	CString	getReservedVal1(CString str);
	CString	getFormM(int pos);
	CString	getValue2(int pos);
	CString	getValue1(int pos);
	LOGFONT	setFont(int fsize, CString fname = "±¼¸²Ã¼", BOOL bBold = FALSE, BOOL bItalic = FALSE);
	void	EraseCtrls(bool bFocus = true);
	void	OnChange();
	void	SetInputState();
	int	getVisibleRows();
	long	getGridHeight();
	int	getType(int pos);
	void	calScrollSz();
	CString	Parser(CString &srcstr, CString substr);
	CString	getHeadCaption(int pos);
	void	DrawCellData(CDC *pDC, CRect rc, CString data, COLORREF bkcolor = RGB(255, 255, 255), COLORREF frcolor = RGB(0, 0, 0), DWORD dwStyle = DT_LEFT);
	void	DrawHead(CDC *pDC);
	void	ResetGrid();
	void	AddCol(CString str);
	bool	DrawCellData(CDC *pDC, int col, int row, CString data);
	void	DrawBitmap(CDC *pDC);
	void	DrawGridData(CDC *pDC);
	void	setData(CString str, long col, long row);
	CString	getData(int col, int row);
	virtual ~CSingleProperty();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSingleProperty)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEPROPERTY_H__C2F19D51_B830_40B7_A193_747310108200__INCLUDED_)
