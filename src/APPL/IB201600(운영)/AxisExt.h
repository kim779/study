#pragma once

#include <math.h>
#include "Common.h"
#include "../../h/ledger.h"

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void AxInit();
protected:
	CWnd *m_pParent;
	CFont *m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBk, m_crBodyBk, m_crFocus, m_crWhite, m_crLine, m_crMadoBk, m_crMasuBk, m_crBlack;
	CString m_usid;
public:
	int      Variant(int cmd, int data);
	LPCTSTR  Variant(int cmd, LPCTSTR data);
	CFont*   GetAxFont(LPCTSTR name, int point, bool italic, int bold);
	CBitmap* GetAxBitmap(LPCTSTR path);
	HBITMAP  GetAxHBitmap(LPCTSTR path);
	COLORREF GetIndexColor(int index);
	CPen*    GetAxPen(COLORREF clr, int width, int style);
	CBrush*  GetAxBrush(COLORREF clr);
	BOOL     SendTR(LPCSTR name, char type, LPCSTR data, int dlen, int key);
	void     SetView(LPCSTR option);
	void     SetModal(LPCSTR option);
	void     SetTrigger(LPCSTR data);
	CString  Int2Str(int data, int round = 2);
	CString  Int2CommaStr(int val);
	CString  DiffStr(char udgb, int diff, int round = 2);
	COLORREF GetColor(int gjga, int curr);
	struct _ledgerH* GetLedger(struct _ledgerH *ledger);
public:
	virtual ~CAxisExt();
};

inline CString CAxisExt::Int2Str( int data, int round )
{
	char buff[32]{};
	if (round==0)
	{
		sprintf(buff, "%d", data);
	}
	else
	{
		const int pval = gsl::narrow_cast<int>(pow(10, round));
		if (data<0)
			sprintf(buff, "-%d.%0.*d", abs(data)/pval, round, abs(data)%pval);
		else
			sprintf(buff, "%d.%0.*d", data/pval, round, data%pval);
	}
	return buff;
}

inline CString CAxisExt::Int2CommaStr( int val )
{
	char dst[32]{}, src[32]{}, * buf{}, * str{};
	
	int len = sprintf(src, "%d", val);
	
	if (val<0)
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
	
	switch ((len - 1) % 3) {
	case 3: /* fake label to make gcc happy */
		while (*str) { 
			*buf++ = ','; 
	case 2: *buf++ = *str++; 
	case 1: *buf++ = *str++; 
	case 0: *buf++ = *str++; 
		} 
	}
	*buf = '\0'; 
	
	return dst;
}

