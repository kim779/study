#if !defined(AFX_PNBASEWND_H__4D6D7A2F_D3A8_4A47_8320_27F2CAA39BEC__INCLUDED_)
#define AFX_PNBASEWND_H__4D6D7A2F_D3A8_4A47_8320_27F2CAA39BEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnBaseWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd window
#include "afxtempl.h"

class CPnBaseWnd : public CWnd
{
// Construction
public:
	CPnBaseWnd(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnBaseWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnBaseWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnBaseWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetCtrlID(int ctrlID) { m_ctrlID = ctrlID; }
	int	GetCtlrID() { return m_ctrlID; }

	void		SetBackColor(COLORREF clrBack)	{ m_clrBack = clrBack; }
	COLORREF	GetBackColor() { return m_clrBack; }
	void		SetFrameColor(COLORREF clrFrame){ m_clrFrame = clrFrame; }
	COLORREF	GetFrameColor() { return m_clrFrame; }

protected:
	class CC_TotalApp	*m_pApp;
	CWnd		*m_pView;
	CWnd		*m_pParent;
	CFont		*m_pFont;
	
	int		m_ctrlID;	
	COLORREF	m_clrBack;
	COLORREF	m_clrFrame;

	// mouse move
	bool		m_bLButton;
	int		m_moveID;
	int		m_selID;
	bool		m_bCapture;

	CArray <struct _comp *, struct _comp *>	m_arComs;
	CString		m_comment;

	COLORREF	m_crColourBk;
	int		m_wdLine;
	enum	{ dtButton = 1, dtColor, dtWidth, dtEtc, dtSeparator };

protected:
	struct _comp*	SearchCom(CPoint pt);
	struct _comp*	SearchCom(int ID);
	CString	GetPosComment(CPoint pt);
	void	DeleteComs();

	void	InvalidateCom(int idc, int status);
	void	InvalidateCom(struct _comp *comp, int status);

	void	DrawBackground(CDC* pDC);
	void	DrawFrame(CDC* pDC);
	void	DrawButton(CDC* pDC, int dtype, struct _comp *comp, CImageList* m_pImage = NULL);
};

////////////////////////////////////////////////////////////
class CPnSlider : public CSliderCtrl
{
public:
	CPnSlider();
	virtual ~CPnSlider();

	//{{AFX_VIRTUAL(CPnSlider)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPnSlider)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////
class CPnScrollBar : public CScrollBar
{
public:
	CPnScrollBar();
	virtual ~CPnScrollBar();
	//{{AFX_VIRTUAL(CPnScrollBar)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPnScrollBar)
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	Initialize(SCROLLINFO sInfo);
private:
	int	getPos(int type, int gap = -1);
};

/////////////////////////////////////////////////////////////////////////////
// CPnToolTip window

class CPnToolTip : public CWnd
{
// Construction
public:
	CPnToolTip(CWnd* pView, CWnd* pParent, int event);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnToolTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnToolTip)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnActivateApp(BOOL bActive, DWORD lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CC_TotalApp	*m_pApp;
	CWnd		*m_pView;
	CWnd		*m_pParent;
	CFont*		m_pFont;
	int		m_event;

	CWnd		*m_pWnd;
	int		m_showDelay;

	COLORREF	m_clrBkColor;
	COLORREF	m_clrTextColor;

	CSize		m_fSize;

	int		m_cntWidth;
	int		m_cntHeight;

	CPoint		m_savePt;

public:
	CString	m_dispStr;
public:
	BOOL	Create(CFont* pFont);
	void	RelayEvent(UINT message);
	void	hideToolTip();

private:
	void	showToolTip();
	bool	checkViewFocus();
	void	getTextInfo(CString str, int& cntWidth, int& cntHeight);
};

class CPnWndEdit : public CEdit
{
// Construction
public:
	CPnWndEdit(CWnd* pParent, CWnd* pView, int type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnWndEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnWndEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnWndEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CWnd	*m_pParent;
	CWnd	*m_pView;
	int	m_Type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNBASEWND_H__4D6D7A2F_D3A8_4A47_8320_27F2CAA39BEC__INCLUDED_)
