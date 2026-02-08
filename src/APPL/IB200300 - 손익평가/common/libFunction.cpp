#include "stdafx.h"
#include "libFunction.h"
#include "../../../h/axisfire.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void _SetImgBtn(CWnd *pParent, CString imgDir, CImgBtn *pButton, int nLength, BOOL bImageFit)
{
	CString sImgDir = "";
	sImgDir.Format("%s%dbtn", imgDir, nLength);

	pButton->m_hBitmap    = _GetAxBitmap(pParent, sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = _GetAxBitmap(pParent, sImgDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = _GetAxBitmap(pParent, sImgDir + "_en.bmp")->operator HBITMAP();
	if (bImageFit)	pButton->FitImageSize();
}

void _SetImgBtnX(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit)
{
	CString sImgDir = "";
	sImgDir.Format("%s%s", imgDir, sImgName);

	pButton->m_hBitmap    = _GetAxBitmap(pParent, sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = _GetAxBitmap(pParent, sImgDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = _GetAxBitmap(pParent, sImgDir + "_en.bmp")->operator HBITMAP();
	if (bImageFit)
		pButton->FitImageSize();
}

void _SetImgBtnT(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit)
{
	CString sImgDir = "";
	sImgDir.Format("%s%s", imgDir, sImgName);

	pButton->m_hBitmap    = _GetAxBitmap(pParent, sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = _GetAxBitmap(pParent, sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = _GetAxBitmap(pParent, sImgDir + ".bmp")->operator HBITMAP();
	if (bImageFit)
		pButton->FitImageSize();
}

CPen* _GetAxPen(CWnd *pParent, COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

COLORREF _GetIndexColor(CWnd *pParent, int index)
{
	if (index & 0x02000000)
		return index;
	return pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CFont* _GetAxFont(CWnd *pParent, CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* _GetAxBitmap(CWnd *pParent, CString path)
{
	return (CBitmap*)pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CString _Parser(CString &srcstr, CString substr)
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
		temp.TrimLeft();	temp.TrimRight();
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString _GetAccEdit(CString accn)
{
	CString tmpS = "";
	if (accn.GetLength() == 10)
		tmpS.Format("%.8s-%.2s", accn.Left(8), accn.Mid(8));

	return tmpS;
}


CString _GetRemoveString(CString str)
{
	str.Remove('-');
	str.Remove('+');
	return str;
}


CString _GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}

CString _GetStringX(char *pChar, int nMinLen, int digit, bool bColor)
{
	bool bPlus = false;
	double val = 0.0;
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	if (bColor)
	{
		if (sRtn.Find('+') >= 0)
			bPlus = true;
	}
	val = atof(sRtn);
	sRtn.Format("%.*f", digit, val);
	if (bPlus)
		sRtn = "+" + sRtn;
	return sRtn;
}

double _Str2Double(CString string, bool bABS /*=true*/)
{
	if (string.IsEmpty())
		return 0.0;

	string.Remove(',');
	
	if (bABS)
		return fabs(atof(string));
	else
		return atof(string);
}

int _Str2Int(CString string, bool bABS /*=true*/)
{
	if (string.IsEmpty())
		return 0;

	string.Remove(',');
	
	if (bABS)
		return abs(atoi(string));
	else
		return atoi(string);
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
	case FK_FUTURE:
		if (atof(str) == 0.0)
			tmpS = "0.0";
		else
			tmpS.Format("%.2f", atof(str) / 100.0);
		break;
	case FK_ACCNx:
		if (str.GetLength() == 10)
			tmpS.Format("%s", str.Left(8));
		else
			tmpS = str;
		break;
	case FK_ACCN:
		if (str.GetLength() == 10)
			tmpS.Format("%s-%s", str.Left(8), str.Mid(8, 2));
		else
			tmpS = str;
		break;
	case FK_TIME:
		if (str.GetLength() == 6)
			tmpS.Format("%s:%s:%s", str.Left(2), str.Mid(2, 2), str.Mid(4, 2));
		else
			tmpS = str;
		break;
	case FK_TRIMLEFT:
		str.TrimLeft();
		str.TrimLeft('0');
		tmpS = str;
		break;
	case FK_RMVACCN:
		str.Remove('-');
		tmpS = str;
		break;
	case FK_RMVCOMMA:
		str.Remove(',');
		tmpS = str;
		break;
	default:
		tmpS = str;
	}
	return tmpS;
}
