#pragma once
// SListCtrl1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl1 window

#include "SHeaderCtrl1.h"
#include "ListEdit.h"
#include "TipDlg.h"

#define WM_REQUEST_NEXT WM_USER + 2007
#define WM_LIST_CLK WM_USER + 2008
#define WM_DO_CALC WM_USER + 2009
#define WM_LIST_DBCLK WM_USER + 2010
#define ARR_UP  "¡è"
#define ARR_DN  "¡é"
#define TRI_UP  "¡ã"
#define TRI_DN  "¡å"

class CSListCtrl1 : public CListCtrl
{
// Construction
public:
	CSListCtrl1(CMapWnd *mapwnd);

// Attributes
public:

// Operations
public:
	bool		m_bSortAsc;
	int			m_nSelectedItem;
	COLORREF	m_clrBk1;
	COLORREF	m_clrBk2;
	COLORREF	m_clrText;
	COLORREF	m_clrSelectText;
	COLORREF	m_clrSelectBk;
	COLORREF	m_clrEdge;

	CFont		m_Font;
	CSHeaderCtrl1 m_HdCtrl;
	int			m_nHideColIdx;
	int			m_FixCol;
	bool		m_bFix;
	bool		m_bEditEnable;
	int			m_disEditCol;
	bool		m_bIsEdit;
	std::unique_ptr<CEdit>	m_pEdit;
	CByteArray	m_EditableList;
	CMapWnd*	m_pMapWnd;
	int			m_nClkItem;
	int			m_nClkSubItem;

	std::unique_ptr<CTipDlg> m_pTooltip;
	HBITMAP		m_hTypeBitmap[2];

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSListCtrl1)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSListCtrl1();
	void HideColumnOver(int nColIdx);
	inline void SetBkColor(COLORREF bk,COLORREF bk1){
		m_clrBk1 = bk;m_clrBk2 = bk1;
	}
	inline void SetEdgeColor(COLORREF clr){
		m_clrEdge = clr;
	}
	inline void SetSelectColor(COLORREF clr){
		m_clrSelectBk = clr;
	}
	inline void SetTextColor(COLORREF clr){
		m_clrText = clr;
	}
	bool	IsEditSubItem(int nItem,int nSubItem);
//	void SetColors( COLORREF bk1,COLORREF bk2,COLORREF text,COLORREF selbk, COLORREF edge);
	void GetDrawColors(int nItem,
							   int nSubItem,
							   COLORREF& colorText,
							   COLORREF& colorBkgnd,
							   bool bSelect = true);

	void DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect );
	int		hitTestEx(const CPoint& Point, int* pColumn);
	bool	GetEditState();
	void	SetEditState(bool bIsEdit);
	void	DeleteEdit();
	CEdit*	EditSubItem(int item, int col);
	void	SetEditLabels();
	void	DisableEditColumn(int col);
    void    DrawTypeImage(CDC *pDC, const CRect& rc);
	void	SetImageType(HBITMAP hbitmap1, HBITMAP hbitmap1_dn);
	void	HeaderFix(bool bFix, int col);// Operations
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CSListCtrl1)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

