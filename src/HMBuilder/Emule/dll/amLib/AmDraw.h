
#pragma once

#define RECTG(rc)	Rect(rc.left, rc.top, rc.Width() - 1, rc.Height() - 1)
#define RECTGA(rc)	Rect(rc.left, rc.top, rc.Width(), rc.Height())
#define RECTF(rc)	RectF((REAL)rc.left, (REAL)rc.top, (REAL)(rc.Width()), (REAL)(rc.Height()))

//	fontR
struct	_fontR {
	CString	name;				// font face name
	int	point;				// font point
	bool	italic;				// italic
	int	bold;				// bold
};

//	penR
struct	_penR {
	ARGB	clr;				// pen color
	int	width;				// pen width
};

class CAmImage
{
public:
	CAmImage();
	virtual ~CAmImage();

public:
	time_t	m_mtime;
	Image*	m_image;
};

class AFX_EXT_CLASS CAmDraw
{
public:
	CAmDraw();
	virtual ~CAmDraw();

public:	
	CFont*			SetDcFont(struct _fontR* fontR);	
	Gdiplus::Font*		SetFont(struct _fontR* fontR);
	Gdiplus::Pen*		SetPen(struct _penR* penR);
	Gdiplus::SolidBrush*	SetBrush(ARGB crARGB);
	Image*			SetImage(char* cFileN);

private:
	void	RemoveAllFont();
	void	RemoveAllPen();
	void	RemoveAllBrush();
	void	RemoveAllImage();

protected:
	CMapStringToOb	m_pointFontObs;
	CMapStringToPtr	m_pointFontPtr;
	CMapStringToPtr	m_penPtr;
	CMap <ARGB, ARGB, SolidBrush*, SolidBrush*> m_brushPtr;
	CMapStringToPtr	m_imagePtr;
};


