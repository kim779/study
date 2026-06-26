// EditX.cpp : implementation file
//

#include "pch.h"
//#include "AxisCode.h"
#include "EditX.h"
//#include "JongmukDlg.h"

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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
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

	/*const CWnd* parent = GetParent();
	CString	string; GetWindowText(string);

	switch (nChar)
	{
	case 0x0d:	parent->SendMessage(WM_EDITX, wpENTER, 0);
	}
	InvalidateRect(m_rectClear);*/
}

void CEditX::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	/*if (!m_bKeyDown)
		return;

	m_bKeyDown = false;

	CString	string; GetWindowText(string);

	if (string.GetLength() <= 0)
	{
		GetParent()->SendMessage(WM_EDITX, wpBLANK, 0);
		return;
	}

	int	xChar{} ; bool digit = false;
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

	InvalidateRect(m_rectClear);*/
}

void CEditX::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//m_bKeyDown = true;
	//CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	//InvalidateRect(m_rectClear);
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

//void CEditX::Init()
//{
//	m_strimagepath = m_path + "\\image\\GRAY_BAR_CLOSE.BMP";
//	 비트맵 객체 생성
//	HBITMAP hBitmap;
//
//	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
//		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
//	if (hBitmap)
//		m_pClearBmp = CBitmap::FromHandle(hBitmap);
//}


void CEditX::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CEdit::OnPaint()을(를) 호출하지 마십시오.

	//CRect rectClient;
	//GetClientRect(&rectClient);

	//int nCenterX = rectClient.right - 10;
	//int nCenterY = rectClient.Height() / 2;

	//// 닫기 버튼 그리기
	//int nBtnSize = 12; // 버튼의 크기
	//int nBtnLeft = nCenterX - nBtnSize / 2;
	//int nBtnTop = nCenterY - nBtnSize / 2;
	//int nBtnRight = nBtnLeft + nBtnSize;
	//int nBtnBottom = nBtnTop + nBtnSize;

	////dc.FillSolidRect(rectClient, RGB(255, 255, 255));
	//m_rectClear.SetRect(nBtnLeft, nBtnTop, nBtnRight, nBtnBottom);

	//if (!m_rectClear.IsRectEmpty())
	//	DrawBitmap(&dc, m_rectClear);
}

//void CEditX::DrawBitmap(CDC* pDC, CRect rect)
//{
//	rect.DeflateRect(2, 2, 1, 2);
//
//	if (m_pClearBmp == NULL)
//		return;
//
//	CDC memDC;
//	CBitmap* pOldBmp = nullptr;
//	BITMAP bmpInfo;
//	memDC.CreateCompatibleDC(pDC);
//	m_pClearBmp->GetBitmap(&bmpInfo);
//
//	if (memDC.m_hDC != NULL)
//	{
//		pOldBmp = (CBitmap*)memDC.SelectObject(m_pClearBmp);
//		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
//			&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
//	}
//
//	if (pOldBmp)
//		memDC.SelectObject(pOldBmp);
//	memDC.DeleteDC();
//}

void CEditX::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//InvalidateRect(m_rectClear);
	//if (m_rectClear.PtInRect(point))
	//{
	//	SetWindowText(_T(""));
	////	((CJongmukDlg*)m_pParent)->ClearSearch();
	//}
	CEdit::OnLButtonDown(nFlags, point);
}


void CEditX::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//InvalidateRect(m_rectClear);
	CEdit::OnLButtonUp(nFlags, point);
}


void CEditX::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
