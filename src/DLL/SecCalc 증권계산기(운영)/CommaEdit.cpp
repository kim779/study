// CommaEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CommaEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommaEdit

CCommaEdit::CCommaEdit()
{
	m_enumNumeric	= TEXT;
	m_bSkipOnChange	= FALSE;
	m_uiChar	= NULL;

	m_pfont		= NULL;
	m_bCreateFont	= FALSE;
	m_bToParent	= false;
}

CCommaEdit::~CCommaEdit()
{
	if (m_bCreateFont && m_pfont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
		m_pfont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCommaEdit, CEdit)
	//{{AFX_MSG_MAP(CCommaEdit)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommaEdit message handlers

BOOL CCommaEdit::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont, CCreateContext* pContext) 
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

int CCommaEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return Initialize();
}

int CCommaEdit::Initialize()
{
	if (m_pfont == NULL)
	{
		m_bCreateFont = TRUE;
		m_pfont = (CFont*) new CFont;
		m_pfont->CreatePointFont(90, "굴림");
	}
	SetFont(m_pfont);
	return 0;
}

void CCommaEdit::OnChange() 
{
	if (m_bSkipOnChange)
	{
		m_bSkipOnChange = FALSE;
		return;
	}

	CString strText, strPoint;
	GetWindowText(strText);
	strText.TrimLeft();
	strText.TrimRight();

	// 커서 위치 알아오기
	int iStart, iEnd;
	GetSel(iStart, iEnd);

	// 콤마를 제거
	int iRemovedComma = strText.Remove(',');
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

	int iPosPoint = strText.Find('.');
	if (iPosPoint >= 0)
	{
		strPoint = strText.Mid(iPosPoint);
		strText = strText.Left(iPosPoint);
		iLength = strText.GetLength();
	}
	else
	{
		strPoint = "";
	}

	int iCommaCount = (iLength - 1) / 3;

	char acNew[20];
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
	m_strTextBefore = acNew + strPoint;
	SetWindowText(acNew + strPoint);
	SetSel(iStart + iCommaCount, iEnd + iCommaCount, FALSE);

	ToParent();
}

void CCommaEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_uiChar = nChar;
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCommaEdit::SetNumericFloat()
{
	if (m_enumNumeric == NATURAL_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = FLOAT_NUMERIC;
	
	return TRUE;
}

BOOL CCommaEdit::SetNumericPositiveFloat()
{
	if (m_enumNumeric == NATURAL_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = POSITIVE_FLOAT_NUMERIC;
	
	return TRUE;
}

BOOL CCommaEdit::SetNumeric()
{
	if (m_enumNumeric == FLOAT_NUMERIC)
	{
		return FALSE;
	}

	m_enumNumeric = NATURAL_NUMERIC;
	
	return TRUE;
}

void CCommaEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void CCommaEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetSel(0, -1);
}

void CCommaEdit::SetFontFromParent(CFont* pfont)
{
	if (m_pfont && m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
		m_bCreateFont = FALSE;
	}

	m_pfont = pfont;
	SetFont(m_pfont);
}

void CCommaEdit::ToParent()
{
	if (m_bToParent)
	{
		CWnd* pwnd = GetParent();
		if (IsWindow(pwnd->m_hWnd))
			pwnd->PostMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_UPDATE), (LPARAM)m_hWnd);
	}
}

void CCommaEdit::SetToParent(bool bToParent)
{
	m_bToParent = bToParent;
}