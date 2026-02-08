#pragma once

#include "axisfire.h"

CString parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

int _httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	std::unique_ptr < TCHAR[] > mstr = std::make_unique<TCHAR[]>(strlen(value) * 2 + 1);
	memcpy(mstr.get(), _tcsupr(_tcsdup(value)), strlen(value));

	TCHAR *ss = mstr.get();
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}

	return result;
}

double GetPriceUnit(double price, CString sJCode, bool bPlus)
{
	if (sJCode.IsEmpty())
		return 0;

	
	if (sJCode[0] == '1')
		return 0.05;
	else
	{
		CString sPrice;
		sPrice.Format("%6.2f", price);
		price = atof(sPrice);

		if (price < 3 || (!bPlus && price == 3))
			return 0.01;
		else
			return 0.05;
	}
}

CString SetComma(CString src)
{
	CString temp, floatStr;
	int	ii=0, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

CRect adjustRect(CRect screenRect)
{
	int nDx = 0, nDy = 0;
	if (screenRect.right > GetSystemMetrics(SM_CXFULLSCREEN))
		nDx = GetSystemMetrics(SM_CXFULLSCREEN) - screenRect.right;
	if (screenRect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		nDy = GetSystemMetrics(SM_CYFULLSCREEN) - screenRect.bottom;
	screenRect.OffsetRect(nDx, nDy);
	return screenRect;
}

int openViewA(CWnd *pParent, int type, CString data)
{
	return pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

BOOL SendTRA(CWnd *pParent, CString name, BYTE type, CString data, int key, CString sCtrlName = "")
{
	CString trData = sCtrlName;
	
	struct _userTH udat;
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	if (type & US_KEY)
	{
		trData = ((char)LOWORD(key));
		key    = HIWORD(key);
	}

	if (!trData.IsEmpty())	// is control
		trData += '\t';

	trData += CString((char *)&udat, L_userTH);
	trData += data;

	return pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)trData.operator LPCTSTR());
}


CFont* getAxFontA(CWnd *pParent, CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* getAxBitmapA(CWnd *pParent, CString path)
{
	return (CBitmap*)pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CPen* getAxPenA(CWnd *pParent, COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* getAxBrushA(CWnd *pParent, COLORREF clr)
{
	return (CBrush*)pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

COLORREF GetColorA(CWnd *pParent, int color)
{
	if (color & 0x02000000)
		return color;

	return pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CString VariantA(CWnd *pParent, int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}


CWnd* CreateMapObjectA(CWnd *pParent, CWnd *pMapWnd, CString sMapName, int &nObjKey, CPoint point)
{
	ASSERT(::IsWindow(pParent->m_hWnd));
	ASSERT(::IsWindow(pMapWnd->m_hWnd));

	std::unique_ptr<CWnd> pObjWnd = nullptr;
	

	if (!sMapName.IsEmpty())
	{
		CRect   rc;
		CString sStr;
		long    nSzMap = 0, nType=0;

		pMapWnd->GetClientRect(&rc);

		sStr = CString(sMapName, 8);
		nSzMap = pParent->SendMessage(WM_USER, sizeMAP, (long)sStr.operator LPCTSTR());

		nType = pParent->SendMessage(WM_USER, typeDLL, (long)sStr.operator LPCTSTR());

		if (nType != vtypeERR)
		{
			pObjWnd = std::make_unique<CWnd>();
			pObjWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, LOWORD(nSzMap), HIWORD(nSzMap)), pMapWnd, 0);
			nObjKey = pParent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, nType), (long)pObjWnd.get());
			sStr = sMapName;
			pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, nObjKey), (long)sStr.operator LPCTSTR());
			pObjWnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
		}
	}
	return pObjWnd.release();
}

void DeleteMapObjectA(CWnd *pParent, CWnd *pObjWnd, int nObjKey)
{
	if (!pObjWnd || !::IsWindow(pObjWnd->m_hWnd))
		return;
	pParent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, nObjKey));
	pObjWnd->DestroyWindow();
	delete pObjWnd;
	pObjWnd = NULL;
}

