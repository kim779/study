#ifndef _WINPRINT_H
#define _WINPRINT_H

// Print Options selection 
#define PW_BESTFIT       	1
#define PW_STRETCHTOPAGE 	2
#define PW_SCALE         	3	    
#define IDC_BESTFIT                     301
#define IDC_STRETCHTOPAGE               302
#define IDC_BOX2                    	200
#define IDC_PRINTTEXT1              	201
#define IDC_PRINTTEXT2              	202
#define IDC_PERCENTAGE              	203

#define IDC_BOX3                    	300
#define IDC_BESTFIT                     301
#define IDC_STRETCHTOPAGE               302
#define IDC_SCALE                       303
#define IDC_XAXIS                       304
#define IDC_XTEXT                       305
#define IDC_YTEXT                       306
#define IDC_YAXIS                       307

#define IDC_BOX4                    	400
#define IDOPTION                        401

#define IDC_CHECK_CONTINUE  			501

typedef struct tagOPTIONS {
    int iOption;       // Print Options
    int iXScale;       // X scale factor (if iOption is DLGPRINT_SCALE)
    int iYScale;       // Y scale factor ('')
    } OPTIONSTRUCT;

typedef OPTIONSTRUCT FAR *LPOPTIONSTRUCT;

// Handle to a DIB 
#define HDIB 				HANDLE

// Print Options selection 
#define PW_BESTFIT       	1
#define PW_STRETCHTOPAGE 	2
#define PW_SCALE         	3	
#define PALVERSION   		0x300
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

#define	INI_GRAPGUBUN		9

#endif
 


class CWinPrint
{
public:
         CWinPrint();
public: 

		//20080711 이문수 >>
		HWND		m_hMainWnd;
		HWND		m_hMDIClient;
		//20080711 이문수 <<
		CString		m_strName;

		HWND 		hDlgAbort;                    // Handle to Abort Dialog
		BOOL 		bAbort;               // Abort a print operation?
		char        szUserName[10];
		char 		gszDriver[50];                // Output device (e.g. "LPT1:")
		char 		gszDevice[50];                // Keeps track out device (e.g. "HP LaserJet")
		char 		gszOutput[50];                // Output device (e.g. "LPT1:")
		BOOL		BkgroundPlg;							// 텍스트, 그래픽 Tr 구별 

		HWND		ghWndFrame;

		PRINTDLG	pd;

		int 		gnWindowsTextSize;
		int 		bPrintContinue ;
		int 		nPrintPage;

		HDC 		hPrnDC;                  // 프린트 DC

		//extern HFONT fnNewFont(int nHeight, int nWidth, int nWeight, LPSTR szFace);
		//extern int fnRSpaceRemove(LPSTR lpString, int nStringCount);

        BOOL      m_Portrait;
		//char*   lpPrintBuff;         
		char   lpWindowTitle[50];
		int     nPrintHotKey;       // 프린트시 HAT KEY인지를 Check
		CWnd *  m_pParentWnd;
        CString m_strMsgLines[5];

public:
		// winprint.c function
	    BOOL fnPrintWindow(int niGubun,HWND hFrameWnd, HDC hPrintDC,CString strFileName,BOOL bPortrait , HDIB hDIB=NULL, CString strTitle = "" ,CString strMsg= "");		
		CString fnGetIpAddress();
		HWND SelectWindow();

		HDIB AddBMPFileHeader(HDIB hDIB);
		int	 ExtensionIndex( const char *pszFilename );
		BOOL SaveImg( const char *pszFilename, HDIB hDIB);
		HDIB CopyBmpToDIB(HBITMAP hBitmap);
		HDIB CopyWindowToDIB(HWND);		
		HDIB CopyScreenToDIB(HWND, LPRECT);
		HBITMAP CopyScreenToBitmap(HWND , LPRECT);
		

		HPALETTE GetSystemPalette ();
		int PalEntriesOnDevice (HDC);
		HANDLE BitmapToDIB (HBITMAP, HPALETTE);
		void InitBitmapInfoHeader (LPBITMAPINFOHEADER, DWORD, DWORD, int);
		LPSTR FindDIBBits (LPSTR);
		BOOL PrintDIB(HDIB,HDC, WORD, WORD, WORD);
		void CalculatePrintRect(HDC, LPRECT, WORD, DWORD, DWORD);  
		BOOL PrintBand(HDC, LPRECT, LPRECT, BOOL, BOOL, LPBITMAPINFOHEADER, LPSTR);
		WORD PaletteSize (LPSTR);
		void DestroyDIB(HDIB);
		BOOL PrintWindowDIB(HDIB,HDC); 
		HDC GetPrinterDC();
		WORD DIBNumColors (LPSTR);
		BOOL  PrintAbortDlg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		BOOL  PrintAbortProc(HDC hDC, int code);
		BOOL PrintDlgProc(HWND, WORD, WORD, LONG);
		void fnPrintDelete();

		//이미지를 화일로 저장하는 부분 
		BOOL SaveDIB(HANDLE hDib, LPSTR lpFileName);
public:
      ~CWinPrint();

};
