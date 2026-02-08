// BitmapItemComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "BitmapItemComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define LINE_ITEM_HEIGHT   15


/////////////////////////////////////////////////////////////////////////////
// CBitmapItemComboBox

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CBitmapItemComboBox::CBitmapItemComboBox()
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CBitmapItemComboBox::~CBitmapItemComboBox()
{
	
}


BEGIN_MESSAGE_MAP(CBitmapItemComboBox, CComboBox)
	//{{AFX_MSG_MAP(CBitmapItemComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapItemComboBox message handlers



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Param  LPDRAWITEMSTRUCT lpDrawItemStruct : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBitmapItemComboBox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct ) 
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	
	if( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
	{
		if( ( long int )lpDrawItemStruct->itemData == -1 )
			return;
		
		CString str( ( LPCTSTR )lpDrawItemStruct->itemData ); 
		int Index = ( int )atof( str );

		if( Index == 0 && str[0] != '0' )
			return;

		CDC memDC;
		memDC.CreateCompatibleDC( pDC );
		CBitmap *pOld = memDC.SelectObject( &m_bitmap );

		CRect rect;
		rect.CopyRect( &lpDrawItemStruct->rcItem );
		pDC->BitBlt( rect.left, rect.top, rect.Width(), LINE_ITEM_HEIGHT,
			&memDC, 0, LINE_ITEM_HEIGHT * Index, SRCCOPY );
		memDC.SelectObject( pOld );
	}

	// Item이 선택 되었을때
	if( ( lpDrawItemStruct->itemState & ODS_SELECTED ) 
		&& ( lpDrawItemStruct->itemAction & ( ODA_SELECT | ODA_DRAWENTIRE ) ) )
	{
		CBrush br( RGB( 192, 0, 192 ) );
		pDC->FrameRect( &lpDrawItemStruct->rcItem, &br );
	}

	// Item이 선택 해제되었을때
	if( !( lpDrawItemStruct->itemState & ODS_SELECTED ) && ( lpDrawItemStruct->itemAction & ODA_SELECT ) )
	{
		CBrush br( RGB( 255,255,255 ) );
		pDC->FrameRect( &lpDrawItemStruct->rcItem, &br );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Param  LPMEASUREITEMSTRUCT lpMeasureItemStruct : 
// Comments		: 
//-----------------------------------------------------------------------------
void CBitmapItemComboBox::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct ) 
{
	lpMeasureItemStruct->itemHeight = LINE_ITEM_HEIGHT;	
}