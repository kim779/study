// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ProgressDlg.h"
#include "axmsg.hxx"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/, CString root)
	: CDialog(CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
	m_current = _T("");
	//}}AFX_DATA_INIT

	m_root    = root;
	m_bitmap = (HBITMAP) NULL;
}

CProgressDlg::~CProgressDlg()
{
	if (m_bitmap)	DeleteObject(m_bitmap);
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_TOTPROGRESS, m_totalCtrl);
	DDX_Control(pDX, IDC_CURPROGRESS, m_currCtrl);
	DDX_Control(pDX, IDC_PROGRESS, m_static);
	DDX_Text(pDX, IDC_FILE, m_current);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDCANCEL, &CProgressDlg::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CProgressDlg::OnBnClickedBtnExit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_totalCtrl.SetRange(0, 100);
	m_currCtrl.SetRange(0, 100);
	m_currCtrl.SetStep(1);

	m_first   = true;
	m_szTotal = 0.0;
	m_szCurr  = 0.0;
	m_nTotal  = 0;
	m_nCurr   = 0;
	m_rate    = 0;
	
	CString	imgN;
	imgN.Format("%s\\%s\\BG-download.bmp", m_root, IMAGEDIR);
	m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, imgN,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	//m_bitmap = (HBITMAP) LoadImage(NULL, imgN, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_bitmap)
	{
		HRGN hWndRgn = DIBToRgn(m_bitmap,RGB(255, 0, 255), FALSE);
		if(hWndRgn)
		{
			SetWindowRgn(hWndRgn,TRUE);
			::DeleteObject(hWndRgn);
		}
	}

	initButtons();

	CenterWindow(GetDesktopWindow());
	
	m_static.SetBitmap(m_bitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnCancel() 
{
	GetOwner()->PostMessage(WM_STOP, 0, 0);
	EndDialog(IDCANCEL);
}

void CProgressDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if (bShow)	SetForegroundWindow();
}

void CProgressDlg::SetTotal(CString total)
{
	CString	tmps;
	int	pos;

	m_first = false;
	pos  = total.Find('\t');
	tmps = total.Left(pos);
	m_nTotal = atoi(tmps);
	m_nCurr  = 0;

	tmps = total.Mid(pos+1);
	m_szTotal = atof(tmps);
	m_szCurr  = 0.0;
	tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
	tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
	m_totalCtrl.SetPos(0);
}

void CProgressDlg::SetProgress(CString detail, int rate, BOOL error)
{
	if (error)
	{
		m_errMsg.SetText(detail);
		return;
	}

	CString tmps;
	if (detail.IsEmpty())
	{
		m_currCtrl.SetPos(100);
		if (rate >= 100 || m_nTotal > 0)
		{
			m_nCurr++;
			tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
			tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
			m_current = detail;
		}
		return;
	}

	if (m_szTotal <= 0)	return;
	int	pos, value;
	double	valuex;

	valuex = 0.0;
	pos    = detail.Find('\t');
	m_current = m_bnFILENAME.GetText();
	if (pos > 0 && detail.GetLength() > 0)
	{
		tmps   = detail.Mid(pos+1);
		detail = detail.Left(pos);
		if (m_current.CompareNoCase(detail) || (!m_current.CompareNoCase(detail) && rate != 100))
		{
			valuex    = atof(tmps);
			m_szCurr += valuex;
			value     = (int)((m_szCurr*100.0)/m_szTotal);

			m_totalCtrl.SetPos(value);

			tmps.Format("( %.f / %.f )", m_szCurr, m_szTotal);
			m_bnTOTSIZE.SetText(tmps);
		}
	}

	m_bnFILENAME.SetText(detail);
	
	if (rate < m_rate)	m_currCtrl.OffsetPos(m_rate-rate);
	else			m_currCtrl.SetPos(m_rate);
	

	if (rate >= 100)
	{
		m_nCurr++;
		tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
		tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
		m_current = detail;
	}
	
	m_rate = rate;
}

void CProgressDlg::visibleButton()
{
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
}

void CProgressDlg::initButtons()
{
	CWnd*	item;
	CRect	wRc;
	CString	file, file_, file2, file2_;
	int	idcX[] = { IDCANCEL, IDC_TOTALSIZE, IDC_TRANSENDFILE, IDC_TOTALFILE, IDC_FILE , IDC_ERRMSG, IDC_BTN_EXIT};

	//file.Format("%s/%s/PROGRESS.bmp", m_root, IMAGEDIR);
	//file_.Format("%s/%s/PROGRESS_.bmp", m_root, IMAGEDIR);
	file.Format("%s\\%s\\BG-download.bmp", m_root, IMAGEDIR);
	file_.Format("%s\\%s\\BG-download.bmp", m_root, IMAGEDIR);
	for (int ii = 0; ii < sizeof(idcX) / sizeof(int); ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&wRc);
		ScreenToClient(&wRc);
		switch (idcX[ii])
		{
		case IDCANCEL:
			VERIFY(m_bnCancel.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TOTALSIZE:
			VERIFY(m_bnTOTSIZE.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TRANSENDFILE:
			VERIFY(m_bnENDCNT.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TOTALFILE:
			VERIFY(m_bnFILECNT.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_FILE:
			VERIFY(m_bnFILENAME.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_ERRMSG:
			{ 
				VERIFY(m_errMsg.Attach(idcX[ii], this, wRc, file, file, file));   
			}
			break;
		case IDC_BTN_EXIT:
			{ 
				VERIFY(m_btnexit.Attach(idcX[ii], this, wRc, file, file, file));   
			}
			break;
		}
	}

	m_bnCancel.SetAlign(DT_CENTER);
	m_bnTOTSIZE.SetAlign(DT_RIGHT);
	m_bnENDCNT.SetAlign(DT_CENTER);
	m_bnFILECNT.SetAlign(DT_CENTER);
	m_bnFILENAME.SetAlign(DT_CENTER);
	m_errMsg.SetAlign(DT_CENTER);
	m_errMsg.SetEBcolor(RGB(42, 46, 53));
	m_btnexit.SetEBcolor(RGB(42, 46, 53));
	

	file.Format("%s/%s/DOWNPROGRESS.bmp", m_root, IMAGEDIR);
	file_.Format("%s/%s/DOWNPROGRESS_.bmp", m_root, IMAGEDIR);

	file2.Format("%s/%s/DOWNPROGRESS2.bmp", m_root, IMAGEDIR);
	file2_.Format("%s/%s/DOWNPROGRESS2_.bmp", m_root, IMAGEDIR);

	m_totalCtrl.SetImage(file2, file2_);
	m_currCtrl.SetImage(file, file_);

	m_errMsg.SetTextColor(RGB(255,255,255));
	m_errMsg.SetText("컴포넌트 수신중...");
	
	if(m_currCtrl.GetSafeHwnd())
	{
		CRect rc;
		m_currCtrl.GetWindowRect(&rc);
		rc.DeflateRect(0,2);
		ScreenToClient(rc);
	//	rc.OffsetRect(-5,0);
	//	rc.right += 5;
		rc.left -= 3;
		m_currCtrl.MoveWindow(rc);
		rc.OffsetRect(0, 33);
		m_totalCtrl.MoveWindow(rc);
	}
}

HRGN CProgressDlg::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
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
						lr = GetRValue(BkColor);
						lg = GetGValue(BkColor);
						lb = GetBValue(BkColor);
						hr = min(0xff, lr + GetRValue(Tolerance));
						hg = min(0xff, lg + GetGValue(Tolerance));
						hb = min(0xff, lb + GetBValue(Tolerance));
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

void CProgressDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	//GetOwner()->PostMessage(WM_STOP, 9999, 0);
}

BOOL CProgressDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}


void CProgressDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CProgressDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//test
	GetOwner()->PostMessage(WM_STOP, 9999, 0);
	CDialog::OnCancel();
}


BOOL CProgressDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(42,46,53));
	return true;
	return CDialog::OnEraseBkgnd(pDC);
}


void CProgressDlg::WriteMsg(CString strdata, CRect rc , COLORREF col)
{
	
}

void CProgressDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
}


void CProgressDlg::OnBnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
