//DrDrop.cpp

#include "stdafx.h"
#include "format.h"
#include "DrDrop.h"

CDrDrop::CDrDrop()
{
    m_pWnd = NULL;
    m_bIsDragging = FALSE;
}

DROPEFFECT CDrDrop::OnDragEnter(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState, CPoint point)
{
    return OnDragOver( pWnd, pObj, dwKeyState, point );
}

DROPEFFECT CDrDrop::OnDragOver(CWnd* pWnd,COleDataObject* pObj,
		DWORD dwKeyState,CPoint point)
{
	if(m_pWnd != NULL)
	{
		m_pWnd->SendMessage(WM_DRAGCHARTDATA, 0, 
					(LPARAM)MAKELONG(point.x, point.y));
		
	}
	DROPEFFECT deResult = DROPEFFECT_NONE;
	if(pObj->IsDataAvailable(CF_TEXT))
	{
		if(dwKeyState & MK_CONTROL )
			deResult = DROPEFFECT_COPY;
		else
			deResult = DROPEFFECT_MOVE;
	}
	return deResult;
}


BOOL CDrDrop::OnDrop( CWnd* pWnd,COleDataObject* pObj,
			DROPEFFECT de,CPoint point )
{
	CFormatEtc	fe;
	STGMEDIUM   stg;
	BOOL bHasText = pObj->GetData( CF_TEXT, &stg, &fe );
	if( bHasText == FALSE )
		return FALSE;
	LPCSTR pszObj = (LPCSTR)GlobalLock( stg.hGlobal );
	FSItem *pData = (FSItem *)pszObj;
	if( pszObj )
	{
		m_pWnd->SendMessage(WM_DROPFSDATA, 0, (LPARAM) pData);
		GlobalUnlock(stg.hGlobal);
		GlobalFree(stg.hGlobal);
		return TRUE;
	}
	return FALSE;
}

void CDrDrop::SetName(LPSTR Name)
{
    m_Name = Name;
}

void CDrDrop::SetParent(CWnd* pWnd)
{
    m_pWnd = pWnd;
}

