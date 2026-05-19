#pragma once

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
	CSize		HeadSz;
	CSize		CellSz;
	COLORREF	lineColor;
	int		rows;
	int		cols;
};

// CMultiPropGrid

class CMultiPropGrid : public CWnd
{
	DECLARE_DYNAMIC(CMultiPropGrid)

public:
	CMultiPropGrid();
	virtual ~CMultiPropGrid();

protected:
	bool		m_invalidate;
	int		m_index;
	CString		m_errs;
	HCURSOR		m_noDropMoveCursor;
	HCURSOR		m_dropMoveCursor;

	CMap <int, int, int, int> m_multiSelMap;
	CStringArray	m_writeInfo;
	CPoint		m_startPnt;
	CPoint		m_selectedPnt;
	CPoint		m_inputPnt;
	CBitmap*	m_bitmap;
	_GridAttr	m_gridAttr;
	CEdit*		m_edit;
	class	CMPropGridCombo*  m_combo;
	class	CMPropGridButton* m_button;

	class	CFormItem* m_formItem;
	struct	_mapH*	m_mapH;
	int		m_cursor;
	bool		m_keyDown;
	bool		m_move;

	CRect		m_clientRc;
	CFont		m_font;
	COLORREF	m_ctlBkColor;
	CBrush		m_ctlBrush;

	CMap <int, int, CProArr*, CProArr*> m_proMap;

	DECLARE_MESSAGE_MAP()

private:
	CPoint		m_dragPnt;
	int		m_dragCol;
	bool		m_drag;
	class	CSepWnd* m_sepWnd;
	CString		m_beforeVal;
	CMapStringToPtr	m_fieldMap;
	CFontDialog*	m_fontD;

	int		m_gMax;

public:
	void	Initialize(struct _mapH* mapH);
	void	LoadData(int idx, int col = 0, int row = -1);
	void	SetHeadInfo(int headNum);
	void	SetHeadInfo(int idx, CString info);
	void	SetCellSize(long cx, long cy);
	int	InsertCol(int pos);
	void	DeleteCol(int col);
	bool	SetIndex(int idx);
	void	EraseCtrls(bool bFocus = true);
	void	ResetGrid();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

protected:
	void	saveData(int idx, int col = 0, int row = -1, bool bMulti = true);
	void	setFormValue(int fnum, int idx, int col, int row, CString data);
	CString	getFormValue(int fnum, int idx, int row, int col = -1);
	void	setData(CString str, long col, long row);
	CString	getData(int col, int row);
	void	setLikeCell(int nCol, int nSrcCol = 0);
	CString	getReservedVal1(CString str);
	CString	getFormM(int pos);
	CString	getValue2(int pos);
	CString	getValue1(int pos);
	int	getType(int pos);
	CString	getHeadCaption(int pos);
	int	getDefaultValue(CString sSection, CString sKey, int nDefault);
	bool	checkReservedWord(CString name);
	CString getInitSymbol(CString sSymbol);
	void	addCol(CString str);
	void	setInputState();
	void	updateCombo();
	int	getVisibleCols();
	int	getVisibleRows();
	long	getGridWidth();
	long	getGridHeight();

	void	setFont(LOGFONT* lf, int fsize, CString fname = _T("MS Sans Serif"), BOOL bBold = FALSE, BOOL bItalic = FALSE);
	void	invalidateRect2(CPoint pnt);
	void	drawCellData(CDC *pDC, CRect rc, CString data, COLORREF bkcolor = RGB(255, 255, 255), COLORREF frcolor = RGB(0, 0, 0), DWORD dwStyle = DT_LEFT);
	bool	drawCellData(CDC *pDC, int col, int row, CString data);
	void	drawHead(CDC *pDC);
	void	drawGridData(CDC *pDC);
	void	drawBitmap(CDC *pDC);
	void	drawBitmap2(int col, int row, CRect rc);

	void	checkGridMultiLine(int idx);
	void	adjustScroll();
	void	calScrollSz();
	CString	parse(CString &src, CString sub);
	int	hex2Int(CString value);

	afx_msg void OnSelEndOk();
	afx_msg void OnBtnClick();
	afx_msg LRESULT OnEraseCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApplyMultiSel(WPARAM wParam, LPARAM lParam);
};


