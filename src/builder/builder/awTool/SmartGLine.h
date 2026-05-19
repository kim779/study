#pragma once
#define LINE_X 0
#define LINE_Y 1

class AFX_EXT_CLASS CSmartGLine : public CWnd
{
public:
	CSmartGLine();
	CSmartGLine(CChildView* view);
	virtual ~CSmartGLine();
public:
	int m_PenWidth;
	int m_PenStyle;
	int m_lineXY;
	CRect m_rc;
public:
	void Init(int PenWidth, int PenStyle, int lineXY);
	void Redraw();
	void SaveRect(CRect rc);
	CRect LoadRect();
public:
	class CChildView* m_view;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};