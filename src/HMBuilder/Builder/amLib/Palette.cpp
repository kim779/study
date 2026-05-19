// Palette.cpp: implementation of the CAmPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "amlib.h"
#include "Palette.inc"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	nSYSCOLOR	64
#define	nRGB		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmPalette::CAmPalette()
{
	m_rgbs.RemoveAll();
}

CAmPalette::CAmPalette(char *rgbN)
{
	char	rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	m_rgbs.RemoveAll();
	for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
	{
		tmps.Format(_T("RGB%03d"), ii);
		GetPrivateProfileString(_T("Palette"), tmps, _T(""), rgbV, sizeof(rgbV), rgbN);

		rgb = rgbV;
		for (int kk = 0; kk < nRGB; kk++)
		{
			idx = rgb.Find(_T(','));
			if (idx != -1)
			{
				tmps = rgb.Left(idx++);
				rgb  = rgb.Mid(idx);
			}
			else
			{
				tmps = rgb;
				rgb.Empty();
			}

			val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE-1);
			if (rgb.IsEmpty())
				break;
		}
		axisRGB[ii] = PALETTERGB(val[1], val[2], val[3]);
	}

	SetPalette(rgbN);
}

CAmPalette::~CAmPalette()
{
}

COLORREF CAmPalette::GetPaletteRGB(int index)
{
	return axisRGB[min(max(0, index), MAX_axisPALETTE-1)];
}

int CAmPalette::GetIndexRGB(COLORREF rgb)
{
	for (int ii = 0; ii < MAX_axisPALETTE; ii++)
	{
		if (axisRGB[ii] == rgb)
			return ii;
	}
	return -1;
}

void CAmPalette::SetPalette(char *rgbN, bool force)
{
	char	section[80], rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	if (force)
	{
		m_rgbs.RemoveAll();
		for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
		{
			tmps.Format(_T("RGB%03d"), ii);
			GetPrivateProfileString(_T("Palette"), tmps, _T(""), rgbV, sizeof(rgbV), rgbN);

			rgb = rgbV;
			for (int kk = 0; kk < nRGB; kk++)
			{
				idx = rgb.Find(_T(','));
				if (idx != -1)
				{
					tmps = rgb.Left(idx++);
					rgb  = rgb.Mid(idx);
				}
				else
				{
					tmps = rgb;
					rgb.Empty();
				}

				val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE-1);
				if (rgb.IsEmpty())
					break;
			}
			axisRGB[ii] = PALETTERGB(val[1], val[2], val[3]);
		}
	}

	GetPrivateProfileString(_T("General"), _T("Palette"), _T(""), section, sizeof(section), rgbN);
	if (strlen(section))
	{
		for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
		{
			tmps.Format(_T("RGB%03d"), ii);
			GetPrivateProfileString(section, tmps, _T(""), rgbV, sizeof(rgbV), rgbN);

			rgb = rgbV;
			if (rgb.IsEmpty())
				continue;
			for (int kk = 0; kk < nRGB; kk++)
			{
				idx = rgb.Find(_T(','));
				if (idx != -1)
				{
					tmps = rgb.Left(idx++);
					rgb  = rgb.Mid(idx);
				}
				else
				{
					tmps = rgb;
					rgb.Empty();
				}

				val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE-1);
				if (rgb.IsEmpty())
					break;
			}
			axisRGB[ii] = PALETTERGB(val[1], val[2], val[3]);
			m_rgbs.SetAt(ii, axisRGB[ii]);
		}
	}
}

BOOL CAmPalette::Lookup(int index, COLORREF& rgb)
{
	return m_rgbs.Lookup(index, rgb);
}
