//
//  CSimpleSplitter
//
//  Splitter window with CWnd-derived panes
//  (C) Robert A. T. K?dy <kaldy@matfyz.cz>
//  last updated on 24.3.2004


#if !defined(SIMPLE_SPLITTER)
#define SIMPLE_SPLITTER

#define SPLITWIDTH		11
#define SSP_HORZ		1
#define SSP_VERT		2

#include "usedefine.h"
class CSimpleSplitter : public CWnd
{
public:
	CSimpleSplitter(int nPanes, UINT nOrientation = SSP_HORZ, int nMinSize = 30, int nBarThickness = 3);
	~CSimpleSplitter();

	BOOL Create(CWnd* pParent, COLORREF barColor = ::GetSysColor(COLOR_BTNFACE), UINT nID = AFX_IDW_PANE_FIRST);
	BOOL Create(CWnd* pParent, CBitmap* bkImage, UINT nID = AFX_IDW_PANE_FIRST);
	BOOL CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName = NULL);
	
	int GetPaneCount() const							{ return m_nPanes; }
	void SetPane(int nIndex, CWnd* pPaneWnd);
	CWnd* GetPane(int nIndex) const;
	virtual void SetActivePane(int nIndex);
	CWnd* GetActivePane(int* pIndex) const;
	void SetPaneSizes(const int* sizes);
	void GetPaneRect(int nIndex, CRect& rcPane) const;
	void GetBarRect(int nIndex, CRect& rcBar) const;
	void DefaultSize();

protected:
	void	draw(CDC* pDC);
	void	drawBackGround(CDC* pDC);
	void	drawSplitter(CDC* pDC);
	void	RecalcLayout();
	void	ResizePanes();
	void	InvertTracker();
	
	//{{AFX_MSG(CSimpleSplitter)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	const int	m_nPanes;
	const UINT	m_nOrientation;
	const int	m_nMinSize, m_nBarThickness;

	int		m_nTrackIndex{}, m_nTracker{}, m_nTrackerLength{}, m_nTrackerMouseOffset{};
	
#ifdef DF_USESTL_SMSPLITTER
	std::vector<CWnd*> m_pane;
	std::vector<int> m_size, m_orig;
#else
	CWnd** m_pane;
	int* m_size, * m_orig;
#endif

	bool		m_down;
	CBitmap*	m_bkIMG;
	COLORREF	m_barColor;
	CRect		m_rcClient;
};


#endif
