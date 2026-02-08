#include "stdafx.h"
#include "libcommon.h"

void LIB_MessageBox(char* fmt, ...)
{
	CString	tmpstr;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	AfxMessageBox(tmpstr);
	va_end(valist);
}


CString LIB_GetStrToken(int nIndex, CString data, char* token)
{
	CString retval = _T("");
	char*	dataptr = nullptr;
	char*	value = nullptr;
	const int nValue = 0;
	const int nLen   = data.GetLength() + 1;
	int	ii = 0;

	if (data.IsEmpty())
		return "";

	dataptr = (char*)malloc(nLen);
	memset(dataptr, 0x00, nLen);
	memcpy(dataptr, data, nLen - 1);

	retval = value = strtok(dataptr, token);

	if ( ii == nIndex)
	{
		free(dataptr);
		return retval;
	}

	while(value)
	{
		ii++;
		retval = value = strtok(nullptr, token);
		
		if (ii == nIndex)
		{
			free(dataptr);
			return retval;
		}
	}

	free(dataptr);

	return "";	
}

CRect	LIB_MoveWindow(CWnd* pWnd, CPoint pt)
{
	if (pWnd == nullptr)
		return nullptr;

	CRect	rect;
	CSize	size;	

	pWnd->GetWindowRect(rect);
	size.cx = rect.Width();
	size.cy = rect.Height();

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	pWnd->MoveWindow(rect);
	return rect;
}

CString LIB_MakeComma(CString data)
{

	int	val = 0, ii = 0;
	CString	tmpstr = _T("");

	val = atoi((char*)(LPSTR)(LPCTSTR)data);

	if (val < 1000)
	{
		data.Format("%d", val);
		return data;
	}

	val = data.GetLength() - 1;

	for ( ii = 0 ; ii < data.GetLength() ; ii++ ) 
	{
		tmpstr = data.GetAt(val - ii) + tmpstr;

		if ((ii + 1) % 3 == 0)
			tmpstr = "," + tmpstr;
	}

	if (tmpstr.GetAt(0) == ',')
		tmpstr = tmpstr.Mid(1);
	
	return tmpstr;
}

COLORREF LIB_GetReverseColor(COLORREF color)
{
	int red = GetRValue(color);
	int green = GetGValue(color);
	int  blue = GetBValue(color);

	if ((red == green && green == blue)
		&& (red > 120 && red < 140) 
		&& (green > 120 && green < 140)
		&& (blue > 120 && blue < 140))
	{
		red = red - 20;
		green = green - 20;
		blue = blue - 20;
	}

	return RGB(255-red, 255-green, 255-blue);
}

void LIB_FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
	switch (point)
	{
	case 2:
		dotL = int(dVal / 100.0);
		dotR = int(int(dVal) % 100);
		break;
	default:
		dotL = dVal;
		break;
	}

	dispTxt = _T("");
	edit1.Format("%.f", dotL);
	edit1.MakeReverse();
	for (int ii = 0; ii < edit1.GetLength(); ii++)
	{
		if (ii && !(ii % 3))
			edit2 += ',';
		edit2 += edit1[ii];
	}
	if (bSign) edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");
	switch (point)
	{
	case 2:
		edit1.Format(".%02d", int(dotR+0.5));
		break;
	default:
		break;
	}

	const int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
	if (trimComma > 0)
	{
		int trim = 0;
		for (int ii = 0; ii < edit2.GetLength(); ii++)
		{
			if (edit2[ii] == ',' && trim < trimComma)
			{
				trim++;
				continue;
			}
			dispTxt += edit2[ii];
		}
		dispTxt += edit1;
	}
	else
	{
		dispTxt = edit2 + edit1;
	}
}