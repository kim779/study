#include "stdafx.h"
#include <assert.h>
#include <Winsock2.h>
#include "resource.h"	
#include "winprint.h"
#include "imageload.h"			

//20080711 이문수 >>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"
//20080711 이문수 <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IMAGETYPE_NONE	0
#define IMAGETYPE_BMP	1
#define IMAGETYPE_GIF	2
#define IMAGETYPE_PCX	3
#define IMAGETYPE_TGA	4
#define IMAGETYPE_JPG	5
#define IMAGETYPE_TIF	6

#define IMAGETYPE_FIRSTTYPE IMAGETYPE_BMP
#define IMAGETYPE_LASTTYPE	IMAGETYPE_TIF


char * szExtensions[] = { ".BMP", ".GIF", ".PCX", ".TGA", ".JPG", ".TIF", "" };


//20080711 이문수 >>
typedef CArray < CRect, CRect & > CRectArray;

BOOL CALLBACK MonitorEnumProc( HMONITOR hMonitor,  // handle to display monitor
							  HDC hdcMonitor,     // handle to monitor-appropriate device context
							  LPRECT lprcMonitor, // pointer to monitor intersection rectangle
							  LPARAM dwData       // data passed from EnumDisplayMonitors
							  )
{
	CRect rect = *lprcMonitor;
	
	if(dwData)
	{
		CRectArray * pRectArray = (CRectArray *)dwData;
		pRectArray->Add(rect);
	}
	return TRUE;
}
//20080711 이문수 <<

char 		szPrintDlg[] = "PRINTING";    // Name of Print dialog from .RC

CWinPrint::CWinPrint()
{
	bPrintContinue	= FALSE;
	nPrintPage		= 0;

	//20080711 이문수 >>
	m_hMainWnd		= NULL; 
	m_hMDIClient	= NULL;
	//20080711 이문수 <<

}
CWinPrint::~CWinPrint()
{
}

//===========================================================================//
// 함수명  : fnPrintWindow                                                   //
// 함수기능: 캡쳐를 위해 사용                                                //
// 파라메타: HWND hFrameWnd      : 메인 윈도우 핸들                          //
//           HWND hPrintWnd      : ALP+P를 사용한 프린트 화면                //
//           HINSTANCE hInstance : DLL을 부르는 윈도우의 인스턴스            //
// 리턴값  : BOOL                                                            //
//===========================================================================//
BOOL CWinPrint::fnPrintWindow(	int niGubun,
								HWND hFrameWnd, 
								HDC hPrintDC, 
								CString strFileName,
								BOOL bPortrait ,
								HDIB _hDIB,
								CString strTitle,
								CString strMsg )
{
    HWND            hWndSelect =  hFrameWnd;    // 프린트 화면 핸들 
    HDIB            hDIB = _hDIB;				// handle to DIB
    BOOL            nErrorCheck;
  
    ghWndFrame = hFrameWnd;
	m_Portrait = bPortrait;

	memset(lpWindowTitle,0x00,50);
	::GetWindowText(hWndSelect,lpWindowTitle,50);
    // 선택된 윈도우 액티브 
    SetActiveWindow(hWndSelect);
    // 선택된 윈도우를 캡쳐하기위해 조정 
    SetWindowPos(hWndSelect, NULL, 0, 0, 0, 0,
                 SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE);
    UpdateWindow(hWndSelect);

    // 선택된 윈도우의 타이틀을 얻는다.
    gnWindowsTextSize = strlen(lpWindowTitle);
    
    //lpPrintBuff = new char[101];
    // 선택된 윈도우에서 DIB를 얻는다.
	
    if(!hDIB)
		hDIB = CopyWindowToDIB(hWndSelect);

	if( niGubun == 500 || niGubun == 600 )
        SaveDIB((HANDLE) hDIB, (LPSTR)(LPCTSTR)strFileName);
	else 
		  nErrorCheck = PrintWindowDIB(hDIB,hPrintDC);     

    return nErrorCheck;
}
//===========================================================================//
// 함수명  : SelectWindow                                                    //
// 함수기능: 마우스가 지정한 윈도우의 핸들을 리턴                            //
// 파라메타: NULL                                                            // 
// 리턴값  : HWND : 지정된 윈도우 핸들                                       //
//===========================================================================//
HWND CWinPrint::SelectWindow()
{
    HCURSOR hOldCursor = NULL;     // Handle to old cursor
    POINT pt;               // Stores mouse position on a mouse click
    HWND hWndClicked;       // Window we clicked on
    MSG msg;

    // 마우스 메세지를 윈도우가 선택될 동안 계속 받는다. 
    SetCapture(ghWndFrame);
    
    // 마우스 모양을 바꾼다.
  //  HCURSOR hCurSelect = AfxGetApp()->LoadCursor(IDR_MAINFRAME);
  //  hOldCursor = SetCursor(hCurSelect);

    // Eat mouse messages until a WM_LBUTTONUP is encountered.
    // WM_LBUTTONUP메세지가 발생할동안 루프  
    for (;;){
    WaitMessage();
    if (PeekMessage(&msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE)) {
        if (msg.message == WM_LBUTTONUP) {
            // 마우스 포지션을 얻는다.
            pt.x = LOWORD(msg.lParam);
            pt.y = HIWORD(msg.lParam);

            // 화면 단위로 바꾼다.
            ClientToScreen(ghWndFrame, &pt);
            // 클릭된 우도우 핸들을 얻는다. 
            hWndClicked = WindowFromPoint(pt);
            // 윈도우 핸들을 검사 
            if (!hWndClicked) {
                ReleaseCapture();
                SetCursor(hOldCursor);
                return NULL;
            }
        break;
        }           

    }
    else
        continue;
    }

    ReleaseCapture();
    SetCursor(hOldCursor);
    return (hWndClicked);
}

//===========================================================================//
// 함수명  : PrintWindowDIB                                                  //
// 함수기능: hDIB를 프린트                                                   //
// 파라메타: HDIB hDIB : 프린트할 hDIB                                       //
// 리턴값  : BOOL      : 프린트의 성공여부                                   //
//===========================================================================//
BOOL CWinPrint::PrintWindowDIB(HDIB hDIB ,HDC hPrintDC) 
{
    static OPTIONSTRUCT opts;
    int     iReturn = TRUE;
    BOOL    wReturn=TRUE;

/*    // Hot Ket에 의한 프린트 인지 검사 
    if( nPrintHotKey ) {
        opts.iOption = IDC_BESTFIT;
        bPrintContinue = FALSE;
        iReturn = TRUE;

    } else {
        // first page print
        if( nPrintPage == 0 ) {
            CPrintDlg z_SabPrintDlg(NULL, &opts);
            iReturn = z_SabPrintDlg.DoModal();
        }
        else 
            iReturn = TRUE;
    }          
  */      
    if (iReturn) {
        if (hDIB) {
            if (iReturn == 2)       // 연속 check
                bPrintContinue = TRUE;

            wReturn = PrintDIB(hDIB,hPrintDC ,PW_BESTFIT, 0, 0);
            // 프린트가 끝난 hDIB의 메모리를 제거 
            DestroyDIB(hDIB);
        }
    }
    return wReturn;
}

//===========================================================================//
// 함수명  : CopyWindowToDIB                                                 //
// 함수기능: 선택된 윈도우를 DIB로 만든다.                                   //
// 파라메타: HWND   hWnd : 선택된 윈도우                                     //
// 리턴값  : HDIB        : 윈도우의 DIB                                      //
//===========================================================================//
HDIB CWinPrint::CopyWindowToDIB(HWND hWnd)
{
    HDIB                hDIB;  // handle to DIB
    RECT                rectClient;

    // 윈도우 핸들을 검사 
    if (!hWnd)
        return NULL;

	GetWindowRect(hWnd, &rectClient);

    // 지정된 화면의 DIB를 복사한다.
	//20080711 이문수 >>
    //hDIB = CopyScreenToDIB(&rectClient);
	hDIB = CopyScreenToDIB(hWnd, &rectClient);
	//20080711 이문수 <<

    return hDIB;
}

//===========================================================================//
// 함수명  : CopyScreenToDIB                                                 //
// 함수기능: 지정된 영역의 HDIB를 얻는다.                                    //
// 파라메타: LPRECT lpRect : 지정된 윈도우의 영역                            //
// 리턴값  : HDIB        : 윈도우의 DIB                                      //
//===========================================================================//
//20080711 이문수 >>
//HDIB CWinPrint::CopyScreenToDIB(HWND hWnd, LPRECT lpRect)
HDIB CWinPrint::CopyScreenToDIB(HWND hWnd, LPRECT lpRect)
//20080711 이문수 <<
{
    HBITMAP hBitmap;    // handle to device-dependent bitmap
    HPALETTE hPalette;  // handle to palette
    HDIB hDIB=NULL;     // handle to DIB

    // 화면의 비트맵을 복사 
	//20080711 이문수 >>
	//hBitmap = CopyScreenToBitmap(lpRect);
    hBitmap = CopyScreenToBitmap(hWnd, lpRect);
	//20080711 이문수 <<

    if (!hBitmap)
        return NULL;

    // 현재의 파렛트를 얻는다.
    hPalette = GetSystemPalette();

    // 비트맵을 DIB로 변환 
    hDIB = BitmapToDIB(hBitmap, hPalette);

    DeleteObject(hBitmap);
    DeleteObject(hPalette);

    return hDIB;
}


HDIB CWinPrint::CopyBmpToDIB(HBITMAP hBitmap)
{
	HPALETTE hPalette;  // handle to palette
    HDIB hDIB=NULL;     // handle to DIB

    if (!hBitmap)
        return NULL;

    // 현재의 파렛트를 얻는다.
    hPalette = GetSystemPalette();

    // 비트맵을 DIB로 변환 
    hDIB = BitmapToDIB(hBitmap, hPalette);
    
    DeleteObject(hPalette);

    return hDIB;
}

//===========================================================================//
// 함수명  : CopyScreenToBitmap                                              //
// 함수기능: 지정된 영역의 HBITMAP를 얻는다.                                 //
// 파라메타: LPRECT lpRect : 지정된 윈도우의 영역                            //
// 리턴값  : HBITMAP       : 윈도우의 HDIB                                   //
//===========================================================================//
HBITMAP CWinPrint::CopyScreenToBitmap(HWND hWnd, LPRECT lpRect)
{
    HDC     hScrDC, hMemDC;             // screen DC and memory DC
    HBITMAP hBitmap;
    HGDIOBJ hOldBitmap;        // handles to deice-dependent bitmaps
    int     nX, nY, nX2, nY2;           // coordinates of rectangle to grab
    int     nWidth, nHeight;            // DIB width and height
    


    // 화면이 비어 있는지를 검사 
    if (IsRectEmpty(lpRect))
        return NULL;



	hScrDC = ::GetWindowDC(hWnd);//20080711 이문수 >>	



    // 화면 DC와 메모리 DC를 생성한다.
    //hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 20080711 이문수 
    hMemDC = CreateCompatibleDC(hScrDC);


    nX	= lpRect->left;
    nY	= lpRect->top;
    nX2 = lpRect->right;
    nY2 = lpRect->bottom;


    nWidth = nX2 - nX;
    nHeight = nY2 - nY;

    // 화면을 비트맵으로 잡는다.
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

    // 비트맵을 메모리 DC로 옮긴다.
    hOldBitmap = SelectObject(hMemDC, hBitmap);


	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX - lpRect->left, nY - lpRect->top, SRCCOPY);
	

    // 비트맵에 사각형 테두리 그리기 
    SelectObject(hMemDC, GetStockObject(BLACK_PEN));
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
    Rectangle(hMemDC, 0, 0, nWidth, nHeight);
    
    
    // 전으로 DC를 돌려 놓는다.
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    
	
    DeleteDC(hMemDC);
	ReleaseDC(hWnd, hScrDC);

	return hBitmap;
}

//===========================================================================//
// 함수명  : GetSystemPalette                                                //
// 함수기능: 지정된 영역의 HBITMAP를 얻는다.                                 //
// 파라메타: VOID       :                                                    //
// 리턴값  : HPALETTE   : 파레트                                             //
//===========================================================================//
HPALETTE CWinPrint::GetSystemPalette ()
{
    HDC           hDC;
    HPALETTE      hPal = NULL;
    HANDLE        hLogPal;
    LPLOGPALETTE  lpLogPal;
    int           i, nColors;


    // 파레트를 찾는다.
    hDC = GetDC (NULL);
    if (!hDC)
        return NULL;

    // device context의 파렛트 목록 수를 찾아낸다.
    nColors = PalEntriesOnDevice (hDC);
    ReleaseDC (NULL, hDC);

    // 파렛트 목록 수 만큼 메모리 할당
    hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
                           nColors * sizeof (PALETTEENTRY));
    if (!hLogPal)
        return NULL;

    lpLogPal = (LPLOGPALETTE) GlobalLock (hLogPal);

    lpLogPal->palVersion    = PALVERSION;
    lpLogPal->palNumEntries = nColors;

    for (i = 0;  i < nColors;  i++) {
        lpLogPal->palPalEntry[i].peBlue  = 0;
        *((LPWORD) (&lpLogPal->palPalEntry[i].peRed)) = i;
        lpLogPal->palPalEntry[i].peFlags = PC_EXPLICIT;
    }

    // 로직 파레트를 생성 
    hPal = CreatePalette (lpLogPal);

    GlobalUnlock (hLogPal);
    GlobalFree (hLogPal);

    return hPal;
}         

//===========================================================================//
// 함수명  : PalEntriesOnDevice                                                //
// 함수기능: 지정된 디바이스의 파렛트 목록 수                                //
// 파라메타: HDC    hDC : device context                                     //
// 리턴값  : int        : device 파레트의 목록 수                            //
//===========================================================================//
int CWinPrint::PalEntriesOnDevice (HDC hDC)
{
    int nColors;

    // 디바이스의 파렛트 목록 수를 얻는다.
    nColors = GetDeviceCaps (hDC, SIZEPALETTE);

    // 파렛트의 목록 수가 0이면 시스템 파렛트에서 찾아낸다.
    if (!nColors)
        nColors = GetDeviceCaps (hDC, NUMCOLORS);

    assert (nColors);

    return nColors;
}

//===========================================================================//
// 함수명  : BitmapToDIB                                                     //
// 함수기능: 비트맵을 DIB로 변환                                             //
// 파라메타: HBITMAP hBitmap : 변환될 비트맵                                 //
//           HPALETTE hPal   : 지정된 파렛트                                 //
// 리턴값  : HDIB            : 변환된 DIB                                    //
//===========================================================================//
HDIB CWinPrint::BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal)
{
    BITMAP             Bitmap;
    BITMAPINFOHEADER   bmInfoHdr;
    LPBITMAPINFOHEADER lpbmInfoHdr;
    LPSTR              lpBits;
    HDC                hMemDC;
    HANDLE             hDIB;
    HPALETTE           hOldPal = NULL;

    if (!hBitmap)
        return NULL;
    
    // 비트맵의 정보를 얻는다.
    if (!GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
        return NULL;

    // 비트맵의 헤드 정보를 얻는다.
    InitBitmapInfoHeader (&bmInfoHdr,
                         Bitmap.bmWidth,
                         Bitmap.bmHeight,
                         Bitmap.bmPlanes * Bitmap.bmBitsPixel);


    // DIB 메모리를 할당한다.
    hDIB = GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) +
                        PaletteSize ((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

    if (!hDIB)
        return NULL;

    lpbmInfoHdr  = (LPBITMAPINFOHEADER) GlobalLock (hDIB);
    *lpbmInfoHdr = bmInfoHdr;
    lpBits       = FindDIBBits((LPSTR) lpbmInfoHdr);

    // 비트맵을 DIB로 변환하기위해 DC를 얻는다.
    hMemDC       = GetDC (NULL);

    // 파렛트를 검사하여 파렛트를 설정 
    if(hPal) 
	{
        hOldPal = SelectPalette (hMemDC, hPal, FALSE);
        RealizePalette (hMemDC);
    }	

    // 글로벌 메모리에 DIB를 복사한다.
    if (!GetDIBits(hMemDC, hBitmap, 0, Bitmap.bmHeight, lpBits,
                   (LPBITMAPINFO) lpbmInfoHdr, DIB_RGB_COLORS)) {
        GlobalUnlock (hDIB);
        GlobalFree (hDIB);
        hDIB = NULL;
    }
    else 
        GlobalUnlock (hDIB);
   
    if (hOldPal)
        SelectPalette (hMemDC, hOldPal, FALSE);

    ReleaseDC (NULL, hMemDC);

    return hDIB;
}

//===========================================================================//
// 함수명  : InitBitmapInfoHeader                                            //
// 함수기능: BITMAPINFOHEADER의 초기화, 넓이,길이, 픽셀당 바이트             //
// 파라메타: LPBITMAPINFOHEADER lpBmInfoHdr : 비트맵 헤드정보                //
//           DWORD dwWidth                  : 비트맵 넓이                    //
//           DWORD dwHeight                 : 비트맵 높이                    //
// 리턴값  : void                                                            //
//===========================================================================//
void CWinPrint::InitBitmapInfoHeader (LPBITMAPINFOHEADER lpBmInfoHdr, DWORD dwWidth,
                           DWORD dwHeight, int nBPP)
{
   memset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));

   lpBmInfoHdr->biSize      = sizeof (BITMAPINFOHEADER);
   lpBmInfoHdr->biWidth     = dwWidth;
   lpBmInfoHdr->biHeight    = dwHeight;
   lpBmInfoHdr->biPlanes    = 1;

   if (nBPP <= 1)
      nBPP = 1;
   else if (nBPP <= 4)
      nBPP = 4;
   else if (nBPP <= 8)
      nBPP = 8;
   else
      nBPP = 24;

   lpBmInfoHdr->biBitCount  = nBPP;
   lpBmInfoHdr->biSizeImage = WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}

//===========================================================================//
// 함수명  : FindDIBBits                                                     //
// 함수기능: DIB의 비트 주소                                                 //
// 파라메타: LPSTR lpbi : DIB의 메모리 주소                                  //
// 리턴값  : LPSTR      : DIB bit의 주소                                     //
//===========================================================================//
LPSTR CWinPrint::FindDIBBits (LPSTR lpbi)
{
   return (lpbi + *(LPDWORD)lpbi + PaletteSize (lpbi));
}

//===========================================================================//
// 함수명  : PrintDIB                                                        //
// 함수기능: hDIB print band or non-banding(프린트이 band의 지원 여부에 따라 //
// 파라메타: HDIB hDIB      : 프린트할 hDIB                                  //
//           WORD fPrintOpt : 프린트 옵션                                    //
//           WORD wXScale   : 프린트 배율 X 길이                             //
//           WORD wYScale   : 프린트 배율 Y 길이                             //
// 리턴값  : BOOL      : 프린트의 성공여부                                   //
//===========================================================================//
BOOL CWinPrint::PrintDIB(HDIB hDib, HDC hPrintDC,WORD fPrintOpt, WORD wXScale, WORD wYScale)  
{
    RECT rect;                   // 밴딩을 위한 Rect 구조체 
    LPSTR lpBits;                // pointer to the DIB bits
    LPBITMAPINFOHEADER lpDIBHdr; // Pointer to DIB header
    int nBandCount = 0;          // used for print dialog box to count bands
    BOOL wErrorCode=TRUE;         // Error code to return
    RECT rPrintRect;             
    char szJobNameTrunc[35];   
    char szBuffer[100];        
    DOCINFO stDocInfo;

    if (!hDib)
        return FALSE;

    // DIB 포인트 헤드를 얻는다.
    lpDIBHdr = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (!lpDIBHdr) 
        return FALSE;
    // DIB bits의 포인트를 찾는다. 
    lpBits = FindDIBBits((LPSTR)lpDIBHdr); 

    // 첫번째 프린트이면 프린트 DC를 얻는다. 
    if( nPrintPage == 0 )
	{
		if(hPrintDC == NULL)
		{
			if( !(hPrnDC = GetPrinterDC()) )
				return FALSE;
		}
		else hPrnDC = hPrintDC;
	}
            
    SetStretchBltMode(hPrnDC, COLORONCOLOR);
    // 프린트할 구간 계산 
    CalculatePrintRect(hPrnDC, &rPrintRect, fPrintOpt, lpDIBHdr->biWidth,
                       lpDIBHdr->biHeight);


//	rPrintRect = CRect (0,0,500,500);
    rPrintRect.top = 20;

    // 프린팅 다이알로그 생성 
   // hDlgAbort = CreateDialog(AfxGetInstanceHandle(), szPrintDlg, GetFocus(), PrintAbortDlg);

    // 프린팅 상태를 나타낸다.
    memcpy(szJobNameTrunc, lpWindowTitle, gnWindowsTextSize+1);
    wsprintf(szBuffer, "인쇄중 '%s'", (LPSTR)szJobNameTrunc);
//    SetDlgItemText(hDlgAbort, IDC_PRINTTEXT1, (LPSTR)szBuffer);
    bAbort = FALSE;

    // ABOUT PROC를 생성 
 //   if (SetAbortProc(hPrnDC, PrintAbortProc) < 0)
  //      return FALSE;

    // 첫번째 프린트
	CString strMsg = "프린트 작업을 수행 할 수 없습니다.\n프린터기 설치를 확인 바랍니다";
    if( nPrintPage == 0 ) {
        // Escape()의 STARTDOC로 프린팅 시작 
        memset(&stDocInfo, 0x00, sizeof(stDocInfo));
        stDocInfo.cbSize = sizeof(stDocInfo);
        stDocInfo.lpszDocName = szJobNameTrunc;
        if (StartDoc(hPrnDC, &stDocInfo) < 0) {
            DestroyWindow(hDlgAbort); 
            DeleteDC(hPrnDC);
            GlobalUnlock(hDib);
			::MessageBox(ghWndFrame,strMsg ,"DRSCPRINT",MB_SYSTEMMODAL);
            return FALSE;
        }
        if (StartPage(hPrnDC) < 0) {
            DestroyWindow(hDlgAbort); 
            DeleteDC(hPrnDC);
            GlobalUnlock(hDib);
			::MessageBox(ghWndFrame,strMsg ,"DRSCPRINT",MB_SYSTEMMODAL);
            return FALSE;
        }
    }   

    // 밴딩이 않됨  
    rect = rPrintRect;
 //   SetDlgItemText(hDlgAbort, IDC_PERCENTAGE, (LPSTR)
//                   "화면 인쇄 중...");
    wErrorCode = PrintBand(hPrnDC, &rPrintRect, &rect, TRUE, TRUE,
                           lpDIBHdr, lpBits);
	
    // Abort procudure 선택 
    if (!bAbort) {
        // 첫번째 프린트물의 여백 
        if( (nPrintPage == 1) || (bPrintContinue == FALSE) ) {
            // Non-banding devices need a NEWFRAME 
            if (EndPage(hPrnDC) < 0)
                return FALSE;
            // ENDDOC로 프린팅을 끝낸다.
            if (EndDoc(hPrnDC) < 0)
                wErrorCode = FALSE;
            nPrintPage = 0;
            DeleteDC(hPrnDC);
        }
        else 
            nPrintPage++;
        DestroyWindow(hDlgAbort);
    }
    else {
        DestroyWindow(hDlgAbort);
        DeleteDC(hPrnDC);
        nPrintPage = 0;
    }
      
    GlobalUnlock(hDib);
    return wErrorCode;
}

//===========================================================================//
// 함수명  : CalculatePrintRect                                              //
// 함수기능: fPrintOpt에 따라서 프린트 사이즈 계산                           //
// 파라메타: HDC hDC            : 프린트 DC                                  //
//           LPRECT lpPrintRect : 프린트 옵션                                //
//           WORD cxDIB         : 프린트할 DIB 넓이                          //
//           WORD cxDIB         : 프린트할 DIB 길이                          //
// 리턴값  : void                                                            //
//===========================================================================//
void CWinPrint::CalculatePrintRect(HDC hDC, LPRECT lpPrintRect, WORD fPrintOpt, 
                        DWORD cxDIB, DWORD cyDIB)  
{
    int cxPage, cyPage, cxInch, cyInch;

    if (!hDC)
        return;

    // 프린트 드라이브 정보를 얻는다.
    cxPage = GetDeviceCaps(hDC, HORZRES);     // Width of printr page - pixels
    cyPage = GetDeviceCaps(hDC, VERTRES);     // Height of printr page - pixels
    cxInch = GetDeviceCaps(hDC, LOGPIXELSX);  // Printer pixels per inch - X
    cyInch = GetDeviceCaps(hDC, LOGPIXELSY);  // Printer pixels per inch - Y

    lpPrintRect->top = 0;
    lpPrintRect->left = 0;
	if(m_Portrait) //가로
		lpPrintRect->bottom = cyPage/2;
	else
		lpPrintRect->bottom = cyPage - 100;

    lpPrintRect->right = cxPage;
}

//===========================================================================//
// 함수명  : PrintBand                                                       //
// 함수기능: 주어진 값에따라서 프린트                                        //
// 파라메타: HDC hDC                     : 프린트 DC                         //
//           LPRECT lpRectOut            : 프린트할 구간                     //
//           LPRECT lpRectClip           : 프린트할 DIB 구간                 //
//           BOOL fDoText                : 프린트할 자료가 텍스트인지        //
//           BOOL fDoGraphics            : 프린트할 자료가 그래프인지        //
//           LPBITMAPINFOHEADER lpDIBHdr : 프린트할 DIB 헤더                 //
//           LPSTR lpDIBBits             : 프린트할 DIB의 버프 주소          //
// 리턴값  : BOOL : 프린트 성공 여부                                         //
//===========================================================================//
BOOL CWinPrint::PrintBand(HDC hDC, LPRECT lpRectOut, LPRECT lpRectClip, BOOL fDoText, 
               BOOL fDoGraphics, LPBITMAPINFOHEADER lpDIBHdr, LPSTR lpDIBBits)
{                                                
    RECT        rect;                            
    double      dblXScaling, dblYScaling;
//    HFONT		hFont, hFontOld;

    if (fDoGraphics) {
        dblXScaling = ((double)lpRectOut->right - lpRectOut->left) / 
                       (double)lpDIBHdr->biWidth;
        dblYScaling = ((double)lpRectOut->bottom - lpRectOut->top) / 
                       (double)lpDIBHdr->biHeight;
        // RectOut, RectClip를 교집합 
        IntersectRect(&rect, lpRectOut, lpRectClip);
        if (!IsRectEmpty(&rect)) {
            RECT rectIn;
            rectIn.left = (int)((rect.left - lpRectOut->left) / dblXScaling + 0.5);
            rectIn.top = (int)((rect.top - lpRectOut->top) / dblYScaling + 0.5);
            rectIn.right = (int)(rectIn.left + (rect.right - rect.left) /
                                 dblXScaling + 0.5);
            rectIn.bottom = (int)(rectIn.top + (rect.bottom - rect.top) /
                                  dblYScaling + 0.5);
            // 첫번째 프린트
            if( nPrintPage != 0 ) {
                rect.top = GetDeviceCaps(hPrnDC, VERTRES)/2;
                // 연속 출력의 용지 크기가 작다. 
                if( rect.bottom >= rect.top )
                    return FALSE;
                rect.bottom = rect.bottom*2 + 400;
                if( GetDeviceCaps(hPrnDC, VERTRES) <= rect.bottom )
                    rect.bottom = GetDeviceCaps(hPrnDC, VERTRES) - 400;
            }
            
            if( bPrintContinue ) {
                if( (GetDeviceCaps(hPrnDC, VERTRES)/2) <= rect.bottom )
                    bPrintContinue = FALSE;
            }
//            hFont = fnNewFont(40, 28, FW_BOLD, "굴림체");
//            hFontOld = (HFONT)SelectObject(hDC, (HGDIOBJ)hFont);
//            TextOut(hDC, 10, rect.top+10, lpPrintBuff, strlen(lpPrintBuff)); 
//            SelectObject(hDC, hFontOld);
            // 주어진 프린트 공간에 DIB를 프린트 한다. 
            if (!StretchDIBits(hDC, rect.left, rect.top+60, rect.right - rect.left, 
                               rect.bottom - rect.top, rectIn.left, 
                               (int)(lpDIBHdr->biHeight) - rectIn.top - (rectIn.bottom - rectIn.top),
                                rectIn.right - rectIn.left, rectIn.bottom - rectIn.top,  
                                lpDIBBits,  (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS, SRCCOPY))
                                
                return FALSE;
        }

		// 출력시간 표시
		CTime currentTime = CTime::GetCurrentTime();
		
		int nYYYY = currentTime.GetYear();
		int nMM = currentTime.GetMonth();
		int nDD = currentTime.GetDay();
		int nm1 = currentTime.GetHour();
		int nm2 = currentTime.GetMinute();
		int nm3 = currentTime.GetSecond();
		CString szCurrentTime;
		szCurrentTime.Format("[출력시간] %04d.%02d.%02d %02d:%02d:%02d",nYYYY, nMM, nDD, nm1,nm2,nm3);
		CSize size;
		::GetTextExtentPoint32(hDC, szCurrentTime, szCurrentTime.GetLength(), &size );
		::TextOut(hDC,rect.left,rect.bottom+50+size.cy,(LPCTSTR)szCurrentTime, szCurrentTime.GetLength());

		CString szPublisher;
		szPublisher.Format("[조작자] %s (%s)",m_strName,fnGetIpAddress());
		::GetTextExtentPoint32(hDC, szPublisher, szPublisher.GetLength(), &size );
		::TextOut(hDC,rect.right-size.cx,rect.bottom+50+size.cy,(LPCTSTR)szPublisher, szPublisher.GetLength());
		


    }
    return TRUE;
}

//===========================================================================//
// 함수명  : PaletteSize                                                     //
// 함수기능: DIB의 칼라수와 RGBQUAD(for a Windows 3.0-style DIB) 또는        //
//           RGBTRIPLE(for an OS/2-style DIB)                                //                                                          //
// 파라메타: LPSTR lpbi : DIB 메모리 포인트                                  //
// 리턴값  : WORD :DIB 파렛트 칼라 크기                                      //
//===========================================================================//
WORD CWinPrint::PaletteSize (LPSTR lpbi)
{
    if (IS_WIN30_DIB (lpbi))
        return (DIBNumColors (lpbi) * sizeof (RGBQUAD));
    else
        return (DIBNumColors (lpbi) * sizeof (RGBTRIPLE));
}

//===========================================================================//
// 함수명  : DestroyDIB                                                      //
// 함수기능: hDIB 메모리를 지운다.                                           //
// 파라메타: HDIB hDib : 지울 hDIB                                           //
// 리턴값  : void                                                            //
//===========================================================================//
void CWinPrint::DestroyDIB(HDIB hDib)
{
    GlobalFree(hDib);
}

//===========================================================================//
// 함수명  : GetPrinterDC                                                    //
// 함수기능: 프린트 DC를 얻는다.                                             //
// 파라메타: void                                                            //
// 리턴값  : HDC : 프린트 DC                                                 //
//===========================================================================//
HDC CWinPrint::GetPrinterDC()
{
    HDC         hPrinterDC;
    LPDEVMODE   lpDevMode = NULL;
    LPDEVNAMES  lpDevNames;

    static char szPrinter[64];
    char *szDevice, *szDriver, *szOutput;

    // Print Dialog box의 선택이 있었는지 확인  
    if (!pd.hDC) 
	{
        GetProfileString("windows", "device", "", szPrinter, 64);
        if ((szDevice = strtok(szPrinter, ",")) && (szDriver = strtok(NULL, ","))
            && (szOutput = strtok(NULL, ", "))) 
		{
            lstrcpy((LPSTR)gszDevice, (LPSTR)szDevice);    // Copy to global variables
            lstrcpy((LPSTR)gszOutput, (LPSTR)szOutput);
            return CreateDC(szDriver, szDevice, szOutput, NULL);
        }   
        return NULL;
    }

    // Print Dialog box에서 프린트 DC를 찾는다.
    lpDevNames = (LPDEVNAMES)GlobalLock(pd.hDevNames);
    lstrcpy((LPSTR)gszDriver, (LPSTR)lpDevNames + lpDevNames->wDriverOffset);
    lstrcpy((LPSTR)gszDevice, (LPSTR)lpDevNames + lpDevNames->wDeviceOffset);
    lstrcpy((LPSTR)gszOutput, (LPSTR)lpDevNames + lpDevNames->wOutputOffset);


    if(pd.hDC) {
        hPrinterDC = pd.hDC;
        GlobalUnlock(pd.hDevNames);
        return hPrinterDC;
    }

    if (pd.hDevMode)
        lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);

    hPrinterDC = CreateDC(gszDriver, gszDevice, gszOutput, lpDevMode);
    GlobalUnlock(pd.hDevNames);
    GlobalFree(pd.hDevNames);
    return hPrinterDC;
}

//===========================================================================//
// 함수명  : DIBNumColors                                                    //
// 함수기능: DIB의 색깔 수를 리턴                                            //
// 파라메타: LPSTR lpbi : DIB 메모리의 주소                                  //
// 리턴값  : WORD       : 색깔 수                                            //
//===========================================================================//
WORD CWinPrint::DIBNumColors (LPSTR lpbi)
{
    WORD wBitCount;

    if (IS_WIN30_DIB (lpbi)) {
        DWORD dwClrUsed;

        dwClrUsed = ((LPBITMAPINFOHEADER) lpbi)->biClrUsed;

        if (dwClrUsed)
            return (WORD) dwClrUsed;
    }


    // DIB의 픽셀당 비트수에서 컬러 수를 계산 
    if (IS_WIN30_DIB (lpbi))
        wBitCount = ((LPBITMAPINFOHEADER) lpbi)->biBitCount;
    else
        wBitCount = ((LPBITMAPCOREHEADER) lpbi)->bcBitCount;

    switch (wBitCount) {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            return 0;
    }
}

//===========================================================================//
// 함수명  : PrintAbortDlg                                                   //
// 함수기능: 프린트를  취소한다.                                             //
// 파라메타: HWND hWnd :                                                     //
// 리턴값  : HDC : 프린트 DC                                                 //
//===========================================================================//
BOOL CWinPrint::PrintAbortDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            char szBuffer[100];
            // 비트맵의 출력 상태를 나타낸다.
            // (EX "on HP LaserJet on LPT1")
            wsprintf(szBuffer, "on %s on %s", (LPSTR)(LPCTSTR)gszDevice, (LPSTR)(LPCTSTR)gszOutput);
            SetDlgItemText(hWnd, IDC_PRINTTEXT2, (LPSTR)szBuffer);
            SetFocus(GetDlgItem(hWnd, IDCANCEL));
//            fnMoveCenter(ghWndFrame, hWnd, 0);
        }
        return TRUE;     // Return TRUE is SetFocus() 출력상태에 대한 반환 

        case WM_COMMAND:
            bAbort = TRUE;
            DestroyWindow(hWnd);
            return TRUE;
        break;
        }
    return FALSE;
}

//===========================================================================//
// 함수명  : PrintAbortProc                                                  //
// 함수기능: 프린팅 상태에서 메세지 루프를 돈다.                             //
// 파라메타: HDC hDC : Dialog DC                                             //
// 리턴값  : BOOL 프린팅 취소의 결과                                         //
//===========================================================================//
BOOL CWinPrint::PrintAbortProc(HDC hDC, int code)
{
    MSG msg;

    while (!bAbort && PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        if (!IsDialogMessage(hDlgAbort, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    return (!bAbort);
}

//===========================================================================//
// 함수명  : PrintDlgProc                                                    //
// 함수기능: 프린트 옵션 선택                                                //
// 파라메타: HWND hWndDlg : 프린트 다이알로그 옵션                           //
// 리턴값  : BOOL :                                                          //
//===========================================================================//
BOOL CWinPrint::PrintDlgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) 
{
    static LPOPTIONSTRUCT lpOS;                   
    DWORD FlagSave;

    switch (Message) {
        case WM_INITDIALOG: {
            // DialogBoxParam()으로 dialog box를 사용하여 
            // lParam에 OPTIONSTRUCT을 포함한다.
            lpOS = (LPOPTIONSTRUCT)lParam;

            // 연속 검사 
            //CheckDlgButton(hWndDlg, IDC_CHECK_CONTINUE, TRUE);
            bPrintContinue = FALSE;
            
            // fill in non-variant fields of PRINTDLG struct.
            pd.lStructSize          = sizeof (PRINTDLG) ;
            pd.hwndOwner            = ghWndFrame;
            pd.hDevMode             = NULL ;
            pd.hDevNames            = NULL ;
            pd.hDC                  = NULL ;
            pd.Flags                = PD_RETURNDC | PD_NOSELECTION | PD_NOPAGENUMS;
            pd.nFromPage            = 0 ;
            pd.nToPage              = 0 ;
            pd.nMinPage             = 0 ;
            pd.nMaxPage             = 0 ;
            pd.nCopies              = 1 ;
            pd.hInstance            = NULL ;
            pd.lCustData            = 0L ;
            pd.lpfnPrintHook        = NULL ;
            pd.lpfnSetupHook        = NULL ;
            pd.lpPrintTemplateName  = NULL ;
            pd.lpSetupTemplateName  = NULL ;
            pd.hPrintTemplate       = NULL ;
            pd.hSetupTemplate       = NULL ;
        }
//        fnMoveCenter(ghWndFrame, hWndDlg, 0);
        break; // End of WM_INITDIALOG                                 

    case WM_CLOSE:
        PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
    break;

    case WM_COMMAND:
        switch (wParam) {
                                             
        case IDOPTION: {
            FlagSave = pd.Flags;
            pd.Flags |= PD_PRINTSETUP;    // Print Setup option 
            PrintDlg((LPPRINTDLG)&pd);
            pd.Flags = FlagSave;          // Remove option 
            
        }           
        break;

        case IDOK: {
            // OPTIONSTRUCT에 지정된 값을 저장 
            if (!lpOS) {
                EndDialog(hWndDlg, FALSE);
                break;
            }

            lpOS->iOption = IDC_BESTFIT;
            // 연속 프린트 검사 
            if( IsDlgButtonChecked(hWndDlg, IDC_CHECK_CONTINUE) )
                bPrintContinue = TRUE;
            else 
                bPrintContinue = FALSE;
            
            EndDialog(hWndDlg, TRUE);
        }
        break;

        case IDCANCEL:
            EndDialog(hWndDlg, FALSE);
        break;

        }
    break;    // End of WM_COMMAND                                 

    default:
        return FALSE;
    }
    return TRUE;
}
/*
//===========================================================================//
// 함수명  : PrintBOGOSE                                                     //
// 함수기능: 캡쳐를 위해 사용                                                //
// 파라메타: HWND hFrameWnd      : 메인 윈도우 핸들                          //
//           HWND hWnd           : DLL을 부르는 윈도우의 핸들                //
// 리턴값  : BOOL                                                            //
//===========================================================================//
BOOL FAR PASCAL _export fnPrintBOGOSE(HWND hFrameWnd, HWND hWnd)
{
    HGLOBAL     hChild;
    LPCHILD     lpChild;
    HDC         hPrnDC;                  // 프린트 DC
    HFONT       hPrintFont;
    static      OPTIONSTRUCT opts;
    FARPROC     lpMsgProc;      
    char        szJobNameTrunc[35];   
    char        szBuffer[50];           // Caption title
    int         xTemp, yTemp, xCharSpace, yCharSpace;
    int         nReturn;
    
    ghWndFrame = hFrameWnd;
    // 선택된 윈도우의 타이틀을 얻는다.
    gnWindowsTextSize = GetWindowText(hWnd, lpWindowTitle, 100);

    // 프린트 옵션 선택 dialog box(ex, bestfit, stretchtopage, scale)
    opts.iOption = 1;
    lpMsgProc = MakeProcInstance((FARPROC)PrintDlgProc, ghDLLInst);
    nReturn = DialogBoxParam(ghDLLInst, (LPSTR)"Print",
                            ghWndFrame, lpMsgProc, (LONG)(LPSTR)&opts);
    FreeProcInstance(lpMsgProc);
    if(!nReturn)
        return FALSE;
    // Print DC get
    hPrnDC = GetPrinterDC();
    if(!hPrnDC)
        return FALSE;               
    
    // 프린팅 다이알로그 생성 
    hDlgAbort = CreateDialog(ghDLLInst, szPrintDlg, GetFocus(), PrintAbortDlg);
    // 프린팅 상태를 나타낸다.
    lstrcpyn(szJobNameTrunc, lpWindowTitle, 31);
    szJobNameTrunc[31] = '\0';           // Truncate string to 31 chars
    wsprintf(szBuffer, "인쇄중 '%s'", (LPSTR)szJobNameTrunc);
    SetDlgItemText(hDlgAbort, IDC_PRINTTEXT1, (LPSTR)szBuffer);
    SetDlgItemText(hDlgAbort, IDC_PERCENTAGE, (LPSTR)"보고서 인쇄 중...");
    bAbort = FALSE;
*/
/*
    // 프린트 드라이브 정보를 얻는다.
    cxPage = GetDeviceCaps(hPrnDC, HORZRES);     // Width of printr page - pixels
    cyPage = GetDeviceCaps(hPrnDC, VERTRES);     // Height of printr page - pixels
    
    if(cxPage < cyPage) {
        xTemp = cxPage/332;
        yTemp = xTemp;
    }
    else {
        yTemp = cyPage/240;
        xTemp = yTemp;
    }
        
    SetMapMode(hPrnDC, MM_ISOTROPIC);
    SetWindowExt(hPrnDC, ((DEFAULT_CHAR_COUNT+1)*xTemp), 
                          ((DEFAULT_CHAR_LINE+1)*(yTemp-2+3)));
    SetViewportExt(hPrnDC, cxPage, cyPage);
*/
/*      
    // ABOUT PROC를 생성 
    if (Escape(hPrnDC, SETABORTPROC, NULL, (LPSTR)PrintAbortProc, NULL) < 0)
        return FALSE;
    // Escape()의 STARTDOC로 프린팅 시작 
    if (Escape(hPrnDC, STARTDOC, lstrlen((LPSTR)szJobNameTrunc), (LPSTR)
             szJobNameTrunc, NULL) < 0) {
        DestroyWindow(hDlgAbort); 
        DeleteDC(hPrnDC);
        return FALSE;
    }
    // Drowing Map 
    hChild = GetWindowWord(hWnd, WW_OFFSET_CHILD);
    lpChild = (LPCHILD)GlobalLock(hChild);
    xCharSpace = lpChild->xChar;
    yCharSpace = lpChild->yChar;
    lpChild->xChar = xTemp;
    lpChild->yChar = yTemp*2;
    
    // 폰트 크기 재지정 
    hPrintFont = fnNewFont((int)(xTemp*1.5), xTemp,FW_NORMAL, lpChild->szFontName);
    SelectObject(hPrnDC, hPrintFont);

    // map drowing
    if(lpChild->hMapData)
        fnDrawMap(hWnd, hPrnDC, lpChild, 0);
        
    lpChild->xChar = xCharSpace;
    lpChild->yChar = yCharSpace;
    GlobalUnlock(hChild);
    
    // Non-banding devices need a NEWFRAME 
    if (Escape(hPrnDC, NEWFRAME, NULL, NULL, NULL) < 0) {
        DeleteDC(hPrnDC);
        return FALSE;
    }
    if (Escape(hPrnDC, ENDDOC, NULL, NULL, NULL) < 0) {
        DeleteDC(hPrnDC);
        return FALSE;
    }
        
    if (!bAbort)
        DestroyWindow(hDlgAbort);
    DeleteDC(hPrnDC);

    return TRUE;
}
*/

//===========================================================================//
// 함수명  : fnPrintDelete()                                                 //
// 함수기능: 메인 종료시 프린트 DC가 남아있을 경우                           //
// 파라메타: NULL                                                            //
// 리턴값  : VOID                                                            //
//===========================================================================//
void CWinPrint::fnPrintDelete()
{
    // 메인 종료시 프린트 DC가 남아있을 경우 
    if(hPrnDC)
        DeleteDC(hPrnDC);
}
//===========================================================================//
// WINPRINT.C(프린트 DLL)   END                                              //
//===========================================================================//

//이미지를 화일로 저장하는 부분 
BOOL CWinPrint::SaveDIB(HANDLE hDib, LPSTR lpFileName)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure
    HANDLE              fh;         // file handle for opened file
    DWORD               dwDIBSize;
    DWORD               dwWritten;

    if (!hDib)
        return FALSE;

    fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    // Get a pointer to the DIB memory, the first of which contains
    // a BITMAPINFO structure

    lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (!lpBI)
    {
        CloseHandle(fh);
        return FALSE;
    }

    // Check to see if we're dealing with an OS/2 DIB.  If so, don't
    // save it because our functions aren't written to deal with these
    // DIBs.

    if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
    {
        GlobalUnlock(hDib);
        CloseHandle(fh);
        return FALSE;
    }

    // Fill in the fields of the file header

    // Fill in file type (first 2 bytes must be "BM" for a bitmap)

    bmfHdr.bfType = 0x4d42;  // "BM"

    // Calculating the size of the DIB is a bit tricky (if we want to
    // do it right).  The easiest way to do this is to call GlobalSize()
    // on our global handle, but since the size of our global memory may have
    // been padded a few bytes, we may end up writing out a few too
    // many bytes to the file (which may cause problems with some apps,
    // like HC 3.0).
    //
    // So, instead let's calculate the size manually.
    //
    // To do this, find size of header plus size of color table.  Since the
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
    // the size of the structure, let's use this.

    // Partial Calculation

    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  

    // Now calculate the size of the image

    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage
    // field

    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
        dwDIBSize += lpBI->biSizeImage;
    else
    {
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only

        // It's not RLE, so size is Width (DWORD aligned) * Height

        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) *
                lpBI->biHeight;

        dwDIBSize += dwBmBitsSize;

        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which 
        // have this field incorrect).

        lpBI->biSizeImage = dwBmBitsSize;
    }


    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
                   
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;

    // Now, calculate the offset the actual bitmap bits will be in
    // the file -- It's the Bitmap file header plus the DIB header,
    // plus the size of the color table.
    
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
            PaletteSize((LPSTR)lpBI);

    // Write the file header

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    // Write the DIB header and the bits -- use local version of
    // MyWrite, so we can write more than 32767 bytes of data
    
    WriteFile(fh, (LPSTR)lpBI, dwDIBSize, &dwWritten, NULL);

    GlobalUnlock(hDib);
    CloseHandle(fh);

    if (dwWritten == 0)
        return FALSE; // oops, something happened in the write
    else
        return 0; // Success code

	return true;
}

CString CWinPrint::fnGetIpAddress()
{
	WORD wVersionRequested;
	char  dst[32];
	char  host[128];
	struct hostent *pent;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 2, 0 );

	if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
	{

		gethostname(host, 127);
		pent = gethostbyname(host);
		if( pent == NULL ) {
			memcpy(dst, "000.000.000.000", 15);
		} else {
			int  i;
			unsigned char ip[4];
			memcpy(dst, "000.000.000.000", 15);
			for(i=0 ; ; i++) {
				if( pent->h_addr_list[i] == NULL )
					break;
				memcpy(ip, pent->h_addr_list[i], 4);
				sprintf(dst, "%u.%u.%u.%u",
						ip[0], ip[1], ip[2], ip[3]
						);
				if( memcmp(dst, "127.000.000.001", 15) != 0 )
					break;
			}
		}
		WSACleanup( );
	}
	return CString(dst);
}

BOOL CWinPrint::SaveImg( const char *pszFilename, HDIB hDIB)
{
	int nType = ExtensionIndex( pszFilename );

	if( nType < IMAGETYPE_FIRSTTYPE || nType > IMAGETYPE_LASTTYPE ) return( FALSE );
	

	switch( nType )
	{
		case IMAGETYPE_BMP:
			return( ::SaveBMP( pszFilename, hDIB ) );
			break;
		case IMAGETYPE_JPG:
			{
				int nQ = 120;
				return( ::SaveJPG( pszFilename, hDIB, nQ ) );
			}			
			break;
		case IMAGETYPE_PCX:
			return( ::SavePCX( pszFilename, hDIB ) );
			break;
	}

	return( TRUE );

}

int CWinPrint::ExtensionIndex( const char *pszFilename )
{
	int Index = 0;
	char *pszExtension;

	pszExtension = (char *) &pszFilename[strlen(pszFilename)-4];

	while( szExtensions[Index][0] )
	{
		if( !stricmp( pszExtension, szExtensions[Index] ) ) return( Index + 1 );
		Index++;
	}

	return -1;
}

HDIB CWinPrint::AddBMPFileHeader(HDIB hDIB)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure
    DWORD               dwDIBSize;
	HDIB				hNewDIB;

    if (!hDIB)
        return NULL;

	lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    if (!lpBI)
        return NULL;
    
	if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
	{
		GlobalUnlock(hDIB);
		return NULL;
	}

	// Partial Calculation
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  

    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
        dwDIBSize += lpBI->biSizeImage;
    else
    {
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only

        // It's not RLE, so size is Width (DWORD aligned) * Height
        dwBmBitsSize= WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
        dwDIBSize	+= dwBmBitsSize;

        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which 
        // have this field incorrect).
        lpBI->biSizeImage = dwBmBitsSize;
    }


	bmfHdr.bfType		= 0x4d42;  // "BM"
    bmfHdr.bfSize		= dwDIBSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1	= 0;
    bmfHdr.bfReserved2	= 0;
    bmfHdr.bfOffBits	= (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
            PaletteSize((LPSTR)lpBI);	


    hNewDIB				= GlobalAlloc (GHND, bmfHdr.bfSize);
	if(hNewDIB)
	{
		BYTE *	pNewDIB	= (BYTE *)GlobalLock(hNewDIB);
		if(pNewDIB)
		{
			BITMAPFILEHEADER *pBmpFile	= (BITMAPFILEHEADER *)pNewDIB;
			*pBmpFile					= bmfHdr;
			LPBITMAPINFOHEADER pBmpInfo = (LPBITMAPINFOHEADER)(pNewDIB + sizeof(BITMAPFILEHEADER));
			memcpy(pBmpInfo, lpBI, dwDIBSize);

			GlobalUnlock(hNewDIB);
		}
		else
		{
			GlobalFree(hNewDIB);
			hNewDIB = NULL;
		}
	}

	GlobalUnlock(hDIB);
	return hNewDIB;
}