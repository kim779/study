// AmDraw.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AmDraw.h"

#define _ARGB(a,r,g,b)        ((ARGB)(((BYTE)(b) | ((WORD)((BYTE)(g))<<8)) | (((DWORD)(BYTE)(r))<<16) | (((DWORD)(BYTE)(a))<<24)))

// CAmimage
CAmImage::CAmImage()
{
	m_mtime = 0;
	m_image = NULL;
}

CAmImage::~CAmImage()
{
}

// CAmDraw
CAmDraw::CAmDraw()
{
}

CAmDraw::~CAmDraw()
{
	RemoveAllFont();
	RemoveAllPen();
	RemoveAllBrush();
	RemoveAllImage();
}

CFont* CAmDraw::SetDcFont(struct _fontR* fontR)
{
	CString	sKeys;
	CFont*	pFont;
	LOGFONT	logfont;

	sKeys.Format(_T("%s:%d:%c:%d"), fontR->name, fontR->point, fontR->italic ? _T('I') : _T('i'), fontR->bold);

	if (!m_pointFontObs.Lookup(sKeys, (CObject*&)pFont))
	{
		pFont = new CFont();

		logfont.lfHeight	= fontR->point * 10 + (fontR->point < 9 ? 5 : 0);
		logfont.lfWeight	= 0;
		logfont.lfEscapement	= 0;
		logfont.lfOrientation	= 0;
		logfont.lfWeight	= fontR->bold;
		logfont.lfItalic	= fontR->italic;
		logfont.lfUnderline	= false;
		logfont.lfStrikeOut	= false;
		logfont.lfCharSet	= DEFAULT_CHARSET;
		logfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
		logfont.lfQuality	= DEFAULT_QUALITY;
		logfont.lfPitchAndFamily= DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, fontR->name);
		
		pFont->CreatePointFontIndirect(&logfont);
		m_pointFontObs.SetAt(sKeys, pFont);
	}

	return pFont;
}

Font* CAmDraw::SetFont(struct _fontR* fontR)
{
	CString		sKeys;
	Font*		font = NULL;
	int		style = FontStyleRegular;
	CString		fontname = fontR->name;

	if (fontname.IsEmpty() && fontR->point <= 0)
		return font;

	sKeys.Format(_T("%s:%d:%c:%d"), fontR->name, fontR->point, fontR->italic ? _T('I') : _T('i'), fontR->bold);

	if (!m_pointFontPtr.Lookup(sKeys, (void*&)font))
	{
		if (fontR->bold)
			style |= FontStyleBold;
		if (fontR->italic)
			style |= FontStyleItalic;

		font = ::new Font(fontname.AllocSysString(), (Gdiplus::REAL)fontR->point, style, UnitPixel);

		m_pointFontPtr.SetAt(sKeys, font);
	}

	return font;
}
Pen* CAmDraw::SetPen(struct _penR* penR)
{
	CString	sKeys;
	Pen*	pen;

	sKeys.Format(_T("%d:%d"), penR->clr, penR->width);

	if (!m_penPtr.Lookup(sKeys, (void*&)pen))
	{
		pen = ::new Pen(penR->clr, (Gdiplus::REAL)penR->width);
		m_penPtr.SetAt(sKeys, pen);
	}

	return pen;
}

SolidBrush* CAmDraw::SetBrush(ARGB crARGB)
{
	SolidBrush*	brush;

	if (!m_brushPtr.Lookup(crARGB, brush))
	{
		brush = ::new SolidBrush(crARGB);
		m_brushPtr.SetAt(crARGB, brush);
	}

	return brush;
}

Image* CAmDraw::SetImage(char* cFileN)
{
	if (cFileN == NULL || strlen(cFileN) <= 0)
		return NULL;

	time_t		tm = 0;
	CFileStatus	status;
	CAmImage*	amimage = NULL;
	CString		files = CString(cFileN);
	CFileFind	file;

	files.MakeUpper();
	if (CFile::GetStatus(files, status))
		tm = status.m_mtime.GetTime();

	if (m_imagePtr.Lookup(files, (void*&)amimage))
	{
		if (amimage->m_mtime == tm)
			return amimage->m_image;

		m_imagePtr.RemoveKey(files);
		delete amimage;
	}
	
	amimage = ::new CAmImage();
	if (file.FindFile(files))
	{
		amimage->m_mtime = tm;
		m_imagePtr.SetAt(files, amimage);
		amimage->m_image = ::new Image(files.AllocSysString());
		return amimage->m_image;
	}

	::delete amimage;
	return NULL;
}

void CAmDraw::RemoveAllFont()
{
	POSITION	pos;
	CFont*		Dcfont;
	Font*		font = NULL;
	CString		sKey;

	for (pos = m_pointFontObs.GetStartPosition(); pos != NULL;)
	{
		m_pointFontObs.GetNextAssoc(pos, sKey, (CObject*&)Dcfont);
		Dcfont->DeleteObject();

		if (Dcfont)
			delete Dcfont;
	}
	
	for(pos = m_pointFontPtr.GetStartPosition(); pos != NULL;)   
	{
		m_pointFontPtr.GetNextAssoc(pos, sKey, (void*&)font);

		if (font)
			::delete font;
	}

	m_pointFontObs.RemoveAll();
	m_pointFontPtr.RemoveAll();
}

void CAmDraw::RemoveAllPen()
{
	POSITION	pos;
	Pen*		pen;
	CString		sKey;

	for(pos = m_penPtr.GetStartPosition(); pos != NULL;)   
	{
		m_penPtr.GetNextAssoc(pos, sKey, (void*&)pen);

		if (pen)
			::delete pen;
	}

	m_penPtr.RemoveAll();
}

void CAmDraw::RemoveAllBrush()
{
	POSITION	pos;
	SolidBrush*	brush;
	ARGB		sKey;

	for(pos = m_brushPtr.GetStartPosition(); pos != NULL;)   
	{
		m_brushPtr.GetNextAssoc(pos, sKey, brush);

		if (brush)
			::delete brush;
	}

	m_brushPtr.RemoveAll();
}

void CAmDraw::RemoveAllImage()
{
	POSITION	pos;
	CAmImage*	amimage;
	CString		sKey;

	for(pos = m_imagePtr.GetStartPosition(); pos != NULL;)   
	{
		m_imagePtr.GetNextAssoc(pos, sKey, (void*&)amimage);

		if (amimage)
			::delete amimage;
	}

	m_imagePtr.RemoveAll();
}