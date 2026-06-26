#pragma once


// CNoteWnd

class CNoteWnd : public CWnd
{
	DECLARE_DYNAMIC(CNoteWnd)

public:
	CNoteWnd();
	virtual ~CNoteWnd();

	CString m_slog;
	CString m_data{};
	BOOL m_bMouseTracking{};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


