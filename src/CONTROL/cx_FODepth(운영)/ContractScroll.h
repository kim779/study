#pragma once
// ContractScroll.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContractScroll window

class CContractScroll : public CScrollBar
{
public:
	CContractScroll();
	virtual ~CContractScroll();
	//{{AFX_VIRTUAL(CContractScroll)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CContractScroll)
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	Initialize(SCROLLINFO sInfo);

private:
	int	getPos(int type, int gap = -1);
};