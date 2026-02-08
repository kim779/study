// MapNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wdrchartdgndlg.h"
#include "MapNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapNameDlg dialog


CMapNameDlg::CMapNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapNameDlg)
	m_stCtrlCode = _T("");
	m_strMapName = _T("");
	//}}AFX_DATA_INIT
}


void CMapNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapNameDlg)
	DDX_Text(pDX, IDC_EDIT_CTRLCODE, m_stCtrlCode);
	DDX_Text(pDX, IDC_EDIT_MAPCODE, m_strMapName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapNameDlg, CDialog)
	//{{AFX_MSG_MAP(CMapNameDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapNameDlg message handlers
