#pragma once

class CChildView;

class AFX_EXT_CLASS CTrackerWnd : public CWnd
{
public:
	CTrackerWnd();
	CTrackerWnd(CChildView* view);
	virtual ~CTrackerWnd();

	void SetFst(bool bFst);
public:
	int m_bFst;
	CChildView* m_view;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};