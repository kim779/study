// LineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineDlg dialog


CLineDlg::CLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineDlg)
	m_lineNum = 0;
	//}}AFX_DATA_INIT
}


void CLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineDlg)
	DDX_Text(pDX, IDC_EDITLINE, m_lineNum);
	DDV_MinMaxInt(pDX, m_lineNum, 0, 10000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineDlg, CDialog)
	//{{AFX_MSG_MAP(CLineDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineDlg message handlers
