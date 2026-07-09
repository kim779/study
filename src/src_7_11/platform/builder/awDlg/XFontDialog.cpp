// XFontDialog.cpp  Version 1.0
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// Description:
//     XFontDialog.cpp implements CXFontDialog, a class to display
//     a customized CFontDialog.
//
// History
//     Version 1.0 - 2003 June 21
//     - Initial public release
//
// This software is released into the public domain.  You are free to use it 
// in any way you like.
//
// This software is provided "as is" with no expressed or implied warranty.  
// I accept no liability for any damage or loss of business that this software 
// may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XFontDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXFontDialog *	g_pDlg = NULL;

#define WM_SHOW_MONOSPACED (WM_USER+1)

// assign id's to controls
#define IDC_SAMPLE_GROUPBOX	1073
#define IDC_FONT_LABEL		1088
#define IDC_SIZE_LABEL		1090
#define IDC_SAMPLE			1092
#define IDC_FONT			1136
#define IDC_FONT_SIZE		1138
#define IDC_MONOSPACED		1200

// private functions
static void DisplayMonoSpaced(HWND hdlg, int nIndex);
static BOOL GetTM(HWND hdlg, int index, TEXTMETRIC *pTM);
static BOOL IsMonoSpaced(HWND hdlg, LPCTSTR lpszFont);
static unsigned int CALLBACK CFHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM /*lParam*/);
static BOOL RemoveFont(HWND hWnd, LPCTSTR lpszFont);

///////////////////////////////////////////////////////////////////////////////
// CXFontDialog

IMPLEMENT_DYNAMIC(CXFontDialog, CFontDialog)

BEGIN_MESSAGE_MAP(CXFontDialog, CFontDialog)
	//{{AFX_MSG_MAP(CXFontDialog)
	ON_MESSAGE(WM_SHOW_MONOSPACED, OnShowMonoSpaced)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXFontDialog::CXFontDialog(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
	g_pDlg           = this;
	m_strInitialFont = _T("");
	m_strCaption     = _T("Font");
	m_CharSet        = 0;
	m_PitchAndFamily = 0;
	m_Height         = 0;
	m_dwFontFilter   = 0;

	if (lplfInitial)
		m_strInitialFont = lplfInitial->lfFaceName;
}

///////////////////////////////////////////////////////////////////////////////
// DoModal
int CXFontDialog::DoModal()
{
	TRACE(_T("in CXFontDialog::DoModal\n"));

	m_cf.Flags         |= CF_ENABLETEMPLATE | CF_ENABLEHOOK;
	m_cf.hInstance      = AfxGetResourceHandle();
	m_cf.lpTemplateName = _T("IDD_XFONTDIALOG");
	m_cf.hwndOwner      = m_hWnd;
	m_cf.lpfnHook       = CFHookProc;

	int rc = CFontDialog::DoModal();

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// OnCtlColor
HBRUSH CXFontDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFontDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	CWnd *pMonoSpaced = GetDlgItem(IDC_MONOSPACED);

	if (pWnd->m_hWnd == pMonoSpaced->m_hWnd)
		pDC->SetTextColor(RGB(0,0,255));

	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// OnShowMonoSpaced
LRESULT CXFontDialog::OnShowMonoSpaced(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd *pWnd = GetDlgItem(IDC_MONOSPACED);

	if (pWnd)
		pWnd->ShowWindow(wParam ? SW_SHOW : SW_HIDE);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// private functions of CXFontDialog
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// CFHookProc
static unsigned int CALLBACK CFHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM /*lParam*/)
{
	///////////////////////////////////////////////////////////////////////////
	// WM_INITDIALOG
	if (uiMsg == WM_INITDIALOG)
	{
		TRACE(_T("WM_INITDIALOG\n"));

		ASSERT(g_pDlg);

		::SetWindowText(hdlg, g_pDlg->GetCaption());

		HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
		ASSERT(hWndFont);

		if (hWndFont && g_pDlg->GetFontFilter())
		{
			// filter fonts to show only selected ones

			TCHAR szFont[1000];
			szFont[0] = _T('\0');
			int nCount = ::SendMessage(hWndFont, CB_GETCOUNT, 0, 0);

			// loop through all fonts in Fonts combo - we do this from
			// end to top so that index will not be affected by deletion.
			// If nCount == CB_ERR this loop will not be executed.
			for (int i = nCount-1; i >= 0; i--)
			{
				::SendMessage(hWndFont, CB_GETLBTEXT, i, (LPARAM)szFont);

				TEXTMETRIC tm;
				BOOL bSuccess = GetTM(hdlg, i, &tm);
				BOOL bDeleted = FALSE;

				if (!bDeleted && bSuccess && (g_pDlg->GetFontFilter() & XCF_SHOW_SYMBOL))
				{
					if (tm.tmCharSet != SYMBOL_CHARSET)
						bDeleted = RemoveFont(hWndFont, szFont);
				}

				if (!bDeleted && bSuccess && (g_pDlg->GetFontFilter() & XCF_SHOW_ALL_EXCEPT_SYMBOL))
				{
					if (tm.tmCharSet == SYMBOL_CHARSET)
						bDeleted = RemoveFont(hWndFont, szFont);
				}

				if (!bDeleted && bSuccess && (g_pDlg->GetFontFilter() & XCF_SHOW_MONOSPACED))
				{
					if (tm.tmPitchAndFamily & TMPF_FIXED_PITCH)
						bDeleted = RemoveFont(hWndFont, szFont);
				}

				if (!bDeleted && bSuccess && (g_pDlg->GetFontFilter() & XCF_SHOW_ALL_EXCEPT_MONOSPACED))
				{
					if ((tm.tmPitchAndFamily & TMPF_FIXED_PITCH) == 0)
						bDeleted = RemoveFont(hWndFont, szFont);
				}
			}

		} // if (hWndFont && g_pDlg->GetFontFilter())

		if (hWndFont)
		{
			// check if any font is selected
			int nCurSel = ::SendMessage(hWndFont, CB_GETCURSEL, 0, 0);
			if (nCurSel == CB_ERR)
			{
				// no font selected - try to select first font in list
				int nCount = ::SendMessage(hWndFont, CB_GETCOUNT, 0, 0);
				if (nCount > 0)
				{
					// select font
					::SendMessage(hWndFont, CB_SETCURSEL, 0, 0);
					TRACE(_T("setting initial font to 0\n"));

					// tell CFontDialog that a new font is selected
					::SendMessage(hdlg, WM_COMMAND, MAKEWPARAM(IDC_FONT, CBN_SELCHANGE), 
						(LPARAM)hWndFont);

				}
				nCurSel = 0;
			}

			// ensure selected font is visible
			TRACE(_T("setting top index to %d\n"), nCurSel);
			::SendMessage(hWndFont, CB_SETTOPINDEX, nCurSel, 0);

			if (nCurSel != CB_ERR)
			{
				// display MONOSPACED
				DisplayMonoSpaced(hdlg, nCurSel);
			}
		}

	}

	///////////////////////////////////////////////////////////////////////////
	// CBN_SELCHANGE
	if (uiMsg == WM_COMMAND && HIWORD(wParam) == CBN_SELCHANGE)
	{
		TRACE(_T("CBN_SELCHANGE\n"));

		HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
		ASSERT(hWndFont);

		if (hWndFont)
		{
			// check if current selection is monospaced
			int nCurSel = ::SendMessage(hWndFont, CB_GETCURSEL, 0, 0);
			if (nCurSel != CB_ERR)
				DisplayMonoSpaced(hdlg, nCurSel);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// IDOK
	if (uiMsg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)
	{
		TRACE(_T("IDOK\n"));

		g_pDlg->SetCharSet(0);
		g_pDlg->SetPitchAndFamily(0);
		g_pDlg->SetTmHeight(0);

		HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
		ASSERT(hWndFont);

		int nCurSel = ::SendMessage(hWndFont, CB_GETCURSEL, 0, 0);
		if (nCurSel != CB_ERR)
		{
			TEXTMETRIC tm;
			BOOL bSuccess = GetTM(hdlg, nCurSel, &tm);

			if (bSuccess)
			{
				// save some textmetric values
				g_pDlg->SetCharSet(tm.tmCharSet);
				g_pDlg->SetPitchAndFamily(tm.tmPitchAndFamily);
				g_pDlg->SetTmHeight(tm.tmHeight);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// IDCANCEL
	if (uiMsg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL)
	{
		TRACE(_T("IDCANCEL\n"));

		g_pDlg->SetCharSet(0);
		g_pDlg->SetPitchAndFamily(0);
		g_pDlg->SetTmHeight(0);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// DisplayMonoSpaced
static void DisplayMonoSpaced(HWND hdlg, int nIndex)
{
	TCHAR szFont[1000];
	szFont[0] = _T('\0');

	HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
	ASSERT(hWndFont);

	::SendMessage(hWndFont, CB_GETLBTEXT, nIndex, (LPARAM)szFont);
	BOOL bIsMonoSpaced = IsMonoSpaced(hdlg, szFont);

	// tell parent to display MONOSPACED
	::SendMessage(hdlg, WM_SHOW_MONOSPACED, bIsMonoSpaced, 0);
}

///////////////////////////////////////////////////////////////////////////////
// GetTM
static BOOL GetTM(HWND hdlg, int index, TEXTMETRIC *pTM)
{
	memset(pTM, 0, sizeof(TEXTMETRIC));

	HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
	ASSERT(hWndFont);
	if (!hWndFont)
		return FALSE;

	HWND hwndSample = GetDlgItem(hdlg, IDC_SAMPLE);
	ASSERT(hwndSample);
	if (!hwndSample)
		return FALSE;

	HDC hDC = ::GetDC(hwndSample);
	ASSERT(hDC);
	if (!hDC)
		return FALSE;

	TCHAR szFont[1000];
	szFont[0] = _T('\0');
	int rc = ::SendMessage(hWndFont, CB_GETLBTEXT, index, (LPARAM)szFont);

	BOOL bSuccess = FALSE;

	if (rc != CB_ERR && szFont[0] != _T('\0'))
	{
		LOGFONT lf;
		memset(&lf, 0, sizeof(lf));
		_tcscpy_s(lf.lfFaceName, szFont);
		lf.lfWeight = 400;
		lf.lfCharSet = DEFAULT_CHARSET;
		HFONT hFont = ::CreateFontIndirect(&lf);
		ASSERT(hFont);

		if (hFont)
		{
			HFONT hFontOld = (HFONT)::SelectObject(hDC, hFont);
			::GetTextMetrics(hDC, pTM);
			::SelectObject(hDC, hFontOld);
			::DeleteObject(hFont);
			bSuccess = TRUE;
		}
	}

	::ReleaseDC(hwndSample, hDC);

	return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////
// IsMonoSpaced
static BOOL IsMonoSpaced(HWND hdlg, LPCTSTR lpszFont)
{
	BOOL bIsMonoSpaced = FALSE;

	HWND hWndFont = ::GetDlgItem(hdlg, IDC_FONT);
	ASSERT(hWndFont);

	int nIndex = ::SendMessage(hWndFont, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)lpszFont);
	if (nIndex != CB_ERR)
	{
		TEXTMETRIC tm;
		BOOL bSuccess = GetTM(hdlg, nIndex, &tm);

		if (bSuccess)
			bIsMonoSpaced = (tm.tmPitchAndFamily & TMPF_FIXED_PITCH) == 0;
	}

	return bIsMonoSpaced;
}

///////////////////////////////////////////////////////////////////////////////
// RemoveFont
static BOOL RemoveFont(HWND hWnd, LPCTSTR lpszFont)
{
	BOOL bSuccess = FALSE;
	int nIndex = ::SendMessage(hWnd, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)lpszFont);
	if (nIndex != CB_ERR)
	{
		int rc = ::SendMessage(hWnd, CB_DELETESTRING, nIndex, 0);
		if (rc != CB_ERR)
		{
			TRACE(_T("removed font %s at %d\n"), lpszFont, nIndex);
			bSuccess = TRUE;
		}
	}
	return bSuccess;
}

