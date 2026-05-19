
#pragma once

class AFX_EXT_CLASS CTlImage
{
public:
	CTlImage(class CAmDraw* draw, CString fileN);
	virtual ~CTlImage();

protected:
	CBitmap*	m_image;
	bool		m_valid;
	CAmDraw*	m_draw;

public:
	void	DrawPicture(CDC* dc, CRect rect, int style);
	bool	GetValid()	{ return m_valid; }
	bool	GetHandle(CBitmap*& bitmap);
	CSize	GetSize();
};


