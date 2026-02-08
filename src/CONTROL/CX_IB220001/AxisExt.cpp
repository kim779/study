#include "stdafx.h"
#include "AxisExt.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxWnd

CAxisExt::CAxisExt(CWnd *pParent)
: m_pParent(pParent)
{
	AxInit();
}

void CAxisExt::AxInit()
{
	m_crMinus  = GetIndexColor(COLOR_MINUSTXT);
	m_crPlus   = GetIndexColor(COLOR_PLUSTXT);
	m_crNormal = GetIndexColor(COLOR_TEXT);
	m_crBodyBk = GetIndexColor(COLOR_BODYBACK);
	m_crLine   = GetIndexColor(COLOR_LINE);
	
	m_pFont  = GetAxFont(_T("±¼¸²Ã¼"), 9, false, 0);
	m_pFontB = GetAxFont(_T("±¼¸²Ã¼"), 9, false, FW_BOLD);
}

CAxisExt::~CAxisExt()
{
}

LPCTSTR CAxisExt::Variant(int cmd, LPCTSTR data)
{
	return (LPCTSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

int CAxisExt::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

CFont* CAxisExt::GetAxFont(LPCTSTR name, int point, bool italic, int bold)
{
	struct	_fontR	fontR {};

	fontR.name   = (LPTSTR)name;
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

COLORREF CAxisExt::GetIndexColor(int index)
{
	return (COLORREF)m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

COLORREF CAxisExt::GetColor(double val)
{
	if (val > 0.0)
		return m_crPlus;
	if (val < 0.0)
		return m_crMinus;
	return m_crNormal;
}

COLORREF CAxisExt::GetColor(double gjga, double curr)
{
	if (gjga < curr)
		return m_crPlus;
	if (gjga > curr)
		return m_crMinus;
	return m_crNormal;
}