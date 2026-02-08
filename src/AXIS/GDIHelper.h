#ifndef __GDIHELPER_H__
#define __GDIHELPER_H__

class CAxBufferDC: public CDC 
{
private:
	CBitmap m_bmp, *m_oldBmp;
	void Init(CDC& dc, int cx, int cy);
public:
	CAxBufferDC(CDC& dc, CRect& rc);
	CAxBufferDC(CDC& dc, int cx, int cy);
	CAxBufferDC(CDC& dc, CBitmap* bmp, int cx = -1, int cy = -1);
	CAxBufferDC(CDC& dc, int cx, int cy, UINT, UINT, LPVOID);
	~CAxBufferDC();
};


#define SRCMASK	0x00220326    // mask

class CAxDrawHelper
{
private:
	CDC* m_dc;
public:
	CAxDrawHelper(CDC* pDC);
	virtual ~CAxDrawHelper();
public:
	void DrawGradient(CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight = false);
	BOOL DrawBitmap(int x, int y, CString filename, int maskcolor);
	BOOL DrawBitmap(int x, int y, UINT nResID, int maskcolor);	
	BOOL DrawBitmap(int x, int y, CBitmap* bitmap, int maskcolor);
	BOOL DrawIndexedBitmap(int x, int y, CBitmap* bitmap, int maskcolor, int width, int height, int offset);

	//void DrawChildFrame(CRect winRc);  //vc2019
	void DrawChildFrame(CRect winRc, bool bActive = true);
};

HRGN DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct);


#endif // __GDIHELPER_H__
