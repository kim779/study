#include "stdafx.h"
#include "gendef.h"

/*
CMapWnd* GetMainWnd(){
	return g_MapWnd;
}
*/

int GetSignText(CString& text )
{
	if (text.GetLength() <= 0)
		return 0;

	int	nSign = 0;
	CString temps;
	switch (text[0])
	{
	case '1':	temps = ARR_UP;	nSign = 1;	break;
	case '2':
	case '+':	temps = TRI_UP;	nSign = 1;	break;
	case '4':	temps = ARR_DN; nSign = 2;	break;
	case '5':
	case '-':	temps = TRI_DN; nSign = 2;	break;
	default:	temps = _T(" ");		break;
	}

	text = text.Mid(1); 
	text.Insert(0, temps);
	return nSign;
}

long ConvertToInt(CString str){
	CString temp = str;
	if( temp=="")
		return 0;

	int nIdx=-1;
	while( (nIdx= temp.Find(",")) >=0 ){

		temp = temp.Left(nIdx ) + temp.Mid(nIdx+1);

	}
	return atol((char*)(LPCTSTR)temp);
}

double ConvertToDouble(CString str){
	CString temp = str;
	if( temp=="")
		return 0;

	int nIdx=-1;
	while( (nIdx= temp.Find(",")) >=0 ){

		temp = temp.Left(nIdx ) + temp.Mid(nIdx+1);

	}

	return atof((char*)(LPCTSTR)temp);
}