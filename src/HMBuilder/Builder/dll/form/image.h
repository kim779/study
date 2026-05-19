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
#define PI_LONGTAB	4

class Cimage  
{
public:
	Cimage(class CAmDraw* draw, CString fileN, bool only = false);
	virtual ~Cimage();

protected:
	//CObArray	m_images;
	CArray<Image*, Image*>	m_images;
	CByteArray	m_valids;
	CAmDraw*	m_draw;

public:
	void	DrawPicture(CDC* dc, CRect rect, int style, int index = PI_DEFAULT);
	void	DrawPicture(Graphics& g, CRect rect, int style, int index = PI_DEFAULT);
	bool	GetValid(int index = PI_DEFAULT);
	bool	GetHandle(CBitmap*& bitmap);
	bool	GetHandle(int index, CBitmap*& bitmap);
	bool	GetSize(int& width, int&height);
};

#endif // !defined(AFX_IMAGE_H__63F2E409_24F9_4C75_AEF9_B0541F087DC5__INCLUDED_)


/*


int ex=100,ey=100;
CachedBitmap *pCBit;
 
void UpdateScreen()
{
     Graphics G(hWndMain);
     RECT crt;
     GetClientRect(hWndMain,&crt);
     Bitmap *pBit=new Bitmap(crt.right,crt.bottom,&G);
     Graphics *memG=new Graphics(pBit);
     memG->FillRectangle(&SolidBrush(Color(255,255,255)),0,0,crt.right,crt.bottom);
 
     SolidBrush S(Color(0,0,255));
     int x,y;
 
     for (x=0;x<=800;x+=50) {
          for (y=0;y<=600;y+=50) {
              memG->FillRectangle(&S,x,y,40,40);
          }
     }
 
     SolidBrush S2(Color(128,255,0,0));
     memG->FillEllipse(&S2,ex,ey,150,150);
     if (pCBit) {
          delete pCBit;
     }
     pCBit=new CachedBitmap(pBit,&G);
     delete pBit;
     delete memG;
     InvalidateRect(hWndMain,NULL,FALSE);
}
 
void OnPaint(HDC hdc)
{
     Graphics G(hdc);
 
     if (pCBit == NULL) {
          UpdateScreen();
     }
     G.DrawCachedBitmap(pCBit,0,0);
}

*/