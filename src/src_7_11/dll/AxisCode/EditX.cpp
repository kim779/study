// EditX.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "EditX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
	m_bKeyDown = false;
}

CEditX::~CEditX()
{
}


BEGIN_MESSAGE_MAP(CEditX, CEdit)
	//{{AFX_MSG_MAP(CEditX)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers

LRESULT CEditX::OnImeComposition(WPARAM wParam, LPARAM lParam)
{
	UINT nChar = (UINT)wParam;
	OnChar(nChar, 1, 0);
	return 0;
}

void CEditX::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CWnd* parent = GetParent();
	CString	string; GetWindowText(string);

	switch (nChar)
	{
	case 0x0d:	parent->SendMessage(WM_EDITX, wpENTER, 0);
	}
}

void CEditX::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	if (!m_bKeyDown)
		return;

	m_bKeyDown = false;

	CString	string; GetWindowText(string);

	if (string.GetLength() <= 0)
	{
		GetParent()->SendMessage(WM_EDITX, wpBLANK, 0);
		return;
	}

	int	xChar; bool digit = false;
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
		GetParent()->SendMessage(WM_EDITX, wpSEARCH1, 0);
	else
		GetParent()->SendMessage(WM_EDITX, wpSEARCH2, 0);
}

void CEditX::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyDown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CEditX::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
		case VK_LEFT:	
			GetParent()->SendMessage(WM_EDITX, wpUP, 0);
			return TRUE;
		case VK_DOWN:
		case VK_RIGHT:
			GetParent()->SendMessage(WM_EDITX, wpDOWN, 0);
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
