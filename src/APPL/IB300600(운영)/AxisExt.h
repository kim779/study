#pragma once

#include <math.h>
#include "Common.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxStatic.h"

class CfxEditEx : public CfxEdit {
public:
	CfxEditEx(CFont *pFont=NULL) : CfxEdit(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CEdit::WindowProc(message, wParam, lParam);
	}
};

class CfxStaticEx : public CfxStatic {
public:
	CfxStaticEx(CFont *pFont=NULL) : CfxStatic(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CfxStatic::WindowProc(message, wParam, lParam);
	}
};

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void AxInit();

// Logging 기능 추가 2011.01.13 by LKM
protected:
	BOOL m_bLogging;
	CString m_bLogPath;
	void SetLogMode(BOOL bLogging);
	int GetLogConfig(LPCSTR key);
	void WriteLog(LPCSTR log, ...);

protected:
	CWnd *m_pParent;
	CFont *m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBk, m_crBodyBk, m_crFocus, m_crWhite, m_crLine, m_crMadoBk, m_crMasuBk, m_crBlack, m_crMadoRsvdBk,m_crMasuRsvdBk;
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
	int      GetNextPrice(int price, CODE_TYPE type, bool isplus);
	int		 GetMiniNextPrice( int price, CODE_TYPE type, bool isplus );
	int      GetTick(int fprc, int tprc, CODE_TYPE type);
	int      GetPrice(int price, CODE_TYPE type, int tick);
	void     SetView(LPCSTR option);
	void     SetModal(LPCSTR option);
	void     SetTrigger(LPCSTR data);
	double   Str2Double(LPCSTR data, int dlen);
	double   Str2Double(CString &data) { return Str2Double((LPCSTR)data, data.GetLength()); }
	double   Str2Double(StringProxy &sp) { return Str2Double(sp.first, sp.second-sp.first); }
	double	 Str2Double(char* data) { return Str2Double(data,sizeof(data)); }
	int      Str2Int(LPCSTR data, int dlen);
	int      Str2Int(CString &data) { return Str2Int((LPCSTR)data, data.GetLength()); }
	int      Str2Int(StringProxy &sp) { return Str2Int(sp.first, sp.second-sp.first); }
	int		 Str2Int(char* data) { return Str2Int(data,sizeof(data));}
	int      GetGjga(int curr, int diff, char udgb);
	CString  Int2Str(int data, int round = 2);
	CString  Int2CommaStr(int val);
	CString  DiffStr(char udgb, int diff, int round = 2);
	COLORREF GetColor(int gjga, int curr);
	struct _ledgerH* GetLedger(struct _ledgerH *ledger);
	void     ParseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval);
	void     ParseString(StringProxy &sp, CHAR del, vector<StringProxy> &retval);
public:
	virtual ~CAxisExt();
};

inline double CAxisExt::Str2Double( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	//CString slog;
	//slog.Format("SRC : [%s]	CONVERT : [%s]\n",data,buff);
	//OutputDebugString(slog);
	buff[dlen] = 0;
	return (buff[1]=='-' || buff[1]=='+' || buff[1]==' ') ? atof(&buff[1]) : atof(&buff[0]);
}

inline int CAxisExt::Str2Int( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	buff[dlen] = 0;
	return atoi(buff);
}

inline CString CAxisExt::Int2Str( int data, int round )
{
	char buff[32];
	if (round==0)
	{
		sprintf(buff, "%d", data);
	}
	else
	{
		int pval = (int)pow(10, round);
		if (data<0)
			sprintf(buff, "-%d.%0.*d", abs(data)/pval, round, abs(data)%pval);
		else
			sprintf(buff, "%d.%0.*d", data/pval, round, data%pval);
	}
	return buff;
}

inline CString CAxisExt::Int2CommaStr( int val )
{
	char dst[32], src[32], *buf, *str;
	
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
		/* fake label to make gcc happy */
	case 3: 
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


inline void CAxisExt::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
{
	retval.clear();
	LPCSTR pos;
	while(st<ed)
	{
		pos = find(st, ed, del);
		retval.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
}

inline int CAxisExt::GetTick( int fprc, int tprc, CODE_TYPE type )
{
	int tick=0;
	if (fprc<tprc) 
	{
		for(; fprc<tprc; ++tick)
			fprc = GetNextPrice(fprc, type, true);
	}
	else if (fprc>tprc)
	{
		for(; fprc>tprc; --tick)
			fprc = GetNextPrice(fprc, type, false);
	}
	return tick;
}

inline int CAxisExt::GetPrice(int price, CODE_TYPE type, int tick)
{
	bool isplus = (tick>0);
	tick = (isplus) ? tick : -tick;
	for(int n=0; n<tick; ++n)
		price = GetNextPrice(price, type, isplus);
	return price;
}


