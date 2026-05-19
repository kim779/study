// AmPalette.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AmPalette.h"
#include "Palette.inc"

// CAmPalette
#define	nSYSCOLOR	64
#define	nARGB		4

CAmPalette::CAmPalette()
{
	m_argbs.RemoveAll();
	m_palette = _T("");
}

CAmPalette::CAmPalette(char* path, char* theme)
{
	char	argbV[128];
	CString	argb, tmps;
	int	idx, val[nARGB];
	CFileFind	file;

	if (!file.FindFile(path))
		return;

	m_argbs.RemoveAll();
	m_palette = CString(path);

	for (int ii = 0; ii < MAX_axisPALETTE - nSYSCOLOR; ii++)
	{
		tmps.Format(_T("RGB%03d"), ii);
		GetPrivateProfileString(_T("Palette"), tmps, _T(""), argbV, sizeof(argbV), m_palette);

		argb = argbV;
		for (int kk = 0; kk < nARGB; kk++)
		{
			idx = argb.Find(_T(','));
			if (idx != -1)
			{
				tmps = argb.Left(idx++);
				argb = argb.Mid(idx);
			}
			else
			{
				tmps = argb;
				argb.Empty();
			}

			val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE - 1);
			if (argb.IsEmpty())
				break;
		}
		axisARGB[ii] = _ARGB(val[0], val[1], val[2], val[3]);
	}

	if (theme != nullptr)
		Setpalette(theme);
}

CAmPalette::~CAmPalette()
{
	m_argbs.RemoveAll();
}

void CAmPalette::Setpalette(char* theme)
{
	char	argbV[128];
	CString	argb, tmps;
	int	idx, val[nARGB];

	m_argbs.RemoveAll();
	if (strlen(theme))
	{
		for (int ii = 0; ii < MAX_axisPALETTE - nSYSCOLOR; ii++)
		{
			tmps.Format(_T("RGB%03d"), ii);
			GetPrivateProfileString(_T("Palette"), tmps, _T(""), argbV, sizeof(argbV), m_palette);

			argb = argbV;
			
			if (argb.IsEmpty())
				continue;

			for (int kk = 0; kk < nARGB; kk++)
			{
				idx = argb.Find(_T(','));
				if (idx != -1)
				{
					tmps = argb.Left(idx++);
					argb  = argb.Mid(idx);
				}
				else
				{
					tmps = argb;
					argb.Empty();
				}

				val[kk] = min(max(0, atoi(tmps)), MAX_axisPALETTE - 1);
				if (argb.IsEmpty())
					break;
			}

			axisARGB[ii] = _ARGB(val[0], val[1], val[2], val[3]);
			m_argbs.SetAt(ii, axisARGB[ii]);
		}
	}
}

ARGB CAmPalette::GetPaletteARGB(int index)
{
	return axisARGB[min(max(0, index), MAX_axisPALETTE - 1)];
}

BOOL CAmPalette::Lookup(int index, ARGB& argb)
{
	return m_argbs.Lookup(index, argb);
}

int CAmPalette::GetIndexARGB(ARGB argb)
{
	for (int ii = 0; ii < MAX_axisPALETTE; ii++)
	{
		if (axisARGB[ii] == argb)
			return ii;
	}
	return -1;
}