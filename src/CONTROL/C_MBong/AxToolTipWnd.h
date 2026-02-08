#pragma once

#define UNUSED_VALUE	-99999.999
#ifndef TITLETIP_CLASSNAME
#define TITLETIP_CLASSNAME _T("__mytooltipwindow")
#endif
class CAxToolTipWnd : public CWnd
{
	
public:
	CAxToolTipWnd(CWnd* pView);
	virtual ~CAxToolTipWnd();

public:
	CSize CalcuWndSize(CString strExam);
	void DrawBitmap(CDC* pDC);
	CRect MakeSizeWnd();
	CRect SetData(const  CStringArray& arString);
	
	//{{AFX_VIRTUAL(CAxToolTipWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
#ifdef _DEBUG
#endif
	//{{AFX_MSG(CAxToolTipWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	
	int			m_nCellHeight;
	CRect			m_rcBase;
	CRect			m_rcDraw;
	CString			m_strFontName;
	int			m_nFontSize;	
	CWnd*			m_bongwnd;
	COLORREF		m_clrBack;
	CFont*			m_font;	
	CStringArray		m_arTitle;
	CStringArray		m_arContent;
	CRgn			m_rgn;	
};