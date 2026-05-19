#pragma once


// CSepWnd

class CSepWnd : public CWnd
{
	DECLARE_DYNAMIC(CSepWnd)

public:
	CSepWnd();
	virtual ~CSepWnd();

	void	MoveSWWnd(int x, int y);
	void	MoveWnd(int x, int y);

protected:
	CRgn	m_wndRgn;

	void	setTP();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


