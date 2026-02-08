// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000X2.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
	m_bKeyDown = false;
}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

LRESULT CEditEx::OnImeComposition(WPARAM wParam, LPARAM lParam)
{
	const UINT nChar = (UINT)wParam;
	OnChar(nChar, 1, 0);
	return 0;
}

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	
	CString	string; GetWindowText(string);
	
	if (nChar == 0x0d)
		GetParent()->SendMessage(WM_EDITEX, wpENTER, 0);
}

void CEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	if (!m_bKeyDown)
		return;
	
	m_bKeyDown = false;
	CString	string; GetWindowText(string);
	
	if (string.GetLength() <= 0)
	{
		GetParent()->SendMessage(WM_EDITEX, wpBLANK, 0);
		
		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITEX, wpDown, 0);
		return;
	}
	
	int	xChar{}; bool digit = false;
	for (int ii = 0; ii < string.GetLength(); ii++)
	{
		xChar = (int)string.GetAt(ii);
		if (xChar >= (int)'0' && xChar <= (int)'9')
		{
			digit = true;
			continue;
		}
		digit = false;
		break;
	}
	
	if (digit)
	{
		GetParent()->SendMessage(WM_EDITEX, wpSEARCH1, 0);
		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITEX, wpDown, 0);
	}
	else
	{
		GetParent()->SendMessage(WM_EDITEX, wpSEARCH2, 0);
		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITEX, wpDown, 0);
	}
}

void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyDown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


// void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
// {
// 	// TODO: Add your message handler code here and/or call default
// 	if(nChar == VK_RETURN)
// 		GetParent()->SendMessage(WM_EDITEX);
// 
// 	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
// }
