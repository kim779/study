// findTextDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "findTextDlg.h"
#include "afxdialogex.h"

#include "CCrystalTextView.h"


// CFindTextDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFindTextDlg, CDialogEx)

CFindTextDlg::CFindTextDlg(CCrystalTextView* pBuddy)
	: CDialogEx(CFindTextDlg::IDD, NULL)
{
	m_pBuddy = pBuddy;
	
	m_direction = 0;
	m_sText = _T("");
	m_matchCase = FALSE;
	m_wholeWord = FALSE;
	m_currentPt = CPoint(0, 0);
}

CFindTextDlg::~CFindTextDlg()
{
}

void CFindTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_EDIT_DIRECTION_UP, m_direction);
	DDX_Check(pDX, IDC_EDIT_MATCH_CASE, m_matchCase);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	DDX_Check(pDX, IDC_EDIT_WHOLE_WORD, m_wholeWord);
}


BEGIN_MESSAGE_MAP(CFindTextDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TEXT, &CFindTextDlg::OnChangeEditText)
END_MESSAGE_MAP()


// CFindTextDlg 메시지 처리기입니다.


BOOL CFindTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(m_sText != _T(""));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFindTextDlg::OnOK()
{
	if (UpdateData())
	{
		ASSERT(m_pBuddy != NULL);
		DWORD dwSearchFlags = 0;
		if (m_matchCase)
			dwSearchFlags |= FIND_MATCH_CASE;
		if (m_wholeWord)
			dwSearchFlags |= FIND_WHOLE_WORD;
		if (m_direction == 0)
			dwSearchFlags |= FIND_DIRECTION_UP;

		CPoint ptTextPos;
		if (!m_pBuddy->FindText(m_sText, m_currentPt, dwSearchFlags, TRUE, &ptTextPos))
		{
			CString prompt;
			prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_sText);
			AfxMessageBox(prompt);
			m_currentPt = CPoint(0, 0);
			return;
		}

		m_pBuddy->HighlightText(ptTextPos, lstrlen(m_sText));
		CDialogEx::OnOK();
	}
}


void CFindTextDlg::OnCancel()
{
	VERIFY(UpdateData());
	CDialogEx::OnCancel();
}


void CFindTextDlg::OnChangeEditText()
{
	CString text;

	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(text);
	GetDlgItem(IDOK)->EnableWindow(text != _T(""));
}
