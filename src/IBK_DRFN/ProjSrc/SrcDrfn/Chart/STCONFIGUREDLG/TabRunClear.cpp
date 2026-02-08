// TabRunClear.cpp : implementation file
//

#include "stdafx.h"
#include "TabRunClear.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigdef.h"
#include ".\tabrunclear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTabRunClear dialog

CTabRunClear::CTabRunClear(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabRunClear::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRunClear)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabRunClear::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRunClear)
	DDX_Control(pDX, IDC_CB_ALLOW, m_cbAllow);
	DDX_Control(pDX, IDC_CB_REPEAT, m_cbRepeat);
	DDX_Control(pDX, IDC_CB_TAX, m_cbTax);
	DDX_Control(pDX, IDC_CB_SLIPIGY, m_cbSlipigy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabRunClear, CRscDlg)
	//{{AFX_MSG_MAP(CTabRunClear)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRunClear message handlers

BOOL CTabRunClear::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CConfigHelper::cmAllow(&m_cbAllow);
	CConfigHelper::cmRepeatIn(&m_cbRepeat);
	CConfigHelper::cmPercentPoint(&m_cbTax);
	CConfigHelper::cmPercentPoint(&m_cbSlipigy);
	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CTabRunClear::LoadInfo()
{
	CSTConfigEnv envHelper;
	envHelper.LoadInfo(m_szCFGFile, m_szDefCFGFile, STConfigEnv_Clear);

	//	CString szKey, szTmp;
	//	COLORREF clrSignal = RGB(0,0,0);
	//	int nSel;
	//	szTmp.GetBufferSetLength(MAX_PATH);

	// 허용포지션
	m_cbAllow.SetCurSel(envHelper.m_InfoClear.m_Allow);

	//-----------------------------------------------------------------------------------------------------
	// 중복진입 여부
	CheckRadioButton(IDC_RADIO_ALLOW, IDC_RADIO_NOALLOW, IDC_RADIO_ALLOW+envHelper.m_InfoClear.m_Repeat);

	// 중복진입 허용회수
	SetDlgItemInt(IDC_ED_ALLOW, envHelper.m_InfoClear.m_JCount);
	//-----------------------------------------------------------------------------------------------------

	//거래비용_수수료+세금_진입
	SetDlgItemText(IDC_ED_TAXBUY, envHelper.m_InfoClear.m_TaxBuy);

	//거래비용_수수료+세금_청산
	SetDlgItemText(IDC_ED_TAXSELL, envHelper.m_InfoClear.m_TaxSell);

	//거래비용_수수료+세금_POINT
	m_cbTax.SetCurSel(envHelper.m_InfoClear.m_cbTax);

	//거래비용_슬리피지_진입
	SetDlgItemText(IDC_ED_SLIPIGYBUY, envHelper.m_InfoClear.m_SlipigyBuy);

	//거래비용_슬리피지_청산
	SetDlgItemText(IDC_ED_SLIPIGYSELL, envHelper.m_InfoClear.m_SlipigySell);

	//거래비용_슬리피지_POINT
	m_cbSlipigy.SetCurSel(envHelper.m_InfoClear.m_cbSplipigy);

	//	szTmp.ReleaseBuffer();
}

BOOL CTabRunClear::SaveInfo()
{
	CString szKey, szTmp;
	int nSel;

	//허용포지션
	szKey.Format("%s", "Allow");
	szTmp = "";
	nSel = m_cbAllow.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//-----------------------------------------------------------------------------------------------------
	// 중복진입 여부
	szKey.Format("%s", "Repeat");
	nSel = GetCheckedRadioButton(IDC_RADIO_ALLOW, IDC_RADIO_NOALLOW);
	szTmp.Format("%d", nSel-IDC_RADIO_ALLOW);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	// 중복진입 허용회수
	szKey.Format("%s", "JCount");
	GetDlgItemText(IDC_ED_ALLOW, szTmp);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);
	//-----------------------------------------------------------------------------------------------------

	//거래비용_수수료+세금_진입
	szKey.Format("%s", "TaxBuy");
	GetDlgItemText(IDC_ED_TAXBUY, szTmp);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//거래비용_수수료+세금_청산
	szKey.Format("%s", "TaxSell");
	GetDlgItemText(IDC_ED_TAXSELL, szTmp);
	if(szTmp =="")
		szTmp = "0.05";
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//거래비용_수수료+세금_POINT
	szKey.Format("%s", "cbTax");
	szTmp = "";
	nSel = m_cbTax.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//거래비용_슬리피지_진입
	szKey.Format("%s", "SlipigyBuy");
	GetDlgItemText(IDC_ED_SLIPIGYBUY, szTmp);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//거래비용_슬리피지_청산
	szKey.Format("%s", "SlipigySell");
	GetDlgItemText(IDC_ED_SLIPIGYSELL, szTmp);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	//거래비용_슬리피지_POINT
	szKey.Format("%s", "cbSplipigy");
	szTmp = "";
	nSel = m_cbSlipigy.GetCurSel();		if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_SETUP, szKey, szTmp, m_szCFGFile);

	return TRUE;
}

void CTabRunClear::RemoveInfo()
{
}

void CTabRunClear::SetDefault()
{
	m_cbAllow.SetCurSel(0);
	m_cbRepeat.SetCurSel(0);
	SetDlgItemText(IDC_ED_AMOUNT, "");
	SetDlgItemText(IDC_ED_JCOUNT, "");
	SetDlgItemText(IDC_ED_TAXBUY, "");
	SetDlgItemText(IDC_ED_TAXSELL, "");
	m_cbTax.SetCurSel(0);
	SetDlgItemText(IDC_ED_SLIPIGYBUY, "");
	SetDlgItemText(IDC_ED_SLIPIGYSELL, "");
	m_cbSlipigy.SetCurSel(0);
	
	SaveInfo();
}

HBRUSH CTabRunClear::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return *m_pBrBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


