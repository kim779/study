
#pragma once
/////////////////////////////////////////////////////////////////////////////
#define ID_CUSTOMLIST			WM_USER + 101 
#define CM_SELECTION_CHANGED	WM_USER + 102

/////////////////////////////////////////////////////////////////////////////
// One Item Structure
typedef struct tagLISTITEM
{	
	BYTE		btHeader;
	COLORREF	crTextColor;
	COLORREF	crTextHighlightColor;
	 
} LISTITEM;

/////////////////////////////////////////////////////////////////////////////
// CDropList window
class CDropList : public CListBox
{
	int			m_nGroupCount,
				m_nPrevSelection;
	CPtrList	m_ItemList;
	CRect		m_WindowRect;
	
	CString		m_csText;

	CFont*		m_pTextFont;
	std::unique_ptr<CFont>	m_pHeaderFont;
	std::unique_ptr<CPen>	m_pBkPen;
	std::unique_ptr<CBrush>	m_pBkBrush;
	std::unique_ptr<CBrush>	m_pBkHighlightBrush;

	// Current Items
	std::shared_ptr<LISTITEM> 	m_pCurListItem;
	int			AddToList(int nGroup, CString csText);
	void		ClearItems();
	
// Construction
public:
	CDropList();

// Attributes
private:
	void	DrawItem(CDC* pDC, CRect ItemRect, BOOL bSelected);

// Operations
public:
	void	SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void	SetBkColor( COLORREF crColor );
	void	SetHighlightBkColor( COLORREF crColor );

	// General Info Functions
	inline	int GetGroupCount() { return m_nGroupCount; };
	inline	LISTITEM* GetCurListItem() { return m_pCurListItem.get(); };
	inline	int GetPreviousSelection() { return m_nPrevSelection; };
	void	SetPreviousSelection(int nPrevSelection);

	// Overrides
	int		AddString(LPCTSTR lpszItem);
	int		AddString(LPCTSTR lpszItem, int GroupPos, COLORREF crTextColor = RGB(0,0,0), COLORREF crTextHighlightColor = RGB(0,0,0));
	int		DeleteString(UINT nIndex);
	void	ResetContent();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropList)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDropList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDropList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CListWnd window
class CListWnd : public CWnd
{
public:
	CWnd*	m_pParent;
	
// Construction
	CListWnd();
	BOOL	Create(CWnd* pParentWnd, UINT uID);

// Attributes
public:
	std::unique_ptr<CDropList>		m_pDropList;
	bool			m_bShow;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListWnd)
	afx_msg void OnSelectionChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDropButton window
class CDropButton : public CButton
{
public:
	int				m_nListWidth,
					m_nListHeight;
	UINT			m_uID;
	bool			m_bDeleteFont,
					m_bChangeTitle,
					m_bClked,
					m_bOver;
	CWnd*			m_pParent;

	// Painting
	CRect			m_WindowRect,
					m_ListRect;
	std::unique_ptr <CPen>			m_pBorderPen;
	std::unique_ptr <CPen>			m_pTriPen;
	std::shared_ptr <CFont>			m_pTextFont;
	std::unique_ptr <CBrush>		m_pBkBrush;
	std::unique_ptr <CBrush>		m_pBkOverBrush;
	std::unique_ptr <CBrush>		m_pTriBrush;
   	COLORREF		m_crTxtColor;
	COLORREF		m_crTxtOverColor;

	void	DrawTriangle(CDC* pDC, CPoint ptTopLeft, int nWidth);
	void	DisplayList();

	// Custom List box
	std::unique_ptr <CListWnd>		m_pDropWnd;

//	friend static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

// Construction
public:
	CDropButton();

// Attributes
public:

// Operations
public:
	BOOL	Create(CString csCaption, CRect Rect, CWnd* pParent, UINT uID, COLORREF crBkColor = -1, COLORREF crBkOverColor = -1, COLORREF crTxtColor = -1, COLORREF crTxtOverColor = -1, CFont* pTextFont = NULL, COLORREF crBkListColor = RGB(255,255,255));
	BOOL	Init(CString csCaption, CRect Rect, CWnd* pParent, UINT uID, COLORREF crBkColor = -1, COLORREF crBkOverColor = -1, COLORREF crTxtColor = -1, COLORREF crTxtOverColor = -1, CFont* pTextFont = NULL, COLORREF crBkListColor = RGB(255,255,255));
	void	SetListWidth(int nWidth) { m_nListWidth = nWidth; };
	void	SetListHeight(int nHeight) { m_nListHeight = nHeight; };
	void	ChangeTitleWithSelection(bool bChangeTitle) { m_bChangeTitle = bChangeTitle; };
	void	AddString(CString str);
	void	ResetContent();
	
	// List box Settings
	CDropList* GetList() { return m_pDropWnd->m_pDropList.get(); };

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDropButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	// Generated message map functions
protected:
	afx_msg LONG OnSelectionChanged(UINT wParam, LONG lParam);
	//{{AFX_MSG(CDropButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

