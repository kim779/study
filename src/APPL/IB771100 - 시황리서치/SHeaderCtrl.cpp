// SHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "SHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl

CSHeaderCtrl::CSHeaderCtrl()
{
	m_Font.CreatePointFont(90,"±¼¸²Ã¼");
	m_nHideColIdx = 0xffff;
	m_clrBk = 0;
	m_clrEdge = 0;
	m_clrText = 0;
	m_pMapDlg = NULL;
}

CSHeaderCtrl::~CSHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CSHeaderCtrl message handlers
#include "BkUtil.h"
void CSHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc,rcHeader;

	GetClientRect(&rcHeader);
	int nTotW = 0;
	const int nCount = GetItemCount();

#ifdef _USE_MEMDC	
	CMemDC mdc(&dc,&rcHeader);
#endif
	
	for(int i = 0 ; i < nCount; i++ ){
		if( i >= m_nHideColIdx)
			break;

		const int iItem = OrderToIndex(i);

		TCHAR szText[100]{};

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT;//|HDI_IMAGE|HDI_BITMAP;
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
		mdc.Draw3dRect(rc , m_clrEdge, m_clrEdge );//RGB(255,255,255));
		
		if (rc.Width() > 0 && hditem.mask & HDI_TEXT && hditem.fmt & HDF_STRING)
		{

			mdc.SetBkMode(TRANSPARENT);
			mdc.SetTextColor( m_clrText );
			CFont* pOld= mdc.SelectObject(&m_Font);
			mdc.DrawText(hditem.pszText, rc, 
				DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			mdc.SelectObject(pOld);
			DeleteObject(pOld);
		}
#else		
		dc.FillSolidRect(rc , m_clrBk);
		rc.right+=1;
		dc.Draw3dRect(rc , m_clrEdge, m_clrEdge );//RGB(255,255,255));
		
		if (rc.Width() > 0 && hditem.mask & HDI_TEXT && hditem.fmt & HDF_STRING)
		{

			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor( m_clrText );
			CFont* pOld= dc.SelectObject(&m_Font);
			dc.DrawText(hditem.pszText, rc, 
				DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			dc.SelectObject(pOld);
			DeleteObject(pOld);
		}
#endif
	}
	
	if( rcHeader.right > nTotW ){
		dc.FillSolidRect( nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom,m_clrBk);
		dc.Draw3dRect(nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom , m_clrEdge,RGB(255,255,255));
	}
}

LRESULT CSHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
//	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;
	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}

#include "MapDlg.h"

void CSHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB771100\n%d\t%d\n",point.x,point.y);

	m_pMapDlg->m_pParent->SendMessage( WM_USER ,MAKEWPARAM(mouseDLL,false) ,(LPARAM)(LPCTSTR)str );
//	mouseDLL		0x0b		// OnMouse(lParam, lbutton)
						// HIWORD(wParam) : lbutton(True) or rbutton(False)
						// lParam : mapN \n pt.x \t pt.y \n SYM \t DATA \n
						// if grid, lParam : mapN \t \n pt.x \t pt.y \n SYM \t DATA \n
	CHeaderCtrl::OnRButtonDown(nFlags, point);
}

