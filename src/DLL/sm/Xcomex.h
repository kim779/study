#if !defined(AFX_XCOMEX_H__CAAD59AA_F201_4FEC_813C_5960933FFEC7__INCLUDED_)
#define AFX_XCOMEX_H__CAAD59AA_F201_4FEC_813C_5960933FFEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xcomex.h : header file
//


#define	XCOM_HEIGHT	22

#define	COMM_BASE	7000
#define	COMM_END	7100
/////////////////////////////////////////////////////////////////////////////
// CXcomex window

class CXcomex : public CWnd
{
// Construction
public:
	CXcomex(CWnd* parent);

// Attributes
protected:
	CWnd*		m_parent;
	CString		m_gName;
	CRect		m_bRc;
	CPen		m_pen;
	CFont		m_font;
	CBrush		m_brush;
	CBitmap*		m_arr;
	CBitmap*		m_arr_dn;
	CBitmap*		m_back;
	CBitmap*		m_band_back;

	BOOL		m_bTracking;
	BOOL		m_bHover;
	BOOL		m_bDown;

// Operations
public:
	void	init();
	void	changepos();
	void	set_gName(CString name);

	BOOL DrawMaskedBitmap(CDC* pDC, int x, int y, CBitmap* bitmap, UINT maskcolor, int width, int height);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXcomex)
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	virtual ~CXcomex();

	// Generated message map functions
protected:
	void	draw_Button(CDC* pDC);
	//{{AFX_MSG(CXcomex)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCOMEX_H__CAAD59AA_F201_4FEC_813C_5960933FFEC7__INCLUDED_)
