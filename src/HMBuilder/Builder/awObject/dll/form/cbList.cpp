// cbList.cpp : implementation file
//

#include "stdafx.h"
#include "cbList.h"
#include "ambase.h"
#include "amform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CcbList

CcbList::CcbList(CamBase* form, CStringArray& items, CFont* font)
{
	m_form = form;
	m_font = font;
	m_items.Copy(items);
}

CcbList::~CcbList()
{
}


BEGIN_MESSAGE_MAP(CcbList, CListBox)
	//{{AFX_MSG_MAP(CcbList)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcbList message handlers

BOOL CcbList::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_BORDER|WS_VSCROLL;
	return CListBox::PreCreateWindow(cs);
}

int CcbList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_items.GetSize() <= 0)
		return -1;

	SetFont(m_font);

	for (int ii = 0; ii < m_items.GetSize(); ii++)
	AddString(m_items.GetAt(ii));
	SetCurSel(0);
	SetCaretIndex(0);
	SetFocus();

	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	return 0;
}

void CcbList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case VK_RETURN:
		m_form->DoListBox(true);
		break;
	case VK_ESCAPE:
		m_form->DoListBox(false);
		break;
	}
}

void CcbList::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
	m_form->DoListBox(false);
}

void CcbList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonDown(nFlags, point);

	BOOL	out;
	ItemFromPoint(point, out);
	m_form->DoListBox(out ? false : true);
}

void CcbList::OnMouseMove(UINT nFlags, CPoint point) 
{
	CListBox::OnMouseMove(nFlags, point);

	BOOL	out;
	int	idx = ItemFromPoint(point, out);
	if (!out && idx != GetCurSel())
	{
		SetCurSel(idx);
		SetCaretIndex(idx);
	}
}

void CcbList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	result;

//	CListBox::OnChar(nChar, nRepCnt, nFlags);
	m_form->UpdateData(nChar, false, result);
}


void CcbList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  지정된 항목의 크기를 확인하는 코드를 추가합니다.
	ASSERT(lpMeasureItemStruct->CtlType == 2);

	CString strText(_T(""));
	GetText(lpMeasureItemStruct->itemID, strText);
	ASSERT(TRUE != strText.IsEmpty());

	
	CRect rect;
	GetItemRect(lpMeasureItemStruct->itemID, &rect);
	
	CDC* pDC = GetDC(); 
	lpMeasureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, DT_WORDBREAK | DT_CALCRECT); 
	ReleaseDC(pDC);
}


void CcbList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{	
	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
//	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
//	LISTBOX_COLOR* pListBox = (LISTBOX_COLOR*)GetItemDataPtr(lpDrawItemStruct->itemID);
//	ASSERT(NULL != pListBox);
	CDC dc;

	CString strText;
	GetText(lpDrawItemStruct->itemID, strText);
	
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(RGB(0, 0, 0));
//		dc.SetBkColor(RGB(0, 0, 255));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_selbk);
	}
	else
	{
		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetBkColor(RGB(255, 255, 255));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255));
	}
	
	lpDrawItemStruct->rcItem.left += 5;
	// Draw the text.	
	dc.DrawText(strText, strText.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK);
	
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach();	
}

/*
void CcbList::AppendString(LPCSTR lpszText, COLORREF fgColor, COLORREF bgColor)
{
	LISTBOX_COLOR* pInfo = new LISTBOX_COLOR;

	pInfo->strText.Format(_T("%s"), lpszText);
	pInfo->fgColor = fgColor; 
	pInfo->bgColor = bgColor;

	SetItemDataPtr(AddString(pInfo->strText), pInfo);
}
*/