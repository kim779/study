// SListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "SListCtrl.h"
#include "MapDlg.h"

#include <MMSYSTEM.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define T_LIST_SELECT 244
#define SHOW_TOOLTIP  245
/////////////////////////////////////////////////////////////////////////////
// CSListCtrl
#define SO(ob,pDC)\
	((CDC*)pDC)->SelectObeject(&ob);
		
CSListCtrl::CSListCtrl()
{
	m_clrBk1 = RGB(255,255,255);
	m_clrBk2 = RGB(244,244,244);
	m_clrText = RGB(30,30,30);
	m_clrSelectText = RGB( 30,80,244);
	m_clrSelectBk = RGB(180,180,233);
	m_clrEdge = RGB(200,200,200);
	m_nHideColIdx = 0xffff ;
	
	m_hTypeBitmap[0] = NULL;
	m_hTypeBitmap[1] = NULL;

	m_hTypeBitmap[2] = NULL;
	m_hTypeBitmap[3] = NULL;
	m_iNewsType = 0;
	m_pTooltip = NULL;
	m_iInterval = 0;
}

CSListCtrl::~CSListCtrl()
{	
}

//#else
//	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
//#endif

BEGIN_MESSAGE_MAP(CSListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSListCtrl)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, OnItemchanging)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSListCtrl message handlers

BOOL CSListCtrl::OnEraseBkgnd(CDC* pDC){
//	TRACE("CSListCtrl::OnEraseBkgnd\n");
	return true;
//	return CListCtrl::OnEraseBkgnd(pDC);
}

void CSListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HdCtrl.SubclassWindow(pHeader->m_hWnd));
	}
	
	if (m_pTooltip == NULL)
	{
	//	m_pTooltip = new CTipDlg(m_pMapDlg->m_pParent, this);
		m_pTooltip = std::make_unique<CTipDlg>(m_pMapDlg->m_pParent, this);
		m_pTooltip->Create(CTipDlg::IDD, NULL);			
	}

	m_bmpNumberBack.LoadBitmap(IDB_BITMAP7);
	m_fontNumber.CreateFont(
						16,                        // nHeight
						0,                         // nWidth
						0,                         // nEscapement
						0,                         // nOrientation
						FW_BOLD,				   // nWeight
						FALSE,                     // bItalic
						FALSE,                     // bUnderline
						0,                         // cStrikeOut
						ANSI_CHARSET,              // nCharSet
						OUT_DEFAULT_PRECIS,        // nOutPrecision
						CLIP_DEFAULT_PRECIS,       // nClipPrecision
						DEFAULT_QUALITY,           // nQuality
						DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
						_T("Arial Black"));     
	
	CListCtrl::PreSubclassWindow();
}

void CSListCtrl::HideColumnOver(int nColIdx){
	m_nHideColIdx = nColIdx;
	m_HdCtrl.HideColumnOver(nColIdx);

}
#ifdef _CUSTOM_DRAW

void CSListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messssages for every item.

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.

		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		int nSubItem = pLVCD->iSubItem;
		
		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = m_clrText;
		
//		GetDrawColors(nItem,nSubItem,pLVCD->clrText, pLVCD->clrTextBk);
		
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	
#ifdef _USE_MEMDC
		CMemDC dc(pDC,&rect);
		dc.FillSolidRect( rect,pLVCD->clrTextBk );
		
		CPen pen;
		pen.CreatePen(PS_SOLID,1, m_clrEdge);
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.MoveTo( rect.left,rect.top );
		dc.LineTo( rect.left,rect.bottom );
		dc.SelectObject(pOldPen);
		pen.DeleteObject();

		rect.left++;
		
	//	pDC->FillSolidRect( rect, )
		CRect rcText = rect;
		rcText.left +=2;
		rcText.right-=3;
		DrawText(nItem,nSubItem,&dc, m_clrText,pLVCD->clrTextBk,rcText);
#else 
//		rect.left--;
		pDC->FillSolidRect( rect,pLVCD->clrTextBk );
		
		CPen pen;
		pen.CreatePen(PS_SOLID,1, m_clrEdge);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo( rect.left,rect.top );
		pDC->LineTo( rect.left,rect.bottom );
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();

		rect.left++;
		
	//	pDC->FillSolidRect( rect, )
		CRect rcText = rect;
		rcText.left +=2;
		rcText.right-=3;
		DrawText(nItem,nSubItem,pDC, m_clrText,pLVCD->clrTextBk,rcText);
		
#endif
	//	TRACE("%d,%d\n", nItem,nSubItem );

		*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		//TRACE("CDRF_SKIPDEFAULT\n");
	}
}
#endif


BOOL CSListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY   *pHDN = (HD_NOTIFY*)lParam;


	if( pHDN->hdr.code == HDN_DIVIDERDBLCLICKW
		||pHDN->hdr.code == HDN_DIVIDERDBLCLICKA)
	{
		if( GetItemCount() == 0 )
		{
			*pResult = TRUE;	// disable tracking
			return TRUE;		// Processed message
		}
		
	}
	
    return CListCtrl::OnNotify(wParam, lParam, pResult);
}


void CSListCtrl::DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect )
{
	ASSERT(pDC);

//	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	CString str;
	str = GetItemText(nItem, nSubItem);

	if (!str.IsEmpty())
	{
		// get text justification
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_HdCtrl.GetItem(nSubItem, &hditem);
		const int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE|DT_NOPREFIX;
		if (nFmt == HDF_CENTER)
			nFormat |= DT_CENTER;
		else if (nFmt == HDF_LEFT)
			nFormat |= DT_LEFT;
		else
			nFormat |= DT_RIGHT;

		CFont *pOldFont = NULL;
		CFont boldfont;

	//	pOldFont= pDC->SelectObject( &m_Font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
	//	pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, nFormat);
		if (pOldFont)
		{
			pDC->SelectObject(pOldFont);
			DeleteObject(pOldFont);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
// GetDrawColors
void CSListCtrl::GetDrawColors(int nItem,
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
	char szTemp[1024]={0};
	if( GetItemText(nItem,3,szTemp,1024 ) ){
		CString szText = szTemp;

		szText.TrimLeft(" ");szText.TrimRight(" ");
		if( szText == STR_MONEYTODAY_ROOT)
			crText = m_pMapDlg->m_clrTitleText[0];
		else if( szText == STR_INFOSTOCK_ROOT)
			crText = m_pMapDlg->m_clrTitleText[1];
		else if( szText == STR_HANENWS_ROOT)
			crText = m_pMapDlg->m_clrTitleText[2];
		else if(szText == STR_EDAILY_ROOT)
			crText = m_pMapDlg->m_clrTitleText[3];	

		else if( szText == STR_YOUNHAP_ROOT)
			crText = m_pMapDlg->m_clrTitleText[4];
		else if( szText == STR_MAEKYONG_ROOT)
			crText = m_pMapDlg->m_clrTitleText[5];
		else if(szText == STR_ETODAY_ROOT)
			crText = m_pMapDlg->m_clrTitleText[6];	
		else if(szText == STR_NEWSP_ROOT)
			crText = m_pMapDlg->m_clrTitleText[7];	
	}
	
	colorText = crText;
	colorBkgnd = crBkgnd;
}

int b =0 ;

#ifndef _CUSTOM_DRAW
void CSListCtrl::InvalidateHedarCtrl() 
{
	m_HdCtrl.Invalidate();	
}

void CSListCtrl::DrawNumberBackground( CRect* prc, CDC* paint )
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, (HBITMAP)m_bmpNumberBack); 

	::TransparentBlt(paint->GetSafeHdc(), prc->left+5, prc->top+1, 22, prc->Height()-3, hDC, 0, 0, 15, 13, 0xFF00FF);
	::DeleteDC (hDC);
}

void CSListCtrl::OnPaint() 
{
	CPaintDC pdc(this); // device context for painting

	CRect rc,rcList;
	int nH = 0,nW = 0,nItemH=0;
	COLORREF clrText{},clrBk{};
	GetClientRect(&rcList);
	CMemDC dc(&pdc,&rcList);
	const int nStartIndex = GetTopIndex();
	const int nVListCount = min( GetItemCount() ,nStartIndex + GetCountPerPage()+1 );

	for(int i = nStartIndex ; i < GetItemCount()  ; i++ ){
		
		GetItemRect(i,rc,LVIR_BOUNDS );
	
		//느려지는 문제 수정
		if (rc.top > rcList.bottom)
		   break;

	
		if( rc.top < 18 )
			continue;

		nH = rc.bottom;nItemH = rc.Height();

		// CELL 한개씩 그리기 
		//CFont* pOldFont = (CFont*)dc.SelectObject( *m_pnn);
		for(int j = 0 ; j < m_HdCtrl.GetItemCount() ; j++ )
		{
			GetDrawColors(i,j,clrText,clrBk);
			GetSubItemRect(i,j,LVIR_BOUNDS,rc);
			if(rc.Width() <= 0 )
				continue;

			if( j== 0)
			{
				CRect rc1;
				GetSubItemRect(i,1,LVIR_BOUNDS,rc1);
				rc.right = rc1.left;	

			}
			if( i== 0){
				rc.top -=2;
			}

			dc.FillSolidRect(rc, clrBk);

//			kbkim temp
// 			CPoint pt;
// 			pt.x = 0;
// 			pt.y = 0;
// 			dc.RoundRect(rc,pt);

			if( j == 1 )
			{
				CRect rc1;
				GetSubItemRect(i,1,LVIR_BOUNDS,rc1);
				DrawNumberBackground( &rc1, &dc );
			}			
			
			if( j < m_nHideColIdx )
			{
				nW = rc.right; 
				CPen pen;
				pen.CreatePen(PS_SOLID,1, m_clrEdge);
				CPen* pOldPen = dc.SelectObject(&pen);
				dc.MoveTo( rc.left,rc.top );
				dc.LineTo( rc.left,rc.bottom );
				dc.SelectObject(pOldPen);
				pen.DeleteObject();
				DeleteObject(pOldPen);
			
				dc.SetBkMode(TRANSPARENT);

				if( j == 1 )
				{
					dc.SelectObject( m_fontNumber );
					dc.SetTextColor(RGB(255,255,255));
				}
				else
				{
					dc.SelectObject( *m_pnn );
					dc.SetTextColor(clrText);
				}
				
				CString str = GetItemText(i,j);
				rc.left+=2;

				if(j == m_iTypeField)
					DrawTypeImage(&dc, rc);
				else if(j==4)
				{
					DrawColorText(&dc, str, rc, clrText);
			//		dc.DrawText(str , rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
				}
				else
				{
					HDITEM hditem;
					hditem.mask = HDI_FORMAT;
					m_HdCtrl.GetItem(j, &hditem);
					const int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
					UINT nFormat = DT_VCENTER | DT_SINGLELINE|DT_NOPREFIX;
					if (nFmt == HDF_CENTER)
						nFormat |= DT_CENTER;
					else if (nFmt == HDF_LEFT)
						nFormat |= DT_LEFT;
					else
						nFormat |= DT_RIGHT;
					
					dc.DrawText(str , rc, nFormat);
				}
				
			}			
		}
		//dc.SelectObject(pOldFont);
		//DeleteObject(pOldFont);
	}

}

#endif
void CSListCtrl::DrawColorText(CDC* pDC, CString sTxt, CRect& rc, COLORREF tcolor)
{
	int iFind = -1;
	CString sKey = "";
	int istep{};
	for(int i=0; i<m_pMapDlg->m_arSpecialKeyList.GetSize(); i++)
	{
		if(  (iFind=sTxt.Find(m_pMapDlg->m_arSpecialKeyList.GetAt(i).sTxt)) > -1)
		{
			sKey = m_pMapDlg->m_arSpecialKeyList.GetAt(i).sTxt;
			istep = m_pMapDlg->m_arSpecialKeyList.GetAt(i).iVal-1;
			break;
		}
	}

	if(iFind > -1)
	{
		CString sLeft = sTxt.Left(iFind);
		iFind += sKey.GetLength(); 
		CString sRight = sTxt.Right(sTxt.GetLength() - iFind);  
		
		CRect trc = rc;
		CSize tsize;
		if(sLeft.GetLength() > 0)
		{
			pDC->DrawText(sLeft, rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			tsize = pDC->GetTextExtent(sLeft); 

			trc.left += tsize.cx; 
		}

		const COLORREF color = m_pMapDlg->m_arStepColor.GetAt(istep);
		pDC->SetTextColor(color); 
		pDC->DrawText(sKey, trc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		pDC->SetTextColor(tcolor); 
		tsize = pDC->GetTextExtent(sKey); 
		trc.left += tsize.cx; 
		pDC->DrawText(sRight, trc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	}
	else
		pDC->DrawText(sTxt, rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
//	dc.DrawText(str , rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
}

void CSListCtrl::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;

	if(pNMListView->iItem == 0){// && pNMListView->uNewState != 0){
		GetItemRect(0,rc,LVIR_BOUNDS);
		rc.top-=2;
		InvalidateRect( rc);
	}
	
	*pResult = 0;
}

BOOL CSListCtrl::DestroyWindow() 
{
	m_arNewsInfos.RemoveAll();
	return CListCtrl::DestroyWindow();
}

int CSListCtrl::GetSelectedItem(){
	int nRet = -1;
	for( int i = 0 ; i < GetItemCount() ; i++ ){
		if( GetItemState(i,LVIS_SELECTED) ){
			nRet = i;
			break;
		}
	}
	return nRet;
}

void CSListCtrl::OnDestroy() 
{
	if (m_pTooltip)
	{
		m_pTooltip->DestroyWindow();
	//	delete m_pTooltip;
	//	m_pTooltip = NULL;
	}
	CListCtrl::OnDestroy();
	// TODO: Add your message handler code here
	
}

LRESULT CSListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_PAINT )
	{
		//NM_CUSTOMDRAW
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

BOOL CSListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CListCtrl::PreTranslateMessage(pMsg);
}


void CSListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	const int cPos = GetScrollPos(SB_VERT);
	const int nLimit = GetScrollLimit(SB_VERT);
	
	if( nSBCode==SB_ENDSCROLL && (cPos == nLimit) && cPos != 0 ){
		GetParent()->SendMessage( WM_REQUEST_NEXT );		
// 		TRACE("OnVScroll()\n");
	}
//	TRACE("OnVScroll,(%d,%d,%d,%d),%d,%d\n",nSBCode ,SB_LINEDOWN,SB_ENDSCROLL,SB_BOTTOM,cPos,nLimit);
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSListCtrl::IsScroll( void )
{
	const int n = GetTopIndex();
	const int nLast = n + GetCountPerPage();

	const int nCount = GetItemCount();
	if( nLast >= nCount )
		return FALSE;

	return TRUE;
}

BOOL CSListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	const int cPos = GetScrollPos(SB_VERT);
	const int nLimit = GetScrollLimit(SB_VERT);

	const DWORD dwTime = timeGetTime();

//	TRACE("OnMouseWheel,%d, %d,%d,%d\n",nFlags,zDelta,pt.x,pt.y);
	if( zDelta < 0 &&(cPos == nLimit) && cPos != 0 && dwTime-m_iInterval > 500 ){
		GetParent()->SendMessage( WM_REQUEST_NEXT );

		m_iInterval = timeGetTime();
		
// 		TRACE("OnMouseWheel()\n");
	}
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CSListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{

	CPoint pt;
	CString string;

	GetCursorPos(&pt);				//마우스 커서 위치
	ScreenToClient(&pt);			// 리스트 컨트롤에 맞게 좌표를 변경

	ht.pt = pt;
	ht.flags = LVHT_ONITEM;
	const int nCursorItem = HitTest(&ht);
	
	if(nCursorItem == -1)
	{
		return;						//커서가 리스트컨트롤 내부에 존재하지 않는다
	}
	
	//Paint에다가 커서 잡아서 ht row 잡아서 색칠한다
	if(ht.pt.x >=0 && ht.pt.x <18)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649)));
		CRect rect, cellRC;

		if(!m_iNewsType)
		{
			string = "스크랩을 원하실경우 클릭하세요";
		}
		else
		{
			string = "삭제를 원하실경우 클릭하세요";
		}
		
			
		GetClientRect(rect);
		
		cellRC.left = ht.pt.x;
		cellRC.top = ht.pt.y;
		cellRC.right = ht.pt.x + 17;
		cellRC.bottom = ht.pt.y + 20;

		if(m_pTooltip)
			m_pTooltip->ShowTips(cellRC, rect, string);	
			
	}
}

void CSListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB771000\n%d\t%d\n",point.x,point.y);

	m_pMapDlg->m_pParent->SendMessage( WM_USER ,MAKEWPARAM(mouseDLL,false) ,(LPARAM)(LPCTSTR)str );
//	mouseDLL		0x0b		// OnMouse(lParam, lbutton)
						// HIWORD(wParam) : lbutton(True) or rbutton(False)
						// lParam : mapN \n pt.x \t pt.y \n SYM \t DATA \n
						// if grid, lParam : mapN \t \n pt.x \t pt.y \n SYM \t DATA \n
*/	CListCtrl::OnRButtonDown(nFlags, point);

}


void CSListCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CRect rt, winrt;
	
	CString ment = CString("본문조회를 원하실경우 엔터키를 누르세요");

	if( pLVKeyDow->wVKey == VK_DOWN ||	pLVKeyDow->wVKey == VK_UP )
	{
		for(int i = 0 ; i < GetItemCount() ; i++){
		
			if( GetItemState( i, LVIS_SELECTED) )
			{
				//m_pMapDlg->RequestContent(pLVKeyDow->wVKey == VK_DOWN ? i +1  : i-1);
				if( VK_DOWN == pLVKeyDow->wVKey && i+1 == GetItemCount()-1)
				{
					m_pMapDlg->RequestTitleList();					
				}

				GetClientRect(winrt);
				GetItemRect(i, rt, LVIR_BOUNDS);
				m_pTooltip->ShowTipsInItem(rt, winrt, ment);
				break;
			}
		}	
	}
	else if( pLVKeyDow->wVKey == VK_RETURN )
	{
		for(int i = 0 ; i < GetItemCount() ; i++)
		{		
			if( GetItemState( i, LVIS_SELECTED) )
			{
				m_pMapDlg->RequestContent(i);
					
				
				break;
			}
		}
	}
	*pResult = 0;
}



void CSListCtrl::SetScroll(int index, bool bSel)
{
	if (bSel)
	{
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	
	CRect rc;
	GetItemRect(0, rc, LVIR_LABEL);

	CSize size;
	size.cx = 0;
	size.cy = rc.Height() * (index - GetTopIndex());
	Scroll(size);
}


void CSListCtrl::SetImageType(HBITMAP hbitmap1, HBITMAP hbitmap1_dn, HBITMAP hbitmap2, HBITMAP hbitmap2_dn)
{
	m_hTypeBitmap[0] = hbitmap1;
	m_hTypeBitmap[1] = hbitmap1_dn;

	m_hTypeBitmap[2] = hbitmap2;
	m_hTypeBitmap[3] = hbitmap2_dn;
}


void CSListCtrl::DrawTypeImage(CDC *pDC, const CRect& rc)
{
	int idx = 0;
	if(!m_iNewsType) idx = 0;
	else idx = 2;
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, m_hTypeBitmap[idx]); //

	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, 16, 16, hDC, 0, 0, 16, 16, 0xFF00FF);
	::DeleteDC (hDC);
}

void CSListCtrl::SetNewsInfo(int index, 	CString sGisn,	CString sSisn,	CString sKeyv,	CString sCode,	CString sFlag)
{
	m_arNewsInfos.InsertAt(index, NewsInfo(sGisn, sSisn, sKeyv, sCode, sFlag));
}

bool CSListCtrl::GetNewsInfo(int index, NewsInfo& pnewsInfo)
{
	try
	{
		if( m_arNewsInfos.GetSize() <= index) 
		{
			return false;
		}
		
		if(index < 0)
		{
			return false;
		}

		pnewsInfo = m_arNewsInfos.GetAt(index);
		/*CopyMemory(pnewsInfo.gisn , pnews.sGisn, min(pnews.sGisn.GetLength(), 10));
		CopyMemory(pnewsInfo.sisn , pnews.sSisn, min(pnews.sSisn.GetLength(), 10));
		CopyMemory(pnewsInfo.keyv , pnews.sKeyv, min(pnews.sKeyv.GetLength(), 24));
		CopyMemory(pnewsInfo.code , pnews.sCode, min(pnews.sCode.GetLength(), 12));
		CopyMemory(pnewsInfo.flag , pnews.sFlag, min(pnews.sFlag.GetLength(), 1));*/
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return true;
}

void CSListCtrl::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case T_LIST_SELECT:
		for(int i = 0 ; i < GetItemCount() ; i++)
		{		
			if( GetItemState( i, LVIS_SELECTED) )
			{
				m_pMapDlg->RequestContent(i);					
				
				break;
			}
		}
		break;
	}
	CListCtrl::OnTimer(nIDEvent);
}

