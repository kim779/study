// HeaderCtrlEx1.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderCtrlExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NUM_COLOR 10
////color
static COLORREF _gszColor2[NUM_COLOR] =
{
	RGB(255,0,0), RGB(0,255,0),RGB(0,0,255),RGB(255,255,0),RGB(0,255,255),
	RGB(255,0,255),RGB(200,200,50),RGB(50,200,200),RGB(200,50,100),RGB(250,190,250)
};

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers

void CHeaderCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);

	HDITEM hdi;
	TCHAR  lpBuffer[256];

	hdi.mask = HDI_TEXT;	
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;


	GetItem(lpDrawItemStruct->itemID, &hdi);

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//THIS FONT IS ONLY FOR DRAWING AS LONG AS WE DON'T DO A SetFont(...)
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, 
	&lpDrawItemStruct->rcItem, DFC_CAPTION/*DFC_BUTTON*/, DFCS_FLAT);// | DFCS_BUTTONPUSH);

//   ChangeColumnColor();         //color를 변경하고 싶을때..........

	pDC->FillSolidRect(&lpDrawItemStruct->rcItem, RGB(0, 0, 128));
	pDC->SetBkColor(RGB(0, 0, 128));
	pDC->SetTextColor(RGB(255, 255, 255));

/*   switch(lpDrawItemStruct->itemID)
   {
   case 0:
	   {
		   pDC->FillSolidRect(&lpDrawItemStruct->rcItem, _gszColor2[9]);
		   pDC->SetBkColor(_gszColor2[9]);
		   pDC->SetTextColor(_gszColor2[8]);
	   }
	   break;
   case 1:
	   {
		   pDC->FillSolidRect(&lpDrawItemStruct->rcItem, _gszColor2[0]);
		   pDC->SetBkColor(_gszColor2[0]);
		   pDC->SetTextColor(_gszColor2[10]);
	   }
	   break;
   case 2:
	   {
		   pDC->FillSolidRect(&lpDrawItemStruct->rcItem, _gszColor2[2]);
		   pDC->SetBkColor(_gszColor2[2]);
		   pDC->SetTextColor(_gszColor2[9]);
	   }
	   break;
   case 3:
	   {
		   pDC->FillSolidRect(&lpDrawItemStruct->rcItem, _gszColor2[4]);
		   pDC->SetBkColor(_gszColor2[4]);
		   pDC->SetTextColor(_gszColor2[5]);
	   }
	   break;
   }
*/ 
	UINT uFormat = DT_CENTER;                                                                                      
	//DRAW THE TEXT
	::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
	&lpDrawItemStruct->rcItem, uFormat);

	pDC->SelectStockObject(SYSTEM_FONT);
}

void CHeaderCtrlEx::ChangeColumnColor()
{
	//_gszColor2[NUM_COLOR]변경.......
	_gszColor2[8] = RGB(0,0,255);
}
