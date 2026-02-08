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
	DDX_Control(pDX, IDC_EDIT_DAYFIFTH, m_editDayFifth);
	DDX_Control(pDX, IDC_EDIT_DAYSIXTH, m_editDaySixth);

	DDX_Control(pDX, IDC_EDIT_MINFIRST, m_editMinFirst);
	DDX_Control(pDX, IDC_EDIT_MINSECOND, m_editMinSecond);
	DDX_Control(pDX, IDC_EDIT_MINTHIRD, m_editMinThird);
	DDX_Control(pDX, IDC_EDIT_MINFOURTH, m_editMinFourth);
	DDX_Control(pDX, IDC_EDIT_MINFIFTH, m_editMinFifth);
	DDX_Control(pDX, IDC_EDIT_MINSIXTH, m_editMinSixth);
	
	DDX_Control(pDX, IDC_EDIT_SECFIRST, m_editSecFirst);
	DDX_Control(pDX, IDC_EDIT_SECSECOND, m_editSecSecond);
	DDX_Control(pDX, IDC_EDIT_SECTHIRD, m_editSecThird);
	DDX_Control(pDX, IDC_EDIT_SECFOURTH, m_editSecFourth);
	DDX_Control(pDX, IDC_EDIT_SECFIFTH, m_editSecFifth);
	DDX_Control(pDX, IDC_EDIT_SECSIXTH, m_editSecSixth);

	DDX_Control(pDX, IDC_EDIT_TICKFIRST, m_editTickFirst);
	DDX_Control(pDX, IDC_EDIT_TICKSECOND, m_editTickSecond);
	DDX_Control(pDX, IDC_EDIT_TICKTHIRD, m_editTickThird);
	DDX_Control(pDX, IDC_EDIT_TICKFOURTH, m_editTickFourth);
	DDX_Control(pDX, IDC_EDIT_TICKFIFTH, m_editTickFifth);
	DDX_Control(pDX, IDC_EDIT_TICKSIXTH, m_editTickSixth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCycleDlg, CRscDlg)
	//{{AFX_MSG_MAP(CSetCycleDlg)
	ON_BN_CLICKED(IDAPPLY, OnApply)
	ON_EN_CHANGE(IDC_EDIT_DAYFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYTHIRD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYFIFTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_DAYSIXTH, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_MINFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINTHIRD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINFIFTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_MINSIXTH, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_SECFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECTHIRD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECFIFTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_SECSIXTH, OnChangeEdit)

	ON_EN_CHANGE(IDC_EDIT_TICKFIRST, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKFOURTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKSECOND, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKTHIRD, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKFIFTH, OnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_TICKSIXTH, OnChangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCycleDlg message handlers

BOOL CSetCycleDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	char szTemp[64];
	// 일
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[0]);
	m_editDayFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[1]);
	m_editDaySecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[2]);
	m_editDayThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[3]);
	m_editDayFourth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[4]);
	m_editDayFifth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfButton[5]);
	m_editDaySixth.SetWindowText(szTemp);	

	// 분
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[0]);
	m_editMinFirst.SetWindowText(szTemp);	//½
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[1]);
	m_editMinSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[2]);
	m_editMinThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[3]);
	m_editMinFourth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[4]);
	m_editMinFifth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfMinButton[5]);
	m_editMinSixth.SetWindowText(szTemp);	

	// 초
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[0]);
	m_editSecFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[1]);
	m_editSecSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[2]);
	m_editSecThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[3]);
	m_editSecFourth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[4]);
	m_editSecFifth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfSecButton[5]);
	m_editSecSixth.SetWindowText(szTemp);	

	// 틱
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[0]);
	m_editTickFirst.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[1]);
	m_editTickSecond.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[2]);
	m_editTickThird.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[3]);
	m_editTickFourth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[4]);
	m_editTickFifth.SetWindowText(szTemp);	
	sprintf(szTemp,"%d",m_pInputBarDlg->m_lValueOfTickButton[5]);
	m_editTickSixth.SetWindowText(szTemp);	
	
	if(m_pInputBarDlg->m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_editTickFirst.EnableWindow(FALSE);
		m_editTickSecond.EnableWindow(FALSE);	
		m_editTickThird.EnableWindow(FALSE);	
		m_editTickFourth.EnableWindow(FALSE);	
		m_editTickFifth.EnableWindow(FALSE);	
		m_editTickSixth.EnableWindow(FALSE);	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetCycleDlg::OnApply() 
{
	m_pInputBarDlg->ResetUnitValue(TRUE);

	// 초
	VerifySettingData(m_editSecFirst, m_pInputBarDlg->m_lValueOfSecButton[0]);
	VerifySettingData(m_editSecSecond, m_pInputBarDlg->m_lValueOfSecButton[1]);
	VerifySettingData(m_editSecThird, m_pInputBarDlg->m_lValueOfSecButton[2]);
	VerifySettingData(m_editSecFourth, m_pInputBarDlg->m_lValueOfSecButton[3]);
	VerifySettingData(m_editSecFifth, m_pInputBarDlg->m_lValueOfSecButton[4]);
	VerifySettingData(m_editSecSixth, m_pInputBarDlg->m_lValueOfSecButton[5]);

	// 일
	VerifySettingData(m_editDayFirst, m_pInputBarDlg->m_lValueOfButton[0]);
	VerifySettingData(m_editDaySecond, m_pInputBarDlg->m_lValueOfButton[1]);
	VerifySettingData(m_editDayThird, m_pInputBarDlg->m_lValueOfButton[2]);
	VerifySettingData(m_editDayFourth, m_pInputBarDlg->m_lValueOfButton[3]);
	VerifySettingData(m_editDayFifth, m_pInputBarDlg->m_lValueOfButton[4]);
	VerifySettingData(m_editDaySixth, m_pInputBarDlg->m_lValueOfButton[5]);

	// 분
	VerifySettingData(m_editMinFirst, m_pInputBarDlg->m_lValueOfMinButton[0]);
	VerifySettingData(m_editMinSecond, m_pInputBarDlg->m_lValueOfMinButton[1]);
	VerifySettingData(m_editMinThird, m_pInputBarDlg->m_lValueOfMinButton[2]);
	VerifySettingData(m_editMinFourth, m_pInputBarDlg->m_lValueOfMinButton[3]);
	VerifySettingData(m_editMinFifth, m_pInputBarDlg->m_lValueOfMinButton[4]);
	VerifySettingData(m_editMinSixth, m_pInputBarDlg->m_lValueOfMinButton[5]);

	// 틱
	VerifySettingData(m_editTickFirst, m_pInputBarDlg->m_lValueOfTickButton[0]);
	VerifySettingData(m_editTickSecond, m_pInputBarDlg->m_lValueOfTickButton[1]);
	VerifySettingData(m_editTickThird, m_pInputBarDlg->m_lValueOfTickButton[2]);
	VerifySettingData(m_editTickFourth, m_pInputBarDlg->m_lValueOfTickButton[3]);
	VerifySettingData(m_editTickFifth, m_pInputBarDlg->m_lValueOfTickButton[4]);
	VerifySettingData(m_editTickSixth, m_pInputBarDlg->m_lValueOfTickButton[5]);

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
	CString strData;
	CEdit * pEdit = (CEdit*)GetFocus();
	if (pEdit)
	{
		int nID = pEdit->GetDlgCtrlID();
		if (nID == IDC_EDIT_SECFIRST || nID == IDC_EDIT_SECSECOND || nID == IDC_EDIT_SECTHIRD || 
			nID == IDC_EDIT_SECFOURTH || nID == IDC_EDIT_SECFIFTH || nID == IDC_EDIT_SECSIXTH)
		{
			pEdit->GetWindowText(strData);
			int nData = atoi(strData);
			if (nData > 30)
			{
				AfxMessageBox("초봉 데이터는 30초봉까지만 조회가 가능합니다.");
				pEdit->SetWindowText("30");
			}

			//--> 0016490: [7111] 종합차트에서 초봉주기 설정시 60의 약수로 설정되도록 변경
			if (nData > 0)
			{
				int nSecData = 60%nData;
				if (nData < 30 && nSecData != 0)
				{
					AfxMessageBox("조회가능한 초봉이 아닙니다.");
					pEdit->SetWindowText("30");
				}
			}
			//<--
		}
	}

	m_btApply.EnableWindow(TRUE);
}

BOOL CSetCycleDlg::VerifySettingData(CString& strData)
{
	LONG lData = atol(strData);
	if(lData <= 0)
		lData = 1;
	else if(lData > 360)//KHD : 최대 360으로 변경
		lData = 360;
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
