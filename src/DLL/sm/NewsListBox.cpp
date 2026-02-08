// NewsListBox.cpp : implementation file
//

#include "stdafx.h"
#include "NewsListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewsListBox

NewsListBox::NewsListBox()
{
}

NewsListBox::~NewsListBox()
{
}


BEGIN_MESSAGE_MAP(NewsListBox, CListBox)
	//{{AFX_MSG_MAP(NewsListBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewsListBox message handlers

void NewsListBox::DeleteAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		delete (LPNEWSITEM)GetItemDataPtr(i);
	}

	ResetContent();
}

void NewsListBox::Init()
{
	m_backColor[0] = 0x00FFFFFF;
	m_backColor[1] = 0x00F5F5F5;
	m_foreColor    = 0x002C2C2C;
	m_gridColor    = 0x00D3D0CB;
}

const int maxItem = 50;

int NewsListBox::AddItem(const char* date, const char* source, const char* title, const char* itemCd)
{

#ifdef DF_USESTL_NLISTBOX
	std::unique_ptr<NEWSITEM> item = std::make_unique<NEWSITEM>();

	item->date = date;
	item->source = source;
	item->title = title;
	item->item = itemCd;

	const int result = AddString(title);

	SetItemDataPtr(result, item.release());

	if (GetCount() > maxItem)
	{
		const gsl::not_null<LPNEWSITEM> item = (LPNEWSITEM)GetItemDataPtr(maxItem);
		//ASSERT(item);
		delete item;
		DeleteString(maxItem);
	}
	return result;
#else
	LPNEWSITEM item = new NEWSITEM;

	item->date = date;
	item->source = source;
	item->title = title;
	item->item = itemCd;

	const int result = AddString(title);

	SetItemDataPtr(result, item);

	if (GetCount() > maxItem)
	{
		item = (LPNEWSITEM)GetItemDataPtr(maxItem);
		ASSERT(item);
		delete item;
		DeleteString(maxItem);
	}
	return result;
#endif
}

int NewsListBox::AddEmptyItem()
{
	return AddItem("", "", "", "");
}
//#define DF_NULLSTRING 1
/************************************************************************
    UINT        CtlType;
    UINT        CtlID;
    UINT        itemID;
    UINT        itemAction;
    UINT        itemState;
    HWND        hwndItem;
    HDC         hDC;
    RECT        rcItem;
    DWORD       itemData;
************************************************************************/
void NewsListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	const int index = lpDrawItemStruct->itemID;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	COLORREF backColor{}, textColor{};
	
//	if (lpDrawItemStruct->itemState)
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		backColor = 0x0092623B;
		textColor = 0x00FFFFFF;
	}
	else
	{
		backColor = m_backColor[index % 2];
		textColor = m_foreColor;
	}

	const LPNEWSITEM item = (LPNEWSITEM)GetItemDataPtr(index);
	ASSERT(item);

	CRect rcItem(lpDrawItemStruct->rcItem);

	dc.FillSolidRect(rcItem, backColor);
	dc.SetTextColor(textColor);
	dc.SetBkMode(TRANSPARENT);

	const int grid1 = 38, grid2 = 72;

	CPen pen(PS_SOLID, 1, m_gridColor);
	CPen* oldPen = dc.SelectObject(&pen);

	dc.MoveTo(grid1, rcItem.top);
	dc.LineTo(grid1, rcItem.bottom);

	dc.MoveTo(grid2, rcItem.top);
	dc.LineTo(grid2, rcItem.bottom);

	if (index == GetCount() -1)
	{
		dc.MoveTo(rcItem.left, rcItem.bottom);
		dc.LineTo(rcItem.right, rcItem.bottom);
	}

	dc.SelectObject(oldPen);

	// date
#ifdef DF_NULLSTRING
	CString stmp;
	stmp.Format("%s%c", item->date, 0x00);
	dc.DrawText(stmp, CRect(3, rcItem.top, grid1 - 2, rcItem.bottom),
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// source
	stmp.Format("%s%c", item->source, 0x00);
	dc.DrawText(stmp, CRect(grid1 + 3, rcItem.top, grid2 - 2, rcItem.bottom),
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// title
	stmp.Format("%s%c", item->title, 0x00);
	dc.DrawText(stmp, CRect(grid2 + 3, rcItem.top, rcItem.right - 2, rcItem.bottom),
		DT_SINGLELINE | DT_LEFT | DT_VCENTER);
#else
	dc.DrawText(item->date, CRect(3, rcItem.top, grid1 - 2, rcItem.bottom), 
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// source
	dc.DrawText(item->source, CRect(grid1 + 3, rcItem.top, grid2 - 2, rcItem.bottom),
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	// title
	dc.DrawText(item->title, CRect(grid2 + 3, rcItem.top, rcItem.right - 2, rcItem.bottom),
		DT_SINGLELINE | DT_LEFT | DT_VCENTER);
#endif

	dc.Detach();
}

void NewsListBox::OnDestroy() 
{
	CListBox::OnDestroy();
	
	DeleteAll();	
}
