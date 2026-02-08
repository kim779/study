#include "stdafx.h"
#include "libDefine.h"

CString _GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, nMinLen);
	sRtn.TrimRight();	sRtn.TrimLeft();
	return sRtn;
}

CString _GetFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

CString	_Parser(CString &srcstr, CString substr)
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