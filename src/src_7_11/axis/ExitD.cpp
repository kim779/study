// ExitD.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ExitD.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitD dialog

CExitD::CExitD(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CExitD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_home    = home;
	m_bitmap  = (HBITMAP) NULL;
}

CExitD::~CExitD()
{
	m_font.DeleteObject();
	if (m_bitmap)
		DeleteObject(m_bitmap);
}

void CExitD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitD)
	DDX_Control(pDX, IDC_BITMAP, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitD, CDialog)
	//{{AFX_MSG_MAP(CExitD)
	ON_BN_CLICKED(IDOKX, OnOkx)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CExitD::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitD message handlers
BOOL CExitD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	fontName, hourS, minuteS, secondS;
	fontName.LoadString(ST_FONTNAME_DEFAULT2);
	m_font.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, fontName);
	
	CString	path, path_;
	path.Format("%s\\%s\\접속종료.bmp", m_home, IMAGEDIR);
	path_.Format("%s\\%s\\접속종료_dn.bmp", m_home, IMAGEDIR);
	m_bitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_bitmap)
	{
		HRGN hWndRgn = DIBToRgn(m_bitmap,RGB(255, 0, 255),FALSE);
		if(hWndRgn)
		{
			SetWindowRgn(hWndRgn,TRUE);
			::DeleteObject(hWndRgn);
		}
	}
	else	visibleButton();

	if (!m_bitmap)	visibleButton();

	initButtons();
	m_static.SetBitmap(m_bitmap);

	SYSTEMTIME	time;
	CString		text, userName;
	GetLocalTime(&m_etime);

	CWinApp* app = AfxGetApp();
	userName = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = userName.Find('|');

//	if (pos < 0)	
//		userName = _T("Anonymous");
//	else		
		userName = userName.Mid(pos+1);
		userName.TrimLeft();
		userName.TrimRight();
	//if (userName.IsEmpty())	
	//	userName = _T("Anonymous");

	m_bnUSER.SetText(userName + _T(" 고객님"));
	m_bnUSER.SetAlign(DT_LEFT);

	m_bnTEXT.SetText(_T("K-Bond를 이용해 주셔서 감사합니다"));
	m_bnTEXT.SetAlign(DT_LEFT);

	text.Format("%04d시 %02d월 %02d일   %02d시 %02d분 %02d초", m_stime.wYear, m_stime.wMonth, m_stime.wDay, m_stime.wHour, m_stime.wMinute, m_stime.wSecond);

	m_bnSTIME.SetText(text);
	m_bnSTIME.SetAlign(DT_LEFT);

	text.Format("%04d시 %02d월 %02d일   %02d시 %02d분 %02d초", m_etime.wYear, m_etime.wMonth, m_etime.wDay, m_etime.wHour, m_etime.wMinute, m_etime.wSecond);
	m_bnETIME.SetText(text);
	m_bnETIME.SetAlign(DT_LEFT);

	time = GetUseTime();
	text.Format("%d일 %2d시간 %2d분 %2d초", time.wDay, time.wHour, time.wMinute, time.wSecond);
	m_bnTIME.SetText(text);
	m_bnTIME.SetAlign(DT_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExitD::visibleButton()
{
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);

	GetDlgItem(IDOK)->ModifyStyle(BS_OWNERDRAW, 0);
	GetDlgItem(IDCANCEL)->ModifyStyle(BS_OWNERDRAW, 0);
}

void CExitD::initButtons()
{
	CWnd*	item;
	CRect	wRc;
	CString	path, path_, path__;
	int	idcX[] = { IDC_USERNAME, IDC_STARTTIME, IDC_ENDTIME, IDC_USETIME, IDOK, IDCANCEL , IDC_BTN_TEXT};

	path.Format("%s/%s/접속종료.bmp",  m_home, IMAGEDIR);
	path_.Format("%s/%s/접속종료_dn.bmp", m_home, IMAGEDIR);
	path__.Format("%s/%s/접속종료_en.bmp", m_home, IMAGEDIR);
	for (int ii = 0; ii < sizeof(idcX) / sizeof(int); ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&wRc);
		ScreenToClient(&wRc);
		switch (idcX[ii])
		{
		case IDC_USERNAME:	VERIFY(m_bnUSER.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_STARTTIME:	VERIFY(m_bnSTIME.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_ENDTIME:	VERIFY(m_bnETIME.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_USETIME:	VERIFY(m_bnTIME.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDOK:			VERIFY(m_bnOK.Attach(idcX[ii], this, wRc, path, path_, path__));		break;
		case IDCANCEL:		VERIFY(m_bnCANCEL.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BTN_TEXT:		VERIFY(m_bnTEXT.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		}
	}

	m_bnUSER.SetTextColor(RGB(255, 255, 255));
	m_bnTEXT.SetTextColor(RGB(255, 255, 255));
	m_bnSTIME.SetTextColor(RGB(255, 255, 255));
	m_bnETIME.SetTextColor(RGB(255, 255, 255));
	m_bnTIME.SetTextColor(RGB(255, 255, 255));

	m_bnUSER.SetFont(&m_font);
	m_bnTEXT.SetFont(&m_font);
	m_bnSTIME.SetFont(&m_font);
	m_bnETIME.SetFont(&m_font);
	m_bnTIME.SetFont(&m_font);

	m_bnUSER.SetEBcolor(RGB(37,41,47));
	m_bnTEXT.SetEBcolor(RGB(37,41,47));
	m_bnSTIME.SetEBcolor(RGB(45,49,56));
	m_bnETIME.SetEBcolor(RGB(50,55,62));
	m_bnTIME.SetEBcolor(RGB(57,62,69));
}

void CExitD::OnOkx() 
{
	CDialog::OnOK();	
}

SYSTEMTIME CExitD::GetUseTime()
{
	SYSTEMTIME time;
	int	day, hour, minute, second;

	day = m_etime.wDay - m_stime.wDay;
	hour = m_etime.wHour - m_stime.wHour;
	minute = m_etime.wMinute - m_stime.wMinute;
	second = m_etime.wSecond - m_stime.wSecond;

	if (second < 0)
	{
		second += 60;
		minute--;
	}
	if (minute < 0)
	{
		minute += 60;
		hour--;
	}
	if (hour < 0)
	{
		hour += 24;
		day--;
	}
	if (day > 0)
		hour += day*24;
	time.wDay = day;
	time.wHour = hour;
	time.wMinute = minute;
	time.wSecond = second;

	return time;
}

HRGN CExitD::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
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

void CExitD::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

BOOL CExitD::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)	
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		case VK_SPACE:
			{
				CWnd* ctrlOK = GetDlgItem(IDOK);
				CWnd* ctrlNO = GetDlgItem(IDCANCEL);
				CWnd* focus  = GetFocus();

				if (ctrlOK == focus)
				{
					CDialog::OnOK();
					return TRUE;
				}

				if (ctrlNO == focus)
				{
					CDialog::OnCancel();
					return TRUE;
				}
			}
		default:	break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CExitD::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}
