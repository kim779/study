// SubList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_codectrl.h"
#include "SubList.h"
#include "Controlwnd.h"

// CSubList
#define nDEL 12
IMPLEMENT_DYNAMIC(CSubList, CListBox)

CSubList::CSubList(class CControlwnd *pCodeCtrl, class CCodeCombo *pCodeCombo) 
{ 
	m_pCodeCtrl = pCodeCtrl; 
	m_pCodeCombo = pCodeCombo;

	m_pFont = NULL;
	m_tRGB = RGB(0, 0, 0);
	m_pRGB = RGB(255, 255, 255);
	m_bRGB = RGB(128, 128, 128);
	m_height = 14;	
}

CSubList::CSubList()
{

}

CSubList::~CSubList()
{
}


BEGIN_MESSAGE_MAP(CSubList, CListBox)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CSubList::SetListItems(CFont *pFont, int height)
{
	m_pFont = pFont;
	m_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_height = height;
}

// CSubList 메시지 처리기입니다.




void CSubList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CDC dc;
	
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;
	
	CFont* font = dc.SelectObject(m_pFont);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.SetTextColor(m_tRGB);
		dc.SetBkColor(m_pRGB);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_pRGB);
	}
	
	if (lpDrawItemStruct->itemID != -1)
	{
		CString text;
		CRect rc;
		
		rc = lpDrawItemStruct->rcItem;
		text = m_pCodeCombo->GetText(lpDrawItemStruct->itemID);
		
		dc.SetBkMode(TRANSPARENT);
		rc.left += 3;
		text.Replace("&", "&&");
		dc.DrawText(text, rc, DT_VCENTER|DT_SINGLELINE); 
		
		if (m_pCodeCombo->IsHistoryMode() && (lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			/*
			CPen* ppen;
			
			rc.left   = rc.right - m_height;
			rc.right  = rc.left + nDEL;
			rc.top   += (rc.Height() - nDEL)/2;
			rc.bottom = rc.top + nDEL;
			dc.FillSolidRect(rc, m_bRGB);
			ppen = dc.SelectObject(&m_pen);
			
			dc.MoveTo(rc.left, rc.top);
			dc.LineTo(rc.right, rc.bottom);
			dc.MoveTo(rc.right, rc.top);
			dc.LineTo(rc.left, rc.bottom);
			
			dc.SelectObject(ppen);
			*/
		}
	}
	
	dc.SelectObject(font);
	dc.Detach();
}


void CSubList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->CtlType == ODT_LISTBOX)
		lpMeasureItemStruct->itemHeight = m_height;	
}


LRESULT CSubList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (m_pCodeCombo->IsHistoryMode())
		{
			CPoint pt = CPoint(LOWORD(lParam), HIWORD(lParam));
			int top = GetTopIndex();

			CRect	rc;
			GetClientRect(rc);
			int index = (pt.y - rc.top) / (m_height);

			bool	bRemove = false;
			int	left   = rc.right - m_height;
			int	right  = left + nDEL;
			if (left <= pt.x && pt.x <= right)
				bRemove = true;
			
			if (bRemove)
			{
				DeleteString(index);
				m_pCodeCombo->RemoveString(index);
				Invalidate();
				
				return TRUE;
			}
		}
		break;
	case WM_LBUTTONUP:
	//	GetParent()->GetParent()->SendMessage(WM_LBUTTONUP, 0,0);
		m_pCodeCtrl->LButtonUp();
		break;
	}
	return CListBox::WindowProc(message, wParam, lParam);
}


int CSubList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
