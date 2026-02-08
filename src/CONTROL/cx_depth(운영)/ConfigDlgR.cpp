// ConfigDlgR.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Depth.h"
#include "ConfigDlgR.h"
#include "depth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgR dialog


CConfigDlgR::CConfigDlgR(struct _config *pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlgR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlgR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pConfig = pConfig;
}


void CConfigDlgR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlgR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlgR, CDialog)
	//{{AFX_MSG_MAP(CConfigDlgR)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlgR message handlers
int RRadio[] = { IDC_RADIO1, IDC_RADIO2, IDC_RADIO3, IDC_RADIO4};
BOOL CConfigDlgR::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPoint	pt;
	GetCursorPos(&pt);
	CRect	rc;
	GetWindowRect(&rc);
	MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());
	
	int	sel = (m_pConfig->info > 3)? 0:m_pConfig->info;
	((CButton*)GetDlgItem(RRadio[sel]))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlgR::OnOK() 
{
	for (int ii = 0; ii < 4; ii++)
	{
		if (((CButton *)GetDlgItem(RRadio[ii]))->GetCheck())
		{
			m_pConfig->info = ii;
			break;
		}
	}
	
	CDialog::OnOK();
}

void CConfigDlgR::OnRadio1() {	OnOK();	}
void CConfigDlgR::OnRadio2() {	OnOK();	}
void CConfigDlgR::OnRadio3() {	OnOK();	}
void CConfigDlgR::OnRadio4() {	OnOK();	}

