/*****************************************************************************
File: DDEdit.cpp

  Purpose: Source file for the sample's Drag/Drop enabled Edit control
  
    Functions:  
    
      ----- CEdit derived class member functions -----
      
        CDDEdit::Initialize() - Registers this window with OLE Dll's to 
	enable drag/drop
        CDDEdit::OnCreate() - Handles WM_CREATE message                                  
        CDDEdit::OnLButtonDown() - Handles WM_LBUTTONDOWN message
        CDDEdit::GetDragSourceRect() - Gets the drag source rectangle stored 
	in the application view class
        CDDEdit::SetDragSourceRect() - Sets the drag source rectangle stored 
	in the application view class
	
	  ----- COleDropTarget derived class member functions -----
	  
	    COleEditDropTarget::COleEditDropTarget() - constructor 
	    COleEditDropTarget::~COleEditDropTarget() - destructor
	    COleEditDropTarget::OnDragEnter() - Called by the OLE Dll's when 
	    drag/drop cursor enters edit control
	    COleEditDropTarget::OnDragLeave() - Called by the OLE Dll's when
	    drag/drop cursor leaves edit control
	    COleEditDropTarget::OnDragOver() - Called by the OLE Dll's when 
	    drag/drop cursor is moved over edit control
	    COleEditDropTarget::OnDrop() -  Called by the OLE Dll's when Drop 
	    action occurs in edit control
	    
	      Development Team: Steve Horne
	      
		Written by Microsoft Product Support Services, Languages Developer Support
		Copyright (c) 1993 Microsoft Corporation. All rights reserved.
\****************************************************************************/

#include "stdafx.h"
#include "afxole.h"   

#include "ddedit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

[[ gsl::suppress(26409) ]]
IMPLEMENT_DYNCREATE(CDDEdit, CEdit)

/////////////////////////////////////////////////////////////////////////////
// CDDEdit

// ****
// This member calls the Register method of m_DropTarget to register
// this control as a drop target with the OLE dll's. We need to call 
// this method in the CFormView::OnInitialUpdate for controls created 
// by a dialog template since the OnCreate does not get called for 
// template created controls (this is because the windows controls 
// get created before the CWnd wrappers).
// *****
void CDDEdit::Initialize()
{
	m_bSetFocusSel = FALSE;
	m_bNoDrop = FALSE;
	m_hCursor = NULL;
	m_bTracking  = FALSE;
	m_DropTarget.Register(this);
}                                       

BEGIN_MESSAGE_MAP(CDDEdit, CEdit)
//{{AFX_MSG_MAP(CDDEdit)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDEdit message handlers

//*****
// OnCreate only gets called for dynamically created controls. For 
// controls created by a dialog template, Initialize() must be 
// called in the OnInitialUpdate method of the CFormView.
//*****  
int CDDEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bTracking  = FALSE;
	m_bNoDrop = FALSE;
	// Call base class method for default processing
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Register the COleDropTarget object with the OLE DLL's
	const	BOOL	success = m_DropTarget.Register(this);
	if (!success )
	{
		MessageBox("Ole Register Drop Target Failed");    
		return -1;
	}
	return 0;
}

void CDDEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bNoDrop)
	{
		CEdit::OnLButtonDown(nFlags, point);  
		SetSel(0, -1);
		return;
	}
	int nStart, nEnd;
	GetSel(nStart, nEnd);
	
	// Let Base class do default message processing
	CEdit::OnLButtonDown(nFlags, point);  
	
	if (nStart - nEnd == 0 || m_hCursor == NULL)
		return;

	CString sData, sDragData;
	GetWindowText(sData);
	sDragData = sData.Mid(nStart, nEnd - nStart);

	if (sDragData.GetLength() != 6)
		return;

	sDragData = "1301\t" + sDragData;
	sData.Delete(nStart, nEnd - nStart);
	
	// Create global memory for sharing drag drop text
	HGLOBAL hgData=GlobalAlloc(GPTR, sDragData.GetLength() + 1);   
	ASSERT(hgData!=NULL);
	
	// Lock global data (get pointer)
	LPSTR   lpData = (LPSTR)GlobalLock(hgData);
	ASSERT(lpData!=NULL);
	if (lpData)
		CopyMemory(lpData, sDragData.operator LPCTSTR(), sDragData.GetLength() + 1);
	
	if (!sDragData.GetLength())    // If no string, don't do dragdrop
        {
		GlobalUnlock(hgData);
		return;              
        }
	
	// Get the client rect. Pass this into DoDragDrop
	// so the dragdrop will not be initiated until text is dragged out
	// of the control
	RECT rClient;
	GetClientRect(&rClient);
	
	// Cache the data, and initiate DragDrop
	m_COleDataSource.CacheGlobalData(CF_TEXT, hgData);   
	
	// Set drag/drop source rectangle (screen coord) so we don't 
	// drop on the source rectangle
	
	const	DROPEFFECT dropEffect=m_COleDataSource.DoDragDrop(
		DROPEFFECT_COPY|DROPEFFECT_MOVE,(LPCRECT)&rClient);  
	
	// If the dropEffect was DROPEFFECT_MOVE,   
	// Clear the text from edit control (source)
	if ((dropEffect&DROPEFFECT_MOVE)==DROPEFFECT_MOVE)
		SetWindowText(sData);

        
	// We need to send WM_LBUTTONUP to control. WM_LBUTTONUP was sent to 
	// window that recieved the drag/drop, not the one that initiated it, 
	// so we simulate an WM_LBUTTONUP to the initiating window
	LPARAM lparam;
	
	// "Pack" lparam with x and y coordinates where lbuttondown originated
	lparam = point.y; 
	lparam = lparam<<16;
	lparam &= point.x;
	
	SendMessage(WM_LBUTTONUP, 0, lparam);        
	
	// Clear the Data Source's cache
	m_COleDataSource.Empty();
}  


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget

COleEditDropTarget::COleEditDropTarget() {}

COleEditDropTarget::~COleEditDropTarget() {}  

//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT COleEditDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* 
					   pDataObject, DWORD dwKeyState, CPoint point )
{       
	// Check if the control key was pressed          
//	if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY; // Copy the source text
//	else
//		return DROPEFFECT_MOVE; // Move the source text
} 

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void COleEditDropTarget::OnDragLeave(CWnd* pWnd)
{
	// Call base class implementation
	COleDropTarget:: OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT COleEditDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* 
					  pDataObject, DWORD dwKeyState, CPoint point )
{
	//if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY;
	//else
	//	return DROPEFFECT_MOVE;    
}

// 
// OnDrop is called by OLE dll's when item is dropped in a window
// that is REGISTERed with the OLE dll's
//
BOOL COleEditDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, 
				DROPEFFECT dropEffect, CPoint point )
{           
	HGLOBAL  hGlobal = NULL;
	LPCSTR   pData = NULL;
	
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
		CString strText = pData;
		CString str1301, strCode;

		strCode.Empty();str1301.Empty();
		str1301 = Parser(strText,"\t");



		if (str1301.Find("1301") != -1)
		{
 			strCode = Parser(strText, "\t");
		}
		else
		{
			for (int ii=0 ; ii<10 ; ii++)
			{
				str1301 = Parser(strText, "\t");
				str1301.TrimRight();
				
				if (strText.GetLength() <=0)
					break;
				
				if (str1301.Find("1301") != -1)
				{
					strCode = Parser(strText, "\t");
					break;
				}
				
			}
		}
		
		strCode.TrimRight();
		CString sendCode;
		
		
		if (!strCode.IsEmpty() && str1301.Find("1301") >= 0)
		{
			((CEdit*)pWnd)->SetWindowText(strCode);
//			((CEdit*)pWnd)->SendMessage(WM_KEYUP, strCode.GetAt(5), 0);
			((CEdit*)pWnd)->SendMessage(WM_CHAR, VK_RETURN);
			return TRUE;
		}
	}
	// Unlock memory
	GlobalUnlock(hGlobal);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// 


BOOL CDDEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor)
	{
		SetCursor(m_hCursor);
		return TRUE;
	}

	return CEdit::OnSetCursor(pWnd, nHitTest, message);
}

void CDDEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	int	nStart, nEnd;

	GetSel(nStart, nEnd);
	if (nStart - nEnd != 0)
	{
		const	CPoint posStart = PosFromChar(nStart);
		CPoint posEnd   = PosFromChar(nEnd);
		if (posEnd.x == -1)
		{
			CRect	rc;

			GetRect(&rc);
			posEnd.x = rc.right;
		}
		if (point.x >= posStart.x && point.x <= posEnd.x)
			m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		else
			m_hCursor = NULL;
	}
	else
		m_hCursor = NULL;

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	
	CEdit::OnMouseMove(nFlags, point);
}

LRESULT CDDEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	return CEdit::WindowProc(message, wParam, lParam);
}

LRESULT CDDEdit::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	return 0;
}

CString COleEditDropTarget::Parser(CString &srcstr, CString substr)
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

HBRUSH CDDEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/*
	if (IsWindowEnabled())
	{
		pDC->SetBkColor(CLR_BACKCOLOR);
		return m_backColor;
	}
	else
	{
		pDC->SetBkColor(CLR_NOBACKCOLOR);
		return m_NObackColor;
	}
	*/
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDDEdit::OnDestroy() 
{
	m_DropTarget.Revoke();

	CEdit::OnDestroy();
	// TODO: Add your message handler code here
	
}

void CDDEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();

	CEdit::OnLButtonUp(nFlags, point);
}

BOOL CDDEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CDDEdit::OnEditCopy()
{
	Copy();
}

void CDDEdit::OnEditPaste()
{
	Paste();
}
