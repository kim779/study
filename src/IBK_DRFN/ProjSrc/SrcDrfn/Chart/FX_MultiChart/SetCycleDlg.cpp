// SetCycleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetCycleDlg.h"
#include "InputBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCycleDlg dialog


CSetCycleDlg::CSetCycleDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CSetCycleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCycleDlg)
	//}}AFX_DATA_INIT
	m_pInputBarDlg = NULL;
}


void CSetCycleDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCycleDlg)
	DDX_Control(pDX, IDAPPLY, m_btApply);
	DDX_Control(pDX, IDC_EDIT_DAYFIRST, m_editDayFirst);
	DDX_Control(pDX, IDC_EDIT_DAYSECOND, m_editDaySecond);
	DDX_Control(pDX, IDC_EDIT_DAYTHIRD, m_editDayThird);
	DDX_Control(pDX, IDC_EDIT_DAYFOURTH, m_editDayFourth);

	DDX_Control(pDX, IDC_EDIT_MINFIRST, m_editMinFirst);
	DDX_Control(pDX, IDC_EDIT_MINSECOND, m_editMinSecond);
	DDX_Control(pDX, IDC_EDIT_MINTHIRD, m_editMinThird);
	DDX_Control(pDX, IDC_EDIT_MINFOURTH, m_editMinFourth);
	
	DDX_Control(pDX, IDC_EDIT_SECFIRST, m_editSecFirst);
	DDX_Control(pDX, IDC_EDIT_SECSECOND, m_editSecSecond);
	DDX_Control(pDX, IDC_EDIT_SECTHIRD, m_editSecThird);
	DDX_Control(pDX, IDC_EDIT_SECFOURTH, m_editSecFourth);

	DDX_Control(pDX, IDC_EDIT_TICKFIRST, m_editTickFirst);
	DDX_Control(pDX, IDC_EDIT_TICKSECOND, m_editTickSecond);
	DDX_Control(pDX, IDC_EDIT_TICKTHIRD, m_editTickThird);
	DDX_Control(pDX, IDC_EDIT_TICKFOURTH, m_editTickFourth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCycleDlg, CRscDlg)
	//{{AFX_MSG_MAP(CSetCycleDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_CHANGE(IDC_EDIT_DAYFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYTHIRD, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_MINFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINTHIRD, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_SECFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECTHIRD, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_TICKFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKTHIRD, OnChangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCycleDlg message handlers

BOOL CSetCycleDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	char szTemp[64];
	// 老
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[0]);
	m_editDayFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[1]);
	m_editDaySecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[2]);
	m_editDayThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[3]);
	m_editDayFourth.SetWindowText(szTemp);	

	// 盒
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[0]);
	m_editMinFirst.SetWindowText(szTemp);	//
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[1]);
	m_editMinSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[2]);
	m_editMinThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[3]);
	m_editMinFourth.SetWindowText(szTemp);	

	// 檬
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[0]);
	m_editSecFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[1]);
	m_editSecSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[2]);
	m_editSecThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[3]);
	m_editSecFourth.SetWindowText(szTemp);	

	// 平
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[0]);
	m_editTickFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[1]);
	m_editTickSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[2]);
	m_editTickThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[3]);
	m_editTickFourth.SetWindowText(szTemp);	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetCycleDlg::OnApply() 
{
	m_pInputBarDlg->ResetUnitValue(TRUE);
	CString strOutput;

	//{{JS.Kim_20100903 Solomon	林扁 蔼 眉农
	/*
	// 老
	m_editDayFirst.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfButton[0] = atol(strOutput);
	m_editDaySecond.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfButton[1] = atol(strOutput);
	m_editDayThird.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfButton[2] = atol(strOutput);
	m_editDayFourth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfButton[3] = atol(strOutput);

	// 盒
	m_editMinFirst.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfMinButton[0] = atol(strOutput);
	m_editMinSecond.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfMinButton[1] = atol(strOutput);
	m_editMinThird.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfMinButton[2] = atol(strOutput);
	m_editMinFourth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfMinButton[3] = atol(strOutput);

	// 盒
	m_editSecFirst.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfSecButton[0] = atol(strOutput);
	m_editSecSecond.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfSecButton[1] = atol(strOutput);
	m_editSecThird.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfSecButton[2] = atol(strOutput);
	m_editSecFourth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfSecButton[3] = atol(strOutput);

	// 平
	m_editTickFirst.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfTickButton[0] = atol(strOutput);
	m_editTickSecond.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfTickButton[1] = atol(strOutput);
	m_editTickThird.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfTickButton[2] = atol(strOutput);
	m_editTickFourth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfTickButton[3] = atol(strOutput);
	*/

	// 老
	VerifySettingData(m_editDayFirst, m_pInputBarDlg->m_lValueOfButton[0]);
	VerifySettingData(m_editDaySecond, m_pInputBarDlg->m_lValueOfButton[1]);
	VerifySettingData(m_editDayThird, m_pInputBarDlg->m_lValueOfButton[2]);
	VerifySettingData(m_editDayFourth, m_pInputBarDlg->m_lValueOfButton[3]);

	// 盒
	VerifySettingData(m_editMinFirst, m_pInputBarDlg->m_lValueOfMinButton[0]);
	VerifySettingData(m_editMinSecond, m_pInputBarDlg->m_lValueOfMinButton[1]);
	VerifySettingData(m_editMinThird, m_pInputBarDlg->m_lValueOfMinButton[2]);
	VerifySettingData(m_editMinFourth, m_pInputBarDlg->m_lValueOfMinButton[3]);

	// 檬
	VerifySettingData(m_editSecFirst, m_pInputBarDlg->m_lValueOfSecButton[0]);
	VerifySettingData(m_editSecSecond, m_pInputBarDlg->m_lValueOfSecButton[1]);
	VerifySettingData(m_editSecThird, m_pInputBarDlg->m_lValueOfSecButton[2]);
	VerifySettingData(m_editSecFourth, m_pInputBarDlg->m_lValueOfSecButton[3]);

	// 平
	VerifySettingData(m_editTickFirst, m_pInputBarDlg->m_lValueOfTickButton[0]);
	VerifySettingData(m_editTickSecond, m_pInputBarDlg->m_lValueOfTickButton[1]);
	VerifySettingData(m_editTickThird, m_pInputBarDlg->m_lValueOfTickButton[2]);
	VerifySettingData(m_editTickFourth, m_pInputBarDlg->m_lValueOfTickButton[3]);
//}}


	m_pInputBarDlg->UpdateBtNCbValue();
	m_pInputBarDlg->SaveValue();
	m_pInputBarDlg->ResetUnitValue(FALSE);
	m_btApply.EnableWindow(FALSE);	
}

void CSetCycleDlg::OnOK() 
{
	OnApply();	
	CRscDlg::OnOK();
}

void CSetCycleDlg::OnChangeEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRscDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	m_btApply.EnableWindow(TRUE);
	
}

BOOL CSetCycleDlg::VerifySettingData(CString& strData)
{
	LONG lData = atol(strData);
	if(lData <= 0)
		lData = 1;
	else if(lData > EDITPERIOD_MAX_CNT)
		lData = EDITPERIOD_MAX_CNT;
	else 
		return TRUE;

	strData.Format("%ld", lData);
	return FALSE;
}

void CSetCycleDlg::VerifySettingData(CEdit& ctrlEdit, LONG& lData)
{
	CString strOutput;

	ctrlEdit.GetWindowText(strOutput);
	if(!VerifySettingData(strOutput))
		ctrlEdit.SetWindowText(strOutput);
	lData = atol(strOutput);
}
