// TextWnd.cpp : implementation file
//

#include "stdafx.h"
#include "TextWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextWnd

CTextWnd::CTextWnd()
{
	m_pfont		= NULL;
	m_bCreateFont	= FALSE;
	m_dwAlign	= DT_CENTER;
	m_pbmpBG	= NULL;
}

CTextWnd::~CTextWnd()
{
	//if (m_pfont && m_bCreateFont)
	//{
	//	m_pfont->DeleteObject();
	//	m_pfont = NULL;
	//}

	if (m_pbmpBG)
	{
		m_pbmpBG->DeleteObject();
		m_pbmpBG = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTextWnd, CWnd)
	//{{AFX_MSG_MAP(CTextWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTextWnd message handlers

int CTextWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	//return 0;
	return Initialize();
}

int CTextWnd::Initialize()
{
	if (m_pfont == NULL)
	{
		m_pfontLocal = std::make_unique<CFont>();
		m_pfontLocal->CreatePointFont(90, "±¼¸²Ã¼");
		m_pfont = m_pfontLocal.get();
		m_bCreateFont	= TRUE;
	}
	
	return 0;
}

void CTextWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Draw(&dc);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CTextWnd::Draw(CDC* pdc)
{
	CRect rect;
	GetClientRect(rect);

	if (m_pbmpBG)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(pdc);

		CBitmap* pbitmapOld= dcMem.SelectObject(m_pbmpBG.get());

		pdc->BitBlt(0, 0, m_rectI.Width(), m_rectI.Height(), &dcMem, 0, 0, SRCCOPY);

		dcMem.SelectObject(pbitmapOld);
		dcMem.DeleteDC();
	}
	else
	{
		pdc->FillSolidRect(rect, m_rgbBG);
		pdc->Draw3dRect(rect, m_rgbBorder, m_rgbBorder);
	}
	
	CFont* pfontOld = pdc->SelectObject(m_pfont);
	const int iBkModeOld = pdc->SetBkMode(TRANSPARENT);
	const COLORREF rgbFGOld = pdc->SetTextColor(m_rgbFG);

	rect.DeflateRect(3, 1, 3, 0);
	pdc->DrawText(m_strText, rect, m_dwAlign | DT_VCENTER | DT_SINGLELINE);
	//CString strText;
	//GetWindowText(strText);
	//pdc->DrawText(strText, rect, m_dwAlign | DT_VCENTER | DT_SINGLELINE);

	pdc->SetTextColor(rgbFGOld);
	pdc->SetBkMode(iBkModeOld);
	pdc->SelectObject(pfontOld);
}

BOOL CTextWnd::Create(	LPCTSTR lpszWindowName,
			DWORD dwStyle,
			const RECT& rect,
			CWnd* pParentWnd,
			UINT nID,
			CFont* pfont,
			COLORREF rgbBG,
			COLORREF rgbFG,
			COLORREF rgbBorder) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dwStyle == NULL)
	{
		dwStyle =	WS_CHILD
				| WS_VISIBLE
				| WS_CLIPSIBLINGS
				| WS_CLIPCHILDREN;
	}

	m_rgbBG		= rgbBG;
	m_rgbFG		= rgbFG;
	m_rgbBorder	= rgbBorder;

	m_strText = lpszWindowName;

	m_pfont = pfont;
	
	//return CWnd::Create(_T("WND"), lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
	return CWnd::Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
}


BOOL CTextWnd::SetFont(int iPoint, CString strFontName)
{
	if (m_pfontLocal)
	{
		m_pfontLocal->DeleteObject();
	}
	else {
		m_pfontLocal = std::make_unique<CFont>();
		m_pfont = m_pfontLocal.get();
	}

	const BOOL bResult = m_pfont->CreatePointFont(iPoint * 10, strFontName);

	m_bCreateFont = TRUE;
	Invalidate();
	return bResult;
}

void CTextWnd::SetFontFromParent(CFont* pfont)
{
	if (m_pfontLocal) {
		m_pfontLocal->DeleteObject();
		m_pfontLocal.reset();
		m_pfontLocal = nullptr;
	}

	m_pfont = pfont;

	Invalidate();
}

void CTextWnd::SetColor(COLORREF rgbBG, COLORREF rgbFG, COLORREF rgbBorder)
{
	m_rgbBG		= rgbBG;
	m_rgbFG		= rgbFG;
	m_rgbBorder	= rgbBorder;

	Invalidate();
}

void CTextWnd::SetColorBG(COLORREF rgbBG)
{
	m_rgbBG		= rgbBG;
	Invalidate();
}

void CTextWnd::SetColorFG(COLORREF rgbFG)
{
	m_rgbFG		= rgbFG;
	Invalidate();
}

void CTextWnd::SetText(CString strText)
{
	m_strText = strText;
	if (::IsWindow(m_hWnd))
		Invalidate();
}

char* CTextWnd::GetText()
{
	return (char*)m_strText.operator LPCTSTR();
}

void CTextWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LButtonDown(point);
}

void CTextWnd::LButtonDown(CPoint point)
{
	ClientToScreen(&point);
	GetParent()->SendMessage(WM_USER + 888, 100, (LPARAM)GetDlgCtrlID());
}

void CTextWnd::SetAlign(DWORD dwAlign)
{
	switch (dwAlign)
	{
	case DT_LEFT:
	case DT_CENTER:
	case DT_RIGHT:
		m_dwAlign = dwAlign;
		Invalidate();
		break;
	}
}

void CTextWnd::SetImagePath(CString strPath, CRect rectBmp)
{
	HBITMAP hBmp = (HBITMAP)::LoadImage(	AfxGetInstanceHandle(), 
						strPath,
						IMAGE_BITMAP,
						0,
						0,
						LR_LOADFROMFILE
						| LR_CREATEDIBSECTION);

	if (hBmp)
	{
		if (m_pbmpBG)
		{
			m_pbmpBG->DeleteObject();
			m_pbmpBG = NULL;
			m_rectI.SetRectEmpty();
		}

		m_pbmpBG = std::make_unique<CBitmap>();
		m_pbmpBG->Attach(hBmp);
		m_rectI = rectBmp;
	}
}