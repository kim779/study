// 3DEdit.cpp : implementation file
//

#include "stdafx.h"
#include "3DEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DEdit

C3DEdit::C3DEdit()
{
	m_enumNumeric	= TEXT;
	m_bSkipOnChange	= FALSE;
	m_uiChar	= NULL;

	m_pfont		= NULL;
	m_pfont2	= NULL;
	m_bCreateFont	= FALSE;
	m_bToParent	= false;
}

C3DEdit::~C3DEdit()
{
	//if (m_bCreateFont && m_pfont2)
	if (m_pfont2)
	{
		m_pfont2->DeleteObject();
		m_pfont2 = NULL;
	}
}
	
BEGIN_MESSAGE_MAP(C3DEdit, CEdit)
	//{{AFX_MSG_MAP(C3DEdit)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DEdit message handlers

BOOL C3DEdit::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dwExStyle == NULL)
	{
		dwExStyle = WS_EX_LEFT 
				| WS_EX_LTRREADING
				| WS_EX_RIGHTSCROLLBAR
				| WS_EX_NOPARENTNOTIFY
				| WS_EX_CLIENTEDGE;
	}

	if (dwStyle == NULL)
		dwStyle = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP;
	
	if ((dwStyle & ES_NUMBER) == ES_NUMBER)
		m_enumNumeric = NATURAL_NUMERIC;

	m_pfont = pfont;

	return CWnd::CreateEx(dwExStyle, _T("EDIT"), NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

int C3DEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return Initialize();
}

int C3DEdit::Initialize()
{
	if (m_pfont == NULL)
	{
		m_bCreateFont = TRUE;
		m_pfont2 = std::make_unique<CFont>();

		m_pfont2->CreatePointFont(90, "굴림체");
		m_pfont = m_pfont2.get();
	}

	SetFont(m_pfont);
	return 0;
}

void C3DEdit::OnChange() 
{
	if (m_bSkipOnChange)
	{
		m_bSkipOnChange = FALSE;
		return;
	}

	if (m_enumNumeric == NATURAL_NUMERIC)	// 자연수 숫자라면
	{
		CString strText;
		GetWindowText(strText);
		strText.TrimLeft();
		strText.TrimRight();

		// 커서 위치 알아오기
		int iStart, iEnd;
		GetSel(iStart, iEnd);

		// 콤마를 제거
		const int iRemovedComma = strText.Remove(',');
		iStart -= iRemovedComma;
		iEnd -= iRemovedComma;
		CString strBeforeRemoved = m_strTextBefore;
		strBeforeRemoved.Remove(',');

		if (m_uiChar == VK_BACK && strText == strBeforeRemoved)
		{
			iStart--;
			iEnd--;
			strText.Delete(iStart);
		}
		else if (m_uiChar == VK_DELETE && strText == strBeforeRemoved)
		{
			strText.Delete(iStart);
		}

		int iLength = strText.GetLength();

		if (iLength < 4)
		{
			m_bSkipOnChange = TRUE;
			m_strTextBefore = strText;
			SetWindowText(strText);
			SetSel(iStart, iEnd, FALSE);
			ToParent();
			return;
		}

	
		int iCommaCount = (iLength - 1) / 3;

		char acNew[20]{};
		int iDest = 0;
		for (int ii = 0; ii < iLength; ii++)
		{
			if ((iLength - ii) % 3 == 0 && ii != 0)
			{
				acNew[iDest++] = ',';
			}

			acNew[iDest++] = strText[ii];
		}

		acNew[iLength + iCommaCount] = 0;

		m_bSkipOnChange = TRUE;
		m_strTextBefore = acNew;
		SetWindowText(acNew);
		SetSel(iStart + iCommaCount, iEnd + iCommaCount, FALSE);
	}
	else if (m_enumNumeric == FLOAT_NUMERIC
			|| m_enumNumeric == POSITIVE_FLOAT_NUMERIC)	// 소수점 숫자라면
	{
		CString strText;
		GetWindowText(strText);
		strText.TrimLeft();
		strText.TrimRight();

		// 커서 위치 알아오기
		int iStart, iEnd;
		GetSel(iStart, iEnd);

		CString strBeforeRemoved = m_strTextBefore;
		strBeforeRemoved.Remove('.');
		// BackSpace 적용
		if (m_uiChar == VK_BACK && strText == strBeforeRemoved)
		{
			iStart--;
			iEnd--;
			strText.Delete(iStart);
		}
		// Delete 적용
		else if (m_uiChar == VK_DELETE && strText == strBeforeRemoved)
		{
			strText.Delete(iStart);
		}

		// 전체길이
		const int iLength = strText.GetLength();
		// .이 있는 위치
		const int iPosPoint = strText.Find('.');
		// 정수의 길이
		int iNaturalLength{};
		if (iPosPoint < 0)
		{
			iNaturalLength = iLength;
		}
		else
		{
			iNaturalLength = iPosPoint;
		}

		// 제한된 정수 길이를 넘겼다면 . 을 더함
		if (iNaturalLength > 3 && iPosPoint == -1)
		{
			strText = strText.Left(3) + "." + strText.Right(iNaturalLength - 3);
			iNaturalLength = 3;
			iStart++;
			iEnd++;
		}

		// 지정된 숫자길이를 넘겼다면 이전걸로 back
		if (iNaturalLength > 3)
		{
			m_bSkipOnChange = TRUE;
			SetWindowText(m_strTextBefore);
			SetSel(iStart, iEnd, FALSE);
			ToParent();
			return;
		}
		else if (iNaturalLength != iLength && (iLength - iNaturalLength - 1) > 2)
		{
			m_bSkipOnChange = TRUE;
			SetWindowText(m_strTextBefore);
			SetSel(iStart, iEnd, FALSE);
			ToParent();
			return;
		}

		// .xx 인 경우 -> xx
		if (iNaturalLength == 0 && iPosPoint == 0 && iLength > 1)
		{
			strText.Delete(0);
		}
		// . 인 경우 -> 0.
		else if (iNaturalLength == 0 && iPosPoint == 0 && iLength == 1)
		{
			strText = "0.";
			iStart++;
			iEnd++;
		}

		m_bSkipOnChange = TRUE;
		m_strTextBefore = strText;
		SetWindowText(strText);
		SetSel(iStart, iEnd, FALSE);
	}

	ToParent();
}

void C3DEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_uiChar = nChar;
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL C3DEdit::SetNumericFloat()
{
	if (m_enumNumeric == NATURAL_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = FLOAT_NUMERIC;
	
	return TRUE;
}

BOOL C3DEdit::SetNumericPositiveFloat()
{
	if (m_enumNumeric == NATURAL_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = POSITIVE_FLOAT_NUMERIC;
	
	return TRUE;
}

BOOL C3DEdit::SetNumeric()
{
	if (m_enumNumeric == FLOAT_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = NATURAL_NUMERIC;
	
	return TRUE;
}

void C3DEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_enumNumeric == FLOAT_NUMERIC)
	{
		if ((nChar < '0' || nChar > '9') && nChar != '.' && nChar != '-' && nChar != VK_BACK)
		{
			return;
		}
	}
	else if (m_enumNumeric == POSITIVE_FLOAT_NUMERIC)
	{
		if ((nChar < '0' || nChar > '9') && nChar != '.' && nChar != VK_BACK)
		{
			return;
		}
	}
	if (nChar == VK_RETURN)
		GetParent()->SendMessage(WM_USER + 1, GetDlgCtrlID(), 0);

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void C3DEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetSel(0, -1);
}

void C3DEdit::SetFontFromParent(CFont* pfont)
{
	if (m_pfont2) {
		m_pfont2->DeleteObject();
		m_pfont2.reset();
	}

	m_pfont = pfont;

	SetFont(m_pfont);
}

void C3DEdit::ToParent()
{
	if (m_bToParent)
	{
		CWnd* pwnd = GetParent();
		if (IsWindow(pwnd->m_hWnd))
			pwnd->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_UPDATE), (LPARAM)m_hWnd);
	}
}

void C3DEdit::SetToParent(bool bToParent)
{
	m_bToParent = bToParent;
}
