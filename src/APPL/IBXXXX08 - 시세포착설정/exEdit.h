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
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	SetParent(CWnd* pParent) { m_pParent = pParent; }
	CString	GetText() { return m_str; }
private:
	CWnd*		m_pParent;
	CString		m_str;
};

