#if !defined(AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_)
#define AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIClient.h : header file
//
#include "resource.h"
#include "EnBitmap.h"
#include "1Picture.h"
/////////////////////////////////////////////////////////////////////////////
// CMDIClient window
#define	COLOR_BKGROUND	GetSysColor(COLOR_APPWORKSPACE)
class AFX_EXT_CLASS CMDIClient : public CWnd
{
// Construction
public:
	CMDIClient(bool (*axiscall)(int, WPARAM, LPARAM), COLORREF color = COLOR_BKGROUND);
	virtual ~CMDIClient();

// Attributes
public:
	CWnd		*m_pBkMapWnd;
	int		m_nBkMode;
protected:
	CString		m_img;
	COLORREF	m_bk;
//	class CBb*	m_bb;

	int		m_iDrawStartScreen;
	

	CEnBitmap	m_edgeBmp[4];

	int		m_ctrlKey;
	int		m_shiftKey;
	bool		(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIClient)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	DrawBitmapByMask(CDC *pDC, CBitmap *pBitmap, CPoint pos, COLORREF maskcolor = Axis::maskColor);
	void	draw_background(CDC *pDC);
	void	change_Color(CString img, COLORREF color);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIClient)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString getHotCode(int key);

	BOOL		m_drawBackColor;
	BOOL		m_drawBackImage;
	COLORREF	m_colorBackground;
	CString		m_bitmapFileBackground;
	int			m_backImagePos;
	CPicture	m_picture;

	void LoadConfig();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_)
