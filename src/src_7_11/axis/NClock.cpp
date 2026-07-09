// NClock.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NClock.h"
#include "../dll/axissm/MenuXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GAP		2
#define BKCOLOR		RGB(11, 60, 11)
#define DATECOLOR	RGB(255, 255, 255)
#define TIMECOLOR	RGB(241, 239,   0)
#define BORDERCOLOR	RGB(  0, 140,   0)

#define TM_CLOCK	1234
/////////////////////////////////////////////////////////////////////////////
// CNClock
UINT DayOf[] = {
   LOCALE_SDAYNAME7,   // Sunday
   LOCALE_SDAYNAME1,   
   LOCALE_SDAYNAME2,
   LOCALE_SDAYNAME3,
   LOCALE_SDAYNAME4, 
   LOCALE_SDAYNAME5, 
   LOCALE_SDAYNAME6   // Saturday
};

CNClock::CNClock(CString home)
{
	m_home	= home;
	CString	fontName;
	LOGFONT	lf;
	m_bSmall = FALSE;
	
	//fontName.LoadString(ST_TEXT_CLOCKFONT);
	fontName = "±¼¸²";
	m_font.CreatePointFont(120, fontName);

	m_font.GetLogFont(&lf);
	m_font.DeleteObject();

	m_font.CreateFont(	lf.lfHeight,			// nHeight
				lf.lfWidth,			// nWidth
				lf.lfEscapement,		// nEscapement
				lf.lfOrientation,		// nOrientation
				FW_SEMIBOLD,			// nWeight
				lf.lfItalic,			// bItalic
				lf.lfUnderline,			// bUnderline
				lf.lfStrikeOut,			// cStrikeOut
				lf.lfCharSet,			// nCharSet
				lf.lfOutPrecision,		// nOutPrecision
				lf.lfClipPrecision,		// nClipPrecision
				lf.lfQuality,			// nQuality
				lf.lfPitchAndFamily,		// nPitchAndFamily
				(char *) fontName.operator LPCTSTR());        // lpszFacename//*/

	m_font2.CreatePointFont(80, fontName);

	m_font2.GetLogFont(&lf);
	m_font2.DeleteObject();
	m_font2.CreateFont(	lf.lfHeight,			// nHeight
				lf.lfWidth,			// nWidth
				lf.lfEscapement,		// nEscapement
				lf.lfOrientation,		// nOrientation
				FW_SEMIBOLD,			// nWeight
				lf.lfItalic,			// bItalic
				lf.lfUnderline,			// bUnderline
				lf.lfStrikeOut,			// cStrikeOut
				lf.lfCharSet,			// nCharSet
				lf.lfOutPrecision,		// nOutPrecision
				lf.lfClipPrecision,		// nClipPrecision
				lf.lfQuality,			// nQuality
				lf.lfPitchAndFamily,		// nPitchAndFamily
				(char *) fontName.operator LPCTSTR());        // lpszFacename//*/
}

CNClock::~CNClock()
{
	m_bitmap.DeleteObject();
	m_font.DeleteObject();
	m_font2.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNClock, CWnd)
	//{{AFX_MSG_MAP(CNClock)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNClock message handlers

void CNClock::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	cRc;
	GetClientRect(cRc);

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(&dc))
	{
		dc.SetBkColor(BKCOLOR);
		dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
		dc.Draw3dRect(cRc, BORDERCOLOR, BORDERCOLOR);
		DrawClock(&dc);
	}
	else
	{
		CBitmap	bmp, *pbmp;
		bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		pbmp = (CBitmap *) memDC.SelectObject(&bmp);

		if (m_bitmap.GetSafeHandle())
		{
			m_gap = GAP*2;
			DrawImg(&memDC);
		}
		else
		{
			m_gap = GAP;
			memDC.FillSolidRect(cRc, BKCOLOR);
			memDC.Draw3dRect(cRc, BORDERCOLOR, BORDERCOLOR);
		}

		DrawClock(&memDC);
		
		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
}

void CNClock::DrawClock(CDC* pDC)
{
	CRect	cRc, dRc;
	GetClientRect(cRc);

	CFont	dfont;
	dfont.CreatePointFont(100, "±¼¸²Ã¼");
	CFont*	font = pDC->SelectObject(&dfont);

	pDC->SetBkMode(TRANSPARENT);
	COLORREF color = pDC->SetTextColor(DATECOLOR);

	SYSTEMTIME	tm;
	CString		timeS, sign = _T("AM");
	TCHAR		strWeekday[256];

	GetLocalTime(&tm);
	::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
			DayOf[tm.wDayOfWeek],   // Get day of week from CTime
			strWeekday, sizeof(strWeekday));

	timeS.Format("%04d/%02d/%02d(%s)", tm.wYear, tm.wMonth, tm.wDay, CString(strWeekday).Left(2));
	dRc.SetRect(cRc.left, cRc.top, cRc.right, cRc.Height()/2);
	dRc.OffsetRect(17, m_gap);
	pDC->DrawText(timeS, dRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	int	hour = tm.wHour;
	if (hour > 12)
	{
		hour -= 12;
		sign = _T("PM");
	}
	
	if (!hour)	hour = 12;
	timeS.Format("%2d:%02d:%02d", hour, tm.wMinute, tm.wSecond);

	dRc.SetRect(cRc.left, dRc.bottom, cRc.right, cRc.bottom);
	dRc.OffsetRect(73, -m_gap);
	pDC->SetTextColor(TIMECOLOR);
	pDC->SelectObject(&m_font);
	pDC->DrawText(timeS, dRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE);

	dRc.SetRect(cRc.left, dRc.top, cRc.right, dRc.bottom);
	dRc.OffsetRect(48, 2);
	pDC->SelectObject(&m_font2);
	pDC->DrawText(sign, dRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE);

	pDC->SelectObject(font);
	pDC->SetTextColor(color);
}

void CNClock::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CWnd::OnLButtonDown(nFlags, point);
}

void CNClock::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == TM_CLOCK)
		Invalidate(FALSE);
	CWnd::OnTimer(nIDEvent);
}

void CNClock::Init()
{
	if (m_bitmap.GetSafeHandle())
	{
		BITMAP	bm;
		CRect	wRc;

		m_bitmap.GetBitmap(&bm);
		GetDesktopWindow()->GetWindowRect(&wRc);
		SetWindowPos(&wndTopMost, wRc.right - bm.bmWidth, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE|SWP_HIDEWINDOW);
	}
	else
	{
		CDC*	pDC = GetDC();
		CFont*	font = pDC->SelectObject(&m_font);

		SYSTEMTIME	tm;
		CString		timeS;
		TCHAR		strWeekday[256];

		GetLocalTime(&tm);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
				DayOf[tm.wDayOfWeek],   // Get day of week from CTime
				strWeekday, sizeof(strWeekday));

		CSize	size, size2;
		timeS.Format("%04d/%d/%02d[%s]", tm.wYear, tm.wMonth, tm.wDay, CString(strWeekday).Left(2));
		size = pDC->GetTextExtent(timeS);
		timeS.Format("PM %2d:%02d:%02d", tm.wHour, tm.wMinute, tm.wSecond);
		size2 = pDC->GetTextExtent(timeS);
		pDC->SelectObject(font);

		if (size.cx < size2.cx)	size.cx = size2.cx;
		if (size.cy < size2.cy)	size.cy = size2.cy;

		size.cx += int(size.cx*0.2);
		size.cy += int(size.cy*1.4);
		SetWindowPos(&wndTopMost, 0, 0, size.cx, size.cy, SWP_NOMOVE|SWP_HIDEWINDOW);
		ReleaseDC(pDC);
	}

	SetTimer(TM_CLOCK, 1000, NULL);
}

void CNClock::OnClose() 
{
	ShowWindow(SW_HIDE);
	return;
	CWnd::OnClose();
}

void CNClock::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenuXP	menu;
	CString	string;
	CPoint	lp(point);

	string.LoadString(ST_TEXT_HIDECLOCK);
	menu.CreatePopupMenu();
	ClientToScreen(&lp);
	menu.AppendMenuX(MF_STRING, 9999, string);
	if (m_bSmall)
		string.LoadString(ST_TEXT_NORMCLOCK);
	else
		string.LoadString(ST_TEXT_MINICLOCK);
	menu.AppendMenuX(MF_STRING, 10000, string);	
	int cmd = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN | TPM_LEFTBUTTON, lp.x, lp.y, this);
	if (cmd == 9999)	ShowWindow(SW_HIDE);
	if (cmd == 10000)	SetMini(!m_bSmall);
	CWnd::OnRButtonDown(nFlags, point);
}

int CNClock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString imgN;
	imgN.Format("%s\\image\\timer.bmp", m_home);
	HBITMAP bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (bitmap)
	{
		m_bitmap.Attach(bitmap);
		HRGN hWndRgn = DIBToRgn(bitmap, RGB(255, 0, 255), FALSE);
		if(hWndRgn)
		{
			SetWindowRgn(hWndRgn,TRUE);
			::DeleteObject(hWndRgn);
		}
	}

	return 0;
}

void CNClock::DrawImg(CDC* pDC)
{
	CRect	bRC;
	GetClientRect(bRC);

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return;

	BITMAP	bm;
	m_bitmap.GetBitmap(&bm);
	CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bitmap);

	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldbuffBitmap);
}

HRGN CNClock::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
{
	// use to return the handle of the HGRN
  	HRGN hRgn = NULL;					
	#define MAX_ALLOC_RECTS  100
	//the difference of the color
	COLORREF  Tolerance=BkColor;//0x00101010;
	int	r = GetRValue(0x111111);
	int	g = GetGValue(0x111111);
	int	b = GetBValue(0x111111);
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
			LPVOID pBit32; 
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
		             		BYTE hr,hg,hb,lr,lg,lb;
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
							int x0 = x;
							DWORD *pColor = (DWORD *)pBits + x;
							BYTE dr,dg,db;
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

void CNClock::SetMini(BOOL bMini)
{
	m_bSmall = bMini;
	if (!bMini)
		SetWindowRgn(NULL, TRUE);
	else
	{
		CRect	rect;
		GetClientRect(rect);
		int	nHeight = rect.Height();
		CRgn	rgn;
	
		rect.DeflateRect(40, 22, 15, 4);	
		rgn.CreateRectRgnIndirect(rect);
		SetWindowRgn(rgn, TRUE);
		rgn.DeleteObject();
	}
}