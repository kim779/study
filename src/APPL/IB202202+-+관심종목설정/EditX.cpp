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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSELEAVE()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
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

void CEditX::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.

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


void CEditX::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnLButtonUp(nFlags, point);
}

void CEditX::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
}


void CEditX::OnMouseLeave()
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	OutputDebugString("clearbtn202201 OnMouseLeave");
	CEdit::OnMouseLeave();
	//return ;
}


void CEditX::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

}


void CEditX::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
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
//	pDC->StretchBlt(rect.left, rect.top, rect.Width() , rect.Height(),
//		&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
//	}
//
//	if (pOldBmp)
//		memDC.SelectObject(pOldBmp);
//	memDC.DeleteDC();
//}

//#define BTN_GAP 2
//void CEditX::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//					   // TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
//					   // ±×¸®±â ¸Þ½ÃÁö¿¡ ´ëÇØ¼­´Â CEdit::OnPaint()À»(¸¦) È£ÃâÇÏÁö ¸¶½Ê½Ã¿À.
//
//	CRect rectClient;
//	GetClientRect(&rectClient);
//
//	int nCenterX = rectClient.right - 10;
//	int nCenterY = rectClient.Height() / 2;
//
//	// ´Ý±â ¹öÆ° ±×¸®±â
//	int nBtnSize = 12; // ¹öÆ°ÀÇ Å©±â
//	int nBtnLeft = nCenterX - nBtnSize / 2;
//	int nBtnTop = nCenterY - nBtnSize / 2;
//	int nBtnRight = nBtnLeft + nBtnSize;
//	int nBtnBottom = nBtnTop + nBtnSize;
//
//	m_rectClear.SetRect(nBtnLeft, nBtnTop, nBtnRight, nBtnBottom);
//
//	if(!m_rectClear.IsRectEmpty())
//		DrawBitmap(&dc, m_rectClear);
//}




///////////////////////////////////////////////////////////////////////////////
// ctor
CXEditPrompt::CXEditPrompt()
    : m_bShowCueBanner(TRUE), m_strPromptText(_T("<Enter text here>")), m_crPromptColor(RGB(119, 121, 118)) // RAL 9023 (Pearl dark gray)
													    // see http://www.highplains.net/pixelcolor.html
      ,
      m_dwCueBannerAlign(ES_LEFT)
{
	m_crBkColor = GetSysColor(COLOR_WINDOW);
	m_bKeyDown = false;
	m_bTracking = false;
	_preinput.Empty();
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXEditPrompt::~CXEditPrompt()
{
	m_brush.DeleteObject();
	m_robrush.DeleteObject();
}


/*----------------------------------------------------------------------------*/
/* CXEditPrompt
/*----------------------------------------------------------------------------*/
IMPLEMENT_DYNCREATE(CXEditPrompt, CEdit)

BEGIN_MESSAGE_MAP(CXEditPrompt, CEdit)
ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
ON_WM_CREATE()
ON_WM_CTLCOLOR_REFLECT()
ON_WM_KEYDOWN()
ON_WM_SETFOCUS()
ON_WM_KILLFOCUS()
ON_WM_LBUTTONDOWN()
ON_WM_MBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_KEYUP()
ON_WM_KEYDOWN()
ON_WM_MOUSELEAVE()
ON_WM_MOUSEHOVER()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


int CXEditPrompt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	OnInitEdit();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXEditPrompt::PreSubclassWindow()
{
	CEdit::PreSubclassWindow();
	const _AFX_THREAD_STATE *pThreadState = AfxGetThreadState();
	if (pThreadState->m_pWndInit == nullptr)
	{
		OnInitEdit();
	}
}


BOOL CXEditPrompt::OnInitEdit()
{
	m_brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));

	m_robrush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	EnableCueBanner();

	//SetWindowText(m_strPromptText);
	SetSel(-1, 0); // get rid of standard highlighting
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Reset
void CXEditPrompt::Reset()
{
	EnableCueBanner();
	_preinput.Empty();
	SetWindowText(m_strPromptText);
	SetSel(m_strPromptText.GetLength(), m_strPromptText.GetLength());
	RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// SetPromptColor
void CXEditPrompt::SetPromptColor(COLORREF crText)
{
	m_crPromptColor = crText;
	//if (m_bFirstTime)
	//	RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// SetPromptText
void CXEditPrompt::SetPromptText(LPCTSTR lpszPrompt)
{
	m_strPromptText = lpszPrompt;
	if (m_strPromptText.IsEmpty())
	{
		EnableCueBanner(FALSE);
	}
	if (m_bShowCueBanner)
		SetWindowText(m_strPromptText);
}

///////////////////////////////////////////////////////////////////////////////
// OnSetFocus
void CXEditPrompt::OnSetFocus(CWnd *pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	if (m_bShowCueBanner)
	{
		// get rid of standard highlighting
		SetSel(m_strPromptText.GetLength(), m_strPromptText.GetLength());
	}
}

///////////////////////////////////////////////////////////////////////////////
// CtlColor
HBRUSH CXEditPrompt::CtlColor(CDC *pDC, UINT /*nCtlColor*/)
{
	if (!IsWindowEnabled())
		return nullptr;
	if (m_bShowCueBanner)
	{
		pDC->SetTextColor(m_crPromptColor);
	}
	if (GetStyle() & ES_READONLY)
	{
		pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
		return m_robrush;
	}
	pDC->SetBkColor(m_crBkColor);
	return m_brush; // setting text color will have no effect unless
			// we return a valid brush
}

///////////////////////////////////////////////////////////////////////////////
// OnKeyDown
void CXEditPrompt::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_bKeyDown = true;
	if (m_bShowCueBanner)
	{
		// key down includes keys like shift and ctrl
		EnableCueBanner(FALSE);
		SetWindowText(_T(""));
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_RETURN)
		GetParent()->SendMessage(WM_EDITX, wpENTER, 0);
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
}





///////////////////////////////////////////////////////////////////////////////
// OnLButtonDown
void CXEditPrompt::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShowCueBanner)
	{
		EnableCueBanner(FALSE);
		SetWindowText(_T(""));
	}

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

///////////////////////////////////////////////////////////////////////////////
// OnMButtonDown
void CXEditPrompt::OnMButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShowCueBanner)
	{
		EnableCueBanner(FALSE);
		SetWindowText(_T(""));
	}

	CEdit::OnMButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnRButtonDown
void CXEditPrompt::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShowCueBanner)
	{
		EnableCueBanner(FALSE);
		SetWindowText(_T(""));
	}

	CEdit::OnRButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// GetWindowText
int CXEditPrompt::GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const
{
	if (m_bShowCueBanner)
	{
		if (lpszStringBuf && (nMaxCount > 0))
			lpszStringBuf[0] = _T('\0');
		return 0;
	}

	return CEdit::GetWindowText(lpszStringBuf, nMaxCount);
}

///////////////////////////////////////////////////////////////////////////////
// GetWindowText
void CXEditPrompt::GetWindowText(CString &rString) const
{
	if (m_bShowCueBanner)
		rString = _T("");
	else
		CEdit::GetWindowText(rString);
}

///////////////////////////////////////////////////////////////////////////////
// SetWindowText
void CXEditPrompt::SetWindowText(LPCTSTR lpszString)
{
	if (!lpszString)
		return;

	// the control is being initialized, just ignore
	if (m_bShowCueBanner && (lpszString[0] == _T('\0')))
		return;

	// if this is not prompt string, then no need to prompt
	if (lpszString &&
	    (m_strPromptText.Compare(lpszString) != 0))
		EnableCueBanner(FALSE);

	CEdit::SetWindowText(lpszString);
}

///////////////////////////////////////////////////////////////////////////////
// DefWindowProc
LRESULT CXEditPrompt::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SETTEXT)
	{
		const TCHAR *cp = (TCHAR *)lParam;

		if (!cp)
			return TRUE;

		// the control is being initialized, just ignore
		if (m_bShowCueBanner && (cp[0] == _T('\0')))
			return TRUE;

		// if this is not prompt string, then no need to prompt
		if (m_strPromptText.Compare(cp) != 0)
			EnableCueBanner(FALSE);
	}
	else if (message == WM_GETTEXT)
	{
		if (m_bShowCueBanner)
		{
			TCHAR *cp = (TCHAR *)lParam;

			if (cp && (wParam != 0))
				*cp = _T('\0');

			return 0;
		}
	}
	else if (WM_GETTEXTLENGTH == message)
	{
		if (m_bShowCueBanner)
			return 0;
	}
	else if (WM_CHAR == message && VK_BACK == wParam)
			_preinput.Empty();

	return CEdit::DefWindowProc(message, wParam, lParam);
}

void CXEditPrompt::SetBKColor(COLORREF crBK)
{
	m_crBkColor = crBK;
	RedrawWindow();
}

void CXEditPrompt::OnKillFocus(CWnd *pOldWnd)
{
	CEdit::OnKillFocus(pOldWnd);
	if (!m_bShowCueBanner)
	{
		CString strWndText;
		GetWindowText(strWndText);
		if (strWndText.IsEmpty())
		{
			EnableCueBanner();
			SetWindowText(m_strPromptText);
		}
	}
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
}

int CXEditPrompt::GetWindowTextLength() const
{
	if (m_bShowCueBanner)
		return 0;
	return CEdit::GetWindowTextLength();
}

void CXEditPrompt::SetCueBannerAlign(DWORD val)
{
	m_dwCueBannerAlign = val;
	if (m_bShowCueBanner)
		UpdateStyleForCueBanner();
}

LRESULT CXEditPrompt::OnImeComposition(WPARAM wParam, LPARAM lParam)
{
	const UINT nChar = (UINT)wParam;
	OnChar(nChar, 1, 0);
	return 0;
}

static bool IsHangul(wchar_t ch)
{
	// ÇÑ±Û À½Àý(°¡-ÆR) + ÀÚ¸ð(¤¡-¤¾, ¤¿-¤Ó)±îÁö Æ÷ÇÔ
	return (ch >= 0xAC00 && ch <= 0xD7A3) ||  // °¡-ÆR
		(ch >= 0x3131 && ch <= 0x318E) ||  // ¤¡-¤¾, ¤¿-¤Ó (È£È¯ ÀÚ¸ð)
		(ch >= 0x1100 && ch <= 0x11FF);    // ÇÑ±Û ÀÚ¸ð
}

static bool IsStockCodeString(CString& s) // s¸¦ ´ë¹®ÀÚ·Î Á¤¸®ÇØÁÖ·Á°í non-const
{
	s.Trim(); // ¾ÕµÚ °ø¹é Á¦°Å
	const int len = s.GetLength();
	if (len <= 0) return false;

	// ±æÀÌ Á¦ÇÑ(³Ê È¯°æ¿¡ ¸Â°Ô Á¶Àý °¡´É)
	if (len > 12) return false;

	bool hasDigit = false;

	for (int i = 0; i < len; ++i)
	{
		const wchar_t ch = (wchar_t)s.GetAt(i);

		// ÇÑ±Û Æ÷ÇÔÀÌ¸é ¹«Á¶°Ç Á¾¸ñ¸í
		if (IsHangul(ch))
			return false;

		// ¼ýÀÚ/¿µ¹®¸¸ Çã¿ë (±× ¿Ü: -, _, °ø¹é, Æ¯¼ö¹®ÀÚ => Á¾¸ñ¸í)
		if (!_istalnum(ch))
			return false;

		if (_istdigit(ch))
		{
			hasDigit = true;
			continue;
		}

		// ¿µ¹®Àº ´ë¹®ÀÚ·Î ÅëÀÏ
		if (_istalpha(ch))
		{
			s.SetAt(i, (TCHAR)_totupper(ch));
			continue;
		}
	}

	// ¼ýÀÚ 1°³µµ ¾øÀ¸¸é ÄÚµå·Î º¸Áö ¾ÊÀ½(ÀüºÎ ¿µ¹®ÀÌ¸é Á¾¸ñ¸í Ã³¸®)
	if (!hasDigit)
		return false;

	return true;
}

void CXEditPrompt::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (1)
	{
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);

		if (!m_bKeyDown)
			return;
		m_bKeyDown = false;

		CString text;
		GetWindowText(text);
		text.Trim();

		// ºó ¹®ÀÚ¿­ Ã³¸®
		if (text.IsEmpty())
		{
			GetParent()->SendMessage(WM_EDITX, wpBLANK, 0);

			// Down Å°(40) Ã³¸® À¯Áö
			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
			return;
		}

		// ÄÚµå/ÀÌ¸§ ÆÇº°
		CString key = text; // IsStockCodeString¿¡¼­ ´ë¹®ÀÚ º¯È¯ÇÒ ¼ö ÀÖÀ¸´Ï º¹»çº» »ç¿ë
		const bool isCode = IsStockCodeString(key);

		if (isCode)
		{
			// ´ë¹®ÀÚ ¹Ý¿µÀÌ ÇÊ¿äÇÏ¸é ¿¡µðÆ®¿¡ ´Ù½Ã ¼¼ÆÃ(¿øÇÏ¸é ÁÖ¼® ÇØÁ¦)
			// if (key != text) SetWindowText(key);

			GetParent()->SendMessage(WM_EDITX, wpSEARCH1, 0); // Á¾¸ñÄÚµå
			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
		}
		else
		{
			GetParent()->SendMessage(WM_EDITX, wpSEARCH2, 0); // Á¾¸ñ¸í
			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
		}
	}
	else
	{
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		if (!m_bKeyDown)
			return;

		m_bKeyDown = false;
		CString	string; GetWindowText(string);

		if (string.GetLength() <= 0)
		{
			GetParent()->SendMessage(WM_EDITX, wpBLANK, 0);

			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
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
			GetParent()->SendMessage(WM_EDITX, wpSEARCH1, 0);
			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
		}
		else
		{
			GetParent()->SendMessage(WM_EDITX, wpSEARCH2, 0);
			if (nChar == 40)
				GetParent()->SendMessage(WM_EDITX, wpDown, 0);
		}
	}
}


void CXEditPrompt::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	const CWnd* parent = GetParent();
	CString	string; GetWindowText(string);

	switch (nChar)
	{
	case 0x0d:	parent->SendMessage(WM_EDITX, wpENTER, 0);
	}
	//GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
}

void CXEditPrompt::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnLButtonUp(nFlags, point);
}

void CXEditPrompt::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
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

void CXEditPrompt::OnMouseLeave()
{
	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ ¹×/¶Ç´Â ±âº»°ªÀ» È£ÃâÇÕ´Ï´Ù.
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnMouseLeave();
	//return ;
}

void CXEditPrompt::OnMouseHover(UINT nFlags, CPoint point)
{	
	GetParent()->SendMessage(WM_EDITX, wpBTNREDRAW, 0);
	CEdit::OnMouseHover(nFlags, point);
}


