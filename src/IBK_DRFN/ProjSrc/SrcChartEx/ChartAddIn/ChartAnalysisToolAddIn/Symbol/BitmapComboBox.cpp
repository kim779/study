// BitmapComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static char* szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
	_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
	_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
	_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
	_T("24")
};
///////////////////////////////////////////////////////////////////////////////
// CBitmapComboBox

CBitmapComboBox::CBitmapComboBox():
	m_nCount(0)
{
}

BEGIN_MESSAGE_MAP(CBitmapComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBitmapComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CBitmapComboBox message handlers

void CBitmapComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(lpDrawItemStruct->itemData == -1)
		return;

	if(lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		DrawItem(CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct->rcItem, lpDrawItemStruct->itemID);
	
	// item 선택 표시
	if(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))
		DrawSelectedItem(CDC::FromHandle(lpDrawItemStruct->hDC), lpDrawItemStruct->rcItem, (lpDrawItemStruct->itemState & ODS_SELECTED));
}

void CBitmapComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = GetItemHight();
}

int CBitmapComboBox::GetCount() const
{
	return m_nCount;
}


///////////////////////////////////////////////////////////////////////////////
// public =====================================================================
void CBitmapComboBox::LoadBitmap(const int nBitmapResource, const unsigned int nCount)
{
	ResetContent();
	m_bitmap.LoadBitmap(nBitmapResource);
	SetCount(nCount);
	InitializeData();
}

// private ====================================================================
void CBitmapComboBox::InitializeData()
{
	int nHight = GetItemHight();
	for(int i= -1 ;i < m_nCount; i++){
		SetItemHeight(i, nHight);
	}
	
	for(int nIndex = 0; nIndex < m_nCount; nIndex++){
		AddString(szType[nIndex]);
	}

	SetCurSel(0);
}

void CBitmapComboBox::SetCount(const unsigned int nCount)
{
	if(nCount < 0)
		m_nCount = 0;
	else
		m_nCount = nCount;
}

int CBitmapComboBox::GetItemHight() const
{
	if(m_nCount <= 0)
		return 15;

	BITMAP bmInfo;
	m_bitmap.GetObject(sizeof(bmInfo), &bmInfo);
	return bmInfo.bmHeight / m_nCount;
}

// ----------------------------------------------------------------------------
void CBitmapComboBox::DrawItem(CDC* pDC, const CRect& rect, const int nItemID)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOld = memDC.SelectObject(&m_bitmap);

	int nHight = GetItemHight();
	pDC->BitBlt(rect.left, rect.top, rect.Width(), nHight, &memDC, 0, nItemID * nHight, SRCCOPY);
	memDC.SelectObject(pOld);
}

void CBitmapComboBox::DrawSelectedItem(CDC* pDC, const CRect& rect, const bool bSelected)
{
	if(bSelected)
		DrawFrame(pDC, rect, RGB(150, 150, 150));
	else
		DrawFrame(pDC, rect, RGB(255, 255, 255));
}

void CBitmapComboBox::DrawFrame(CDC* pDC, const CRect& rect, const COLORREF& color)
{
	CBrush brush(color);
	pDC->FrameRect(rect, &brush);
}