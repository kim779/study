#pragma once

#define	userCOLOR	0x02000000

// CColorBox

class CColorBox : public CWnd
{
	DECLARE_DYNAMIC(CColorBox)

public:
	CColorBox(CWnd* pParent, DWORD color, bool user =true, int offsetClr = 0);
	virtual ~CColorBox();

	DWORD	m_color;

protected:
	CString	m_path;
	bool	m_user;
	int	m_offsetClr;

	void	saveUserColor(COLORREF* ccolor);
	void	loadUserColor(COLORREF* ccolor);
	void	onUser();
	CRect	makeRect(int row = -1, int col = -1);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


