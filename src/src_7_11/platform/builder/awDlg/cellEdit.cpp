// cellEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cellEdit.h"
#include "coRGB.h"

#include "../form_w.h"
#include "../mapvar.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellEdit dialog

CCellEdit::CCellEdit(CWnd* pParent, BYTE kind, BYTE type, BYTE isEdit, DWORD onEdit, char* editS,
		     CString name, DWORD attr, int size)
	: CDialog(CCellEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCellEdit)
	m_format = _T("");
	m_title  = _T("");
	//}}AFX_DATA_INIT

	m_kind   = kind;
	m_type   = type;

	m_isEdit = isEdit;
	m_onEdit = onEdit;
	m_format = editS;
	m_attr   = attr;
	m_size   = size;
	m_name   = name;

	m_first  = true;
}


void CCellEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCellEdit)
	DDX_Text(pDX, IDC_EDIT, m_format);
	DDX_Text(pDX, IDC_TITLE, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCellEdit, CDialog)
	//{{AFX_MSG_MAP(CCellEdit)
	ON_BN_CLICKED(IDC_CORGB, OnColor)
	ON_EN_CHANGE(IDC_EDIT, OnChangeEdit)
	ON_BN_CLICKED(IDC_IE1, OnNOP)
	ON_BN_CLICKED(IDC_IE2, OnFORMAT)
	ON_BN_CLICKED(IDC_IE3, OnCORGB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellEdit message handlers

BOOL CCellEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (!(m_type == IO_NUM || m_type == IO_ALPHA))
		GetDlgItem(IDC_IE2)->EnableWindow(FALSE);

	CButton*	opt;
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);
	switch (m_isEdit)
	{
	case IE_NOP:
		opt = (CButton *) GetDlgItem(IDC_IE1);
		opt->SetCheck(1);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
		m_title = _T("No Edit");
		break;
	case IE_EDIT:
		opt = (CButton *) GetDlgItem(IDC_IE2);
		opt->SetCheck(1);
		m_title = _T("Edit Format");
		break;
	case IE_CORGB:
		opt = (CButton *) GetDlgItem(IDC_IE3);
		opt->SetCheck(1);
		m_title  = _T("Conditioanl Color");
		m_format = _T("");
		GetDlgItem(IDC_CORGB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
		break;
	default :
		break;
	}
	
	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCellEdit::OnColor() 
{
	CCoRGB*		coRGB;

	if (m_isEdit == IE_CORGB)
		coRGB = new CCoRGB(this, m_name, m_isEdit, HIWORD(m_onEdit), LOWORD(m_onEdit));
	else
	{
		long lCorgb = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
		coRGB = new CCoRGB(this, m_name, m_isEdit, LOWORD(lCorgb), HIWORD(lCorgb));
	}

	if (coRGB->DoModal() == IDOK)
	{
		m_isEdit = coRGB->m_isEdit;
		if (m_isEdit == IE_CORGB)
		{
			m_onEdit = MAKELONG(coRGB->m_dnRGB, coRGB->m_upRGB);
			m_attr   |= FA_CORGB;
		}
		else
		{
			m_onEdit = 0;
			m_attr  &= ~FA_CORGB;
		}
	}
	delete coRGB;
}

void CCellEdit::OnChangeEdit() 
{
	if (m_first || m_size == 0)
		return;

	UpdateData(TRUE);

	CButton* opt = (CButton *) GetDlgItem(IDC_IE2);
	if (opt->GetCheck() == 1)
	{
		CString	edit = m_format;
		int	charN, tmpN;

		tmpN = 0;
		charN = edit.GetLength();
		for (int ii = 0; ii < charN; ii++)
		{
			if (edit[ii] == '9')
			{
				if (tmpN == m_size)
				{
					m_format = CString(edit, ii);
					UpdateData(FALSE);
					return;
				}
				tmpN++;
			}
		}
	}
}

void CCellEdit::OnNOP() 
{
	m_isEdit = IE_NOP;
	m_title  = _T("No Edit");
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void CCellEdit::OnFORMAT() 
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	m_title = _T("Edit Format");
	if (m_isEdit == IE_EDIT)
		m_format = _T("");

	m_isEdit = IE_EDIT;
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT)->SetFocus();

	UpdateData(FALSE);
}

void CCellEdit::OnCORGB() 
{
	m_title = _T("Conditioanl Color");
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_SHOW);

	UpdateData(FALSE);
}

void CCellEdit::OnOK() 
{
	UpdateData(TRUE);

	switch (m_isEdit)
	{
	case IE_EDIT:
		m_isEdit = m_format.IsEmpty() ? IE_NOP : IE_EDIT;
		break;
	case IE_NOP:
	case IE_CORGB:
	default:
		break;
	}
	
	CDialog::OnOK();
}
