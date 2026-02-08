// CodeDuplicate.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "CodeDuplicate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeDuplicate dialog


CCodeDuplicate::CCodeDuplicate(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeDuplicate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeDuplicate)
	//}}AFX_DATA_INIT

	m_bAlert = FALSE;
}


void CCodeDuplicate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeDuplicate)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCodeDuplicate, CDialog)
	//{{AFX_MSG_MAP(CCodeDuplicate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeDuplicate message handlers

void CCodeDuplicate::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_bAlert  = TRUE;	
	CDialog::OnOK();
}

void CCodeDuplicate::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(TRUE);
	m_bAlert  = FALSE;
	
	CDialog::OnCancel();
}

BOOL CCodeDuplicate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
