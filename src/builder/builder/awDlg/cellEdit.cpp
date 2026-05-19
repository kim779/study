// cellEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cellEdit.h"
#include "afxdialogex.h"

#include "coRGB.h"
//#include "../h/form_w.h"
#include "../h/formOb.h"
#include "../h/mapvar.h"
#include "../h/mainvar.h"


// CCellEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCellEdit, CDialogEx)

CCellEdit::CCellEdit(CWnd* pParent, BYTE kind, BYTE charType, BYTE whatEdit, DWORD auxValues, CString editFormat, CString name, DWORD attributes, int size)
	: CDialogEx(CCellEdit::IDD, pParent)
{
	m_title = _T("");

	m_kind   = kind;
	m_type = charType;

	m_whatEdit = whatEdit;
	m_auxValues = auxValues;
	m_editFormat = editFormat;
	m_attributes = attributes;
	m_size   = size;
	m_name   = name;

	m_first  = true;
}

CCellEdit::~CCellEdit()
{
}

void CCellEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_editFormat);
	DDX_Text(pDX, IDC_TITLE, m_title);
}


BEGIN_MESSAGE_MAP(CCellEdit, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT, &CCellEdit::OnChangeEdit)
	ON_BN_CLICKED(IDC_CORGB, &CCellEdit::OnColor)
	ON_COMMAND(IDC_IE1, &CCellEdit::OnNOP)
	ON_COMMAND(IDC_IE2, &CCellEdit::OnFormat)
	ON_COMMAND(IDC_IE3, &CCellEdit::OnCoRGB)
END_MESSAGE_MAP()


// CCellEdit 메시지 처리기입니다.


BOOL CCellEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!(m_type == ctNUMERIC || m_type == ctALPHA))
		GetDlgItem(IDC_IE2)->EnableWindow(FALSE);

	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);
	switch (m_whatEdit)
	{
	case weNOP:
		((CButton *)GetDlgItem(IDC_IE1))->SetCheck(1);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
		m_title = _T("No Edit");
		break;

	case weIT:
		((CButton *)GetDlgItem(IDC_IE2))->SetCheck(1);
		m_title = _T("Edit Format");
		break;

	case weCORGB:
		((CButton *)GetDlgItem(IDC_IE3))->SetCheck(1);
		m_title  = _T("Conditioanl Color");
		m_editFormat = _T("");
		GetDlgItem(IDC_CORGB)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
		break;
	default :
		break;
	}
	
	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCellEdit::OnOK()
{
	UpdateData(TRUE);

	switch (m_whatEdit)
	{
	case weIT:
		m_whatEdit = m_editFormat.IsEmpty() ? weNOP : weIT;
		break;
	case weNOP:
	case weCORGB:
	default:
		break;
	}
	
	CDialogEx::OnOK();
}


void CCellEdit::OnChangeEdit()
{
	if (m_first || m_size == 0)
		return;

	UpdateData(TRUE);
	if (((CButton *)GetDlgItem(IDC_IE2))->GetCheck() == 1)
	{
		CString	edit = m_editFormat;
		int	charN, tmpN;

		tmpN  = 0;
		charN = edit.GetLength();
		for (int ii = 0; ii < charN; ii++)
		{
			if (edit[ii] == '9')
			{
				if (tmpN == m_size)
				{
					m_editFormat = CString(edit, ii);
					UpdateData(FALSE);
					return;
				}
				tmpN++;
			}
		}
	}
}


void CCellEdit::OnColor()
{
	CCoRGB*	coRGB;

	if (m_whatEdit == weCORGB)
		coRGB = new CCoRGB(this, m_name, m_whatEdit, HIWORD(m_auxValues), LOWORD(m_auxValues));
	else
	{
		long	lCorgb = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
		coRGB = new CCoRGB(this, m_name, m_whatEdit, LOWORD(lCorgb), HIWORD(lCorgb));
	}

	if (coRGB->DoModal() == IDOK)
	{
		m_whatEdit = coRGB->m_whatEdit;
		if (m_whatEdit == weCORGB)
		{
			m_auxValues = MAKELONG(coRGB->m_dnRGB, coRGB->m_upRGB);
			m_attributes   |= faCORGB;
		}
		else
		{
			m_auxValues = 0;
			m_attributes  &= ~faCORGB;
		}
	}
	delete coRGB;
}


void CCellEdit::OnNOP()
{
	m_whatEdit = weNOP;
	m_title  = _T("No Edit");
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}


void CCellEdit::OnFormat()
{
	GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	m_title = _T("Edit Format");
	if (m_whatEdit == weIT)
		m_editFormat = _T("");

	m_whatEdit = weIT;
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT)->SetFocus();

	UpdateData(FALSE);
}


void CCellEdit::OnCoRGB()
{
	m_title = _T("Conditioanl Color");
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CORGB)->ShowWindow(SW_SHOW);

	UpdateData(FALSE);
}
