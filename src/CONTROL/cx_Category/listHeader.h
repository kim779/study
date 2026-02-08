#pragma once

class CExListHeader : public CHeaderCtrl
{
public:
	CExListHeader();
	virtual ~CExListHeader();

	//{{AFX_VIRTUAL(CExListHeader)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExListHeader)
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnLayout(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};