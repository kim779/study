// AxisExt.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "AxisExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAxisExt::CAxisExt(CWnd *pParent)
: m_pParent(pParent), m_bLogging(FALSE), m_bLogPath("")
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
	m_crMadoRsvdBk = RGB(242,242,252);
	m_crMasuBk = RGB(0xFF, 0xE8, 0xF3);
	m_crMasuRsvdBk = RGB(255,243,247);
	m_crBlack = RGB(0, 0, 0);
	
	m_crWhite = RGB(255, 255, 255);
	m_pFont  = GetAxFont(_T("굴림체"), 9, false, 0);
	m_pFontB = GetAxFont(_T("굴림체"), 9, false, true);
	
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

CFont* CAxisExt::GetAxFont(CString fName, int point, bool bItalic, bool bBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CAxisExt::GetAxFont(struct _fontR* font)
{
	return GetAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

CBitmap* CAxisExt::GetAxBitmap(LPCTSTR path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path);
}

HBITMAP CAxisExt::GetAxHBitmap(LPCTSTR path)
{
	CBitmap* pBmp = GetAxBitmap(path);
	return (pBmp) ? pBmp->operator HBITMAP() : nullptr;
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

BOOL CAxisExt::SendTR(LPCSTR name, std::byte type, LPCSTR data, int dlen, int key)
{
	if (strlen(name)!=8) return FALSE;

	std::string buff;
	buff.resize(1024*16);

	// Header setting
	struct _userTH *udat = (struct _userTH*)buff.data();
	CopyMemory(udat->trc, name, 8);
	udat->key  = key;
	udat->stat = (char)type;
	
	// Data setting
	CopyMemory(&buff[L_userTH], data, dlen);
	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dlen), (LPARAM)buff.data());
}

int CAxisExt::GetNextPrice( int price, CODE_TYPE type, bool isplus )
{
	switch(type)
	{
	case CT_SPREAD:
	case CT_FUTURE:
		if (isplus) 
			return price + 5;
		else
			return price - 5;
		break;
	case CT_OPTION:
		if (isplus)
		{
			if (price>=300)
				return price + 5;
			else
				return price + 1;
		}
		else
		{
			if (price>300)
				return price - 5;
			else
				return price - 1;
		}
		break;
	}
	MessageBox(0,  "가격산정오류", "IBK투자증권", MB_OK|MB_ICONERROR);
	return 0;
}

COLORREF CAxisExt::GetColor( int gjga, int curr )
{
	if      (gjga<curr) return m_crPlus;
	else if (gjga>curr) return m_crMinus;
	else                return m_crNormal;
}

CString CAxisExt::DiffStr( char udgb, int diff, int round /*= 2*/ )
{
	int pos{};
	char buff[32]{};

	switch(udgb)
	{
	case '1': 
		pos = sprintf(buff, "↑"); break;
	case '2':
	case '+':
		pos = sprintf(buff, "▲"); break;
	case '5':
	case '-':
		pos = sprintf(buff, "▼"); break;
	case '4':
		pos = sprintf(buff, "↓"); break;
	case '3':
	default:
		pos = sprintf(buff, " "); break;
	}

	if (round==0)
	{
		sprintf(&buff[pos], "%2d", diff);
	}
	else
	{
		const int pval = gsl::narrow_cast<int>(pow(10, round));
		sprintf(&buff[pos], "%2d.%0.*d", diff/pval, round, diff%pval);
	}
	return buff;
}

int CAxisExt::GetGjga( int curr, int diff, char udgb )
{
	if (udgb=='1' || udgb=='2' || udgb=='+') return curr-diff;
	else if (udgb=='4' || udgb=='5' || udgb=='-') return curr+diff;
	return curr;
}

struct _ledgerH* CAxisExt::GetLedger( struct _ledgerH *ledger )
{
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	LPCSTR usid = Variant(userCC, (LPCSTR)nullptr);
	CopyMemory(ledger->usid, usid, strlen(usid));
	CopyMemory(ledger->fkey, "C", 1);
	CopyMemory(ledger->mkty, "1", 1);

	return ledger;
}

void CAxisExt::ParseString(const StringProxy &sp, CHAR del, vector<StringProxy> &retval )
{
	ParseString(sp.first, sp.second, del, retval);
}

void CAxisExt::SetView( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)option);
}

void CAxisExt::SetPopup( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)option);
}

void CAxisExt::SetModal( LPCSTR option )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)option);
}

void CAxisExt::SetTrigger( LPCSTR data )
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)data);
}

void CAxisExt::SetLogMode( BOOL bLogging )
{
	if (m_bLogging == bLogging) return;

	m_bLogging = bLogging;

	if (m_bLogging)
	{
		LPCSTR home = Variant(homeCC, "");
		LPCSTR usid = Variant(userCC, "");
		const  int major = Variant(majorCC, 0);
		const CTime time = CTime::GetCurrentTime();
		m_bLogPath.Format("%s\\user\\%s_%04X_%s.log", home, usid, major, time.FormatGmt("%Y%m%d"));
		//MessageBox(0, m_bLogPath, "", MB_OK);
	}
}

void CAxisExt::WriteLog( LPCSTR log, ... )
{
	if (!m_bLogging) return;
	if (m_bLogPath.IsEmpty()) return;

	FILE *fp = fopen((LPCSTR)m_bLogPath, "a+");
	if (!fp) return;
	
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	char buff[80];
	const int dlen = sprintf(buff, "[%02d/%02d %02d:%02d:%02d.%03d] ", 
					tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	fwrite(buff, 1, dlen, fp);
	
	va_list argptr;
	va_start(argptr, log);
	vfprintf(fp, log, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);
}

int CAxisExt::GetLogConfig( LPCSTR key )
{
	CString iniPath;
	LPCSTR home = Variant(homeCC, "");
	LPCSTR name = Variant(nameCC, "");
	
	iniPath.Format("%s\\user\\%s\\DEBUG.INI", home, name);
	return GetPrivateProfileInt("ENABLE", key, 0, iniPath);
}

CString CAxisExt::GetCodeName(LPCSTR code)
{
	CString	ret = _T("");
	CString	sztmp;
	sztmp.Format("%s", code);
	ret.Format("%s", (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)sztmp.operator LPCTSTR()));
	ret.TrimLeft(), ret.TrimRight();
	return ret;
}

