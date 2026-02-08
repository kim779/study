// fxLineStyleCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fxLineStyleCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	maxSTYLE	4
int	lineStyle[maxSTYLE] = {
	PS_SOLID,
	PS_DASH,
	PS_DOT,
	PS_DASHDOT,
};
/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo

CfxLineStyleCombo::CfxLineStyleCombo()
{
	loadPen();
}

CfxLineStyleCombo::~CfxLineStyleCombo()
{
	removePen();
}


BEGIN_MESSAGE_MAP(CfxLineStyleCombo, CComboBox)
	//{{AFX_MSG_MAP(CfxLineStyleCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxLineStyleCombo message handlers

void CfxLineStyleCombo::PreSubclassWindow() 
{
	for (size_t ii = 0; ii < _vPen.size(); ii++)
		AddString(_T(""));
	
	SetCurSel(0);
	CComboBox::PreSubclassWindow();
}

void CfxLineStyleCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (_vPen.size() <= 0)
		return;

	CDC	dc;
	CRect	rect;
	int	y{};
	CPen* ppen{};

	dc.Attach(lpDrawItemStruct->hDC);
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	rect.DeflateRect(2, 2);
	ppen = dc.SelectObject((CPen *)_vPen.at(lpDrawItemStruct->itemID).get());
	y = rect.top + rect.Height()/2;
	dc.MoveTo(rect.left, y);
	dc.LineTo(rect.right, y);
	dc.SelectObject(ppen);

	rect.InflateRect(2, 2);
	dc.DrawEdge(rect, BDR_SUNKENINNER, BF_RECT);
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		rect.DeflateRect(1, 1);
		dc.DrawFocusRect(rect);
	}

	dc.Detach();
}

int CfxLineStyleCombo::GetStyle()
{
	const size_t index = GetCurSel();
	if (index == CB_ERR || index >= _vPen.size())
		return PS_SOLID;

	return lineStyle[index];
}

void CfxLineStyleCombo::SetStyle(int style)
{
	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		if (lineStyle[ii] == style)
		{
			SetCurSel(ii);
			return;
		}
	}

	SetCurSel(0);
}

void CfxLineStyleCombo::loadPen()
{
	_vPen.clear();
	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		auto& it = _vPen.emplace_back(std::make_unique<CPen>());
		it->CreatePen(lineStyle[ii], 1, RGB(0, 0, 0));
	}
}

void CfxLineStyleCombo::removePen()
{
	_vPen.clear();
}