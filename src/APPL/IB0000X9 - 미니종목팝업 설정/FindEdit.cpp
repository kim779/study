// FindEdit.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000X1.h"
#include "FindEdit.h"
#include "Common.h"
#include "imm.h"

#pragma comment(lib, "imm32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindEdit


BEGIN_MESSAGE_MAP(CFindEdit, CEdit)
	//{{AFX_MSG_MAP(CFindEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindEdit message handlers

void CFindEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	if (pNewWnd != m_pList.get())
	{
		ShowWindow(SW_HIDE);
		m_pList->ShowWindow(SW_HIDE);
	}
}

LRESULT CFindEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WM_IME_STARTCOMPOSITION: m_bComposition = TRUE;  break;
	case WM_IME_ENDCOMPOSITION  : m_bComposition = FALSE; break;
	default:
		break;
	}

	if (message==WM_IME_COMPOSITION)
	{
		//TRACE("Composition [%08X]\n", lParam);
		
		//if (lParam&GCS_DELTASTART) m_bImeNoRemove = TRUE;

		if ( (lParam&CS_INSERTCHAR) || (lParam&GCS_DELTASTART))
		{
			if (m_bImeNoInsert)
			{
				m_bImeNoInsert = FALSE;
				m_bImeNoRemove = TRUE;
			}
			else
			{
				int nLen{}, tLen{};
				char buff[128]{};
				HIMC hImc = ImmGetContext(m_hWnd);
				CString text;
				GetWindowText(text);
				tLen = text.GetLength();
				nLen = ImmGetCompositionString(hImc, GCS_COMPSTR, buff, sizeof(buff)); buff[nLen] = 0;
				//TRACE("[%s][%s] -> %s\n", text, buff, (m_bImeNoRemove) ? (text+buff) : (text.Left(tLen-2)+buff) );
				ImmReleaseContext(m_hWnd, hImc);

				text = (m_bImeNoRemove) ? (text+buff) : (text.Left(tLen-2)+buff);
				m_bImeNoRemove = FALSE;
				GetParent()->SendMessage(WX_EDIT_KEYPRESS, 0, (LPARAM)(LPCSTR)text);
			}
		}
		else m_bImeNoInsert = TRUE;
	}

	return CEdit::DefWindowProc(message, wParam, lParam);;
}

void CFindEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	if (!m_bComposition)
	{
		CString text;
		GetWindowText(text);
		GetParent()->SendMessage(WX_EDIT_KEYPRESS, 0, (LPARAM)(LPCSTR)text);
	}
}

void CFindEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar==VK_UP || nChar==VK_DOWN || nChar==VK_PRIOR || nChar==VK_NEXT)
	{
		if (m_pList)
		{
			m_pList->SelectKey(nChar);
		}
		return;
	}
	else if (nChar==VK_RETURN)
	{
		CString code, name;
		m_pList->GetSelection(code, name);
		const CWnd *pParent = GetParent();
		if (pParent)
			pParent->SendMessage(WX_CHANGE_CODE, (WPARAM)(LPCSTR)code, (LPARAM)(LPCSTR)name);
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CFindEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//m_pList = new CListPopup(this);

	m_pList = std::make_unique<CListPopup>(this);

	if (m_pList)
	{
		const BOOL bRet = m_pList->CreateEx(WS_EX_TOPMOST, ::AfxRegisterWndClass(0), NULL, WS_POPUP, CRect(0,0,0,0), 0, NULL);
		if (!bRet)
		{
			//delete m_pList;
			m_pList.reset();
		}
	}
	return 0;
}

void CFindEdit::SetFont( CFont *pFont )
{
	CWnd::SetFont(pFont, TRUE);
	m_pList->SetFont(pFont);
}

void CFindEdit::SetCodeList( CStringArray *pCodeArray, CStringArray *pNameArray )
{
	if (m_pList)
	{
		m_pList->SetCodeList(pCodeArray, pNameArray);

		CRect rc;
		GetWindowRect(&rc);
		rc.OffsetRect(0, rc.Height());
		rc.bottom = rc.top + 300;
		rc.right = rc.left + 200;
		m_pList->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}
}

void CFindEdit::OnDestroy() 
{
	if (m_pList) 
		m_pList->DestroyWindow(); 
	CEdit::OnDestroy();
}
