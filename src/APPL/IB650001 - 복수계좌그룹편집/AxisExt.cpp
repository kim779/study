#include "stdafx.h"
#include "AxisExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAxisExt::CAxisExt(CWnd *pParent)
: m_pParent(pParent)
{
	AxInit();
}

void CAxisExt::AxInit()
{
	m_crMinus = GetIndexColor(COLOR_MINUSTXT);
	m_crPlus  = GetIndexColor(COLOR_PLUSTXT);
	m_crNormal= GetIndexColor(COLOR_TEXT);
	m_crBk = GetIndexColor(COLOR_BK);
	m_crBodyBk = GetIndexColor(COLOR_BODYBACK);
	m_crFocus =  GetIndexColor(COLOR_FOCUS);
	m_crLine = GetIndexColor(COLOR_LINE);
	m_crMadoBk = RGB(0xE6, 0xE6, 0xFF);
	m_crMasuBk = RGB(0xFF, 0xE8, 0xF3);
	m_crBlack = RGB(0, 0, 0);
	
	m_crWhite = RGB(255, 255, 255);
	m_pFont  = GetAxFont(_T("±¼¸²Ã¼"), 9, false, 0);
	m_pFontB = GetAxFont(_T("±¼¸²Ã¼"), 9, false, FW_BOLD);
	
	m_usid = Variant(userCC, "");
}

CAxisExt::~CAxisExt()
{
}

LPCTSTR CAxisExt::Variant( int cmd, LPCTSTR data )
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

int CAxisExt::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

CFont* CAxisExt::GetAxFont(LPCTSTR name, int point, bool italic, int bold)
{
	struct _fontR fontR;
	fontR.name   = (LPTSTR)name;
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CAxisExt::GetAxBitmap(LPCTSTR path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path);
}

HBITMAP CAxisExt::GetAxHBitmap(LPCTSTR path)
{
	CBitmap* pBmp = GetAxBitmap(path);
	return (pBmp) ? pBmp->operator HBITMAP() : NULL;
}

COLORREF CAxisExt::GetIndexColor(int index)
{
	return (COLORREF)m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CPen* CAxisExt::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CAxisExt::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

BOOL CAxisExt::SendTR(LPCSTR name, BYTE type, LPCSTR data, int dlen, int key)
{
	if (strlen(name)!=8) return FALSE;

	char buff[1024*16];

	// Header setting
	struct _userTH *udat = (struct _userTH*)buff;
	CopyMemory(udat->trc, name, 8);
	udat->key  = key;
	udat->stat = type;
	
	// Data setting
	CopyMemory(&buff[L_userTH], data, dlen);
	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dlen), (LPARAM)buff);
}

struct _ledgerH* CAxisExt::GetLedger( struct _ledgerH *ledger )
{
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	LPCSTR usid = Variant(userCC, (LPCSTR)NULL);
	CopyMemory(ledger->usid, usid, strlen(usid));
	CopyMemory(ledger->fkey, "C", 1);
	CopyMemory(ledger->mkty, "1", 1);

	return ledger;
}

void CAxisExt::SetView( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)option);
}

void CAxisExt::SetModal( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)option);
}

void CAxisExt::SetTrigger( LPCSTR data )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)data);
}

int CAxisExt::Str2Int( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	buff[dlen] = 0;
	return atoi(buff);
}

double CAxisExt::Str2Double( LPCSTR data, int dlen )
{
	char buff[32];
	CopyMemory(buff, data, dlen);
	buff[dlen] = 0;
	return (buff[1]=='-') ? atof(&buff[1]) : atof(&buff[0]);
}

void CAxisExt::ParseString( StringProxy &sp, CHAR del, vector<StringProxy> &retval )
{
	ParseString(sp.first, sp.second, del, retval);
}

void CAxisExt::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
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