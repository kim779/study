// DlgPatternEnvSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEnvSetup1.h"

#include "PatternDefine.h"
#include "Include/ColorTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup1 dialog


CDlgEnvSetup1::CDlgEnvSetup1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnvSetup1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnvSetup1)
	m_radiovalMMA = -1;
	//}}AFX_DATA_INIT

	m_pPatternEnvData = NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
}


void CDlgEnvSetup1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvSetup1)
	DDX_Control(pDX, IDC_EDIT_HLPT_ALLOWABLE, m_edtHLPtAllowable);
	DDX_Radio(pDX, IDC_RADIO_MMA, m_radiovalMMA);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvSetup1, CDialog)
	//{{AFX_MSG_MAP(CDlgEnvSetup1)
	ON_BN_CLICKED(IDC_RADIO_MMA, OnRadioMma)
	ON_BN_CLICKED(IDC_RADIO_CANDLE, OnRadioCandle)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup1 message handlers


BOOL CDlgEnvSetup1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetPatternEnvDataToControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnvSetup1::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

void CDlgEnvSetup1::SetPatternEnvDataToControl()
{
	if ( m_pPatternEnvData)
	{
		CString strTemp;

		// 모든 선 그리지 않음
		//m_chkNotDrawAll.SetCheck(m_pPatternEnvData->m_bNotDrawAll);

		// 자동추세선 작도법(0: 조정이동평균법, 1:전후캔들)
		m_radiovalMMA = m_pPatternEnvData->m_nDrawMethod;
		if ( m_radiovalMMA == 0)
			OnRadioMma();
		else
			OnRadioCandle();

		// 이동평균 기간
		strTemp.Format("%d", m_pPatternEnvData->m_nMAPeriod);
		SetDlgItemText(IDC_EDIT_MMA, strTemp);

		// 전후 봉갯수
		strTemp.Format("%d", m_pPatternEnvData->m_nCandleCnt);
		SetDlgItemText(IDC_EDIT_CANDLE, strTemp);

		// 전후 봉갯수
		strTemp.Format("%d", m_pPatternEnvData->m_nCandleAfterCnt);
		SetDlgItemText(IDC_EDIT_CANDLE2, strTemp);

		// 고점, 저점 차이%
		strTemp.Format("%.1f", m_pPatternEnvData->m_dHLPtAllowalbeError);
		m_edtHLPtAllowable.SetWindowText(strTemp);
		
		UpdateData(FALSE);
	}
}

void CDlgEnvSetup1::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		UpdateData(TRUE);
		

		CString strTemp;

		// 자동추세선 작도법(0: 조정이동평균법, 1:전후캔들)
		m_pPatternEnvData->m_nDrawMethod = m_radiovalMMA;

		// 이동평균 기간
		GetDlgItemText(IDC_EDIT_MMA, strTemp);
		strTemp.Remove(',');
		m_pPatternEnvData->m_nMAPeriod = atoi(strTemp);
		
		// 전후 봉갯수
		GetDlgItemText(IDC_EDIT_CANDLE, strTemp);
		strTemp.Remove(',');
		m_pPatternEnvData->m_nCandleCnt = atoi(strTemp);
		
		// 전후 봉갯수
		GetDlgItemText(IDC_EDIT_CANDLE2, strTemp);
		strTemp.Remove(',');
		m_pPatternEnvData->m_nCandleAfterCnt = atoi(strTemp);
	
		// 고점, 저점 차이%
		m_edtHLPtAllowable.GetWindowText(strTemp);
		strTemp.Format("%.1f", atof(strTemp));
		m_pPatternEnvData->m_dHLPtAllowalbeError = atof(strTemp);
	}

//	CDialog::OnOK();
}

void CDlgEnvSetup1::OnRadioMma() 
{
	// TODO: Add your control notification handler code here

	UINT nID[] = {	IDC_EDIT_MMA,		
					IDC_TIT_COND1,	IDC_EDIT_HLPT_ALLOWABLE,	IDC_TIT_CONDPER1,	
				};

	int nIDCnt = sizeof(nID) / sizeof(UINT);
	CWnd *pWnd = NULL;

	for ( int i = 0; i < nIDCnt; i++)
	{
		pWnd = GetDlgItem(nID[i]);
		if ( pWnd)
			pWnd->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_EDIT_CANDLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CANDLE2)->EnableWindow(FALSE);
}


void CDlgEnvSetup1::OnRadioCandle() 
{
	// TODO: Add your control notification handler code here
	UINT nID[] = {	IDC_EDIT_MMA,		
					IDC_TIT_COND1,	IDC_EDIT_HLPT_ALLOWABLE,	IDC_TIT_CONDPER1,	
				};

	int nIDCnt = sizeof(nID) / sizeof(UINT);
	CWnd *pWnd = NULL;

	for ( int i = 0; i < nIDCnt; i++)
	{
		pWnd = GetDlgItem(nID[i]);
		if ( pWnd)
			pWnd->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_EDIT_CANDLE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CANDLE2)->EnableWindow(TRUE);
}

HBRUSH CDlgEnvSetup1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
//	pDC->SetBkMode( TRANSPARENT);

	if ( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor( COLOR_WHITE);

		hbr = m_brWhite;
	}
	else
	{
		pDC->SetBkColor( COLOR_BG_ANLYS_DLG);
		hbr = m_brBack;
	}
		
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEnvSetup1::OnDestroy() 
{
	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
	if ( m_brWhite.GetSafeHandle())	m_brWhite.DeleteObject();


	CDialog::OnDestroy();

}
