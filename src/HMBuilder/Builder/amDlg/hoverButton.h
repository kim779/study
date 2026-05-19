#pragma once


// CHoverButton

class CHoverButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CHoverButton)

public:
	CHoverButton();
	virtual ~CHoverButton();

protected:
	BOOL	m_tracking;
	BOOL	m_hover;
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


