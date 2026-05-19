// FontSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "FontSetDlg.h"
#include "chaser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontSetDlg dialog


CFontSetDlg::CFontSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for (int ii = 0; ii < IDX_MAX; ii++)
	{
		m_bBold[ii]     = FALSE;
		m_bItalic[ii]   = FALSE;
		m_bUline[ii]    = FALSE;
		m_bStrike[ii]   = FALSE;
	}
}


void CFontSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontSetDlg, CDialog)
	//{{AFX_MSG_MAP(CFontSetDlg)
	ON_BN_CLICKED(IDC_SETALLFONT, OnSetallfont)
	ON_BN_CLICKED(IDC_SETSNRFONT, OnSetsnrfont)
	ON_BN_CLICKED(IDC_SETRTMFONT, OnSetrtmfont)
	ON_BN_CLICKED(IDC_SETSYSFONT, OnSetsysfont)
	ON_BN_CLICKED(IDC_SETRPTFONT, OnSetrptfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontSetDlg message handlers

BOOL CFontSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	loadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFontSetDlg::OnOK() 
{
	saveInfo();
	CDialog::OnOK();
}

void CFontSetDlg::OnSetallfont() 
{
	ChangeFont(IDX_ALL);
}

void CFontSetDlg::OnSetsnrfont() 
{
	ChangeFont(IDX_SNR);
}

void CFontSetDlg::OnSetrtmfont() 
{
	ChangeFont(IDX_RTM);
}

void CFontSetDlg::OnSetsysfont() 
{
	ChangeFont(IDX_SYS);
}

void CFontSetDlg::OnSetrptfont() 
{
	ChangeFont(IDX_RPT);
}

void CFontSetDlg::loadInfo()
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	if (app->GetProfileInt(TRACESETTING, SAMEFONT, 1))
		((CButton *) GetDlgItem(IDC_ALLFONTSETTING))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_EACHFONTSETTING))->SetCheck(1);
	
	m_fName[IDX_ALL] = app->GetProfileString(TRACESETTING, ALLFONT, DEFAULTFONT);
	m_fName[IDX_SNR] = app->GetProfileString(TRACESETTING, SNRFONT, DEFAULTFONT);
	m_fName[IDX_RTM] = app->GetProfileString(TRACESETTING, RTMFONT, DEFAULTFONT);
	m_fName[IDX_SYS] = app->GetProfileString(TRACESETTING, SYSFONT, DEFAULTFONT);
	m_fName[IDX_RPT] = app->GetProfileString(TRACESETTING, RPTFONT, DEFAULTFONT);

	m_fSize[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLSIZE, DEFAULTFONTN);
	m_fSize[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRSIZE, DEFAULTFONTN);
	m_fSize[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMSIZE, DEFAULTFONTN);
	m_fSize[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSSIZE, DEFAULTFONTN);
	m_fSize[IDX_RPT] = app->GetProfileInt(TRACESETTING, RPTSIZE, DEFAULTFONTN);

	for (int ii = 0; ii < IDX_MAX; ii++)
	{
		UINT	nRes;
		CString tmps;
		switch (ii)
		{
		case IDX_ALL:	nRes = IDC_ALLFONT;	break;
		case IDX_SNR:	nRes = IDC_SNRFONT;	break;
		case IDX_RTM:	nRes = IDC_RTMFONT;	break;
		case IDX_SYS:	nRes = IDC_SYSFONT;	break;
		case IDX_RPT:	nRes = IDC_RPTFONT;	break;
		default:	continue;
		}

		tmps.Format(" %d, %s", m_fSize[ii], m_fName[ii]);
		GetDlgItem(nRes)->SetWindowText(tmps);
	}

	m_bBold[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allBold", 0);
	m_bBold[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrBold", 0);
	m_bBold[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmBold", 0);
	m_bBold[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysBold", 0);
	m_bBold[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptBold", 0);

	m_bItalic[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allItalic", 0);
	m_bItalic[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrItalic", 0);
	m_bItalic[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmItalic", 0);
	m_bItalic[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysItalic", 0);
	m_bItalic[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptItalic", 0);

	m_bUline[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allUline", 0);
	m_bUline[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrUline", 0);
	m_bUline[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmUline", 0);
	m_bUline[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysUline", 0);
	m_bUline[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptUline", 0);

	m_bStrike[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allStrike", 0);
	m_bStrike[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrStrike", 0);
	m_bStrike[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmStrike", 0);
	m_bStrike[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysStrike", 0);
	m_bStrike[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptStrike", 0);
}


void CFontSetDlg::saveInfo()
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();

	
	if (((CButton *) GetDlgItem(IDC_ALLFONTSETTING))->GetCheck() == BST_CHECKED)
		app->WriteProfileInt(TRACESETTING, SAMEFONT, 1);
	else	app->WriteProfileInt(TRACESETTING, SAMEFONT, 0);

	app->WriteProfileString(TRACESETTING, ALLFONT, m_fName[IDX_ALL]);
	app->WriteProfileString(TRACESETTING, SNRFONT, m_fName[IDX_SNR]);
	app->WriteProfileString(TRACESETTING, RTMFONT, m_fName[IDX_RTM]);
	app->WriteProfileString(TRACESETTING, SYSFONT, m_fName[IDX_SYS]);
	app->WriteProfileString(TRACESETTING, RPTFONT, m_fName[IDX_RPT]);

	app->WriteProfileInt(TRACESETTING, ALLSIZE, m_fSize[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, SNRSIZE, m_fSize[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, RTMSIZE, m_fSize[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, SYSSIZE, m_fSize[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, RPTSIZE, m_fSize[IDX_RPT]);

	app->WriteProfileInt(TRACESETTING, "allBold", m_bBold[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, "snrBold", m_bBold[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, "rtmBold", m_bBold[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, "sysBold", m_bBold[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, "rptBold", m_bBold[IDX_RPT]);
	
	app->WriteProfileInt(TRACESETTING, "allItalic", m_bItalic[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, "snrItalic", m_bItalic[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, "rtmItalic", m_bItalic[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, "sysItalic", m_bItalic[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, "rptItalic", m_bItalic[IDX_RPT]);
	
	app->WriteProfileInt(TRACESETTING, "allUline", m_bUline[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, "snrUline", m_bUline[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, "rtmUline", m_bUline[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, "sysUline", m_bUline[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, "rptUline", m_bUline[IDX_RPT]);
	

	app->WriteProfileInt(TRACESETTING, "allStrike", m_bStrike[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, "snrStrike", m_bStrike[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, "rtmStrike", m_bStrike[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, "sysStrike", m_bStrike[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, "rptStrike", m_bStrike[IDX_RPT]);
}

void CFontSetDlg::ChangeFont(int kind)
{
	if (kind < 0 || kind > IDX_MAX)
		return;

	LOGFONT lf;
	CClientDC dc(this);

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(m_fSize[kind], dc.GetDeviceCaps(LOGPIXELSY), 72);
	strcpy(lf.lfFaceName, (char *) m_fName[kind].operator LPCTSTR());

	CFontDialog	dlg(&lf);
	if (dlg.DoModal() == IDOK)
	{
		CString tmps;
		UINT	nRes;

		switch (kind)
		{
		case IDX_ALL:	nRes = IDC_ALLFONT;	break;
		case IDX_SNR:	nRes = IDC_SNRFONT;	break;
		case IDX_RTM:	nRes = IDC_RTMFONT;	break;
		case IDX_SYS:	nRes = IDC_SYSFONT;	break;
		case IDX_RPT:	nRes = IDC_RPTFONT;	break;
		default:	return;
		}

		m_fName[kind] = dlg.GetFaceName();
		m_fSize[kind] = dlg.GetSize()/10;
		
		m_bBold[kind] = dlg.IsBold();
		m_bItalic[kind] = dlg.IsItalic();
		m_bUline[kind] = dlg.IsUnderline();
		m_bStrike[kind] = dlg.IsStrikeOut();

		if (m_fName[kind].IsEmpty())
			m_fName[kind] = DEFAULTFONT;
		tmps.Format(" %d, %s", m_fSize[kind], m_fName[kind]);
		GetDlgItem(nRes)->SetWindowText(tmps);
	}
}
