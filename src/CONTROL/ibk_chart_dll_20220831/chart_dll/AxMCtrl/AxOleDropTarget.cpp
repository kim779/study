// AxOleDropTarget.cpp: implementation of the CAxOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AxMCtrl.h"
#include "AxOleDropTarget.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxOleDropTarget::CAxOleDropTarget()
{

}

CAxOleDropTarget::~CAxOleDropTarget()
{

}

//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CAxOleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
					   pDataObject, DWORD dwKeyState, CPoint point )
{       
	// Check if the control key was pressed          
//	if((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY; // Copy the source text
//	else
//		return DROPEFFECT_MOVE; // Move the source text
} 

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void CAxOleDropTarget::OnDragLeave(CWnd* pWnd)
{
	// Call base class implementation
	COleDropTarget:: OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CAxOleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
					  pDataObject, DWORD dwKeyState, CPoint point )
{
	//if((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY;
	//else
	//	return DROPEFFECT_MOVE;    
}

// 
// OnDrop is called by OLE dll's when item is dropped in a window
// that is REGISTERed with the OLE dll's
//
BOOL CAxOleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
				DROPEFFECT dropEffect, CPoint point )
{           
	HGLOBAL  hGlobal;
	LPCSTR   pData;
	
	// Get text data from COleDataObject
	hGlobal=pDataObject->GetGlobalData(CF_TEXT);
	
	// Get pointer to data
	pData = (LPCSTR)GlobalLock(hGlobal);    
	ASSERT(pData!=NULL); 
	
	if (dropEffect == DROPEFFECT_MOVE)
	{
		// Set text in dropped window
		((CEdit*)pWnd)->SetWindowText(pData);
		
	}
	else if (dropEffect == DROPEFFECT_COPY)
	{
		CString sDragData(pData);
		CString sSym = Parser(sDragData, "\t");
		if (sSym == "1301" && sDragData.GetLength() == 6)
		{
			CString sCode;
			((CEdit*)pWnd)->GetWindowText(sCode);
			if (sCode != sDragData)
			{
				((CEdit*)pWnd)->SetWindowText(sDragData);
				//((CEdit*)pWnd)->SendMessage(WM_KEYUP, sDragData.GetAt(5), 0);
				((CEdit*)pWnd)->SendMessage(WM_CHAR, VK_RETURN);
			}
		}
		/*CString sData;
		((CEdit*)pWnd)->GetWindowText(sData);
		sData += pData;
		((CEdit*)pWnd)->SetWindowText(sData);*/
	}
	// Unlock memory
	GlobalUnlock(hGlobal);
	
	return TRUE;
}

CString CAxOleDropTarget::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}
