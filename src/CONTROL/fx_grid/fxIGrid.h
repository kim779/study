#pragma once
// QGrid.h : header file
//
#define	IGRIDCLASS	"fxIGrid"
#include <afxtempl.h>

#define	IGAT_COMMA	0x00000001
#define	IGAT_CONTRAST	0x00000002
#define	IGAT_CONDITION	0x00000004
#define	IGAT_CONDITIONx	0x00000008
#define	IGAT_ZEROSUP	0x00000010
#define	IGAT_ZEROSUPx	0x00000020
#define	IGAT_EDITFORMAT	0x00000040

#define	IGAT_LEFT	0x10000000
#define	IGAT_CENTER	0x20000000
#define	IGAT_RIGHT	0x40000000
using XArray = std::shared_ptr<CStringArray>;
class CIColumn
{
public:
	CString		m_sHeadCaption;
	DWORD		m_dAttr;
	long		m_lWidth;
	CString		m_sEditFormat;

public:
	CIColumn& operator = (const CIColumn qColumn)
	{
		m_sHeadCaption = qColumn.m_sHeadCaption;
		m_dAttr        = qColumn.m_dAttr;
		m_lWidth       = qColumn.m_lWidth;
		m_sEditFormat  = qColumn.m_sEditFormat;
		return (CIColumn&)*this;
	}

	CIColumn()
	{
		m_sHeadCaption.Empty();
		m_dAttr  = NULL;
		m_lWidth = 100;
		m_sEditFormat.Empty();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CfxIGrid window

class AFX_EXT_CLASS CfxIGrid : public CWnd
{
// Construction
public:
	CfxIGrid();

// Attributes
public:
	long		m_nSelectedRow;
	long		m_lHeadHeight;
	long		m_lDataHeight;

	COLORREF	m_lineColor;
	COLORREF	m_headBgColor;
	COLORREF	m_headFgColor;
	COLORREF	m_dataBgColor;
	COLORREF	m_dataBgColor1;
	COLORREF	m_dataBgColor2;
	COLORREF	m_dataFgColor;

	COLORREF	m_selFgColor;
	COLORREF	m_selBgColor;

	COLORREF	m_upColor;
	COLORREF	m_dnColor;
	COLORREF	m_eqColor;

	COLORREF	m_upRvColor;
	COLORREF	m_dnRvColor;
	
	BOOL		m_b3DHead;
	BOOL		m_b2RowColor;
	BOOL		m_bFillEmpty;

	BOOL		m_bMemDC;
	BOOL		m_bFixScroll;		// If scroll position is not zero, when add row, fix scroll position.
	BOOL		m_bSelection;

	BOOL		m_bColumnResize;

	CFont		*m_pFont;
protected:
	long		m_lSumColumnWidth;
	long		m_lSumVisColWidth;

	CPoint		m_StartPnt;
	CArray	<CIColumn, CIColumn>	m_arColumn;

	std::vector<XArray> _vArrData;
	//CArray	<CStringArray*, CStringArray*>	m_arData;
private:
	HCURSOR		m_hCursor;
	long		m_lMoveWColumn;
	long		m_lMoveWPos;

	BOOL		m_bFirstRowColor;

	int		m_wDisableBar;
	

// Operations
public:
	long	GetRowCount();
	void	ResetContent();
	void	DeleteRow(long nRow);
	void	AddQColumn(CIColumn qCol);
	void	AddRow(CString sDat, bool bTop = false);
	void	SetData(long nRow, long nCol, CString sDat);
	CString GetData(long nRow, long nCol);
protected:
	long	GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	long	GetScrollMax32(int nBar);
	void	adjustScroll(int wBar = SB_BOTH);

	CString Parser(CString &srcstr, CString substr);
private:
	CString CommaModify(CString string);
	CString	SetEditMask(CString sSrc, CString sMask);
	CString	getFormatData(CString src, DWORD dAttr);
	int	GetLastColCnt();
	long	GetColumnWidth(long nCol);
	
	void	DrawCellData(CDC *pDC, CRect cellRc, CString sDat, DWORD dwStyle, BOOL bHead = FALSE, BOOL bSelected = FALSE);
	void	drawContrast(CDC *pDC, CRect cellRc, CString sText, COLORREF clr);
	BOOL	DrawRow(CDC *pDC, long nRow, CRect visibleRc);
	void	DrawHead(CDC *pDC, CRect visibleRc);
	void	DrawAddRow(CDC *pDC, bool bTop);

	BOOL	RegisterWndClass();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxIGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawEmptyRow(CDC *pDC, CRect visibleRc);
	void ClearColumn();
	
	virtual ~CfxIGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxIGrid)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CMemDC : public CDC
{
public:

	// constructor sets up the memory DC
	CMemDC(CDC* pDC) : CDC()
	{
		ASSERT(pDC != nullptr);

		m_pDC = pDC;
		m_oldBitmap = nullptr;
		m_bMemDC = !pDC->IsPrinting();
              
		if (m_bMemDC)    // Create a Memory DC
		{
			pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());
			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else        // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
    
	// Destructor copies the contents of the mem DC to the original DC
	~CMemDC()
	{
		if (m_bMemDC) 
		{    
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						this, m_rect.left, m_rect.top, SRCCOPY);
			SelectObject(m_oldBitmap);
		}
		else
		{
			m_hDC = m_hAttribDC = nullptr;
		}
	}

	// Allow usage as a pointer
	CMemDC* operator->() { return this; }

	// Allow usage as a pointer
	operator CMemDC*() { return this; }

private:
	CBitmap  m_bitmap;
	CBitmap* m_oldBitmap;
	CDC*     m_pDC;
	CRect    m_rect;
	BOOL     m_bMemDC;
};

