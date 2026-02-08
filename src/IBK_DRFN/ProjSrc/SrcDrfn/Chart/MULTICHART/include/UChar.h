// UChar.h: interface for the UChar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UCHAR_H__44AF4A33_B155_4D56_982C_149F68962EAE__INCLUDED_)
#define AFX_UCHAR_H__44AF4A33_B155_4D56_982C_149F68962EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

class UChar  
{
public:
	UChar();
	virtual ~UChar();
	
	static double atoff(LPSTR src, UINT uSize); 
	static int atoif(LPSTR src, UINT uSize); 
	static int atoaf(LPSTR src, UINT uSize, LPSTR dest);
	static CString atoaf(LPSTR src, UINT uSize);

	static int RemoveChar(LPSTR pData, char ch, UINT uSize);
	static int RemoveChar(CString strData, char ch);
	static int Str2CommaStr(LPSTR dest, LPSTR src,UINT uSize);
	static int Double2CommaStr(LPSTR dest, double src);
	static int Int2CommaStr(LPSTR dest, int src);
	
	static void SetTextItem(char *str, int len, char *src);
	static void SetDoubleItem(char *str, int len, double value, int point=2);
	static void SetIntItem(char *str, int len, int value);

	static long		dtol(const double value, int point=2);
	static double	ltod(const long   value, int point=2);


};

#endif // !defined(AFX_UCHAR_H__44AF4A33_B155_4D56_982C_149F68962EAE__INCLUDED_)
