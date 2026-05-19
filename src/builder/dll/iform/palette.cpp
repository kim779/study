#include "stdafx.h"
#include "palette.h"
#include "palette.inc"


#define	nSYSCOLOR	64
#define	nRGB		3

CAxPalette::CAxPalette(void)
{
	m_rgbs.RemoveAll();
}

CAxPalette::CAxPalette(CString fileN, CString theme)
{
	char	rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	m_rgbs.RemoveAll();
	m_paletteini  = CString(fileN);
	for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
	{
		tmps.Format(_T("RGB%03d"), ii);
		GetPrivateProfileString(_T("Palette"), tmps, "", rgbV, sizeof(rgbV), m_paletteini);

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
		axisRGB[ii] = PALETTERGB(val[0], val[1], val[2]);
	}

	SetPalette(theme);
}


CAxPalette::~CAxPalette(void)
{
}

COLORREF CAxPalette::GetPaletteRGB(int index)
{
	return axisRGB[min(max(0, index), MAX_axisPALETTE-1)];
}

int CAxPalette::GetIndexRGB(COLORREF rgb)
{
	for (int ii = 0; ii < MAX_axisPALETTE; ii++)
	{
		if (axisRGB[ii] == rgb)
			return ii;
	}
	return -1;
}

void CAxPalette::SetPalette(CString theme)
{
	char	rgbV[128];
	CString	rgb, tmps;
	int	idx, val[nRGB];

	m_rgbs.RemoveAll();
	if (!theme.IsEmpty())
	{
		for (int ii = 0; ii < MAX_axisPALETTE-nSYSCOLOR; ii++)
		{
			tmps.Format(_T("RGB%03d"), ii);
			GetPrivateProfileString(theme, tmps, "", rgbV, sizeof(rgbV), m_paletteini);

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
			axisRGB[ii] = PALETTERGB(val[0], val[1], val[2]);
			m_rgbs.SetAt(ii, axisRGB[ii]);
		}
	}
}

BOOL CAxPalette::Lookup(int index, COLORREF& rgb)
{
	return m_rgbs.Lookup(index, rgb);
}