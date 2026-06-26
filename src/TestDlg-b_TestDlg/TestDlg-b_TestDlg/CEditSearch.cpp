#include "pch.h"
#include "CEditSearch.h"


CEditSearch::CEditSearch()
{
	m_bKeyDown = false;
}

CEditSearch::~CEditSearch()
{
}

BOOL CEditSearch::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
BEGIN_MESSAGE_MAP(CEditSearch, CEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


void CEditSearch::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CEditSearch::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CEditSearch::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditSearch::OnDraw(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);
}
//void CEditSearch::OnPaint()
//{
 //   CPaintDC dc(this); // device context for painting
	//				   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//				   // 그리기 메시지에 대해서는 CEdit::OnPaint()을(를) 호출하지 마십시오.
	//CRect rectClient;
	//GetClientRect(&rectClient);
	//OnDraw(&dc);
	//int nCenterX = rectClient.right - 10;
	//int nCenterY = rectClient.Height() / 2;

	//int nBtnSize = 12; // 버튼의 크기
	//int nBtnLeft = nCenterX - nBtnSize / 2;
	//int nBtnTop = nCenterY - nBtnSize / 2;
	//int nBtnRight = nBtnLeft + nBtnSize;
	//int nBtnBottom = nBtnTop + nBtnSize;

	//dc.FillSolidRect(rectClient, RGB(255, 255, 255));
	//m_rectClear.SetRect(nBtnLeft, nBtnTop, nBtnRight, nBtnBottom);

	//if (!m_rectClear.IsRectEmpty())
	//	DrawBitmap(&dc, m_rectClear);
//}

void CEditSearch::DrawBitmap(CDC* pDC, CRect rect)
{
	CClientDC dc(this);
	CRect rec;
	GetClientRect(rec);
	rec.right = rect.Width();
	rec.bottom = rect.Height();
	rect.DeflateRect(2, 2, 1, 2);

	if (m_pClearBmp == NULL)
		return;

	CDC memDC;
	CBitmap* pOldBmp = nullptr;
	BITMAP bmpInfo;
	memDC.CreateCompatibleDC(pDC);
	m_pClearBmp->GetBitmap(&bmpInfo);

	if (memDC.m_hDC != NULL)
	{
		pOldBmp = (CBitmap*)memDC.SelectObject(m_pClearBmp);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	}

	//dc.FillSolidRect(rec, RGB(255, 0, 0));

	if (pOldBmp)
		memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CEditSearch::Init()
{
	m_strimagepath = m_path + "\\image\\GRAY_BAR_CLOSE.BMP";
	HBITMAP hBitmap;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
		m_pClearBmp = CBitmap::FromHandle(hBitmap);

	CRect rectClient;
	GetClientRect(&rectClient);
	
	int nCenterX = rectClient.right - 10;
	int nCenterY = rectClient.Height() / 2;
	
	// 닫기 버튼 그리기
	int nBtnSize = 12; // 버튼의 크기
	int nBtnLeft = nCenterX - nBtnSize / 2;
	int nBtnTop = nCenterY - nBtnSize / 2;
	int nBtnRight = nBtnLeft + nBtnSize;
	int nBtnBottom = nBtnTop + nBtnSize;
	
	//dc.FillSolidRect(rectClient, RGB(255, 255, 255));
	m_rectClear.SetRect(nBtnLeft, nBtnTop, nBtnRight, nBtnBottom);
	
//	if (!m_rectClear.IsRectEmpty())
//		DrawBitmap(&dc, m_rectClear);
}


void CEditSearch::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CEdit::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
