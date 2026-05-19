// checkComboBox.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "checkComboBox.h"


// CCheckComboBox

static WNDPROC m_pWndProc = 0;
static CCheckComboBox *m_pComboBox = 0;

extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case LB_GETCURSEL:
		return -1;

	case WM_CHAR:
		if (wParam == VK_SPACE)
		{
			CRect	rcItem;
			int	nIndex = (int)CallWindowProcA(m_pWndProc, hWnd, LB_GETCURSEL, wParam, lParam);

			SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LPARAM)(void *)&rcItem);
			InvalidateRect(hWnd, rcItem, FALSE);

			m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));
			m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
			return 0;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			CRect	rc;
			CPoint pt;

			GetClientRect(hWnd, rc);
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			if (PtInRect(rc, pt))
			{
				CRect	rcItem;
				int	nItemHeight = (int)SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0);
				int	nTopIndex   = (int)SendMessage(hWnd, LB_GETTOPINDEX, 0, 0);
				int	nIndex = nTopIndex + pt.y / nItemHeight;

				SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LPARAM)(void *)&rcItem);
				if (PtInRect(rcItem, pt))
				{
					// Invalidate this window
					InvalidateRect(hWnd, rcItem, FALSE);
					m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));

					// Notify that selection has changed
					m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
				}
		}
		break;
	}

	case WM_LBUTTONUP:
		return 0;
	}

	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}

IMPLEMENT_DYNAMIC(CCheckComboBox, CComboBox)

CCheckComboBox::CCheckComboBox()
{
	m_listBox       = 0;
	m_bTextUpdated   = FALSE;
	m_bItemHeightSet = FALSE;
}

CCheckComboBox::~CCheckComboBox()
{
}


BEGIN_MESSAGE_MAP(CCheckComboBox, CComboBox)
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
END_MESSAGE_MAP()

// CCheckComboBox 메시지 처리기입니다.


void CCheckComboBox::DrawItem(LPDRAWITEMSTRUCT lpDS)
{
	CString strText;
	HDC	dc = lpDS->hDC;
	CRect	rcBitmap = lpDS->rcItem;
	CRect	rcText   = lpDS->rcItem;
	int	nCheck = 0;	// 0 - No check, 1 - Empty check, 2 - Checked

	// Check if we are drawing the static portion of the combobox
	if ((LONG)lpDS->itemID < 0)
	{
		// Make sure the m_strText member is updated
		recalcText();

		// Get the text
		strText = m_strText;

		// Don't draw any boxes on this item
		nCheck = 0;
	}
	else	// Otherwise it is one of the items
	{
		TEXTMETRIC metrics;

		GetLBText(lpDS->itemID, strText);
		nCheck = 1 + (GetItemData(lpDS->itemID) != 0);
		GetTextMetrics(dc, &metrics);

		rcBitmap.left    = 0;
		rcBitmap.right   = rcBitmap.left + metrics.tmHeight + metrics.tmExternalLeading + 6;
		rcBitmap.top    += 1;
		rcBitmap.bottom -= 1;
		rcText.left = rcBitmap.right;
	}

	if (nCheck > 0)
	{
		UINT	nState = DFCS_BUTTONCHECK;

		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
		if (nCheck > 1)
			nState |= DFCS_CHECKED;

		// Draw the checkmark using DrawFrameControl
		DrawFrameControl(dc, rcBitmap, DFC_BUTTON, nState);
	}

	if (lpDS->itemState & ODS_SELECTED)
	{
		SetBkColor(dc, GetSysColor(COLOR_HIGHLIGHT));
		SetTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
	}

	// Erase and draw
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rcText, 0, 0, 0);
	DrawText(dc, ' ' + strText, strText.GetLength() + 1, &rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	if ((lpDS->itemState & (ODS_FOCUS|ODS_SELECTED)) == (ODS_FOCUS|ODS_SELECTED))
		DrawFocusRect(dc, &rcText);
}


void CCheckComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMS)
{
	CClientDC	dc(this);
	CFont*	pFont = dc.SelectObject(GetFont());

	if (pFont != 0)
	{
		TEXTMETRIC metrics;

		dc.GetTextMetrics(&metrics);
		lpMS->itemHeight = metrics.tmHeight + metrics.tmExternalLeading;

		// This is needed since the WM_MEASUREITEM message is sent before
		// MFC hooks everything up if used in i dialog. So adjust the
		// static portion of the combo box now
		if (!m_bItemHeightSet)
		{
			m_bItemHeightSet = TRUE;
			SetItemHeight(-1, lpMS->itemHeight);
		}
		dc.SelectObject(pFont);
	}
}

LRESULT CCheckComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam)
{
	if (m_listBox == 0)
	{
		HWND	hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd)
		{
			m_listBox = hWnd;
			// Do the subclassing
			m_pWndProc = (WNDPROC)GetWindowLong(m_listBox, GWL_WNDPROC);
			SetWindowLong(m_listBox, GWLP_WNDPROC, (long)ComboBoxListBoxProc);
		}
	}
	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

LRESULT CCheckComboBox::OnGetText(WPARAM wParam, LPARAM lParam)
{
	recalcText();

	if (lParam == 0)
		return 0;

	lstrcpyn((LPSTR)lParam, m_strText, (int)wParam);
	return m_strText.GetLength();
}

LRESULT CCheckComboBox::OnGetTextLength(WPARAM wParam, LPARAM lParam)
{
	recalcText();
	return m_strText.GetLength();
}

void CCheckComboBox::OnDropDown()
{
	m_pComboBox = this;
}

BOOL CCheckComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle &= ~0xF;
	dwStyle |= (CBS_DROPDOWNLIST | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS);
	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

int CCheckComboBox::SetCheck(int nIndex, BOOL bFlag)
{
	int	nResult = SetItemData(nIndex, bFlag);
	if (nResult < 0)
		return nResult;

	m_bTextUpdated = FALSE;	// Signal that the text need updating
	Invalidate(FALSE);
	return nResult;
}

BOOL CCheckComboBox::GetCheck(int nIndex)
{
	return GetItemData(nIndex) ? TRUE : FALSE;
}

void CCheckComboBox::SelectAll(BOOL bCheck)
{
	for (int ii = 0; ii < GetCount(); ii++)
		SetCheck(ii, bCheck);
}

void CCheckComboBox::recalcText()
{
	if (!m_bTextUpdated)
	{
		CString	strText;
		int	nCount    = GetCount();

		// Get the list separator
		TCHAR	szBuffer[10] = {0};

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, szBuffer, sizeof(szBuffer));
		CString strSeparator = szBuffer;

		// If none found, the the ';'
		if (strSeparator.GetLength() == 0)
			strSeparator = _T(';');

		// Trim extra spaces
		strSeparator.TrimRight();

		// And one...
		strSeparator += _T(' ');
		for (int ii = 0; ii < nCount; ii++)
		{
			if (GetItemData(ii))
			{
				CString	strItem;

				GetLBText(ii, strItem);
				if (!strText.IsEmpty())
				{
					if (!strText.CompareNoCase(_T("input")) && !strItem.CompareNoCase(_T("output")))
					{
						strText = _T("Both");
						break;
					}
					strText += strSeparator;
				}

				strText += strItem;
			}
		}

		// Set the text
		m_strText = strText;
		m_bTextUpdated = TRUE;
	}
}
