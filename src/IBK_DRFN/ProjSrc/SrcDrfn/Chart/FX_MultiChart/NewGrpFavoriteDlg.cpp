// NewGrpFavoriteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewGrpFavoriteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewGrpFavoriteDlg dialog


CNewGrpFavoriteDlg::CNewGrpFavoriteDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CNewGrpFavoriteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGrpFavoriteDlg)
	m_strNewName = _T("");
	//}}AFX_DATA_INIT
}


void CNewGrpFavoriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGrpFavoriteDlg)
	DDX_Text(pDX, IDC_EDITNEWNAME, m_strNewName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGrpFavoriteDlg, CRscDlg)
	//{{AFX_MSG_MAP(CNewGrpFavoriteDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGrpFavoriteDlg message handlers

BOOL CNewGrpFavoriteDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDITNEWNAME)->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
