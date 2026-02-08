#include "stdafx.h"
#include "common_ml.h"
#include "math.h"

#define	FORMAT_TEXT		0
#define	FORMAT_NUMCOMMA_SIGN	1
#define	FORMAT_NUMCOMMA		2
#define	FORMAT_ACCOUNT		3
#define	FORMAT_YYMMDD		4
#define	FORMAT_MMDD		5
#define	FORMAT_HHMMSS		6
#define	FORMAT_MMSS		7
#define	FORMAT_ONLYINT		8

#define	COLOR_WHITE		RGB(255,255,255)
#define	COLOR_BLACK		RGB(0,0,0)
#define	NCOLORSHADES		128		

///////////////////////////////////////////////////////////////////////////////////////////////////

bool isNum(char c)
{
	const	char*	pnum = "0123456789.";
	
	for (int ii = 0; ii < int(strlen(pnum)) ; ii++)
	{
		if (c == pnum[ii])
			return true;
	}

	return false;
}

BOOL isNumber(CString sData)
{
	if (sData.FindOneOf("+-") == 0)
		sData = sData.Mid(1);

	if (sData.Trim().IsEmpty())
		return FALSE;
	
	for (int ii = 0; ii < sData.GetLength(); ii++)
	{
		if (!isNum(sData.GetAt(ii)))
			return FALSE;
	}

	return TRUE;
}

int CInt(CString sData)
{
	if (sData == "")
		return 0;
	return atoi(sData);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//String Parsing
/////////////////////////////////////////////////////////////////////////////////////////////////////

CString SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;

	if (strToken.IsEmpty())
		return sResult;

	const	int	iFind = strData.Find(strToken); 
	if (iFind > -1) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength());  
	}

	return sResult;
}

int SplitDivString(CString strData, CString *sList, int idivcnt)
{
	if (sList == nullptr)
		return 0;

	int	index = 0;
	while (true)
	{
		if (strData.GetLength() < idivcnt)
			break;

		sList[index++] = strData.Left(idivcnt);
		strData = strData.Mid(idivcnt);  
	}

	return index;
}

int SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt)
{
	if (strToken.IsEmpty())
		return 0;

	int	iFind = 0;

	astrResult.RemoveAll();
	while (true)
	{
		iFind = strData.Find(strToken); 
		if (iFind < 0) 
		{
			if (!strData.IsEmpty())
				astrResult.Add(strData);
			break;
		}
	
		astrResult.Add(strData.Left(iFind)); 
		strData = strData.Mid(iFind + strToken.GetLength());  
		if (strData.IsEmpty())
			break;
		if (imaxcnt <= astrResult.GetSize())
			break;
	}

	return astrResult.GetSize(); 
}

CString GetFindOptionString(CString sData, CString sKey, CString sEndChar)
{
	CString sResult;

	int	iFind = sData.Find(sKey);
	if (iFind < 0)
		return _T("");

	int	iendFind = sData.Find(sEndChar, iFind);
	if (iendFind < 0) 
	{
		iendFind = sData.GetLength(); 
//		return _T("");
	}

	iFind = iFind + sKey.GetLength(); 
	sResult = sData;
	sResult = sResult.Mid(iFind, iendFind - iFind); 
	return sResult;
}

CString SetComma(CString sData, int ifloatlen)
{
	if (!isNumber(sData))
		return sData;

	CString sign = "";
	if (sData.FindOneOf("+-") == 0)
	{
		sign = sData.Left(1);
		sData = sData.Right(sData.GetLength() -1);  
	}
	sData.Trim();

	CString strIntVal = sData;
	CString	strFloatVal, strResult;

	int	iFind = 0;
	if ((iFind = sData.Find(".")) != -1)
	{
		strIntVal   = sData.Left(iFind);
		strFloatVal = sData.Mid(iFind);
	}

	//선물시세 이상 --0+0.05? 
	const	int	lval = atoi(strIntVal);

	strIntVal.Format("%d",  lval);
	for (int ii = 0; ii < strIntVal.GetLength(); ii++)
	{
		strResult += strIntVal.GetAt(ii); 
		if ((strIntVal.GetLength()-ii-1)%3 == 0 && ii!= strIntVal.GetLength()-1) // 
			strResult += ",";
	}

	if (strFloatVal.GetLength() > ifloatlen+1)	// +1 : '.'
		strFloatVal = strFloatVal.Left(ifloatlen+1); 
	strResult += strFloatVal;

	return sign+strResult;
}