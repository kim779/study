// Ctrl.h: interface for the CCtrl class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

/////////////////////////////////////////////////////////////
class CExScrollBar : public CScrollBar
{
public:
	CExScrollBar();
	virtual ~CExScrollBar();
	//{{AFX_VIRTUAL(CExScrollBar)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExScrollBar)
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	Initialize(SCROLLINFO sInfo);

private:
	int	getPos(int type, int gap = -1);
};