#if !defined(AFX_LISTGRID_H__0DF3837D_4711_48B3_8CEA_985C6E30D614__INCLUDED_)
#define AFX_LISTGRID_H__0DF3837D_4711_48B3_8CEA_985C6E30D614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ListGrid.h : header file
//
#include <afxtempl.h>
#include "BtnCombo.h"
#include "NFBtn.h"
#include "mapform.h"
#include "awCommon/FormItem.h"
#include "awDlg/SepWnd.h"

#define	PA_HIDDEN	0x00000001

/////////////////////////////////////////////////////////////////////////////
// CListGrid window

class	CProHead
{
public:
	CString		headCaption;
	int		type;
	CString		value1;
	CString		value2;
	CString		formM;
	CString		desc;
	int		nAttributes;
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

class AFX_EXT_CLASS CListGrid : public CWnd
{
// Construction
public:
	CListGrid();

// Attributes
public:
	bool			m_bInvalidate;
	int			m_index;
	CString			m_errStr;
	HCURSOR			m_NoDropMoveCursor, m_DropMoveCursor;

	CMap	<int, int, int, int>	m_multiSelMap;

protected:
	CArray	<CString, CString>	m_writeInfo;
	CPoint			m_StartPnt, m_SelectedPnt, m_InputPnt;
	CBitmap			*m_pBitmap;
	_GridAttr		m_GridAttr;
	CEdit			*m_pEdit;
	CBtnCombo		*m_pCombo;
	CNFBtn			*m_pButton;
	CRect			m_ClientRt;
	CFont			m_font;

	CFormItem		*m_pFormItem;
	struct	_mapH*		m_mapH;
	CString			m_field;
	int			m_nCursor;
	bool			m_bKeyDown, m_bMove;

	COLORREF		m_ctlBkColor;
	CBrush			m_ctlBrush;

	CMap	<int, int, CProArr*, CProArr*>	m_proMap;
private:
	CPoint			m_dragPnt;
	int			m_nDragCol;
	bool			m_bDrag;
	CSepWnd			*m_pSepWnd;
	CString			m_sBeforeVal;

	CMapStringToPtr		m_fieldMap;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListGrid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	int	getAttr(int pos);
	void	SetCellSize(long cx, long cy);
	void	UpdateCombo();
	int	getDefaultValue(CString sSection, CString sKey, int nDefault);
	void	RedrawGrid();
	void	setHeadInfo(int headNum);
	void	setHeadInfo(int idx, CString headStr);
	CString getDesc(int pos);
	bool	checkReservedWord(CString name);
	CString getInitSymbol(CString sSymbol);
	void	refreshData();
	void	SetKeyIn(char nChar);
	void	gridMultiLineCheck(int idx);
	int	insertCol(int pos);
	void	deleteCol(int col);
	int	getVisibleCols();
	long	getGridWidth();
	void	DrawBitmap2(int col, int row, CRect rc);
	void	InvalidateRect2(CPoint pnt);
	void	SetMulti(bool bMulti);
	bool	IsGroupSameValue(int nGroup, int fnum, int row);
	void	OnBtnClick();
	void	setFormValue(int fnum, int idx, int col, int row, CString data);
	CString	getFormValue(int fnum, int idx, int row, int col = -1);
	void	OnSelEndOk();
	int	_httoi(const TCHAR *value);
	bool	setIndex(int idx);
	void	SaveData(int idx, int col = 0, int row = -1, bool bMulti = true);
	void	Initialize(struct _mapH* mapH);
	void	LoadData(int idx, int col = 0, int row = -1);
	CString	getReservedVal1(CString str);
	CString	getFormM(int pos);
	CString	getValue2(int pos);
	CString	getValue1(int pos);
	LOGFONT	setFont(int fsize, CString fname = "MS Sans Serif", BOOL bBold = FALSE, BOOL bItalic = FALSE);
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
	void	SetLikeCell(int nCol, int nSrcCol = 0);
	virtual ~CListGrid();

protected:
	bool duplicateSym(CString sym);
	void AdjustScroll();
private:

	// Generated message map functions
protected:
	//{{AFX_MSG(CListGrid)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnEraseCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMultiSel(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTGRID_H__0DF3837D_4711_48B3_8CEA_985C6E30D614__INCLUDED_)
