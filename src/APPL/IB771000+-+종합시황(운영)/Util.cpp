
#include "StdAfx.h"
#include "Util.h"

//extern CMapDlg* g_MapDlgPtr;

/*
CMapDlg* GetMapDlg(){
	return g_MapDlgPtr;
}
*/
CString Parser(CString &srcstr, CString substr)
{
	CString temp;
	
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	
	return "";
}

void AddComma(char* pcData)
{
	CString strBuffer = CString(pcData);
	strBuffer.TrimLeft();
	strBuffer.TrimRight();
	strBuffer.Remove(0);
	const int iLength = strBuffer.GetLength();
	const int iPosPoint = strBuffer.Find('.') + 1;
	int iNaturalLength = iLength - iPosPoint;

	if (iNaturalLength < 4)
	{
		return;
	}


	int iCommaCount = (iNaturalLength - 1) / 3;


	int iDest = 0;
	for (int ii = 0; ii < iNaturalLength; ii++)
	{
		if (	(iNaturalLength - ii) % 3 == 0
			&& ii != 0)
		{
			pcData[iDest++] = ',';
		}

		pcData[iDest++] = strBuffer[ii];
	}

	pcData[iNaturalLength + iCommaCount] = 0;
}

LOGFONT GetSimpleFont(int fsize, DWORD weight)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, "±¼¸²Ã¼");   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = weight;
	lf.lfItalic = FALSE;
	return lf;
}
