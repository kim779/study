// AlertSoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "AlertSoundDlg.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertSoundDlg dialog


CAlertSoundDlg::CAlertSoundDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CAlertSoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bApply = FALSE;
}


void CAlertSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertSoundDlg)
	DDX_Control(pDX, IDC_CHECK_APPLY, m_cbApply);
	DDX_Control(pDX, IDC_STATIC_FILE, m_staticFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertSoundDlg)
	ON_BN_CLICKED(IDC_CHECK_APPLY, OnCheckApply)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, OnButtonListen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertSoundDlg Operators

void CAlertSoundDlg::EnableConrol(BOOL bEnable)
{
	GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(bEnable);
}

void CAlertSoundDlg::PlayWave(CString strFile)
{
	CFileFind ff;

	if (ff.FindFile(m_strFile))
		sndPlaySound(m_strFile, SND_ASYNC);
}

/////////////////////////////////////////////////////////////////////////////
// CAlertSoundDlg message handlers

BOOL CAlertSoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_staticFile.SetWindowText(m_strFile);
	m_cbApply.SetCheck(m_bApply);
	EnableConrol(m_bApply);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlertSoundDlg::OnCheckApply() 
{
	m_bApply = m_cbApply.GetCheck();

	EnableConrol(m_bApply);
}

void CAlertSoundDlg::OnButtonFile() 
{
	CFileDialog dlg(TRUE, nullptr, m_strFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "wave file(*.wav)|*.wav||");

	if (dlg.DoModal())
	{
		m_strFile = dlg.GetPathName();
		m_staticFile.SetWindowText(m_strFile);
	}
}

void CAlertSoundDlg::OnButtonListen() 
{
	PlayWave(m_strFile);
}
