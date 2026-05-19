// folderBrowser.cpp: implementation of the CFolderBrowser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "folderBrowser.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IDC_FOLDERTREE		0x3741
#define IDC_TITLE		0x3742
#define IDC_STATUS		0x3743
#define IDC_EDIT_PATH		0x3744

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolderBrowser::CFolderBrowser(CString initPath/*= _T("")*/, CWnd* pParent)
{
	m_hWnd = NULL;
	m_folderSub.m_folder = this;
	m_parentDisabled = false;

	// Get the true parent of the dialog
	m_parent = CWnd::GetSafeOwner(pParent);

	m_caption  = _T("Path");
	m_title    = _T("Path");
	m_initPath = initPath;

	ZeroMemory(&m_bi, sizeof(BROWSEINFO));
	m_bi.hwndOwner      = (m_parent == NULL) ? NULL : m_parent->GetSafeHwnd();
	m_bi.pszDisplayName = 0;
	m_bi.pidlRoot       = 0;
	m_bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.lpfn           = BrowseCallbackProc;
	m_bi.lpszTitle      = m_title;
}

CFolderBrowser::~CFolderBrowser()
{
}

CString CFolderBrowser::GetPathName()
{
	return m_path;
}

int CFolderBrowser::DoModal()
{
	LPITEMIDLIST	pidl;
	LPMALLOC	pMalloc;
	char	tmpPath[MAX_PATH];
	int	iResult = -1;

	m_bi.lpfn   = BrowseCallbackProc;
	m_bi.lParam = (LPARAM)this;
	m_bi.pszDisplayName = tmpPath;

	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		m_getSuccess = false;
		pidl = SHBrowseForFolder(&m_bi);
		if (pidl!=NULL) 
			pMalloc->Free(pidl);

		if (m_getSuccess)
			iResult = IDOK;

		pMalloc->Release();
	}

	if (m_parentDisabled && (m_parent != NULL))
		m_parent->EnableWindow(TRUE);

	m_parentDisabled = false;

	return iResult;
}

// Return Value 0: no error
//              1: path is invalid
//              2: path can not be created(validate==false)
int CFolderBrowser::Touch(CString path, bool validate)
{
	if (path.IsEmpty())
		return 1;

	CString	dirN;
	int	len = path.GetLength();

	// path must be "x:\..."
	if ((len < 3) || ((path.GetAt(0) < _T('A') || _T('Z') < path.GetAt(0))
		&& (path.GetAt(0) < _T('a') || _T('z') < path.GetAt(0))
		|| (path.GetAt(1) != _T(':')) || (path.GetAt(2) != _T('\\'))))
	{
		return 1;
	}

	if (len == 3)
	{
		if (!validate && _access(path, 0) != 0)
			return 2;

		return 0;
	}

	int	offs;
	bool	lastOne = false;
	CString	currentN;

	dirN = path.Left(3);
	for (int ii = 3; ii < len; ii++)
	{
		currentN = path.Mid(ii);
		offs = currentN.Find('\\');
		if (offs != -1)
		{
			currentN = currentN.Left(offs);
			ii += offs;
			lastOne = false;
		}
		else
			lastOne = true;

		if (!IsFileNameValid(currentN))
			return 1;

		if (!validate)
		{
			dirN += currentN;
			CreateDirectory(dirN, NULL);
			if (_access(dirN, 0) != 0)
				return 2;
		}

		if (lastOne)
			break;
		else
			dirN += '\\';
	}

	return (lastOne ? 0 : 1);
}

const TCHAR c_FolderDoesNotExist[] = _T(
		"The folder:\n"
		"%s\n\n"
		"This folder is not exitst.. Do you make thf folder?");
const TCHAR c_szErrInvalidPath[] = _T(
		"The folder:\n"
		"%s\n\n"
		"Incorrect path\n"
		"Input the correct folder name.");
const TCHAR c_szErrCreatePath[] = _T(
		"The folder:\n"
		"%s"
		"\n\nCan't create folder.");

// Return Value -1: user break;
//               0: no error
//               1: path is invalid
//               2: can not create path
int CFolderBrowser::MakeSurePathExists(CString path)	// manie
{
	CString	msg;
	int	iRet = 0;

	try
	{
		//validate path
		iRet = Touch(path, true);
		if (iRet != 0)
			throw iRet;

		if (!_access(path, 0))
			return 0;

		msg.Format(c_FolderDoesNotExist, path);
		if (AfxMessageBox(msg, MB_YESNO|MB_ICONQUESTION) != IDYES)
			return -1;

		//create path
		iRet = Touch(path, FALSE);
		if (iRet != 0)
			throw iRet; 

		return 0;
	}
	catch (int errCode)
	{
		switch (errCode)
		{
		case 1:
			msg.Format(c_szErrInvalidPath, path);
			break;
		case 2:
		default:
			msg.Format(c_szErrCreatePath, path);
			break;
		}

		AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION);
	}

	return iRet;
}

bool CFolderBrowser::IsFileNameValid(CString files)
{
	if (files.IsEmpty())
		return false;

	if (files.GetLength() <= 0)
		return false;

	// check first char
	switch (files.GetAt(0))
	{
	case '.':
	case ' ':
	case '\t':
		return false;
	}

	// check last char
	switch (files.GetAt(files.GetLength()-1))
	{
	case '.':
	case ' ':
	case '\t':
		return false;
	}

	// check all
	for (int ii = 0; ii < files.GetLength(); ii++)
	{
		switch (files.GetAt(ii))
		{
		case '\\':
		case '/':
		case ':':
		case '*':
		case '?':
		case '\"':
		case '<':
		case '>':
		case '|':
			return false;
		}
	}

	return true;
}

int CALLBACK CFolderBrowser::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam, LPARAM pData)
{
	CFolderBrowser* folder = (CFolderBrowser *)pData;

	switch (uMsg)
	{
	case BFFM_INITIALIZED: 
		{
			RECT	rc;
			HWND	hEdit;
			HFONT	hFont;

			folder->m_hWnd = hwnd;
			if (!folder->m_caption.IsEmpty())
				::SetWindowText(hwnd, folder->m_caption);

			VERIFY(folder->m_folderSub.SubclassWindow(hwnd));
			::ShowWindow(::GetDlgItem(hwnd, IDC_STATUS), SW_HIDE);
			::GetWindowRect(::GetDlgItem(hwnd, IDC_FOLDERTREE), &rc);
			rc.bottom = rc.top - 4;
			rc.top    = rc.bottom - 23;
			::ScreenToClient(hwnd, (LPPOINT)&rc);
			::ScreenToClient(hwnd, ((LPPOINT)&rc)+1);
			hEdit = ::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
					WS_CHILD|WS_TABSTOP|WS_VISIBLE|ES_AUTOHSCROLL,
					rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top,
					hwnd, NULL, NULL, NULL);
			::SetWindowLong(hEdit, GWL_ID, IDC_EDIT_PATH);
			::ShowWindow(hEdit, SW_SHOW);

			hFont = (HFONT)::SendMessage(hwnd, WM_GETFONT, 0, 0);
			::SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

			CString	path = folder->m_initPath;
			char	tmpb[MAX_PATH];
			if (path.IsEmpty())
			{
				::GetCurrentDirectory(MAX_PATH, tmpb);
				path = tmpb;
			}
			// WParam is TRUE since you are passing a path.
			// It would be FALSE if you were passing a pidl.
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(char *)path.operator LPCTSTR());
		}
		break;

	case BFFM_SELCHANGED:
		{
			char	selb[MAX_PATH];
			if (!::SHGetPathFromIDList((LPITEMIDLIST)lParam, selb) || selb[1]!=':')
			{
				selb[0] = '\0';
				::SendMessage(hwnd, BFFM_ENABLEOK, 0, FALSE);
			}
			else
				::SendMessage(hwnd, BFFM_ENABLEOK, 0, TRUE);

			::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)selb);
			::SetWindowText(::GetDlgItem(hwnd, IDC_EDIT_PATH), selb);
		}
		break;

	default:
		break;
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////
//	Class	CFolderBrowserSub
//
BEGIN_MESSAGE_MAP(CFolderBrowserSub, CWnd)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_EN_CHANGE(IDC_EDIT_PATH, OnChangePath)
END_MESSAGE_MAP()

void CFolderBrowserSub::OnOK()
{
	::GetWindowText(::GetDlgItem(m_hWnd, IDC_EDIT_PATH), m_folder->m_path, MAX_PATH);
	if (!CFolderBrowser::MakeSurePathExists(m_folder->m_path))
	{
		m_folder->m_getSuccess = true;
		::EndDialog(m_folder->m_hWnd, IDOK);
	}
	else
		::SetFocus(::GetDlgItem(m_hWnd, IDC_EDIT_PATH));
}

void CFolderBrowserSub::OnChangePath()
{
	::GetWindowText(::GetDlgItem(m_hWnd, IDC_EDIT_PATH), m_folder->m_path, MAX_PATH);
	BOOL	enableOKButton = (strlen(m_folder->m_path) > 0) ? TRUE : FALSE;
	SendMessage(BFFM_ENABLEOK, 0, enableOKButton);
}
