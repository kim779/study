#pragma once

class AFX_EXT_CLASS CPointerWnd : public CWnd
{
public:
	CPointerWnd();
	CPointerWnd(CChildView* view);
	virtual ~CPointerWnd();
public:
	int m_PenWidth;
	int m_PenStyle;
	CRect m_rc;
public:
	void Init(int PenWidth, int PenStyle);
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