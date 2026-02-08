#pragma once


// CNoteWnd

class CNoteWnd : public CWnd
{
	DECLARE_DYNAMIC(CNoteWnd)

public:
	CNoteWnd();
	virtual ~CNoteWnd();


	CString m_data{};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


