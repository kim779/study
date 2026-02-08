#include "stdafx.h"
#include "afxole.h" 
#include "DropTarget.h" 

CDropTarget::CDropTarget() {}

CDropTarget::~CDropTarget() {}  

DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
                           pDataObject, DWORD dwKeyState, CPoint point )
{       
    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY; // Copy the source text
    else
        return DROPEFFECT_MOVE; // Move the source text
} 


void CDropTarget::OnDragLeave(CWnd* pWnd)
{
    COleDropTarget:: OnDragLeave(pWnd);
}
 

DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
           pDataObject, DWORD dwKeyState, CPoint point )
{
    if((dwKeyState&MK_CONTROL) == MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;    
}


BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
                 DROPEFFECT dropEffect, CPoint point )
{           
    HGLOBAL  hGlobal;
    LPCSTR   pData;

    hGlobal=pDataObject->GetGlobalData(CF_OWNERDISPLAY); //Change to CF_TEXT if you are only working with text

    pData=(LPCSTR)GlobalLock(hGlobal);    
    ASSERT(pData!=NULL); 
                   
	pWnd->SetWindowText( pData );

    GlobalUnlock(hGlobal);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 

BOOL CDropTarget::Initialize(CWnd* wnd)
{
    BOOL success = Register(wnd);
    if(!success )
		return FALSE;
	return TRUE;

}
