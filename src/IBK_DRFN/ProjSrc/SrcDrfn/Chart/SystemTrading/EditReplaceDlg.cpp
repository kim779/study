////////////////////////////////////////////////////////////////////////////
//	Implementation of the CEditReplaceDlg dialog, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditReplaceDlg.h"

#include "CrystalEditView.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditReplaceDlg dialog

CEditReplaceDlg::CEditReplaceDlg(CCrystalEditView *pBuddy) : CDialog(CEditReplaceDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CEditReplaceDlg)
	m_bMatchCase = FALSE;
	m_bWholeWord = FALSE;
	m_sText = _T("");
	m_sNewText = _T("");
	m_nScope = -1;
	//}}AFX_DATA_INIT

	ASSERT(pBuddy != NULL);
	m_pBuddy = pBuddy;
	m_bEnableScopeSelection = TRUE;
}

void CEditReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditReplaceDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_SKIP, m_btnFind);
	DDX_Control(pDX, IDC_EDIT_REPLACE_ALL, m_btnReplaceAll);
	DDX_Control(pDX, IDC_EDIT_REPLACE, m_btnReplace);
	DDX_Check(pDX, IDC_EDIT_MATCH_CASE, m_bMatchCase);
	DDX_Check(pDX, IDC_EDIT_WHOLE_WORD, m_bWholeWord);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	DDX_Text(pDX, IDC_EDIT_REPLACE_WITH, m_sNewText);
	DDX_Radio(pDX, IDC_EDIT_SCOPE_SELECTION, m_nScope);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CEditReplaceDlg)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	ON_BN_CLICKED(IDC_EDIT_REPLACE, OnEditReplace)
	ON_BN_CLICKED(IDC_EDIT_REPLACE_ALL, OnEditReplaceAll)
	ON_BN_CLICKED(IDC_EDIT_SKIP, OnEditSkip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditReplaceDlg message handlers

BOOL CEditReplaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_SKIP)->EnableWindow(m_sText != _T(""));
	GetDlgItem(IDC_EDIT_SCOPE_SELECTION)->EnableWindow(m_bEnableScopeSelection);
	m_bFound = FALSE;

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP2);
	
	CImageList imageList;
	imageList.Create(24, 24, ILC_COLOR24|ILC_MASK, 4, 1);
	imageList.Add(&bitmap, RGB(0,255,0));

	m_btnFind.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));
	m_btnReplace.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));
//	m_btnCancel.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));

	m_btnFind.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnReplace.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnReplaceAll.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnCancel.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);

	return TRUE;
}

void CEditReplaceDlg::OnCancel() 
{
	VERIFY(UpdateData());

	CDialog::OnCancel();
}

void CEditReplaceDlg::OnChangeEditText() 
{
	CString text;
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(text);
	GetDlgItem(IDC_EDIT_SKIP)->EnableWindow(text != _T(""));
}

BOOL CEditReplaceDlg::DoHighlightText()
{
	ASSERT(m_pBuddy != NULL);
	DWORD dwSearchFlags = 0;
	if(m_bMatchCase) dwSearchFlags |= FIND_MATCH_CASE;
	if(m_bWholeWord) dwSearchFlags |= FIND_WHOLE_WORD;

	BOOL bFound;
	if(m_nScope == 0)
	{
		// Searching selection only
		bFound = m_pBuddy->FindTextInBlock(m_sText, m_ptFoundAt, m_ptBlockBegin, m_ptBlockEnd, dwSearchFlags, FALSE, &m_ptFoundAt);
	}
	else
	{
		// Searching whole text
		bFound = m_pBuddy->FindText(m_sText, m_ptFoundAt, dwSearchFlags, FALSE, &m_ptFoundAt);
	}

	if(!bFound)
	{
		CString prompt;
		prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_sText);
		AfxMessageBox(prompt);
		m_ptCurrentPos = m_nScope == 0 ? m_ptBlockBegin : CPoint(0, 0);
		return FALSE;
	}

	m_pBuddy->HighlightText(m_ptFoundAt, lstrlen(m_sText));
	return TRUE;
}

void CEditReplaceDlg::OnEditSkip() 
{
	if(!UpdateData()) return;

	if(!m_bFound)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_bFound = DoHighlightText();
		return;
	}

	m_ptFoundAt.x += 1;
	m_bFound = DoHighlightText();
}

void CEditReplaceDlg::OnEditReplace() 
{
	if(!UpdateData()) return;

	if(!m_bFound)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_bFound = DoHighlightText();
		return;
	}

	// We have highlighted text
	VERIFY(m_pBuddy->ReplaceSelection(m_sNewText));

	// Manually recalculate points
	if(m_bEnableScopeSelection)
	{
		if(m_ptBlockBegin.y == m_ptFoundAt.y && m_ptBlockBegin.x > m_ptFoundAt.x)
		{
			m_ptBlockBegin.x -= lstrlen(m_sText);
			m_ptBlockBegin.x += lstrlen(m_sNewText);
		}
		if(m_ptBlockEnd.y == m_ptFoundAt.y && m_ptBlockEnd.x > m_ptFoundAt.x)
		{
			m_ptBlockEnd.x -= lstrlen(m_sText);
			m_ptBlockEnd.x += lstrlen(m_sNewText);
		}
	}
	m_ptFoundAt.x += lstrlen(m_sNewText);
	m_bFound = DoHighlightText();
}

void CEditReplaceDlg::OnEditReplaceAll() 
{
	if(!UpdateData()) return;

	if(!m_bFound)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_bFound = DoHighlightText();
	}

	while(m_bFound)
	{
		// We have highlighted text
		VERIFY(m_pBuddy->ReplaceSelection(m_sNewText));

		// Manually recalculate points
		if(m_bEnableScopeSelection)
		{
			if(m_ptBlockBegin.y == m_ptFoundAt.y && m_ptBlockBegin.x > m_ptFoundAt.x)
			{
				m_ptBlockBegin.x -= lstrlen(m_sText);
				m_ptBlockBegin.x += lstrlen(m_sNewText);
			}
			if(m_ptBlockEnd.y == m_ptFoundAt.y && m_ptBlockEnd.x > m_ptFoundAt.x)
			{
				m_ptBlockEnd.x -= lstrlen(m_sText);
				m_ptBlockEnd.x += lstrlen(m_sNewText);
			}
		}
		m_ptFoundAt.x += lstrlen(m_sNewText);
		m_bFound = DoHighlightText();
	}
}
