#pragma once

#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <functional>
#include <algorithm>
using namespace std;

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxLineStyleCombo.h"
#include "../../control/fx_misc/misctype.h"

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// 매도 back
#define	COLOR_MSB			170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78
#define COLOR_CONTROLBACK	66

#define PADDING_VALUE	(6)


class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};

class op_notice_spliter {
public:
	bool operator() (char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void AxInit();
protected:
	CWnd *m_pParent;
	CFont *m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBk, m_crBodyBk, m_crFocus, m_crWhite, m_crLine, m_crMadoBk, m_crMasuBk, m_crBlack, m_crCtrlBk;
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
	void     SetSize(int width, int height);
	void     SetTrigger(LPCSTR data);
	double   Str2Double(LPCSTR data, int dlen);
	double   Str2Double(const CString &data) { return Str2Double((LPCSTR)data, data.GetLength()); }
	double   Str2Double(const StringProxy &sp) { return Str2Double(sp.first, sp.second-sp.first); }
	int      Str2Int(LPCSTR data, int dlen);
	int      Str2Int(const CString &data) { return Str2Int((LPCSTR)data, data.GetLength()); }
	int      Str2Int(const StringProxy &sp) { return Str2Int(sp.first, sp.second-sp.first); }
	int      GetGjga(int curr, int diff, char udgb);
	CString  Int2Str(int data, int round = 2);
	CString  Int2CommaStr(int val);
	CString  DiffStr(char udgb, int diff, int round = 2);
	COLORREF GetColor(int gjga, int curr);
	struct _ledgerH* GetLedger(struct _ledgerH *ledger);
	void     ParseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval);
	void     ParseString(const StringProxy &sp, CHAR del, vector<StringProxy> &retval);
public:
	virtual ~CAxisExt();
};

inline double CAxisExt::Str2Double( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	buff[dlen] = 0;
	return (buff[1]=='-') ? atof(&buff[1]) : atof(&buff[0]);
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
	char buff[32]{};
	if (round==0)
	{
		sprintf_s(buff, "%d", data);
	}
	else
	{
		const int pval = (int)pow(10, round);
		if (data<0)
			sprintf_s(buff, "-%d.%0.*d", abs(data)/pval, round, abs(data)%pval);
		else
			sprintf_s(buff, "%d.%0.*d", data/pval, round, data%pval);
	}
	return buff;
}

inline CString CAxisExt::Int2CommaStr( int val )
{
	char dst[32]{}, src[32]{};

	char* buf = NULL;
	char* str = NULL;
	
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


inline void CAxisExt::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
{
	retval.clear();
	LPCSTR pos{};
	while(st<ed)
	{
		pos = find(st, ed, del);
		retval.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
}

