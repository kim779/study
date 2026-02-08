// MiniTickBarDlg.cpp: implementation of the CMiniTickBarDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniTickBarDlg.h"
#include "StdDialog.h"
#include "CodeDataFromInputBar.h"
#include "ChartMng.h"
#include "MiniTickBarCFG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DEF_SIZE 9
extern HINSTANCE g_hInstance;

CSetTickButton::CSetTickButton()
{
	m_pSetTick = new ST_SETTICK[DEF_SIZE];
}

CSetTickButton::~CSetTickButton()
{
	if(m_pSetTick)
		delete [] m_pSetTick;
}

CSetTickButton& CSetTickButton::operator = (CSetTickButton& right)
{
	for(int i = 0; i < 9; i++)
		m_pSetTick[i] = right.m_pSetTick[i];

	return *this;
}

void CSetTickButton::Init()
{
	m_pSetTick[0].strTitle = _T("1틱");
	m_pSetTick[0].nMarket = 0;
	m_pSetTick[0].nDate = 0;
	m_pSetTick[0].nTick = 1;
	m_pSetTick[0].bAllDay = TRUE;

	m_pSetTick[1].strTitle = _T("1분");
	m_pSetTick[1].nMarket = 0;
	m_pSetTick[1].nDate = 1;
	m_pSetTick[1].nTick = 1;
	m_pSetTick[1].bAllDay = TRUE;

	m_pSetTick[2].strTitle = _T("3분");
	m_pSetTick[2].nMarket = 0;
	m_pSetTick[2].nDate = 1;
	m_pSetTick[2].nTick = 3;
	m_pSetTick[2].bAllDay = TRUE;

	m_pSetTick[3].strTitle = _T("5분");
	m_pSetTick[3].nMarket = 0;
	m_pSetTick[3].nDate = 1;
	m_pSetTick[3].nTick = 5;
	m_pSetTick[3].bAllDay = TRUE;

	m_pSetTick[4].strTitle = _T("10분");
	m_pSetTick[4].nMarket = 0;
	m_pSetTick[4].nDate = 1;
	m_pSetTick[4].nTick = 10;
	m_pSetTick[4].bAllDay = TRUE;

	m_pSetTick[5].strTitle = _T("30분");
	m_pSetTick[5].nMarket = 0;
	m_pSetTick[5].nDate = 1;
	m_pSetTick[5].nTick = 30;
	m_pSetTick[5].bAllDay = TRUE;

	m_pSetTick[6].strTitle = _T("일");
	m_pSetTick[6].nMarket = 0;
	m_pSetTick[6].nDate = 2;
	m_pSetTick[6].nTick = 1;
	m_pSetTick[6].bAllDay = TRUE;

	m_pSetTick[7].strTitle = _T("선1");
	m_pSetTick[7].nMarket = 2;
	m_pSetTick[7].nDate = 0;
	m_pSetTick[7].nTick = 1;
	m_pSetTick[7].bAllDay = TRUE;

	m_pSetTick[8].strTitle = _T("종1");
	m_pSetTick[8].nMarket = 1;
	m_pSetTick[8].nDate = 0;
	m_pSetTick[8].nTick = 1;
	m_pSetTick[8].bAllDay = TRUE;
}

void CSetTickButton::SetButton(CILButton* m_pBtn, CImageList* pIL_W20H20)
{
	for(int i = 0; i < DEF_SIZE; i++)
		m_pBtn[i].FP_vSetImage( pIL_W20H20, 80, 82, TRUE, m_pSetTick[i].strTitle, TRUE);
}

CString CSetTickButton::GetTitle(int nIndex)
{
	return m_pSetTick[nIndex].strTitle;
}

int CSetTickButton::GetMarket(int nIndex)
{
	int nMarket = m_pSetTick[nIndex].nMarket;
	switch(nMarket)
	{
	case 0:
		return CMasterTypeInfo::STOCK_CHART;
	case 1:
		return CMasterTypeInfo::UPJONG_CHART;
	case 2:
		return CMasterTypeInfo::FUTOPT_CHART;
	}

	return CMasterTypeInfo::STOCK_CHART;
}

char CSetTickButton::GetDate(int nIndex)
{
	int nDate = m_pSetTick[nIndex].nDate;
	switch(nDate)
	{
	case 0:
		return TICK_DATA_CHART;
	case 1:
		return MIN_DATA_CHART;
	case 2:
		return DAY_DATA_CHART;
	case 3:
		return WEEK_DATA_CHART;
	case 4:
		return MONTH_DATA_CHART;
	case 5:
		return YEAR_DATA_CHART;
	}

	return '0';
}

int CSetTickButton::GetDateIndex(int nIndex)
{
	return m_pSetTick[nIndex].nDate;
}

int CSetTickButton::GetTick(int nIndex)
{
	return m_pSetTick[nIndex].nTick;
}

BOOL CSetTickButton::GetAllDay(int nIndex)
{
	return m_pSetTick[nIndex].bAllDay;
}

void CSetTickButton::SetTitle(int nIndex, CString strTitle)
{
	m_pSetTick[nIndex].strTitle = strTitle;
}

void CSetTickButton::SetMarket(int nIndex, int nMarket)
{
	m_pSetTick[nIndex].nMarket = nMarket;
}

void CSetTickButton::SetDateIndex(int nIndex, int nDate)
{
	m_pSetTick[nIndex].nDate = nDate;
}

void CSetTickButton::SetTick(int nIndex, int nTick)
{
	m_pSetTick[nIndex].nTick = nTick;
}

void CSetTickButton::SetAllDay(int nIndex, BOOL bAllDay)
{
	m_pSetTick[nIndex].bAllDay = bAllDay;
}

CString CSetTickButton::MakeTickButtonTitle(int nIndex)
{
	m_pSetTick[nIndex].strTitle.Empty();

	if(m_pSetTick[nIndex].nMarket == CMasterTypeInfo::UPJONG_CHART)
		m_pSetTick[nIndex].strTitle = _T("종");
	else if(m_pSetTick[nIndex].nMarket == CMasterTypeInfo::FUTOPT_CHART)
		m_pSetTick[nIndex].strTitle = _T("선");

	CString strTitle;
	switch(m_pSetTick[nIndex].nDate)
	{
	case 0:
		strTitle.Format(_T("%s%d"), m_pSetTick[nIndex].strTitle, m_pSetTick[nIndex].nTick);
		m_pSetTick[nIndex].strTitle = strTitle;

		if(m_pSetTick[nIndex].nMarket == CMasterTypeInfo::STOCK_CHART)
			strTitle = _T("틱");
		else
			strTitle.Empty();
		break;
	case 1:
		strTitle.Format(_T("%s%d"), m_pSetTick[nIndex].strTitle, m_pSetTick[nIndex].nTick);
		m_pSetTick[nIndex].strTitle = strTitle;

		if(m_pSetTick[nIndex].nMarket == CMasterTypeInfo::STOCK_CHART)
			strTitle = _T("분");
		else
			strTitle.Empty();
		break;
	case 2:
		strTitle = _T("일");
		break;
	case 3:
		strTitle = _T("주");
		break;
	case 4:
		strTitle = _T("월");
		break;
	}

	m_pSetTick[nIndex].strTitle += strTitle;
		
	return m_pSetTick[nIndex].strTitle;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniTickBarDlg::CMiniTickBarDlg(CWnd* pParent /*=NULL*/)
	: CInputBarDlg(pParent)
{
	//{{AFX_DATA_INIT(CMiniTickBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsMiniBar = TRUE;
	m_nMoveDirection = 1;
}

void CMiniTickBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX); // No CInputBarDlg!!
	//{{AFX_DATA_MAP(CMiniTickBarDlg)
	DDX_Control(pDX, IDC_BTINITIAL, m_btnInitail);
	DDX_Control(pDX, IDC_BTN_MOVETOLEFTRIGHT, m_btnMoveToLeftRight);
	DDX_Control(pDX, IDC_BTN_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_EDITCNTOFDATALL, m_editCntOfDatAll);
	DDX_Control(pDX, IDC_CBMARKET, m_cbMarket);
	DDX_Control(pDX, IDC_STJONGNAME, m_stJongName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BTN_01, m_btn[IDC_BTN_01 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_02, m_btn[IDC_BTN_02 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_03, m_btn[IDC_BTN_03 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_04, m_btn[IDC_BTN_04 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_05, m_btn[IDC_BTN_05 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_06, m_btn[IDC_BTN_06 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_07, m_btn[IDC_BTN_07 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_08, m_btn[IDC_BTN_08 - IDC_BTN_01]);
	DDX_Control(pDX, IDC_BTN_09, m_btn[IDC_BTN_09 - IDC_BTN_01]);
}

BEGIN_MESSAGE_MAP(CMiniTickBarDlg, CInputBarDlg)
	//{{AFX_MSG_MAP(CMiniTickBarDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG, OnBtnConfig)
	ON_BN_CLICKED(IDC_BTN_MOVETOLEFTRIGHT, OnBtnMovetoleftright)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTINITIAL, OnBtinitial)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_01, IDC_BTN_09, OnBtn)
END_MESSAGE_MAP()

void CMiniTickBarDlg::SetCodeName(int nMarketType, LPCSTR szCode, LPCSTR szName)
{
	if(m_nMarketType==nMarketType)
		m_stJongName.SetText(szName);
}

void CMiniTickBarDlg::Init()
{
	m_pCodeInput->SetUseFutureOption(true);	//@Solomon

	//m_stJongName.SetStaticColor(RGB(132,132,132), RGB(255,255,255) , RGB(0,0,0));
	{
		CRect rectComp;
		m_pCodeInput->GetWindowRect(&rectComp);
		ScreenToClient(rectComp);

		m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, 63, 20, SWP_NOZORDER);
	}
	
	CImageList* pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(pIL_W20H20 == NULL) return;

	m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
	m_btnMoveToLeftRight.SetCheck(0);
	m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 60, 62);
	
	m_SetTickBtn.Init();
	m_SetTickBtn.SetButton(*&m_btn, pIL_W20H20);

	m_btn[6].SetCheck(TRUE);
	
	m_btnConfig.FP_vSetImage( pIL_W20H20, 100, 102);
	
	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
	m_btnInitail.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "초기화", FALSE);
	m_btnInitail.FP_vSetToolTipText("전체차트초기화");

	LoadValue();

	if(((CStdDialog*)GetParent())->IsSimpleChart())
		SetAllPosition(1);
	else
	{
// 			CRect rectComp;
// 			m_btnMoveToLeftRight.GetWindowRect(rectComp);
// 			ScreenToClient(rectComp);
// 			int nRight = rectComp.left - 1;
// 			
// 			m_pCodeInput->GetWindowRect(rectComp);
// 			ScreenToClient(rectComp);
// 		
// 	//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
// 			//@Solomon-091026
// 			m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
	}

	m_editPeriod.SetLimitText(3);
	m_editCntOfDatAll.SetLimitText(3);
}

void CMiniTickBarDlg::LoadIniValue()
{
	CString strUserDir = m_pStdDialog->m_strConfig;

	char szTemp[20];
	char szKey[10];

	BOOL bCheck;
	int nMarket;
	int nDate;
	int nTick;
	BOOL bAllDay;

	for(int i = 0; i < 9; i++)
	{
		::memset(szKey, 0x00, sizeof(szKey));
		::sprintf(szKey, "TickBtn%d", i);

		::GetPrivateProfileString("0000", szKey, "", szTemp, sizeof(szTemp), strUserDir);
		if(!::lstrlen(szTemp))
			continue;

		::sscanf(szTemp, "%d;%d;%d;%d;%d", &bCheck, &nMarket, &nDate, &nTick, &bAllDay);

		m_btn[i].SetCheck(bCheck);
		m_SetTickBtn.SetMarket(i, nMarket);
		m_SetTickBtn.SetDateIndex(i, nDate);
		m_SetTickBtn.SetTick(i, nTick);
		m_SetTickBtn.SetAllDay(i, bAllDay);

		m_SetTickBtn.MakeTickButtonTitle(i);
	}

	for(int nPos=0;nPos<5;nPos++)
	{
		sprintf(szTemp,"%d",nPos);
		m_lCntInView[nPos] = ::GetPrivateProfileInt("CntInView",szTemp,m_lCntInView[nPos], strUserDir);
	}

	CImageList* pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(pIL_W20H20 == NULL) return;

	m_SetTickBtn.SetButton(*&m_btn, pIL_W20H20);
}

void CMiniTickBarDlg::SaveIniValue()
{
	CString strUserDir = m_pStdDialog->m_strConfig;
	
	char szTemp[20];
	char szKey[10];

	for(int i = 0; i < 9; i++)
	{
		::memset(szTemp, 0x00, sizeof(szTemp));
		::sprintf(szTemp, "%d;%d;%d;%d;%d", m_btn[i].GetCheck(), m_SetTickBtn.GetMarket(i), m_SetTickBtn.GetDateIndex(i), m_SetTickBtn.GetTick(i), m_SetTickBtn.GetAllDay(i));

		::memset(szKey, 0x00, sizeof(szKey));
		::sprintf(szKey, "TickBtn%d", i);

		::WritePrivateProfileString("0000", szKey, szTemp, strUserDir);
	}
}

void CMiniTickBarDlg::SetType(BOOL bSendChartMng/* = TRUE*/)
{
}

void CMiniTickBarDlg::OnBtn(UINT nID) 
{
	int nIndex = nID - IDC_BTN_01;

	for(int i = 0; i < DEF_SIZE; i++)
	{
		if(i == nIndex)
		{
			m_btn[i].SetCheck(TRUE);
			continue;
		}

		m_btn[i].SetCheck(FALSE);
	}

	m_chType = m_SetTickBtn.GetDate(nIndex);
	int nTick = m_SetTickBtn.GetTick(nIndex);

	int nMarket = m_SetTickBtn.GetMarket(nIndex);
	CString strCode = GetCurrentCode();
	if(nMarket != m_nMarketType)
	{
		CStringArray arrCode, arrName;

		SetMarketType(nMarket);
		((CStdDialog*)GetParent())->ChangeStockMode(nMarket);
		//((CChartMng*)m_pChartMng)->SetMarketType(nMarket);

		m_pDataManager->GetRecentCodeNameArray(nMarket, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			strCode = arrCode.GetAt(0);
		}

//@solomon	100104SM019	-->
		CImageList* pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
		if(nMarket == CMasterTypeInfo::UPJONG_CHART)
		{
			strCode = _T("001");
// 			::EnableWindow(m_pCodeInput->GetSafeHwnd(), FALSE);
// 			UINT nMoveDirection = GetMoveDirection();
// 			if(nMoveDirection!=2)
// 			{
// 				SetMoveDirection(2);
// 				MoveToLeftRight(2);
// 				m_btnMoveToLeftRight.EnableWindow(FALSE);
// 				m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 59, 59);
// 			}
		
		}
		else if(nMarket == CMasterTypeInfo::FUTOPT_CHART)
		{
			CString strFOCode;
			m_pDataManager->GetRecentCode(strCode, strFOCode, 10);
		//	::EnableWindow(m_pCodeInput->GetSafeHwnd(), FALSE);
			strCode = strFOCode;
// 			UINT nMoveDirection = GetMoveDirection();
// 			if(nMoveDirection!=2)
// 			{
// 				SetMoveDirection(2);
// 				MoveToLeftRight(2);
// 				m_btnMoveToLeftRight.EnableWindow(FALSE);
// 				m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 59, 59);
// 			}
		}
		else
		{
			m_pDataManager->GetRecentCodeNameArray(nMarket, arrCode, arrName);
			if(arrCode.GetSize()>0)
			{
				strCode = arrCode.GetAt(0);
			}
// 			UINT nMoveDirection = GetMoveDirection();
// 			if(nMoveDirection!=1)
// 			{
// 				SetMoveDirection(1);
// 				MoveToLeftRight(1);
// 				m_btnMoveToLeftRight.EnableWindow(TRUE);
// 				m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 60, 62);
// 			}
// 			::EnableWindow(m_pCodeInput->GetSafeHwnd(), TRUE);
		}
//@solomon	100104SM019	<--
	}

	BOOL bAllDay = m_SetTickBtn.GetAllDay(nIndex);
	m_pChartMng->SendMessage(RMSG_SETQUERYOPT, !bAllDay);

	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = strCode;
	CodeData.m_lEndTime = GetEndDate();
	CodeData.m_chType = GetType();
	CodeData.m_lTypeNum = nTick;
//	m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	//	20130518 이상신 : 주기변경으로 조회하는 것이기 때문에 종목공유를 사용하지 않음.
	m_pChartMng->SendMessage(UMSG_INPUTCODEFROMINPUTBAR, (WPARAM)&CodeData, (LPARAM)1);
//	m_pChartMng->SendMessage(UMSG_SETTYPE, (WPARAM)m_chType, (LPARAM)nTick);

}

void CMiniTickBarDlg::OnBtnConfig() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//HINSTANCE hInstance = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);

	CSetTickButton setTickBtn;
	setTickBtn = m_SetTickBtn;
	
	CMiniTickBarCFG	dlg(&setTickBtn, this);
	if(dlg.DoModal() == IDOK)
	{
		m_SetTickBtn = setTickBtn;

		CImageList* pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
		if(pIL_W20H20 == NULL) return;

		m_SetTickBtn.SetButton(*&m_btn, pIL_W20H20);

		Invalidate();

		for(int i = 0; i < DEF_SIZE; i++)
		{
			if(m_btn[i].GetCheck())
			{
				OnBtn(IDC_BTN_01 + i);
				break;
			}
		}
	}

	//AfxSetResourceHandle(hInstance);
}

void CMiniTickBarDlg::SetAllPosition(int nType)
{
	if(nType == 0)
		return;

	CWnd* pChild = GetWindow(GW_CHILD);
	CRect rectComp;
	while(pChild)
	{
//@Solomon 		if(m_hKSJongName == pChild->GetSafeHwnd())
//@Solomon 		{
//@Solomon 			pChild = pChild->GetNextWindow();
//@Solomon 			continue;
//@Solomon 		}

		pChild->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		pChild->SetWindowPos(NULL, rectComp.left - 45, rectComp.top, rectComp.Width(), 20, SWP_NOZORDER);

		if(pChild->GetDlgCtrlID() == IDC_CBMARKET)// || pChild->GetDlgCtrlID() == IDC_CTRL_CODEINPUT || pChild->GetDlgCtrlID() == IDC_BTCHARTMINI || pChild->GetDlgCtrlID() == IDC_BTINDEXMINI)
		{
			pChild->ShowWindow(SW_HIDE);
		}

		pChild = pChild->GetNextWindow();
	}

	CRect rcTemp;
	GetDlgItem(IDC_CBMARKET)->GetWindowRect(rcTemp);
	ScreenToClient(rcTemp);

	m_pCodeInput->GetWindowRect(rectComp);
	ScreenToClient(rectComp);
	m_pCodeInput->MoveWindow(rcTemp.left, rcTemp.top, rectComp.Width(), rectComp.Height());

	m_btnMoveToLeftRight.GetWindowRect(rectComp);
	ScreenToClient(rectComp);
	int nRight = rectComp.left - 1;


//@Solomon 	::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	//@Solomon-091026
	m_stJongName.SetWindowPos(NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
}

void CMiniTickBarDlg::MoveToLeftRight(UINT nMoveDirection)
{
	CImageList* pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	
	switch(nMoveDirection)
	{
	case 0:
	case 2:
		m_btnMoveToLeftRight.FP_vSetToolTipText("열기");
		m_btnMoveToLeftRight.SetCheck(1);
		m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 56, 58);

		if(m_cbMarket.GetSafeHwnd() && m_pCodeInput->GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			m_pCodeInput->GetWindowRect(&rcCodeInput);
			int nItemNameWidth = 0;
//@Solomon 			if(m_hKSJongName)
//@Solomon 			{
//@Solomon 				CRect rcItemName;
//@Solomon 				::GetWindowRect(m_hKSJongName, &rcItemName);
//@Solomon 				nItemNameWidth = rcItemName.Width();
//@Solomon 			}

			{
				CRect rcItemName;
				m_stJongName.GetWindowRect(&rcItemName);
				ScreenToClient(rcItemName);
				nItemNameWidth = rcItemName.right;
			}
			int cx = nItemNameWidth;
			//if(((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE2_CHART)
			//	cx = rcCodeInput.Width() + nItemNameWidth;
			//else
			//	cx = rcMarketCombo.Width() + rcCodeInput.Width() + nItemNameWidth;

			m_cbMarket.ShowWindow(SW_HIDE);
			m_pCodeInput->ShowWindow(SW_HIDE);
//@Solomon 			if(m_hKSJongName)
//@Solomon 				::ShowWindow(m_hKSJongName, SW_HIDE);

			UINT nCtrlId[] = {
				IDC_BTN_MOVETOLEFTRIGHT,   
				IDC_BTN_01,    
				IDC_BTN_02, 
				IDC_BTN_03,
				IDC_BTN_04,
				IDC_BTN_05,
				IDC_BTN_06,
				IDC_BTN_07,
				IDC_BTN_08,
				IDC_BTN_09,
				IDC_BTN_CONFIG,
				IDC_EDITCNTOFDATALL,
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
		break;
	case 1:
		m_btnMoveToLeftRight.FP_vSetToolTipText("닫기");
		m_btnMoveToLeftRight.SetCheck(0);
		m_btnMoveToLeftRight.FP_vSetImage( pIL_W20H20, 60, 62);


		if(m_cbMarket.GetSafeHwnd() && m_pCodeInput->GetSafeHwnd())
		{
			CRect rcMarketCombo, rcCodeInput;
			m_cbMarket.GetWindowRect(&rcMarketCombo);
			m_pCodeInput->GetWindowRect(&rcCodeInput);

			int nItemNameWidth = 0;
//@Solomon 			if(m_hKSJongName)
//@Solomon 			{
//@Solomon 				CRect rcItemName;
//@Solomon 				::GetWindowRect(m_hKSJongName, &rcItemName);
//@Solomon 				nItemNameWidth = rcItemName.Width();
//@Solomon 			}

			{
				CRect rcItemName;
				m_stJongName.GetWindowRect(&rcItemName);
				ScreenToClient(rcItemName);
				nItemNameWidth = rcItemName.right;
			}
			int cx = nItemNameWidth;
			//if(((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE2_CHART)
			//	cx = rcCodeInput.Width() + nItemNameWidth;
			//else
			//	cx = rcMarketCombo.Width() + rcCodeInput.Width() + nItemNameWidth;

			UINT nCtrlId[] = {
				IDC_BTN_MOVETOLEFTRIGHT,   
				IDC_BTN_01,    
				IDC_BTN_02, 
				IDC_BTN_03,
				IDC_BTN_04,
				IDC_BTN_05,
				IDC_BTN_06,
				IDC_BTN_07,
				IDC_BTN_08,
				IDC_BTN_09,
				IDC_BTN_CONFIG,
				IDC_EDITCNTOFDATALL,
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

			m_cbMarket.ShowWindow(SW_SHOW);
			m_pCodeInput->ShowWindow(SW_SHOW);
//@Solomon 			if(m_hKSJongName)
//@Solomon 				::ShowWindow(m_hKSJongName, SW_SHOW);
			m_stJongName.ShowWindow(SW_SHOW);
		}
		break;
	}

	Invalidate();
}

void CMiniTickBarDlg::OnBtnMovetoleftright() 
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

void CMiniTickBarDlg::OnSize(UINT nType, int cx, int cy) 
{
//	CInputBarDlg::OnSize(nType, cx, cy);
	
/*
	if(m_hKSJongName)
	{
		CRect rcItemName;
		::GetWindowRect(m_hKSJongName, &rcItemName);
		ScreenToClient(rcItemName);

		::SetParent(m_hKSJongName, m_hWnd);

		::ShowWindow(m_hKSJongName, SW_SHOW);
	}
*/
}

void CMiniTickBarDlg::MoveCodeInputCtrl()
{
	if(m_pCodeInput->GetSafeHwnd())
	{
		CRect rcTemp, rectComp;

		m_btnMoveToLeftRight.GetWindowRect(rcTemp);
		ScreenToClient(rcTemp);
		int nRight = rcTemp.left - 1;

		GetDlgItem(IDC_CBMARKET)->GetWindowRect(rcTemp);
		ScreenToClient(rcTemp);
		
		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		m_pCodeInput->MoveWindow(2, rcTemp.top, rectComp.Width(), rectComp.Height());
		
//		m_ctrlCodeInput.GetWindowRect(rcTemp);
//		ScreenToClient(rcTemp);

//@Solomon 		::SetWindowPos(m_hKSJongName, NULL, rectComp.Width() + 2, rectComp.top, nRight - rectComp.Width() - 2, rectComp.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
		//@Solomon-091026
		m_stJongName.SetWindowPos(NULL, rectComp.Width() + 2, rectComp.top, nRight - rectComp.Width() - 2, rectComp.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	}
}

void CMiniTickBarDlg::OnBtinitial() 
{
	CString strMessage = "전체 차트의 환경이 초기화 됩니다. 계속 하시겠습니까?";
	if(MessageBox(strMessage, "차트 설정", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		if(GetMoveDirection() != 1)
		{
			SetMoveDirection(1);

			// MoveToLeft
			MoveToLeftRight(GetMoveDirection());
		}

		m_SetTickBtn.Init();
		m_btn[6].SetCheck(TRUE);
		OnBtn(IDC_BTN_07);
		
		Invalidate();
//		if(((CStdDialog*)GetParent())->IsSimpleChart())
//			SetAllPosition(1);
//		else
//		{
//			CRect rectComp;
//			m_btnMoveToLeftRight.GetWindowRect(rectComp);
//			ScreenToClient(rectComp);
//			int nRight = rectComp.left - 1;
//			
//			m_ctrlCodeInput.GetWindowRect(rectComp);
//			ScreenToClient(rectComp);
//			
//			::SetWindowPos(m_hKSJongName, NULL, rectComp.right, rectComp.top, nRight - rectComp.right, rectComp.Height(), SWP_NOZORDER);
//		}
	}	
}
