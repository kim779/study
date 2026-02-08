//DivDrop.cpp

#include "stdafx.h"
#include "ChartDrop.h"

CChartDrop::CChartDrop()
{
    m_pWnd = NULL;
    m_bIsDragging = FALSE;
}

DROPEFFECT CChartDrop::OnDragEnter(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState, CPoint point)
{
    return OnDragOver( pWnd, pObj, dwKeyState, point );
}

DROPEFFECT CChartDrop::OnDragOver(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState,CPoint point)
{
	if(m_pWnd != NULL) {
		m_pWnd->SendMessage(WM_DRAGCHARTDATA, 0, 
					(LPARAM)MAKELONG(point.x, point.y));
		
	}
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if(pObj->IsDataAvailable(CF_TEXT)) {
		if(dwKeyState & MK_CONTROL )
			deResult = DROPEFFECT_COPY;
		else
			deResult = DROPEFFECT_MOVE;
	}
	return deResult;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.26
//
//	분석	:	Graph의 등록을 처리한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartDrop::OnDrop( CWnd* pWnd, COleDataObject *pObj, DROPEFFECT de, CPoint point)
{
	// 1. 먼저 OLE Data Object로부터 TEXT Data로 등록된 ChartItem Data의 Pointer를 구한다.
	HGLOBAL	hGlobal = pObj->GetGlobalData( CF_TEXT);
	if( !hGlobal) return FALSE;
	ChartItem *pData = ( ChartItem *)GlobalLock( hGlobal);
	if( !pData)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	// 2. 구해진 ChartItem Data를 Tree Control에게 전달하여 처리토록 한다.
	m_pWnd->SendMessage( WM_DROPCHARTDATA, 0, ( LPARAM)pData);

	// 3. 다 이용한 ChartItem Data를 제거한다.
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
	return TRUE;
}

void CChartDrop::SetName(LPSTR Name)
{
    m_Name = Name;
}

void CChartDrop::SetParent(CWnd* pWnd)
{
    m_pWnd = pWnd;
}

