// SListCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "SListCtrl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl1

CSListCtrl1::CSListCtrl1(CMapWnd* mapwnd)
{

	m_pMapWnd = mapwnd;
	m_HdCtrl.SetParentWnd(m_pMapWnd);
	m_bSortAsc = false;
	m_clrBk1 = RGB(255,255,255);
	m_clrBk2 = RGB(244,244,244);
	m_clrText = RGB(30,30,30);
	m_clrSelectText = RGB( 30,80,244);
	m_clrSelectBk = RGB(180,180,233);
	m_clrEdge = RGB(200,200,200);
	m_nHideColIdx = 0xffff ;
	m_Font.CreatePointFont(90,"굴림");
	m_FixCol = -1;
	m_bFix = false;
	m_bIsEdit = false;
	m_bEditEnable = true;
	m_pEdit = NULL;
	m_disEditCol = -1;
	m_nClkItem = -1;
	m_nClkSubItem = -1;
	m_pTooltip = NULL;
}

CSListCtrl1::~CSListCtrl1()
{
	if( m_Font.GetSafeHandle())
		m_Font.DeleteObject();

	DeleteEdit();

}


BEGIN_MESSAGE_MAP(CSListCtrl1, CListCtrl)
	//{{AFX_MSG_MAP(CSListCtrl1)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl1 message handlers
BOOL CSListCtrl1::OnEraseBkgnd(CDC* pDC)
{
	return false;
}
void CSListCtrl1::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HdCtrl.SubclassWindow(pHeader->m_hWnd));
	}



	CListCtrl::PreSubclassWindow();
}


void CSListCtrl1::HeaderFix(bool bFix, int col)
{
	m_bFix = bFix;
	m_FixCol = col;
}
void CSListCtrl1::DisableEditColumn(int col)
{
	m_disEditCol = col;
}
int CSListCtrl1::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HdCtrl.SubclassWindow(pHeader->m_hWnd));
	}
	
	if (m_pTooltip == NULL)
	{
		m_pTooltip = std::make_unique<CTipDlg>(m_pMapWnd->m_pParent, this);
		m_pTooltip->Create(CTipDlg::IDD, NULL);			
	}


	return 0;
}
void CSListCtrl1::OnPaint() 
{
	CPaintDC pdc(this); // device context for painting

	CRect rc,rcList;
	int nH = 0,nW = 0,nItemH=0;
	COLORREF clrText{}, clrBk{};
	
	GetClientRect(&rcList);

	m_HdCtrl.Invalidate();	

	CMemDC dc(&pdc,&rcList);
	
	CRect headerRect;
	m_HdCtrl.GetWindowRect(&headerRect);
	ScreenToClient(&headerRect);
	dc.ExcludeClipRect(&headerRect);
	   
	CRect clip;
	dc.GetClipBox(&clip);
	dc.FillSolidRect(clip, m_clrBk2);

	for(int i = 0 ; i < GetItemCount() ; i++ ){
		
		GetItemRect(i,rc,LVIR_BOUNDS );
//		if( rc.top < 18 )
//			continue;

//			if( i==0 )
		nH = rc.bottom;nItemH = rc.Height();
		
		GetDrawColors(i,0,clrText,clrBk);

		// CELL 한개씩 그리기 
		for(int j = 0 ; j < m_HdCtrl.GetItemCount() ; j++ )
		{
			GetDrawColors(i,j,clrText,clrBk);
			if(j==2)
			{
				DrawTypeImage(&dc, rc);
			}


			GetSubItemRect(i,j,LVIR_BOUNDS,rc);
			if(rc.Width() <= 0 )
				continue;

			if( j== 0)
			{
				CRect rc1;
				GetSubItemRect(i,1,LVIR_BOUNDS,rc1);
				rc.right = rc1.left;	
			}
			if(i==0)
				rc.top-=2;
			//rc.bottom-=2;

			dc.FillSolidRect(rc, clrBk);
			
			if( j < m_nHideColIdx ){
				nW = rc.right; 
				CPen pen;
				pen.CreatePen(PS_SOLID,1, m_clrEdge);
				CPen* pOldPen = dc.SelectObject(&pen);
				dc.MoveTo( rc.left,rc.top );
				dc.LineTo( rc.left,rc.bottom );
				dc.SelectObject(pOldPen);
				pen.DeleteObject();

			
				dc.SetBkMode(TRANSPARENT);
				dc.SetTextColor(clrText);
	//			CFont font;font.CreatePointFont(90,"굴림");
				CFont* pOldFont = dc.SelectObject( &m_Font);
				CString str = GetItemText(i,j);
				rc.left+=2;
				rc.right-=3;
				//if(j==2)
				//	dc.DrawText(str , rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
				//else
				//	dc.DrawText(str , rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
				DrawText(i,j,&dc, clrText,clrBk,rc);

				dc.SelectObject(pOldFont);
			}			
		}
	}

}

void CSListCtrl1::DrawTypeImage(CDC *pDC, const CRect& rc)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, m_hTypeBitmap[0]); //
	
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, 16, 16, hDC, 0, 0, 16, 16, 0xFF00FF);
	::DeleteDC (hDC);
}

void CSListCtrl1::SetImageType(HBITMAP hbitmap1, HBITMAP hbitmap1_dn)
{
	m_hTypeBitmap[0] = hbitmap1;
	m_hTypeBitmap[1] = hbitmap1_dn;
}

void CSListCtrl1::DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect )
{
	ASSERT(pDC);

	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

//	pDC->FillSolidRect(&rect, crBkgnd);

	CString str;
	str = GetItemText(nItem, nSubItem);

	if (!str.IsEmpty())
	{
		// get text justification
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_HdCtrl.GetItem(nSubItem, &hditem);
		const int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE| DT_NOPREFIX;
		if (nFmt == HDF_CENTER)
			nFormat |= DT_CENTER;
		else if (nFmt == HDF_LEFT)
			nFormat |= DT_LEFT;
		else
			nFormat |= DT_RIGHT;

		CFont *pOldFont = NULL;
		CFont boldfont;

		pOldFont= pDC->SelectObject( &m_Font);
		pDC->SetBkMode(TRANSPARENT);
		CString strDB = GetItemText(nItem, 4);
		if( strDB != ""){
			
			if( nSubItem == 3 ||nSubItem == 4||nSubItem == 5 ){
				if( strDB.Left(2) == ARR_UP || strDB.Left(2)  == TRI_UP )
					pDC->SetTextColor(RGB(255,0,0));
				else if ( strDB.Left(2) == ARR_DN || strDB.Left(2)  == TRI_DN )
					pDC->SetTextColor(RGB(0,0,255));
			}else
				pDC->SetTextColor(crText);
		}else{
			pDC->SetTextColor(crText);
		}

		if( nSubItem == 9 || nSubItem == 10 ){
			if( str.GetLength() > 0 )
			{
				if( str.Left(1) == "-" )
					pDC->SetTextColor(RGB(0,0,255));
				else
				{
					str.Remove(_T('%'));
					str.TrimLeft();
					str.TrimRight();
					if( str != "0")
						pDC->SetTextColor(RGB(255,0,0));
				}
			}
		}

		if( nSubItem == 4 ){
			if( str.Left(2) == ARR_UP || str.Left(2)  == TRI_UP ||
				str.Left(2) == ARR_DN || str.Left(2)  == TRI_DN )
			{
			
				pDC->DrawText(str.Left(2), &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				pDC->DrawText(str.Mid(2), &rect, nFormat);
			}else
				pDC->DrawText(str, &rect, nFormat);
		}else{
			pDC->DrawText(str, &rect, nFormat);
		}

		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}
}
///////////////////////////////////////////////////////////////////////////////
// GetDrawColors
void CSListCtrl1::GetDrawColors(int nItem,
							   int nSubItem,
							   COLORREF& colorText,
							   COLORREF& colorBkgnd,
							   bool bSelect)
{
	const DWORD dwStyle    = GetStyle();
	const DWORD dwExStyle  = GetExtendedStyle();

	COLORREF crText  = colorText;
	COLORREF crBkgnd = colorBkgnd;

	if (GetItemState(nItem, LVIS_SELECTED) && bSelect && nSubItem < m_nHideColIdx)
	{
		crText = m_clrText;
		crBkgnd = m_clrSelectBk;
	}else{
		crBkgnd = nItem%2?m_clrBk2:m_clrBk1;
		crText = m_clrText;
	}
	
	colorText = crText;
	colorBkgnd = crBkgnd;
}

void CSListCtrl1::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;

	if(pNMListView->iItem == 0){// && pNMListView->uNewState != 0){
		GetItemRect(0,rc,LVIR_BOUNDS);
		rc.top -=2;
		InvalidateRect( rc);
	}
	
	*pResult = 0;
}


void CSListCtrl1::OnDestroy() 
{

	CListCtrl::OnDestroy();

	if (m_pTooltip)
	{
		m_pTooltip->DestroyWindow();
		m_pTooltip = nullptr;
	}
	
	// TODO: Add your message handler code here
	
}


void CSListCtrl1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("OnHScroll\n");
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSListCtrl1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	const int cPos = GetScrollPos(SB_VERT);
	const int nLimit = GetScrollLimit(SB_VERT);

	if( nSBCode==SB_ENDSCROLL && (cPos == nLimit) && cPos != 0){
//		GetParent()->SendMessage( WM_REQUEST_NEXT );
	}
//	TRACE("OnVScroll,(%d,%d,%d,%d),%d,%d\n",nSBCode ,SB_LINEDOWN,SB_ENDSCROLL,SB_BOTTOM,cPos,nLimit);
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSListCtrl1::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	const int cPos = GetScrollPos(SB_VERT);
	const int nLimit = GetScrollLimit(SB_VERT);

//	TRACE("OnMouseWheel,%d, %d,%d,%d\n",nFlags,zDelta,pt.x,pt.y);
	if( zDelta < 0 &&(cPos == nLimit) && cPos != 0 ){
//		GetParent()->SendMessage( WM_REQUEST_NEXT );
	}
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CSListCtrl1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY   *pHDN = (HD_NOTIFY*)lParam;

	if( pHDN->hdr.code == HDN_ITEMCLICK ){
		
	}
	if (!m_bFix)
		return CListCtrl::OnNotify(wParam, lParam, pResult);

    if (pHDN->hdr.code == HDN_BEGINTRACKW 
		|| pHDN->hdr.code == HDN_BEGINTRACKA)
	{
		if (m_FixCol == -1 || pHDN->iItem <= m_FixCol)
		{
			*pResult = TRUE;	// disable tracking
			return TRUE;		// Processed message
		}
	}else if( pHDN->hdr.code == HDN_DIVIDERDBLCLICKW
		||pHDN->hdr.code == HDN_DIVIDERDBLCLICKA)
	{
		if (m_FixCol == -1 || pHDN->iItem <= m_FixCol)
		{
			*pResult = TRUE;	// disable tracking
			return TRUE;		// Processed message
		}
		
		TCHAR szText[100];
		memset(szText,0x00, sizeof(TCHAR)*100);

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT;//|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(m_HdCtrl.GetItem(pHDN->iItem, &hditem));
		CPaintDC dc(this);
		CFont* pOld = dc.SelectObject(GetFont());
		int cx = dc.GetTextExtent(hditem.pszText).cx;
		dc.SelectObject(pOld);
		if( pHDN->iItem ==4 )cx+=20;
		SetColumnWidth( pHDN->iItem, cx);
		*pResult = TRUE;
		return TRUE;
		
	}
 
    return CListCtrl::OnNotify(wParam, lParam, pResult);
}


void CSListCtrl1::SetEditLabels()
{
	m_bEditEnable = true;
}

void CSListCtrl1::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
/*	
	int index;
	int colNum = 0;
	
	if ((index = hitTestEx(point, &colNum)) != -1)
	{
		
		if (m_bEditEnable)
		{
			bool bIn = false;
			for( int i = 0 ; i < m_EditableList.GetSize(); i++ )
			{	
				if(m_EditableList.GetAt(i) == colNum)
				{
					bIn = true;
					break;
				}

			}

			if( !bIn  ){
				GetParent()->SendMessage(WM_LIST_DBCLK,index,colNum);
				return;
			}
			
			if( colNum != 0){
				CString str = GetItemText( index, 0);
				if( str == "")
					return;
			}
			LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
			if (m_disEditCol == colNum)
			{
			//	style &= ~LVS_EDITLABELS;
			//	SetWindowLong(m_hWnd, GWL_STYLE, style);
				return;
			}
			else
			{
			//	style |= LVS_EDITLABELS;
			//	SetWindowLong(m_hWnd, GWL_STYLE, style);
				EditSubItem(index, colNum);
			}
		}
	}
*/
}

int CSListCtrl1::hitTestEx(const CPoint& Point, int* pColumn)
{
	int row = HitTest(Point, NULL);

	if (pColumn)
		*pColumn = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;
  
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if (bottom > GetItemCount())
		bottom = GetItemCount();

	if (!m_HdCtrl.GetSafeHwnd() )
		m_HdCtrl.SubclassDlgItem(0, this);
	const int nColCount = m_HdCtrl.GetItemCount();

	CRect rect;
	int colNum = 0;
	int colWidth = 0;

	for(; row <= bottom; row++)
	{
		GetItemRect(row, &rect, LVIR_BOUNDS);
		if (rect.PtInRect(Point))
		{
			for (colNum = 0; colNum < nColCount; colNum++)
			{
				colWidth = GetColumnWidth(colNum);
				if (Point.x >= rect.left && Point.x <= (rect.left + colWidth))
				{
					if (pColumn)
						*pColumn = colNum;
					return row;
				}
				rect.left += colWidth;
			}
		}
	}
	return -1;
}

CEdit* CSListCtrl1::EditSubItem(int item, int col)
{
	const int nColCnt = m_HdCtrl.GetItemCount();
	if (col >= nColCnt || GetColumnWidth(col) < 5)
		return (CEdit *)NULL;

	int offset = 0;
	for (int ii = 0; ii < col; ii++)
		offset += GetColumnWidth(ii);

	CRect iRect, cRect;
	GetItemRect(item, &iRect, LVIR_BOUNDS);
	GetClientRect (&cRect);

	CSize size;
	if (offset + iRect.left < 0 || offset + iRect.left > cRect.right)
	{
		if (offset + iRect.left > 0)
			size.cx = -(offset - iRect.left);
		else
			size.cx = offset - iRect.left;
		size.cy = 0;
		Scroll(size);
		iRect.left -= size.cx;
	}

	LV_COLUMN lvCol;
	lvCol.mask = LVCF_FMT;
	GetColumn(col, &lvCol);
	DWORD dwStyle{};
	if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else 
		dwStyle = ES_CENTER;

//	iRect.left += offset+4;
	iRect.left += offset+2;
	if( item == 0)
		iRect.top -= 1;
//	iRect.bottom -=1;
	iRect.right = iRect.left + GetColumnWidth(col) - 3;
	if (iRect.right > cRect.right)
		iRect.right = cRect.right;
	
	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	DeleteEdit();
	m_pEdit = std::make_unique<CListEdit>(this, item, col, GetItemText(item, col));
	m_pEdit->SetOwner(GetOwner());
	m_pEdit->Create(dwStyle, iRect, this, IDC_EDITCELL);
	m_pEdit->SetFont(&m_Font);
	SetEditState(true);
	int limit = 9;
	switch (col)
	{
	case 0:
		limit = 6;
		break;
	case 6:
		limit = 9;
		break;
	case 7:
		limit = 11;
		break;
	case 11:
		limit = 9;
		break;
	}
	m_pEdit->SetLimitText(limit);
	return m_pEdit.get();
}
bool CSListCtrl1::GetEditState()
{
	return m_bIsEdit;
}

void CSListCtrl1::SetEditState(bool bIsEdit)
{
	m_bIsEdit = bIsEdit;
}

void CSListCtrl1::DeleteEdit()
{
	if (m_pEdit)
	{
		m_pEdit = nullptr;
	}
}

void CSListCtrl1::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnLButtonDown(nFlags, point);
	
	int index=0;
	int colNum = 0;
	
	if ((index = hitTestEx(point, &colNum)) != -1)
	{
		TRACE("%d,%d,%d,%d\n",index,colNum,m_nClkItem,m_nClkSubItem);
		if( IsEditSubItem(index,colNum) &&
			m_nClkItem == index &&
			m_nClkSubItem == colNum&&
			colNum != 1 )
		{
			EditSubItem(index,colNum);
		}else{		
			GetParent()->SendMessage(WM_LIST_CLK,index,colNum);
		}
		m_nClkItem = index;
		m_nClkSubItem = colNum;

	}	

}

bool CSListCtrl1::IsEditSubItem(int nItem,int nSubItem){
	
	if (m_bEditEnable)
	{
		bool bIn = false;
		for( int i = 0 ; i < m_EditableList.GetSize(); i++ )
		{	
			if(m_EditableList.GetAt(i) == nSubItem)
			{
				bIn = true;
				break;
			}
		}

		if( !bIn)
			return false;
			
		if( nSubItem != 0){
			CString str = GetItemText( nItem, 0);
			if( str == "")
				return false;
		}

		if (m_disEditCol == nSubItem)
		{
			return false;
		}
	}

	return true;
}

void CSListCtrl1::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bSortAsc = !m_bSortAsc;
	const int nSub = pNMListView->iSubItem;
	CString sza,szb;	
	bool bChange = false;
	int nSize = 0;
	for(int i = 0 ; i < GetItemCount() ; i++){
		if( GetItemText(i,0) == ""){
			nSize = i;
			break;
		}
	}
	for(int i = 0 ; i < nSize -1 ;i++){

		for(int j = i+1 ; j < nSize; j++ )
		{
			bChange = false;
			sza = GetItemText(i,nSub);
			szb = GetItemText(j,nSub);
			
			if( nSub == 3 ||nSub == 5 ||nSub == 6 ||nSub == 7 ||nSub == 8 ||nSub == 9||nSub == 10  ){
				const double na = ConvertToDouble(sza)	;
				const double nb = ConvertToDouble(szb)	;

				if( m_bSortAsc )
				{
					if( na > nb ){		
						bChange = true;
					}
				}else{
					if( na < nb ){		
						bChange = true;
					}
				}
			}
			else if (nSub == 4)
			{
				if (sza.Find(ARR_UP) != -1)	sza.Replace(ARR_UP, "+");
				if (sza.Find(TRI_UP) != -1)	sza.Replace(TRI_UP, "+");
				if (sza.Find(ARR_DN) != -1)	sza.Replace(ARR_DN, "-");
				if (sza.Find(TRI_DN) != -1)	sza.Replace(TRI_DN, "-");

				if (szb.Find(ARR_UP) != -1)	szb.Replace(ARR_UP, "+");
				if (szb.Find(TRI_UP) != -1)	szb.Replace(TRI_UP, "+");
				if (szb.Find(ARR_DN) != -1)	szb.Replace(ARR_DN, "-");
				if (szb.Find(TRI_DN) != -1)	szb.Replace(TRI_DN, "-");

				const double na = ConvertToDouble(sza);
				const double nb = ConvertToDouble(szb);

				if( m_bSortAsc )
				{
					if( na > nb )	bChange = true;
				}
				else
				{
					if( na < nb )	bChange = true;
				}
			}
			else
			{
				if( m_bSortAsc )
				{
					if( sza.Compare(szb) > 0 ){		
						bChange = true;
					}
				}else{
					if( sza.Compare(szb) < 0 ){		
						bChange = true;
					}
				}
			}

			if( bChange ){

				for(int k = 0 ; k < m_HdCtrl.GetItemCount() ; k++){
					sza = GetItemText(i,k);
					szb = GetItemText(j,k);
					
					SetItemText( j,k, sza);
					SetItemText( i,k, szb);
				}
			}
		}
	}
	*pResult = 0;
}

void CSListCtrl1::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//마우스 포인터가 그리드에 있을 경우에 마우스 모양을 손가락 모양으로 바꿔주고
	//툴팁을 띄운다

	CPoint pt;
	LVHITTESTINFO ht;

	GetCursorPos(&pt);			
	ScreenToClient(&pt);
	
	ht.pt = pt;
	ht.flags = LVHT_ONITEM;

	const int nCursorItem = HitTest(&ht);

	if(nCursorItem == -1)
	{
		return;
	}

	if(ht.pt.x >= 0 && ht.pt.x < 60)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649)));
	}

	if(ht.pt.x >= 0 && ht.pt.x < 44)
	{
		CRect rect, cellRC;
		CString string = "더블클릭하시면 종목번호를 입력하실 수 있습니다";
		
		GetClientRect(rect);

		cellRC.left = ht.pt.x;
		cellRC.top = ht.pt.y;
		cellRC.right = ht.pt.x + 60;
		cellRC.bottom = ht.pt.y + 21;

		m_pTooltip->ShowTips(cellRC, rect, string);
	}
	
	CListCtrl::OnMouseMove(nFlags, point);
}
