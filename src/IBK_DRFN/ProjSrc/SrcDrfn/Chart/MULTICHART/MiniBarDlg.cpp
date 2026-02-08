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

const UINT RMSG_GET_CONTROLHANDLE = ::RegisterWindowMessage(_T("RMSG_GET_CONTROLHANDLE"));
//#define timer_CMiniBarDlg_CodeCync		1

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
	DDX_Control(pDX, IDC_BTINITIAL, m_btnInitail);
	DDX_Control(pDX, IDC_BTN_REQUEST, m_btnReuest);
	DDX_Control(pDX, IDC_CHK_CODESYNC, m_btnCodeSync);
	DDX_Control(pDX, IDC_CHK_TYPE2, m_btnLayout2);
	DDX_Control(pDX, IDC_CHK_TYPE1, m_btnLayout1);
	DDX_Control(pDX, IDC_BTN_ALLDAY, m_btnAllDay);
	DDX_Control(pDX, IDC_BTN_UPDOWN, m_btnUpDown);
	DDX_Control(pDX, IDC_BTN_ONEDAY, m_btnOneDay);
	DDX_Control(pDX, IDC_BTN_LOWHIGH, m_btnLowHigh);
	DDX_Control(pDX, IDC_BTN_MOVETOLEFTRIGHT, m_btnMoveToLeftRight);
	DDX_Control(pDX, IDC_BTN_DAYWEEKMONTH, m_btnDayWeekMonth);
	DDX_Control(pDX, IDC_BTN_MINSEC, m_btnMinSec);
	DDX_Control(pDX, IDC_BTN_TICK, m_btnTick);
	DDX_Control(pDX, IDC_BTN_SEL_DAYWEEKMONTH, m_btnSel_DayWeekMonth);
	DDX_Control(pDX, IDC_BTN_SEL_MINSEC, m_btnSel_MinSec);
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
	DDX_Control(pDX, IDC_BTPERIOD, m_btPeriod);
	DDX_Control(pDX, IDC_EDITPERIOD, m_editPeriod);
	DDX_Control(pDX, IDC_EDITCNTOFDATALL, m_editCntOfDatAll);
	DDX_Control(pDX, IDC_STJONGNAME, m_stJongName);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_BTCHARTMINI, m_btChart);
//	DDX_Control(pDX, IDC_BTINDEXMINI, m_btIndex);
//	DDX_Control(pDX, IDC_BTADDINDEXMINI, m_btAddIndex);

}


BEGIN_MESSAGE_MAP(CMiniBarDlg, CInputBarDlg)
	//{{AFX_MSG_MAP(CMiniBarDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnSelchangeCbmarket)
	ON_BN_CLICKED(IDC_BTN_ALLDAY, OnBtnAllday)
	ON_BN_CLICKED(IDC_BTN_UPDOWN, OnBtnUpdown)
	ON_BN_CLICKED(IDC_BTN_ONEDAY, OnBtnOneday)
	ON_BN_CLICKED(IDC_BTN_LOWHIGH, OnBtnLowhigh)
	ON_BN_CLICKED(IDC_CHK_TYPE1, OnChkType1)
	ON_BN_CLICKED(IDC_CHK_TYPE2, OnChkType2)
	ON_BN_CLICKED(IDC_CHK_CODESYNC, OnChkCodesync)
	ON_BN_CLICKED(IDC_BTN_REQUEST, OnBtnRequest)
	ON_BN_CLICKED(IDC_BTN_MOVETOLEFTRIGHT, OnMoveToLeftRight)
	ON_BN_CLICKED(IDC_BTN_DAYWEEKMONTH, OnBtnDayWeekMonth)
	ON_BN_CLICKED(IDC_BTN_MINSEC, OnBtnMinSec)
	ON_BN_CLICKED(IDC_BTN_TICK, OnBtnTick)
	ON_BN_CLICKED(IDC_BTN_SEL_DAYWEEKMONTH, OnBtn_Sel_DayWeekMonth)
	ON_BN_CLICKED(IDC_BTN_SEL_MINSEC, OnBtn_Sel_MinSec)
	ON_BN_CLICKED(IDC_BTINITIAL, OnBtinitial)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_BTCHARTMINI, OnBtchart)
//	ON_BN_CLICKED(IDC_BTINDEXMINI, OnBtindex)
////	ON_BN_CLICKED(IDC_BTADDINDEXMINI, OnBtaddindex)
//	ON_BN_CLICKED(IDC_CHKSHOWSCALE2, OnChkshowscale2)
//	ON_BN_CLICKED(IDC_BTN_CHANGSYNIT, OnBtnChangsynit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniBarDlg message handlers

void CMiniBarDlg::SetCodeName(int nMarketType, LPCSTR szCode, LPCSTR szName)
{
	if (m_nMarketType==nMarketType)
		m_stJongName.SetText(szName);

	//@Solomon	091120SM057	-->
	//if (m_nMarketType == CMasterTypeInfo::UPJONG_CHART) : KHD : 미니차트에도 초는 안들어간다.
	{
		if (m_btnSel_MinSec.GetSafeHwnd())
			m_btnSel_MinSec.EnableWindow(FALSE);
	}
	//@Solomon	091120SM057	<--
}

void CMiniBarDlg::Init()
{
	m_pCodeInput->SetUseFutureOption(true);	//@Solomon

	m_stJongName.SetStaticColor(RGB(132,132,132), RGB(255,255,255) , RGB(0,0,0));
	MoveCodeInputCtrl();
	{
		CRect rectComp;
		m_pCodeInput->GetWindowRect(&rectComp);
		ScreenToClient(rectComp);

		if(theStdParent->m_nScreenID == nScreenID_6401)
		{
			m_pCodeInput->SetUnit("선물");
			//m_pCodeInput->SetFixUnit(futureCODE);
//KHD			m_pCodeInput->SetFixUnit(linkFCODE);
		}

		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, 63, 20, SWP_NOZORDER);
	}

	//m_editCode.SetPath(theStdParent->m_strRootPath);
	m_strImagePath = theStdParent->m_strRootPath + "\\Image";
	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;

	m_brBkColor.CreateSolidBrush(m_crBk);

//@유진추가
	m_pIL_W20H20 = &(theStdParent->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return;

	m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
	m_btnMoveToLeftRight.SetCheck(0);
	m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 60, 62);

	m_btnDayWeekMonth.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "일", TRUE);
	m_btnDayWeekMonth.FP_vSetToolTipText("일");

	m_btnMinSec.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "분", TRUE);
	m_btnMinSec.FP_vSetToolTipText("분");

	m_btnLayout1.FP_vSetImage( m_pIL_W20H20, 120, 122, TRUE, "", TRUE);
	m_btnLayout2.FP_vSetImage( m_pIL_W20H20, 123, 125, TRUE, "", TRUE);

	m_btnTick.FP_vSetImage( m_pIL_W20H20, 80, 82, TRUE, "틱", TRUE);
	m_btnTick.FP_vSetToolTipText("틱");

	m_pIL_W17H20 = &(theStdParent->m_ImgList_W17H20);
	if(m_pIL_W17H20 == NULL) return;

	m_btnSel_DayWeekMonth.FP_vSetImage( m_pIL_W17H20, 0, 2);
	m_btnSel_MinSec.FP_vSetImage( m_pIL_W17H20, 0, 2, FALSE, "", FALSE, 3);
	m_btPeriod.FP_vSetImage( m_pIL_W17H20, 0, 2);
	m_editPeriod.SetLimitText(3);

	m_pIL_W46H20 = &(theStdParent->m_ImgList_W46H20);
	m_btnInitail.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "초기화", FALSE);
	m_btnInitail.FP_vSetToolTipText("전체차트초기화");

	LoadValue();
	
	CString sTemp;
	char szTemp[10];
	memset(szTemp, 0x00, 10);
	CStdDialog* pParent = m_pStdDialog;
	CString strConfig = pParent->m_strConfig;

//	m_hKSJongName = (HWND)pParent->SendMessage(RMSG_GET_CONTROLHANDLE, NULL, (LPARAM)"ItemName1");
//	::SetParent(m_hKSJongName, m_hWnd);
//	if(m_pCodeInput) m_pCodeInput->SetWindowPos(NULL, 1, 2, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
//	m_pCodeInput->SetWindowPos(NULL, 1, 2, 88, 20, SWP_NOZORDER);

	m_btnUpDown.SetCheck(TRUE);

	if(pParent->IsSimpleChart())
	{
		if(pParent->m_nScreenID == nScreenID_6204)
		{
			CImageList* pIL_W43H20 = &(theStdParent->m_ImgList_W43H20);

			m_btnAllDay.FP_vSetImage( pIL_W43H20, 0, 2, TRUE, "전체", TRUE);
			m_btnAllDay.FP_vSetToolTipText("전체일수");

			m_btnOneDay.FP_vSetImage( pIL_W43H20, 0, 2, TRUE, "당일", TRUE);
			m_btnOneDay.FP_vSetToolTipText("당일");

			m_btnUpDown.FP_vSetImage( pIL_W43H20, 0, 2, TRUE, "고/저", TRUE);
			m_btnUpDown.FP_vSetToolTipText("고가/저가");

			m_btnLowHigh.FP_vSetImage( pIL_W43H20, 0, 2, TRUE, "상/하", TRUE);
			m_btnLowHigh.FP_vSetToolTipText("상한가/하한가");
			
			SetAllPosition(10);

			::GetPrivateProfileString("0000", "Scale", "0", szTemp, sizeof(szTemp), strConfig);
			
			if(szTemp[0] == '1')
			{
				m_btnUpDown.SetCheck(FALSE);
				m_btnLowHigh.SetCheck(TRUE);
			}

			m_btnAllDay.SetCheck(TRUE);

			::GetPrivateProfileString("0000", "Type", "0", szTemp, sizeof(szTemp), strConfig);
		}
		else if(pParent->m_nScreenID == nScreenID_6401)
		{
			CImageList* pIL_W50H20 = &(theStdParent->m_ImgList_W50H20);
			m_btnReuest.FP_vSetImage( pIL_W50H20, 0, 2, TRUE, "재조회");
			m_btnReuest.FP_vSetToolTipText("재조회");

			SetAllPosition(30);

			m_btnOneDay.SetCheck(TRUE);

			::GetPrivateProfileString("0000", "Type", "1", szTemp, sizeof(szTemp), strConfig);
		}
		else if(pParent->m_nScreenID == nScreenID_6301)
		{
			CImageList* pIL_W43H20 = &(theStdParent->m_ImgList_W43H20);

			m_btnCodeSync.FP_vSetImage( pIL_W43H20, 0, 2, TRUE, "동기화", TRUE);
			m_btnCodeSync.FP_vSetToolTipText("동기화");

			::GetPrivateProfileString("0000", "Type", "3", szTemp, sizeof(szTemp), strConfig);
			SetAllPosition(20);

			int nLayOut = ::GetPrivateProfileInt("DWM", "Layout", 1, strConfig);
			((CChartMng*)m_pChartMng)->SetChartLayout(nLayOut);

			if(nLayOut == 1)
				m_btnLayout1.SetCheck(TRUE);
			else if(nLayOut == 2)
				m_btnLayout2.SetCheck(TRUE);
			
			nLayOut = ::GetPrivateProfileInt("DWM", "CodeSync", 1, strConfig);
//			m_btnCodeSync.SetCheck(nLayOut);
 			m_btnCodeSync.SetCheck(0);
			m_btnInitail.ShowWindow(SW_HIDE);
		

			
// 			if(nLayOut)
// 			{
// 				// @Solomon DWN에서 자동으로 동기화화 처리되어야 하는데. ExtInputBar의 초기화루틴에 의해
// 				// 셋팅이 풀리기 때문에 다시 셋팅하는 루틴을 추가함.
// 				SetTimer(timer_CMiniBarDlg_CodeCync, 500, NULL);
// 			}
		}
// 		else if(pParent->m_nScreenID ==7122)
// 		{
// 		
// 			::GetPrivateProfileString("0000", "Type", "0", szTemp, sizeof(szTemp), strConfig);
// 			m_btnInitail.ShowWindow(SW_SHOW);
// 			SetAllPosition(10);
// 		}
		else
		{
			::GetPrivateProfileString("0000", "Type", "0", szTemp, sizeof(szTemp), strConfig);
			SetAllPosition(1);

			m_btnInitail.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		CRect rectComp;
		m_btnMoveToLeftRight.GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		int nRight = rectComp.left - 1;

		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);

//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
		//@Solomon-091026
		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);

		::GetPrivateProfileString("0000", "Type", "2", szTemp, sizeof(szTemp), strConfig);

		m_btnInitail.ShowWindow(SW_SHOW);
	}

	//@100122 m_editCntOfDatAll.SetLimitText(4);
	m_editCntOfDatAll.SetLimitText(3);
	
	if(szTemp[0] == DAY_DATA_CHART || szTemp[0] == WEEK_DATA_CHART || szTemp[0] == MONTH_DATA_CHART || szTemp[0] == YEAR_DATA_CHART)
	{
		if(szTemp[0] == DAY_DATA_CHART)
		{
			m_btnDayWeekMonth.SetBtnTitle("일");
			m_btnDayWeekMonth.FP_vSetToolTipText("일");

			m_editPeriod.SetWindowText("1");
			
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
		else if(szTemp[0] == YEAR_DATA_CHART)
		{
			m_btnDayWeekMonth.SetBtnTitle("년");
			m_btnDayWeekMonth.FP_vSetToolTipText("년");

			m_editPeriod.SetWindowText("1");			
			m_chType = YEAR_DATA_CHART;
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

	//@Solomon	091120SM057	-->
	if (m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		if (m_btnSel_MinSec.GetSafeHwnd())
			m_btnSel_MinSec.EnableWindow(FALSE);
	}
	//@Solomon	091120SM057	<--
}

void CMiniBarDlg::SetType(BOOL bSendChartMng/*=TRUE*/)
{	
	EnableWindow(FALSE);

	// 틱(0)/분(1)/일(2)/주(3)/월(4)/초(6)
	switch(m_chType)
	{
	case TICK_DATA_CHART:	
		m_btnDayWeekMonth.SetCheck(0);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(1);
		break;
	case SEC_DATA_CHART:	
		m_btnMinSec.SetBtnTitle("초");
		m_btnMinSec.FP_vSetToolTipText("초");

		m_btnDayWeekMonth.SetCheck(0);
		m_btnMinSec.SetCheck(1);
		m_btnTick.SetCheck(0);
		break;
	case MIN_DATA_CHART:	
		m_btnMinSec.SetBtnTitle("분");
		m_btnMinSec.FP_vSetToolTipText("분");

		m_btnDayWeekMonth.SetCheck(0);
		m_btnMinSec.SetCheck(1);
		m_btnTick.SetCheck(0);
		break;
	case DAY_DATA_CHART:
		m_editPeriod.EnableWindow(FALSE);
		{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(FALSE);}
		m_btnDayWeekMonth.SetBtnTitle("일");
		m_btnDayWeekMonth.FP_vSetToolTipText("일");

		m_btnDayWeekMonth.SetCheck(1);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(0);
		break;
	case WEEK_DATA_CHART:
		m_editPeriod.EnableWindow(FALSE);
		{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(FALSE);}
		m_btnDayWeekMonth.SetBtnTitle("주");
		m_btnDayWeekMonth.FP_vSetToolTipText("주");

		m_btnDayWeekMonth.SetCheck(1);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(0);
		break;
	case MONTH_DATA_CHART:
		m_editPeriod.EnableWindow(FALSE);
		{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(FALSE);}
		m_btnDayWeekMonth.SetBtnTitle("월");
		m_btnDayWeekMonth.FP_vSetToolTipText("월");

		m_btnDayWeekMonth.SetCheck(1);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(0);
		break;
	case YEAR_DATA_CHART:
		m_editPeriod.EnableWindow(FALSE);
		{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(FALSE);}
		m_btnDayWeekMonth.SetBtnTitle("년");
		m_btnDayWeekMonth.FP_vSetToolTipText("년");

		m_btnDayWeekMonth.SetCheck(1);
		m_btnMinSec.SetCheck(0);
		m_btnTick.SetCheck(0);
		break;
	}

	m_btnDayWeekMonth.Invalidate();
	m_btnMinSec.Invalidate();
	m_btnTick.Invalidate();
	
	if(bSendChartMng)
	{
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

				m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, nValue);//사용자 지정 기간
			}
		}
		else // 주 / 월 / 년
		{
			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_pChartMng->SendMessage(UMSG_SETTYPE,m_chType);
		}		
	}

	EnableWindow();
}

void CMiniBarDlg::OnDestroy() 
{
	CInputBarDlg::OnDestroy();
}

#define ID_SHARE_CODE_WAIT 1754
void CMiniBarDlg::OnSelchangeCbmarket() 
{
	if(!theStdParent->GetStatusEnable()) return;

	int nSelMarketType = m_cbMarket.GetCurSel();
	m_strCodeFromOutside = "";
	theStdParent->ChangeStockMode(nSelMarketType); 

	CString strCode;
	int nSetp=0;
	CStringArray arrCode, arrName;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	pMasterDataManager->GetRecentCodeNameArray(nSelMarketType, arrCode, arrName);
	if(arrCode.GetSize()>0)
	{
		strCode = arrCode.GetAt(0);
	}
	else
	{
		theStdParent->GetFirstCode(strCode, nSelMarketType, nSetp);
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
	int i = 0;
	while(pChild)
	{
//@Solomon 		if(m_hKSJongName == pChild->GetSafeHwnd())
//@Solomon 		{
//@Solomon 			pChild = pChild->GetNextWindow();
//@Solomon 			continue;
//@Solomon 		}

		//@Solomon-091026
		if(m_stJongName.GetSafeHwnd() == pChild->GetSafeHwnd())
 		{
 			pChild = pChild->GetNextWindow();
 			continue;
 		}

		pChild->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		pChild->SetWindowPos(NULL, rectComp.left - 45, rectComp.top, rectComp.Width(), 20, SWP_NOZORDER);

		if(pChild->GetDlgCtrlID() == IDC_CBMARKET)// || pChild->GetDlgCtrlID() == IDC_CTRL_CODEINPUT || pChild->GetDlgCtrlID() == IDC_BTCHARTMINI || pChild->GetDlgCtrlID() == IDC_BTINDEXMINI)
		{
			pChild->ShowWindow(SW_HIDE);
		}

		pChild = pChild->GetNextWindow();
	}

	if(nType == 10)
	{
		m_btnDayWeekMonth.ShowWindow(SW_HIDE);
		m_btnSel_DayWeekMonth.ShowWindow(SW_HIDE);

		m_btnDayWeekMonth.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		int nOffset = rectComp.left;

		m_btnMinSec.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		nOffset = rectComp.left - nOffset;

		UINT nCtrlId[] = {
			IDC_BTN_MINSEC,
			IDC_BTN_SEL_MINSEC,
			IDC_BTN_TICK,
			IDC_EDITPERIOD,
			IDC_BTPERIOD,
			IDC_EDITCNTOFDATALL,
			IDC_BTN_ALLDAY,
			IDC_BTN_UPDOWN,
			IDC_BTN_ONEDAY,
			IDC_BTN_LOWHIGH
		};

		CRect rcCtrl;
		int nCnt = sizeof(nCtrlId) / sizeof(UINT);
		for(int i = 0; i < nCnt ; i++)
		{
			GetDlgItem(nCtrlId[i])->GetWindowRect(rcCtrl);
			ScreenToClient(rcCtrl);
			
			rcCtrl.OffsetRect(-nOffset, 0);

			GetDlgItem(nCtrlId[i])->MoveWindow(rcCtrl);
			GetDlgItem(nCtrlId[i])->ShowWindow(SW_SHOW);
		}

		m_btnOneDay.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		m_btnInitail.GetWindowRect(rcCtrl);
		rectComp.OffsetRect(rectComp.Width() + 3, 0);
		rectComp.right = rectComp.left + rcCtrl.Width();
		m_btnInitail.MoveWindow(rectComp);

		m_btnAllDay.ShowWindow(SW_SHOW);
		m_btnUpDown.ShowWindow(SW_SHOW);
		m_btnOneDay.ShowWindow(SW_SHOW);
		m_btnLowHigh.ShowWindow(SW_SHOW);
		m_btnInitail.ShowWindow(SW_SHOW);
	}
	else if(nType == 20)
	{
		UINT nCtrlId[] = {
			IDC_CHK_TYPE1,
			IDC_CHK_TYPE2,
			IDC_CHK_CODESYNC
		};

		m_btnLayout1.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		int nOffset = rectComp.left;

		m_editCntOfDatAll.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		nOffset = nOffset - rectComp.right - 4;

		CRect rcCtrl;
		int nCnt = sizeof(nCtrlId) / sizeof(UINT);
		for( i = 0; i < nCnt ; i++)
		{
			GetDlgItem(nCtrlId[i])->GetWindowRect(rcCtrl);
			ScreenToClient(rcCtrl);
			
			rcCtrl.OffsetRect(-nOffset, 0);

			GetDlgItem(nCtrlId[i])->MoveWindow(rcCtrl);
			GetDlgItem(nCtrlId[i])->ShowWindow(SW_SHOW);
		}

		UINT nCtrlId2[] = {
			IDC_BTN_MOVETOLEFTRIGHT,
			IDC_BTN_DAYWEEKMONTH,
			IDC_BTN_SEL_DAYWEEKMONTH,
			IDC_BTN_MINSEC,
			IDC_BTN_SEL_MINSEC,
			IDC_BTN_TICK,
			IDC_EDITPERIOD,
			IDC_BTPERIOD,
			IDC_EDITCNTOFDATALL,
			IDC_CHK_TYPE1,
			IDC_CHK_TYPE2,
			IDC_CHK_CODESYNC
		};

		nCnt = sizeof(nCtrlId2) / sizeof(UINT);
		for(i = 0; i < nCnt ; i++)
		{
			GetDlgItem(nCtrlId2[i])->GetWindowRect(rcCtrl);
			ScreenToClient(rcCtrl);
			
			rcCtrl.OffsetRect(38, 0);

			GetDlgItem(nCtrlId2[i])->MoveWindow(rcCtrl);
			GetDlgItem(nCtrlId2[i])->ShowWindow(SW_SHOW);
		}

		m_stJongName.SetWindowPos(NULL, 0, 0, 97, 20, SWP_NOMOVE|SWP_NOZORDER);
	}
	else if(nType == 30)
	{
		UINT nCtrlId[] = {
			IDC_BTN_MOVETOLEFTRIGHT,
			IDC_BTN_DAYWEEKMONTH,
			IDC_BTN_SEL_DAYWEEKMONTH,
			IDC_BTN_MINSEC,
			IDC_BTN_SEL_MINSEC,
			IDC_BTN_TICK,
			IDC_EDITPERIOD,
			IDC_BTPERIOD,
			IDC_EDITCNTOFDATALL,
			IDC_BTN_ALLDAY,
			IDC_BTN_UPDOWN,
			IDC_BTN_ONEDAY,
			IDC_BTN_LOWHIGH,
			IDC_BTINITIAL
		};

		CRect rcCtrl;
		int nCnt = sizeof(nCtrlId) / sizeof(UINT);
		for(int i = 0; i < nCnt ; i++)
		{
			GetDlgItem(nCtrlId[i])->ShowWindow(SW_HIDE);
		}

//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, 120, rectComp.Height(), SWP_NOZORDER);
		//@Solomon-091026
		//m_stJongName.SetWindowPos(NULL, 0, 0, 120, 20, SWP_NOMOVE|SWP_NOZORDER);
		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, 120, rectComp.Height(), SWP_NOZORDER);
		m_stJongName.GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		GetDlgItem(IDC_BTN_REQUEST)->SetWindowPos(NULL, rectComp.right + 1, rectComp.top, 50, rectComp.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);

	}

	if(m_btnMoveToLeftRight.IsWindowVisible())
	{
		m_btnMoveToLeftRight.GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		int nRight = rectComp.left - 1;

		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
		//@Solomon-091026
		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
	}
}

//void CMiniBarDlg::OnBtnChangsynit() 
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
	switch(message)
	{
		case UMSG_END_REQUEST_CHARTDATA:
		{	
			if(theStdParent->m_nScreenID != nScreenID_6301)
			{
				int nScaleType = m_btnUpDown.GetCheck() ? 0 : 2;
				if(theStdParent->m_nScreenID == nScreenID_6204)
					m_pChartMng->SendMessage(RMSG_CHANGEVERTSCALETYPE, nScaleType);

				if(theStdParent->m_nScreenID == nScreenID_6401)
				{
					CChartItem* pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
//					pChartItem->AddChartAllBlock("선차트");
					pChartItem->ChangeGraphTitle(0, 0, 1, "전일가격");
					pChartItem->ChangeGraphColor("전일가격", "16,99,191;16,99,191;16,99,191;16,99,191;16,99,191;16,99,191;16,99,191;16,99,191;16,99,191;16,99,191");
					pChartItem->SetBWholeView(TRUE);
					pChartItem->SetBAddGraphMenuShow(FALSE);

					//LH/LC/HC표시여부 셋팅 : 보이지않게.
					pChartItem->ChangeGraphDrawingData("선가격차트", FALSE, FALSE, FALSE, FALSE, 0);

					pChartItem->SetDrawBaseLine(FALSE);
// #ifdef __MULTI_LANG__	//@Solomon-MultiLang:091117
// 					pChartItem->SetDrawBaseLine(FALSE);
// #else
// 					pChartItem->SetDrawBaseLine(FALSE);
// #endif

//					pChartItem->SetS
				}
			}
/*
			else
			{
				BOOL bCheck = m_btnCodeSync.GetCheck();
				if(bCheck)
					((CChartMng*)m_pChartMng)->SetInLoop(TRUE);
			}
*/
		}
		break;
		case UMSG_CHART_INIT_FROMOCX:
			if(theStdParent->m_nScreenID == nScreenID_6401)
			{
				m_btnAllDay.SetCheck(FALSE);
				m_btnOneDay.SetCheck(TRUE);
			}
			else
			{
				m_btnAllDay.SetCheck(TRUE);
				m_btnUpDown.SetCheck(TRUE);
				m_btnOneDay.SetCheck(FALSE);
				m_btnLowHigh.SetCheck(FALSE);

				//@Solomon:091123SM070 초기화는 "1"으로 변경한다.
				m_pChartMng->SendMessage(UMSG_SETTYPE, DAY_DATA_CHART, 1);
				m_pChartMng->SendMessage(UMSG_SETTYPE, TICK_DATA_CHART, 1);
			}
		break;
		case UMSG_INPUTCODETOINPUTBAR:
			if(theStdParent->m_nScreenID == nScreenID_6401)
			{
				STINPUTCODEDATA2 *pStInputCodeData2 = (STINPUTCODEDATA2 *)wParam;
				if(GetCurrentCode() != pStInputCodeData2->m_strCode || pStInputCodeData2->m_bRequestWithoutHesitation)
				{
					CChartItem* pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
					if(pChartItem->IsBuiltChart())
					{
						pChartItem->SetCurrentRQ("DEFAULT");
						pChartItem->m_strSelectedRQ = "DEFAULT";

						pChartItem->SetBWholeView(FALSE);
						
						pChartItem->ResetChart();				
						pChartItem->ClearData();
						
						((CChartMng*)m_pChartMng)->SetFutPrevChart(FALSE);
						((CChartMng*)m_pChartMng)->InitInLoop();
						
						((CChartMng*)m_pChartMng)->SetMode(CHANGE_ITEM);
						((CChartMng*)m_pChartMng)->SetAddMode(FALSE);
					}
				}
			}
		break;
	}
	return CInputBarDlg::WindowProc(message, wParam, lParam);
}

void CMiniBarDlg::OnBtnDayWeekMonth()
{
	char cValue_DayWeekMonth[] = { '2', '3', '4' };
	char* szDayWeekMonth[] = { "일", "주", "월" };

	long lCount = sizeof(szDayWeekMonth)/sizeof(char*);
	char cCurSelectedType = m_chType;

	BOOL bFound = FALSE;
	int nIndx = 0;
	for( nIndx = 0; nIndx < lCount; ++nIndx)
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

	//@Solomon 일/주/월 데이터가 1봉만제공. -->
	m_editPeriod.SetWindowText("1");
	m_editPeriod.EnableWindow(FALSE);
	m_nDay = 1;
	CWnd* pWnd = GetDlgItem(IDC_BTPERIOD);
	if (pWnd)
		pWnd->EnableWindow(FALSE);
	//@Solomon 일데이터가 1봉만제공. <--

	SetType(FALSE);

	m_btnDayWeekMonth.SetBtnTitle(szDayWeekMonth[nIndx]);
	m_btnDayWeekMonth.FP_vSetToolTipText(szDayWeekMonth[nIndx]);
	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, m_nDay);
}

void CMiniBarDlg::OnBtnMinSec()
{
	m_editPeriod.EnableWindow(TRUE);
	{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(TRUE);}

	char cValue_MinSec[] = { '1'};//KHD '6' };
	char* szMinSec[] = { "분"};//KHD : "초" };

	long lCount = sizeof(szMinSec)/sizeof(char*);
	char cCurSelectedType = m_chType;

	BOOL bFound = FALSE;
	int nIndx = 0;
	for( nIndx = 0; nIndx < lCount; ++nIndx)
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

	//@Solomon	091120SM057	-->
	if (m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_chType = MIN_DATA_CHART;
		nIndx = 0;
	}
	//@Solomon	091120SM057	<--

	CString strNumber;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
		{
			char szType[2] = {NULL, };
			::sprintf(szType, "%c", m_chType);
			m_nSec = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, atoi(szType));
		}

		strNumber.Format("%d", m_nSec);

		if(m_nSec >= 300)
			strNumber = "0";

		m_editPeriod.SetWindowText(strNumber);
	}
	else
	{
		m_editPeriod.GetWindowText(strNumber);
		m_nSec = atoi(strNumber);
	}

	SetType(FALSE);

	m_btnMinSec.SetBtnTitle(szMinSec[nIndx]);
	m_btnMinSec.FP_vSetToolTipText(szMinSec[nIndx]);

	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, m_nSec);
}

void CMiniBarDlg::OnBtnTick()
{
	CString strNumber;
	if(!GetUseUserInputPeriod())
	{
		if(m_pChartMng->GetSafeHwnd())
			m_nTick = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 0);	

		strNumber.Format("%d", m_nTick); 
		m_editPeriod.SetWindowText(strNumber);
	}
	else
	{
		m_editPeriod.GetWindowText(strNumber);
		m_nTick = atoi(strNumber);
	}

	SetType(FALSE);
	m_chType = TICK_DATA_CHART;

	m_btnTick.SetBtnTitle("틱");
	m_btnTick.FP_vSetToolTipText("틱");

	m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, m_nTick);

	m_editPeriod.EnableWindow(TRUE);
	{CWnd* pWnd = GetDlgItem(IDC_BTPERIOD); if(pWnd) pWnd->EnableWindow(TRUE);}
}

void CMiniBarDlg::OnBtn_Sel_DayWeekMonth()
{
	char cValue_DayWeekMonth[] = { '2', '3', '4' };
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

		//@Solomon 일데이터가 1봉만제공. -->
		if(m_chType=='2')
		{
			m_editPeriod.SetWindowText("1");
		}
		m_editPeriod.EnableWindow(FALSE);
		//@Solomon 일데이터가 1봉만제공. <--

		CString strNumber;
		if(!GetUseUserInputPeriod())
		{
			if(m_pChartMng->GetSafeHwnd())
				m_nDay = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, 2);
			
			strNumber.Format("%d", m_nDay);
			m_editPeriod.SetWindowText(strNumber);	
		}
		else
		{
			m_editPeriod.GetWindowText(strNumber);
			m_nDay = atoi(strNumber);
		}
		
		SetType(FALSE);
		
		m_btnDayWeekMonth.SetBtnTitle(szDayWeekMonth[nRetValue-1000]);
		m_btnDayWeekMonth.FP_vSetToolTipText(szDayWeekMonth[nRetValue-1000]);
		m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, m_nDay);
	}
}

void CMiniBarDlg::OnBtn_Sel_MinSec()
{
	char cValue_MinSec[] = { '1', '6' };
	char* szMinSec[] = { "분", "초" };

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
		if(!GetUseUserInputPeriod())
		{
			if(m_pChartMng->GetSafeHwnd())
			{
				char szType[2] = {NULL, };
				::sprintf(szType, "%c", m_chType);
				m_nSec = m_pChartMng->SendMessage( UMSG_GET_TICKMINDAY_VALUE, 0, atoi(szType));
			}

			strNumber.Format("%d", m_nSec);

			if(m_nSec >= 300)
				strNumber = "0";

			m_editPeriod.SetWindowText(strNumber);
		}
		else
		{
			m_editPeriod.GetWindowText(strNumber);
			m_nSec = atoi(strNumber);
		}

		SetType(FALSE);

		m_btnMinSec.SetBtnTitle(szMinSec[nRetValue-1000]);
		m_btnMinSec.FP_vSetToolTipText(szMinSec[nRetValue-1000]);
		m_pChartMng->SendMessage(UMSG_SETTYPE, m_chType, m_nSec);
	}
}

void CMiniBarDlg::MoveToLeftRight(UINT nMoveDirection)
{
	CStdDialog* pParent = (CStdDialog*)GetParent();

	switch(nMoveDirection)
	{
	case 0:
	case 2:
		m_btnMoveToLeftRight.FP_vSetToolTipText("열기");
		m_btnMoveToLeftRight.SetCheck(1);
		m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 56, 58);

		if(m_cbMarket.GetSafeHwnd() && m_pCodeInput->GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			m_pCodeInput->GetWindowRect(&rcCodeInput);
			int nItemNameWidth = 0;
			{
				CRect rcItemName;
				m_stJongName.GetWindowRect(&rcItemName);
				ScreenToClient(rcItemName);
				nItemNameWidth = rcItemName.right;
			}
			int cx = nItemNameWidth;

			m_cbMarket.ShowWindow(SW_HIDE);
			m_pCodeInput->ShowWindow(SW_HIDE);
			m_stJongName.ShowWindow(SW_HIDE);

			if(pParent->m_nScreenID == nScreenID_6204)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_BTN_ALLDAY,
								IDC_BTN_UPDOWN,
								IDC_BTN_ONEDAY,
								IDC_BTN_LOWHIGH,
								IDC_BTINITIAL
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
			else if(pParent->m_nScreenID == nScreenID_6301)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_DAYWEEKMONTH,
								IDC_BTN_SEL_DAYWEEKMONTH,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_CHK_TYPE1,
								IDC_CHK_TYPE2,
								IDC_CHK_CODESYNC
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
			else if(pParent->m_nScreenID == nScreenID_6401)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_DAYWEEKMONTH,
								IDC_BTN_SEL_DAYWEEKMONTH,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_BTN_ALLDAY,
								IDC_BTN_UPDOWN,
								IDC_BTN_ONEDAY,
								IDC_BTN_LOWHIGH,
								IDC_BTINITIAL
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
			else
			{
				UINT nCtrlId[] = {
					IDC_BTN_MOVETOLEFTRIGHT,   
						IDC_BTN_DAYWEEKMONTH,    
						IDC_BTN_SEL_DAYWEEKMONTH, 
						IDC_BTN_MINSEC,
						IDC_BTN_SEL_MINSEC,
						IDC_BTN_TICK,
						IDC_EDITPERIOD,
						IDC_BTPERIOD,
						IDC_EDITCNTOFDATALL,
						IDC_BTN_ALLDAY,
						IDC_BTN_UPDOWN,
						IDC_BTN_ONEDAY,
						IDC_BTN_LOWHIGH,
						IDC_BTINITIAL
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
		}
		break;
	case 1:
		m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
		m_btnMoveToLeftRight.SetCheck(0);
		m_btnMoveToLeftRight.FP_vSetImage( m_pIL_W20H20, 60, 62);


		if(m_cbMarket.GetSafeHwnd() && m_pCodeInput->GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			m_pCodeInput->GetWindowRect(&rcCodeInput);

			int nItemNameWidth = 0;
			{
				CRect rcItemName;
				m_stJongName.GetWindowRect(&rcItemName);
				ScreenToClient(rcItemName);
				nItemNameWidth = rcItemName.right;
			}

			int cx = nItemNameWidth;

			if(pParent->m_nScreenID == nScreenID_6204)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_BTN_ALLDAY,
								IDC_BTN_UPDOWN,
								IDC_BTN_ONEDAY,
								IDC_BTN_LOWHIGH,
								IDC_BTINITIAL
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
			}
			else if(pParent->m_nScreenID == nScreenID_6301)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_DAYWEEKMONTH,
								IDC_BTN_SEL_DAYWEEKMONTH,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_CHK_TYPE1,
								IDC_CHK_TYPE2,
								IDC_CHK_CODESYNC
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
			}
			else if(pParent->m_nScreenID == nScreenID_6401)
			{
				UINT nCtrlId[] = {
								IDC_BTN_MOVETOLEFTRIGHT,
								IDC_BTN_DAYWEEKMONTH,
								IDC_BTN_SEL_DAYWEEKMONTH,
								IDC_BTN_MINSEC,
								IDC_BTN_SEL_MINSEC,
								IDC_BTN_TICK,
								IDC_EDITPERIOD,
								IDC_BTPERIOD,
								IDC_EDITCNTOFDATALL,
								IDC_BTN_ALLDAY,
								IDC_BTN_UPDOWN,
								IDC_BTN_ONEDAY,
								IDC_BTN_LOWHIGH,
								IDC_BTINITIAL
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
			}
			else
			{
				UINT nCtrlId[] = {
					IDC_BTN_MOVETOLEFTRIGHT,   
						IDC_BTN_DAYWEEKMONTH,    
						IDC_BTN_SEL_DAYWEEKMONTH, 
						IDC_BTN_MINSEC,
						IDC_BTN_SEL_MINSEC,
						IDC_BTN_TICK,
						IDC_EDITPERIOD,
						IDC_BTPERIOD,
						IDC_EDITCNTOFDATALL,
						IDC_BTN_ALLDAY,
						IDC_BTN_UPDOWN,
						IDC_BTN_ONEDAY,
						IDC_BTN_LOWHIGH,
						IDC_BTINITIAL
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
			}

			m_cbMarket.ShowWindow(SW_SHOW);
			m_pCodeInput->ShowWindow(SW_SHOW);
			m_stJongName.ShowWindow(SW_SHOW);
		}
		break;
	}

	if(pParent->m_nScreenID == nScreenID_6204)
	{
		m_btnDayWeekMonth.ShowWindow(SW_HIDE);
		m_btnSel_DayWeekMonth.ShowWindow(SW_HIDE);
	}
	else if(pParent->m_nScreenID == nScreenID_6301)
		m_btnInitail.ShowWindow(SW_HIDE);
	else
	{
		m_btnAllDay.ShowWindow(SW_HIDE);
		m_btnUpDown.ShowWindow(SW_HIDE);
		m_btnOneDay.ShowWindow(SW_HIDE);
		m_btnLowHigh.ShowWindow(SW_HIDE);
	}

	Invalidate();
}

void CMiniBarDlg::OnMoveToLeftRight()
{
	UINT nMoveDirection = GetMoveDirection();
	if(nMoveDirection == 0 || nMoveDirection == 2)
	{
		SetMoveDirection(1);

		// MoveToLeft
		MoveToLeftRight(GetMoveDirection());
		m_stJongName.ShowWindow(SW_SHOW);
	}
	else if(nMoveDirection == 1)
	{
		SetMoveDirection(0);

		// MoveToRight
		MoveToLeftRight(GetMoveDirection());
		m_stJongName.ShowWindow(SW_HIDE);
	}
}

void CMiniBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CInputBarDlg::OnSize(nType, cx, cy);	
}

void CMiniBarDlg::OnBtnAllday() 
{
	m_btnOneDay.SetCheck(FALSE);
	m_btnAllDay.SetCheck(TRUE);

	if(m_pChartMng)
	{
		m_pChartMng->SendMessage(RMSG_SETQUERYOPT, 0);
		m_pChartMng->SendMessage(UMSG_REQUEST_FORQUERYOPT);
	}
}

void CMiniBarDlg::OnBtnUpdown() 
{
	m_btnUpDown.SetCheck(TRUE);
	m_btnLowHigh.SetCheck(FALSE);

	if(m_pChartMng)
		m_pChartMng->SendMessage(RMSG_CHANGEVERTSCALETYPE, 0);
}

void CMiniBarDlg::OnBtnOneday() 
{
	m_btnOneDay.SetCheck(TRUE);
	m_btnAllDay.SetCheck(FALSE);

	if(m_pChartMng)
	{
		m_pChartMng->SendMessage(RMSG_SETQUERYOPT, 1);
		m_pChartMng->SendMessage(UMSG_REQUEST_FORQUERYOPT);
	}
}

void CMiniBarDlg::OnBtnLowhigh() 
{
	m_btnUpDown.SetCheck(FALSE);
	m_btnLowHigh.SetCheck(TRUE);

	if(m_pChartMng)
		m_pChartMng->SendMessage(RMSG_CHANGEVERTSCALETYPE, 2);
}

void CMiniBarDlg::SaveIniValue()
{
	CInputBarDlg::SaveIniValue();

	CStdDialog* pParent = m_pStdDialog;
	CString strUserDir = pParent->m_strConfig;
	
	char szTemp[8];

	BOOL bCheck = m_btnLowHigh.GetCheck();
	sprintf(szTemp, "%d", bCheck);
	
	::WritePrivateProfileString("0000", "Scale", szTemp, strUserDir);

	if(pParent->m_nScreenID == nScreenID_6301)
	{
		int nLayOut = 0;
		if(m_btnLayout1.GetCheck())
			nLayOut = 1;
		else if(m_btnLayout2.GetCheck())
			nLayOut = 2;
		
		sprintf(szTemp, "%d", nLayOut);
		::WritePrivateProfileString("DWM", "Layout", szTemp, strUserDir);

		sprintf(szTemp, "%d", m_btnCodeSync.GetCheck());
		::WritePrivateProfileString("DWM", "CodeSync", szTemp, strUserDir);
	}
}

void CMiniBarDlg::OnChkType1() 
{
	m_btnLayout1.SetCheck(TRUE);
	m_btnLayout2.SetCheck(FALSE);

	((CChartMng*)m_pChartMng)->SetChartLayout(1);
	m_pChartMng->SendMessage(UMSG_SETMATRIX, 2, 2);			
}

void CMiniBarDlg::OnChkType2() 
{
	m_btnLayout1.SetCheck(FALSE);
	m_btnLayout2.SetCheck(TRUE);
	((CChartMng*)m_pChartMng)->SetChartLayout(2);
	m_pChartMng->SendMessage(UMSG_SETMATRIX, 2, 2);			
}

void CMiniBarDlg::OnChkCodesync() 
{
	BOOL bCheck = m_btnCodeSync.GetCheck();
	m_pChartMng->SendMessage(UMSG_SETJONGMOKLINK, bCheck);
}

void CMiniBarDlg::CreatedChartMng()
{
	CStdDialog* pParent = (CStdDialog*)GetParent();
	if(pParent->m_nScreenID == nScreenID_6301)
	{
		m_pChartMng->SendMessage(UMSG_SETMATRIX, 2, 2);
		((CChartMng*)m_pChartMng)->LoadDWMChart();
	}
	else if(pParent->m_nScreenID == nScreenID_6401)
		m_pChartMng->SendMessage(RMSG_SETQUERYOPT, 1);
}

void CMiniBarDlg::RecievedChartMng()
{
	BOOL bCheck = m_btnCodeSync.GetCheck();
	if(bCheck)
		((CChartMng*)m_pChartMng)->SetInLoop(TRUE);
}

void CMiniBarDlg::MoveCodeInputCtrl()
{
	if(m_btnMoveToLeftRight.IsWindowVisible())
	{
		CRect rectComp;
		m_btnMoveToLeftRight.GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		int nRight = rectComp.left - 1;
		
		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		
//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);

		//@Solomon-091026
		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
	}
}

void CMiniBarDlg::OnBtnRequest() 
{
	((CChartMng*)m_pChartMng)->SetMode(CHANGE_ITEM);
	((CChartMng*)m_pChartMng)->SetAddMode(FALSE);

	CChartItem* pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();
	pChartItem->SetCurrentRQ("DEFAULT");
	pChartItem->ClearData();

	((CChartMng*)m_pChartMng)->RequestChartData();
}

void CMiniBarDlg::OnBtinitial() 
{
	CString strMessage = "전체 차트의 환경이 초기화 됩니다. 계속 하시겠습니까?";
	if(MessageBox(strMessage, "차트 설정", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		//@Solomon-091120SM060
		if(GetMoveDirection() != 1)
		{
			SetMoveDirection(1);
			
			// MoveToLeft
			MoveToLeftRight(GetMoveDirection());
		}

 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 			m_pChartMng->SendMessage(UMSG_RESETCHART);
// 		
// 		//StdDialog에도 차트 초기화 추가
 		GetParent()->SendMessage(UMSG_RESETCHART);

	//	CStdDialog* pParent = (CStdDialog*)GetParent();
	//	if(pParent->m_nScreenID == nScreenID_6204)
		{
		//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		//		m_pChartMng->SendMessage(UMSG_RESETCHART);
			
			//StdDialog에도 차트 초기화 추가
	//		GetParent()->SendMessage(UMSG_RESETCHART);
	//		((CStdDialog*)GetParent())->DoClearUserChartData();
		} 
	//	else
	//	{
	//		m_chType = '4';
	//		//OnBtnMinSec();
	//		OnBtnDayWeekMonth();
	//	}

	//	Invalidate();
	}	
}

void CMiniBarDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
// 	if(nIDEvent==timer_CMiniBarDlg_CodeCync)
// 	{
// 		CStdDialog* pParent = (CStdDialog*)GetParent();
// 		int nLayOut = ::GetPrivateProfileInt("DWM", "CodeSync", 1, pParent->m_strConfig);
// 		m_btnCodeSync.SetCheck(nLayOut);
// 		OnChkCodesync();
// 		//m_pChartMng->SendMessage(UMSG_SETJONGMOKLINK, TRUE);
// 	}
	CInputBarDlg::OnTimer(nIDEvent);
}
