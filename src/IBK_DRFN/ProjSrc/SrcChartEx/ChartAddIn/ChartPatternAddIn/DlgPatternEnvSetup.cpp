// DlgPatternEnvSetup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPatternEnvSetup.h"

#include "PatternDefine.h"
#include "Include/ColorTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternEnvSetup dialog


CDlgPatternEnvSetup::CDlgPatternEnvSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatternEnvSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPatternEnvSetup)
	m_radiovalMMA = -1;
	//}}AFX_DATA_INIT

	m_pPatternEnvData = NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
}


void CDlgPatternEnvSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPatternEnvSetup)
	DDX_Control(pDX, IDC_CHK_NOTDRAWALL, m_chkNotDrawAll);
	DDX_Control(pDX, IDC_EDIT_NEXTPT_ALLOWABLE, m_edtNextPtAllowable);
	DDX_Control(pDX, IDC_EDIT_HLPT_ALLOWABLE, m_edtHLPtAllowable);
	DDX_Control(pDX, IDC_CHK_MMA, m_chkMMa);
	DDX_Control(pDX, IDC_CHK_LOWPTSUPPORTLINE, m_chkLowPtSupportLine);
	DDX_Control(pDX, IDC_CHK_LOWPTHLINE, m_chkLowPtHLine);
	DDX_Control(pDX, IDC_CHK_HIGHPTREGISTLINE, m_chkHighPtRegistLine);
	DDX_Control(pDX, IDC_CHK_HIGHPTHLINE, m_chkHighPtHLine);
	DDX_Control(pDX, IDC_CHK_HIGHLOWPTLINE, m_chkHighLowPtLine);
	DDX_Radio(pDX, IDC_RADIO_MMA, m_radiovalMMA);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPatternEnvSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgPatternEnvSetup)
	ON_BN_CLICKED(IDC_RADIO_MMA, OnRadioMma)
	ON_BN_CLICKED(IDC_RADIO_CANDLE, OnRadioCandle)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPatternEnvSetup message handlers


BOOL CDlgPatternEnvSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetPatternEnvDataToControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPatternEnvSetup::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

void CDlgPatternEnvSetup::SetPatternEnvDataToControl()
{
	if ( m_pPatternEnvData)
	{
		CString strTemp;

		// 모든 선 그리지 않음
		m_chkNotDrawAll.SetCheck(m_pPatternEnvData->m_bNotDrawAll);

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

		// 파동선
		m_chkHighLowPtLine.SetCheck(m_pPatternEnvData->m_bDrawHighLowPtLine);
		
		// 조정이동평균선
		m_chkMMa.SetCheck(m_pPatternEnvData->m_bDrawMMa);

		// 고점, 저점 수평선
		m_chkHighPtHLine.SetCheck(m_pPatternEnvData->m_bDrawHighPtHLine);
		m_chkLowPtHLine.SetCheck(m_pPatternEnvData->m_bDrawLowPtHLine);
		
		// 고점저항선, 저점 지지선
		m_chkHighPtRegistLine.SetCheck(m_pPatternEnvData->m_bDrawHighRegistLine);
		m_chkLowPtSupportLine.SetCheck(m_pPatternEnvData->m_bDrawLowSupportLine);
		
		// 고점, 저점 차이%
		strTemp.Format("%.1f", m_pPatternEnvData->m_dHLPtAllowalbeError);
		m_edtHLPtAllowable.SetWindowText(strTemp);
		
		// 다음주요점과 허용오차%(기울기를 이용한 다음 포인트와의 차이)
		strTemp.Format("%.1f", m_pPatternEnvData->m_dNextPtAllowableError);
		m_edtNextPtAllowable.SetWindowText(strTemp);


		UpdateData(FALSE);
	}
}

void CDlgPatternEnvSetup::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		UpdateData(TRUE);
		

		CString strTemp;

		// 모든 선 그리지 않음
		m_pPatternEnvData->m_bNotDrawAll = m_chkNotDrawAll.GetCheck();

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
		
		// 파동선
		m_pPatternEnvData->m_bDrawHighLowPtLine = m_chkHighLowPtLine.GetCheck();

		// 조정이동평균선
		m_pPatternEnvData->m_bDrawMMa = m_chkMMa.GetCheck();

		// 고점, 저점 수평선
		m_pPatternEnvData->m_bDrawHighPtHLine = m_chkHighPtHLine.GetCheck();
		m_pPatternEnvData->m_bDrawLowPtHLine = m_chkLowPtHLine.GetCheck();
		
		// 고점저항선, 저점 지지선
		m_pPatternEnvData->m_bDrawHighRegistLine = m_chkHighPtRegistLine.GetCheck();
		m_pPatternEnvData->m_bDrawLowSupportLine = m_chkLowPtSupportLine.GetCheck();
		
		// 고점, 저점 차이%
		m_edtHLPtAllowable.GetWindowText(strTemp);
		strTemp.Format("%.1f", atof(strTemp));
		m_pPatternEnvData->m_dHLPtAllowalbeError = atof(strTemp);
		
		// 다음주요점과 허용오차%(기울기를 이용한 다음 포인트와의 차이)
		m_edtNextPtAllowable.GetWindowText(strTemp);
		strTemp.Format("%.1f", atof(strTemp));
		m_pPatternEnvData->m_dNextPtAllowableError = atof(strTemp);
	}

	CDialog::OnOK();
}

void CDlgPatternEnvSetup::OnRadioMma() 
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
}


void CDlgPatternEnvSetup::OnRadioCandle() 
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
}

HBRUSH CDlgPatternEnvSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgPatternEnvSetup::OnDestroy() 
{
	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
	if ( m_brWhite.GetSafeHandle())	m_brWhite.DeleteObject();


	CDialog::OnDestroy();

}
