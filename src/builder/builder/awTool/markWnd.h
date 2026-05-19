#pragma once


// CMarkWnd

class CMarkWnd : public CWnd
{
	DECLARE_DYNAMIC(CMarkWnd)

public:
	CMarkWnd();
	virtual ~CMarkWnd();

protected:
	DECLARE_MESSAGE_MAP()

	CRgn	m_rgn;

public:
	afx_msg void OnPaint();

	void	SetTP(int type);
};
