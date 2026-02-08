#pragma once

#include <math.h>
#include "stdAfx.h"
#include "Common.h"

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void	AxInit();

protected:
	CWnd	*m_pParent;
	CFont	*m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBodyBk, m_crLine;

public:
	int      Variant(int cmd, int data);
	LPCTSTR  Variant(int cmd, LPCTSTR data);
	CFont*   GetAxFont(LPCTSTR name, int point, bool italic, int bold);
	COLORREF GetIndexColor(int index);
	double   Str2Double(LPCSTR data, int dlen);
	double   Str2Double(const CString &data)	{ return Str2Double(data.GetString(), data.GetLength()); }
	int      Str2Int(LPCSTR data, int dlen);
	int      Str2Int(const CString &data)		{ return Str2Int(data.GetString(), data.GetLength()); }
	CString  Int2CommaStr(int val);
	COLORREF GetColor(double gjga, double curr);
	COLORREF GetColor(double val);

public:
	virtual ~CAxisExt();
};

inline double CAxisExt::Str2Double(LPCSTR data, int dlen)
{
	char	buff[32]{};

	CopyMemory(buff, data, dlen);
	return (buff[1] == '-') ? atof(&buff[1]) : atof(buff);
}

inline int CAxisExt::Str2Int(LPCSTR data, int dlen)
{
	char	buff[32]{};

	CopyMemory(buff, data, dlen); 
	return atoi(buff);
}

inline CString CAxisExt::Int2CommaStr(int val)
{
	char	dst[32]{}, src[32]{};
	char	*buf = nullptr, *str = nullptr;
	int	len = sprintf(src, "%d", val);
	
	if (val < 0)
	{
		dst[0] = '-';
		str = &src[1];
		buf = &dst[1];
		--len;
	}
	else
	{
		str = &src[0];
		buf = &dst[0];
	}
	
	switch ((len - 1) % 3)
	{
	case 3: /* fake label to make gcc happy */
		while (*str) { 
			*buf++ = ','; 
	case 2: *buf++ = *str++; 
	case 1: *buf++ = *str++; 
	case 0: *buf++ = *str++; 
		} 
		break;
	default:break;
	}
	*buf = '\0'; 
	
	return dst;
}
