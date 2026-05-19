// Draw.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "amlib.h"
#include "../h/axisvar.h"

// CAmDraw

CAmDraw::CAmDraw()
{
}

CAmDraw::~CAmDraw()
{
	RemoveAllFont();
	RemoveAllPen();
	RemoveAllBrush();
	RemoveAllBitmap();
}

CFont* CAmDraw::SetFont(CDC *pDC, char* fontN, int point, bool italic, int bold)
{
	CString	keys;
	CFont*	font;
	LOGFONT	logfont;

	keys.Format("%s:%d:%c:%d", fontN, point, italic ? 'I' : 'i', bold);
	if (!m_pointFontObs.Lookup(keys, (CObject*&)font))
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
		m_pointFontObs.SetAt(keys, font);
	}
	pDC->SelectObject(font);
	return font;
}

void CAmDraw::SetPen(CDC *pDC, COLORREF crRGB, int width, int style)
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
	pDC->SelectObject(pen);
}

CBrush* CAmDraw::SetBrush(CDC *pDC, COLORREF crRGB)
{
	CBrush*	brush;

	if (!m_brushObs.Lookup(crRGB, brush))
	{
		brush = new CBrush();
		brush->CreateSolidBrush(crRGB);
		m_brushObs.SetAt(crRGB, brush);
	}
	pDC->SelectObject(brush);
	return brush;
}

CBitmap* CAmDraw::SetBitmap(char* fileN)
{
	if (fileN == NULL || strlen(fileN) <= 0)
		return NULL;

	time_t	tm = 0;
	CFileStatus	status;
	Cbitmap* bmp  = NULL;
	CString	files = CString(fileN);

	files.MakeUpper();
	if (CFile::GetStatus(files, status))
		tm = status.m_mtime.GetTime();
	if (m_bmpObs.Lookup(files, (void*&)bmp))
	{
		if (bmp->m_mtime == tm)
			return  bmp->m_bitmap;

		bmp->m_bitmap->DeleteObject();
		delete bmp->m_bitmap;
		m_bmpObs.RemoveKey(files);
		delete bmp;
	}

	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, files, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		bmp = new Cbitmap;
		bmp->m_mtime  = tm;
		bmp->m_bitmap = new CBitmap;
		bmp->m_bitmap->Attach(hbitmap);
		m_bmpObs.SetAt(files, bmp);
		return bmp->m_bitmap;
	}

	return NULL;
}

void CAmDraw::RemoveAllFont()
{
	CFont*	font;
	CString	key;

	for (POSITION pos = m_pointFontObs.GetStartPosition(); pos != NULL; )
	{
		m_pointFontObs.GetNextAssoc(pos, key, (CObject *&)font);
		font->DeleteObject();
		delete font;
	}
	m_pointFontObs.RemoveAll();
}

void CAmDraw::RemoveAllPen()
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

void CAmDraw::RemoveAllBrush()
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

void CAmDraw::RemoveAllBitmap()
{
	Cbitmap* bmp;
	CString	key;
	
	for (POSITION pos = m_bmpObs.GetStartPosition(); pos != NULL; )
	{
		m_bmpObs.GetNextAssoc(pos, key, (void *&)bmp);
		bmp->m_bitmap->DeleteObject();
		delete bmp->m_bitmap;
		delete bmp;
	}
	m_bmpObs.RemoveAll();
}
