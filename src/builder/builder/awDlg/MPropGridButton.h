#pragma once


// CMPropGridButton

class CMPropGridButton : public CWnd
{
	DECLARE_DYNAMIC(CMPropGridButton)

public:
	CMPropGridButton();
	virtual ~CMPropGridButton();

protected:
	DWORD		m_state;
	bool		m_capture;
	bool		m_check;
	HBITMAP		m_hbitmap, m_hbitmap2;
	CFont		m_font;
	CImageList	m_imageList;

	void	setFont(LOGFONT* lf, int fsize, CString fname, BOOL bBold, BOOL bItalic);
	DWORD	getState();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


