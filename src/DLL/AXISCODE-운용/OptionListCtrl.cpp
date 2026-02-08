// OptionListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "OptionListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionListCtrl

COptionListCtrl::COptionListCtrl()
{
}

COptionListCtrl::~COptionListCtrl()
{
}


BEGIN_MESSAGE_MAP(COptionListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(COptionListCtrl)
	ON_WM_CREATE()	
	ON_WM_MEASUREITEM_REFLECT()	
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)	
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionListCtrl message handlers
LRESULT COptionListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
    CRect rc;
	GetWindowRect(&rc);

	WINDOWPOS wp;
	wp.hwnd = this->m_hWnd;
	wp.cx = rc.Width() ;
	wp.cy = rc.Height() ;
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

	const LRESULT lrReturn(Default());

	SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));

	return lrReturn;
}


void COptionListCtrl::MakeImageList( void )
{
	CFont font1;
	font1.CreatePointFont( 190,"±¼¸²Ã¼" );	
	SetFont(&font1);

	m_header.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() );
	
	CBitmap bm,bm1;
	
	m_imageList.Create(IDB_CHECK, 16,1,RGB(255,255,255));	
	
	SetImageList( &m_imageList, LVSIL_SMALL );

	CFont font;
	font.CreatePointFont( 300,"±¼¸²Ã¼" );	
	m_header.SetFont( &font );		
}


int COptionListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void COptionListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	const NMITEMACTIVATE* nm = reinterpret_cast<NMITEMACTIVATE*>( pNMHDR );

	CString str = GetItemText( nm->iItem, nm->iSubItem );
	if( str != ""  )
	{
		if( str.GetLength() > 7 )
			GetParent()->PostMessage( WM_SELECT_OPTION, nm->iItem, nm->iSubItem );
	}
	
	*pResult = 0;
}

void COptionListCtrl::DrawItem(LPDRAWITEMSTRUCT lp )
{
	const int iRow = GetItemCount();
	const int iCol = GetHeaderCtrl()->GetItemCount();

	CDC* pDC = CDC::FromHandle(lp->hDC);

	for( int i = 0; i < iRow; i++ )
	{
		for( int j = 0; j < iCol; j++ )
		{
			CRect rcItem;
			CString str;

			GetSubItemRect( i, j, LVIR_BOUNDS, rcItem );
			str = GetItemText( i, j );

			pDC->SetBkMode( TRANSPARENT );
			
			if( j < iCol/2 ) 
			{
				pDC->FillRect( &rcItem, &CBrush(RGB(239,217,204)) );
				if( str != "" )
				{
					m_imageList.Draw( pDC, 0, CPoint( rcItem.left+5, rcItem.top+5) , ILD_TRANSPARENT );
				}
			}
			else if( j == iCol/2 )
			{
				pDC->FillRect( &rcItem, &CBrush(RGB(214,215,214)) );
				pDC->DrawText( str, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			}
			else
			{
				pDC->FillRect( &rcItem, &CBrush(RGB(210,227,240)) );
				if( str != "" )
				{
					m_imageList.Draw( pDC, 0, CPoint( rcItem.left+5, rcItem.top+5), ILD_TRANSPARENT );
				}
			}

			pDC->FrameRect( &rcItem, &CBrush(RGB(0,0,0)) );
			
		}
	}
}

int COptionListCtrl::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int type, int nFormat, int nWidth, int nSubItem )
{
	const int i = CListCtrl::InsertColumn( nCol, lpszColumnHeading, nFormat, nWidth, nSubItem );
	
	if( i < 0 )
		return i;
	
	HDITEM info;
	info.mask = HDI_FORMAT|HDI_HEIGHT;
	info.lParam = type;

	if( nWidth == -1 )
		info.cxy = 30;
	else
		info.cxy = nWidth;
	
	if( nFormat == LVCFMT_RIGHT )
		info.fmt = HDF_OWNERDRAW|HDF_RIGHT;
	else
		info.fmt = HDF_OWNERDRAW;
	
	GetHeaderCtrl()->SetItem( i, &info );
	
	return i;
}

BOOL COptionListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_header.SubclassWindow( m_hWnd );
	
	return CListCtrl::PreCreateWindow(cs);
}

void COptionListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
    lpMeasureItemStruct->itemHeight = 20;     
}


void COptionListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
