
#pragma once


// CTracker 명령 대상입니다.

class AFX_EXT_CLASS CTracker : public CRectTracker
{
public:
	CTracker(LPCRECT lpRect, UINT style);
	virtual ~CTracker();

public:
	void	GetTrueRect(LPRECT lpTrueRect);
	int	HitTest(CPoint point);
	void	Draw(CDC* pDC);
	int	HitTestHandles(CPoint point);
	void	GetHandleRect(int nHandle, CRect* pHandleRect);
};


