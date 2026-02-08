#include "stdafx.h"
#include "axisGView.h"
#include "grpView.h"
#include "fileIO.h"
#include "ndib.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFileIO::CFileIO()
{
}

CFileIO::~CFileIO()
{

}

bool CFileIO::ReadFile(CWnd *pView, CString path, char *pBytes, int nBytes)
{
	DWORD	dwAccess = GENERIC_READ;
	DWORD	dwShareMode = FILE_SHARE_READ;
	DWORD	dwDistribution = OPEN_EXISTING;

	HANDLE hFile = CreateFile(path.operator LPCTSTR(), dwAccess,
				dwShareMode, NULL, dwDistribution,
				FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("hFile Error [%d] on OpenHandle\n", GetLastError());
		AfxGetMainWnd()->MessageBox("오픈할 파일 생성에 실패했습니다.", "확인", MB_OK);

		return false;
	}

	char	*rWb = new char[nBytes + 1];
	DWORD	dwReadLen;

	BOOL bRc = ::ReadFile(hFile, rWb, nBytes, &dwReadLen, NULL);
	CloseHandle(hFile);
	if (dwReadLen != DWORD(nBytes))
	{
		BUF_DELETE(rWb);
		//delete rWb;
		return false;
	}
	CopyMemory(pBytes, rWb, nBytes);
	delete rWb;

	return true;
}

bool CFileIO::WriteFile(CWnd *pView, CString path, char *pBytes, int nBytes)
{
	DWORD	dwAccess = GENERIC_WRITE;
	DWORD	dwShareMode = FILE_SHARE_WRITE;
	DWORD	dwDistribution = CREATE_ALWAYS;

	HANDLE hFile = CreateFile(path.operator LPCTSTR(), dwAccess,
				dwShareMode, NULL, dwDistribution,
				FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("hFile Error [%d] on OpenHandle\n", GetLastError());
		AfxGetMainWnd()->MessageBox("저장할 파일 생성에 실패했습니다.", "확인", MB_OK);

		return false;
	}

	DWORD	dwWriteLen;
	BOOL bRc = ::WriteFile(hFile, pBytes, nBytes, &dwWriteLen, NULL);
	CloseHandle(hFile);
	if (!bRc)
	{
		AfxGetMainWnd()->MessageBox("저장할 파일 생성에 실패했습니다.", "확인", MB_OK);
		return false;
	}
	return true;
}

bool CFileIO::GetFilenameInPath(CString &strPath, CString name, CStringArray &aryFname)
{
	aryFname.RemoveAll();

	HANDLE	hFile;
	BOOL	bSuccess = FALSE;
	WIN32_FIND_DATA	findfiledata;

	hFile = ::FindFirstFile(strPath.operator LPCTSTR(), &findfiledata);
	if (hFile == INVALID_HANDLE_VALUE)	return FALSE;

	name = name + "_";
	UINT	cLen = name.GetLength();
	CString fname;

	if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (strlen(findfiledata.cFileName) > cLen)
		{
			fname.Format("%s", findfiledata.cFileName);
			if (fname.Left(cLen) == name)
				aryFname.Add(fname.Mid(cLen));
		}
	}
	while(1)
	{
		bSuccess = ::FindNextFile(hFile, &findfiledata);
		if (!bSuccess)
		{
			::FindClose(hFile);
			break;
		}
		if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (strlen(findfiledata.cFileName) <= cLen)
				continue;

			fname.Format("%s", findfiledata.cFileName);
			if (fname.Left(cLen) == name)
				aryFname.Add(fname.Mid(cLen));
		}
	}
	return TRUE;
}

BOOL CFileIO::ImageWindow(CWnd *pMain, CString& path)
{
	CBitmap 	bitmap;
	CWindowDC	dc(pMain);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pMain->GetWindowRect(&rect);

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
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)
		return FALSE;

	CFileDialog	dlg(FALSE, "bmp", path, OFN_OVERWRITEPROMPT, 
		"BMP Files(*.bmp)|*.bmp||", NULL);
	if (dlg.DoModal() == IDOK)
	{
		path = dlg.GetPathName();
		if (dlg.GetFileName().IsEmpty())
		{
			GlobalFree(hDIB);
			return FALSE;	
		}
	}
	else
	{
		GlobalFree(hDIB);
		return FALSE;
	}

	//szFile = (LPSTR)(LPCTSTR)path;
	// Write it to file
	WriteDIB((LPSTR)(LPCTSTR)path, hDIB);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree(hDIB);
	return TRUE;
}

void CFileIO::PrintWindow(CString root, CWnd *pMain, bool bDlgPopup)
{
	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pMain);
	CDC 		memDC;
	CRect		rect;

	memDC.CreateCompatibleDC(&dc); 
	pMain->GetWindowRect(&rect);

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
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)
		return;

	file.Format("%s\\%d", root, (int)pMain);
	WriteDIB((LPSTR)(LPCTSTR)file, hDIB);
	GlobalFree(hDIB);

	CNDib*	nDib = new CNDib();
	if (nDib->SetNDIB(file))
	{
		DeleteFile(file);
		delete nDib;
		return;
	}

	HANDLE	pDib = nDib->GetNDIB();
	BITMAP	bm;
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);


	CDC	printDC, *pDC = pMain->GetDC();
////////////////////////////////////////////////// 설정창
	PRINTDLG	pd;
	memset(&pd, 0, sizeof(PRINTDLG));
	// Get MFC's printer
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		// default setting page Orientation horizontally
		LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;  // DMORIENT_PORTRATE // 세로
		::GlobalUnlock(pd.hDevMode); 
	}

	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = NULL;
	pd.nFromPage = 1;
	pd.nToPage = 1;
	pd.nMinPage = 1;
	pd.nMaxPage = 1;
	pd.nCopies = 1;

	PrintDlg(&pd);
	if (!printDC.Attach(pd.hDC))
	{
		DeleteFile(file);
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}
//////////////////////////////////////////////////

//	hdcPrinterDC = pd.hDC;        // 프린트 DC초기화 
	//HDC hdcPrinter = printDlg.GetPrinterDC();    //Printer의 DC를 얻어온다.
//	if (hdcPrinterDC == NULL)	//인쇄 다이얼로그에서 취소했을때..
//	    return;

/*	m_pPrinterSettings->SetThisPrinter();
	AfxGetApp()->CreatePrinterDC(printDC);
	if (!printDC)
	{
		DeleteFile(file);
		ReleaseDC(pDC);
		delete nDib;
		return;
	}*/	

	SetMapMode(printDC.m_hDC, MM_TEXT);
	DOCINFO docinfo = { sizeof(DOCINFO), "차트인쇄", NULL};
 	if(!StartDoc(printDC.m_hDC, &docinfo))
	{
		DeleteFile(file);
		printDC.Detach();
		printDC.DeleteDC();
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}

	int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
	int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);

	int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
	int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
	int	deviceX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
	int	deviceY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
	
	int	width  = (int) (bm.bmWidth*printX/deviceX);
	int	height = (int) (bm.bmHeight*printY/deviceY);
	int	pagexN = (int) ((width+paperX-1)/paperX);
	int	pageyN = (int) ((height+paperY-1)/paperY);

	::SetStretchBltMode(printDC.m_hDC, COLORONCOLOR);

	LPBITMAPINFOHEADER lpbi;
	LPSTR        pBuf;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(pDib);
	if (!lpbi)
	{
		DeleteFile(file);
		printDC.Detach();
		printDC.DeleteDC();
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}
	pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + nDib->PaletteSize(lpbi);

	if (pagexN == 1 && pageyN == 1)
	{
		if(StartPage(printDC.m_hDC))
		{
			// Left Top
			int ret = StretchDIBits(printDC.m_hDC, 0, 0, width ,height, 0, 0, 
				bm.bmWidth, bm.bmHeight, pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
			EndPage(printDC.m_hDC);
		}
	}
	else
	{
		int iGapX =  width - paperX;
		int iGapY =  height - paperY;

		// 가로가 짤릴때
		if (iGapX >= iGapY)
		{
			height = (int)((double)height * (double)paperX / (double)width);
			width = paperX;
		}
		// 세로가 짤릴때
		else
		{
			width = (int)((double)width * (double)paperY / (double)height);
			height = paperY;
		}

		if(StartPage(printDC.m_hDC))
		{
			// Left Top
			int iReturn = StretchDIBits(	printDC.m_hDC, 0, 0, width, height,
							0, 0, bm.bmWidth, bm.bmHeight,
							pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
			EndPage(printDC.m_hDC);
		}
	}
	EndDoc(printDC.m_hDC);
//	m_pPrinterSettings->RestorePrinter();
	
	DeleteObject(hBitmap);
	DeleteFile(file);
	printDC.Detach();
	printDC.DeleteDC();
	pMain->ReleaseDC(pDC);
	delete nDib;
}

void CFileIO::PrintWindow(CString root, CWnd *pMain, CWnd *pBottom, CString title, CFont *pFont, bool bDlgPopup)
{
	CString		file;
	CBitmap 	bitmap;
	CWindowDC	dc(pMain);
	CDC 		memDC;

	memDC.CreateCompatibleDC(&dc); 
/////////////////////////////////////////	draw chart
	CRect	WinRC, TRC, BRC;

	pBottom->GetWindowRect(BRC);
	TRC.SetRect(0, 0, BRC.Width(), 15);
	CDC *pBDC = pBottom->GetDC();

	WinRC = BRC;
	WinRC.bottom += TRC.Height();
	
	bitmap.CreateCompatibleBitmap(&dc, WinRC.Width(), WinRC.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, TRC.Height(), BRC.Width(), BRC.Height(), pBDC, 0, 0, SRCCOPY);

////////////////////////////////////////	draw text
	CBrush	cBrush(RGB(255,255,255));
	memDC.FillRect(&TRC, &cBrush);

	COLORREF sColor = memDC.SetTextColor(RGB(0,0,0));
	COLORREF sbkColor = memDC.SetBkColor(RGB(255,255,255));
	CFont	*sFont = memDC.SelectObject(pFont);

	CSize	sz = memDC.GetOutputTextExtent(title);
	CRect	rc = TRC;
	rc.left = (TRC.Width() - sz.cx)/2;
	memDC.DrawText(title, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDC.SelectObject(sFont);
	memDC.SetTextColor(sColor);
	memDC.SetBkColor(sbkColor);
	pBottom->ReleaseDC(pBDC);
//////////////////////////////////////


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
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal);
	if(hDIB == NULL)
		return;

	file.Format("%s\\%d", root, (int)pMain);
	WriteDIB((LPSTR)(LPCTSTR)file, hDIB);
	GlobalFree(hDIB);

	CNDib*	nDib = new CNDib();
	if (nDib->SetNDIB(file))
	{
		DeleteFile(file);
		delete nDib;
		return;
	}

	HANDLE	pDib = nDib->GetNDIB();
	BITMAP	bm;
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);


	CDC	printDC, *pDC = pMain->GetDC();
///////////////////////////////////////////// 설정창
	PRINTDLG	pd;
	memset(&pd, 0, sizeof(PRINTDLG));
	// Get MFC's printer
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		// default setting page Orientation horizontally
		LPDEVMODE pDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
		pDevMode->dmOrientation = DMORIENT_LANDSCAPE;  // DMORIENT_PORTRATE // 세로
		::GlobalUnlock(pd.hDevMode); 
	}

	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = NULL;
	pd.nFromPage = 1;
	pd.nToPage = 1;
	pd.nMinPage = 1;
	pd.nMaxPage = 1;
	pd.nCopies = 1;

	PrintDlg(&pd);
	if (!printDC.Attach(pd.hDC))
	{
		DeleteFile(file);
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}
///////////////////////////////////////////////////////

//	hdcPrinterDC = pd.hDC;        // 프린트 DC초기화 
	//HDC hdcPrinter = printDlg.GetPrinterDC();    //Printer의 DC를 얻어온다.
//	if (hdcPrinterDC == NULL)	//인쇄 다이얼로그에서 취소했을때..
//	    return;

/*	m_pPrinterSettings->SetThisPrinter();
	AfxGetApp()->CreatePrinterDC(printDC);
	if (!printDC)
	{
		DeleteFile(file);
		ReleaseDC(pDC);
		delete nDib;
		return;
	}*/

	SetMapMode(printDC.m_hDC, MM_TEXT);
	DOCINFO docinfo = { sizeof(DOCINFO), "차트인쇄", NULL};
 	if(!StartDoc(printDC.m_hDC, &docinfo))
	{
		DeleteFile(file);
		printDC.Detach();
		printDC.DeleteDC();
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}

	int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
	int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);

	int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
	int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
	int	deviceX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
	int	deviceY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);
	
	int	width  = (int) (bm.bmWidth*printX/deviceX);
	int	height = (int) (bm.bmHeight*printY/deviceY);
	int	pagexN = (int) ((width+paperX-1)/paperX);
	int	pageyN = (int) ((height+paperY-1)/paperY);

	::SetStretchBltMode(printDC.m_hDC, COLORONCOLOR);

	LPBITMAPINFOHEADER lpbi;
	LPSTR        pBuf;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(pDib);
	if (!lpbi)
	{
		DeleteFile(file);
		printDC.Detach();
		printDC.DeleteDC();
		pMain->ReleaseDC(pDC);
		delete nDib;
		return;
	}
	pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + nDib->PaletteSize(lpbi);

	if (pagexN == 1 && pageyN == 1)
	{
		if(StartPage(printDC.m_hDC))
		{
			// Left Top
			int ret = StretchDIBits(printDC.m_hDC, 0, 0, width ,height, 0, 0, 
				bm.bmWidth, bm.bmHeight, pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
			EndPage(printDC.m_hDC);
		}
	}
	else
	{
		int iGapX =  width - paperX;
		int iGapY =  height - paperY;

		// 가로가 짤릴때
		if (iGapX >= iGapY)
		{
			height = (int)((double)height * (double)paperX / (double)width);
			width = paperX;
		}
		// 세로가 짤릴때
		else
		{
			width = (int)((double)width * (double)paperY / (double)height);
			height = paperY;
		}

		if(StartPage(printDC.m_hDC))
		{
			// Left Top
			int iReturn = StretchDIBits(	printDC.m_hDC, 0, 0, width, height,
							0, 0, bm.bmWidth, bm.bmHeight,
							pBuf, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS, SRCCOPY);
			EndPage(printDC.m_hDC);
		}
	}
	EndDoc(printDC.m_hDC);
//	m_pPrinterSettings->RestorePrinter();
	
	DeleteObject(hBitmap);
	DeleteFile(file);
	printDC.Detach();
	printDC.DeleteDC();
	pMain->ReleaseDC(pDC);
	delete nDib;
}

HANDLE CFileIO::DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal) 
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

// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
BOOL CFileIO::WriteDIB(LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if(!file.Open(szFile, CFile::modeWrite|CFile::modeCreate))
		return FALSE;

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

	return TRUE;
}
