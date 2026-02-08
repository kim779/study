
#pragma once

class CExEdit : public CEdit
{
public:
	CExEdit();
	virtual ~CExEdit();
	
	//{{AFX_VIRTUAL(CExEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExEdit)
	afx_msg void OnSetfocus();
	afx_msg void OnKillfocus();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	SetParent(CWnd *pParent) { m_pParent = pParent; }
private:
	CWnd*	m_pParent;
};


