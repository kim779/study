// image.h: interface for the Cimage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__63F2E409_24F9_4C75_AEF9_B0541F087DC5__INCLUDED_)
#define AFX_IMAGE_H__63F2E409_24F9_4C75_AEF9_B0541F087DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define	PI_DEFAULT	0
#define	PI_ENTER	1
#define	PI_DOWN		2
#define	PI_OFF		3

class Cimage  
{
public:
	Cimage(class CAxisDraw* draw, CString fileN, bool only = false);
	virtual ~Cimage();

protected:
	CObArray	m_images;
	CByteArray	m_valids;
	CAxisDraw*	m_draw;

public:
	void	DrawPicture(CDC* dc, CRect rect, int style, bool oldVers, int index = PI_DEFAULT);
	bool	GetValid(int index = PI_DEFAULT);
	bool	GetHandle(CBitmap*& bitmap);
	bool	GetHandle(int index, CBitmap*& bitmap);

protected:
	void	transparentBlt(CDC *pDC, CBitmap *pBitmap, int x, int y, int cx, int cy);
};

#endif // !defined(AFX_IMAGE_H__63F2E409_24F9_4C75_AEF9_B0541F087DC5__INCLUDED_)
