// picture.cpp: implementation of the Cpicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "picture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cpicture::Cpicture(CString fileN)
{
	m_ext   = FX_NONE;
	m_valid = false;
	m_picImpl = (CpictureImpl *) 0;

	GetFileType(fileN);
	if (m_ext == FX_NONE)
		return;

	DWORD	dwRc = ::GetFileAttributes(fileN);
	if (dwRc != 0xFFFFFFFF || !(dwRc & FILE_ATTRIBUTE_DIRECTORY))
	{
		m_valid = true;
		switch (m_ext)
		{
		case FX_BMP:
			m_picImpl = new CpictureBMP(fileN);	break;
		case FX_GIF:
			m_picImpl = new CpictureGIF(fileN);	break;
		}
	}
}

Cpicture::~Cpicture()
{
	if (m_picImpl)
		delete m_picImpl;
}

void Cpicture::DrawPicture(CDC* dc, CRect rect, int style)
{
	if (m_picImpl && m_valid)
		m_picImpl->Draw(dc, rect, style);
}

void Cpicture::GetFileType(CString fileN)
{
	CString	tmps;
	int	dot = fileN.ReverseFind('.');

	tmps = fileN.Mid(++dot);
	if (!tmps.CompareNoCase("BMP"))
		m_ext = FX_BMP;
	else if (!tmps.CompareNoCase("GIF"))
		m_ext = FX_GIF;
	else
		m_ext = FX_NONE;
}

CSize Cpicture::getImageSize()
{
	if (m_picImpl)
		return m_picImpl->getImageSize();
	else
		return CSize(0, 0);
}
