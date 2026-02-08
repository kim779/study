// BackgroundDrawer.cpp: implementation of the CBackgroundDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BackgroundDrawer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackgroundDrawer::CBackgroundDrawer()
{
}

CBackgroundDrawer::~CBackgroundDrawer()
{

}

// public ===============================================================================
// background 와 backgroundFrame를 그린다 
void CBackgroundDrawer::DrawBackground(CDC* pDC,
					CRect& backgroundRegion,		// 그릴 background 영역 
					const int frameThickness,		// frame 굵기
					COLORREF backgroundColor,		// background 색
					COLORREF backFrameColor,		// Frame 색
					const int frameType)			// frame 타입
{
	CBrush* pOldBrush;
	CPen* pOldPen; 
	
	CBrush backgroundBrush(backgroundColor);
	pOldBrush = (CBrush*)pDC->SelectObject(&backgroundBrush);

	CPen backgroundPen;
	if(frameType==3)
		backgroundPen.CreatePen(PS_INSIDEFRAME, 0, backFrameColor);
	else
		backgroundPen.CreatePen(PS_INSIDEFRAME, frameThickness, backFrameColor);

	pOldPen = (CPen*)pDC->SelectObject(&backgroundPen);

	switch(frameType){
	case 0:
		pDC->Rectangle(backgroundRegion);
		break ;
	case 1:
		pDC->Rectangle(backgroundRegion);
		// (2006/10/8 - Seung-Won, Bae) Use 3D Frame with Frame Color
		// pDC->DrawEdge(backgroundRegion, EDGE_SUNKEN, BF_RECT);
		pDC->Draw3dRect( backgroundRegion,
			RGB( GetRValue( backgroundColor) / 2, GetGValue( backgroundColor) / 2, GetBValue( backgroundColor) / 2),
			RGB( ( GetRValue( backgroundColor) + 255) / 2, ( GetGValue( backgroundColor) + 255) / 2, ( GetBValue( backgroundColor) + 255) / 2));
		break ;
	case 2:
		pDC->Rectangle(backgroundRegion);
		// (2006/10/8 - Seung-Won, Bae) Use 3D Frame with Frame Color
		// pDC->DrawEdge(backgroundRegion, EDGE_RAISED, BF_RECT);
		pDC->Draw3dRect( backgroundRegion,
			RGB( ( GetRValue( backgroundColor) + 255) / 2, ( GetGValue( backgroundColor) + 255) / 2, ( GetBValue( backgroundColor) + 255) / 2),
			RGB( GetRValue( backgroundColor) / 2, GetGValue( backgroundColor) / 2, GetBValue( backgroundColor) / 2));
		break ;
	case 3:
		pDC->FillRect(backgroundRegion, &backgroundBrush);
		break ;
	default:
		break ;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	backgroundBrush.DeleteObject();
	backgroundPen.DeleteObject();	
}