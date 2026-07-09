// format.cpp : implementation file
//

#include "stdafx.h"
#include "format.h"

#include "../form_w.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormat dialog


CFormat::CFormat(CWnd* pParent, BYTE isEdit, char* onEdit, DWORD attr, char* name, int size)
	: CDialog(CFormat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormat)
	m_onEdit = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT

	m_isEdit = isEdit;
	m_onEdit = onEdit;
	m_name   = name;
	m_attr   = attr;
	m_size   = size;

	m_edit   = m_onEdit;
	m_first  = true;
}


void CFormat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormat)
	DDX_Text(pDX, IDC_FORMAT, m_onEdit);
	DDX_Text(pDX, IDC_SYMBOL, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormat, CDialog)
	//{{AFX_MSG_MAP(CFormat)
	ON_BN_CLICKED(IDC_NOEDIT, OnNoEdit)
	ON_EN_CHANGE(IDC_FORMAT, OnChangeFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormat message handlers

BOOL CFormat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_isEdit != IE_EDIT)
		m_onEdit = _T("");

	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormat::OnOK() 
{
	if (m_size == 0)
		return;

	UpdateData(TRUE);
	if (m_onEdit.IsEmpty())
		m_isEdit = IE_NOP;
	else
		m_isEdit = IE_EDIT;

	m_edit = m_onEdit;
	CDialog::OnOK();
}

void CFormat::OnNoEdit() 
{
	m_isEdit = IE_NOP;
	m_onEdit = _T("");
	m_edit   = m_onEdit;

	CDialog::OnOK();
}

void CFormat::OnChangeFormat() 
{
	if (m_first || m_size == 0)
		return;

	UpdateData(TRUE);

	CString	edit = _T("");
	int	charN;
	int	tmpN = 0;

	edit  = m_onEdit;
	charN = edit.GetLength();

	for (int ii = 0; ii < charN; ii++)
	{
		if (edit[ii] == '9')
		{
			if (tmpN == m_size)
			{
				m_onEdit = CString(edit, ii);
				UpdateData(FALSE);
				return;
			}
			tmpN++;
		}
	}
}
