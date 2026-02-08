#pragma once
// CodeReg.h : header file
//
#include "MapWnd.h"
#include "ControlWnd.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCodeReg window
struct _code
{
	CString code;
	CString name;
};

class CCodeReg : public CWnd
{
// Construction
public:
	CCodeReg(CWnd* pParent, CMapWnd* pMapWnd, CWnd* pView);

protected:
	CWnd*		m_pParent;
	CMapWnd*	m_pMapWnd;
	CWnd*		m_pView;

	CString		m_home;
	CString		m_user;

	CWnd*		m_pBtnHead;

	bool		m_bLBDown;
	bool		m_bPopMenu;
	CPoint		m_ptPopMenu;

	double		m_row;
	CArray <struct _code, struct _code>	m_arCode;

	void		setButtonBitmaps();

	COLORREF	GetIndexColor(int index);
	CBitmap*	GetAxBitmap(CString path);
	CFont*		GetAxFont(CString fName = "±¼¸²", int point = 9, bool bItalic = false, int nBold = 0);
	CBrush*		GetAxBrush(COLORREF clr);
	CPen*		GetAxPen(COLORREF clr, int width, int style);
	CString		SetComma(CString src);
	CString		Parser(CString &srcstr, CString substr);
	void		DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat = 0, UINT type = 0);
	void		LoadCode();
	void		SaveCode();

private:
	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;

// Operations
public:
	void		Init(CString home, CString user, bool bMenu);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeReg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeReg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeReg)
	afx_msg LRESULT OnButtonHead();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuDeleteall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

