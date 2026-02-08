// ExMaskEdit.cpp: implementation of the CExMaskEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExMaskEdit.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; 
#endif

IMPLEMENT_DYNAMIC(CExMaskEdit, CEdit)

BEGIN_MESSAGE_MAP(CExMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CExButton)
	ON_MESSAGE_VOID(WM_CUT, MaskCut)
	ON_MESSAGE_VOID(WM_PASTE, MaskPaste)
	ON_MESSAGE_VOID(WM_CLEAR, MaskClear)
	ON_MESSAGE_VOID(WM_UNDO, MaskUndo)
	ON_MESSAGE_VOID(WM_COPY, MaskCopy)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CExMaskEdit::CExMaskEdit(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderMaskEdit(this, bUseDefSkin);
}

CExMaskEdit::~CExMaskEdit()
{
	delete m_pLoader;
}

long CExMaskEdit::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

void CExMaskEdit::SetUseMask(BOOL bUseMask)
{
	m_pLoader->SetUseMask(bUseMask);
}

BOOL CExMaskEdit::CanUseMask()
{
	return m_pLoader->CanUseMask();
}

void CExMaskEdit::SetOverType(BOOL bOverType) 
{
	m_pLoader->SetOverType(bOverType);
}

BOOL CExMaskEdit::CanOverType() 
{
	return m_pLoader->CanOverType();
}

BOOL CExMaskEdit::PosInRange(int nPos) 
{
	return m_pLoader->PosInRange(nPos);
}

TCHAR CExMaskEdit::GetPromptChar()
{
	return m_pLoader->GetPromptChar();
}

void CExMaskEdit::SetPromptChar(TCHAR ch)
{
	m_pLoader->SetPromptChar(ch);
}

void CExMaskEdit::MaskCut()
{
	m_pLoader->MaskCut();
}

void CExMaskEdit::MaskCopy()
{
	m_pLoader->MaskCopy();
}

void CExMaskEdit::MaskPaste()
{
	m_pLoader->MaskPaste();
}

void CExMaskEdit::MaskClear()
{
	m_pLoader->MaskClear();
}

void CExMaskEdit::MaskUndo()
{
	m_pLoader->MaskUndo();
}

void CExMaskEdit::MaskSelectAll()
{
	m_pLoader->MaskSelectAll();
}

BOOL CExMaskEdit::IsModified() 
{
	return m_pLoader->IsModified();
}

//			0                 Numeric (0-9)
//			9                 Numeric (0-9) or space (' ')
//			#                 Numeric (0-9) or space (' ') or ('+') or ('-')
//			L                 Alpha (a-Z) 
//			?                 Alpha (a-Z) or space (' ') 
//			A                 Alpha numeric (0-9 and a-Z)
//			a                 Alpha numeric (0-9 and a-Z) or space (' ') 
//			&                 All print character only 
//			H                 Hex digit (0-9 and A-F)
//			X                 Hex digit (0-9 and A-F) and space (' ')
//			>                 Forces characters to upper case (A-Z)
//			<                 Forces characters to lower case (a-z)</pre>
void CExMaskEdit::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault/* = NULL*/)
{
	m_pLoader->SetEditMask(lpszMask, lpszLiteral, lpszDefault);
}

BOOL CExMaskEdit::CheckChar(UINT& nChar, BOOL bBeep/* = TRUE*/)
{
	return m_pLoader->CheckChar(nChar, bBeep);
}

BOOL CExMaskEdit::ProcessMask(UINT& nChar, int nEndPos)
{
	return m_pLoader->ProcessMask(nChar, nEndPos);
}

BOOL CExMaskEdit::PreTranslateMessage(MSG* pMsg)
{
	return CEdit::PreTranslateMessage(pMsg);
//	return m_pLoader->PreTranslateMessage(pMsg);
}

BOOL CExMaskEdit::CorrectPosition(int& nPos, BOOL bForward/* = TRUE*/, BOOL bBeep/* = TRUE*/) 
{
	return m_pLoader->CorrectPosition(nPos, bForward, bBeep);
}

void CExMaskEdit::DeleteCharAt(int nPos)
{
	m_pLoader->DeleteCharAt(nPos);
}

void CExMaskEdit::InsertCharAt(int nPos, TCHAR nChar)
{
	m_pLoader->InsertCharAt(nPos, nChar);
}

BOOL CExMaskEdit::CopyToClipboard(LPCTSTR lpszText)
{
	return m_pLoader->CopyToClipboard(lpszText);
}

CString CExMaskEdit::GetMaskedText(int nPos/* = 0*/)
{
	return m_pLoader->GetMaskedText(nPos);
}

BOOL CExMaskEdit::SelectionMade() 
{
	return m_pLoader->SelectionMade();
}

void CExMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_pLoader->OnChar(nChar, nRepCnt, nFlags);
}

void CExMaskEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pLoader->OnLButtonDown(nFlags, point);
}

void CExMaskEdit::OnSetFocus(CWnd* pOldWnd)
{
	m_pLoader->OnSetFocus(pOldWnd);
}

void CExMaskEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_pLoader->OnLButtonDblClk(nFlags, point);
}

void CExMaskEdit::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_pLoader->OnUpdateEditUndo(pCmdUI);
}

BOOL CExMaskEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return m_pLoader->OnCommand(wParam, lParam);
}

void CExMaskEdit::OnPaint() 
{
	Default();
	m_pLoader->OnPaint();
}

void CExMaskEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
#ifndef _DEBUG
	SetPermanentHandle(m_hWnd, this);
#endif
}
