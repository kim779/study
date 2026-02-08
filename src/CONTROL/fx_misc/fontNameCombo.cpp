// fontNameCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fontNameCombo.h"
#include <afxdlgs.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define dxBITMAP        16
#define dyBITMAP        12

/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo

CfontNameCombo::CfontNameCombo()
{
	HINSTANCE hinst = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetModuleHandle("fx_misc.dll"));
	m_loaded = m_bitmap.LoadBitmap(IDB_TRUETYPE);
	initMaskFromBitmap(&m_bitmap, &m_mask);
	AfxSetResourceHandle(hinst);
}

CfontNameCombo::~CfontNameCombo()
{
	if (m_loaded)
	{
		m_bitmap.DeleteObject();
		m_mask.DeleteObject();
	}
}


BEGIN_MESSAGE_MAP(CfontNameCombo, CComboBox)
	//{{AFX_MSG_MAP(CfontNameCombo)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfontNameCombo message handlers
void CfontNameCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);

	if (lpDIS->itemID == -1)
		return;

	int	x{}, y{};
	CDC*	dc   = CDC::FromHandle(lpDIS->hDC);
	const fontITEM* fI = (fontITEM *)lpDIS->itemData;
	const LOGFONT*  lf = &fI->lf;
	COLORREF  crBk{}, crText{};
	TEXTMETRIC tm{};

	crBk   = dc->SetBkColor(GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHT : COLOR_WINDOW));
	crText = dc->SetTextColor(GetSysColor(lpDIS->itemState & ODS_SELECTED ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));

	dc->GetTextMetrics(&tm);
	x = LOWORD(GetDialogBaseUnits()) / 4;
	y = (lpDIS->rcItem.bottom + lpDIS->rcItem.top - tm.tmHeight) / 2;

	dc->ExtTextOut(lpDIS->rcItem.left + dxBITMAP + x, y, ETO_CLIPPED | ETO_OPAQUE,
			&lpDIS->rcItem, (LPCTSTR)lf->lfFaceName, lstrlen((LPCTSTR) lf->lfFaceName), nullptr);

	dc->SetTextColor(crText);
	dc->SetBkColor(crBk);

	if (fI->dwType & TRUETYPE_FONTTYPE)
	{
		const int dy = ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) - dxBITMAP) / 2;
		drawMaskedBitmap(dc, &m_bitmap, &m_mask, x, lpDIS->rcItem.top+dy, dxBITMAP, dyBITMAP);
	}

	if (lpDIS->itemState & ODS_FOCUS)
		dc->DrawFocusRect(&lpDIS->rcItem);
}

void CfontNameCombo::DeleteItem(LPDELETEITEMSTRUCT lpDIS) 
{
	if (lpDIS->itemID == -1)
		return;

	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);

	const int sel = lpDIS->itemID;
	if (_vFont.begin() + sel < _vFont.end())
		_vFont.erase(_vFont.begin() + sel);

//	fontITEM* fI = GetFontItem(lpDIS->itemID);
//	ASSERT(fI);
//	delete fI;
}

void CfontNameCombo::OnDropdown() 
{
	CRect	rect;
	int	width;

	GetWindowRect(&rect);
	width = (int)((float)rect.Width() * 1.6);
	SetDroppedWidth(width);
}

int CfontNameCombo::AddFont(LOGFONT* lf, DWORD type)
{
	int	entry{};
	if (FindString(-1, (LPCTSTR)lf->lfFaceName) != CB_ERR)
		return CB_ERR;

	entry = AddString((LPCTSTR)(*lf).lfFaceName);
	if (entry != CB_ERR)
	{
		auto& font = _vFont.emplace_back(std::move(std::make_unique<fontITEM>(type, *lf)));
		SetItemData(entry, (DWORD)font.get());
	}
	return entry;

}

CString CfontNameCombo::GetCurrentName()
{
	CString str;

	GetWindowText(str);
	return str;
}

fontITEM* CfontNameCombo::GetFontItem(int sel)
{
	if (sel == -1)
		sel = GetCurSel();

	if (sel == -1)
	{
		CString str;

		GetWindowText(str);
		sel = FindString(-1, str);
		if (sel == CB_ERR)
			sel = 0;
	}

	ASSERT(GetItemData(sel));	
	return (fontITEM *)GetItemData(sel);
}

DWORD CfontNameCombo::GetFontType(int sel)
{
	return GetFontItem(sel)->dwType;
}

#define DSx     0x00660046L
#define DSna    0x00220326L

void CfontNameCombo::drawMaskedBitmap(CDC* dc, CBitmap* pbmp, CBitmap* pbmpMask, int x, int y, int cx, int cy)
{
	CDC	memDC;
	CBitmap* pBMP{};
	const COLORREF pBK   = dc->SetBkColor(RGB(255, 255, 255));
	const COLORREF pTEXT = dc->SetTextColor(RGB(0, 0, 0));

	memDC.CreateCompatibleDC(dc);
	pBMP = memDC.SelectObject(pbmp);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSx);
	memDC.SelectObject(pbmpMask);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSna);
	memDC.SelectObject(pbmp);
	dc->BitBlt(x, y, cx, cy, &memDC, 0, 0, DSx);
	memDC.SelectObject(pBMP);

	dc->SetBkColor(pBK);
	dc->SetTextColor(pTEXT);
}

void CfontNameCombo::initMaskFromBitmap(CBitmap* pbmp, CBitmap* pbmpMask)
{
	BITMAP	bm{};
	CDC	destDC, srcDC;
	CBitmap* pDEST{}, * pSRC{};
	COLORREF pBK{};

	pbmp->GetObject(sizeof(BITMAP), &bm);
	pbmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, nullptr);

	destDC.CreateCompatibleDC(nullptr);
	srcDC.CreateCompatibleDC(nullptr);
	pDEST = destDC.SelectObject(pbmpMask);
	pSRC  = srcDC.SelectObject(pbmp);
	pBK   = srcDC.SetBkColor(srcDC.GetPixel(0, 0));
	destDC.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &srcDC, 0, 0, NOTSRCCOPY);
	srcDC.SetBkColor(pBK);

	destDC.SelectObject(pDEST);
	srcDC.SelectObject(pSRC);
}
