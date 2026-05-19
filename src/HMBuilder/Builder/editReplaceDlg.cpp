// editReplaceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "editReplaceDlg.h"
#include "afxdialogex.h"

#include "CCrystalEditView.h"


// CEditReplaceDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditReplaceDlg, CDialogEx)

CEditReplaceDlg::CEditReplaceDlg(CCrystalEditView* pBuddy)
	: CDialogEx(CEditReplaceDlg::IDD, NULL)
{
	ASSERT(pBuddy != NULL);
	m_pBuddy = pBuddy;

	m_matchCase = FALSE;
	m_wholeWord = FALSE;
	m_sText = _T("");
	m_sNewText = _T("");
	m_scope = -1;
	m_enableScopeSelection = TRUE;
}

CEditReplaceDlg::~CEditReplaceDlg()
{
}

void CEditReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_EDIT_MATCH_CASE, m_matchCase);
	DDX_Check(pDX, IDC_EDIT_WHOLE_WORD, m_wholeWord);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	DDX_Text(pDX, IDC_EDIT_REPLACE_WITH, m_sNewText);
	DDX_Radio(pDX, IDC_EDIT_SCOPE_SELECTION, m_scope);
}


BEGIN_MESSAGE_MAP(CEditReplaceDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	ON_BN_CLICKED(IDC_EDIT_REPLACE, OnEditReplace)
	ON_BN_CLICKED(IDC_EDIT_REPLACE_ALL, OnEditReplaceAll)
	ON_BN_CLICKED(IDC_EDIT_SKIP, OnEditSkip)
END_MESSAGE_MAP()


// CEditReplaceDlg 메시지 처리기입니다.


BOOL CEditReplaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT_SKIP)->EnableWindow(m_sText != _T(""));
	GetDlgItem(IDC_EDIT_SCOPE_SELECTION)->EnableWindow(m_enableScopeSelection);
	m_found = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEditReplaceDlg::OnCancel()
{
	VERIFY(UpdateData());
	CDialogEx::OnCancel();
}

void CEditReplaceDlg::OnChangeEditText()
{
	CString text;
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(text);
	GetDlgItem(IDC_EDIT_SKIP)->EnableWindow(text != _T(""));
}

void CEditReplaceDlg::OnEditReplace()
{
	if (! UpdateData())
		return;

	if (!m_found)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_found = doHighlightText();
		return;
	}

	//	We have highlighted text
	VERIFY(m_pBuddy->ReplaceSelection(m_sNewText));

	//	Manually recalculate points
	if (m_enableScopeSelection)
	{
		if (m_ptBlockBegin.y == m_ptFoundAt.y && m_ptBlockBegin.x > m_ptFoundAt.x)
		{
			m_ptBlockBegin.x -= lstrlen(m_sText);
			m_ptBlockBegin.x += lstrlen(m_sNewText);
		}
		if (m_ptBlockEnd.y == m_ptFoundAt.y && m_ptBlockEnd.x > m_ptFoundAt.x)
		{
			m_ptBlockEnd.x -= lstrlen(m_sText);
			m_ptBlockEnd.x += lstrlen(m_sNewText);
		}
	}
	m_ptFoundAt.x += lstrlen(m_sNewText);
	m_found = doHighlightText();
}

void CEditReplaceDlg::OnEditReplaceAll()
{
	if (!UpdateData())
		return;

	if (!m_found)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_found = doHighlightText();
	}

	while (m_found)
	{
		//	We have highlighted text
		VERIFY(m_pBuddy->ReplaceSelection(m_sNewText));

		//	Manually recalculate points
		if (m_enableScopeSelection)
		{
			if (m_ptBlockBegin.y == m_ptFoundAt.y && m_ptBlockBegin.x > m_ptFoundAt.x)
			{
				m_ptBlockBegin.x -= lstrlen(m_sText);
				m_ptBlockBegin.x += lstrlen(m_sNewText);
			}
			if (m_ptBlockEnd.y == m_ptFoundAt.y && m_ptBlockEnd.x > m_ptFoundAt.x)
			{
				m_ptBlockEnd.x -= lstrlen(m_sText);
				m_ptBlockEnd.x += lstrlen(m_sNewText);
			}
		}
		m_ptFoundAt.x += lstrlen(m_sNewText);
		m_found = doHighlightText();
	}
}

void CEditReplaceDlg::OnEditSkip()
{
	if (!UpdateData())
		return;

	if (!m_found)
	{
		m_ptFoundAt = m_ptCurrentPos;
		m_found = doHighlightText();
		return;
	}

	m_ptFoundAt.x += 1;
	m_found = doHighlightText();
}

BOOL CEditReplaceDlg::doHighlightText()
{
	ASSERT(m_pBuddy != NULL);

	BOOL	bFound;
	DWORD	dwSearchFlags = 0;

	if (m_matchCase)
		dwSearchFlags |= FIND_MATCH_CASE;
	if (m_wholeWord)
		dwSearchFlags |= FIND_WHOLE_WORD;

	if (m_scope == 0)
	{
		//	Searching selection only
		bFound = m_pBuddy->FindTextInBlock(m_sText, m_ptFoundAt, m_ptBlockBegin, m_ptBlockEnd, dwSearchFlags, FALSE, &m_ptFoundAt);
	}
	else
	{
		//	Searching whole text
		bFound = m_pBuddy->FindText(m_sText, m_ptFoundAt, dwSearchFlags, FALSE, &m_ptFoundAt);
	}

	if (!bFound)
	{
		CString prompt;

		prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_sText);
		AfxMessageBox(prompt);
		m_ptCurrentPos = m_scope == 0 ? m_ptBlockBegin : CPoint(0, 0);
		return FALSE;
	}

	m_pBuddy->HighlightText(m_ptFoundAt, lstrlen(m_sText));
	return TRUE;
}
