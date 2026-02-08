#include "stdafx.h"
#include "ExEdit.h"
#include "DefUserMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExEdit::CExEdit()
{
	m_isNum = false;
	m_isPoint = false;
	m_isThous = false;
	m_isUpdn = false;
	m_isDiff = false;
	m_fix = _T("");
	m_bColor = false;
	m_bZero = false;
	m_bCheck = true;
	m_rgbText = RGB(0, 0, 0);
	m_rgbBk = RGB(255, 255, 255);

}

CExEdit::~CExEdit()
{
}

BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_USER_ON_ERROR, OnEditError)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CExEdit* CExEdit::CreateInPlace(CWnd* pWnd, CRect rect, int row, int col, int limit, long style)
{
	CExEdit* pEdit = (CExEdit*)new CExEdit;
	pEdit->Create(style, rect, pWnd, 1);
	pEdit->SetLimitText(limit);
	pEdit->SetPlace(row, col);
	return pEdit;
}

void CExEdit::SetPlace(int row, int col)
{
	m_row = row;
	m_col = col;
}

void CExEdit::GetPlace(int& row, int& col)
{
	row = m_row;
	col = m_col;
}

void CExEdit::MoveControl(CRect& rect)
{
	MoveWindow(rect, FALSE);
}

void CExEdit::SetNumber(bool isNum)
{
	m_isNum = isNum;
}

void CExEdit::SetPoint(bool bCheck, bool isPoint, bool bZero)
{
	m_isPoint = isPoint;
	m_bCheck = bCheck;
	m_bZero = bZero;
}


void CExEdit::SetPointEx(bool bCheck, bool isPoint, bool isThous)
{
	m_bCheck = bCheck;
	m_isPoint = isPoint;
	m_isThous = isThous;
}

void CExEdit::SetUpdn(bool isUpdn)
{
	m_isUpdn = isUpdn;
	m_bColor = true;
}

void CExEdit::SetDiff(bool isDiff)
{
	m_isDiff = isDiff;
	m_bColor = true;
}
	
void CExEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_isDiff)
	{
		CString text;
		GetWindowText(text);

		bool isHan = false;
		m_bHipon = false;
		int sel = -1;
		const char* tmp = (LPSTR)text.operator LPCTSTR();
		CString tmpS = _T("");
		for (int jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				tmpS += text.GetAt(jj);
			 	m_bHipon = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (m_bHipon && nChar == '-')
			return;
		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK || (!m_bHipon && nChar == '-'))
		;
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
		{
			return;	
		}
	}

	if (m_isUpdn)
	{
		CString text;
		GetWindowText(text);

		bool isHan = false;
		m_bNot = false;	
		m_bHipon = false;
		
		int sel = -1;
		const char* tmp = (LPSTR)text.operator LPCTSTR();
		CString tmpS = _T("");
		for (int jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if ((tmp[jj] >= 'a' && tmp[jj] <= 'z')
				|| (tmp[jj] >= 'A' && tmp[jj] <= 'Z'))
				sel = jj;
			else if (tmp[jj] == '.')
			{
				tmpS += text.GetAt(jj);
			 	m_bNot = true;
			}
			else if (tmp[jj] == '-')
			{
				tmpS += text.GetAt(jj);
			 	m_bHipon = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (m_bNot && nChar == '.')
			return;
		else if (m_bHipon && nChar == '-')
			return;
		else if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK 
			|| (!m_bNot && nChar == '.') || (!m_bHipon && nChar == '-'))
		;
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
		{
			return;	
		}
	}

	if (m_isNum)
	{
		CString text;
		GetWindowText(text);

		bool isHan = false;
		int sel = -1;
		const char* tmp = (LPSTR)text.operator LPCTSTR();
		CString tmpS = _T("");
		for (int jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK)
		;
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
		{
			return;	
		}
	}
	
	if (m_isPoint)
	{
		CString text;
		GetWindowText(text);

		bool isHan = false;
		m_bNot = false;		
		int sel = -1;
		const char* tmp = (LPSTR)text.operator LPCTSTR();
		CString tmpS = _T("");
		for (int jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if ((tmp[jj] >= 'a' && tmp[jj] <= 'z')
				|| (tmp[jj] >= 'A' && tmp[jj] <= 'Z'))
				sel = jj;
			else if (tmp[jj] == '.')
			{
				tmpS += text.GetAt(jj);
			 	m_bNot = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (m_bNot && nChar == '.')
			return;
		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK 
			|| (!m_bNot && nChar == '.'))
		;
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
		{
			return;	
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

int CExEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	const CWnd* pParent = GetParent();
	ASSERT(pParent != NULL);
	CFont* pFont = pParent->GetFont();
	SetFont(pFont);
	return 0;
}

void CExEdit::SetFixText(CString fixS)
{
	m_fix = fixS;
	SetWindowText(m_fix);
}

void CExEdit::SetText(CString str)
{
	SetWindowText(str + m_fix);
}

CString CExEdit::GetText()
{
	CString tmpS;
	GetWindowText(tmpS);
	const int len = m_fix.GetLength();
	tmpS = tmpS.Left(tmpS.GetLength() - len);
	tmpS.TrimRight();
	return tmpS;
}

void CExEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);

	GetWindowText(m_sText);

	CString tmpS;
	GetWindowText(tmpS);
	const int len = m_fix.GetLength();
	tmpS = tmpS.Left(tmpS.GetLength() - len);
	tmpS.TrimRight();
	
	SetWindowText(tmpS);
	SetSel(0, tmpS.GetLength());
}

void CExEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CString tmpS, tmpS_;
	GetWindowText(tmpS);
	if (m_isPoint)
	{
		if (!checkPoint(tmpS))
		{
			if (!m_isThous)
			{
				if (!m_bZero)
					SendMessage(WM_USER_ON_ERROR, error_pr);
				else
					SendMessage(WM_USER_ON_ERROR, error_pa);
			}
			else
				SendMessage(WM_USER_ON_ERROR, error_pe);
			SetWindowText(m_sText);
			CEdit::OnKillFocus(pNewWnd);
			return;
		}
	}

	SetWindowText(tmpS + m_fix);
	CEdit::OnKillFocus(pNewWnd);
}

bool CExEdit::checkPoint(CString& tmpS)
{
	const double tmpD = atof(tmpS);
	if (!m_bCheck)
	{
		if (tmpD == 0.0)
			tmpS = "0";
		return true;
	}
	if (m_isThous)
	{
		if (tmpD == 0.0)
		{
			tmpS = "0";
			return true;
		}
		if (tmpD < 0 || tmpD >= 10000.0)
			return false;
	}
	else
	{
		if (!m_bZero)
		{
			if (tmpD < 0.5 || tmpD > 100.0)
				return false;
		}
		else
		{
			if (tmpD < 0 || tmpD > 100.0)
				return false;
		}
	}
	tmpS.Format("%.2f", tmpD);
	return true;
}

BOOL CExEdit::PreTranslateMessage(MSG* pMsg) 
{
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CExEdit::OnEditError(WPARAM wParam, LPARAM lParam)
{
	GetOwner()->SendMessage(WM_EDIT_ON_ERROR, wParam);
	return TRUE;
}

void CExEdit::SetTextColor(COLORREF rgbText)
{
	m_rgbText = rgbText;
	Invalidate();
}

void CExEdit::SetBkColor(COLORREF rgbBk)
{
	m_rgbBk = rgbBk;
	Invalidate();
}

HBRUSH CExEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

HBRUSH CExEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (m_bColor)
	{
		pDC->SetTextColor(m_rgbText);
		pDC->SetBkColor(m_rgbBk);
		pDC->SetBkMode(TRANSPARENT);
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(m_rgbBk);
		return (HBRUSH)m_brush;	
	}
	else
		return NULL;
}
