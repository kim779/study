///////////////////////////////////////////////////////////////////////////
//	Implementation of the CCrystalEditView class, a part of the Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrystalEditView.h"

#include "EditCmd.h"
#include "EditReg.h"
#include "CrystalTextBuffer.h"
#include "EditReplaceDlg.h"

#ifndef __AFXPRIV_H__
#pragma message("Include <afxpriv.h> in your stdafx.h to avoid this message")
#include <afxpriv.h>
#endif

#ifndef __AFXOLE_H__
#pragma message("Include <afxole.h> in your stdafx.h to avoid this message")
#include <afxole.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DRAG_BORDER_X	5
#define DRAG_BORDER_Y	5

/////////////////////////////////////////////////////////////////////////////
// CEditDropTargetImpl class declaration

class CEditDropTargetImpl : public COleDropTarget
{
private:
	CCrystalEditView *m_pOwner;

public:
	CEditDropTargetImpl(CCrystalEditView *pOwner) { m_pOwner = pOwner; };

	virtual DROPEFFECT OnDragEnter(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd *pWnd);
	virtual DROPEFFECT OnDragOver(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd *pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragScroll(CWnd *pWnd, DWORD dwKeyState, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
// CCrystalEditView

IMPLEMENT_DYNCREATE(CCrystalEditView, CCrystalTextView)

CCrystalEditView::CCrystalEditView()
{
	AFX_ZERO_INIT_OBJECT(CCrystalTextView);
	m_bAutoIndent = TRUE;
}

CCrystalEditView::~CCrystalEditView()
{
}

BEGIN_MESSAGE_MAP(CCrystalEditView, CCrystalTextView)
	//{{AFX_MSG_MAP(CCrystalEditView)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_DELETE_BACK, OnEditDeleteBack)
	ON_COMMAND(ID_EDIT_UNTAB, OnEditUntab)
	ON_COMMAND(ID_EDIT_TAB, OnEditTab)
	ON_COMMAND(ID_EDIT_SWITCH_OVRMODE, OnEditSwitchOvrmode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SWITCH_OVRMODE, OnUpdateEditSwitchOvrmode)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_READ, OnUpdateIndicatorRead)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateIndicatorOvr)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INDICATOR_COL, OnUpdateIndicatorCol)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrystalEditView message handlers

void CCrystalEditView::OnEditPaste() 
{
	Paste();
}

void CCrystalEditView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TextInClipboard());
}

void CCrystalEditView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSelection());
}

void CCrystalEditView::OnEditCut() 
{
	Cut();
}

void CCrystalEditView::OnEditDelete() 
{
	if(!QueryEditable() || !m_pTextBuffer) return;

	CPoint ptSelStart, ptSelEnd;
	GetSelection(ptSelStart, ptSelEnd);
	if(ptSelStart == ptSelEnd)
	{
		if(ptSelEnd.x == GetLineLength(ptSelEnd.y))
		{
			if(ptSelEnd.y == GetLineCount() - 1) return;
			ptSelEnd.y ++;
			ptSelEnd.x = 0;
		}
		else 
		{
			// LeeYong
			//	2001.6.27
			//
			//	NOTES:
			//		Delete()가 발생시 한글을 처리하기위해 필요함.
			if(m_pTextBuffer->IsMBSLead(ptSelEnd.y, ptSelEnd.x) == TRUE)
				ptSelEnd.x += 2;
			else
				ptSelEnd.x += 1;
		}
	}

	CPoint ptCursorPos = ptSelStart;
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	SetAnchor(ptCursorPos);
	SetSelection(ptCursorPos, ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	m_pTextBuffer->DeleteText(this, ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, CE_ACTION_DELETE);
}

void CCrystalEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCrystalTextView::OnChar(nChar, nRepCnt, nFlags);

	if((::GetAsyncKeyState(VK_LBUTTON) & 0x8000) || (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)) return;

	BOOL bTranslated = FALSE;
	if(nChar == VK_RETURN)
	{
		if(m_bOvrMode)
		{
			CPoint ptCursorPos = GetCursorPos();
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			if(ptCursorPos.y < GetLineCount() - 1)
			{
				ptCursorPos.x = 0;
				ptCursorPos.y++;

				ASSERT_VALIDTEXTPOS(ptCursorPos);
				SetSelection(ptCursorPos, ptCursorPos);
				SetAnchor(ptCursorPos);
				SetCursorPos(ptCursorPos);
				EnsureVisible(ptCursorPos);
				return;
			}
		}

		m_pTextBuffer->BeginUndoGroup();

		if(QueryEditable() && m_pTextBuffer)
		{
			DeleteCurrentSelection();

			CPoint ptCursorPos = GetCursorPos();
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			const static TCHAR pszText[3] = _T("\r\n");

			int x, y;
			m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, pszText, y, x, CE_ACTION_TYPING);
			ptCursorPos.x = x;
			ptCursorPos.y = y;
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			SetSelection(ptCursorPos, ptCursorPos);
			SetAnchor(ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);
		}

		m_pTextBuffer->FlushUndoGroup(this);
		return;
	}

	if(nChar > 31)
	{
		if(QueryEditable() && m_pTextBuffer)
		{
			m_pTextBuffer->BeginUndoGroup(nChar != _T(' '));

			CPoint ptSelStart, ptSelEnd;
			GetSelection(ptSelStart, ptSelEnd);
			CPoint ptCursorPos;
			if(ptSelStart != ptSelEnd)
			{
				ptCursorPos = ptSelStart;
				DeleteCurrentSelection();
			}
			else
			{
				ptCursorPos = GetCursorPos();
				if(m_bOvrMode && ptCursorPos.x < GetLineLength(ptCursorPos.y))
				{
					BOOL	bHangul;
					int		iInc = 1;

					bHangul = m_pTextBuffer->IsMBSLead(ptCursorPos.y, ptCursorPos.x);
					if(bHangul)
						iInc ++;

					m_pTextBuffer->DeleteText(this, ptCursorPos.y, ptCursorPos.x, ptCursorPos.y, ptCursorPos.x+iInc, CE_ACTION_TYPING);
				}
			}

			ASSERT_VALIDTEXTPOS(ptCursorPos);

			char pszText[2];
			pszText[0] = (char) nChar;
			pszText[1] = 0;

			int x, y;
			USES_CONVERSION;
			m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, A2T(pszText), y, x, CE_ACTION_TYPING);
			ptCursorPos.x = x;
			ptCursorPos.y = y;
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			SetSelection(ptCursorPos, ptCursorPos);
			SetAnchor(ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);

			m_pTextBuffer->FlushUndoGroup(this);
		}
	}
}

// Added Support For "Disable Backspace At Start Of Line"
void CCrystalEditView::OnEditDeleteBack() 
{
	if(IsSelection())
	{
		OnEditDelete();
		return;
	}

	if(!QueryEditable() || !m_pTextBuffer) return;

	CPoint	ptCursorPos = GetCursorPos();
	CPoint	ptCurrentCursorPos = ptCursorPos;
	bool	bDeleted = false;

	if(!ptCursorPos.x)											// If At Start Of Line
	{
		if(!m_bDisableBSAtSOL)									// If DBSASOL Is Disabled
		{
			if(ptCursorPos.y > 0)								// If Previous Lines Available
			{
				ptCursorPos.y--;								// Decrement To Previous Line
				ptCursorPos.x = GetLineLength(ptCursorPos.y);	// Set Cursor To End Of Previous Line
				bDeleted = true;								// Set Deleted Flag
			}
		}
	}
	else														// If Caret Not At SOL
	{
		// LeeYong
		//	2001.6.27
		//
		ptCursorPos.x --;
		if(m_pTextBuffer->IsMBSTrail(ptCursorPos.y, ptCursorPos.x) == TRUE)
			ptCursorPos.x --;

		bDeleted = true;										// Set Deleted Flag
	}

//	if(ptCursorPos.x == 0)
//	{
//		if(ptCursorPos.y == 0) return;
//		ptCursorPos.y--;
//		ptCursorPos.x = GetLineLength(ptCursorPos.y);
//	}
//	else ptCursorPos.x--;

	ASSERT_VALIDTEXTPOS(ptCursorPos);
	SetAnchor(ptCursorPos);
	SetSelection(ptCursorPos, ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	if(bDeleted)
		m_pTextBuffer->DeleteText(this, ptCursorPos.y, ptCursorPos.x, ptCurrentCursorPos.y, ptCurrentCursorPos.x, CE_ACTION_BACKSPACE);
}

void CCrystalEditView::OnEditTab() 
{
	if(!QueryEditable() || !m_pTextBuffer) return;

	BOOL bTabify = FALSE;
	CPoint ptSelStart, ptSelEnd;
	if(IsSelection())
	{
		GetSelection(ptSelStart, ptSelEnd);
		bTabify = ptSelStart.y != ptSelEnd.y;
	}

	if(bTabify)
	{
		m_pTextBuffer->BeginUndoGroup();

		int nStartLine = ptSelStart.y;
		int nEndLine = ptSelEnd.y;
		ptSelStart.x = 0;
		if(ptSelEnd.x > 0)
		{
			if(ptSelEnd.y == GetLineCount()-1) ptSelEnd.x = GetLineLength(ptSelEnd.y);
			else
			{
				ptSelEnd.x = 0;
				ptSelEnd.y++;
			}
		}
		else nEndLine--;
		SetSelection(ptSelStart, ptSelEnd);
		SetCursorPos(ptSelEnd);
		EnsureVisible(ptSelEnd);

		// Shift selection to right
		m_bHorzScrollBarLocked = TRUE;
		static const TCHAR pszText[] = _T("\t");
		for(int L = nStartLine; L <= nEndLine; L++)
		{
			int x, y;
			m_pTextBuffer->InsertText(this, L, 0, pszText, y, x, CE_ACTION_INDENT);
		}
		m_bHorzScrollBarLocked = FALSE;
		RecalcHorzScrollBar();

		m_pTextBuffer->FlushUndoGroup(this);
		return;
	}

	if(m_bOvrMode)
	{
		CPoint ptCursorPos = GetCursorPos();
		ASSERT_VALIDTEXTPOS(ptCursorPos);

		int nLineLength = GetLineLength(ptCursorPos.y);
		LPCTSTR pszLineChars = GetLineChars(ptCursorPos.y);
		if(ptCursorPos.x < nLineLength)
		{
			int nTabSize = GetTabSize();
			int nChars = nTabSize - CalculateActualOffset(ptCursorPos.y, ptCursorPos.x) % nTabSize;
			ASSERT(nChars > 0 && nChars <= nTabSize);

			while(nChars > 0)
			{
				if(ptCursorPos.x == nLineLength) break;
				if(pszLineChars[ptCursorPos.x] == _T('\t'))
				{
					ptCursorPos.x++;
					break;
				}
				ptCursorPos.x++;
				nChars--;
			}
			ASSERT(ptCursorPos.x <= nLineLength);
			ASSERT_VALIDTEXTPOS(ptCursorPos);

			// LeeYong
			//	2001.6.27
			if(m_pTextBuffer->IsMBSTrail(ptCursorPos.y, ptCursorPos.x) == TRUE)
				ptCursorPos.x ++;

			SetSelection(ptCursorPos, ptCursorPos);
			SetAnchor(ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);
			return;
		}
	}

	m_pTextBuffer->BeginUndoGroup();

	DeleteCurrentSelection();

	CPoint ptCursorPos = GetCursorPos();
	ASSERT_VALIDTEXTPOS(ptCursorPos);

	static const TCHAR pszText[] = _T("\t");
	int x, y;
	m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, pszText, y, x, CE_ACTION_TYPING);
	ptCursorPos.x = x;
	ptCursorPos.y = y;
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	SetSelection(ptCursorPos, ptCursorPos);
	SetAnchor(ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	m_pTextBuffer->FlushUndoGroup(this);
}

void CCrystalEditView::OnEditUntab() 
{
	if(!QueryEditable() || !m_pTextBuffer) return;

	BOOL bTabify = FALSE;
	CPoint ptSelStart, ptSelEnd;
	if(IsSelection())
	{
		GetSelection(ptSelStart, ptSelEnd);
		bTabify = ptSelStart.y != ptSelEnd.y;
	}

	if(bTabify)
	{
		m_pTextBuffer->BeginUndoGroup();

		CPoint ptSelStart, ptSelEnd;
		GetSelection(ptSelStart, ptSelEnd);
		int nStartLine = ptSelStart.y;
		int nEndLine = ptSelEnd.y;
		ptSelStart.x = 0;
		if(ptSelEnd.x > 0)
		{
			if(ptSelEnd.y == GetLineCount()-1) ptSelEnd.x = GetLineLength(ptSelEnd.y);
			else
			{
				ptSelEnd.x = 0;
				ptSelEnd.y++;
			}
		}
		else nEndLine--;
		SetSelection(ptSelStart, ptSelEnd);
		SetCursorPos(ptSelEnd);
		EnsureVisible(ptSelEnd);

		// Shift selection to left
		m_bHorzScrollBarLocked = TRUE;
		for(int L = nStartLine; L <= nEndLine; L++)
		{
			int nLength = GetLineLength(L);
			if(nLength > 0)
			{
				LPCTSTR pszChars = GetLineChars(L);
				int nPos = 0, nOffset = 0;
				while(nPos < nLength)
				{
					if(pszChars[nPos] == _T(' '))
					{
						nPos++;
						if(++nOffset >= GetTabSize()) break;
					}
					else
					{
						if(pszChars[nPos] == _T('\t')) nPos++;
						break;
					}
				}

				if(nPos > 0) m_pTextBuffer->DeleteText(this, L, 0, L, nPos, CE_ACTION_INDENT);
			}
		}
		m_bHorzScrollBarLocked = FALSE;
		RecalcHorzScrollBar();

		m_pTextBuffer->FlushUndoGroup(this);
	}
	else
	{
		CPoint ptCursorPos = GetCursorPos();
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		if(ptCursorPos.x > 0)
		{
			int nTabSize = GetTabSize();
			int nOffset = CalculateActualOffset(ptCursorPos.y, ptCursorPos.x);
			int nNewOffset = nOffset / nTabSize * nTabSize;
			if(nOffset == nNewOffset && nNewOffset > 0) nNewOffset -= nTabSize;
			ASSERT(nNewOffset >= 0);

			LPCTSTR pszChars = GetLineChars(ptCursorPos.y);
			int nCurrentOffset = 0;
			int I = 0;
			while(nCurrentOffset < nNewOffset)
			{
				if(pszChars[I] == _T('\t')) nCurrentOffset = nCurrentOffset / nTabSize * nTabSize + nTabSize;
				else nCurrentOffset++;
				I++;
			}

			ASSERT(nCurrentOffset == nNewOffset);

			ptCursorPos.x = I;
			ASSERT_VALIDTEXTPOS(ptCursorPos);

			// LeeYong
			//	2001.6.27
			if(m_pTextBuffer->IsMBSTrail(ptCursorPos.y, ptCursorPos.x) == TRUE)
				ptCursorPos.x --;

			SetSelection(ptCursorPos, ptCursorPos);
			SetAnchor(ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);
		}
	}
}

void CCrystalEditView::OnUpdateIndicatorCol(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CCrystalEditView::OnUpdateIndicatorOvr(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bOvrMode);
}

void CCrystalEditView::OnUpdateIndicatorRead(CCmdUI* pCmdUI)
{
	if(!m_pTextBuffer) pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(m_pTextBuffer->GetReadOnly());
}

void CCrystalEditView::OnEditSwitchOvrmode() 
{
	m_bOvrMode = !m_bOvrMode;
}

void CCrystalEditView::OnUpdateEditSwitchOvrmode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bOvrMode ? 1 : 0);
}

DROPEFFECT CEditDropTargetImpl::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(!pDataObject->IsDataAvailable(CF_TEXT))
	{
		m_pOwner->HideDropIndicator();
		return DROPEFFECT_NONE;
	}
	m_pOwner->ShowDropIndicator(point);
	if(dwKeyState & MK_CONTROL) return DROPEFFECT_COPY;
	return DROPEFFECT_MOVE;
}

void CEditDropTargetImpl::OnDragLeave(CWnd* pWnd)
{
	m_pOwner->HideDropIndicator();
}

DROPEFFECT CEditDropTargetImpl::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
//	if(! pDataObject->IsDataAvailable(CF_TEXT))
//	{
//		m_pOwner->HideDropIndicator();
//		return DROPEFFECT_NONE;
//	}

	bool bDataSupported = false;

	if(!m_pOwner || !m_pOwner->QueryEditable() || m_pOwner->GetDisableDragAndDrop())	// Or Drag And Drop Disabled
	{
		m_pOwner->HideDropIndicator();		// Hide Drop Caret
		return DROPEFFECT_NONE;				// Return DE_NONE
	}
//	if(pDataObject->IsDataAvailable(CF_TEXT) || pDataObject->IsDataAvailable(xxx) || pDataObject->IsDataAvailable(yyy))
	if(pDataObject->IsDataAvailable(CF_TEXT)) bDataSupported = true;	// If Text Available
	if(!bDataSupported)						// If No Supported Formats Available
	{
		m_pOwner->HideDropIndicator();		// Hide Drop Caret
		return DROPEFFECT_NONE;				// Return DE_NONE
	}
	m_pOwner->ShowDropIndicator(point);
	if(dwKeyState & MK_CONTROL) return DROPEFFECT_COPY;
	return DROPEFFECT_MOVE;
}

BOOL CEditDropTargetImpl::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	// ( m_pOwner->GetDisableDragAndDrop() )			// Or Drag And Drop Disabled
	bool bDataSupported = false;

	m_pOwner->HideDropIndicator();						// Hide Drop Caret
	if(!m_pOwner || !m_pOwner->QueryEditable() || m_pOwner->GetDisableDragAndDrop()) return DROPEFFECT_NONE;
//	if(pDataObject->IsDataAvailable(CF_TEXT) || pDataObject->IsDataAvailable(xxx) || pDataObject->IsDataAvailable(yyy))
	if(pDataObject->IsDataAvailable(CF_TEXT)) bDataSupported = true;	// If Text Available
	if(!bDataSupported) return DROPEFFECT_NONE;							// If No Supported Formats Available

	return m_pOwner->DoDropText(pDataObject, point);	// Return Result Of Drop
}

DROPEFFECT CEditDropTargetImpl::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	ASSERT(m_pOwner == pWnd);
	m_pOwner->DoDragScroll(point);

	if(dwKeyState & MK_CONTROL) return DROPEFFECT_COPY;
	return DROPEFFECT_MOVE;
}

void CCrystalEditView::DoDragScroll(const CPoint &point)
{
	CRect rcClientRect;
	GetClientRect(rcClientRect);
	if(point.y < rcClientRect.top + DRAG_BORDER_Y)
	{
		HideDropIndicator();
		ScrollUp();
		UpdateWindow();
		ShowDropIndicator(point);
		return;
	}
	if(point.y >= rcClientRect.bottom - DRAG_BORDER_Y)
	{
		HideDropIndicator();
		ScrollDown();
		UpdateWindow();
		ShowDropIndicator(point);
		return;
	}
	if(point.x < rcClientRect.left + GetMarginWidth() + DRAG_BORDER_X)
	{
		HideDropIndicator();
		ScrollLeft();
		UpdateWindow();
		ShowDropIndicator(point);
		return;
	}
	if(point.x >= rcClientRect.right - DRAG_BORDER_X)
	{
		HideDropIndicator();
		ScrollRight();
		UpdateWindow();
		ShowDropIndicator(point);
		return;
	}
}

BOOL CCrystalEditView::DoDropText(COleDataObject *pDataObject, const CPoint &ptClient)
{
	HGLOBAL hData = pDataObject->GetGlobalData(CF_TEXT);
	if(hData == NULL) return FALSE;

	CPoint ptDropPos = ClientToText(ptClient);
	if(IsDraggingText() && IsInsideSelection(ptDropPos))
	{
		SetAnchor(ptDropPos);
		SetSelection(ptDropPos, ptDropPos);
		SetCursorPos(ptDropPos);
		EnsureVisible(ptDropPos);
		return FALSE;
	}

	LPSTR pszText = (LPSTR) ::GlobalLock(hData);
	if(pszText == NULL) return FALSE;

	int x, y;
	USES_CONVERSION;
	m_pTextBuffer->InsertText(this, ptDropPos.y, ptDropPos.x, A2T(pszText), y, x, CE_ACTION_DRAGDROP);
	CPoint ptCurPos(x, y);
	ASSERT_VALIDTEXTPOS(ptCurPos);
	SetAnchor(ptDropPos);
	SetSelection(ptDropPos, ptCurPos);
	SetCursorPos(ptCurPos);
	EnsureVisible(ptCurPos);

	::GlobalUnlock(hData);
	return TRUE;
}

int CCrystalEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CCrystalTextView::OnCreate(lpCreateStruct) == -1) return -1;
	
	ASSERT(m_pDropTarget == NULL);
	m_pDropTarget = new CEditDropTargetImpl(this);
	if(!m_pDropTarget->Register(this))
	{
		TRACE0("Warning: Unable to register drop target for CCrystalEditView.\n");
		delete m_pDropTarget;
		m_pDropTarget = NULL;
	}

	return 0;
}

void CCrystalEditView::OnDestroy() 
{
	if(m_pDropTarget)
	{
		m_pDropTarget->Revoke();
		delete m_pDropTarget;
		m_pDropTarget = NULL;
	}

	CCrystalTextView::OnDestroy();
}

void CCrystalEditView::OnDropSource(DROPEFFECT de)
{
	if(!IsDraggingText()) return;

	ASSERT_VALIDTEXTPOS(m_ptDraggedTextBegin);
	ASSERT_VALIDTEXTPOS(m_ptDraggedTextEnd);

	if(de == DROPEFFECT_MOVE)
		m_pTextBuffer->DeleteText(this, m_ptDraggedTextBegin.y, m_ptDraggedTextBegin.x, m_ptDraggedTextEnd.y, m_ptDraggedTextEnd.x, CE_ACTION_DRAGDROP);
}

void CCrystalEditView::OnEditReplace() 
{
	if(!QueryEditable()) return;

	CWinApp *pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	CEditReplaceDlg dlg(this);

	// Take search parameters from registry
	dlg.m_bMatchCase = pApp->GetProfileInt(REG_REPLACE_SUBKEY, REG_MATCH_CASE, FALSE);
	dlg.m_bWholeWord = pApp->GetProfileInt(REG_REPLACE_SUBKEY, REG_WHOLE_WORD, FALSE);
	dlg.m_sText = pApp->GetProfileString(REG_REPLACE_SUBKEY, REG_FIND_WHAT, _T(""));
	dlg.m_sNewText = pApp->GetProfileString(REG_REPLACE_SUBKEY, REG_REPLACE_WITH, _T(""));

	if(IsSelection())
	{
		GetSelection(m_ptSavedSelStart, m_ptSavedSelEnd);
		m_bSelectionPushed = TRUE;

		dlg.m_nScope = 0;	// Replace in current selection
		dlg.m_ptCurrentPos = m_ptSavedSelStart;
		dlg.m_bEnableScopeSelection = TRUE;
		dlg.m_ptBlockBegin = m_ptSavedSelStart;
		dlg.m_ptBlockEnd = m_ptSavedSelEnd;
	}
	else
	{
		dlg.m_nScope = 1;	// Replace in whole text
		dlg.m_ptCurrentPos = GetCursorPos();
		dlg.m_bEnableScopeSelection = FALSE;
	}

	// Execute Replace dialog
	m_bShowInactiveSelection = TRUE;
	dlg.DoModal();
	m_bShowInactiveSelection = FALSE;

	// Restore selection
	if(m_bSelectionPushed)
	{
		SetSelection(m_ptSavedSelStart, m_ptSavedSelEnd);
		m_bSelectionPushed = FALSE;
	}

	// Save search parameters to registry
	pApp->WriteProfileInt(REG_REPLACE_SUBKEY, REG_MATCH_CASE, dlg.m_bMatchCase);
	pApp->WriteProfileInt(REG_REPLACE_SUBKEY, REG_WHOLE_WORD, dlg.m_bWholeWord);
	pApp->WriteProfileString(REG_REPLACE_SUBKEY, REG_FIND_WHAT, dlg.m_sText);
	pApp->WriteProfileString(REG_REPLACE_SUBKEY, REG_REPLACE_WITH, dlg.m_sNewText);
}

void CCrystalEditView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	BOOL bCanUndo = m_pTextBuffer != NULL && m_pTextBuffer->CanUndo();
	pCmdUI->Enable(bCanUndo);

	// Since we need text only for menus...
	if(pCmdUI->m_pMenu)
	{
		// Tune up 'resource handle'
		HINSTANCE hOldResHandle = AfxGetResourceHandle();
		AfxSetResourceHandle(GetResourceHandle());

		CString menu;
		if(bCanUndo)
		{
			// Format menu item text using the provided item description
			CString desc;
			m_pTextBuffer->GetUndoDescription(desc);
			menu.Format(IDS_MENU_UNDO_FORMAT, desc);
		}
		else menu.LoadString(IDS_MENU_UNDO_DEFAULT);	// Just load default menu item text

		AfxSetResourceHandle(hOldResHandle);	// Restore original handle
		pCmdUI->SetText(menu);	// Set menu item text
	}
}

void CCrystalEditView::OnEditUndo() 
{
	if(m_pTextBuffer && m_pTextBuffer->CanUndo())
	{
		CPoint ptCursorPos;
		if(m_pTextBuffer->Undo(ptCursorPos))
		{
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			SetAnchor(ptCursorPos);
			SetSelection(ptCursorPos, ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);

			// LeeYong, 2001.6.29
			ResetScriptError();
		}
	}
}

void CCrystalEditView::OnEditRedo() 
{
	if(m_pTextBuffer && m_pTextBuffer->CanRedo())
	{
		CPoint ptCursorPos;
		if(m_pTextBuffer->Redo(ptCursorPos))
		{
			ASSERT_VALIDTEXTPOS(ptCursorPos);
			SetAnchor(ptCursorPos);
			SetSelection(ptCursorPos, ptCursorPos);
			SetCursorPos(ptCursorPos);
			EnsureVisible(ptCursorPos);

			// LeeYong, 2001.6.29
			ResetScriptError();
		}
	}
}

void CCrystalEditView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	BOOL bCanRedo = m_pTextBuffer && m_pTextBuffer->CanRedo();
	pCmdUI->Enable(bCanRedo);

	// Since we need text only for menus...
	if(pCmdUI->m_pMenu)
	{
		// Tune up 'resource handle'
		HINSTANCE hOldResHandle = AfxGetResourceHandle();
		AfxSetResourceHandle(GetResourceHandle());

		CString menu;
		if(bCanRedo)
		{
			// Format menu item text using the provided item description
			CString desc;
			m_pTextBuffer->GetRedoDescription(desc);
			menu.Format(IDS_MENU_REDO_FORMAT, desc);
		}
		else menu.LoadString(IDS_MENU_REDO_DEFAULT);	// Just load default menu item text

		AfxSetResourceHandle(hOldResHandle);	// Restore original handle
		pCmdUI->SetText(menu);	// Set menu item text
	}
}

void CCrystalEditView::OnEditOperation(int nAction, LPCTSTR pszText)
{
	if(m_bAutoIndent)
	{
		// Analyse last action...
		if(nAction == CE_ACTION_TYPING && _tcscmp(pszText, _T("\r\n")) == 0 && !m_bOvrMode)
		{
			// Enter stroke!
			CPoint ptCursorPos = GetCursorPos();
			ASSERT(ptCursorPos.y > 0);

			// Take indentation from the previos line
			int nLength = m_pTextBuffer->GetLineLength(ptCursorPos.y - 1);
			LPCTSTR pszLineChars = m_pTextBuffer->GetLineChars(ptCursorPos.y - 1);
			int nPos = 0;

			// LeeYong
			//	2001.6.27
			//
			// while(nPos < nLength && isspace(pszLineChars[nPos])) nPos++;
			while(nPos < nLength && ::IsDBCSLeadByte(pszLineChars[nPos]) == FALSE && isspace(pszLineChars[nPos])) nPos++;

			if(nPos > 0)
			{
				// Insert part of the previos line
				TCHAR *pszInsertStr = (TCHAR *) _alloca(sizeof(TCHAR) * (nLength + 1));
				_tcsncpy(pszInsertStr, pszLineChars, nPos);
				pszInsertStr[nPos] = 0;

				int x, y;

				// LeeYong, 2001.6.29
				m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, pszInsertStr, y, x, CE_ACTION_AUTOINDENT);
//				m_pTextBuffer->InsertText(NULL, ptCursorPos.y, ptCursorPos.x, pszInsertStr, y, x, CE_ACTION_AUTOINDENT);
				CPoint pt(x, y);
				SetCursorPos(pt);
				SetSelection(pt, pt);
				SetAnchor(pt);
				EnsureVisible(pt);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCrystalEditView User Function

void CCrystalEditView::ResetView()
{
	m_bAutoIndent = TRUE;
	m_bOvrMode = FALSE;
	CCrystalTextView::ResetView();
}

BOOL CCrystalEditView::QueryEditable()
{
	if(m_pTextBuffer == NULL) return FALSE;
	return !m_pTextBuffer->GetReadOnly();
}

BOOL CCrystalEditView::DeleteCurrentSelection()
{
	if(IsSelection())
	{
		CPoint ptSelStart, ptSelEnd;
		GetSelection(ptSelStart, ptSelEnd);

		CPoint ptCursorPos = ptSelStart;
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		SetAnchor(ptCursorPos);
		SetSelection(ptCursorPos, ptCursorPos);
		SetCursorPos(ptCursorPos);
		EnsureVisible(ptCursorPos);

		m_pTextBuffer->DeleteText(this, ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, CE_ACTION_DELSEL);
		return TRUE;
	}
	return FALSE;
}

void CCrystalEditView::Paste()
{
	if(!QueryEditable()) return;
	if(m_pTextBuffer == NULL) return;

	m_pTextBuffer->BeginUndoGroup();

	DeleteCurrentSelection();

	CString text;
	if(GetFromClipboard(text))
	{
		CPoint ptCursorPos = GetCursorPos();
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		int x, y;
		m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, text, y, x, CE_ACTION_PASTE);
		ptCursorPos.x = x;
		ptCursorPos.y = y;
		ASSERT_VALIDTEXTPOS(ptCursorPos);
		SetAnchor(ptCursorPos);
		SetSelection(ptCursorPos, ptCursorPos);
		SetCursorPos(ptCursorPos);
		EnsureVisible(ptCursorPos);
	}

	m_pTextBuffer->FlushUndoGroup(this);
}

void CCrystalEditView::Cut()
{
	if(!QueryEditable()) return;
	if(!m_pTextBuffer) return;
	if(!IsSelection()) return;

	CPoint ptSelStart, ptSelEnd;
	GetSelection(ptSelStart, ptSelEnd);
	CString text;
	GetText(ptSelStart, ptSelEnd, text);
	PutToClipboard(text);

	CPoint ptCursorPos = ptSelStart;
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	SetAnchor(ptCursorPos);
	SetSelection(ptCursorPos, ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	m_pTextBuffer->DeleteText(this, ptSelStart.y, ptSelStart.x, ptSelEnd.y, ptSelEnd.x, CE_ACTION_CUT);
}

void CCrystalEditView::SetDisableBSAtSOL(BOOL bDisableBSAtSOL)
{
	m_bDisableBSAtSOL = bDisableBSAtSOL;
}

void CCrystalEditView::ShowDropIndicator(const CPoint &point)
{
	if(!m_bDropPosVisible)
	{
		HideCursor();
		m_ptSavedCaretPos = GetCursorPos();
		m_bDropPosVisible = TRUE;
		::CreateCaret(m_hWnd, (HBITMAP) 1, 2, GetLineHeight());
	}
	m_ptDropPos = ClientToText(point);
	if(m_ptDropPos.x >= m_nOffsetChar)
	{
		SetCaretPos(TextToClient(m_ptDropPos));
		ShowCaret();
	}
	else HideCaret();
}

void CCrystalEditView::HideDropIndicator()
{
	if(m_bDropPosVisible)
	{
		SetCursorPos(m_ptSavedCaretPos);
		ShowCursor();
		m_bDropPosVisible = FALSE;
	}
}

DROPEFFECT CCrystalEditView::GetDropEffect()
{
	return DROPEFFECT_COPY | DROPEFFECT_MOVE;
}

void CCrystalEditView::UpdateView(CCrystalTextView *pSource, CUpdateContext *pContext, DWORD dwFlags, int nLineIndex)
{
	CCrystalTextView::UpdateView(pSource, pContext, dwFlags, nLineIndex);

	if(m_bSelectionPushed && pContext != NULL)
	{
		pContext->RecalcPoint(m_ptSavedSelStart);
		pContext->RecalcPoint(m_ptSavedSelEnd);
		ASSERT_VALIDTEXTPOS(m_ptSavedSelStart);
		ASSERT_VALIDTEXTPOS(m_ptSavedSelEnd);
	}
}

BOOL CCrystalEditView::ReplaceSelection(LPCTSTR pszNewText)
{
	ASSERT(pszNewText != NULL);
	if(!IsSelection()) return FALSE;

	DeleteCurrentSelection();

	CPoint ptCursorPos = GetCursorPos();
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	int x, y;
	m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, pszNewText, y, x, CE_ACTION_REPLACE);
	CPoint ptEndOfBlock = CPoint(x, y);
	ASSERT_VALIDTEXTPOS(ptCursorPos);
	ASSERT_VALIDTEXTPOS(ptEndOfBlock);
	SetAnchor(ptEndOfBlock);
	SetSelection(ptCursorPos, ptEndOfBlock);
	SetCursorPos(ptEndOfBlock);
	EnsureVisible(ptEndOfBlock);

	return TRUE;
}

// LeeYong, 2001.6.29
void
CCrystalEditView::DisplayIMECompStr(LPSTR lpCompStr, LPSTR lpCompAttr)
{
	int		iSLen		= strlen(lpCompStr);
	CPoint	ptCursorPos	= GetCursorPos();
	int		x, y;

	ASSERT_VALIDTEXTPOS(ptCursorPos);

	x = ptCursorPos.x;
	y = ptCursorPos.y;

	if(m_bHangulEndComposition && !m_bOvrMode)
		m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, lpCompStr, y, x, CE_ACTION_TYPING, FALSE);
	else
	{
		int		iLineLen;

		iLineLen = GetLineLength(ptCursorPos.y);
		if(ptCursorPos.x < iLineLen)
		{
			int		iInc;

			iInc = iLineLen - ptCursorPos.x;
			if(iInc > 2)
			{
				if(m_bOvrMode && !m_pTextBuffer->IsMBSLead(ptCursorPos.y, ptCursorPos.x) && m_pTextBuffer->IsMBSLead(ptCursorPos.y, ptCursorPos.x+1))
					iInc = 3;
				else
					iInc = 2;
			}
			m_pTextBuffer->DeleteText(this, ptCursorPos.y, ptCursorPos.x, ptCursorPos.y, ptCursorPos.x + iInc, CE_ACTION_TYPING, FALSE);
		}

		if(*lpCompStr != NULL)
			m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, lpCompStr, y, x, CE_ACTION_TYPING, FALSE);
	}

	if(m_bHangulEndComposition)
		m_bHangulEndComposition = FALSE;

	ptCursorPos.x = x - iSLen;
	ptCursorPos.y = y;

	ASSERT_VALIDTEXTPOS(ptCursorPos);

	SetSelection(ptCursorPos, ptCursorPos);
	SetAnchor(ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);
}

void
CCrystalEditView::DisplayIMERestStr(LPSTR lpRestStr)
{
	int		iSLen		= strlen(lpRestStr);
	CPoint	ptCursorPos	= GetCursorPos();
	char	TmpBuf[3];
	int		x, y;

	ASSERT_VALIDTEXTPOS(ptCursorPos);

	x = ptCursorPos.x;
	y = ptCursorPos.y;

	strncpy(TmpBuf, lpRestStr, 2);
	TmpBuf[2] = NULL;

	m_pTextBuffer->DeleteText(this, ptCursorPos.y, ptCursorPos.x, ptCursorPos.y, ptCursorPos.x + 2, CE_ACTION_TYPING, FALSE);

	m_pTextBuffer->BeginUndoGroup();

	m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, TmpBuf, y, x, CE_ACTION_TYPING, TRUE);

	ptCursorPos.x = x;
	ptCursorPos.y = y;

	if(iSLen > 2)
	{
		m_pTextBuffer->InsertText(this, ptCursorPos.y, ptCursorPos.x, lpRestStr + 2, y, x, CE_ACTION_TYPING, TRUE);
		ptCursorPos.x = x;
		ptCursorPos.y = y;
	}

	ASSERT_VALIDTEXTPOS(ptCursorPos);

	SetSelection(ptCursorPos, ptCursorPos);
	SetAnchor(ptCursorPos);
	SetCursorPos(ptCursorPos);
	EnsureVisible(ptCursorPos);

	m_pTextBuffer->FlushUndoGroup(this);

	m_bHangulEndComposition = TRUE;
}
