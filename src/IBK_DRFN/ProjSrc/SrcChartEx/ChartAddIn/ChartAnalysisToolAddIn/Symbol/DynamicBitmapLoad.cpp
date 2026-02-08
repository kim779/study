// DynamicBitmapLoad.cpp: implementation of the CDynamicBitmapLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "DynamicBitmapLoad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DynamicBitmapLoadButton.h"

//file
#include <fcntl.h>
#include <io.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDynamicBitmapLoad::CDynamicBitmapLoad():
	  m_nRow(1),
	  m_nButtonGap(11),
	  m_ptOrgButton(m_nButtonGap, m_nButtonGap)
{
}

CDynamicBitmapLoad::~CDynamicBitmapLoad()
{

}


void CDynamicBitmapLoad::AttachBitmapToButton(CPoint& btOrgPt, int btWidth, int btHeight, CDynamicBitmapLoadButton* button, CString upImageFileName, CString downImageFileName)
{
	CString up = GetFilePath(upImageFileName);
	CString down = GetFilePath(downImageFileName);
	if(up == "NULL" && down == "NULL"){
		button->ShowWindow(SW_HIDE);
		return;
	}

	if(up == "NULL")
		up = down;
	else if(down == "NULL")
		down = up;

	AutoLoad(button, up, down);
	ButtonMoveWindow(button, btOrgPt, btWidth, btHeight);
}

bool CDynamicBitmapLoad::GetImageSize(int& x, int& y)
{
	CString filePath;
	if((filePath = GetFilePath("left_up.bmp")) == "NULL")
		return false;

	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

	CBitmap bitmap;
	bitmap.Attach(hBitmap);

	BITMAP bitMap;
	bitmap.GetBitmap(&bitMap);

	x = bitMap.bmWidth;
	y = bitMap.bmHeight;
	
	return true;
}

/*
// Button 사이에 공간을 둔다.
void CDynamicBitmapLoad::ButtonDivision(CPoint& btOrgPt)
{
	btOrgPt.y = btOrgPt.y + m_nButtonGap;
}
*/
// 이미지를 Button에 로드
void CDynamicBitmapLoad::AutoLoad(CDynamicBitmapLoadButton* button, CString upImageFileName, CString downImageFileName)
{
	button->SetButtonStyle(BS_OWNERDRAW);
	button->Load(upImageFileName, downImageFileName);
	button->SizeToContent();
}

// 이미지 크기에 맞게 ToolBar Dlg의 크기를 조절한다.
void CDynamicBitmapLoad::ButtonMoveWindow(CDynamicBitmapLoadButton* button, CPoint& btOrgPt, int btWidth, int btHeight)
{
/*
	CRect rc;
	button->GetWindowRect(&rc);
//	rc.right = rc.left + btWidth;
//	rc.bottom = rc.top + btHeight;
	button->ScreenToClient(&rc);
*/
	CRect rect(btOrgPt.x, btOrgPt.y, btOrgPt.x + btWidth, btOrgPt.y + btHeight);
	button->MoveWindow(&rect);

	// 다음 버튼의 시작위치를 정한다.
	btOrgPt.x = rect.right;
//	btOrgPt.y = rect.bottom;

	//전체 Dialog 크기, 위치.
//	MoveWindow(0, 0, rect.right + 4, rect.bottom + 4);
//	MoveWindow(0, 0, rect.right + m_nButtonGap, rect.bottom + m_nButtonGap);
}

void CDynamicBitmapLoad::LoadBitmap(CDynamicBitmapLoadButton *pbutton, const CString& upfilepath, const CString& downfilepath)
{
	int x = 0, y = 0;
	GetImageSize(x, y);
	if(m_ptOrgButton.x == 95) {//일렬로 된 버튼이 아니다. 그래서 필요함.
	   m_ptOrgButton.x = m_nButtonGap;
	   m_ptOrgButton.y = m_nButtonGap + (y * m_nRow);
	   m_nRow++;
	}
	AttachBitmapToButton(m_ptOrgButton, x, y, pbutton, upfilepath, downfilepath);
}

// 파일 경로를 읽어온다.
CString CDynamicBitmapLoad::GetFilePath(CString fileName)
{
//	CString path = ((CDrChartCtrl*)m_wndParent)->GetSaveFileName(fileName, m_strDeskPath + FILE_IMAGE_PATH, TRUE);
//	CString path = m_pMainSymbol->GetFilePath() + fileName;
	CString path = m_strTotalPath + fileName;

	// 해당 파일이 있는지 체크한다.
	_finddata_t c_File;
	long hFile = _findfirst(path, &c_File);
	if(hFile == -1)
		return CString("NULL");

	return path;
}

void CDynamicBitmapLoad::LoadButtonBitmap(LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal )
{
/*
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,		// handle to device context 
				&bmiHeader,	// pointer to bitmap size and format data 
				CBM_INIT,	// initialization flag 
				lpDIBBits,	// pointer to initialization data 
				&bmInfo,	// pointer to bitmap color-format data 
				DIB_RGB_COLORS);		// color-data usage 
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;
*/
}
/*
			bitmap.LoadBitmap(IDB_DEFLATE2);
			((CButton*)m_wndToolBar->GetDlgItem(IDC_DEFLATE))->SetBitmap(bitmap);
*/

/*
void DrawBitmap(CDC* pDC, CBitmap& bitmap, CPalette *pPal )
{
	// Create a compatible memory DC
	CDC memDC;
	memDC.CreateCompatibleDC( pDC );
	memDC.SelectObject( &bitmap );

	// Select and realize the palette
	if( pPal != NULL && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		pDC->SelectPalette( pPal, FALSE );
		pDC->RealizePalette();
	}

	BITMAP bm;
	bitmap.GetBitmap( &bm );

	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0,SRCCOPY);
}

*/
/*
BOOL CMyBitmap::LoadBitmap(LPCTSTR szFilename) 
{ 
	ASSERT(szFilename);
	DeleteObject();

	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
	return Attach(hBitmap); 
}
*/

void CDynamicBitmapLoad::SetFilePath(const CString &filePath)
{
	m_strTotalPath = filePath;
}
