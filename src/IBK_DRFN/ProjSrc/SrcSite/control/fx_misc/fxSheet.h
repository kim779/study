#if !defined(AFX_FXSHEET_H__B9AFB97B_FA15_4BFE_8AB7_A1CF506A4841__INCLUDED_)
#define AFX_FXSHEET_H__B9AFB97B_FA15_4BFE_8AB7_A1CF506A4841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fxSheet.h : header file
//
#include <afxtempl.h>

// CwkButton window
class CwkButton : public CButton
{
// Construction
public:
	CwkButton(int buttonID = -1, bool repeatLBDN = false);
// Attributes
protected:
	CSize   m_szIcon;

	CRect	m_rcButton;
	CRect	m_rcIcon;

	bool	m_mouse;
	bool	m_flate;

	int	m_buttonID;
	bool	m_repeatLBDN;

// Operations
public:
	void	disableFlate() { m_flate = false; }
	void	setIconRect();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CwkButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void SetIcon(HICON hIcon,CSize cSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CwkButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CwkButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

typedef struct _SVITEM {
	int	 key;
	char	 desc[24];

	WORD	 mask;
	COLORREF fgcol;
	COLORREF bkcol;
	CFont*	 font;

	CRect	 rect;
	WORD	 angle;			// 0 : none, 1:left, 2:right
} SVITEM;

const int scrollBTN = 4;
/////////////////////////////////////////////////////////////////////////////
// CfxSheet window

class AFX_EXT_CLASS CfxSheet : public CWnd
{
// Construction
public:
	CfxSheet(CString imageDir = _T(""), CFont* defaultFont = NULL, CFont* activeFont = NULL);

// Attributes
protected:
	bool		m_first;
	CWnd*		m_parent;
	UINT		m_parentID;
	bool		m_bottomPos;

	CImageList*	m_image;
	CString		m_imageDir;
	CRect		m_buttonRC;
	CwkButton*	m_button[scrollBTN];

	CRect		m_drawRC;

	int		m_scrollV;
	int		m_scrollMAX;

	CBrush		m_brushBK;
	COLORREF	m_colorBK;
	COLORREF	m_colorLN;		// Å×µÎ¸® Color
	CFont*		m_defaultFont;

	int		m_active;
	CFont*		m_activeFont;
	COLORREF	m_activeFG;
	COLORREF	m_activeBK;
	CArray <SVITEM*, SVITEM*> m_array;

	bool		m_mouse;
	int		m_move;
// Operations
public:
	CWnd*	Initial(CWnd* parent, UINT parentID, COLORREF bk = (COLORREF)0, bool bottom = true);

	bool	SetItem(const SVITEM* item);
	bool	GetItem(SVITEM* item);

	int	GetActiveKey();
	void	SetActiveKey(int key);

	bool	SetFgColor(int key, COLORREF fgcol = (COLORREF) 0);
	bool	SetBkColor(int key, COLORREF bkcol = (COLORREF) 0);
	bool	SetItemFont(int key, CFont* font);

	void	Adjust();
	void	Refresh();
	bool	Delete(int key);
	void	DeleteAll(bool refresh = true);
	bool	Append(SVITEM* item);

	void	SetActiveFont(CFont* font) { m_activeFont = font; }
	void	SetActiveColor(COLORREF fgcol, COLORREF bkcol) { m_activeFG = fgcol; m_activeBK = bkcol; }
	void	SetSheetLine(COLORREF color)	 { m_colorLN = color; }

protected:
	void	drawingSheet(CDC* pDC);
	int	getScrollWidth(bool bNext);
	LRESULT	messageToParent(int key, int msg, CPoint pt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxSheet)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCommandButton(UINT nID);
	afx_msg LONG OnRepeatLButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FXSHEET_H__B9AFB97B_FA15_4BFE_8AB7_A1CF506A4841__INCLUDED_)
