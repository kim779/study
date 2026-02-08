// iclncom.cpp : implementation file
//

#include "stdafx.h"
#include "iclncom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CiLineComboBox

CiLineComboBox::CiLineComboBox()
{
}

CiLineComboBox::~CiLineComboBox()
{
}


BEGIN_MESSAGE_MAP(CiLineComboBox, CComboBox)
	//{{AFX_MSG_MAP(CiLineComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CiLineComboBox message handlers

#define LINE_ITEM_HEIGHT   15

void CiLineComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// all items are of fixed size
	// must use LBS_OWNERDRAWVARIABLE for this to work
	lpMIS->itemHeight = LINE_ITEM_HEIGHT;
}

void CiLineComboBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		if ((long int)lpDIS->itemData == -1) return;
		CString str((LPCTSTR)lpDIS->itemData); 
		int Index = (int)atof(str);

		if (Index == 0 && str[0] != '0') return;

		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOld = memDC.SelectObject(&m_bitmap);

		CRect rect;
		rect.CopyRect(&lpDIS->rcItem);
//		int top = LINE_ITEM_HEIGHT;
		pDC->BitBlt(rect.left, rect.top, rect.Width(), LINE_ITEM_HEIGHT,
			&memDC, 0, LINE_ITEM_HEIGHT*Index, SRCCOPY);
		memDC.SelectObject(pOld);
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		CBrush br(RGB(192,0,192));
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(RGB(255,255,255));
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}
}

void CiLineComboBox::OnDropdown() 
{
	// TODO: Add your control notification handler code here
	
}
