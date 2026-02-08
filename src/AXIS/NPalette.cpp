// NPalette.cpp: implementation of the CNPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "NPalette.h"


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

COLORREF CNPalette::GetColor(int index)
{
	CString		data, skinName, SNKey;

	if (Axis::home.IsEmpty())	return RGB(0, 0, 0);

	CProfile profile(GetProfileFileName(pkPalette));

	skinName = profile.GetString(GENERALSN, "Palette");

	if (!skinName.CompareNoCase("Green") || !skinName.CompareNoCase("Violet"))
	{
		skinName = "Blue";
	}

	if (index == -1)
		SNKey = _T("RGB");
	else	
		SNKey.Format("RGB%03d", index);

	data = profile.GetString(skinName, SNKey);
	if (data.IsEmpty())
	{
		if (!skinName.Compare("Palette"))
			return RGB(0, 0, 0);
		skinName = _T("Palette");
		data = profile.GetString(skinName, SNKey);
		if (data.IsEmpty())
			return RGB(0, 0, 0);
	}

	BYTE rgb[3]{};

	int find = data.Find(',');
	if (find < 0) return 0;

	rgb[0] = atoi(data.Left(find));
	data = data.Mid(find + 1);

	find = data.Find(',');
	if (find < 0) return 0;

	rgb[1] = atoi(data.Left(find));
	rgb[2] = atoi(data.Mid(find + 1));

	return RGB(rgb[0], rgb[1], rgb[2]);
}
