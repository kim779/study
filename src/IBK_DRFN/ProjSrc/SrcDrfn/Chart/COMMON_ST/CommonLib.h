#include "stdafx.h"

CString Dtos(double dValue)
{
	CString strResult;
	strResult.Format("%f",dValue);
	if(strResult.Find('.')!=-1)
	{
		strResult.TrimRight('0');
		if(strResult[strResult.GetLength()-1]=='.') strResult.TrimRight('.');
	}
	return strResult;
}

CString Parser(LPCTSTR lpInputData, int *pNShifter ) 
{
	if(!pNShifter) return "";
	CString strInputData;
	int nPosStart = *pNShifter;
	int nPosEnd = 0;
	strInputData = lpInputData;
	nPosEnd = strInputData.Find(',',nPosStart);
	if(nPosEnd==-1)
	{
		nPosEnd = strInputData.GetLength();
		if(nPosStart>=nPosEnd)
		{
			return "";
		}
	}
	*pNShifter = nPosEnd + 1;
	return strInputData.Mid(nPosStart,nPosEnd-nPosStart);
}
