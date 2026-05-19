// format.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "format.h"
#include "afxdialogex.h"

//#include "../h/form_w.h"
#include "../h/formOb.h"

// CFormat 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormat, CDialogEx)

CFormat::CFormat(CWnd* pParent, BYTE whatEdit, CString editFormat, DWORD attributes, CString name, int size)
	: CDialogEx(CFormat::IDD, pParent)
{
	m_whatEdit = whatEdit;
	m_editFormat = editFormat;
	m_name   = name;
	m_attributes = attributes;
	m_size   = size;

	m_edit = m_editFormat;
	m_first  = true;
}

CFormat::~CFormat()
{
}

void CFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FORMAT, m_editFormat);
	DDX_Text(pDX, IDC_SYMBOL, m_name);
}


BEGIN_MESSAGE_MAP(CFormat, CDialogEx)
	ON_EN_CHANGE(IDC_FORMAT, &CFormat::OnChangeFormat)
	ON_BN_CLICKED(IDC_NOEDIT, &CFormat::OnNoEdit)
END_MESSAGE_MAP()


// CFormat 메시지 처리기입니다.


BOOL CFormat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_whatEdit != weIT)
		m_editFormat = _T("");

	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFormat::OnOK()
{
	if (m_size == 0)
		return;

	UpdateData(TRUE);
	if (m_editFormat.IsEmpty())
		m_whatEdit = weNOP;
	else
		m_whatEdit = weIT;

	m_edit = m_editFormat;
	CDialogEx::OnOK();
}


void CFormat::OnChangeFormat()
{
	if (m_first || m_size == 0)
		return;

	UpdateData(TRUE);

	CString	edit = _T("");
	int	charN;
	int	tmpN = 0;

	edit = m_editFormat;
	charN = edit.GetLength();

	for (int ii = 0; ii < charN; ii++)
	{
		if (edit[ii] == _T('9'))
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


void CFormat::OnNoEdit()
{
	m_whatEdit = weNOP;
	m_editFormat = _T("");
	m_edit = m_editFormat;

	CDialogEx::OnOK();
}
