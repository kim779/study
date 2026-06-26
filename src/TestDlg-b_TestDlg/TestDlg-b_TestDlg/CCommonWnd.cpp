#include "pch.h"
#include "CCommonWnd.h"

CCommonWnd::CCommonWnd(CWnd* pParent)
	: m_pParent(pParent)
{
	AxInit();
}

void CCommonWnd::AxInit()
{
	m_crMinus = GetIndexColor(COLOR_MINUSTXT);
	m_crPlus = GetIndexColor(COLOR_PLUSTXT);
	m_crNormal = GetIndexColor(COLOR_TEXT);
	m_crBk = GetIndexColor(COLOR_BK);
	m_crBodyBk = GetIndexColor(COLOR_BODYBACK);
	m_crFocus = GetIndexColor(COLOR_FOCUS);
	m_crLine = GetIndexColor(COLOR_LINE);
	m_crCtrlBk = GetIndexColor(COLOR_CONTROLBACK);
	m_crMadoBk = RGB(0xE6, 0xE6, 0xFF);
	m_crMasuBk = RGB(0xFF, 0xE8, 0xF3);
	m_crBlack = RGB(0, 0, 0);

	m_crWhite = RGB(255, 255, 255);
	m_pFont = GetAxFont(_T("굴림체"), 9, false, 0);
	m_pFontB = GetAxFont(_T("굴림체"), 9, false, FW_BOLD);

	m_usid = Variant(userCC, "");
}

CCommonWnd::~CCommonWnd()
{

}

LPCTSTR CCommonWnd::Variant(int cmd, LPCTSTR data)
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

int CCommonWnd::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

CFont* CCommonWnd::GetAxFont(LPCTSTR name, int point, bool italic, int bold)
{
	struct _fontR fontR;
	fontR.name = (LPTSTR)name;
	fontR.point = point;
	fontR.italic = italic;
	fontR.bold = bold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CCommonWnd::GetAxBitmap(LPCTSTR path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path);
}

HBITMAP CCommonWnd::GetAxHBitmap(LPCTSTR path)
{
	CBitmap* pBmp = GetAxBitmap(path);
	return (pBmp) ? pBmp->operator HBITMAP() : NULL;
}

COLORREF CCommonWnd::GetIndexColor(int index)
{
	return (COLORREF)m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CPen* CCommonWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CCommonWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

BOOL CCommonWnd::SendTR(LPCSTR name, char type, LPCSTR data, int dlen, int key)
{
	if (strlen(name) != 8) return FALSE;

	std::unique_ptr<char[]> buff = std::make_unique<char[]>(1024 * 6);

	// Header setting
	struct _userTH* udat = (struct _userTH*)buff.get();
	CopyMemory(udat->trc, name, 8);
	udat->key = key;
	udat->stat = type;

	// Data setting
	CopyMemory(&buff.get()[L_userTH], data, dlen);

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, dlen), (LPARAM)buff.get());
}

int CCommonWnd::GetNextPrice(int price, CODE_TYPE type, bool isplus)
{
	switch (type)
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
			if (price >= 300)
				return price + 5;
			else
				return price + 1;
		}
		else
		{
			if (price > 300)
				return price - 5;
			else
				return price - 1;
		}
		break;
	}
	MessageBox(0, "가격산정오류", "IBK투자증권", MB_OK | MB_ICONERROR);
	return 0;
}

COLORREF CCommonWnd::GetColor(double gjga, double curr)
{
	if (gjga < curr) return m_crPlus;
	else if (gjga > curr) return m_crMinus;
	else                return m_crNormal;
}

CString CCommonWnd::DiffStr(char udgb, int diff, int round /*= 2*/)
{
	int pos{};
	char buff[32]{};

	switch (udgb)
	{
	case '1':
		pos = sprintf_s(buff, sizeof(buff), "↑");
	case '2':
	case '+':
		pos = sprintf_s(buff, sizeof(buff), "▲");
	case '5':
	case '-':
		pos = sprintf_s(buff, sizeof(buff), "▼");
	case '4':
		pos = sprintf_s(buff, sizeof(buff), "↓");
	case '3':
	default:
		pos = sprintf_s(buff, sizeof(buff), " ");
	}

	if (round == 0)
	{
		sprintf_s(&buff[pos], sizeof(buff) - pos, "%2d", diff);
	}
	else
	{
		const int pval = (int)pow(10, round);
		//sprintf(&buff[pos], "%2d.%0.*d", diff / pval, round, diff % pval);
		sprintf_s(&buff[pos], sizeof(buff) - pos, "%2d.%0.*d", diff / pval, round, diff % pval);
	}
	return buff;
}

int CCommonWnd::GetGjga(int curr, int diff, char udgb)
{
	if (udgb == '1' || udgb == '2' || udgb == '+') return curr - diff;
	else if (udgb == '4' || udgb == '5' || udgb == '-') return curr + diff;
	return curr;
}

struct _ledgerH* CCommonWnd::GetLedger(struct _ledgerH* ledger)
{
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	LPCSTR usid = Variant(userCC, (LPCSTR)NULL);
	CopyMemory(ledger->usid, usid, strlen(usid));
	CopyMemory(ledger->fkey, "C", 1);
	CopyMemory(ledger->mkty, "1", 1);

	return ledger;
}

void CCommonWnd::ParseString(const StringProxy& sp, CHAR del, vector<StringProxy>& retval)
{
	ParseString(sp.first, sp.second, del, retval);
}

void CCommonWnd::SetView(LPCSTR option)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)option);
}

void CCommonWnd::SetModal(LPCSTR option)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeMODAL), (LPARAM)option);
}

void CCommonWnd::SetTrigger(LPCSTR data)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)data);
}

void CCommonWnd::ParseNotice(LPCSTR st, LPCSTR ed, vector<StringProxy>& retval)
{
	op_notice_splitter op;
	retval.clear();
	LPCSTR pos{};
	while (st < ed)
	{
		pos = find_if(st, ed, op);
		retval.push_back(StringProxy(st, pos));
		st = pos + 1;
	}
}
//
void CCommonWnd::ParseAccount(LPCSTR st, LPCSTR ed, vector<StringProxy>& retval)
{
	op_account_splitter op;
	retval.clear();
	LPCSTR pos{};
	while (st < ed)
	{
		pos = find_if(st, ed, op);
		retval.push_back(StringProxy(st, pos));
		st = pos + 1;
	}
}
//
void CCommonWnd::SetPopup(LPCSTR option, BOOL bClose)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, bClose ? typePOPUPXN : typePOPUPX), (LPARAM)option);
}

void CSortStringArray::Sort()
{
	BOOL bNotDone = TRUE;

	while (bNotDone)
	{
		bNotDone = FALSE;
		for (int pos = 0; pos < GetUpperBound(); pos++)
			bNotDone |= CompareAndSwap(pos);
	}
}
//
BOOL CSortStringArray::CompareAndSwap(int pos)
{
	CString temp;
	const int posFirst = pos;
	const int posNext = pos + 1;

	if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
	{
		temp = GetAt(posFirst);
		SetAt(posFirst, GetAt(posNext));
		SetAt(posNext, temp);
		return TRUE;
	}
	return FALSE;
}
