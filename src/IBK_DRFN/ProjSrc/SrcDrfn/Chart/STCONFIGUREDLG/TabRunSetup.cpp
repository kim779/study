// TabRunSetup.cpp : implementation file
//

#include "stdafx.h"
#include "TabRunSetup.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/STConfigdef.h"
#include ".\tabrunsetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTabRunSetup dialog

CTabRunSetup::CTabRunSetup(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabRunSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRunSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabRunSetup::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRunSetup)
	DDX_Control(pDX, IDC_CBP1, m_cbP1);
	DDX_Control(pDX, IDC_CBP2, m_cbP2);
	DDX_Control(pDX, IDC_CBP3, m_cbP3);
	DDX_Control(pDX, IDC_CBP4, m_cbP4);
	DDX_Control(pDX, IDC_CBP5, m_cbP5);
	DDX_Control(pDX, IDC_CBP6, m_cbP6);
	DDX_Control(pDX, IDC_CBP7, m_cbP7);
	DDX_Control(pDX, IDC_CBS1, m_cbS1);
	DDX_Control(pDX, IDC_CBS2, m_cbS2);
	DDX_Control(pDX, IDC_CBS3, m_cbS3);
	DDX_Control(pDX, IDC_CBS4, m_cbS4);
	DDX_Control(pDX, IDC_CBS5, m_cbS5);
	DDX_Control(pDX, IDC_BT_C1, m_btnC1);
	DDX_Control(pDX, IDC_BT_C2, m_btnC2);
	DDX_Control(pDX, IDC_BT_C3, m_btnC3);
	DDX_Control(pDX, IDC_BT_C4, m_btnC4);
	DDX_Control(pDX, IDC_BT_C5, m_btnC5);
	DDX_Control(pDX, IDC_CB_TIME, m_cbTime);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ED_TIME, m_xEditTime);
}


BEGIN_MESSAGE_MAP(CTabRunSetup, CRscDlg)
	//{{AFX_MSG_MAP(CTabRunSetup)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_C1, OnBnClickedBtC1)
	ON_BN_CLICKED(IDC_BT_C2, OnBnClickedBtC2)
	ON_BN_CLICKED(IDC_BT_C3, OnBnClickedBtC3)
	ON_BN_CLICKED(IDC_BT_C4, OnBnClickedBtC4)
	ON_BN_CLICKED(IDC_BT_C5, OnBnClickedBtC5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabRunSetup message handlers

BOOL CTabRunSetup::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CConfigHelper::cmPercentPoint(&m_cbP1);
	CConfigHelper::cmPercentPoint(&m_cbP2);
	CConfigHelper::cmPercentPoint(&m_cbP3);
	CConfigHelper::cmPercentPoint(&m_cbP4);
	CConfigHelper::cmPercentPoint(&m_cbP5);
	CConfigHelper::cmPercentPoint(&m_cbP6);
	CConfigHelper::cmPercentPoint(&m_cbP7);
	CConfigHelper::cmStar(&m_cbS1);
	CConfigHelper::cmStar(&m_cbS2);
	CConfigHelper::cmStar(&m_cbS3);
	CConfigHelper::cmStar(&m_cbS4);
	CConfigHelper::cmStar(&m_cbS5);
	CConfigHelper::cmTimeA(&m_cbTime);

	m_xEditTime.SetEditType(CXEdit::TIMER_TYPE);
	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTabRunSetup::LoadInfo()
{
	CSTConfigEnv envHelper;
	envHelper.LoadInfo(m_szCFGFile,  m_szDefCFGFile, STConfigEnv_Setup);

	// 강제청산 시점
	CheckRadioButton(IDC_SELLINFO_OPT1, IDC_SELLINFO_OPT2, IDC_SELLINFO_OPT1+envHelper.m_InfoSetup.m_ClearPoint);

	//-------- 1 -----------------------------
	//손절매(손실률)
	CheckDlgButton(IDC_CHK1, atoi(envHelper.m_InfoSetup.m_Chk1));
	SetDlgItemText(IDC_ED_1, envHelper.m_InfoSetup.m_ED1);

	// 손절매(손실률) : P
	m_cbP1.SetCurSel(envHelper.m_InfoSetup.m_P1);

	// 손절매(손실률) : S
	m_cbS1.SetCurSel(envHelper.m_InfoSetup.m_S1);

	// 손절매(손실률) : C
	m_btnC1.SetColor(atol(envHelper.m_InfoSetup.m_CLR_1));

	//------- 2 ------------------------------
	//목표수익	
	CheckDlgButton(IDC_CHK2, atoi(envHelper.m_InfoSetup.m_Chk2));
	SetDlgItemText(IDC_ED_2, envHelper.m_InfoSetup.m_ED2);

	// 목표수익 : P
	m_cbP2.SetCurSel(envHelper.m_InfoSetup.m_P2);

	// 목표수익 : S
	m_cbS2.SetCurSel(envHelper.m_InfoSetup.m_S2);

	// 목표수익 : C
	m_btnC2.SetColor(atol(envHelper.m_InfoSetup.m_CLR_2));

	//----------- 3 --------------------------
	// 최고가대비 하락
	CheckDlgButton(IDC_CHK3, atoi(envHelper.m_InfoSetup.m_Chk3));
	SetDlgItemText(IDC_ED_3, envHelper.m_InfoSetup.m_ED3);

	// 최고가대비 하락 : P
	m_cbP3.SetCurSel(envHelper.m_InfoSetup.m_P3);

	// 최고가대비 하락 : S
	m_cbS3.SetCurSel(envHelper.m_InfoSetup.m_S3);

	// 최고가대비 하락 : C
	m_btnC3.SetColor(atol(envHelper.m_InfoSetup.m_CLR_3));

	//----------- 4 --------------------------
	// 목표가보존청산
	CheckDlgButton(IDC_CHK4, atoi(envHelper.m_InfoSetup.m_Chk4));
	SetDlgItemText(IDC_ED_4, envHelper.m_InfoSetup.m_ED4);

	// 목표가보존청산 : P
	m_cbP4.SetCurSel(envHelper.m_InfoSetup.m_P4);

	// 목표가보존청산 : S
	m_cbS4.SetCurSel(envHelper.m_InfoSetup.m_S4);

	// 목표가보존청산승 : C
	m_btnC4.SetColor(atol(envHelper.m_InfoSetup.m_CLR_4));

	//----------- 5 --------------------------
	//최소가격변화
	CheckDlgButton(IDC_CHK5, atoi(envHelper.m_InfoSetup.m_Chk5));
	SetDlgItemText(IDC_ED_5, envHelper.m_InfoSetup.m_ED5);

	// 최소가격변화 : P
	m_cbP5.SetCurSel(envHelper.m_InfoSetup.m_P5);

	// 최소가격변화 : S

	// 봉
	SetDlgItemText(IDC_ED_BONG, envHelper.m_InfoSetup.m_Bong);


	//----------- 6 --------------------------
	//당일청산
	CheckDlgButton(IDC_CHK6, atoi(envHelper.m_InfoSetup.m_Chk6));


	// 오전/오후
	m_cbTime.SetCurSel(envHelper.m_InfoSetup.m_TimeA);	

	// 시간
	SetDlgItemText(IDC_ED_TIME, envHelper.m_InfoSetup.m_ED_Time);

	//----------- 7 --------------------------
	//이익보존청산
	CheckDlgButton(IDC_CHK7, atoi(envHelper.m_InfoSetup.m_Chk7));
	SetDlgItemText(IDC_ED_6, envHelper.m_InfoSetup.m_ED6);
	SetDlgItemText(IDC_ED_7, envHelper.m_InfoSetup.m_ED7);

	// 이익보존청산 : P
	m_cbP6.SetCurSel(envHelper.m_InfoSetup.m_P6);
	m_cbP7.SetCurSel(envHelper.m_InfoSetup.m_P7);

	// 이익보존청산 : S
	m_cbS5.SetCurSel(envHelper.m_InfoSetup.m_S5);

	// 이익보존청산 : C
	m_btnC5.SetColor(atol(envHelper.m_InfoSetup.m_CLR_5));
}

BOOL CTabRunSetup::SaveInfo()
{
	CString szKey, szTmp;
	COLORREF clrSignal = RGB(0,0,0);
	int nSel;

	// 강제청산 시점
	szKey.Format("%s", "ClearPoint");
	nSel = GetCheckedRadioButton(IDC_SELLINFO_OPT1, IDC_SELLINFO_OPT2);
	szTmp.Format("%d", nSel-IDC_SELLINFO_OPT1);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//-------- 1 -----------------------------
	//손절매(손실률)
	szKey.Format("%s", "Chk1");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK1));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED1");
	GetDlgItemText(IDC_ED_1, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 손절매(손실률) : P
	szKey.Format("%s", "P1");
	nSel = m_cbP1.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 손절매(손실률) : S
	szKey.Format("%s", "S1");
	nSel = m_cbS1.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 손절매(손실률) : C
	szKey.Format("CLR_%s", "1");
	szTmp.Format("%ld", m_btnC1.GetColor());
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//------- 2 ------------------------------
	//목표수익	
	szKey.Format("%s", "Chk2");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK2));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED2");
	GetDlgItemText(IDC_ED_2, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표수익 : P
	szKey.Format("%s", "P2");
	nSel = m_cbP2.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표수익 : S
	szKey.Format("%s", "S2");
	nSel = m_cbS2.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표수익 : C
	szKey.Format("CLR_%s", "2");
	szTmp.Format("%ld", m_btnC2.GetColor());
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//----------- 3 --------------------------
	// 최고가대비 하락
	szKey.Format("%s", "Chk3");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK3));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED3");
	GetDlgItemText(IDC_ED_3, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 최고가대비 하락 : P
	szKey.Format("%s", "P3");
	nSel = m_cbP3.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 최고가대비 하락 : S
	szKey.Format("%s", "S3");
	nSel = m_cbS3.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 최고가대비 하락 : C
	szKey.Format("CLR_%s", "3");
	szTmp.Format("%ld", m_btnC3.GetColor());
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//----------- 4 --------------------------
	// 목표가보존 상승
	szKey.Format("%s", "Chk4");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK4));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED4");
	GetDlgItemText(IDC_ED_4, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표가보존 상승 : P
	szKey.Format("%s", "P4");
	nSel = m_cbP4.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표가보존 상승 : S
	szKey.Format("%s", "S4");
	nSel = m_cbS4.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 목표가보존 상승 : C
	szKey.Format("CLR_%s", "4");
	szTmp.Format("%ld", m_btnC4.GetColor());
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//----------- 5 --------------------------
	// 최소가격변화
	szKey.Format("%s", "Chk5");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK5));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED5");
	GetDlgItemText(IDC_ED_5, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 최소가격변화 : P
	szKey.Format("%s", "P5");
	nSel = m_cbP5.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 최소가격변화 : S

	// 봉
	szKey.Format("%s", "Bong");
	GetDlgItemText(IDC_ED_BONG, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);


	//----------- 6 --------------------------
	//당일청산
	szKey.Format("%s", "Chk6");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK6));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);


	// 오전/오후
	szKey.Format("%s", "TimeA");
	nSel = m_cbTime.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 시간
	szKey.Format("%s", "ED_Time");
	GetDlgItemText(IDC_ED_TIME, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	//----------- 7 --------------------------
	// 이익보존 상승
	szKey.Format("%s", "Chk7");
	szTmp.Format("%d", IsDlgButtonChecked(IDC_CHK7));
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED6");
	GetDlgItemText(IDC_ED_6, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	szKey.Format("%s", "ED7");
	GetDlgItemText(IDC_ED_7, szTmp);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 이익보존 상승 : P
	szKey.Format("%s", "P6");
	nSel = m_cbP6.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 이익보존 상승 : P
	szKey.Format("%s", "P7");
	nSel = m_cbP7.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 이익보존 상승 : S
	szKey.Format("%s", "S5");
	nSel = m_cbS5.GetCurSel();	if(nSel==CB_ERR) nSel=0;
	szTmp.Format("%d", nSel);
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	// 이익보존 상승 : C
	szKey.Format("CLR_%s", "5");
	szTmp.Format("%ld", m_btnC5.GetColor());
	::WritePrivateProfileString(STCONFIG_CLEAR, szKey, szTmp, m_szCFGFile);

	return TRUE;
}

void CTabRunSetup::RemoveInfo()
{
}
HBRUSH CTabRunSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CTabRunSetup::SetDefault()
{
	CheckRadioButton(IDC_SELLINFO_OPT1, IDC_SELLINFO_OPT2, IDC_SELLINFO_OPT1+0);
	CheckDlgButton(IDC_CHK1, 0);
	SetDlgItemText(IDC_ED_1, "");
	m_cbP1.SetCurSel(0);
	m_cbS1.SetCurSel(0);
	m_btnC1.SetColor(255);

	CheckDlgButton(IDC_CHK2, 0);
	SetDlgItemText(IDC_ED_2, "");
	m_cbP2.SetCurSel(0);
	m_cbS2.SetCurSel(0);
	m_btnC2.SetColor(255);

	CheckDlgButton(IDC_CHK3, 0);
	SetDlgItemText(IDC_ED_3, "");
	m_cbP3.SetCurSel(0);
	m_cbS3.SetCurSel(0);
	m_btnC3.SetColor(255);

	CheckDlgButton(IDC_CHK4, 0);
	SetDlgItemText(IDC_ED_4, "");
	m_cbP4.SetCurSel(0);
	m_cbS4.SetCurSel(0);
	m_btnC4.SetColor(255);
	
	CheckDlgButton(IDC_CHK5, 0);
	SetDlgItemText(IDC_ED_5, "");
	m_cbP5.SetCurSel(0);
	SetDlgItemText(IDC_ED_BONG, "");

	CheckDlgButton(IDC_CHK6, 0);
	m_cbTime.SetCurSel(0);
	SetDlgItemText(IDC_ED_TIME, "");

	CheckDlgButton(IDC_CHK7, 0);
	SetDlgItemText(IDC_ED_6, "");
	SetDlgItemText(IDC_ED_7, "");
	m_cbP6.SetCurSel(0);
	m_cbP7.SetCurSel(0);
	m_cbS5.SetCurSel(0);
	m_btnC5.SetColor(255);

	SaveInfo();

	m_btnC1.Invalidate();
	m_btnC2.Invalidate();
	m_btnC3.Invalidate();
	m_btnC4.Invalidate();
	m_btnC5.Invalidate();
}

void CTabRunSetup::OnBnClickedBtC1()
{
	OnBtnClr(m_btnC1);
}

void CTabRunSetup::OnBnClickedBtC2()
{
	OnBtnClr(m_btnC2);
}

void CTabRunSetup::OnBnClickedBtC3()
{
	OnBtnClr(m_btnC3);
}

void CTabRunSetup::OnBnClickedBtC4()
{
	OnBtnClr(m_btnC4);
}

void CTabRunSetup::OnBnClickedBtC5()
{
	OnBtnClr(m_btnC5);
}

void CTabRunSetup::OnBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}
