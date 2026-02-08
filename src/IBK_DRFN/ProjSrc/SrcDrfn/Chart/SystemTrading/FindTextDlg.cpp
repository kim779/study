////////////////////////////////////////////////////////////////////////////
//	Implementation of the CFindTextDlg dialog, a part of Crystal Edit -
//	syntax coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FindTextDlg.h"
#include "CrystalTextView.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindTextDlg dialog

CFindTextDlg::CFindTextDlg(CCrystalTextView *pBuddy) : CDialog(CFindTextDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CFindTextDlg)
	m_nDirection = 1;
	m_bMatchCase = FALSE;
	m_bWholeWord = FALSE;
	m_sText = _T("");
	//}}AFX_DATA_INIT

	m_pBuddy = pBuddy;
	m_ptCurrentPos = CPoint(0, 0);
}

void CFindTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindTextDlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnFind);
	DDX_Radio(pDX, IDC_EDIT_DIRECTION_UP, m_nDirection);
	DDX_Check(pDX, IDC_EDIT_MATCH_CASE, m_bMatchCase);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sText);
	DDX_Check(pDX, IDC_EDIT_WHOLE_WORD, m_bWholeWord);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindTextDlg, CDialog)
	//{{AFX_MSG_MAP(CFindTextDlg)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindTextDlg message handlers

BOOL CFindTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDOK)->EnableWindow(m_sText != _T(""));

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP2);
	
	CImageList imageList;
	imageList.Create(24, 24, ILC_COLOR24|ILC_MASK, 4, 1);
	imageList.Add(&bitmap, RGB(0,255,0));

	m_btnFind.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));
//	m_btnCancel.SetIcon(CSize(24,24), imageList.ExtractIcon(1), imageList.ExtractIcon(2));

	m_btnFind.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnCancel.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	
	return TRUE;
}

void CFindTextDlg::OnOK() 
{
	if(UpdateData())
	{
		ASSERT(m_pBuddy != NULL);
		DWORD dwSearchFlags = 0;
		if(m_bMatchCase) dwSearchFlags |= FIND_MATCH_CASE;
		if(m_bWholeWord) dwSearchFlags |= FIND_WHOLE_WORD;
		if(m_nDirection == 0) dwSearchFlags |= FIND_DIRECTION_UP;

		CPoint ptTextPos;
		if(!m_pBuddy->FindText(m_sText, m_ptCurrentPos, dwSearchFlags, TRUE, &ptTextPos))
		{
			CString prompt;
			prompt.Format(IDS_EDIT_TEXT_NOT_FOUND, m_sText);
			AfxMessageBox(prompt);
			m_ptCurrentPos = CPoint(0, 0);
			return;
		}

		m_pBuddy->HighlightText(ptTextPos, lstrlen(m_sText));

		CDialog::OnOK();
	}
}

void CFindTextDlg::OnCancel() 
{
	VERIFY(UpdateData());

	CDialog::OnCancel();
}

void CFindTextDlg::OnChangeEditText() 
{
	CString text;
	GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(text);
	GetDlgItem(IDOK)->EnableWindow(text != _T(""));
}
