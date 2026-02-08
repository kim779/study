// SkinListCtrl2.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "SkinListCtrl2.h"
#include "CommodityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl2

CSkinListCtrl2::CSkinListCtrl2()
{
	m_brFrame.CreateSolidBrush( RGB(0,0,0) );
	m_brColumn.CreateSolidBrush( RGB(225,225,225) );
	m_brBack.CreateSolidBrush( RGB(255,255,255) );
	m_brSelected.CreateSolidBrush( RGB(0,0,255) );
	m_brColumnSelected.CreateSolidBrush( RGB(191,191,255) );

	m_clrSelect = RGB(255,255,255);
	m_clrNormal = RGB(0,0,0);	

	m_mapYear.RemoveAll();
}

CSkinListCtrl2::~CSkinListCtrl2()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl2, CListCtrl)
	//{{AFX_MSG_MAP(CSkinListCtrl2)
	ON_WM_CREATE()
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl2 message handlers
int CSkinListCtrl2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here	
	//SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_OWNERDRAWFIXED);	
	return 0;
}

void CSkinListCtrl2::DrawItem(LPDRAWITEMSTRUCT lp )
{	
	const int iItem = lp->itemID;
	const int iColCount = GetHeaderCtrl()->GetItemCount();

	CDC* pDC = CDC::FromHandle(lp->hDC);
	

	for( int i = 0; i < iColCount; i++ )
	{
		DrawSubItem( pDC, iItem, i );
	}
}

BOOL CSkinListCtrl2::IsSelected( int nItem, int nSub )
{
	if( m_ptCurrent.x == nSub && m_ptCurrent.y == nItem )
		return TRUE;
	return FALSE;
}

BOOL CSkinListCtrl2::IsSelectedRow( int nItem )
{
	if( m_ptCurrent.y == nItem )
		return TRUE;
	return FALSE;
}

void CSkinListCtrl2::DrawSubItem(CDC* pDC, int nItem, int nSubItem )
{
	CRect rc;

	if( !GetSubItemRect( nItem, nSubItem, LVIR_BOUNDS, rc ) )
		return;

	const BOOL bSelect = IsSelected( nItem, nSubItem );

	CString strText = GetItemText( nItem, nSubItem );

	if( nSubItem == 0 )
	{	
		if( IsSelectedRow( nItem ) )
		{
			pDC->SetTextColor( m_clrNormal );
			pDC->FillRect( &rc, &m_brColumnSelected );			
		}
		else
		{
			pDC->SetTextColor( m_clrNormal );
			pDC->FillRect( &rc, &m_brColumn );			
		}

		
		pDC->DrawText( strText, &rc, DT_SINGLELINE|DT_LEFT|DT_VCENTER );				
	}
	else 
	{
		if( bSelect )
		{
			pDC->SetTextColor( m_clrSelect );
			pDC->FillRect( &rc, &m_brSelected );		
		}
		else
		{
			pDC->SetTextColor( m_clrNormal );
			pDC->FillRect( &rc, &m_brBack );		
		}

		
		pDC->DrawText( strText, &rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER );
	}

	
	pDC->FrameRect( &rc, &m_brFrame );	
}

void CSkinListCtrl2::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight += 20;
}


void CSkinListCtrl2::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	const NMITEMACTIVATE* nm = reinterpret_cast<NMITEMACTIVATE*>( pNMHDR );

	m_ptCurrent.x = nm->iSubItem;
	m_ptCurrent.y = nm->iItem;

	Invalidate( FALSE );

	GetParent()->PostMessage( WM_CLICK );
	
	*pResult = 0;
}

BOOL CSkinListCtrl2::DeleteAllItems()
{
	m_ptCurrent.x = -1;
	m_ptCurrent.y = -1;

	return CListCtrl::DeleteAllItems();
}

void CSkinListCtrl2::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	const NMITEMACTIVATE* nm = reinterpret_cast<NMITEMACTIVATE*>( pNMHDR );

	m_ptCurrent.x = nm->iSubItem;
	m_ptCurrent.y = nm->iItem;

	Invalidate( FALSE );

	GetParent()->PostMessage( WM_BLCLICK );
	
	*pResult = 0;
}

void CSkinListCtrl2::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

    // Activate the standard CWnd tooltip functionality
    VERIFY( EnableToolTips(TRUE) );
}

int CSkinListCtrl2::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
{
	int             row, col;
	RECT    cellrect;
	row = CellRectFromPoint(point, &cellrect, &col );

	if ( row == -1 )        return -1;
	
	pTI->hwnd               = m_hWnd;
	pTI->uId                = (UINT)((row<<10)+(col&0x3ff)+1);
	pTI->lpszText   = LPSTR_TEXTCALLBACK;
	pTI->rect               = cellrect;

	CString slog;
	slog.Format("[axiscode] hwnd=[%x]  row=[%d] col=[%d] uId=[%d]", pTI->hwnd, row, col, pTI->uId);
	OutputDebugString(slog);

	return  pTI->uId;
}

int CSkinListCtrl2::CellRectFromPoint(const CPoint &point, RECT *cellrect, int *col) const
{
	int colnum{};
	
	// 리스트뷰가 LVS_REPORT 모드에있는지 확인
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT ) 
		return -1;
	
	
	// 현재 화면에 보이는처음과 끝 Row 를 알아내기
	int row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )   bottom = GetItemCount();
    
	// 컬럼갯수 알아내기
	const CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	const int nColumnCount = pHeader->GetItemCount();
	
	
	// 현재보이는 Row 들간에 루프 돌기
	for( ;row <=bottom;row++)               {
		// 아이템의 경계 사각형을 가져오고, 점이 포함되는지 체크        
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )              {
			// 컬럼찾기
			for( colnum = 0; colnum < nColumnCount; colnum++ )  {
				const int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left && point.x <= (rect.left + colwidth) ) {
					RECT rectClient;
					GetClientRect( &rectClient );
					if( col ) *col = colnum;
					rect.right = rect.left + colwidth;
					
					// 오른쪽 끝이 클라이언트 영역을 벗어나지 않도록 확인
					if( rect.right > rectClient.right )             
						rect.right = rectClient.right;
					*cellrect = rect;
					
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

BOOL CSkinListCtrl2::OnToolTipText( UINT, NMHDR* pNMHDR, LRESULT* pResult )
{
	// ANSI 와 UNICODE 양쪽버젼의 메시지를 처리해야함
	TOOLTIPTEXTA*   pTTTA   = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW*   pTTTW   = (TOOLTIPTEXTW*)pNMHDR;
	CString         strTipText;
	const UINT    nID = pNMHDR->idFrom;

	if( nID == 0 )                  // NT 에서의 자동생성 툴팁으로부터의 통지
		return FALSE;       // 그냥 빠져나간다.
	
	const int row     = ((nID-1) >> 10) & 0x3fffff ;
	const int col     = (nID-1) & 0x3ff;
	
	if (row < 0 || row > 3 || col <= 0 || (col != 1 && col != 2))
	{
		CString slog;
		slog.Format("[axiscode]row=[%d] col=[%d] nID=[%d]",  row, col, nID);
		OutputDebugString(slog);
		return FALSE;
	}
	
	strTipText = GetItemText( row, col );

	LVITEM item;
	item.mask = LVIF_TEXT | LVIF_STATE;
	item.iItem = row;
	item.iSubItem = col;

	GetItem(&item);

	//AfxMessageBox(item.pszText);
	CString strTip;
	
	//strTip.Format("%s년 %s", strYear ,strTipText);
	CString strKey;
	strKey.Format("%d%d", row, col);
	m_mapYear.Lookup(strKey, strTip);
	
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)      
		lstrcpyn(pTTTA->szText, strTip, 80);
	else                                                                            
		_mbstowcsz(pTTTW->szText, strTip, 80);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)      
		_wcstombsz(pTTTA->szText, strTip, 80);
	else                                                                            
		lstrcpyn(pTTTW->szText, strTip, 80);
#endif
	*pResult = 0;
	return TRUE;            // 메시지가 처리되었다.
}
