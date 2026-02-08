// ConfigDlgL.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
#include "ConfigDlgL.h"
#include "depth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgL dialog


CConfigDlgL::CConfigDlgL(struct _config *pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlgL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlgL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pConfig = pConfig;
}


void CConfigDlgL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlgL)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlgL, CDialog)
	//{{AFX_MSG_MAP(CConfigDlgL)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgL message handlers
int LRadio[] = { IDC_RADIO1, IDC_RADIO2, IDC_RADIO3};
BOOL CConfigDlgL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPoint	pt;
	GetCursorPos(&pt);
	CRect	rc;
	GetWindowRect(&rc);
	MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());

	int	sel = (m_pConfig->mbong > 2)? 0:m_pConfig->mbong;
	((CButton*)GetDlgItem(LRadio[sel]))->SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlgL::OnOK() 
{
	for (int ii = 0; ii < 3; ii++)
	{
		if (((CButton *)GetDlgItem(LRadio[ii]))->GetCheck())
		{
			m_pConfig->mbong = ii;
			break;
		}
	}
	
	CDialog::OnOK();
}

void CConfigDlgL::OnRadio1() {	OnOK();	}
void CConfigDlgL::OnRadio2() {	OnOK();	}
void CConfigDlgL::OnRadio3() {	OnOK();	}
