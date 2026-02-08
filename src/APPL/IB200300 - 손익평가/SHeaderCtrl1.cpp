// SHeaderCtrl1.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "SHeaderCtrl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl11

CSHeaderCtrl1::CSHeaderCtrl1()
{
	m_pMapWnd = NULL;
	m_Font.CreatePointFont(90,"±¼¸²");
	m_nHideColIdx = 0xffff;
}

CSHeaderCtrl1::~CSHeaderCtrl1()
{
}


BEGIN_MESSAGE_MAP(CSHeaderCtrl1, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSHeaderCtrl1)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSHeaderCtrl1::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl1 message handlers


void CSHeaderCtrl1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc,rcHeader;

	GetClientRect(&rcHeader);
	int nTotW = 0;
	const int nCount = GetItemCount();
/*	if( GetMapDlg()->m_bSize && bFirst ){
		GetMapDlg()->m_bSize = true;
		return;
	}
*/
		
#ifdef _USE_MEMDC	
	CMemDC mdc(&dc,&rcHeader);
#endif
	
	for(int i = 0 ; i < nCount; i++ ){
		if( i >= m_nHideColIdx)
			break;

		const int iItem = OrderToIndex(i);

		TCHAR szText[100]{};

		HDITEM hditem;
		hditem.mask = HDI_HEIGHT|HDI_WIDTH|HDI_FORMAT|HDI_TEXT;//|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rc));
//		rc.bottom=18;
		nTotW += rc.Width();
//		CMemDC memDC(&dc);
//		memDC.FillSolidRect(rc , m_clrBk);
//		memDC.Draw3dRect(rc , RGB(255,255,255), m_clrEdge);
		
//		dc.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&memDC,0,0,SRCCOPY);
#ifdef _USE_MEMDC
		mdc.FillSolidRect(rc , m_clrBk);
		rc.right+=1;
//		rc.bottom+=1;
		mdc.Draw3dRect(rc , m_clrEdge, m_clrEdge );//RGB(255,255,255));
		
		if (rc.Width() > 0 && hditem.mask & HDI_TEXT && hditem.fmt & HDF_STRING)
		{
//			int size = pDC->GetTextExtent(hditem.pszText);

			mdc.SetBkMode(TRANSPARENT);
			mdc.SetTextColor( m_clrText );
			// always center column headers
			CFont* pOld= mdc.SelectObject(&m_Font);
			mdc.DrawText(hditem.pszText, rc, 
				DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			mdc.SelectObject(pOld);
		}
#else		
		dc.FillSolidRect(rc , m_clrBk);
		rc.right+=1;
//		rc.bottom+=1;
		dc.Draw3dRect(rc , m_clrEdge, m_clrEdge );//RGB(255,255,255));
		
		if (rc.Width() > 0 && hditem.mask & HDI_TEXT && hditem.fmt & HDF_STRING)
		{
//			int size = pDC->GetTextExtent(hditem.pszText);

			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor( m_clrText );
			// always center column headers
			CFont* pOld= dc.SelectObject(&m_Font);
			dc.DrawText(hditem.pszText, rc, 
				DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			dc.SelectObject(pOld);
		}
#endif
	}
	
	if( rcHeader.right > nTotW ){
		dc.FillSolidRect( nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom,m_clrBk);
		dc.Draw3dRect(nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom , m_clrEdge,m_clrEdge);
	}
}

LRESULT CSHeaderCtrl1::OnLayout(WPARAM, LPARAM lParam)
{
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

//	if (m_bStaticBorder)
//		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}



void CSHeaderCtrl1::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB200300\n%d\t%d\tSYM\t\n",point.x,point.y);

	m_pMapWnd->m_pParent->SendMessage( WM_USER ,MAKEWPARAM(mouseDLL,false) ,(LPARAM)(LPCTSTR)str );
//	mouseDLL		0x0b		// OnMouse(lParam, lbutton)
						// HIWORD(wParam) : lbutton(True) or rbutton(False)
						// lParam : mapN \n pt.x \t pt.y \n SYM \t DATA \n
						// if grid, lParam : mapN \t \n pt.x \t pt.y \n SYM \t DATA \n
	CHeaderCtrl::OnRButtonDown(nFlags, point);
}

