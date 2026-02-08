// BmpFileMaker.cpp: implementation of the CBmpFileMaker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "BmpFileMaker.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"	// for ML_LANGUAGE_RES

//jpg
#include "BMPFILE.h"
#include "JpegFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------
 - Function    :  MakeBmpFile
 - Created at  :  2003-10-28   09:20
 - Analysis    :  차트이미지를 memoryDC에 그리고 Bitmap파일에 저장하는 작업을 한다.
 -------------------------------------------------------------------------------*/
void CBmpFileMaker::MakeBmpFile(const int nWidth,
								const int nHeight,
								const CString& strFileName,
								IChartOCX *p_pIChartOCX)
{
	//	bitmap create
	CClientDC dc( CWnd::FromHandle( p_pIChartOCX->GetOcxHwnd()));
	CRect rcSource(0, 0, nWidth, nHeight);

	CDC memoryDC;
	CBitmap bufferBitmap;
	memoryDC.CreateCompatibleDC(&dc);
	bufferBitmap.CreateCompatibleBitmap(&dc, rcSource.Width(), rcSource.Height());

	CBitmap* pOldBitmap = (CBitmap*) memoryDC.SelectObject(&bufferBitmap);

	// 차트이미지를 memoryDC에 담는 작업을 한다.
	p_pIChartOCX->DrawForImage( memoryDC.GetSafeHdc(), rcSource, TRUE);

	// Bitmap파일로 저장을 한다.
	WriteToFile(memoryDC, bufferBitmap, strFileName, p_pIChartOCX);

	memoryDC.SelectObject(pOldBitmap);
	bufferBitmap.DeleteObject();
	memoryDC.DeleteDC();
}


/*-------------------------------------------------------------------------------
 - Function    :  WriteToFile
 - Created at  :  2003-10-23   14:45
 - Analysis    :  지정된 파일명으로 Bitmap을 저장한다. 이 Bitmap파일은 임시파일로 
				  '이미지저장'작업이 끝나면 삭제됨.
 -------------------------------------------------------------------------------*/
void CBmpFileMaker::WriteToFile(const CDC& memoryDC,
								CBitmap& bitmap,
								const CString& strFileName,
								IChartOCX *p_pIChartOCX)
{
	CPalette pal;
	if( memoryDC.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries( memoryDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

		delete[] pLP;
	}

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB(bitmap, BI_RGB, &pal, p_pIChartOCX);

	if( hDIB == NULL )
		return;

	// Write it to file
	if( ! WriteDIB((LPTSTR)(LPCTSTR)strFileName, hDIB ))
	{
		ML_SET_LANGUAGE_RES();
		CString strMsg;
		strMsg.LoadString( IDS_SAVE_SCREEN_FAILED);
		AfxMessageBox( strMsg);
	}

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree( hDIB );
}

HANDLE CBmpFileMaker::DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal,
								IChartOCX *p_pIChartOCX) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
//	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= 24; //bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	CClientDC dc( CWnd::FromHandle( p_pIChartOCX->GetOcxHwnd()));
	pPal = dc.SelectPalette(pPal, FALSE);
	dc.RealizePalette();

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		dc.SelectPalette(pPal, FALSE);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(dc.m_hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
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
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		dc.SelectPalette(pPal, FALSE);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits(dc.m_hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		dc.SelectPalette(pPal, FALSE);
		return NULL;
	}

	dc.SelectPalette(pPal, FALSE);
	return hDIB;
}

bool CBmpFileMaker::WriteDIB( LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)	{
		return false;
//		AfxMessageBox("요기서 리턴되나? 1");
	}

	CFile file;
	BOOL bopen = file.Open(szFile, CFile::modeCreate | CFile::modeWrite);
	if( !bopen )	{
//		AfxMessageBox("요기서 리턴되나? 2");
		return false;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	//SJ 2002.9.2 -> bmp 파일에 저장할 위치 잡기.
	//hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
	//					nColors * sizeof(RGBQUAD));
	hdr.bfOffBits		= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//end

	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
	file.Write( lpbi, GlobalSize(hDIB) );

	return true;
}


/*
void CBmpFileMaker::MakeJpgFile(CString& strFilePath)
{
	#define ALL_MARKS  0
 	#define WINDOWS_DEFAULT  2
	CImgEdit ImgEdit1;
	ImgEdit1.Create(NULL, WS_VISIBLE, CRect(0, 0, 10, 10), this, IDC_EDITCTRL1);
	ImgEdit1.SetImage(strFilePath);
	ImgEdit1.Display();
	VARIANT vPageType;
	ImgEdit1.SetFocus();
	VARIANT vRepaint; 
	V_VT(&vRepaint) = VT_BOOL; 
	V_BOOL(&vRepaint) = TRUE;
	VARIANT evt; V_VT(&evt) = VT_ERROR;       
	ImgEdit1.ConvertPageType(6,vRepaint);  
	// Save all marks in their original colors
    ImgEdit1.BurnInAnnotations(ALL_MARKS, WINDOWS_DEFAULT,evt);
	VARIANT vFileType;
	VARIANT vCompressionType;
	VARIANT	vCompressionInfo;
	V_VT(&vCompressionType) = VT_I2;
	V_VT(&vCompressionInfo) = VT_I2;
	V_I2(&vCompressionType) = 6;
	V_I2(&vCompressionInfo) = 4096;
	V_VT(&vFileType) = VT_I2; // set FileType for saveas
	V_VT(&vPageType) = VT_I2; // set to PageType for saveas
	V_I2(&vFileType) = 6;
	V_I2(&vPageType) = 6;
	strFilePath.Replace(".bmp", ".jpg");
//	CString strFilePath1 = dlg.m_strFilePath + "\\" + dlg.m_strFileName+"OCX" + ".jpg";
	ImgEdit1.SaveAs(strFilePath ,vFileType, vPageType, vCompressionType, vCompressionInfo, evt);
}
*/


void CBmpFileMaker::SaveJPG(const CString fileName,
							const int nWidth,
							const int nHeight,
							BYTE *pBitmapBuffer,
							BOOL color,
							IChartOCX *p_pIChartOCX)
{
	// note, because i'm lazy, most image data in this app
	// is handled as 24-bit images. this makes the DIB
	// conversion easier. 1,4,8, 15/16 and 32 bit DIBs are
	// significantly more difficult to handle.
	if (pBitmapBuffer == NULL) {
		AfxMessageBox("No Image!");
		return;
	}

	//뒤집기.
	// we vertical flip for display. undo that.
//	JpegFile::VertFlipBuf(pp, Width * 3, Height);

	//색깔 바꾸기
	// we swap red and blue for display, undo that.
//	JpegFile::BGRFromRGB(pBitmapBuffer, nWidth, nHeight);

	// save RGB packed buffer to JPG
	BOOL ok=JpegFile::RGBToJpegFile(fileName, 
									pBitmapBuffer,
									nWidth,
									nHeight,
									color, 
									75);			// quality value 1-100.
	if (!ok) 
	{
		AfxMessageBox("Write Error");
	} 
	else
	{
		// load what we just saved
		LoadJPG(fileName, nWidth, nHeight, pBitmapBuffer);
		p_pIChartOCX->InvalidateControl();
	}
}

void CBmpFileMaker::LoadJPG(const CString fileName,
							UINT nWidth,
							UINT nHeight,
							BYTE *pBitmapBuffer)
{
	// m_buf is the global buffer
	if (pBitmapBuffer != NULL) {
		pBitmapBuffer = NULL;
	}

	// read to buffer tmp
	pBitmapBuffer = JpegFile::JpegFileToRGB(fileName, &nWidth, &nHeight);

	//////////////////////
	// set up for display

	// do this before DWORD-alignment!!!
	// this works on packed (not DWORD-aligned) buffers
	// swap red and blue for display
	JpegFile::BGRFromRGB(pBitmapBuffer, nWidth, nHeight);

	// vertical flip for display
	JpegFile::VertFlipBuf(pBitmapBuffer, nWidth * 3, nHeight);

	// (2006/3/28 - Seung-Won, Bae) Release Bitmap Buffer
	//		Caution! This does not come from caller. It comes from JpegFile::JpegFileToRGB()
	if( pBitmapBuffer) delete [] pBitmapBuffer;
}

void CBmpFileMaker::LoadBMP(const CString fileName,
							UINT nWidth,
							UINT nHeight,
							BYTE *pBitmapBuffer)
{
	BMPFile theBmpFile;

	pBitmapBuffer = theBmpFile.LoadBMP(fileName, &nWidth, &nHeight);

	if ((pBitmapBuffer==NULL) || (theBmpFile.m_errorText!="OK")) 
	{
		AfxMessageBox(theBmpFile.m_errorText);
		pBitmapBuffer=NULL;
		return;
	}

	//////////////////////
	// set up for display

	// do this before DWORD-alignment!!!
	// this works on packed (not DWORD-aligned) buffers
	// swap red and blue for display
	JpegFile::BGRFromRGB(pBitmapBuffer, nWidth, nHeight);

	// vertical flip for display
	JpegFile::VertFlipBuf(pBitmapBuffer, nWidth * 3, nHeight);
}

void CBmpFileMaker::SaveBMP24(const CString fileName,
							const UINT nWidth,
							const UINT nHeight,
							BYTE *pBitmapBuffer,
							IChartOCX *p_pIChartOCX)
{
	// note, because i'm lazy, most image data in this app
	// is handled as 24-bit images. this makes the DIB
	// conversion easier. 1,4,8, 15/16 and 32 bit DIBs are
	// significantly more difficult to handle.

	if (pBitmapBuffer == NULL) {
		AfxMessageBox("No Image!");
		return;
	}

	// image in m_buf is already BGR and vertically flipped, so we don't need
	// to do that for this function.

	// i really should make an RGB to BMP fn.

	BMPFile theBmpFile;
	theBmpFile.SaveBMP(fileName,
						pBitmapBuffer,
						nWidth,
						nHeight);

	if (theBmpFile.m_errorText!="OK") 
		AfxMessageBox(theBmpFile.m_errorText, MB_ICONSTOP);
	else {
		// load what we just saved
		LoadBMP(fileName, nWidth, nHeight, pBitmapBuffer);
		p_pIChartOCX->InvalidateControl();
	}

}


/*-------------------------------------------------------------------------------
 - Function    :  MakeJpgFile
 - Created at  :  2003-10-23   14:24
 - Parameters  :  nWidth      - JPG이미지의 가로크기
 -                nHeight     - JPG이미지의 세로크기
 -                strFileName - JPG이미지 파일명(경로포함)
 -						(그러나 실제는 bmp확장자로 들어온다. 이유는 소스를 보십셔!)
 -                pCtrl       - 이 클래스를 사용하는 부모윈도우 포인터
 - Return Value:  None
 - Description :  (분석) BMP파일로 저장한다음 그 파일을 JPG포맷으로 변환 저장하는 함수
 -------------------------------------------------------------------------------*/
void CBmpFileMaker::MakeJpgFile(const int nWidth,
								const int nHeight,
								CString& strFileName,
								IChartOCX *p_pIChartOCX)
{
	// Bitmap파일로 저장한다.
	MakeBmpFile(nWidth, nHeight, strFileName, p_pIChartOCX);

	UINT m_width = (UINT)nWidth;
	UINT m_height = (UINT)nHeight;

	BMPFile theBmpFile;
	BYTE *pBmpBuffer;

	// 차트이미지를 저장한 Bitmap파일을 불러온다. 
	pBmpBuffer = theBmpFile.LoadBMP(strFileName, &m_width, &m_height);

	CString bmpPath = strFileName;
	// JPEG로 압축함수로 들어가는 곳
	strFileName.Replace(".bmp", ".jpg");
	SaveJPG(strFileName, m_width, m_height, pBmpBuffer, TRUE, p_pIChartOCX);

	CFile::Remove(bmpPath);
	
	// (2006/3/28 - Seung-Won, Bae) Release Bmp Buffer
	if( pBmpBuffer) delete [] pBmpBuffer;
}
