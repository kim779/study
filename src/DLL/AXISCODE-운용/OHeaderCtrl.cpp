//  OHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "OHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COHeaderCtrl

COHeaderCtrl::COHeaderCtrl()
{
	m_Font.CreateFont( 12, 0, 0, 0, FW_NORMAL,
				FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
				"奔覆眉" );
}

COHeaderCtrl::~COHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(COHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(COHeaderCtrl)
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
	ON_WM_MEASUREITEM()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COHeaderCtrl message handlers

void COHeaderCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void COHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lp )
{
}

void COHeaderCtrl::DrawUpColumn(  int iStart, int iCol, int type, CDC* pDC )
{
	CRect rc1, rc2;
	GetItemRect( iStart, &rc1 );	
	GetItemRect( iCol, &rc2 );	

	CRect rc;

	rc.left = rc1.left;
	rc.top = rc1.top;
	rc.right = rc2.right;
	rc.bottom = rc1.Height()/2;	

	CString str;
	if( type == HEADER_PUT )
		str = "遣可记";
	else if( type == HEADER_CALL )
		str = "妮可记";
	
	pDC->FillRect( &rc, &CBrush(RGB(112,112,112)) );
	pDC->DrawText( str, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	pDC->FrameRect( &rc, &CBrush(RGB(0,0,0)) );
}

int COHeaderCtrl::GetColumnType( int iCol )
{
	HDITEM info;
	info.mask = HDI_LPARAM;
	if( !GetItem( iCol, &info ) )
		return -1;

	return (int)info.lParam;
}

void COHeaderCtrl::DrawColumn( int iCol, CDC* pDC )
{
	CRect rc;
	GetItemRect( iCol, &rc );

	HDITEM info;
	info.mask = HDI_LPARAM|HDI_TEXT|HDI_WIDTH;
	GetItem( iCol, &info );

	CString str = info.pszText;
	const int type = (int)info.lParam;

	CRect rcUp,rcDown, rcDraw;
	
	rcUp = rc;
	rcUp.bottom = rc.Height()/2;
	
	rcDown = rc;
	rcDown.top = rc.Height()/2;

	if( type == HEADER_CALL || type == HEADER_PUT )
	{		
		rcDraw = rcDown;		
	}
	else if( type == HEADER_NORMAL )
	{
		rcDraw = rc;		
	}

	pDC->FillRect( &rcDraw, &CBrush(RGB(112,112,112)) );
	pDC->DrawText( str, rcDraw, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	pDC->FrameRect( &rcDraw, &CBrush(RGB(0,0,0)) );	
}

int COHeaderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHeaderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void COHeaderCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL COHeaderCtrl::Layout(HDLAYOUT* pHeaderLayout)
{
	/*CHeaderCtrl::Layout( pHeaderLayout );*/

	pHeaderLayout->pwpos->x = 0;
	pHeaderLayout->pwpos->y = 0;
	pHeaderLayout->pwpos->cx = pHeaderLayout->prc->right - pHeaderLayout->prc->left;
	pHeaderLayout->pwpos->cy = 30;

	return TRUE;
}

void COHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect( &rc );	

	dc.FillRect( &rc, &CBrush(RGB(214,215,214)) );
	dc.FrameRect( &rc, &CBrush(RGB(0,0,0)) );

	char szBuf[MAX_PATH]{};

	HDITEM info;
	info.mask = HDI_TEXT;
	info.pszText = szBuf;
	info.cchTextMax = MAX_PATH;

	const int iHeight = rc.Height()/2;
	int iLeft = 0;
	int iRight = 0;

	dc.SelectObject( &m_Font );

	const int iCount = GetItemCount();
	for( int i = 0; i < iCount; i++ )
	{
		CRect rcItem;
		CString str;

		GetItemRect( i, &rcItem );
		rcItem.top = iHeight;

		GetItem( i, &info );	

		if( CString(info.pszText) == "青荤啊拜" )		
			rcItem.top = 0;			
		
		dc.FrameRect( &rcItem, &CBrush(RGB(0,0,0)) );	
		
		iRight += rcItem.Width();

		if( i == (iCount/2)-1 )
		{
			dc.MoveTo( rcItem.right-1, 0 );
			dc.LineTo( rcItem.right-1, iHeight );

			CRect rcText;
			rcText.SetRect( iLeft, 0, iRight, iHeight );
			dc.DrawText( "妮可记", &rcText, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		}
		else if( i == iCount/2 )
		{
			iLeft = iRight;
			iRight = 0;
		}
		else if( i == (iCount/2)+1 ) 
		{
			dc.MoveTo( rcItem.left, 0 );
			dc.LineTo( rcItem.left, iHeight );		
		}
		else if( i == iCount-1 )
		{
			dc.MoveTo( rcItem.right-1, 0 );
			dc.LineTo( rcItem.right-1, iHeight );

			CRect rcText;
			rcText.SetRect( iLeft, 0, iLeft+iRight, iHeight );
			dc.DrawText( "遣可记", &rcText, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		}
		

		dc.SetBkMode( TRANSPARENT );
		dc.DrawText( info.pszText, &rcItem, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}		
}
