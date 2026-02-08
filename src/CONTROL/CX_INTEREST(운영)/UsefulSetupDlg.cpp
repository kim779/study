// UsefulSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "UsefulSetupDlg.h"

#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsefulSetupDlg dialog


CUsefulSetupDlg::CUsefulSetupDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CUsefulSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsefulSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUsefulSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsefulSetupDlg)
	DDX_Control(pDX, IDC_TEXT_COLOR, m_clTextColor);
	DDX_Control(pDX, IDC_BKG_COLOR, m_clBkgColor);
	DDX_Control(pDX, IDC_KOSDAQ_COLOR, m_clKosdaqColor);
	DDX_Control(pDX, IDC_KOSPI_COLOR, m_clKospiColor);
	DDX_Control(pDX, IDC_CHECK_CLRAPPLY, m_cbClrApply);
	DDX_Control(pDX, IDC_TEXT_SHADOW, m_cbShadow);
	DDX_Control(pDX, IDC_CHECKBOX_ALLAPPLY, m_cbMKAllApply);
	DDX_Control(pDX, IDC_CHECK_ALERT_APPLY, m_cbWaveApply);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFile);
	DDX_Control(pDX, IDC_CHECK_ITEMADD, m_cbAdd);
	DDX_Control(pDX, IDC_CHECK_ITEMCHG, m_cbChg);
	DDX_Control(pDX, IDC_CHECK_SAVE_ALLAPPLY, m_cbAllApply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsefulSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CUsefulSetupDlg)
	ON_BN_CLICKED(IDC_CHECK_CLRAPPLY, OnCheckClrApply)
	ON_BN_CLICKED(IDC_CHECK_ALERT_APPLY, OnCheckAlertApply)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_WAVLISTEN, OnButtonWavlisten)
	ON_BN_CLICKED(IDC_STATIC_ITEMCHG, OnStaticItemchg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsefulSetupDlg Operators

void CUsefulSetupDlg::EnableConrol(int nType, BOOL bEnable)
{
	switch(nType) {
	case 0:
		{
/*@@Zeta
			m_ctlKospiColor.EnableWindow(bEnable);
			m_ctlKosdaqColor.EnableWindow(bEnable);
*/
		}
		break;
	case 1:
		{
		}
		break;
	case 2:
		{
			m_staticFile.EnableWindow(bEnable);
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(bEnable);
			GetDlgItem(IDC_BUTTON_WAVLISTEN)->EnableWindow(bEnable);
		}
		break;
	}
}

void CUsefulSetupDlg::PlayWave(CString strFile)
{
	CFileFind ff;

	if (ff.FindFile(m_strFile))
		sndPlaySound(m_strFile, SND_ASYNC);
}

/////////////////////////////////////////////////////////////////////////////
// CUsefulSetupDlg message handlers

BOOL CUsefulSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// KOSPI, KOSDAQ 종목명 구분 색상
	m_cbClrApply.SetCheck(m_bClrApply);
	m_clKospiColor.SetColor(m_clrKospi);
	m_clKosdaqColor.SetColor(m_clrKosdaq);

	// 책갈피
	m_clBkgColor.SetColor(m_clrMarkerBKG);
	m_clTextColor.SetColor(m_clrMarkerTXT);

	m_cbShadow.SetCheck(m_bShadow);
	m_cbMKAllApply.SetCheck(m_bMKAllApply);

	// 알림소리
	m_staticFile.SetWindowText(m_strFile);
	m_cbWaveApply.SetCheck(m_bWaveApply);

	// 자동저장
	m_cbAdd.SetCheck(m_bAddCnd);
	m_cbChg.SetCheck(m_bChgCnd);
	m_cbAllApply.SetCheck(m_bAllApply);

	EnableConrol(0, m_bClrApply);
	EnableConrol(2, m_bWaveApply);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUsefulSetupDlg::OnOK() 
{
	m_clrKospi  = m_clKospiColor.GetColor();
	m_clrKosdaq = m_clKosdaqColor.GetColor();

	m_clrMarkerBKG	= m_clBkgColor.GetColor();
	m_clrMarkerTXT	= m_clTextColor.GetColor();

	m_bShadow		= m_cbShadow.GetCheck();
	m_bMKAllApply   = m_cbMKAllApply.GetCheck();

	m_bAddCnd = m_cbAdd.GetCheck();
	m_bChgCnd = m_cbChg.GetCheck();
	m_bAllApply = m_cbAllApply.GetCheck();
	
	CDialog::OnOK();
}

void CUsefulSetupDlg::OnCheckClrApply() 
{
	m_bClrApply = !m_bClrApply;
	EnableConrol(0, m_bClrApply);
}

void CUsefulSetupDlg::OnCheckAlertApply() 
{
	m_bWaveApply = !m_bWaveApply;
	EnableConrol(2, m_bWaveApply);
}

void CUsefulSetupDlg::OnButtonOpen() 
{
	CFileDialog dlg(TRUE, nullptr, m_strFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "wave file(*.wav)|*.wav||");

	if (dlg.DoModal())
	{
		m_strFile = dlg.GetPathName();
		m_staticFile.SetWindowText(m_strFile);
	}
}

void CUsefulSetupDlg::OnButtonWavlisten() 
{
	PlayWave(m_strFile);
}

void CUsefulSetupDlg::OnStaticItemchg() 
{
	m_bChgCnd = !m_cbChg;
	m_cbChg.SetCheck(!m_bChgCnd);
}
