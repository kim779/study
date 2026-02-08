	#include "stdafx.h"
#include "libFunction.h"
#include "math.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBrush* _GetAxBrush(CWnd* m_pView, COLORREF clr)
{
	return (CBrush*)m_pView->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CBitmap* _GetAxBitmap(CWnd* m_pView, CString path)
{
	return (CBitmap*)m_pView->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}


COLORREF _GetIndexColor(CWnd* m_pView, int index)
{
	if (index & 0x02000000)
		return index;
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CString	_GetString(char* datB, int datL)
{
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}

CString _GetFormatData(int fkind, CString str)
{
	CString tmpS = "";
	switch (fkind)
	{
	case FK_COMMA:
		{
			bool bPlus = false;
			bool bMinus = false;
			if (atof(str) < 0 && !str.IsEmpty())
			{
				bMinus = true;
				str = str.Mid(1);
			}
			if (!str.IsEmpty() && str.GetAt(0) == '+')
			{
				bPlus = true;
				str = str.Mid(1);
			}
			str.Remove(',');
			str.MakeReverse();
			for (int ii = 0; ii < str.GetLength(); ii++)
			{
				if (ii && !(ii % 3))
					tmpS += ',';
				tmpS += str[ii];
			}

			tmpS.MakeReverse();
			if (bMinus)	tmpS = "-" + tmpS;
			if (bPlus)	tmpS = "+" + tmpS;
		}
		break;
	case FK_FLOAT:
		tmpS.Format("%.2f", atof(str));
		break;
	case FK_FLOATx:
		tmpS.Format("%.2f", atof(str));
		if (atof(tmpS) > 0)
			tmpS = "+" + tmpS;
		break;
	case FK_REMOVE:
		str.Remove(',');
		tmpS = str;
		break;
	default:
		tmpS = str;
	}
	return tmpS;
}

