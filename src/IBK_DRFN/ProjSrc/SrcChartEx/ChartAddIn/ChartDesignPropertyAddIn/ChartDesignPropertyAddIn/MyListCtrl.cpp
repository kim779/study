// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyListCtrl.h"
//#include "BlockBaseData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.26
//
//	분석	:	Graph의 등록을 처리한다.
///////////////////////////////////////////////////////////////////////////////
void CMyListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW *pNMListView = ( NM_LISTVIEW *)pNMHDR;
	// TODO: Add your control notification handler code here
	
	// 0. 먼저 Result 값을 초기화한다.
	*pResult = 0;
	if( !pNMListView) return;

	// 1. ChartItem Data를 위한 Memory를 확보한다.
	HGLOBAL hGlobal = GlobalAlloc( GMEM_SHARE | GMEM_ZEROINIT, sizeof( ChartItem));
	if( !hGlobal) return;
	ChartItem *pszGlobal = ( ChartItem *)GlobalLock( hGlobal);
	if( !pszGlobal)
	{
		GlobalFree( hGlobal);
		return;
	}

	// 2. ChartItem Data를 설정한다. (hItem는 Tree내에서 이동시 이용한다.)
	GetItemText( pNMListView->iItem, 0, pszGlobal->ChartName, 64);
	GlobalUnlock( hGlobal);

	// 3. Chart Drop Object에게 등록되는 Graph Type명을 지정한다. (왜?)
	m_dropTarget.SetName( pszGlobal->ChartName);

	// 4. DragSource에 Text Type으로 Data를 등록시킨다. (Text가 아니면서...)
	m_dragSource.CacheGlobalData( CF_TEXT, hGlobal);

	// 5. DragDrop을 처리한다.
	//	(Copy Option이면 Data를 제거하는 것은 어떻게 되지? 또 앞이 NULL이 들어간 CF_TEXT인데?)
	DROPEFFECT de;
	de = m_dragSource.DoDragDrop( DROPEFFECT_COPY);
}

void CMyListCtrl::InitDropTarget()
{
	m_dropTarget.SetName("ChartInfo");
	m_dropTarget.Register(this);
	m_dropTarget.SetParent(this);
}
