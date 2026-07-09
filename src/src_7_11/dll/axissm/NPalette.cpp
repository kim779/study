// NPalette.cpp: implementation of the CNPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPalette.h"
#include "../../h/axisvar.h"

#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// class NPalette Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPalette::CNPalette()
{
}

CNPalette::~CNPalette()
{

}

const char* szPalette;
COLORREF CNPalette::GetColor(int index)
{
	COLORREF	color;
	CString		data, skinName, SNKey;

	CProfile profile(5);  //pkPalette
	skinName = profile.GetString(GENERALSN, szPalette);
	if (index == 1)
		SNKey = "RGB";
	else
		SNKey.Format("RGB%03d", index);

	data = profile.GetString(skinName, SNKey);
	if (data.IsEmpty())
	{
		if (!skinName.Compare(szPalette))
			return 0;
		skinName = szPalette;
		data = profile.GetString(skinName, SNKey);
		if (data.IsEmpty())
			return 0;
	}

	CArray	< int, int >	arRGB;
	while (!data.IsEmpty())
	{
		int pos = data.Find(',');
		if (pos == -1)
		{
			arRGB.Add(atoi(data));
			break;
		}
		SNKey = data.Left(pos++);
		data = data.Mid(pos);
		arRGB.Add(atoi(SNKey));
	}

	color = RGB(arRGB.GetAt(0), arRGB.GetAt(1), arRGB.GetAt(2));
	arRGB.RemoveAll();
	return color;
}

COLORREF CNPalette::GetIndexColor(int index)
{
	CProfile profile(pkPalette);
	CString skinName(profile.GetString(GENERALSN, szPalette, szPalette));
	CString SNKey;

	SNKey.Format("RGB%03d", index);
	CString data(profile.GetString(skinName, SNKey));

	if (data.IsEmpty())
	{
		if (!skinName.Compare(szPalette))
			return 0;
		skinName = szPalette;
		data = profile.GetString(skinName, SNKey);
		if (data.IsEmpty())
			return 0;
	}

	CArray	< int, int >	arRGB;
	while (!data.IsEmpty())
	{
		int pos = data.Find(',');
		if (pos == -1)
		{
			arRGB.Add(atoi(data));
			break;
		}
		SNKey = data.Left(pos++);
		data = data.Mid(pos);
		arRGB.Add(atoi(SNKey));
	}

	COLORREF color = RGB(arRGB.GetAt(0), arRGB.GetAt(1), arRGB.GetAt(2));
	arRGB.RemoveAll();
	return color;
}

CString CNPalette::GetTheme()
{
	CProfile profile(pkPalette);
	return profile.GetString(GENERALSN, szPalette, szPalette);
}
