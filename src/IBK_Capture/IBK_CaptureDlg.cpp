// IBK_CaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IBK_Capture.h"
#include "IBK_CaptureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_START 9898


#include <math.h>
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBK_CaptureDlg dialog
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

CIBK_CaptureDlg::CIBK_CaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIBK_CaptureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIBK_CaptureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIBK_CaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIBK_CaptureDlg)
	DDX_Control(pDX, IDC_CB_TIME, m_cbTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIBK_CaptureDlg, CDialog)
	//{{AFX_MSG_MAP(CIBK_CaptureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBtnFind)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_JPEG, OnBtnJpeg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIBK_CaptureDlg message handlers

BOOL CIBK_CaptureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
//	((CWnd*)GetDlgItem(IDC_EDIT_PATH))->SetWindowText(fileDlg.GetPathName());

	CString temp;
	CString strpath;
	CTime	time = CTime::GetCurrentTime();

//	temp.Format("\\\\172.17.1.177\\정보전략팀\\Public\\37_IT개발팀\\09. HTS\\서버모니터링\\%04d", time.GetYear());
//	temp.Format("\\\\172.16.213.33\\정보전략팀\\Public\\37_IT개발팀\\09. HTS\\서버모니터링\\2023\\Temp");  
	temp.Format("\\\\172.16.213.33\\정보전략팀\\Public\\37_IT개발팀\\09. HTS\\서버모니터링\\%04d\\Temp", time.GetYear());  
//	temp.Format("D:\\capture\\%04d\\Temp", time.GetYear());

//	if(IsWow64() == "64")
//		strpath.Format(temp);
//	else
	strpath.Format(temp);

	((CWnd*)GetDlgItem(IDC_EDIT_PATH))->SetWindowText(strpath);

	m_cbTime.AddString("1분");
	m_cbTime.AddString("5분");
	m_cbTime.AddString("10분");
	m_cbTime.AddString("15분");

	m_cbTime.SetCurSel(1);

//	int inerval =  ConvertSecondTime(time.GetYear(),time.GetMonth(),time.GetDay(), 8, 30, 0) - 
//		ConvertSecondTime(time.GetYear(),time.GetMonth(),time.GetDay(), time.GetHour() ,time.GetMinute(), time.GetSecond());

//	if(inerval > 0 )
//	SetTimer(TM_START, 1000 * inerval , NULL);

//	SetTimer(9999, 1000, NULL);

	CFile		Dfile;
	CString		filename;
	((CWnd*)GetDlgItem(IDC_EDIT_PATH))->GetWindowText(strpath);
	CFileFind ff;

	if (!ff.FindFile(strpath))
	{
		CString slog;
		bool ret = ::CreateDirectory(strpath, NULL);
		slog.Format("oninitdialog %d err=[%d]  [%s]", ret, GetLastError(), strpath);
	}

	if(!IsToDayHoliDay())
		OnBtnStart();
//	((CButton*)GetDlgItem(IDC_BTN_START))->SendMessage(WM_LBUTTONDOWN, 0, 0);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

time_t CIBK_CaptureDlg::ConvertSecondTime(int parm_year, int parm_month, int parm_day,int parm_hour, int parm_minute, int parm_second)
{
    struct tm base_time;
	
    base_time.tm_sec = parm_second;
    base_time.tm_min = parm_minute;
    base_time.tm_hour = parm_hour;
    base_time.tm_mday = parm_day;
    base_time.tm_mon = parm_month - 1;
	
    // 1900년 부터 지정시간까지의 초단위로 계산하므로..
    base_time.tm_year = parm_year - 1900;
    base_time.tm_isdst = 0;
	
    // 초단위로 변경하여 리턴한다.
    return mktime(&base_time);
}

void CIBK_CaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIBK_CaptureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIBK_CaptureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIBK_CaptureDlg::StartCapture()
{
	if(IsToDayHoliDay())
		return;

	SetWindowText("IBK_Capture ----   캡쳐중  ----!!!!!");
	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(GetDesktopWindow());
	CDC 		memDC;
	CRect		rect;
	CRect      tmrect;
	memDC.CreateCompatibleDC(&dc); 
	this->GetWindowRect(&tmrect);
	rect.left = 0 ;
	rect.top = 0 ;
	rect.right = GetSystemMetrics(SM_CXSCREEN);
	rect.bottom = GetSystemMetrics(SM_CYSCREEN);
	
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY); 
	
	// Create logical palette if device support a palette
	CPalette pal;
	if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		
		pLP->palNumEntries = 
			GetSystemPaletteEntries(dc, 0, 255, pLP->palPalEntry);
		
		// Create the palette
		pal.CreatePalette(pLP);
		
		delete[] pLP;
	}
	
	memDC.SelectObject(pOldBitmap);
	
	// Convert the bitmap to a DIB
	HANDLE hDIB = writeDDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)	
	{
		//	Axis::MessageBox("bitmap to DIB Failed.", MB_OK | MB_ICONSTOP);
		AfxMessageBox("bitmap to DIB Failed");
		return;
	}
	
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime, strMonthDay, stmp;
	strCurTime.Format(_T("%d월%d일%dh%dm%ds.bmp"), oTime.GetMonth(), oTime.GetDay(),oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	
	CString sPath;
	((CWnd*)GetDlgItem(IDC_EDIT_PATH))->GetWindowText(sPath);

	strMonthDay.Format("%02d%02d", oTime.GetMonth(), oTime.GetDay());

//	stmp.Format("%04d", oTime.GetYear());
//	sPath += "\\" + stmp;
//	sPath += "\\" + strMonthDay;
	CFile		Dfile;
	CFileFind ff;

	if (!ff.FindFile(sPath))
	{
	//	MessageBox(sPath, "startcapture", MB_OK);
		CString slog;
		int ret =::CreateDirectory(sPath, NULL);
		slog.Format("startcapture createfile ret=[%d] err=[%d] [%s]", ret, GetLastError(),sPath);
		MessageBox(slog, "startcapture", MB_OK);
	}

//	BOOL bExist = Dfile.Open(sPath, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);

	file.Format("%s\\%s", sPath, strCurTime);

	//file.Replace("/", "");
	CFileDialog	dlg(FALSE, "bmp", file, OFN_OVERWRITEPROMPT, 
		"BMP Files(*.bmp)|*.bmp||", NULL);
	
	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	writeDIB((LPSTR)(LPCTSTR)file, hDIB);  
	
	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);
}

void CIBK_CaptureDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	KillTimer(9999);
	if(m_cbTime.GetCurSel() == 0)
		SetTimer(9999, 1000 * 60 , NULL);
	else if(m_cbTime.GetCurSel() == 1)
		SetTimer(9999, 1000 * 60 * 5 , NULL);
	else if(m_cbTime.GetCurSel() == 2)
		SetTimer(9999, 1000 * 60 * 10 , NULL);	
	else if(m_cbTime.GetCurSel() == 3)
		SetTimer(9999, 1000 * 60 * 10 , NULL);
	
	StartCapture();
}

BOOL CIBK_CaptureDlg::writeDIB(LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;
	
	if (!hDIB)
		return FALSE;
	
	CFile file;
	if(!file.Open(szFile, CFile::modeWrite|CFile::modeCreate))
	{
		CString slog;
		slog.Format("[%d] ", GetLastError());
//		AfxMessageBox(slog);
		return FALSE;
	}
	
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	
	int nColors = int(pow(2, lpbi->biBitCount));
	if (nColors > 256)
		nColors = 0;
	
	// Fill in the fields of the file header 
	hdr.bfType	= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize	= GlobalSize (hDIB) + sizeof(hdr);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits	= (DWORD) (sizeof(hdr) + lpbi->biSize +	nColors * sizeof(RGBQUAD));
	
	// Write the file header 
	file.Write(&hdr, sizeof(hdr));
	
	// Write the DIB header and the bits 
	file.Write(lpbi, GlobalSize(hDIB));
	file.Close();
	return TRUE;
}


HANDLE CIBK_CaptureDlg::writeDDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if (dwCompression == BI_BITFIELDS)	return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal == NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = int(pow(2, bi.biBitCount));
	if (nColors > 256)
		nColors = 0;

	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);
	if (!hDIB)
	{
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;
		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	{
		hDIB = handle;
	}
	else
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);	// Reselect the original palette
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if (!bGotBits)
	{
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	SelectPalette(hDC, hPal, FALSE);
	::ReleaseDC(NULL, hDC);

	return hDIB;
}

void CIBK_CaptureDlg::OnBtnFind() 
{
	// TODO: Add your control notification handler code here/

	ITEMIDLIST*  pildBrowse;
	TCHAR   pszPathname[MAX_PATH];
	BROWSEINFO  bInfo;
	memset(&bInfo, 0, sizeof(bInfo));
	bInfo.hwndOwner   = GetSafeHwnd();
	bInfo.pidlRoot   = NULL;
	bInfo.pszDisplayName = pszPathname;
	bInfo.lpszTitle   = _T("디렉토리를 선택하세요");
	bInfo.ulFlags   = BIF_RETURNONLYFSDIRS; 
	bInfo.lpfn    = NULL;
	bInfo.lParam  = (LPARAM)(LPCTSTR)"C:\\";
	bInfo.lParam  = (LPARAM)NULL;
	pildBrowse    = ::SHBrowseForFolder(&bInfo);
	if(pildBrowse)
	{
		SHGetPathFromIDList(pildBrowse, pszPathname);
		((CWnd*)GetDlgItem(IDC_EDIT_PATH))->SetWindowText(pszPathname);
	}
}


CString CIBK_CaptureDlg::IsWow64()
{
	CString sResult = "UNKNOWN";
	BOOL bIsWow64 = FALSE;
	
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	
	if(fnIsWow64Process != NULL)
	{
		if(!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			return sResult;
		} 
	}
	
	if(bIsWow64 == FALSE)
	{
		sResult = "32";
	}
	else
	{
		sResult = "64";
	}
	
	return sResult;
}

void CIBK_CaptureDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TM_START:
			{
				KillTimer(TM_START);
				if(!IsToDayHoliDay())
					OnBtnStart();
			}
			break;
		case 9999:
			{
				CTime	time = CTime::GetCurrentTime();
				CString temp;
				temp.Format("%02d%02d", time.GetHour(), time.GetMinute());
				if(!IsToDayHoliDay())  
				{	
					if(atoi(temp) >= 800 && atoi(temp) <= 1530  )  //1100 -> 1530
					{
						StartCapture();
					}
					else
					{
						SetWindowText("IBK_Capture ----   캡쳐종료  ----!!!!!");
					}
				}
			
			}
			break;
	}
		
	CDialog::OnTimer(nIDEvent);
}

void CIBK_CaptureDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	KillTimer(9999);
}

BOOL CIBK_CaptureDlg::IsToDayHoliDay()
{
	return FALSE;

	CTime time = CTime::GetCurrentTime();
	int iday = time.GetDayOfWeek();
	if(iday == 1 || iday == 7)
		return TRUE;

	return FALSE;
}


void CIBK_CaptureDlg::BmpTojpg(CString sfile)
{
	CLSID jpegClsid;
	GetEncoderClsid("image/jpeg", &jpegClsid);

	Image* pimg = Image::FromFile((const WCHAR*)(LPCTSTR)sfile);
	pimg->Save((const WCHAR *)(LPCTSTR)sfile, &jpegClsid);
}



int CIBK_CaptureDlg::GetEncoderClsid(const char* format, CLSID* pClsid)
{
	GdiplusStartupInput gpStartupInput;

	 ULONG_PTR gpToken;

	 int val = GdiplusStartup(&gpToken, &gpStartupInput, NULL);

//	 if(val != 1)
//		 return -1;

	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	
	if(size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0 ; j < num; ++j)
	{
		if(strcmp((char*)pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);

	return 1;
}

void CIBK_CaptureDlg::OnBtnJpeg() 
{
	// TODO: Add your control notification handler code here
	BmpTojpg("15.bmp");
}
