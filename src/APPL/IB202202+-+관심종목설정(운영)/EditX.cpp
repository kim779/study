// EditX.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
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
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers

LRESULT CEditX::OnImeComposition(WPARAM wParam, LPARAM lParam)
{
	const UINT nChar = (UINT)wParam;
	OnChar(nChar, 1, 0);
	return 0;
}

void CEditX::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	CString	string; GetWindowText(string);

	if (nChar == 0x0d)
		GetParent()->SendMessage(WM_EDITX, wpENTER, 0);
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

		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITX, wpDown, 0);
		return;
	}

	int	xChar{}; 
	bool digit = false;
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
		GetParent()->SendMessage(WM_EDITX, wpSEARCH1, 0);
		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITX, wpDown, 0);
	}
	else
	{
		GetParent()->SendMessage(WM_EDITX, wpSEARCH2, 0);
		if(nChar == 40)
			GetParent()->SendMessage(WM_EDITX, wpDown, 0);
	}
}

void CEditX::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyDown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
}

void CEditX::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnLButtonUp(nFlags, point);
}


void CEditX::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rec;
	GetClientRect(rec);
	rec.left = rec.right - 15;
	if (rec.PtInRect(point))
	{
		SetWindowText(_T(""));
		GetParent()->SendMessage(WM_EDITX, wpBTNCLEAR, 0);
	}
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	
	CEdit::OnLButtonDown(nFlags, point);
}


void CEditX::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CEditX::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnMouseMove(nFlags, point);
}


void CEditX::OnMouseLeave()
{
	OutputDebugString("clearbtn 202202 OnMouseLeave");
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnMouseLeave();
}


void CEditX::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
