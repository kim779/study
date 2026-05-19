#pragma once


// CPreWnd

class CPreWnd : public CWnd
{
	DECLARE_DYNAMIC(CPreWnd)

public:
	CPreWnd();
	virtual ~CPreWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


