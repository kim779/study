#if !defined(AFX_SDIBAR_H__0756EFF4_1847_4729_9175_8630FD6796A5__INCLUDED_)
#define AFX_SDIBAR_H__0756EFF4_1847_4729_9175_8630FD6796A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sdibar.h : header file
//
#define SDIBUTTON_CNT	4
#define SDIBAR_ICON  	0
#define SDIBAR_PIN		1
#define SDIBAR_SWITCH	2
#define SDIBAR_CLOSE	3


#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CSdibar window
#include "usedefine.h"
class AFX_EXT_CLASS CSdibar : public CDialogBar
{
// Construction
public:
	CSdibar(bool (*callback)(int, WPARAM, LPARAM));

// Attributes
protected:
	int		m_width;
	int		m_hoverI;
	int		m_downI;
	bool		m_pinON;
	bool		m_mouseOver;
	BOOL		m_active;

	CBitmap*		m_logo;
	CBitmap*		m_bitmap[SDIBUTTON_CNT];

#ifdef DF_USESTL_SDIBAR
	std::unique_ptr <CToolTipCtrl>m_ToolTip;
#else
	CToolTipCtrl* m_ToolTip;
#endif


	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	CArray	< CRect, CRect > m_arRc;	// button rect
// Operations
public:
	void	drawBackground(CDC* pDC);
	void	drawButton(CDC* pDC);
	void	drawImg(CDC* pDC, CRect bRc, int idx, bool bDown = false);
	void	SetActive(BOOL bActive);
	void	SetRegion();
	int	GetSelectItem(CPoint pt);

	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	InitToolTip();
	void	HoverItem(CPoint point);
	CString	GetSMString(int idx);
	void	DrawBitmapByMask(CDC* pDC, CBitmap* bitmap, CRect bRC, BOOL bDown = FALSE, int maskcolor = Axis::maskColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSdibar)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSdibar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSdibar)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDIBAR_H__0756EFF4_1847_4729_9175_8630FD6796A5__INCLUDED_)
