#pragma once
#define UNUSED_VALUE	-99999.999
#ifndef TITLETIP_CLASSNAME
#define TITLETIP_CLASSNAME _T("__mytooltipwindow")
#endif
class CMBongToolTip : public CWnd
{
	
public:
	CMBongToolTip(CWnd* pView);
	virtual ~CMBongToolTip();

	CWnd*		m_bongwnd;
public:
	CSize CalcuWndSize(CString strExam);
	void DrawBitmap(CDC* pDC);
	CRect MakeSizeWnd();
	CRect SetData(const CStringArray& arString);
	
	//{{AFX_VIRTUAL(CMBongToolTip)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
#ifdef _DEBUG
#endif
	//{{AFX_MSG(CMBongToolTip)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	int		m_nCellHeight;
	CRect		m_rcBase;
	CRect		m_rcDraw;
	CString		m_strFontName;
	int		m_nFontSize;	
	COLORREF	m_clrBack;
	CFont*		m_font;	
	CStringArray	m_arTitle;
	CStringArray	m_arContent;
	CRgn		m_rgn;	

	CString		GetStrToken(int nIndex, CString data, char* token);
};