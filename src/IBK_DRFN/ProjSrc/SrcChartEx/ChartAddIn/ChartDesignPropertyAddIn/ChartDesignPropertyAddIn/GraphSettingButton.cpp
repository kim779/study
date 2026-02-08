// GraphSettingButton.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "GraphSettingButton.h"

#include "DlgGraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphSettingButton

CGraphSettingButton::CGraphSettingButton(int nIndex, CWnd* pDlgChartWnd)
{
	m_nIndex = nIndex;
	m_pDlgGraphSetting = (CDlgGraphSetting* )pDlgChartWnd;
}

CGraphSettingButton::~CGraphSettingButton()
{
}


BEGIN_MESSAGE_MAP(CGraphSettingButton, CButton)
	//{{AFX_MSG_MAP(CGraphSettingButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphSettingButton message handlers

void CGraphSettingButton::OnClicked() 
{
	SetState(TRUE);
	m_pDlgGraphSetting->ButtonClicked(m_nIndex);
}
/*
void CGraphSettingButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CString str;
	str.Format("%s%d", "tab", m_nIndex+1);
	
	COLORREF cr = RGB(192,192,192);
    // Paint the color item in the color requested
	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) {
      // Paint the color item in the color requested
      CBrush br(cr);
      pDC->FillRect(&lpDrawItemStruct->rcItem, &br);
   }

   if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
      (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
      // item has been selected - hilite frame
      COLORREF crHilite = RGB(255-GetRValue(cr),
                  255-GetGValue(cr), 255-GetBValue(cr));
      CBrush br(crHilite);
      pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
   }

   if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
      (lpDrawItemStruct->itemAction & ODA_SELECT)) { 
      // Item has been de-selected -- remove frame
      CBrush br(cr);
      pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
   }	
   pDC->SetBkMode(TRANSPARENT);
   pDC->TextOut(lpDrawItemStruct->rcItem.left+3,lpDrawItemStruct->rcItem.top, str);
}
*/
int CGraphSettingButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* font = GetParent()->GetFont();	
	SetFont(font);

	return 0;
}
