// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMyBitmap : public CBitmap  
{
public:

	CMyBitmap();
	virtual ~CMyBitmap();

	int GetWidth(){ return m_nWidth; }
	int GetHeight(){ return m_nHeight; }

	void Draw(CDC *pDC, int x, int y);
	void Draw(CDC *pDC, int x, int y, int srcX, int srcY, int esrcX, int esrcY);
	void Draw(CDC *pDC, int x, int y, int w, int h, POINT p, DWORD dwRop);
	void Draw(CDC *pDC, int x, int y, int w, int h, int srcX, int srcY, DWORD dwRop);
	void Draw(CDC *pDC, RECT dest);
	
	BOOL LoadBitmap(UINT nIDResource);
	BOOL LoadBitmap(CString strFilename);

protected:
	
	int		m_nWidth;
	int		m_nHeight;
};


