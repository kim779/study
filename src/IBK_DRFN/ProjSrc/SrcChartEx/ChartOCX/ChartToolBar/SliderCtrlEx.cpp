// SliderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "SliderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx

CSliderCtrlEx::CSliderCtrlEx()
{
	// (2008/12/20 - Seung-Won, Bae) Draw with PolyPolyLine()
	m_ptPoints = NULL;
	m_dwLines = NULL;
	m_nLines = 0;
}

CSliderCtrlEx::~CSliderCtrlEx()
{
	// (2008/12/20 - Seung-Won, Bae) Draw with PolyPolyLine()
	if( m_ptPoints) delete [] m_ptPoints;
	m_ptPoints = NULL;
	if( m_dwLines) delete [] m_dwLines;
	m_dwLines = NULL;
}


BEGIN_MESSAGE_MAP(CSliderCtrlEx, CSliderCtrl)
	//{{AFX_MSG_MAP(CSliderCtrlEx)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrlEx message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  OnCustomdraw
 - Created at  :  2005-09-02   13:39
 - Author      :  최종찬
 - Description :  슬라이더바에 눈금을 그린다.
 -----------------------------------------------------------------------------------------*/
void CSliderCtrlEx::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;

	if (lpcd->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}

	// (2006/4/4 - Seung-Won, Bae) Check Resource Empty!
	if(lpcd->dwDrawStage == CDDS_ITEMPREPAINT && lpcd->hdc)
	{	
		if (lpcd->dwItemSpec == TBCD_TICS)
		{
			if( 0 < m_nLines) ::PolyPolyline( lpcd->hdc, m_ptPoints, m_dwLines, m_nLines);
			*pResult = CDRF_SKIPDEFAULT; 
			return;
		}
	}	
	*pResult = 0;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSetFocus
 - Created at  :  2005-09-10   15:51
 - Author      :  최종찬
 - Description :  SliderCtrl에 포커스가 들어오면 ScrollBar에 포커스를 준다.
 -----------------------------------------------------------------------------------------*/
void CSliderCtrlEx::OnSetFocus(CWnd* pOldWnd) 
{
//	CSliderCtrl::OnSetFocus(pOldWnd);
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
}

void CSliderCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CSliderCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	// (2008/12/20 - Seung-Won, Bae) Draw with PolyPolyLine()
	int startPos = 5;
	int endPos = cx - 5;
	m_nLines = ( endPos - startPos) / 3 + 1;

	if( m_ptPoints) delete [] m_ptPoints;
	m_ptPoints = new POINT[ m_nLines * 2];
	if( m_dwLines) delete [] m_dwLines;
	m_dwLines = new DWORD[ m_nLines];

	m_nLines = 0;
	for( int i = 0; i <= endPos - startPos; i++) if( 0 == i % 3)
	{
		m_ptPoints[ m_nLines * 2].x = i + startPos;
		m_ptPoints[ m_nLines * 2].y = cy;

		m_ptPoints[ m_nLines * 2 + 1].x = i + startPos;
		if( 0 == i % 12)		m_ptPoints[ m_nLines * 2 + 1].y = cy - 5;
		else if( 0 == i % 6)	m_ptPoints[ m_nLines * 2 + 1].y = cy - 4;
		else					m_ptPoints[ m_nLines * 2 + 1].y = cy - 3;

		m_dwLines[ m_nLines] = 2;
		m_nLines++;
	}
}
