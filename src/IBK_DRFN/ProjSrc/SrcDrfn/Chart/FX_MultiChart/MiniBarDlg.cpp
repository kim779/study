// MiniBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiniBarDlg.h"
#include "./define/ChartMsgDef.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniBarDlg dialog


CMiniBarDlg::CMiniBarDlg(CWnd* pParent /*=NULL*/)
	: CInputBarDlg(pParent)
{
	//{{AFX_DATA_INIT(CMiniBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsMiniBar = TRUE;
	m_nMoveDirection = 1;
//	m_nYUnitType = WON_TICK;
}


void CMiniBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX); // No CInputBarDlg!!
	//{{AFX_DATA_MAP(CMiniBarDlg)
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
//	DDX_Control(pDX, IDC_CMB_TYPE, m_cmbChartViewType);
//	DDX_Control(pDX, IDC_CHKRATEVALUE, m_chkRateValue);
//	DDX_Control(pDX, IDC_CHKSHOWSCALE2, m_chkShowScale);
//	DDX_Control(pDX, IDC_STTEXT, m_stText);
	DDX_Control(pDX, IDC_BTPERIOD, m_btPeriod);
	DDX_Control(pDX, IDC_EDITPERIOD, m_editPeriod);
//	DDX_Control(pDX, IDC_STJONGNAME, m_stJongName);
//	DDX_Control(pDX, IDC_BTNJCODEHISTORY, m_btCodeHistory);
//	DDX_Control(pDX, IDC_CHKWEEK, m_chkWeek);
//	DDX_Control(pDX, IDC_CHKMONTH, m_chkMonth);
//	DDX_Control(pDX, IDC_CHKDAY, m_chkDay);
//	DDX_Control(pDX, IDC_CHKTICK, m_chkTick);
//	DDX_Control(pDX, IDC_CHKSEC, m_chkSec);
//	DDX_Control(pDX, IDC_CHKMINUTE, m_chkMinute);
	DDX_Control(pDX, IDC_EDITCNTOFDATALL, m_editCntOfDatAll);
//	DDX_Control(pDX, IDC_CHKFIRST, m_chkFirst);
//	DDX_Control(pDX, IDC_CHKSECOND, m_chkSecond);
//	DDX_Control(pDX, IDC_CHKTHIRD, m_chkThird);
//	DDX_Control(pDX, IDC_CHKFOURTH, m_chkFourth);
//	DDX_Control(pDX, IDC_BTN_CHANGEUNIT, m_btYUnit);
//@유진추가
	DDX_Control(pDX, IDC_BTN_MOVETOLEFTRIGHT, m_btnMoveToLeftRight);
	DDX_Control(pDX, IDC_BTN_DAYWEEKMONTH, m_btnDayWeekMonth);
	DDX_Control(pDX, IDC_BTN_MINSEC, m_btnMinSec);
	DDX_Control(pDX, IDC_BTN_TICK, m_btnTick);
	DDX_Control(pDX, IDC_BTN_SEL_DAYWEEKMONTH, m_btnSel_DayWeekMonth);
	DDX_Control(pDX, IDC_BTN_SEL_MINSEC, m_btnSel_MinSec);
//@유진추가
//	DDX_Control(pDX, IDC_EDITCODE, m_editCode);
//	DDX_Control(pDX, IDC_BTNSEARCH, m_btnSearch);	
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_BTCHARTMINI, m_btChart);
//	DDX_Control(pDX, IDC_BTINDEXMINI, m_btIndex);
//	DDX_Control(pDX, IDC_BTADDINDEXMINI, m_btAddIndex);

}


BEGIN_MESSAGE_MAP(CMiniBarDlg, CInputBarDlg)
	//{{AFX_MSG_MAP(CMiniBarDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_BTCHARTMINI, OnBtchart)
//	ON_BN_CLICKED(IDC_BTINDEXMINI, OnBtindex)
////	ON_BN_CLICKED(IDC_BTADDINDEXMINI, OnBtaddindex)
//	ON_BN_CLICKED(IDC_CHKSHOWSCALE2, OnChkshowscale2)
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnSelchangeCbmarket)
	ON_BN_CLICKED(IDC_BTN_MOVETOLEFTRIGHT, OnMoveToLeftRight)
	ON_BN_CLICKED(IDC_BTN_DAYWEEKMONTH, OnBtnDayWeekMonth)
	ON_BN_CLICKED(IDC_BTN_MINSEC, OnBtnMinSec)
	ON_BN_CLICKED(IDC_BTN_TICK, OnBtnTick)
	ON_BN_CLICKED(IDC_BTN_SEL_DAYWEEKMONTH, OnBtn_Sel_DayWeekMonth)
	ON_BN_CLICKED(IDC_BTN_SEL_MINSEC, OnBtn_Sel_MinSec)
//	ON_BN_CLICKED(IDC_BTN_CHANGEUNIT, OnBtnChangeunit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniBarDlg message handlers

void CMiniBarDlg::Init()
{
	//m_editCode.SetPath(((CStdDialog*)GetParent())->m_strRootPath);
	m_strImagePath = ((CStdDialog*)GetParent())->m_strRootPath + "\\Image";
	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;

	m_brBkColor.CreateSolidBrush(m_crBk);

//@유진추가
	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return;

	m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
	m_btnMoveToLeftRight.SetCheck(0);
	m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 60, 62);

	m_btnDayWeekMonth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "일", TRUE);
	m_btnDayWeekMonth.FP_vSetToolTipText("일");

	m_btnMinSec.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "분", TRUE);
	m_btnMinSec.FP_vSetToolTipText("분");

	m_btnTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "틱", TRUE);
	m_btnTick.FP_vSetToolTipText("틱");

	m_pIL_W17H20 = &(((CStdDialog*)GetParent())->m_ImgList_W17H20);
	if(m_pIL_W17H20 == NULL) return;

	m_btnSel_DayWeekMonth.FP_vSetImage( m_pIL_W17H20, 0, 2);
	m_btnSel_MinSec.FP_vSetImage( m_pIL_W17H20, 0, 2);
	m_btPeriod.FP_vSetImage( m_pIL_W17H20, 0, 2);
	
	m_editPeriod.SetLimitText(2);
	LoadValue();
	
	CString sTemp;
	char szTemp[10];
	memset(szTemp, 0x00, 10);
	CStdDialog* pParent = (CStdDialog*)GetParent();
	CString strConfig = pParent->m_strConfig;
	
	if(pParent->IsSimpleChart())
	{
		SetAllPosition(1);
		::GetPrivateProfileString("0000", "Type", "0", szTemp, sizeof(szTemp), strConfig);
	}
	else
		::GetPrivateProfileString("0000", "Type", "2", szTemp, sizeof(szTemp), strConfig);

	if(szTemp[0] == 0x00)
		szTemp[0] = DAY_DATA_CHART;
	
	if(szTemp[0] == DAY_DATA_CHART || szTemp[0] == WEEK_DATA_CHART || szTemp[0] == MONTH_DATA_CHART)
	{
		if(szTemp[0] == DAY_DATA_CHART)
		{
			m_btnDayWeekMonth.SetBtnTitle("일");
			m_btnDayWeekMonth.FP_vSetToolTipText("일");

			::GetPrivateProfileString("0000", "DayValue", "1", szTemp, sizeof(szTemp), strConfig);
			sTemp.Format("%s", szTemp);
			sTemp.TrimLeft('0');
			m_editPeriod.SetWindowText(sTemp);	
			
			m_chType = DAY_DATA_CHART;
			m_nDay = atoi(sTemp);
			m_btnDayWeekMonth.SetCheck(TRUE);
		}
		else if(szTemp[0] == WEEK_DATA_CHART)
		{
			m_btnDayWeekMonth.SetBtnTitle("주");
			m_btnDayWeekMonth.FP_vSetToolTipText("주");

			m_editPeriod.SetWindowText("1");			
			m_chType = WEEK_DATA_CHART;
			m_nUnitPos = -1;
			m_btnDayWeekMonth.SetCheck(TRUE);
		}
		else if(szTemp[0] == MONTH_DATA_CHART)
		{
			m_btnDayWeekMonth.SetBtnTitle("월");
			m_btnDayWeekMonth.FP_vSetToolTipText("월");

			m_editPeriod.SetWindowText("1");			
			m_chType = MONTH_DATA_CHART;
			m_nUnitPos = -1;
			m_btnDayWeekMonth.SetCheck(TRUE);
		}
	}
	else if(szTemp[0] == MIN_DATA_CHART || szTemp[0] == SEC_DATA_CHART)
	{
		if(szTemp[0] == MIN_DATA_CHART)
		{
			m_btnMinSec.SetBtnTitle("분");
			m_btnMinSec.FP_vSetToolTipText("분");

			::GetPrivateProfileString("0000", "MinValue", "1", szTemp, sizeof(szTemp), strConfig);
			sTemp.Format("%s", szTemp);
			sTemp.TrimLeft('0');
			m_editPeriod.SetWindowText(sTemp);	
			
			m_chType = MIN_DATA_CHART;
			m_nMin = atoi(sTemp);
			m_btnMinSec.SetCheck(TRUE);
		}
		else if(szTemp[0] == SEC_DATA_CHART)
		{
			m_btnMinSec.SetBtnTitle("초");
			m_btnMinSec.FP_vSetToolTipText("초");

			::GetPrivateProfileString("0000", "SecValue", "1", szTemp, sizeof(szTemp), strConfig);
			sTemp.Format("%s", szTemp);
			sTemp.TrimLeft('0');
			m_editPeriod.SetWindowText(sTemp);	
			
			m_chType = SEC_DATA_CHART;
			m_nSec = atoi(sTemp);
			m_btnMinSec.SetCheck(TRUE);
		}
	}
	else if(szTemp[0] == TICK_DATA_CHART)
	{
		m_btnTick.SetBtnTitle("틱");
		m_btnTick.FP_vSetToolTipText("틱");

		::GetPrivateProfileString("0000", "TickValue", "1", szTemp, sizeof(szTemp), strConfig);
		sTemp.Format("%s", szTemp);
		sTemp.TrimLeft('0');
		m_editPeriod.SetWindowText(sTemp);	

		m_chType	= TICK_DATA_CHART;
		m_nTick		= atoi(sTemp);
		m_btnTick	.SetCheck(TRUE);
	}
//@유진추가
//@유진삭제
//	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
//	if(m_pIL_W20H20 == NULL) return;
//
//	m_chkDay.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "일", TRUE);
//	m_chkDay.FP_vSetToolTipText("일");
//
//	m_chkWeek.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "주", TRUE, 91);
//	m_chkWeek.FP_vSetToolTipText("주");
//
//	m_chkMonth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "월", TRUE, 91);
//	m_chkMonth.FP_vSetToolTipText("월");
//
//	m_chkMinute.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "분", TRUE);
//	m_chkMinute.FP_vSetToolTipText("분");
//
//	m_chkSec.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "초", TRUE);
//	m_chkSec.FP_vSetToolTipText("초");
//
//	m_chkTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "틱", TRUE);
//	m_chkTick.FP_vSetToolTipText("틱");
//
//	m_chkShowScale.FP_vSetImage( m_pIL_W20H20, 24, 26, TRUE, "", TRUE);
//	m_chkShowScale.FP_vSetToolTipText("스케일보여주기");
//
//	m_pIL_W17H20 = &(((CStdDialog*)GetParent())->m_ImgList_W17H20);
//	if(m_pIL_W17H20 == NULL) return;
//	m_btPeriod.FP_vSetImage( m_pIL_W17H20, 0, 2);
//
//	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
//	if(m_pIL_W46H20 == NULL) return;
//	m_btChart.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "차트", FALSE);
//	m_btChart.FP_vSetToolTipText("차트");
//	m_btIndex.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "지표", FALSE);
//	m_btIndex.FP_vSetToolTipText("지표");
//	//m_btAddIndex.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "설정", FALSE);
//	//m_btAddIndex.FP_vSetToolTipText("지표종합설정");
//
//	m_chkFirst.FP_vSetImage( m_pIL_W20H20, 90, 89, TRUE, "1", TRUE, 91);
//	m_chkSecond.FP_vSetImage( m_pIL_W20H20, 90, 89, TRUE, "5", TRUE, 91);
//	m_chkThird.FP_vSetImage( m_pIL_W20H20, 90, 89, TRUE, "10", TRUE, 91);
//	m_chkFourth.FP_vSetImage( m_pIL_W20H20, 90, 89, TRUE, "15", TRUE, 91);
//
//	//	//눈금 전환 
//	m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "\\", TRUE, 91);	
//	m_btYUnit.FP_vSetToolTipText("Y축 눈금전환");
//
//	m_cbMarket.SetCurSel(m_nMarketType);
//	LoadValue();
//	m_chkDay.SetCheck(TRUE);
//	UpdateBtNCbValue();
////	m_editCode.SetLimitText(6);		
//	m_editCntOfDatAll.SetLimitText(3);	
//	m_editPeriod.SetLimitText(2);
//
//
//	//2005. 05. 09 by sy,nam--------------------------------------------------------
//	CString sTemp;
//	char szTemp[10];
//	memset(szTemp, 0x00, 10);
//	CStdDialog* pParent = (CStdDialog*)GetParent();
//	CString strConfig = pParent->m_strConfig;
//	
//	if(pParent->IsSimpleChart())
//	{
//		SetAllPosition(1);
//		//szTemp[0] = TICK_DATA_CHART;
//		::GetPrivateProfileString("0000", "Type", "0", szTemp, sizeof(szTemp), strConfig);
//	}
//	else
//		::GetPrivateProfileString("0000", "Type", "2", szTemp, sizeof(szTemp), strConfig);
//
//	if(szTemp[0] == 0x00)
//		szTemp[0] = DAY_DATA_CHART;
//	
//	if(szTemp[0] == DAY_DATA_CHART)
//	{
//		::GetPrivateProfileString("0000", "DayValue", "1", szTemp, sizeof(szTemp), strConfig);
//		sTemp.Format("%s", szTemp);
//		sTemp.TrimLeft('0');
//		m_editPeriod.SetWindowText(sTemp);	
//		
//		m_chType = DAY_DATA_CHART;
////		m_nUnitPos = 2;	
//		m_nDay = atoi(sTemp);
//		m_chkDay.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == WEEK_DATA_CHART)
//	{
//		m_editPeriod.SetWindowText("1");			
//		m_chType = WEEK_DATA_CHART;
//		m_nUnitPos = -1;			
//		m_chkWeek.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == MONTH_DATA_CHART)
//	{
//		m_editPeriod.SetWindowText("1");			
//		m_chType = MONTH_DATA_CHART;
//		m_nUnitPos = -1;			
//		m_chkWeek.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == MIN_DATA_CHART)
//	{
//		::GetPrivateProfileString("0000", "MinValue", "1", szTemp, sizeof(szTemp), strConfig);
//		sTemp.Format("%s", szTemp);
//		sTemp.TrimLeft('0');
//		m_editPeriod.SetWindowText(sTemp);	
//		
//		m_chType = MIN_DATA_CHART;
////		m_nUnitPos = 1;	
//		m_nDay = atoi(sTemp);
//		m_chkMinute.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == SEC_DATA_CHART)
//	{
//		::GetPrivateProfileString("0000", "SecValue", "1", szTemp, sizeof(szTemp), strConfig);
//		sTemp.Format("%s", szTemp);
//		sTemp.TrimLeft('0');
//		m_editPeriod.SetWindowText(sTemp);	
//		
//		m_chType = SEC_DATA_CHART;
//		m_nSec = atoi(sTemp);
//		m_chkSec.SetCheck(TRUE);
//	}
//	else if(szTemp[0] == TICK_DATA_CHART)
//	{
//		::GetPrivateProfileString("0000", "TickValue", "1", szTemp, sizeof(szTemp), strConfig);
//		sTemp.Format("%s", szTemp);
//		sTemp.TrimLeft('0');
//		m_editPeriod.SetWindowText(sTemp);	
//
//		m_chType	= TICK_DATA_CHART;
////		m_nUnitPos	= 0;	
//		m_nTick		= atoi(sTemp);
//		m_chkTick	.SetCheck(TRUE);
//	}
//	
//	SetYUnitType();
//@유진삭제
}

//void CMiniBarDlg::LoadIniValue()
//{
//	long lDefaultValueOfButton[CNTOFMAXBUTTONS] = { 1, 5, 10, 15};
//	long lValueOfCombo[CNTOFMAXCBITEMS] = {1, 2, 3, 4, 5, 10, 15, 20, 30, 60};
//	char szTemp[8];
//	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
//	for(int nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lValueOfCombo[nPos] = ::GetPrivateProfileInt("Combo",szTemp,lValueOfCombo[nPos],strUserDir);
//	}
//	m_nUnitPos = ::GetPrivateProfileInt("Combo","UnitPos",0,strUserDir);
//	for(nPos=0;nPos<5;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos],strUserDir);
//	}	
//}

//void CMiniBarDlg::SaveIniValue()
//{
////	CString strConfig = ((CStdDialog*)GetParent())->m_strConfig;
////	CString strTemp;
////	strTemp.Format("%d", m_chkShowScale.GetCheck());
////	::WritePrivateProfileString("InputBar","ShowScale",strTemp,strConfig);
//
//	CString strUserDir = ((CStdDialog*)GetParent())->m_strConfig;
//	char szTemp[8];
//	char szTemp2[8];
//	CString strTemp;
//	for(int nPos=0;nPos<CNTOFMAXCBITEMS;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lValueOfCombo[nPos]);
//		::WritePrivateProfileString("Combo",szTemp,szTemp2,strUserDir);
//	}
//	sprintf(szTemp2,"%d",m_nUnitPos);	
//	::WritePrivateProfileString("Combo","UnitPos",szTemp2,strUserDir);
//	for(nPos=0;nPos<5;nPos++)
//	{
//		sprintf(szTemp,"%d",nPos);
//		sprintf(szTemp2,"%d",m_lCntInView[nPos]);
//		::WritePrivateProfileString("CntInView",szTemp,szTemp2,strUserDir);
//	}
//
//}

void CMiniBarDlg::SetType(BOOL bSendChartMng/*=TRUE*/)
{	
//#ifdef _DEBUG
//CString strTEST;
//strTEST.Format("[gm0604]SetType(%d) m_chType = %c, m_nUnitPos= %d...",bSendChartMng,m_chType,m_nUnitPos);
//::OutputDebugString(strTEST);
//#endif
	EnableWindow(FALSE);
//@유진추가
	// 틱(0)/분(1)/일(2)/주(3)/월(4)/초(6)
	switch(m_chType)
	{
	case TICK_DATA_CHART:
		//<<20100402_JS.Kim 솔로몬
		m_btPeriod.EnableWindow(TRUE);
		m_editPeriod.SetReadOnly(FALSE);
		//>>
	
		m_btnDayWeekMonth.SetCheck(0);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(1);
		break;
	case SEC_DATA_CHART:	
	case MIN_DATA_CHART:	
		//<<20100402_JS.Kim 솔로몬
		m_btPeriod.EnableWindow(TRUE);
		m_editPeriod.SetReadOnly(FALSE);
		//>>

		m_btnDayWeekMonth.SetCheck(0);
		m_btnMinSec.SetCheck(1);
		m_btnTick.SetCheck(0);
		break;
	case DAY_DATA_CHART:
	case WEEK_DATA_CHART:
	case MONTH_DATA_CHART:
//		Enable_PeriodControl(TRUE);
		//<<20100402_JS.Kim 솔로몬
		m_btPeriod.EnableWindow(FALSE);
		m_editPeriod.SetReadOnly(TRUE);
		//>>

		m_btnDayWeekMonth.SetCheck(1);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(0);
		break;
	}

	m_btnDayWeekMonth.Invalidate();
	m_btnMinSec.Invalidate();
	m_btnTick.Invalidate();

	//<<20100402_JS.Kim 솔로몬
	if (TICK_DATA_CHART == m_chType)	// 틱
	{
		GetDlgItem (IDC_RADIO_BUY)->EnableWindow(FALSE);
		GetDlgItem (IDC_RADIO_SELL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem (IDC_RADIO_BUY)->EnableWindow(TRUE);
		GetDlgItem (IDC_RADIO_SELL)->EnableWindow(TRUE);
	}
	//>>

	if(bSendChartMng)
	{
		//if(m_nUnitPos>-1)
		if(m_chType < WEEK_DATA_CHART || m_chType == SEC_DATA_CHART)
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			{
				CString strValue;				
				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
				int nValue = atoi(strValue);
				if(nValue <= 0) 
				{
					strValue = _T("1");
					m_editPeriod.SetWindowText(strValue);
				}

				m_pChartMng->SendMessage(UMSG_SETTYPE,
										m_chType, // 틱, 분, 일,....구분 
										nValue);//사용자 지정 기간
			}
		}
		else // 주 / 월 / 년
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType);
		}		
	}
//@유진추가
//@유진삭제
//	// 틱(0)/분(1)/일(2)/주(3)/월(4)
//	switch(m_chType)
//	{
//	case TICK_DATA_CHART:	
//	case SEC_DATA_CHART:
//	case MIN_DATA_CHART:
//	case DAY_DATA_CHART:
//		m_chkWeek.SetCheck(0);
//		m_chkMonth.SetCheck(0);
//		Enable_PeriodControl(TRUE);
//		switch(m_chType)
//		{
//		case TICK_DATA_CHART:	
//			m_chkDay.SetCheck(0);
//			m_chkMinute.SetCheck(0);
//			m_chkSec.SetCheck(0);
//			m_chkTick.SetCheck(1);
//			break;
//		case SEC_DATA_CHART:	
//			m_chkDay.SetCheck(0);
//			m_chkMinute.SetCheck(0);
//			m_chkSec.SetCheck(1);
//			m_chkTick.SetCheck(0);
//			break;
//		case MIN_DATA_CHART:	
//			m_chkDay.SetCheck(0);
//			m_chkMinute.SetCheck(1);
//			m_chkSec.SetCheck(0);
//			m_chkTick.SetCheck(0);
//			break;
//		case DAY_DATA_CHART:	
//			m_chkDay.SetCheck(1);
//			m_chkMinute.SetCheck(0);
//			m_chkSec.SetCheck(0);
//			m_chkTick.SetCheck(0);
//			break;
//		}
//		switch(m_nUnitPos)
//		{
//		case 1:
//			m_chkFirst.SetCheck(0);
//			m_chkSecond.SetCheck(1);
//			m_chkThird.SetCheck(0);
//			m_chkFourth.SetCheck(0);
//			break;
//		case 2:
//			m_chkFirst.SetCheck(0);
//			m_chkSecond.SetCheck(0);
//			m_chkThird.SetCheck(1);
//			m_chkFourth.SetCheck(0);
//			break;
//		case 3:
//			m_chkFirst.SetCheck(0);
//			m_chkSecond.SetCheck(0);
//			m_chkThird.SetCheck(0);
//			m_chkFourth.SetCheck(1);
//			break;
//		case 0:
//			m_chkFirst.SetCheck(1);
//			m_chkSecond.SetCheck(0);
//			m_chkThird.SetCheck(0);
//			m_chkFourth.SetCheck(0);
//			break;
//		default:
//			m_chkFirst.SetCheck(0);
//			m_chkSecond.SetCheck(0);
//			m_chkThird.SetCheck(0);
//			m_chkFourth.SetCheck(0);
//			m_nUnitPos = 0;
//
//			//bSendChartMng = FALSE;
//			break;			
//		}
//		break;	
//	case WEEK_DATA_CHART:
//		m_chkDay.SetCheck(0);
//		m_chkWeek.SetCheck(1);
//		m_chkMonth.SetCheck(0);
//		m_chkMinute.SetCheck(0);
//		m_chkSec.SetCheck(0);
//		m_chkTick.SetCheck(0);
//		m_chkFirst.SetCheck(0);
//		m_chkSecond.SetCheck(0);
//		m_chkThird.SetCheck(0);
//		m_chkFourth.SetCheck(0);
//		Enable_PeriodControl(FALSE);
//		break;
//	case MONTH_DATA_CHART:
//		m_chkDay.SetCheck(0);
//		m_chkWeek.SetCheck(0);
//		m_chkMonth.SetCheck(1);
//		m_chkMinute.SetCheck(0);
//		m_chkSec.SetCheck(0);
//		m_chkTick.SetCheck(0);
//		m_chkFirst.SetCheck(0);
//		m_chkSecond.SetCheck(0);
//		m_chkThird.SetCheck(0);
//		m_chkFourth.SetCheck(0);
//		Enable_PeriodControl(FALSE);
//		break;
//	}
//	//m_editPeriod.SetWindowText("");
//
//	m_chkDay.Invalidate();
//	m_chkWeek.Invalidate();
//	m_chkMonth.Invalidate();
//	m_chkMinute.Invalidate();
//	m_chkSec.Invalidate();
//	m_chkTick.Invalidate();
//	m_chkFirst.Invalidate();
//	m_chkSecond.Invalidate();
//	m_chkThird.Invalidate();
//	m_chkFourth.Invalidate();
//	
//	if(bSendChartMng)
//	{
//		//if(m_nUnitPos>-1)
//		if(m_chType < WEEK_DATA_CHART || m_chType == SEC_DATA_CHART)
//		{
//			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			{
//				CString strValue;				
//				m_editPeriod.GetWindowText(strValue); strValue.Remove(' ');
//				int nValue = atoi(strValue);
//				if(nValue <= 0) 
//				{
//					strValue = _T("1");
//					m_editPeriod.SetWindowText(strValue);
//				}
//
//				m_pChartMng->SendMessage(UMSG_SETTYPE,
//										m_chType, // 틱, 분, 일,....구분 
//										nValue);//사용자 지정 기간
//			}
//		}
//		else // 주 / 월 / 년
//		{
//			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//				m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType);
//		}		
//	}
//@유진삭제
	EnableWindow();
}


//void CMiniBarDlg::OnBtperiod() 
//{
//	// TODO: Add your control notification handler code here
//    CMenu menu;
//    menu.CreatePopupMenu();
//
//	char szTemp[64];
//	long lPosAll = 0;
//	long lSize = 0;
////	sprintf(szTemp,"%d",m_lValueOfButton[0]);
////	menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS+1+0,szTemp);
////	sprintf(szTemp,"%d",m_lValueOfButton[1]);
////	menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS+1+1,szTemp);
////	sprintf(szTemp,"%d",m_lValueOfButton[2]);
////	menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS+1+2,szTemp);
////	sprintf(szTemp,"%d",m_lValueOfButton[3]);
////	menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS+1+3,szTemp);
////	menu.AppendMenu(MF_SEPARATOR,(UINT)0,"");
//	
//	for(int nIndex=0;nIndex<CNTOFMAXCBITEMS;nIndex++)
//	{
//		if(m_lValueOfCombo[nIndex])
//		{
//			sprintf(szTemp,"%d",m_lValueOfCombo[nIndex]);
//			menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
//		}
//	}
//	menu.AppendMenu(MF_STRING,(UINT)1000+CNTOFMAXCBITEMS,"설정");
//	
//	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//	CRect rect;
//	GetDlgItem(IDC_BTPERIOD)->GetWindowRect(rect);
//	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, rect.left, rect.bottom, this);
//	if(!nRetValue) return;
//	menu.DestroyMenu();
//
//	if(nRetValue-1000<=CNTOFMAXCBITEMS)
//	{
////		if((nRetValue-1000)==CNTOFMAXCBITEMS)
////		{
////			CMiniSetUnitDlg dlg;
////			dlg.m_pInputBarDlg = this;
////			dlg.DoModal();
////		}
////		else
//		{
////			m_nUnitPos = -1;
//			if(m_chType!='0')
//			{
//				m_chType = '1';
//			}
//			sprintf(szTemp,"%d",m_lValueOfCombo[nRetValue-1000]);
//			SetType(FALSE);
//			m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType,atol(szTemp));			
//		}
//	}
////	else
////	{
////		m_nUnitPos = -1;
////		if(m_chType!='0')
////		{
////			m_chType = '1';
////		}
////		sprintf(szTemp,"%d",m_lValueOfButton[nRetValue-1000-CNTOFMAXCBITEMS-1]);
////		SetType(FALSE);
////		m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType,atol(szTemp));		
////	}
//}

void CMiniBarDlg::OnDestroy() 
{
	
	// TODO: Add your message handler code here

	CInputBarDlg::OnDestroy();
}

//void CMiniBarDlg::OnBtchart() 
//{
//	// TODO: Add your control notification handler code here
//    CMenu menu;
//	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//    menu.CreatePopupMenu();
//
//	CStringArray strArrayIndicator;
//	m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)"특수지표",(LPARAM)&strArrayIndicator);
//	CString strIndicator;
//	BOOL bExistance = FALSE;
//	long lSize = strArrayIndicator.GetSize();
//	UINT nFlags = NULL;
//	for(long lPos=0;lPos<lSize;lPos++)
//	{
//		strIndicator = strArrayIndicator.GetAt(lPos);
//		bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
//		if(bExistance)
//		{
//			nFlags = MF_CHECKED;
//		}
//		else
//		{
//			nFlags = MF_UNCHECKED;
//		}
//		   
//		menu.AppendMenu(nFlags,100+lPos,strIndicator);
//	}
//	
//	CRect rect;
//	GetDlgItem(IDC_BTCHARTMINI)->GetWindowRect(rect);
//	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
//	if(nRetValue)
//	{
//		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicator.GetAt(nRetValue-100));
//	}	
//    menu.DestroyMenu();
//}
//
//void CMiniBarDlg::OnBtindex() 
//{
//	char* szIndexGroupName[] = {					
//								 //"전체지표",
//								 "가격지표",
//								 "추세지표",
//								 "변동성지표",
//								 "거래량지표",
//								 "2차파생지표",
//								 "기타지표",
//								 "시장지표"
//								 };
////-------------------------------------------------------------------
//	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
//	//=============================================================
////#ifdef _DEBUG
//	// 05.10.24
//	// 주식만 시장 지표 추가 
//	//=============================================================
//	if(m_nMarketType != STOCK_CHART)
//		lAllCnt -= 1;
//	//=============================================================
////#endif
//
//    CMenu menu;
//    menu.CreatePopupMenu();
//
//	CStringArray strArrayIndicator;	
//	CStringArray strArrayIndicatorAll;	
//	CStringArray strArrayMarket; //시장 지표리스트 따로 관리.
//	long lPosAll = 0;
//	long lSize = 0;
//	CString strIndicator;
//	CMenu *pMenuChild = NULL;
//	UINT nFlagsChild = NULL;
//	BOOL bExistance = FALSE;
//	strArrayIndicatorAll.RemoveAll();	
//	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
//	{
//		pMenuChild = new CMenu;
//		pMenuChild->CreatePopupMenu();
//		menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szIndexGroupName[lIndex]);
//
//		if(m_pChartMng->GetSafeHwnd())
//			m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)szIndexGroupName[lIndex],(LPARAM)&strArrayIndicator);
//		lSize = strArrayIndicator.GetSize();
//		nFlagsChild = NULL;
//		bExistance = FALSE;
//
//		for(long lPos=0;lPos<lSize;lPos++)
//		{
//			strIndicator = strArrayIndicator.GetAt(lPos);
//			if(m_pChartMng->GetSafeHwnd())
//				bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
//			if(bExistance)
//			{
//				nFlagsChild = MF_CHECKED;
//			}
//			else
//			{
//				nFlagsChild = MF_UNCHECKED;
//			}
//			   
//			pMenuChild->AppendMenu(nFlagsChild,100+lPosAll,strIndicator);
//			lPosAll++;
//		}
//		strArrayIndicatorAll.Append(strArrayIndicator);
//
//		//==========================================================
//		//시장지표 항목은 별도로 따로 저장 한번 더 한다. 05.11. 01
//		//메뉴에서 눌린게 시장지표라는 것을 알기위해 
//		//==========================================================
//		if(lIndex == lAllCnt - 1)
//		{
//			strArrayMarket.Append(strArrayIndicator);
//		}
//	}
//
////	CString sArrMarket = strArrayIndicatorAll.GetAt(lAllCnt); //시장지표 항목이 추가되어 있는 CStringArray를 가져온다.
//
//
//	int nCntAddedByFavorite = 0;
//
//	CFavoriteThingsMng* pFavoriteMng = NULL;
//
//	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;
//	if(pFavoriteMng)
//	{
//		pFavoriteMng->LoadGrpInfo();
//		DWORD wSizeGrp = pFavoriteMng->m_strArrayGrp.GetSize();
//		DWORD wSizeItem = 0;
//		HTREEITEM  hRootItem = NULL;
//		HTREEITEM  hItem = NULL;
//		CString strGrpName;
//		CString strItemName;
//		DWORD wIndexGrp = 0;
//		DWORD wIndexItem = 0;
//		stFvrData _stFvrData;
//		BOOL	bCreateRoot = FALSE;
//		for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
//		{
//			bCreateRoot = FALSE;
//			strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
//			pFavoriteMng->LoadItemInfo(strGrpName);
//			wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
//			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
//			{
//				_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
//				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
//				{
//					if(!nCntAddedByFavorite)
//					{
//						menu.AppendMenu(MF_SEPARATOR);
//						nCntAddedByFavorite++;
//					}
//					bCreateRoot = TRUE;
//					if(m_pChartMng->GetSafeHwnd())
//						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
//					if(bExistance)
//					{
//						nFlagsChild = MF_CHECKED;
//					}
//					else
//					{
//						nFlagsChild = MF_UNCHECKED;
//					}
//					menu.AppendMenu(nFlagsChild,100+lPosAll,_stFvrData.m_chData1);
//					nCntAddedByFavorite++;
//					lPosAll++;
//					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
//				}
//			}		
//		}
//	}
//	
//	CRect rect;
//	GetDlgItem(IDC_BTINDEXMINI)->GetWindowRect(rect);
//	UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//	int nRetValue = menu.TrackPopupMenu(nFlags, rect.left, rect.bottom, this);
//	if(nRetValue)
//	{	
//		CString sJipyo = strArrayIndicatorAll.GetAt(nRetValue-100);
//
//		//==============================================================================
//		BOOL	bMarketJipyo = FALSE;
//		for(int k = 0; k < strArrayMarket.GetSize(); k++)
//		{
//			if( sJipyo.CompareNoCase(strArrayMarket.GetAt(k)) == 0 )
//			{
//				bMarketJipyo = TRUE;
//				break;
//			}
//		}
//		//==============================================================================
//
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)sJipyo, bMarketJipyo ? FROM_EXTINPUTBAR_MARKET : 0) ;
//	}
//	
//	for(long lIndexOfMenu=0;lIndexOfMenu<(lAllCnt+nCntAddedByFavorite);lIndexOfMenu++)
//	{
//		pMenuChild = menu.GetSubMenu(lIndexOfMenu);
//		if(pMenuChild)
//		{
//			pMenuChild->DestroyMenu();
//			delete pMenuChild;
//		}
//	}
//	menu.DestroyMenu();
////	// TODO: Add your control notification handler code here
////	char* szIndexGroupName[] = { "기본지표", "이동평균", "주가지표", "기타지표" ,"첨부지표","추세지표","거래량지표"
////		,"변동성지표","탄력성지표"};
////	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
////
////    CMenu menu;
////    menu.CreatePopupMenu();
////
////	CStringArray strArrayIndicator;	
////	CStringArray strArrayIndicatorAll;	
////	long lPosAll = 0;
////	long lSize = 0;
////	CString strIndicator;
////	CMenu *pMenuChild = NULL;
////	UINT nFlagsChild = NULL;
////	BOOL bExistance = FALSE;
////	strArrayIndicatorAll.RemoveAll();	
////	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
////	{
////		pMenuChild = new CMenu;
////		pMenuChild->CreatePopupMenu();
////		menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szIndexGroupName[lIndex]);
////
////		m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)szIndexGroupName[lIndex],(LPARAM)&strArrayIndicator);
////		lSize = strArrayIndicator.GetSize();
////		nFlagsChild = NULL;
////		bExistance = FALSE;
////
////		for(long lPos=0;lPos<lSize;lPos++)
////		{
////			strIndicator = strArrayIndicator.GetAt(lPos);
////			bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
////			if(bExistance)
////			{
////				nFlagsChild = MF_CHECKED;
////			}
////			else
////			{
////				nFlagsChild = MF_UNCHECKED;
////			}
////			   
////			pMenuChild->AppendMenu(nFlagsChild,100+lPosAll,strIndicator);
////			lPosAll++;
////		}
////		strArrayIndicatorAll.Append(strArrayIndicator);
////	}
////
////	int nCntAddedByFavorite = 0;
////
////	CFavoriteThingsMng* pFavoriteMng = NULL;
////
////	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;
////	if(pFavoriteMng)
////	{
////		pFavoriteMng->LoadGrpInfo();
////		DWORD wSizeGrp = pFavoriteMng->m_strArrayGrp.GetSize();
////		DWORD wSizeItem = 0;
////		HTREEITEM  hRootItem = NULL;
////		HTREEITEM  hItem = NULL;
////		CString strGrpName;
////		CString strItemName;
////		DWORD wIndexGrp = 0;
////		DWORD wIndexItem = 0;
////		stFvrData _stFvrData;
////		BOOL	bCreateRoot = FALSE;
////		for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
////		{
////			bCreateRoot = FALSE;
////			strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
////			pFavoriteMng->LoadItemInfo(strGrpName);
////			wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
////			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
////			{
////				_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
////				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
////				{
////					if(!nCntAddedByFavorite)
////					{
////						menu.AppendMenu(MF_SEPARATOR);
////						nCntAddedByFavorite++;
////					}
////					bCreateRoot = TRUE;
////					bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
////					if(bExistance)
////					{
////						nFlagsChild = MF_CHECKED;
////					}
////					else
////					{
////						nFlagsChild = MF_UNCHECKED;
////					}
////					menu.AppendMenu(nFlagsChild,100+lPosAll,_stFvrData.m_chData1);
////					nCntAddedByFavorite++;
////					lPosAll++;
////					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
////				}
////			}		
////		}
////	}
////	
////	CRect rect;
////	GetDlgItem(IDC_BTINDEXMINI)->GetWindowRect(rect);
////	UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
////	int nRetValue = menu.TrackPopupMenu(nFlags, rect.left, rect.bottom, this);
////	if(nRetValue)
////	{
////		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicatorAll.GetAt(nRetValue-100));
////	}
////	
////	for(long lIndexOfMenu=0;lIndexOfMenu<(lAllCnt+nCntAddedByFavorite);lIndexOfMenu++)
////	{
////		pMenuChild = menu.GetSubMenu(lIndexOfMenu);
////		if(pMenuChild)
////		{
////			pMenuChild->DestroyMenu();
////			delete pMenuChild;
////		}
////	}
////	menu.DestroyMenu();
//}
//
//void CMiniBarDlg::OnBtaddindex() 
//{
//	// TODO: Add your control notification handler code here
////	m_pChartMng->SendMessage(UMSG_SHOWINDICATORCONFIGDLG);
//}
//
//void CMiniBarDlg::OnChkshowscale2() 
//{
//	// TODO: Add your control notification handler code here
//	//m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_chkShowScale.GetCheck());
//	m_pChartMng->SendMessage(UMSG_SHOWINNERTOOLBAR,m_chkShowScale.GetCheck());
//}

#define ID_SHARE_CODE_WAIT 1754
void CMiniBarDlg::OnSelchangeCbmarket() 
{
	if(!((CStdDialog*)GetParent())->GetStatusEnable()) return;
	int nSelMarketType = m_cbMarket.GetCurSel();

	// 업종, 해외 코드 저장 - ojtaso (20080115)
	if(m_nMarketType == FOREIGN_CHART)
		m_strForeignCode = GetCurrentCode();
	else if(m_nMarketType == UPJONG_CHART)
		m_strUpjongCode = GetCurrentCode();

	m_strCodeFromOutside = "";
	((CStdDialog*)GetParent())->ChangeStockMode(nSelMarketType); 

	CString strCode;
	if(m_nMarketType == FX_CHART)
	{
		strCode = GetLastHistoryCode (FX_CHART);
	}

	STINPUTCODEDATA2 stInputCodeData2;
	stInputCodeData2.m_strCode = strCode;
	stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
	stInputCodeData2.m_bDontPutInShared = FALSE;
	this->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
}

void CMiniBarDlg::SetAllPosition(int nType)
{
	if(nType == 0)
		return;

	CWnd* pChild = GetWindow(GW_CHILD);
	CRect rectComp;
	while(pChild)
	{
		pChild->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		pChild->SetWindowPos(NULL, rectComp.left - 45, rectComp.top, rectComp.Width(), 20, SWP_NOZORDER);

		if(pChild->GetDlgCtrlID() == IDC_CBMARKET)// || pChild->GetDlgCtrlID() == IDC_CTRL_CODEINPUT || pChild->GetDlgCtrlID() == IDC_BTCHARTMINI || pChild->GetDlgCtrlID() == IDC_BTINDEXMINI)
		{
			pChild->ShowWindow(SW_HIDE);
		}

		pChild = pChild->GetNextWindow();
	}

}

//void CMiniBarDlg::OnBtnChangeunit() 
//{
//	if( m_nYUnitType == WON_TICK)
//	{
//		m_nYUnitType = PERCENT_TICK;
//		//m_btYUnit.SetWindowText("%");
//		m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "%", TRUE, 91);
//	}
//	else if(m_nYUnitType == DOLLAR_TICK)
//	{
//		m_nYUnitType = WON_TICK;
//		//m_btYUnit.SetWindowText("\\");
//		m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "\\", TRUE, 91);
//	}
//	else if(m_nYUnitType == PERCENT_TICK)
//	{
//		m_nYUnitType = DOLLAR_TICK;
//		//m_btYUnit.SetWindowText("$");
//		m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "$", TRUE, 91);
//	}
//
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_RATEVALUE, m_nYUnitType);	
//}
//
//void CMiniBarDlg::SetYUnitType()
//{
//	if( ( m_nMarketType == STOCK_CHART	)//	||
//		)
//	{
//		m_btYUnit.EnableWindow(TRUE);
//		m_btYUnit.ShowWindow(SW_SHOW);
//	}
//	else
//	{
//		m_btYUnit.EnableWindow(FALSE);
//		m_nYUnitType = WON_TICK;
//		m_btYUnit.SetWindowText("\\");
//		m_btYUnit.ShowWindow(SW_HIDE);
//	}
//} 

LRESULT CMiniBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
//	switch(message)
//	{
//		case UMSG_SETINITIALCHARTDATA:
//		{		
//			stChartInitData* _pStData = (stChartInitData*)lParam;	
//			m_nYUnitType = _pStData->m_nChartNumericValue;
//			if(_pStData->m_nChartNumericValue == WON_TYPE)
//			{				
//				m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "\\", TRUE, 91);
//			}
//			else if(_pStData->m_nChartNumericValue == PERCENT_TYPE)
//			{
//				m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "%", TRUE, 91);
//			}
//			else if(_pStData->m_nChartNumericValue == DOLLAR_TYPE)
//			{
//				m_btYUnit.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "$", TRUE, 91);
//			}
//			m_btYUnit.Invalidate();
//		}
//	}
	return CInputBarDlg::WindowProc(message, wParam, lParam);
}

void CMiniBarDlg::OnBtnDayWeekMonth()
{
	char cValue_DayWeekMonth[] = { '2', '3', '4' };
	char* szDayWeekMonth[] = { "일", "주", "월" };

	long lCount = sizeof(szDayWeekMonth)/sizeof(char*);
	char cCurSelectedType = m_chType;

	BOOL bFound = FALSE;
	for(int nIndx = 0; nIndx < lCount; ++nIndx)
	{
		if(cCurSelectedType == cValue_DayWeekMonth[nIndx])
		{
			bFound = TRUE;
			nIndx++;
			nIndx %= lCount;
			break;
		}
	}

	if(bFound)
	{
		m_chType = cValue_DayWeekMonth[nIndx];		
	}
	else
	{
		CString strTitle = m_btnDayWeekMonth.GetBtnTitle();
		for(nIndx = 0; nIndx < lCount; ++nIndx)
		{
			if(!strTitle.Compare(szDayWeekMonth[nIndx]))
			{
				m_chType = cValue_DayWeekMonth[nIndx];
				break;
			}		
		}
	}

	CString strNumber;
	m_editPeriod.GetWindowText(strNumber);
	
	SetType(FALSE);

	m_btnDayWeekMonth.SetBtnTitle(szDayWeekMonth[nIndx]);
	m_btnDayWeekMonth.FP_vSetToolTipText(szDayWeekMonth[nIndx]);
	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, atol(strNumber));
}

void CMiniBarDlg::OnBtnMinSec()
{
	//<<20100402_JS.Kim 솔로몬
	//char cValue_MinSec[] = { '1', '6' };
	char cValue_MinSec[] = { MIN_DATA_CHART };//, SEC_DATA_CHART };
	//>>
	char* szMinSec[] = { "분" };//, "초" };

	long lCount = sizeof(szMinSec)/sizeof(char*);
	char cCurSelectedType = m_chType;

	BOOL bFound = FALSE;
	for(int nIndx = 0; nIndx < lCount; ++nIndx)
	{
		if(cCurSelectedType == cValue_MinSec[nIndx])
		{
			bFound = TRUE;
			nIndx++;
			nIndx %= lCount;
			break;
		}
	}

	if(bFound)
	{
		m_chType = cValue_MinSec[nIndx];		
	}
	else
	{
		CString strTitle = m_btnMinSec.GetBtnTitle();
		for(nIndx = 0; nIndx < lCount; ++nIndx)
		{
			if(!strTitle.Compare(szMinSec[nIndx]))
			{
				m_chType = cValue_MinSec[nIndx];
				break;
			}
		}
	}

	CString strNumber;
	m_editPeriod.GetWindowText(strNumber);
	
	SetType(FALSE);

	m_btnMinSec.SetBtnTitle(szMinSec[nIndx]);
	m_btnMinSec.FP_vSetToolTipText(szMinSec[nIndx]);
	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, atol(strNumber));
}

void CMiniBarDlg::OnBtnTick()
{
	CString strNumber;
	m_editPeriod.GetWindowText(strNumber);
	
	SetType(FALSE);
	m_chType = TICK_DATA_CHART;

	m_btnTick.SetBtnTitle("틱");
	m_btnTick.FP_vSetToolTipText("틱");
	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, atol(strNumber));
}

void CMiniBarDlg::OnBtn_Sel_DayWeekMonth()
{
	//<<20100402_JS.Kim 솔로몬
	//char cValue_DayWeekMonth[] = { '2', '3', '4' };
	char cValue_DayWeekMonth[] = { DAY_DATA_CHART, WEEK_DATA_CHART, MONTH_DATA_CHART };
	//>>
	char* szDayWeekMonth[] = { "일", "주", "월" };

	long lCount = sizeof(szDayWeekMonth)/sizeof(char*);

	CMenu menu;
    menu.CreatePopupMenu();

	for(int nIndx=0; nIndx < lCount; ++nIndx)
		menu.AppendMenu(MF_STRING, (UINT)1000+nIndx, szDayWeekMonth[nIndx]);
	
	CRect rect;
	GetDlgItem(IDC_BTN_SEL_DAYWEEKMONTH)->GetWindowRect(rect);

	UINT uFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(uFlags, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();

	if(nRetValue-1000 < lCount)
	{
		m_chType = cValue_DayWeekMonth[nRetValue-1000];
		CString strNumber;
		m_editPeriod.GetWindowText(strNumber);

		SetType(FALSE);
		
		m_btnDayWeekMonth.SetBtnTitle(szDayWeekMonth[nRetValue-1000]);
		m_btnDayWeekMonth.FP_vSetToolTipText(szDayWeekMonth[nRetValue-1000]);
		m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, atol(strNumber));
	}
}

void CMiniBarDlg::OnBtn_Sel_MinSec()
{
	//<<20100402_JS.Kim 솔로몬
	//char cValue_MinSec[] = { '1', '6' };
	char cValue_MinSec[] = { MIN_DATA_CHART };//, SEC_DATA_CHART };
	char* szMinSec[] = { "분" };//, "초" };
	//>>

	long lCount = sizeof(szMinSec)/sizeof(char*);

	CMenu menu;
    menu.CreatePopupMenu();

	for(int nIndx=0; nIndx < lCount; ++nIndx)
		menu.AppendMenu(MF_STRING, (UINT)1000+nIndx, szMinSec[nIndx]);
	
	CRect rect;
	GetDlgItem(IDC_BTN_SEL_MINSEC)->GetWindowRect(rect);

	UINT uFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(uFlags, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();

	if(nRetValue-1000 < lCount)
	{
		m_chType = cValue_MinSec[nRetValue-1000];
		CString strNumber;
		m_editPeriod.GetWindowText(strNumber);

		SetType(FALSE);

		m_btnMinSec.SetBtnTitle(szMinSec[nRetValue-1000]);
		m_btnMinSec.FP_vSetToolTipText(szMinSec[nRetValue-1000]);
		m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, atol(strNumber));
	}
}

void CMiniBarDlg::MoveToLeftRight(UINT nMoveDirection)
{
	switch(nMoveDirection)
	{
	case 0:
	case 2:
		m_btnMoveToLeftRight.FP_vSetToolTipText("열기");
		m_btnMoveToLeftRight.SetCheck(1);
		m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 56, 58);

		if(m_cbMarket.GetSafeHwnd() )//&& m_ctrlCodeInput.GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			//m_ctrlCodeInput.GetWindowRect(&rcCodeInput);
			int cx = 0;
			if(((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART)
				cx = rcCodeInput.Width();
			else
				cx = rcMarketCombo.Width() + rcCodeInput.Width();

			m_cbMarket.ShowWindow(SW_HIDE);
			//m_ctrlCodeInput.ShowWindow(SW_HIDE);

			UINT nCtrlId[] = {
				IDC_BTN_MOVETOLEFTRIGHT,   
				IDC_BTN_DAYWEEKMONTH,    
				IDC_BTN_SEL_DAYWEEKMONTH, 
				IDC_BTN_MINSEC,
				IDC_BTN_SEL_MINSEC,
				IDC_BTN_TICK,
				IDC_EDITPERIOD,
				IDC_BTPERIOD,
				IDC_EDITCNTOFDATALL
			};

			CRect rcCtrl;
			int nCnt = sizeof(nCtrlId) / sizeof(UINT);
			for(int i = 0; i < nCnt ; i++)
			{
				GetDlgItem(nCtrlId[i])->GetWindowRect(rcCtrl);
				ScreenToClient(rcCtrl);
				
				int nXPos = rcCtrl.left - cx;
				rcCtrl.OffsetRect(-cx, 0);

				GetDlgItem(nCtrlId[i])->MoveWindow(rcCtrl);
				GetDlgItem(nCtrlId[i])->ShowWindow(SW_SHOW);
			}
		}
		break;
	case 1:
		m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
		m_btnMoveToLeftRight.SetCheck(0);
		m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 60, 62);


		if(m_cbMarket.GetSafeHwnd() )//&& m_ctrlCodeInput.GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			//m_ctrlCodeInput.GetWindowRect(&rcCodeInput);
			int cx = 0;
			if(((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART)
				cx = rcCodeInput.Width();
			else
				cx = rcMarketCombo.Width() + rcCodeInput.Width();

			UINT nCtrlId[] = {
				IDC_BTN_MOVETOLEFTRIGHT,   
				IDC_BTN_DAYWEEKMONTH,    
				IDC_BTN_SEL_DAYWEEKMONTH, 
				IDC_BTN_MINSEC,
				IDC_BTN_SEL_MINSEC,
				IDC_BTN_TICK,
				IDC_EDITPERIOD,
				IDC_BTPERIOD,
				IDC_EDITCNTOFDATALL
			};

			CRect rcCtrl;
			int nCnt = sizeof(nCtrlId) / sizeof(UINT);
			for(int i = 0; i < nCnt ; i++)
			{
				GetDlgItem(nCtrlId[i])->GetWindowRect(rcCtrl);
				ScreenToClient(rcCtrl);
				
				int nXPos = rcCtrl.left + cx;
				rcCtrl.OffsetRect(cx, 0);

				GetDlgItem(nCtrlId[i])->MoveWindow(rcCtrl);
				GetDlgItem(nCtrlId[i])->ShowWindow(SW_SHOW);
			}

			m_cbMarket.ShowWindow(SW_SHOW);
			//m_ctrlCodeInput.ShowWindow(SW_SHOW);
		}
		break;
	}
}

void CMiniBarDlg::OnMoveToLeftRight()
{
	UINT nMoveDirection = GetMoveDirection();
	if(nMoveDirection == 0 || nMoveDirection == 2)
	{
		SetMoveDirection(1);

		// MoveToLeft
		MoveToLeftRight(GetMoveDirection());
	}
	else if(nMoveDirection == 1)
	{
		SetMoveDirection(0);

		// MoveToRight
		MoveToLeftRight(GetMoveDirection());
	}
}