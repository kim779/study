// TextColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "TextColorDlg.h"
#include "chaser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextColorDlg dialog


CTextColorDlg::CTextColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextColorDlg)
	//}}AFX_DATA_INIT
}

CTextColorDlg::~CTextColorDlg()
{
}

void CTextColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextColorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextColorDlg, CDialog)
	//{{AFX_MSG_MAP(CTextColorDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETALLCOLOR, OnSetallcolor)
	ON_BN_CLICKED(IDC_SETSNRCOLOR, OnSetsnrcolor)
	ON_BN_CLICKED(IDC_SETRTMCOLOR, OnSetrtmcolor)
	ON_BN_CLICKED(IDC_SETSYSCOLOR, OnSetsyscolor)
	ON_BN_CLICKED(IDC_SETRPTCOLOR, OnSetrptcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextColorDlg message handlers

BOOL CTextColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	loadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTextColorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT resource = pWnd->GetDlgCtrlID();
	pDC->SetBkMode(TRANSPARENT);
	switch (resource)
	{
	case IDC_ALLCOLOR:	hbr = m_brush[IDX_ALL];	break;
	case IDC_SNRCOLOR:	hbr = m_brush[IDX_SNR];	break;
	case IDC_RTMCOLOR:	hbr = m_brush[IDX_RTM];	break;
	case IDC_SYSCOLOR:	hbr = m_brush[IDX_SYS];	break;
	case IDC_RPTCOLOR:	hbr = m_brush[IDX_RPT];	break;
	}
	return hbr;
}

void CTextColorDlg::OnOK() 
{
	saveInfo();
	CDialog::OnOK();
}

void CTextColorDlg::loadInfo()
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	
	if (app->GetProfileInt(TRACESETTING, SAMECOLOR, 1))
		((CButton *) GetDlgItem(IDC_ALLTEXTCOLOR))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_EACHTEXTCOLOR))->SetCheck(1);
	
	m_color[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLCOLOR, COLOR_BLACK);
	m_color[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRCOLOR, COLOR_BLACK);
	m_color[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMCOLOR, COLOR_RED);
	m_color[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSCOLOR, COLOR_BLUE);
	m_color[IDX_RPT] = app->GetProfileInt(TRACESETTING, RPTCOLOR, COLOR_BLUE);

	for (int ii = 0; ii < IDX_MAX; ii++)
		m_brush[ii].CreateSolidBrush(m_color[ii]);
}

void CTextColorDlg::saveInfo()
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();

	if (((CButton *) GetDlgItem(IDC_ALLTEXTCOLOR))->GetCheck() == BST_CHECKED)
		app->WriteProfileInt(TRACESETTING, SAMECOLOR, 1);
	else	app->WriteProfileInt(TRACESETTING, SAMECOLOR, 0);

	app->WriteProfileInt(TRACESETTING, ALLCOLOR, m_color[IDX_ALL]);
	app->WriteProfileInt(TRACESETTING, SNRCOLOR, m_color[IDX_SNR]);
	app->WriteProfileInt(TRACESETTING, RTMCOLOR, m_color[IDX_RTM]);
	app->WriteProfileInt(TRACESETTING, SYSCOLOR, m_color[IDX_SYS]);
	app->WriteProfileInt(TRACESETTING, RPTCOLOR, m_color[IDX_RPT]);
}

void CTextColorDlg::OnSetallcolor() 
{
	ChangeColor(IDX_ALL);
}

void CTextColorDlg::OnSetsnrcolor() 
{
	ChangeColor(IDX_SNR);
}

void CTextColorDlg::OnSetrtmcolor() 
{
	ChangeColor(IDX_RTM);
}

void CTextColorDlg::OnSetsyscolor() 
{
	ChangeColor(IDX_SYS);
}

void CTextColorDlg::OnSetrptcolor() 
{
	ChangeColor(IDX_RPT);
}
void CTextColorDlg::ChangeColor(int kind)
{
	if (kind < 0 || kind > IDX_MAX)
		return;

	CColorDialog	dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color[kind] = dlg.GetColor();
		m_brush[kind].DeleteObject();
		m_brush[kind].CreateSolidBrush(m_color[kind]);
		
		UINT	nRes;
		switch (kind)
		{
		case IDX_ALL:	nRes = IDC_ALLCOLOR;	break;
		case IDX_SNR:	nRes = IDC_SNRCOLOR;	break;
		case IDX_RTM:	nRes = IDC_RTMCOLOR;	break;
		case IDX_SYS:	nRes = IDC_SYSCOLOR;	break;
		case IDX_RPT:	nRes = IDC_RPTCOLOR;	break;
		default:	return;
		}
		GetDlgItem(nRes)->Invalidate();
	}
}
