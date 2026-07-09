// ControlTracker.h: interface for the CControlTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLTRACKER_H__4D91B176_5A8E_45FD_A932_12A460901713__INCLUDED_)
#define AFX_CONTROLTRACKER_H__4D91B176_5A8E_45FD_A932_12A460901713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CControlTracker : public CRectTracker  
{
public:
	CControlTracker(LPCRECT lpSrcRect, UINT nStyle);
	virtual ~CControlTracker();
public:
	void GetTrueRect(LPRECT lpTrueRect);
	int HitTest(CPoint point);
	int HitTestHandles(CPoint point);
	void Draw(CDC* pDC);
	void GetHandleRect(int nHandle, CRect* pHandleRect);

};

#endif // !defined(AFX_CONTROLTRACKER_H__4D91B176_5A8E_45FD_A932_12A460901713__INCLUDED_)
