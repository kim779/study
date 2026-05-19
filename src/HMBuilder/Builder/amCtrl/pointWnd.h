#pragma once


// CPointWnd

class CPointWnd : public CWnd
{
	DECLARE_DYNAMIC(CPointWnd)

public:
	CPointWnd();
	virtual ~CPointWnd();

protected:
	DECLARE_MESSAGE_MAP()

	COLORREF m_bgcol;
	CRgn	m_rgn;
	CArray	<CRect, CRect> m_rectArr;

public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void	AddRect(CRect rc);
	void	SetTP();
	void	debugX(CString gubn, CRect rect);
	void	SetRGB(COLORREF col){ m_bgcol = col;  }
};


