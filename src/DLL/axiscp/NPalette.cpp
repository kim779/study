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

CNPalette::CNPalette(CString home)
{
	m_home = home;
}

CNPalette::~CNPalette()
{

}

COLORREF CNPalette::GetColor(int index)
{
	char		buffer[128];
	COLORREF	color;
	CString		file, data, skinName, SNKey;

	if (m_home.IsEmpty())	return RGB(0, 0, 0);

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);

	skinName = buffer;
	if (index == -1)
		SNKey = _T("RGB");
	else	SNKey.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!skinName.Compare("Palette"))
			return RGB(0, 0, 0);
		skinName = _T("Palette");
		dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(0, 0, 0);
	}
	/*if (dw <= 0)	skinName = _T("Palette");
	else		skinName = buffer;

	SNKey.Format("RGB%03d", index);
	dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return RGB(255, 255, 255);*/

	data = buffer;
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
	char		buffer[128];
	COLORREF	color;
	CString		file, data, skinName, SNKey, home;
	
	home = AfxGetApp()->GetProfileString(ENVIRONMENT, ROOTDIR);

	file.Format("%s\\%s\\%s", home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);
	
	skinName = buffer;
	SNKey.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!skinName.Compare("Palette"))
			return RGB(0, 0, 0);
		skinName = _T("Palette");
		dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(0, 0, 0);
	}

	data = buffer;
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

CString CNPalette::GetTheme()
{
	char		buffer[128];
	CString		file, data, skinName, SNKey, home;
	
	home = AfxGetApp()->GetProfileString(ENVIRONMENT, ROOTDIR);

	file.Format("%s\\%s\\%s", home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);
	
	skinName = buffer;

	return skinName;
}