#include "stdafx.h"
#include "draw.h"


CAxDraw::CAxDraw(void)
{
	m_penObs.RemoveAll();
	m_brushObs.RemoveAll();
	m_fontObs.RemoveAll();
	m_imgObs.RemoveAll();
}


CAxDraw::~CAxDraw(void)
{
	removePen();
	removeBrush();
	removeFont();
	removeImage();
}

CFont* CAxDraw::SetFont(CString fontN, int point, bool italic, int bold)
{
	CString	keys;
	CFont*	font;
	LOGFONT	logfont;

	keys.Format("%s:%d:%c:%d", fontN, point, italic ? 'I' : 'i', bold);
	if (!m_fontObs.Lookup(keys, (CObject*&)font))
	{
		font = new CFont();

		logfont.lfHeight         = point * 10 + (point < 9 ? 5 : 0);
		logfont.lfWidth          = 0;
		logfont.lfEscapement     = 0;
		logfont.lfOrientation    = 0;
		logfont.lfWeight         = bold;
		logfont.lfItalic         = italic;
		logfont.lfUnderline      = false;
		logfont.lfStrikeOut      = false;
		logfont.lfCharSet        = DEFAULT_CHARSET;
		logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality        = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, fontN);

		font->CreatePointFontIndirect(&logfont);
		m_fontObs.SetAt(keys, font);
	}
	return font;
}

CPen* CAxDraw::SetPen(COLORREF crRGB, int width, int style)
{
	CString	keys;
	CPen*	pen;

	keys.Format("%d:%d:%d", crRGB, width, style);
	if (!m_penObs.Lookup(keys, (CObject*&)pen))
	{
		pen = new CPen();
		pen->CreatePen(style, width, crRGB);
		m_penObs.SetAt(keys, pen);
	}
	return pen;
}

CBrush* CAxDraw::SetBrush(COLORREF crRGB)
{
	CBrush*	brush;

	if (!m_brushObs.Lookup(crRGB, brush))
	{
		brush = new CBrush();
		brush->CreateSolidBrush(crRGB);
		m_brushObs.SetAt(crRGB, brush);
	}
	return brush;
}

void* CAxDraw::SetImage(CString fileN)
{
	if (fileN.IsEmpty())
		return NULL;

	time_t	tm = 0;
	CFileStatus	status;
	CaxImage*	img  = NULL;
	CString	files = CString(fileN);

	files.MakeUpper();
	if (CFile::GetStatus(files, status))
		tm = status.m_mtime.GetTime();
	if (m_imgObs.Lookup(files, (void*&)img))
	{
		if (img->m_mtime == tm)
			return img->m_image;

		m_imgObs.RemoveKey(files);
		delete img;
	}

	img = new CaxImage;
	if (img->Load(files))
	{
		img->m_mtime = tm;
		m_imgObs.SetAt(files, img);
		return img->m_image;
	}
	delete img;
	return NULL;
}

void CAxDraw::removeFont()
{
	CFont*	font;
	CString	key;

	for (POSITION pos = m_fontObs.GetStartPosition(); pos != NULL; )
	{
		m_fontObs.GetNextAssoc(pos, key, (CObject *&)font);
		font->DeleteObject();
		delete font;
	}
	m_fontObs.RemoveAll();
}

void CAxDraw::removePen()
{
	CPen*	pen;
	CString	key;
	
	for (POSITION pos = m_penObs.GetStartPosition(); pos != NULL; )
	{
		m_penObs.GetNextAssoc(pos, key, (CObject *&)pen);
		pen->DeleteObject();
		delete pen;
	}
	m_penObs.RemoveAll();
}

void CAxDraw::removeBrush()
{
	CBrush*	brush;
	COLORREF key;
	for (POSITION pos = m_brushObs.GetStartPosition(); pos != NULL; )
	{
		m_brushObs.GetNextAssoc(pos, key, brush);
		brush->DeleteObject();
		delete brush;
	}
	m_brushObs.RemoveAll();
}

void CAxDraw::removeImage()
{
	CaxImage* img;
	CString	key;
	
	for (POSITION pos = m_imgObs.GetStartPosition(); pos != NULL; )
	{
		m_imgObs.GetNextAssoc(pos, key, (void *&)img);
		delete img;
	}
	m_imgObs.RemoveAll();
}

//--------------------------------------------------------------------------------------
//	class CaxImage
//
CaxImage::CaxImage()
{
	m_type    = iNONE;
	m_image   = NULL;
	m_picture = NULL;
}

CaxImage::~CaxImage()
{
	if (m_picture)
	{
		m_picture->Release();
		m_picture = NULL;
	}

	if (m_image)
	{
		if (m_type == iBMP)
		{
			((CBitmap *)m_image)->DeleteObject();
			delete (CBitmap *)m_image;
		}
		else
			::delete (Image *)m_image;
	}
}

bool CaxImage::Load(CString file)
{
	int	idx = file.ReverseFind('.');
	if (idx != -1 && !file.Mid(idx).CompareNoCase("png"))	// .png file
	{
		Image*	img = new Image(file.AllocSysString());
		if (img != NULL)
		{
			m_image = img;
			m_type = iPNG;
			return true;
		}
		return false;
	}

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
	char	twb[1024*8];
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
		m_image = new CBitmap;
		((CBitmap *)m_image)->Attach(hbitmap);
		m_type = iBMP;
		return true;
	}

	if (m_picture != NULL)
	{
		m_picture->Release();
		m_picture = NULL;
	}
	return false;
}

#define HIMETRIC_INCH	2540			// 1inch -> 1/100 mm

bool CaxImage::getHandle(HBITMAP& bitmap)
{
	if (m_picture == NULL)
		return false;

	OLE_XSIZE_HIMETRIC cx;
	OLE_YSIZE_HIMETRIC cy;
							// calulate size from IPicture
	m_picture->get_Width(&cx);
	m_picture->get_Height(&cy);

	HDC memDC = CreateCompatibleDC(NULL);

	int w = MulDiv(cx, GetDeviceCaps(memDC, LOGPIXELSX), HIMETRIC_INCH);
	int h = MulDiv(cy, GetDeviceCaps(memDC, LOGPIXELSY), HIMETRIC_INCH);

	DWORD*	pBitmapBits;
	BITMAPINFO bmi;

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
	m_picture->Render(memDC, 0, 0, w, h, 0, cy - 1, cx, -cy, NULL);

	SelectObject(memDC, o_bitmap);
	DeleteDC(memDC);
	return true;
}
