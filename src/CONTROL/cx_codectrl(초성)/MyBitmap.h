// MyBitmap.h: interface for the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_)
#define AFX_MYBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_MYBITMAP_H__0C24BF5E_1B92_446C_B69D_35465308FC9E__INCLUDED_)
