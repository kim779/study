// MyBitmap.h: interface for the CAxBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_)
#define AFX_AXBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxBitmap : public CBitmap  
{
public:

	CAxBitmap();
	virtual ~CAxBitmap();

	int GetWidth(){ return m_nWidth; }
	int GetHeight(){ return m_nHeight; }

	void Draw(CDC *pDC, int x, int y);
	void Draw(CDC *pDC, int x, int y, int srcX, int srcY, int esrcX, int esrcY);
	void Draw(CDC *pDC, int x, int y, int w, int h, POINT p, DWORD dwRop);
	void Draw(CDC *pDC, int x, int y, int w, int h, int srcX, int srcY, DWORD dwRop);
	void Draw(CDC *pDC, RECT dest);
	void DrawTilte(CDC *pDC, RECT dest);


	void DrawPosition(CDC *pDC, int x, int y, int width, int height);

	void DrawBitmapByMask(CDC *pDC,  CPoint targetPos = CPoint(0, 0), CPoint srcPos = CPoint(0, 0), COLORREF maskcolor = RGB(255, 0, 255), CRect bmpRc = CRect(0,0,0,0));
	void DrawBitmapByMask(CDC *pDC,  CPoint targetpos, COLORREF maskcolor, CRect bmpRc = CRect(0,0,0,0));
	
	BOOL LoadBitmap(UINT nIDResource);
	BOOL LoadBitmap(CString strFilename);

	void LoadInit();

protected:
	
	int		m_nWidth;
	int		m_nHeight;
};

#endif // !defined(AFX_AXBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_)
