// Draw.cpp: implementation of the CAxisDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axislib.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxisDraw::CAxisDraw()
{
}

CAxisDraw::~CAxisDraw()
{
	RemoveAllFont();
	RemoveAllPen();
	RemoveAllBrush();
	RemoveAllBitmap();
}

CFont* CAxisDraw::SetFont(CDC *pDC, char *fontN, int point, bool italic, int bold)
{
	CString	keys = _T("");
	CFont*	font = nullptr;

	keys.Format(_T("%s:%d:%c:%d"), fontN, point, italic ? 'I' : 'i', bold);
	if (!m_pointFontObs.Lookup(keys, (CObject*&)font))
	{
		LOGFONT	logfont;

		font = new CFont();
		logfont.lfHeight         = point * 10 + (point < 9 ? 5 : 0);
		logfont.lfWidth          = 0;
		logfont.lfEscapement     = 0;
		logfont.lfOrientation    = 0;
		logfont.lfWeight         = bold;
		logfont.lfItalic         = italic;
		logfont.lfUnderline      = false;
		logfont.lfStrikeOut      = false;
		logfont.lfCharSet	 = HANGUL_CHARSET;
		logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality        = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, fontN);

		ModifyFontsize(logfont.lfFaceName, logfont.lfHeight);

		font->CreatePointFontIndirect(&logfont);
		m_pointFontObs.SetAt(keys, font);
	}
	pDC->SelectObject(font);
	return font;
}

void CAxisDraw::SetPen(CDC *pDC, COLORREF crRGB, int width, int style)
{
	CString	keys = _T("");
	CPen*	pen = nullptr;

	keys.Format("%d:%d:%d", crRGB, width, style);
	if (!m_penObs.Lookup(keys, (CObject*&)pen))
	{
		pen = new CPen();
		pen->CreatePen(style, width, crRGB);
		m_penObs.SetAt(keys, pen);
	}
	pDC->SelectObject(pen);
}

CBrush* CAxisDraw::SetBrush(CDC *pDC, COLORREF crRGB)
{
	CBrush*	brush = nullptr;

	if (!m_brushObs.Lookup(crRGB, brush))
	{
		brush = new CBrush();
		brush->CreateSolidBrush(crRGB);
		m_brushObs.SetAt(crRGB, brush);
	}
	pDC->SelectObject(brush);
	return brush;
}

CBitmap* CAxisDraw::SetBitmap(char* fileN)
{
	if (fileN == NULL || strlen(fileN) <= 0)
		return NULL;

	time_t	tm = 0;
	CFileStatus	status = {};
	Cbitmap* bmp  = NULL;
	CString	files = CString(fileN);

	files.MakeUpper();
	if (CFile::GetStatus(files, status))
		tm = status.m_mtime.GetTime();
	if (m_bmpObs.Lookup(files, (void*&)bmp))
	{
		if (bmp->m_mtime == tm)
			return  bmp->m_bitmap;

		m_bmpObs.RemoveKey(files);
		delete bmp;
	}

	bmp = new Cbitmap;
	if (bmp->Load(files))
	{
		bmp->m_mtime = tm;
		m_bmpObs.SetAt(files, bmp);
		return bmp->m_bitmap;
	}
	delete bmp;
	return NULL;
}

CFont* CAxisDraw::SetFont(struct _fontR* fontR)
{
	CString	keys = _T("");
	CFont*	font = nullptr;

	keys.Format(_T("%s:%d:%c:%d"), fontR->name, fontR->point, fontR->italic ? 'I' : 'i', fontR->bold);
	if (!m_pointFontObs.Lookup(keys, (CObject*&)font))
	{
		LOGFONT	logfont;

		font = new CFont();
		logfont.lfHeight         = fontR->point * 10 + (fontR->point < 9 ? 5 : 0);
		logfont.lfWidth          = 0;
		logfont.lfEscapement     = 0;
		logfont.lfOrientation    = 0;
		logfont.lfWeight         = fontR->bold;
		logfont.lfItalic         = fontR->italic;
		logfont.lfUnderline      = false;
		logfont.lfStrikeOut      = false;
		logfont.lfCharSet	 = HANGUL_CHARSET;
		logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality        = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, fontR->name);

		ModifyFontsize(logfont.lfFaceName, logfont.lfHeight);

		font->CreatePointFontIndirect(&logfont);
		m_pointFontObs.SetAt(keys, font);
	}
	return font;
}

CPen* CAxisDraw::SetPen(struct _penR* penR)
{
	CString	keys = _T("");
	CPen*	pen = nullptr;

	keys.Format(_T("%d:%d:%d"), penR->clr, penR->width, penR->style);
	if (!m_penObs.Lookup(keys, (CObject*&)pen))
	{
		pen = new CPen();
		pen->CreatePen(penR->style, penR->width, penR->clr);
		m_penObs.SetAt(keys, pen);
	}
	return pen;
}

CBrush* CAxisDraw::SetBrush(COLORREF crRGB)
{
	CBrush*	brush = nullptr;

	if (!m_brushObs.Lookup(crRGB, brush))
	{
		brush = new CBrush();
		brush->CreateSolidBrush(crRGB);
		m_brushObs.SetAt(crRGB, brush);
	}
	return brush;
}

void CAxisDraw::RemoveAllFont()
{
	CFont*	font = nullptr;
	CString	key = _T("");

	for (POSITION pos = m_pointFontObs.GetStartPosition(); pos != NULL; )
	{
		m_pointFontObs.GetNextAssoc(pos, key, (CObject *&)font);
		font->DeleteObject();
		delete font;
	}
	m_pointFontObs.RemoveAll();
}

void CAxisDraw::RemoveAllPen()
{
	CPen*	pen = nullptr;
	CString	key = _T("");
	
	for (POSITION pos = m_penObs.GetStartPosition(); pos != NULL; )
	{
		m_penObs.GetNextAssoc(pos, key, (CObject *&)pen);
		pen->DeleteObject();
		delete pen;
	}
	m_penObs.RemoveAll();
}

void CAxisDraw::RemoveAllBrush()
{
	CBrush*	brush = nullptr;
	COLORREF key = (COLORREF)0;
	
	for (POSITION pos = m_brushObs.GetStartPosition(); pos != NULL; )
	{
		m_brushObs.GetNextAssoc(pos, key, brush);
		brush->DeleteObject();
		delete brush;
	}
	m_brushObs.RemoveAll();
}

void CAxisDraw::RemoveAllBitmap()
{
	Cbitmap* bmp = nullptr;
	CString	key = _T("");
	
	for (POSITION pos = m_bmpObs.GetStartPosition(); pos != NULL; )
	{
		m_bmpObs.GetNextAssoc(pos, key, (void *&)bmp);
		delete bmp;
	}
	m_bmpObs.RemoveAll();
}

// updateXXX_20220725
#include <VersionHelpers.h>
void CAxisDraw::ModifyFontsize(CString fName, long& height)
{
	if (!IsWindows8OrGreater())
		return;

	if (height >= 110)
		height -= 2;

	return;
}

//--------------------------------------------------------------------------------------
//	class Cbitmap
//
Cbitmap::Cbitmap()
{
	m_bitmap  = NULL;
	m_picture = NULL;
	m_mtime = (time_t)0;
}

Cbitmap::~Cbitmap()
{
//	if (m_picture)
//	{
//		m_picture->Release();
//		m_picture = NULL;
//	}

	if (m_bitmap)
	{
		m_bitmap->DeleteObject();
		delete m_bitmap;
	}
}

/*
bool Cbitmap::Load(LPCTSTR file)
{
	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}

	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	int	size = 0;
	DWORD	read = 0;
	ULONG	realread;
	char	twb[1024*8]{};
	IStream* stream;
	HBITMAP	hbitmap;

	CreateStreamOnHGlobal(NULL, TRUE, &stream);
	for (ReadFile(hFile, twb, sizeof(twb), &read, NULL); read != 0;
		read = 0, ReadFile(hFile, twb, sizeof(twb), &read, NULL))
	{						// file -> stream
		size += read;
		stream->Write(twb, read, &realread);
	}
	CloseHandle(hFile);

	LARGE_INTEGER	move;
	move.QuadPart = 0;
	stream->Seek(move, STREAM_SEEK_SET, NULL);	// set stream position

							// create IPicture from stream
	OleLoadPicture(stream, size, FALSE, IID_IPicture, (LPVOID*)&m_picture);
	if (m_picture == NULL)
	{
		stream->Release();
		return false;
	}
	stream->Release();

	if (getHandle(hbitmap))
	{
		m_bitmap = new CBitmap;
		m_bitmap->Attach(hbitmap);
		return true;
	}

	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}
	return false;
}
*/

bool Cbitmap::Load(LPCTSTR fileN)
{
	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}

	CFile	file;
        DWORD	dwSize = 0, dwBytesRead = 0; 
        LPVOID	pData = NULL; 
        HGLOBAL hGlobal = (HGLOBAL) 0;
	HBITMAP	hbitmap = (HBITMAP) 0;

	if (!file.Open(fileN, CFile::modeRead|CFile::shareDenyNone|CFile::typeBinary))
		return false;

        dwSize = (DWORD)file.GetLength();
	if (dwSize < 0)
	{
		file.Close();
		return false;
	}

        hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if (hGlobal == NULL)
	{
		file.Close();
		return false;
	}

        pData = ::GlobalLock(hGlobal);
	if (pData == NULL)
	{
		file.Close();
		::GlobalFree(hGlobal);
		return false;
	}

        dwBytesRead = file.Read(pData, dwSize);
	if (dwBytesRead != dwSize)
	{
		file.Close();
		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
		return false;
	}
        ::GlobalUnlock(hGlobal);
        file.Close();

	IStream* stream;
	HRESULT	hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, &stream);
	if (FAILED(hr))
	{
		::GlobalFree(hGlobal);
		return false;
	}

	LARGE_INTEGER	move;
	move.QuadPart = 0;
	stream->Seek(move, STREAM_SEEK_SET, NULL);	// set stream position

							// create IPicture from stream
	hr = ::OleLoadPicture(stream, dwSize, FALSE, IID_IPicture, (LPVOID*)&m_picture); 
	stream->Release();
	::GlobalFree(hGlobal);
	if (FAILED(hr) || m_picture == NULL)
		return false;

	if (getHandle(hbitmap))
	{
		m_bitmap = new CBitmap;
		m_bitmap->Attach(hbitmap);

		m_picture->Release();
		m_picture = NULL;
		return true;
	}

	m_picture->Release();
	m_picture = NULL;
	return false;
}

bool Cbitmap::getHandle(HBITMAP& bitmap)
{
	if (m_picture == NULL)
		return false;

	OLE_XSIZE_HIMETRIC cx = 0;
	OLE_YSIZE_HIMETRIC cy = 0;
							// calulate size from IPicture
	m_picture->get_Width(&cx);
	m_picture->get_Height(&cy);

	HDC memDC = CreateCompatibleDC(NULL);

	int w = MulDiv(cx, GetDeviceCaps(memDC, LOGPIXELSX), HIMETRIC_INCH);
	int h = MulDiv(cy, GetDeviceCaps(memDC, LOGPIXELSY), HIMETRIC_INCH);

	DWORD*	pBitmapBits = nullptr;
	BITMAPINFO bmi = {};

	ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;

	bitmap = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, (VOID**)&pBitmapBits, NULL, 0 );
	if (bitmap == NULL)
	{
		DeleteDC(memDC);
		return false;
	}
	SetMapMode(memDC, MM_TEXT);
	HBITMAP o_bitmap = (HBITMAP)SelectObject(memDC, bitmap);
							// IPicture -> HBITMAP
	CRect Bounds(0, 0, 0, 0);
	m_picture->Render(memDC, 0, 0, w, h, 0, cy - 1, cx, -cy, &Bounds);

	SelectObject(memDC, o_bitmap);
	DeleteDC(memDC);
	return true;
}
