// GroupHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CX_GROUPLIST.h"
#include "GroupHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupHeaderCtrl

CGroupHeaderCtrl::CGroupHeaderCtrl()
{
	m_font.CreatePointFont(90, "±¼¸²Ã¼");
}

CGroupHeaderCtrl::~CGroupHeaderCtrl()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CGroupHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CGroupHeaderCtrl)
	ON_WM_DRAWITEM()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupHeaderCtrl message handlers

void CGroupHeaderCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CHeaderCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGroupHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lp)
{
	CDC*	pDC = CDC::FromHandle(lp->hDC);
	pDC->FillRect(&lp->rcItem, &CBrush(RGB(230, 239, 247)));
	if (lp->itemID >= 0)
	{
		CString str = GetItemText(lp->itemID);
		CFont* oldfont = pDC->SelectObject(&m_font);

		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(str, &lp->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SelectObject(oldfont);
	}
}

CString CGroupHeaderCtrl::GetItemText(int nCol)
{
	char	szBuf[256]{};
	HDITEM info{};

	info.mask = HDI_TEXT;	
	info.pszText = szBuf;
	info.cchTextMax = sizeof(szBuf);

	if (!GetItem(nCol, &info))
		return "";

	return CString(szBuf);
}