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
	for (int ii = 0; ii < m_pens.GetSize(); ii++)
		AddString(_T(""));
	
	SetCurSel(0);
	CComboBox::PreSubclassWindow();
}

void CfxLineStyleCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (m_pens.GetSize() <= 0)
		return;

	CDC	dc;
	CRect	rect;
	int	y;
	CPen*	ppen;

	dc.Attach(lpDrawItemStruct->hDC);
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	rect.DeflateRect(2, 2);
	ppen = dc.SelectObject((CPen *)m_pens.GetAt(lpDrawItemStruct->itemID));
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
	int	index = GetCurSel();
	if (index == CB_ERR || index >= m_pens.GetSize())
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
	CPen*	pen;
	
	m_pens.RemoveAll();
	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		pen = new CPen;
		pen->CreatePen(lineStyle[ii], 1, RGB(0,0,0));
		m_pens.Add(pen);
	}
}

void CfxLineStyleCombo::removePen()
{
	CPen*	pen;

	for (int ii = 0; ii < maxSTYLE; ii++)
	{
		pen = (CPen *)m_pens.GetAt(ii);
		pen->DeleteObject();
		delete pen;
	}
	m_pens.RemoveAll();
}