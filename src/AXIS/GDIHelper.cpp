// GDIHelper.cpp
//
#include "stdafx.h"
#include "GDIHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/* CAxBufferDC                                                          */
/************************************************************************/
CAxBufferDC::CAxBufferDC(CDC& dc, CRect& rc)
: CDC(), m_bmp()
{
	Init(dc, rc.Width(), rc.Height());
}

CAxBufferDC::CAxBufferDC(CDC& dc, int cx, int cy)
: CDC(), m_bmp()
{
	Init(dc, cx, cy);
}

void CAxBufferDC::Init(CDC& dc, int cx, int cy)
{
	CreateCompatibleDC(&dc);
	m_bmp.CreateCompatibleBitmap(&dc, cx, cy);
	m_oldBmp = SelectObject(&m_bmp);
}

CAxBufferDC::CAxBufferDC(CDC& dc, int cx, int cy, UINT nPlanes, UINT nBitCount, LPVOID lpBits)
: CDC(), m_bmp()
{
	CreateCompatibleDC(&dc);

	m_bmp.CreateBitmap(cx, cy, nPlanes, nBitCount, lpBits);
	m_oldBmp = SelectObject(&m_bmp);
}

CAxBufferDC::CAxBufferDC(CDC& dc, CBitmap* bmp, int cx, int cy)
: CDC(), m_bmp()
{
	CreateCompatibleDC(&dc);

	if (bmp)
	{
		BITMAP bm;
		bmp->GetBitmap(&bm);
		m_oldBmp = SelectObject(bmp);
	}
	else
		m_oldBmp = NULL;
}

CAxBufferDC::~CAxBufferDC()
{
	if (m_oldBmp)
		SelectObject(m_oldBmp);
}

/************************************************************************/
/* CAxDrawHelper                                                        */
/************************************************************************/
CAxDrawHelper::CAxDrawHelper(CDC* pDC)
{
	m_dc = pDC;
}

CAxDrawHelper::~CAxDrawHelper()
{

}

void CAxDrawHelper::DrawGradient(CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = m_dc->SelectObject(&cPen);
			
			m_dc->MoveTo(drawRC.left, drawRC.top);
			m_dc->LineTo(drawRC.right, drawRC.bottom);
			m_dc->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = m_dc->SelectObject(&cPen);

				m_dc->MoveTo(drawRC.left + ii, drawRC.top);
				m_dc->LineTo(drawRC.left + ii + 1, drawRC.top);
				m_dc->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				m_dc->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = m_dc->SelectObject(&cPen);
			
			m_dc->MoveTo(drawRC.left, drawRC.top);
			m_dc->LineTo(drawRC.right, drawRC.bottom);
			m_dc->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = m_dc->SelectObject(&cPen);

				m_dc->MoveTo(drawRC.left, drawRC.top + ii);
				m_dc->LineTo(drawRC.left, drawRC.top + ii + 1);
				m_dc->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				m_dc->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

BOOL CAxDrawHelper::DrawBitmap(int x, int y, CString filename, int maskcolor)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),filename,
	          IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if( hBitmap == NULL )	return FALSE;

	CBitmap bitmap;
	bitmap.Attach(hBitmap);

	DrawBitmap(x, y, &bitmap, maskcolor);

	bitmap.Detach();
	::DeleteObject(hBitmap);

	return TRUE;
}

BOOL CAxDrawHelper::DrawBitmap(int x, int y, UINT nResID, int maskcolor)
{
	CBitmap	bitmap;
	if (!bitmap.LoadBitmap(nResID))
		return FALSE;

	return DrawBitmap(x, y, &bitmap, maskcolor);
}

BOOL CAxDrawHelper::DrawBitmap(int x, int y, CBitmap* bitmap, int maskcolor)
{
	if (!bitmap || !bitmap->m_hObject)
		return FALSE;

	BITMAP  bm;   
	bitmap->GetBitmap(&bm);
	return DrawIndexedBitmap(x, y, bitmap, maskcolor, bm.bmWidth, bm.bmHeight, 0);
}

BOOL CAxDrawHelper::DrawIndexedBitmap(int x, int y, CBitmap* bitmap, int maskcolor, int width, int height, int offset)
{
	if (!bitmap || !bitmap->m_hObject)
		return FALSE;

	CDC			buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap* oldbuffBitmap{}, * oldmaskBitmap{}, * oldmemoryBitmap{}, * oldcopyBitmap{};

	buffDC.CreateCompatibleDC(m_dc);
	buffBitmap.CreateCompatibleBitmap(m_dc, width, height);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(m_dc);
	maskBitmap.CreateBitmap(width, height, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	const CRect		maskRc( 0, 0, width, height);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(m_dc);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, width, height, &memoryDC, offset, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, width, height, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(m_dc);
	copyBitmap.CreateCompatibleBitmap(m_dc, width, height);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, width, height, m_dc,  x, y, SRCCOPY);
	copyDC.BitBlt( 0, 0, width, height, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, width, height, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, width, height, &buffDC, 0, 0, SRCPAINT);
	m_dc->BitBlt(x, y, width, height, &copyDC, 0, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	
	return TRUE;
}

//void CAxDrawHelper::DrawChildFrame(CRect winRc)  //vc2019
void CAxDrawHelper::DrawChildFrame(CRect winRc, bool bActive)
{
	CString slog;
	COLORREF colRect{};
	slog.Format("[childframe] bActive=[%d]\n", bActive);
	OutputDebugString(slog);
	if (bActive)
	{
		colRect = Axis::GetColor(64);  
	}
	else
	{
		colRect = RGB(153, 153, 153);  
	}

	winRc -= winRc.TopLeft();
	CRect rc(winRc);

	//rc.top += 20;//CAPTION_HEIGHT;
	/*  //vc2019
	rc.bottom -= 2;
	rc.left++;
	rc.right -= 2;
	*/
	rc.bottom -= 1;
	rc.right -= 1;
	/*
	CBrush brush;
	brush.CreateSolidBrush(Axis::GetColor(64));
	m_dc->FillRect(&rc, &brush);
	*/

	//for (int i = 2; i <= 3; i++)  //vc2019

	for (int i = 0; i <= 5; i++)
	{
		CPen pen(PS_SOLID, 1, colRect);//i + 1));
		const CPen* oldPen = m_dc->SelectObject(&pen);

		m_dc->MoveTo(rc.left, rc.top);
		m_dc->LineTo(rc.left, rc.bottom);
		m_dc->LineTo(rc.right, rc.bottom);
		m_dc->LineTo(rc.right, rc.top);

		rc.left++;
		rc.bottom--;
		rc.right--;
	}


	CPen pen;
	//pen.CreatePen(PS_SOLID, 1, Axis::GetColor(1));  //vc2019 원래 테두리를 어두운색으로 한픽셀 그려주려 했지만 그냥 푸른색으로 
	pen.CreatePen(PS_SOLID, 1, colRect);
	CPen* oldPen = m_dc->SelectObject(&pen);

	rc = winRc;
	m_dc->MoveTo(rc.left, rc.top);
	m_dc->LineTo(rc.left, rc.bottom - 1);
	m_dc->LineTo(rc.right - 1, rc.bottom - 1);
	m_dc->LineTo(rc.right - 1, rc.top);
	m_dc->LineTo(rc.left, rc.top);

	m_dc->SelectObject(oldPen);

	rc.bottom = rc.top + 5;
	m_dc->FillSolidRect(rc, colRect);  //vc2019 상단에 빈영역을 칠해주었다
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

HRGN DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
{
	// use to return the handle of the HGRN
  	HRGN hRgn = NULL;					
	#define MAX_ALLOC_RECTS  100
	//the difference of the color
	COLORREF  Tolerance=BkColor;//0x00101010;
	const int	r = GetRValue(0x111111);
	const int	g = GetGValue(0x111111);
	const int	b = GetBValue(0x111111);
	if (hBmp)
	{
		//creat the dib to save the dc
		HDC hMemDC = CreateCompatibleDC(NULL);		
		if (hMemDC)
		{
			BITMAP bm;
			//get the info of the bitmap
			GetObject(hBmp, sizeof(bm), &bm);	

			BITMAPINFOHEADER BmpInfoh = {					//the struct of the bitmap
					sizeof(BITMAPINFOHEADER),			// biSize
					bm.bmWidth,					// biWidth;
					bm.bmHeight,					// biHeight;
					1,						// biPlanes;
					32,						// biBitCount
					BI_RGB,						// biCompression;
					0,						// biSizeImage;
					0,						// biXPelsPerMeter;
					0,						// biYPelsPerMeter;
					0,						// biClrUsed;
					0						// biClrImportant;
			};
			//design a void point to point to the bitmap
			LPVOID pBit32{};
			//creat a DIB
			HBITMAP hDib32 = CreateDIBSection(hMemDC, 
					(BITMAPINFO *)&BmpInfoh, 
					DIB_RGB_COLORS, &pBit32, NULL, 0);
			if (hDib32)
			{
				//copy dib to DC
				HBITMAP hOldib32 = (HBITMAP)SelectObject(hMemDC, hDib32);
				// create a DC to save orgin bitmap
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					BITMAP bm32;
					// get the new 34 bit Dib size
					GetObject(hDib32, sizeof(bm32), &bm32);
					//make sure the 32Dib's every line pilex's is 4 's times
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;
					//copy the orginal dib to DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					//copy dib to memory DC
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					DWORD MaxRects = MAX_ALLOC_RECTS;
					SYSTEM_INFO  Sysinfo;
					//get memory size
					GetSystemInfo(&Sysinfo);
					//make a stack which can chang big
					//alloct memory
					HANDLE hRcData=HeapCreate(HEAP_GENERATE_EXCEPTIONS,Sysinfo.dwPageSize, 0);
					RGNDATA * pRcData=(RGNDATA*)HeapAlloc(hRcData,HEAP_ZERO_MEMORY,
						sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
					 //fill the the RGNDATA struck
					pRcData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pRcData->rdh.iType = RDH_RECTANGLES;
					pRcData->rdh.nCount = pRcData->rdh.nRgnSize = 0;
					SetRect(&pRcData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					BYTE hr{}, hg{}, hb{}, lr{}, lg{}, lb{};
					switch(BkColor)
					{
					case RGB(255,255,255):	//if the bkcolor is white
						hr = GetRValue(BkColor);
						hg = GetGValue(BkColor);
						hb = GetBValue(BkColor);
						lr = min(0xff, hr - GetRValue(Tolerance));
						lg = min(0xff, hg - GetGValue(Tolerance));
						lb = min(0xff, hb - GetBValue(Tolerance));
						break;
					case RGB(0,0,0):	//if the bkcolor is black
						lr = GetRValue(BkColor);
						lg = GetGValue(BkColor);
						lb = GetBValue(BkColor);
						hr = min(0xff, lr + GetRValue(Tolerance));
						hg = min(0xff, lg + GetGValue(Tolerance));
						hb = min(0xff, lb + GetBValue(Tolerance));
						break;
					default:		//if the bkcolor is other color
						Tolerance=0x111111;
						lr =max(0, GetRValue(BkColor)-GetRValue(Tolerance));
						lg = max(0,GetGValue(BkColor)-GetGValue(Tolerance));
						lb = max(0,GetBValue(BkColor)-GetBValue(Tolerance));
						hr=min(0xff,GetRValue(BkColor)+GetRValue(Tolerance));
						hg=min(0xff,GetGValue(BkColor)+GetGValue(Tolerance));
						hb=min(0xff,GetBValue(BkColor)+GetBValue(Tolerance));
						break;
					}
					// Get the bit point and do the search
					BYTE *pBits = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						for (int x = 0; x < bm.bmWidth; x++)
						{
							const int x0 = x;
							DWORD *pColor = (DWORD *)pBits + x;
							BYTE dr{}, dg{}, db{};
							while (x < bm.bmWidth)
							{
								dr=GetRValue(*pColor);
								dg=GetGValue(*pColor);
								db=GetBValue(*pColor);

								if ((dr>= lr && dr<= hr)&&(dg>=lg&&dg<=hg)&&(db>=lb&&db<=hb))
								{
									if(Direct)
										break;
									else
									{
										pColor++;
										x++;
									}
							  	}
								else if(Direct)
								{
									pColor++;
									x++;
								}
								else
									break;

							}
							if (x > x0)
							{
								if (pRcData->rdh.nCount >= MaxRects)
								{
									MaxRects += MAX_ALLOC_RECTS;
									//re alloc the stack
									pRcData=(RGNDATA*)HeapReAlloc(
									hRcData,HEAP_ZERO_MEMORY,pRcData, 
									sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
								}
								RECT *pr = (RECT *)&pRcData->Buffer;
								SetRect(&pr[pRcData->rdh.nCount], x0, y, x, y+1);
								pRcData->rdh.rcBound.left = x0;
								pRcData->rdh.rcBound.top = y;
								pRcData->rdh.rcBound.right = x;
								pRcData->rdh.rcBound.bottom = y+1;
								pRcData->rdh.nCount++;

								if (pRcData->rdh.nCount == 3000)
								{	
									HRGN tmphRgn = ExtCreateRegion(NULL,
									sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects),
									pRcData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
										DeleteObject(tmphRgn);
									}
									else
										hRgn = tmphRgn;
									pRcData->rdh.nCount = 0;
									SetRect(&pRcData->rdh.rcBound, 
									MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// search next line
						pBits -= bm32.bmWidthBytes;
					}
					HRGN tmphRgn = ExtCreateRegion(NULL, 
							sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects), pRcData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
						DeleteObject(tmphRgn);
					}
					else
						hRgn = tmphRgn;
					// make a rect ,use this rect xor to the  BkColor
					//then we can get the rect we want
					if(!Direct)
					{
						HRGN hRect=CreateRectRgn(0,0,bm.bmWidth,bm.bmHeight);
				                        if(hRect)
						{
							CombineRgn(hRgn,hRgn,hRect,RGN_XOR);
							DeleteObject(hRect);
						}
					    else
							return NULL;
					}
					//release the memory
					HeapFree(hRcData,HEAP_NO_SERIALIZE,pRcData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
					DeleteObject(holdBmp);
				}
				SelectObject(hMemDC,hOldib32);
				DeleteDC(hMemDC);
				DeleteObject(hOldib32);
				DeleteObject(hDib32);
			}
			else
				DeleteDC(hMemDC);
		}
	}
	return hRgn;
}
