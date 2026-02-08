// MiniSetUnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiniSetUnitDlg.h"
#include "InputBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniSetUnitDlg dialog


CMiniSetUnitDlg::CMiniSetUnitDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CMiniSetUnitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMiniSetUnitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInputBarDlg = NULL;
}


void CMiniSetUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniSetUnitDlg)
	DDX_Control(pDX, IDC_EDITCBTWELVE, m_editCbTwelve);
	DDX_Control(pDX, IDC_EDITCBNINTH, m_editCbNinth);
	DDX_Control(pDX, IDC_EDITCBTENTH, m_editCbTenth);	
	DDX_Control(pDX, IDC_EDITCBELEVEN, m_editCbEleven);	
	DDX_Control(pDX, IDC_EDITCBEIGHTH, m_editCbEighth);
	DDX_Control(pDX, IDC_EDITCBTHIRD, m_editCbThird);
	DDX_Control(pDX, IDC_EDITCBSIXTH, m_editCbSixth);
	DDX_Control(pDX, IDC_EDITCBSEVENTH, m_editCbSeventh);
	DDX_Control(pDX, IDC_EDITCBSECOND, m_editCbSecond);
	DDX_Control(pDX, IDC_EDITCBFOURTH, m_editCbFourth);
	DDX_Control(pDX, IDC_EDITCBFIFTH, m_editCbFifth);
	DDX_Control(pDX, IDC_EDITCBFIRST, m_editCbFirst);
	DDX_Control(pDX, IDAPPLY, m_btApply);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMiniSetUnitDlg, CRscDlg)
	//{{AFX_MSG_MAP(CMiniSetUnitDlg)
	ON_EN_CHANGE(IDC_EDITCBFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBTHIRD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBFIFTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBSIXTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBSEVENTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBEIGHTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBNINTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBTENTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBELEVEN, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDITCBTWELVE, OnChangeEdit)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniSetUnitDlg message handlers

void CMiniSetUnitDlg::OnOK() 
{
	// TODO: Add extra validation here
	OnApply();	
	CRscDlg::OnOK();
}

BOOL CMiniSetUnitDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	char szTemp[64];

	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[0]);
	m_editCbFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[1]);
	m_editCbSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[2]);
	m_editCbThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[3]);
	m_editCbFourth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[4]);
	m_editCbFifth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[5]);
	m_editCbSixth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[6]);
	m_editCbSeventh.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[7]);
	m_editCbEighth.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[8]);
	m_editCbNinth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[9]);
	m_editCbTenth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[10]);
	m_editCbEleven.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfCombo[11]);
	m_editCbTwelve.SetWindowText(szTemp);		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiniSetUnitDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	CString strOutput;
	
	m_editCbFirst.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[0] = atol(strOutput);
	m_editCbSecond.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[1] = atol(strOutput);
	m_editCbThird.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[2] = atol(strOutput);
	m_editCbFourth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[3] = atol(strOutput);
	m_editCbFifth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[4] = atol(strOutput);
	m_editCbSixth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[5] = atol(strOutput);
	m_editCbSeventh.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[6] = atol(strOutput);
	m_editCbEighth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[7] = atol(strOutput);
	m_editCbNinth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[8] = atol(strOutput);
	m_editCbTenth.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[9] = atol(strOutput);
	m_editCbEleven.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[10] = atol(strOutput);
	m_editCbTwelve.GetWindowText(strOutput);
	m_pInputBarDlg->m_lValueOfCombo[11] = atol(strOutput);

//	m_pInputBarDlg->UpdateBtNCbValue();
	m_pInputBarDlg->SaveValue();
	m_btApply.EnableWindow(FALSE);	
}

void CMiniSetUnitDlg::OnChangeEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRscDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_btApply.EnableWindow(TRUE);
}
