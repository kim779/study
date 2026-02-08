// NClock.cpp : implementation file
//

#include "stdafx.h"
#include "PClock.h"
#include "MenuXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//±¼¸² 13pt color:35/246/253 ÀÔ´Ï´Ù
#define GAP		2
#define BKCOLOR		RGB(217, 217, 217)
#define TEXTCOLOR	RGB( 50,  50,  50)//RGB(35, 246, 253)
#define BORDERCOLOR	RGB(  0, 140,   0)

#define	TM_CLOCK	1235
/////////////////////////////////////////////////////////////////////////////
// CPClock
UINT DayOf[] = {
   LOCALE_SDAYNAME7,   // Sunday
   LOCALE_SDAYNAME1,   
   LOCALE_SDAYNAME2,
   LOCALE_SDAYNAME3,
   LOCALE_SDAYNAME4, 
   LOCALE_SDAYNAME5, 
   LOCALE_SDAYNAME6   // Saturday
};

CPClock::CPClock()
{
	CString	fontName;
	LOGFONT	lf;
	m_bSmall = TRUE;

	//fontName.LoadString(ST_TEXT_CLOCKFONT);
	fontName = "±¼¸²";
	m_font.CreatePointFont(100, "±¼¸²");

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
				(const char*)fontName);        // lpszFacename//*/
	return;
}

CPClock::~CPClock()
{
	m_bitmap.DeleteObject();
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CPClock, CWnd)
	//{{AFX_MSG_MAP(CPClock)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPClock message handlers

void CPClock::OnPaint() 
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

		memDC.SetBkColor(BKCOLOR);
		memDC.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
		memDC.Draw3dRect(cRc, BORDERCOLOR, BORDERCOLOR);
		DrawClock(&memDC);
		
		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
}

void CPClock::DrawClock(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);

	CFont*	font = pDC->SelectObject(&m_font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(TEXTCOLOR);

	SYSTEMTIME	tm;
	CString		timeS, sign = _T("AM");
	TCHAR		strWeekday[256];

	GetLocalTime(&tm);
	::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
			DayOf[tm.wDayOfWeek],   // Get day of week from CTime
			strWeekday, sizeof(strWeekday));

	int	hour = tm.wHour;
	if (hour > 12)
	{
		hour -= 12;
		sign = _T("PM");
	}
	if (hour == 12 && (tm.wMinute > 0 || tm.wSecond > 0))
		sign = _T("PM");
	
	if (!hour)	hour = 12;
	
	timeS.Format("%s %2d:%02d:%02d", sign, hour, tm.wMinute, tm.wSecond);

	pDC->DrawText(timeS, cRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

	pDC->SelectObject(font);
}

void CPClock::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_CLOCK)
		Invalidate(FALSE);
	CWnd::OnTimer(nIDEvent);
}

void CPClock::Init()
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

void CPClock::OnClose() 
{
	ShowWindow(SW_HIDE);
	return;
	CWnd::OnClose();
}

void CPClock::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenuXP	menu;
	CPoint	lp(point);

	menu.CreatePopupMenu();
	ClientToScreen(&lp);
	menu.AppendMenuX(MF_STRING, 9999, "½Ã°è °¨Ãß±â");
	int cmd = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN | TPM_LEFTBUTTON, lp.x, lp.y, this);
	if (cmd == 9999)	ShowWindow(SW_HIDE);
	CWnd::OnRButtonDown(nFlags, point);
}

int CPClock::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString imgN;
	imgN.Format("%s\\image\\timer.bmp", Axis::home);
	HBITMAP bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (bitmap)
	{
		m_bitmap.Attach(bitmap);
		/*HRGN hWndRgn = DIBToRgn(bitmap, Axis::maskColor, FALSE);
		if(hWndRgn)
		{
			SetWindowRgn(hWndRgn,TRUE);
			::DeleteObject(hWndRgn);
		}*/
	}
	return 0;
}

void CPClock::DrawImg(CDC* pDC)
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

HRGN CPClock::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
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

