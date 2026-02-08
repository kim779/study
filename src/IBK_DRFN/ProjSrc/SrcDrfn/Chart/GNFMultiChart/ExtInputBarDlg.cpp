// ExtInputBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExtInputBarDlg.h"
#include "StdDialog.h"
#include "./define/ChartMsgDef.h"
#include "../chart_common/BlockBaseEnum.h"
#include "../../inc/IAUGwanMng.h"
#include "../../../SrcSite/include/define.h"
#include "../../../SrcSite/include/axisfire.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtInputBarDlg dialog
//extern CWnd *g_pViewInMain;

const UINT RMSG_GETOPENEDOWNER = ::RegisterWindowMessage(_T("RMSG_GETOPENEDOWNER"));
const UINT RMSG_PRINTSCREEN = ::RegisterWindowMessage(_T("RMSG_PRINTSCREEN"));
//<< 20091214 by kmlee for account combo
const UINT RMSG_WRAP_GETINFO = ::RegisterWindowMessage(_T("RMSG_WRAP_GETINFO"));
//>>>

#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )

CExtInputBarDlg::CExtInputBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CExtInputBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtInputBarDlg)	
	m_ctlTime = 0;
	//}}AFX_DATA_INIT
	m_lRows = 1;
	m_lCols = 1;
	m_pChartMng = NULL;
	m_pLeftBar = NULL;
//	m_bRevised = TRUE;
	m_nSave = SAVE_TRUE;
//	m_bTimeShareUseage = TRUE;
//	m_bUpperLowPriceMarkUseage = TRUE;
//	m_bShowLock = TRUE;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_bNormalizedMinuteTimeRule = FALSE;
//	m_bUseNormalizedMinuteTimeRule = TRUE;

	m_nDragModeType = 0;

	m_nYUnitType = WON_TICK;

	m_strCfgFile = _T("");

	m_chType = DAY_DATA_CHART;

	m_nMarketType = CMasterTypeInfo::STOCK_CHART;

	m_bEndDate	= TRUE;
	m_strStartDate	= _T("");
	m_strEndDate	= _T("");

	//m_pCommonBaseInfo = NULL;
	
//@유진삭제 m_pIL_W58H20 = NULL;
//@유진삭제 m_pIL_W34H20 = NULL;
	m_pIL_W46H20 = NULL;
	m_pIL_W20H20 = NULL;
	m_pIL_W20H20_Tool = NULL;
//@유진삭제 m_pIL_W58H19 = NULL;
	m_pIL_W58H20 = NULL;

	m_bChkRevised = FALSE;
	m_nDataType = DAY_DATA_CHART;
	m_bChkPeriod = FALSE;
	m_bChkSaveJipyoOneFile = FALSE;
	// 분석툴 개별 저장 - ojtaso (20080723)
	m_bChkSaveAnalTool = TRUE;

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	m_bChkShareAnalTool	= FALSE;
	// 2011.01.12 by SYS <<

	m_bChkRegGwansimUp = FALSE;

	m_bChangeTool	= TRUE;
	m_nScrollState = SCROLL_STOP;

	m_bRealUpdate	= TRUE;
	m_nTimeIndex	= -1;
}


void CExtInputBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtInputBarDlg)
	DDX_Control(pDX, IDC_EDIT_PW, m_edAccPW);
	DDX_Control(pDX, IDC_CHK_ORDER2, m_chkOrderLine);
	DDX_Control(pDX, IDC_BTSCROLLPRE, m_btScrollPre);
	DDX_Control(pDX, IDC_BTSCROLLNEXT, m_btScrollNext);
	DDX_Control(pDX, IDC_BTSCROLLSTOP, m_btScrollStop);
	DDX_Control(pDX, IDC_BTN_GWANSIM, m_btnGwanSim);
//@유진삭제	DDX_Control(pDX, IDC_BTN_CHANGETOOL, m_btnChangeTool);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB1, m_btSub1);
	DDX_Control(pDX, IDC_BT_SUB2, m_btSub2);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB3, m_btSub3);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB4, m_btSub4);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB5, m_btSub5);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB6, m_btSub6);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB7, m_btSub7);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB8, m_btSub8);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB9, m_btSub9);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB10, m_btSub10);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB11, m_btSub11);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB12, m_btSub12);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB13, m_btSub13);
//@유진삭제	DDX_Control(pDX, IDC_BT_SUB14, m_btSub14);
	DDX_Control(pDX, IDC_BTETCMENU, m_btEtcMenu);
	DDX_Control(pDX, IDC_BTINITIAL, m_btInitial);
//@유진삭제 DDX_Control(pDX, IDC_BTINITIAL_PERIOD, m_btInitial_Period);
	DDX_Control(pDX, IDC_BTN_PERIOD, m_btnPeriod);
	DDX_Control(pDX, IDC_CHKUPDATEINDEX, m_chkUpdateIndex);
	DDX_Control(pDX, IDC_CHK_ENDSAVE, m_chkEndSave);
	DDX_Control(pDX, IDC_BTN_BUYSCR, m_btnBuyScreen);
	DDX_Control(pDX, IDC_BTN_SELLSCR, m_btnSellScreen);
	DDX_Control(pDX, IDC_BTN_SDATE, m_btnStartDate);
	DDX_Control(pDX, IDC_BTN_EDATE, m_btnEndDate);
	DDX_Control(pDX, IDC_BTFIXPERIOD, m_btFixPeriod);
	DDX_Control(pDX, IDC_BTST, m_btST);
	DDX_Control(pDX, IDC_BTHIST, m_btHist);
	DDX_Control(pDX, IDC_BTCHARTGUIDELINE, m_btChartGuideLine);
	DDX_Control(pDX, IDC_BTANALYSISPATTERN, m_btAnalysisPattern);
	DDX_Control(pDX, IDC_BTEXPREV, m_btExPrev);
	DDX_Control(pDX, IDC_BTEXNEXT, m_btExNext);
	DDX_Control(pDX, IDC_BTSHOWMATRIX, m_btShowMatrix);
	DDX_Control(pDX, IDC_BTSETENVIR, m_btSetEnvir);
	DDX_Control(pDX, IDC_CHKSHOWSCALE, m_chkShowScale);
	DDX_Control(pDX, IDC_CHKJONGMOKLINK, m_chkJongmokLink);
	DDX_Control(pDX, IDC_BTPRINT, m_btPrint);
	DDX_Control(pDX, IDC_BTLOAD, m_btLoad);
	DDX_Control(pDX, IDC_BTSAVE, m_btSave);
	DDX_Control(pDX, IDC_CHKTYPELINK, m_chkTypeLink);
	DDX_Control(pDX, IDC_BTINITSPLIT, m_btInitSplit);
	DDX_Control(pDX, IDC_STBY, m_stBy);
	DDX_Control(pDX, IDC_STSETMATRIX, m_stSetMatrix);
	DDX_Control(pDX, IDC_BTINDEX, m_btIndex);
	DDX_Control(pDX, IDC_CHKFIXPERIOD, m_chkFixPeriod);
	DDX_Check(pDX, IDC_CHK_REVISED, m_bChkRevised);
	DDX_Control(pDX, IDC_BTN_CHANGEUNIT, m_btYUnit);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datePicker);
//@유진추가
	DDX_Control(pDX, IDC_CMB_SCROLLSPEED, m_cmbScrollSpeed);
	DDX_Control(pDX, IDC_CHK_QUERYOPT, m_chkQueryOpt);
	DDX_Check(pDX, IDC_REAL_CHECK, m_chkRealUpdate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER, m_ctlTime);
	DDX_Control(pDX, IDC_BT_ORDER, m_btOrder);
//@유진추가
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtInputBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CExtInputBarDlg)
	ON_BN_CLICKED(IDC_BTSCROLLPRE, OnBtScrollPre)
	ON_BN_CLICKED(IDC_BTSCROLLNEXT, OnBtScrollNext)
	ON_BN_CLICKED(IDC_BTSCROLLSTOP, OnBtScrollStop)
//@유진추가
	ON_CBN_SELCHANGE(IDC_CMB_SCROLLSPEED, OnSelchangeCmbScrollSpeed)
//@유진추가
	ON_BN_CLICKED(IDC_BTINITSPLIT, OnBtinitsplit)
	ON_BN_CLICKED(IDC_BTINDEX, OnBtindex)
	ON_BN_CLICKED(IDC_CHKJONGMOKLINK, OnChkjongmoklink)
	ON_BN_CLICKED(IDC_CHKTYPELINK, OnChktypelink)
	ON_BN_CLICKED(IDC_BTLOAD, OnBtload)
	ON_BN_CLICKED(IDC_BTSAVE, OnBtsave)
	ON_BN_CLICKED(IDC_BTINITIAL, OnBtinitial)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHKSHOWSCALE, OnChkshowscale)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTPRINT, OnBtprint)
	ON_BN_CLICKED(IDC_BTSETENVIR, OnBtsetenvir)
	ON_BN_CLICKED(IDC_BTSHOWMATRIX, OnBtshowmatrix)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTEXNEXT, OnBtexnext)
	ON_BN_CLICKED(IDC_BTEXPREV, OnBtexprev)
	ON_BN_CLICKED(IDC_BTANALYSISPATTERN, OnBtanalysispattern)
	ON_BN_CLICKED(IDC_BTCHARTGUIDELINE, OnBtchartguideline)
	ON_BN_CLICKED(IDC_BTHIST, OnBthist)
	ON_BN_CLICKED(IDC_BTST, OnBtst)
	ON_BN_CLICKED(IDC_BTN_SDATE, OnBtnSdate)
	ON_BN_CLICKED(IDC_BTN_EDATE, OnBtnEdate)
//	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
//	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER1, OnCloseupDatetimepicker1)
//	ON_CBN_SELCHANGE(IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BTN_SELLSCR, OnBtnSellscr)
	ON_BN_CLICKED(IDC_BTN_BUYSCR, OnBtnBuyscr)
	ON_BN_CLICKED(IDC_BTFIXPERIOD, OnBtfixperiod)
	ON_BN_CLICKED(IDC_CHK_ENDSAVE, OnChkEndsave)
	ON_BN_CLICKED(IDC_CHKUPDATEINDEX, OnChkupdateindex)
	ON_BN_CLICKED(IDC_BTN_ANALIZE_PERIOD, OnBtnAnalizePeriod)
	ON_BN_CLICKED(IDC_CHK_REVISED, OnChkRevised)
	ON_BN_CLICKED(IDC_BTN_PERIOD, OnBtnPeriod)
	ON_BN_CLICKED(IDC_BTETCMENU, OnBtetcmenu)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB1, OnBtSub1)
	ON_BN_CLICKED(IDC_BT_SUB2, OnBtSub2)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB3, OnBtSub3)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB4, OnBtSub4)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB5, OnBtSub5)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB6, OnBtSub6)
	ON_BN_CLICKED(IDC_BTN_CHANGEUNIT, OnBtnChangeunit)
	ON_BN_CLICKED(IDC_CHKFIXPERIOD, OnChkfixperiod)
//@유진삭제 ON_BN_CLICKED(IDC_BTINITIAL_PERIOD, OnBtinitialPeriod)
//@유진삭제	ON_BN_CLICKED(IDC_BTN_CHANGETOOL, OnBtnChangeTool)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB8, OnBtSub8)
//@유진삭제	ON_BN_CLICKED(IDC_BT_SUB9, OnBtSub9)
	ON_BN_CLICKED(IDC_BTN_GWANSIM, OnBtnGwansim)
//@유진추가
	ON_BN_CLICKED(IDC_CHK_QUERYOPT, OnChkQueryOpt)
//@유진추가
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REAL_CHECK, OnChkRealUpdate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIMEPICKER, OnTimepickerchange)
	ON_BN_CLICKED(IDC_BT_ORDER, OnBtOrder)
	ON_BN_CLICKED (IDC_CHK_ORDER2, OnCheckOrderLink)
	ON_CBN_SELENDOK(IDC_CTRL_ACCOUNT, OnSelendokAccCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtInputBarDlg message handlers

BOOL CExtInputBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();

	// 스킨 적용
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);

	//001 2006.06.22{{
//@유진삭제 m_pIL_W58H20 = &(((CStdDialog*)GetParent())->m_ImgList_W58H20);
//@유진삭제 if(m_pIL_W58H20 == NULL) return TRUE;

	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
	if(m_pIL_W46H20 == NULL) return TRUE;

//@유진삭제 m_pIL_W58H19 = &(((CStdDialog*)GetParent())->m_ImgList_W58H19);
//@유진삭제 if(m_pIL_W58H19 == NULL) return TRUE;
	m_pIL_W58H20 = &(((CStdDialog*)GetParent())->m_ImgList_W58H20);
	if(m_pIL_W58H20 == NULL) return TRUE;

//@유진삭제 m_pIL_W34H20 = &(((CStdDialog*)GetParent())->m_ImgList_W34H20);
//@유진삭제 if(m_pIL_W34H20 == NULL) return TRUE;

//@유진삭제 CImageList* pIL_W46H19_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H19_Gwan);
	CImageList* pIL_W46H20_Gwan = &(((CStdDialog*)GetParent())->m_ImgList_W46H20_Gwan);

	m_btEtcMenu.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "보조기능", FALSE);
	m_btEtcMenu.FP_vSetToolTipText("보조기능");

	m_btInitial.FP_vSetImage(m_pIL_W46H20, 32, 34, TRUE, "초기화", FALSE);
	m_btInitial.FP_vSetToolTipText("전체차트초기화");

//@유진삭제 m_btInitial_Period.FP_vSetImage(m_pIL_W34H20, 0, 2, TRUE, "기간", FALSE);
//@유진삭제 m_btInitial_Period.FP_vSetToolTipText("기간");

	m_btnGwanSim.FP_vSetImage(pIL_W46H20_Gwan, 0, 2, FALSE, "", FALSE);
	m_btnGwanSim.FP_vSetToolTipText("관심등록");

	//m_btEtcMenu.InitButton(this, IDC_BTETCMENU);
	//m_btInitial.InitButton(this, IDC_BTINITIAL);
	//m_btInitial_Period.InitButton(this, IDC_BTINITIAL_PERIOD);
	//m_btEtcMenu.SetDataText("보조기능");
	//m_btInitial.SetDataText("전체");
	//m_btInitial_Period.SetDataText("기간");
	//001 2006.06.22 }}

	// TODO: Add extra initialization here
	//m_crBk = ((CStdDialog*)GetParent())->GetSkinColorManager()->GetColor(20);
//@유진삭제	m_crBk = RGB(237,243,249);	//RGB(222,222,222);
	m_brBkColor.CreateSolidBrush(m_crBk);

	m_strImageChartPath = ((CStdDialog*)GetParent())->m_szImageDir;
	//m_strCfgFile		= ((CStdDialog*)GetParent())->m_strUserDir + ENVINFOFILE;

	m_strCfgFile = ((CStdDialog*)GetParent())->m_strConfig;
	//	if(m_nMarketType == STOCK_CHART)	
	//	{	
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + STOCK_ENVINFOFILE;
	//		//m_btnSellScreen.ShowWindow(TRUE);
	//		//m_btnBuyScreen.ShowWindow(TRUE);
	//	}
	//	else if(m_nMarketType == FUTOPT_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + FUTOPT_ENVINFOFILE;
	//	else if(m_nMarketType == UPJONG_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + UPJONG_ENVINFOFILE;
	//	else if(m_nMarketType == FOREIGN_CHART)
	//		m_strCfgFile = ((CStdDialog*)GetParent())->m_strUserDir + FOREIGN_ENVINFOFILE;


	CString strImageSelect;
	CString strImageUp;
	CString strImageDown;
	CString strImageOver;
	CString strImageDis;


	m_pIL_W46H20 = &(((CStdDialog*)GetParent())->m_ImgList_W46H20);
	if(m_pIL_W46H20 == NULL) return TRUE;

	//지표
	m_btIndex.FP_vSetImage( m_pIL_W46H20, 2, 3);	
	m_btIndex.FP_vSetToolTipText("지표");

	//환경설정 	
	m_btSetEnvir.FP_vSetImage(m_pIL_W46H20, 24, 25);
	m_btSetEnvir.FP_vSetToolTipText("환경설정");

	//종목 동기화
	m_chkJongmokLink.FP_vSetImage(m_pIL_W46H20, 4, 6, FALSE, "", TRUE, 7);
	m_chkJongmokLink.FP_vSetToolTipText("종목연동지정");

	//시간 동기화 		
	m_chkTypeLink.FP_vSetImage(m_pIL_W46H20, 8, 10, FALSE, "", TRUE, 11);
	m_chkTypeLink.FP_vSetToolTipText("주기연동지정");

	//=환경 동기화 
	//m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14, FALSE, "", TRUE);
	//m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14);
	m_chkUpdateIndex.FP_vSetImage(m_pIL_W46H20, 12, 14, FALSE, "", FALSE, 15);
	m_chkUpdateIndex.FP_vSetToolTipText("차트지표동기화");	

	//기간 동기화 	
	m_btFixPeriod.FP_vSetImage(m_pIL_W46H20, 16, 18, FALSE, "", FALSE, 19);
	m_btFixPeriod.FP_vSetToolTipText("기간고정");


	m_pIL_W20H20 = &(((CStdDialog*)GetParent())->m_ImgList_W20H20);
	if(m_pIL_W20H20 == NULL) return TRUE;

	//Tool / 동기화버튼
//@유진삭제	m_btnChangeTool.FP_vSetImage(m_pIL_W20H20, 0, 8, FALSE, "T", TRUE);		
//@유진삭제	m_btnChangeTool.FP_vSetToolTipText("Tool");

	m_pIL_W20H20_Tool = &(((CStdDialog*)GetParent())->m_ImgList_W20H20_Tool);
	if(m_pIL_W20H20_Tool == NULL) return TRUE;
	//	m_btSub1.FP_vSetImage(m_pIL_W20H20_Tool, 0, 2, FALSE, "", TRUE);	
	//	m_btSub1.FP_vSetToolTipText("Line추세선");
	m_btSub2.FP_vSetImage(m_pIL_W20H20_Tool, 0, 2, FALSE, "", TRUE);		
	m_btSub2.FP_vSetToolTipText("십자선");
//@유진삭제	m_btSub3.FP_vSetImage(m_pIL_W20H20_Tool, 4, 6, FALSE, "", TRUE);		
//@유진삭제	m_btSub3.FP_vSetToolTipText("수평선");
	//	m_btSub4.FP_vSetImage(m_pIL_W20H20_Tool, 12, 14);		
	//	m_btSub4.FP_vSetToolTipText("전체지우개");
	//	m_btSub5.FP_vSetImage(m_pIL_W20H20_Tool, 20, 22);		
	//	m_btSub5.FP_vSetToolTipText("Y축반전");
	//m_btSub6.FP_vSetImage(m_pIL_W20H20_Tool, 24, 26, FALSE, "", TRUE);		
	//	m_btSub6.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub6.FP_vSetToolTipText("수치조회창");
	//	m_btSub7.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub7.FP_vSetToolTipText("돋보기");
//@유진삭제	m_btSub8.FP_vSetImage( m_pIL_W20H20_Tool, 8, 10, FALSE, "", TRUE);	
//@유진삭제	m_btSub8.FP_vSetToolTipText("수직선");
	//	m_btSub9.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub9.FP_vSetToolTipText("수치조회창");
	//	m_btSub10.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub10.FP_vSetToolTipText("지우개");
	//	m_btSub11.FP_vSetImage( m_pIL_W20H20, 4, 6, FALSE, "", TRUE);	
	//	m_btSub11.FP_vSetToolTipText("저장");
	//	m_btSub12.FP_vSetImage( m_pIL_W20H20, 12, 14, FALSE, "", TRUE);	
	//	m_btSub12.FP_vSetToolTipText("열기");
	//	m_btSub13.FP_vSetImage( m_pIL_W20H20, 20, 22, FALSE, "", TRUE);	
	//	m_btSub13.FP_vSetToolTipText("인쇄");
	//	m_btSub14.FP_vSetImage( m_pIL_W20H20, 96, 98, FALSE, "", TRUE);	
	//	m_btSub14.FP_vSetToolTipText("엑셀");

	//기간설정
	m_btnPeriod.FP_vSetImage( m_pIL_W20H20, 82, 83, TRUE, "P", FALSE);
	m_btnPeriod.FP_vSetToolTipText("기간설정");
	//	//눈금 전환 
	m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 14, FALSE, "\\", TRUE, 15);	
	m_btYUnit.FP_vSetToolTipText("Y축 눈금전환");
	m_nYUnitType = WON_TICK;

	//chart 분할 
	m_btShowMatrix.FP_vSetImage(m_pIL_W20H20, 52, 54);
	m_btShowMatrix.FP_vSetToolTipText("분할");

	//1*1
	m_btInitSplit.FP_vSetImage(m_pIL_W20H20, 72, 74);
	m_btInitSplit.FP_vSetToolTipText("분할복원");

	//스크롤이전	눌린상태 표시로 변경
	//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106, TRUE, "", FALSE);
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106, FALSE, "", TRUE);
	m_btScrollPre.FP_vSetToolTipText("이전");

	//스크롤 정지
//	m_btScrollStop.FP_vSetImage(m_pIL_W20H20, 130, 132);
	m_btScrollStop.FP_vSetImage(m_pIL_W20H20, 64, 66);
	m_btScrollStop.FP_vSetToolTipText("스크롤정지");

	//스크롤이후	
	//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70, TRUE, "", FALSE);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70, FALSE, "", TRUE);
	m_btScrollNext.FP_vSetToolTipText("이후");

	//이전	
	m_btExPrev.FP_vSetImage(m_pIL_W20H20, 60, 62);
	m_btExPrev.FP_vSetToolTipText("이전");

	//이후	
	m_btExNext.FP_vSetImage(m_pIL_W20H20, 56, 58);	
	m_btExNext.FP_vSetToolTipText("이후");

	//스케일 보여주기 	
	m_chkShowScale.FP_vSetImage(m_pIL_W20H20, 24, 26, FALSE, "", TRUE);
	m_chkShowScale.FP_vSetToolTipText("스케일보여주기");

	//차트저장
	m_btSave.FP_vSetImage(m_pIL_W20H20, 4, 6);
	m_btSave.FP_vSetToolTipText("차트저장");

	//저장차트불러오기
	m_btLoad.FP_vSetImage(m_pIL_W20H20, 12, 14);
	m_btLoad.FP_vSetToolTipText("저장차트불러오기");

	//인쇄	
	m_btPrint.FP_vSetImage(m_pIL_W20H20, 20, 22);
	m_btPrint.FP_vSetToolTipText("인쇄");

	//시작일 종료일
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "▶", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "◀", TRUE);

	//매도 매수 화면
	m_btnSellScreen.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "도");
	m_btnSellScreen.FP_vSetToolTipText("매도화면");

	m_btnBuyScreen.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE, "수");
	m_btnBuyScreen.FP_vSetToolTipText("매수화면");

	//기타 Addin 관련 버튼 
	m_btAnalysisPattern.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"P")	;	//m_btAnalysisPattern.FP_vSetToolTipText();
	m_btChartGuideLine.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"G")		;	//m_btChartGuideLine.FP_vSetToolTipText();
	m_btHist.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"C")				;	//m_btHist.FP_vSetToolTipText();
	m_btST.FP_vSetImage(m_pIL_W20H20, 80, 82, TRUE,"S")					;	//m_btST.FP_vSetToolTipText();

	m_btOrder.FP_vSetImage(m_pIL_W58H20, 0, 2, TRUE, "주문화면", FALSE);
	m_btOrder.FP_vSetToolTipText("주문화면");

	SetExt(0);
	char szTemp[64];

	//<<20100308_JS.Kim 솔로몬
	CRect rect;
	GetDlgItem(IDC_CTRL_ACCOUNT)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_CTRL_ACCOUNT)->DestroyWindow();

	m_pAccountCtrl = new CAccountCtrl;
//	m_pAccountCtrl->SetRootPath(((CStdDialog*)GetParent())->m_strRootPath);
	//m_pAccountCtrl->CreateAccountCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(), rect, IDC_CTRL_ACCOUNT, TRKEY_ACCCTRL1, TRKEY_ACCCTRL2, ACC_FOREIGN);
	m_pAccountCtrl->CreateAccountCtrl(this, GetParent()->GetParent()->GetParent()->GetParent(), rect, IDC_CTRL_ACCOUNT, ACC_REV_KEY1, ACC_REV_KEY2, ACC_FOREIGN);


	m_pAccountCtrl->Init();
	OnSelendokAccCtrl();	

/*
	UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);

	//nOpenedOwner  = 0 : 메뉴/툴바에서 열림
	//nOpenedOwner  = 1 : 마우스 오른쪽 버튼에서 열림 (종목코드 포함)
	//nOpenedOwner  = 2 : 스크립트,dll에서 화면 요청 (OpenMapData 포함)
	if(nOpenedOwner==0)
	{
		{
			::GetPrivateProfileString("ExtInputBar","Saved","1",szTemp,sizeof(szTemp),m_strCfgFile);
			if(szTemp[0]=='1')
			{
				m_nSave = SAVE_TRUE;
				m_chkEndSave.SetCheck(1);
			}
			else
			{
				m_nSave = SAVE_FALSE;
				m_chkEndSave.SetCheck(0);
			}
		}
	}
	else
	{
		m_nSave = SAVE_DISABLE;
	}
*/
	::GetPrivateProfileString("ExtInputBar","DragModeType","0",szTemp,sizeof(szTemp),m_strCfgFile);
	m_nDragModeType = atol(szTemp);

	//{{JS.Kim_20100903 Solomon 지표전체적용으로 하면 추세선이 저장이 안되는 오류가 있어 일단 메뉴에서 없앰
	/*
	::GetPrivateProfileString("ExtInputBar","SaveJipyoOneFile","1",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkSaveJipyoOneFile = TRUE;
	}
	else
	{
		m_bChkSaveJipyoOneFile = FALSE;
	}
	*/
	//}}

	::GetPrivateProfileString("ExtInputBar","RegGwansimUp","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkRegGwansimUp = TRUE;
	}
	else
	{
		m_bChkRegGwansimUp = FALSE;
	}

	// 분석툴 개별 저장 - ojtaso (20080723)
	::GetPrivateProfileString("ExtInputBar","SaveAnalTool","1",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
	{
		m_bChkSaveAnalTool = TRUE;
	}
	else
	{
		m_bChkSaveAnalTool = FALSE;
	}

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	// 분석툴 공유 기능
	::GetPrivateProfileString("ExtInputBar","ShareAnalTool","0",szTemp,sizeof(szTemp),m_strCfgFile);
	if(szTemp[0]=='1')
		m_bChkShareAnalTool = TRUE;
	else
		m_bChkShareAnalTool = FALSE;
	// 만약, 분석툴저장 기능이 체크상태가 아니면..  분석툴공유 기능도 체크해지 한다.
	if (!m_bChkSaveAnalTool)
		m_bChkShareAnalTool = FALSE;
	// 2011.01.12 by SYS <<

//@유진삭제
//	m_datePicker.InitCodeInput(this, IDC_DATETIMEPICKER1, 5000);
//@유진삭제
	CRect rectComp;
	GetDlgItem(IDC_DATETIMEPICKER1)->GetWindowRect(rectComp);
	ScreenToClient(rectComp);
	m_datePicker.SetWindowPos(NULL, rectComp.left, rectComp.top, rectComp.Width(), 19, SWP_NOZORDER);


	//>>JS.Kim_20101216
	m_cmbScrollSpeed.AddString("X  1");
	m_cmbScrollSpeed.AddString("X  2");
	m_cmbScrollSpeed.AddString("X  4");
	m_cmbScrollSpeed.AddString("X  8");
	//<<

//@유진추가
	m_cmbScrollSpeed.SetCurSel(0);
//@유진추가

// --> [Edit]  강지원 2008/10/31
	m_ctlTime = CTime::GetCurrentTime();
	m_chkRealUpdate = FALSE;
	UpdateData(FALSE);



////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  강지원 2008/11/09	( 임시로 막기 )
//	GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
//	m_datePicker.EnableWindow(FALSE);
//	GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
// <-- [Edit]  강지원 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//	OnChkRealUpdate();
// <-- [Edit]  강지원 2008/10/31

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CExtInputBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case UMSG_UPDATEMATRIX:
		{
			if(wParam*lParam>20)
			{
				MessageBox("차트의 수를 20개이하로 하여주십시오.",DLLTITLENAME);
			}
			else if(wParam!=m_lCols||lParam!=m_lRows)
			{
				m_lCols = wParam;
				m_lRows = lParam;	
				if(m_pChartMng->GetSafeHwnd())
					m_pChartMng->SendMessage(UMSG_SETMATRIX,m_lRows, m_lCols);			
			}
		}
		break;
	case UMSG_SETMATRIX:
		{
			m_lRows = wParam;
			m_lCols = lParam;
			CString strBy;
			strBy.Format("%2d X%2d",m_lCols,m_lRows);
			//2007.01.13 by LYH 1*1
			CString strOrgBy;
			m_stBy.GetWindowText(strOrgBy);
			if(strBy != strOrgBy)
			{
				if(m_lRows==1 && m_lCols==1 )
				{
					m_chkTypeLink.EnableWindow(FALSE);
					m_chkJongmokLink.EnableWindow(FALSE);
					m_btFixPeriod.EnableWindow(FALSE);
					m_chkUpdateIndex.EnableWindow(FALSE);
				}
				else
				{
					m_chkTypeLink.EnableWindow(TRUE);
					m_chkJongmokLink.EnableWindow(TRUE);
					m_btFixPeriod.EnableWindow(TRUE);
					m_chkUpdateIndex.EnableWindow(TRUE);
				}
			}
			m_stBy.SetWindowText(strBy);
		}
		break;
	case UMSG_CLEARLINK:
		{
			ClearLink();
		}
		break;
	case UMSG_SETJONGMOKLINK:
		{
			m_chkJongmokLink.SetCheck(wParam);
		}
		break;
	case UMSG_GETSAVED:
		{
			if(m_nSave==SAVE_TRUE)
			{
				return TRUE;
			}
			return FALSE;
		}
		break;
	case UMSG_GETJONGMOKLINK:
		{
			return (long)(m_chkJongmokLink.GetCheck());
		}
		break;
	case UMSG_GETSAVEJIPYOONEFILE:
		{
			if(wParam == 100)
				return (long)(m_bChkSaveAnalTool);
			else
				return (long)(m_bChkSaveJipyoOneFile);
		}
		break;

		// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	case UMSG_GET_ALALTOOL_SHAREFILE:
		{
			return (long)(m_bChkShareAnalTool);
		}
		break;
		// 2011.01.12 by SYS <<

	case UMSG_GETENVSINK:
		{
			return (long)0;
		}
		break;
	case UMSG_GETREVISED:
		{
			UpdateData(TRUE);
			return m_bChkRevised;
		}
		break;
	case UMSG_UPDATEREVISED:
		{
			m_bChkRevised = wParam;	
			UpdateData(FALSE);
			if(m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_bChkRevised,1);
		}
		break;
// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	case UMSG_SETNORMALIZEDMINUTETIMERULE:
//		{
//			m_bNormalizedMinuteTimeRule = wParam;
//		}
//		break;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	case UMSG_GETNORMALIZEDMINUTETIMERULE:
//		{
//			return m_bNormalizedMinuteTimeRule;
//		}
//		break;

	case UMSG_SETHIGHLIGHTINFO:
		{
			CString strComment = (LPCTSTR)wParam;
			m_stSetMatrix.SetWindowText(strComment);
		}
		break;
	case UMSG_GETDRAGMODETYPE:
		{
			return m_nDragModeType;
		}
		break;
	case UMSG_SETVIEWENDTIME:
		{
			if((m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART||m_chType==MONTH_DATA_CHART||m_chType==YEAR_DATA_CHART))
			{
				long lEndDate = wParam;
			/*	2005.09.06	
				//달력 컨트롤에 종료날짜를 바꿔주는 루틴이나ㅏ 
				//차트 OCX에 시작일 까지 요청은 무리라는 
				//답변에 따라 종료일만 바꾼다. */

				SYSTEMTIME tm;
				::ZeroMemory(&tm,sizeof(tm));
				if(m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART)
				{
					tm.wYear	= (WORD)lEndDate/10000;
					tm.wMonth	= (WORD)lEndDate%10000/100;
					tm.wDay		= (WORD)lEndDate%100;
				}
				else if(m_chType==MONTH_DATA_CHART)
				{
					tm.wYear	= (WORD)lEndDate/10000;
					tm.wMonth	= (WORD)lEndDate%10000/100;
					tm.wDay = 1;
				}

				/*
				if(m_bEndDate) // 종료일로 선택되어 있을 경우에만 달력 컨트롤에 값을 셋한다.
				{
					m_datePicker.SetTime(&tm);
				}
				*/
				//2005. 07. 28 added by sy, nam chart로 부터 받은 값을 계속 반영한다.
				m_strEndDate.Format("%d", lEndDate);				
			}
		}
		break;
	case UMSG_GET_TIME_FROM_DATETIMEPICKER:
		{
			/* ====================================
			//2005. 07. 28
			SYSTEMTIME* pTm;
			pTm = (SYSTEMTIME*)lParam;

			//차후 마지막 종료 시기변수에서 가져와야 한다. 05.06. 03
			m_datePicker.GetTime(pTm);
			*/
			return atoi(m_strEndDate);
		}
		break;
	case UMSG_GETENDDATEFROMINPUTBAR:
		{
			//SYSTEMTIME tm;		
			if((m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART||m_chType==MONTH_DATA_CHART||m_chType==YEAR_DATA_CHART))
			{
				//m_datePicker.GetTime(&tm);
				//return (long)(tm.wYear*10000+tm.wMonth*100+tm.wDay);
				return atoi(m_strEndDate);
			}
			return 0L;
		}
		break;
	case UMSG_SETINITIALCHARTDATA:
		{		
			m_bEndDate	= TRUE;
			m_strStartDate	= _T("");
			m_strEndDate	= _T("");

			stChartInitData* _pStData = (stChartInitData*)lParam;	
			m_nYUnitType = _pStData->m_nChartNumericValue;
			if(_pStData->m_nChartNumericValue == WON_TYPE)
			{				
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
			}
			else if(_pStData->m_nChartNumericValue == PERCENT_TYPE)
			{
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 20, 21, FALSE, "%", TRUE, 23);
			}
			else if(_pStData->m_nChartNumericValue == DOLLAR_TYPE)
			{
				m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 16, 17, FALSE, "$", TRUE, 19);
			}
			m_btYUnit.Invalidate();
			
			//2005. 08. 25 ================================================================
			//틱/분/일/주/월/년 형태 저장 05.08.31
			m_chType = _pStData->m_chType;

			//종목코드 05. 09. 05 
			//m_strCode = _pStData->m_strCode;

			// chart Type에 따른 Control 달력 Control 활성화 여부..
// --> [Edit]  강지원 2008/12/02	( 주기에 따라 분컨트롤 활성화 )
			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  강지원 2008/12/02
			BOOL bEnable = FALSE;
			if((_pStData->m_chType==DAY_DATA_CHART||_pStData->m_chType==WEEK_DATA_CHART||_pStData->m_chType==MONTH_DATA_CHART||_pStData->m_chType==YEAR_DATA_CHART||_pStData->m_chType==MIN_DATA_CHART))
			{
				bEnable = TRUE;
				GetDlgItem(IDC_REAL_CHECK)->EnableWindow(bEnable);
			}
			else
				GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);


			//m_datePicker.EnableWindow(bEnable);
//@유진삭제
//			if(m_datePicker.GetEnabled() == !bEnable)
//				m_datePicker.SetEnabled(bEnable);
//@유진삭제
//@유진추가

			//	분일경우 시간컨트롤을 활성화 시켜준다.
			if ( !m_bRealUpdate )
			{
				if ( _pStData->m_chType==MIN_DATA_CHART )
					GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
				else
					GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);

				if(m_datePicker.IsWindowEnabled() == !bEnable)
					m_datePicker.EnableWindow(bEnable);
			}
			else
			{
				m_datePicker.EnableWindow(FALSE);
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
			}

////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  강지원 2008/11/09	( 임시로 막기 )
//			m_datePicker.EnableWindow(FALSE);
//			GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
//			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  강지원 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//@유진추가
			m_btnStartDate.EnableWindow(bEnable);
			m_btnEndDate.EnableWindow(bEnable);

// --> [Edit]  강지원 2008/11/07	( 날짜 세팅 안함.. )
//			// 날짜 정보 저장
//			if(bEnable)
//			{		
//				m_strStartDate	.Format("%d", _pStData->m_lStartDate);
//				m_strEndDate	.Format("%d", _pStData->m_lEndDate);
//			
//				if(		(m_strStartDate	.GetLength() != 8) ||
//						(m_strEndDate	.GetLength() != 8) ||	
//						(atoi(m_strStartDate)<= 19700101 ) ||
//						(atoi(m_strEndDate	)<= 19700101 ) 
//				)
//				{
//					CTime currentTime = CTime::GetCurrentTime(); 
////@유진삭제 : m_datePicker 로딩안되게 막았으므로 핸들체크 넣음.
//					if(m_datePicker.GetSafeHwnd()) m_datePicker.SetTime(&currentTime);
//					m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
//				}
//
//				m_bEndDate = _pStData->m_bStartDateState ? FALSE : TRUE;
//				if( m_bEndDate )				
//				{
//					OnBtnEdate();
//				}
//				else 
//				{
//					OnBtnSdate();
//				}
//			}
// <-- [Edit]  강지원 2008/11/07

			//01.11 LYH================================================================
			//수정 주가 및 로그 보여주기 여부
			m_bChkRevised	= _pStData->m_bRevised;

			// (2008/9/23 - Seung-Won, Bae) Disable Sync Button for Empty Chart.
			if( _pStData->m_lEndDate == 0 && _pStData->m_lStartDate == 0)
			{
				m_chkTypeLink.EnableWindow(FALSE);
				m_chkJongmokLink.EnableWindow(FALSE);
				m_btFixPeriod.EnableWindow(FALSE);
				m_chkUpdateIndex.EnableWindow(FALSE);
			}
			else
			{
// --> [Edit]  강지원 2008/10/22
				if(m_lRows==1 && m_lCols==1 )
				{
					m_chkTypeLink.EnableWindow(FALSE);
					m_chkJongmokLink.EnableWindow(FALSE);
					m_btFixPeriod.EnableWindow(FALSE);
					m_chkUpdateIndex.EnableWindow(FALSE);
				}
				else
				{
					m_chkTypeLink.EnableWindow(TRUE);
					m_chkJongmokLink.EnableWindow(TRUE);
					m_btFixPeriod.EnableWindow(TRUE);
					// (2008/9/24 - Seung-Won, Bae) Disable Index Sync for Multi-Item Chart.
					m_chkUpdateIndex.EnableWindow( ( ( CChartMng *)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount() == 1);
				}
// <-- [Edit]  강지원 2008/10/22
			}

			m_edAccPW.GetWindowText(m_strPassWord);
			UpdateData(FALSE);			
			//=====================================================================
			//=============================================================================

		}
		break;
	case UMSG_GET_TICKMINDAY_VALUE:
		{
			ChangeStatusDateControl(lParam);
		}
		break;
	case UMSG_CHART_INIT_FROMOCX: //차트 초기화 처리.
		{
			if(wParam==1)
				ClearLink();
			CTime currentTime = CTime::GetCurrentTime(); 	
			m_datePicker.SetTime(&currentTime);	
			
			
			m_bEndDate = TRUE;

			//시작일 종료일
			m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "▶", TRUE);	
			m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "◀", TRUE);
			m_btnStartDate	.Invalidate();
			m_btnEndDate	.Invalidate();

			m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
		
			if(m_pChartMng && m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strEndDate));

			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->SetEndDateRequested(m_strEndDate);
		}
		break;
	case UMSG_SYNC_TOOLBAR:
		{
			if(wParam == CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL)
			{
				m_btSub2.SetCheck(lParam);
			}
			else if(wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT)
			{
				if(!lParam)
				{
					//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 126, 128);
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
					m_btScrollPre.Invalidate();
				}
				else
				{
					//m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 128, 126);
					m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 106, 104);
					m_btScrollPre.Invalidate();
				}
			}
			else if(wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
			{
				if(!lParam)
				{
					//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 134, 136);
					m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
					m_btScrollNext.Invalidate();
				}
				else
				{
					//m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 136, 134);
					m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 70, 68);
					m_btScrollNext.Invalidate();
				}
			}
		}
		break;
	case UWM_RECEIVE_ITRQUERY:
		{
			if(wParam == 2)
			{
				IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
				IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
				long lGroup = (long)lParam;
				CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
				CStringArray strCodes;
				strCodes.Add(strCode);
				// 4. 관심 매니저의 멤버함수 부재로 인한 에러 수정필요
//				pMng->AddGwansimtem_New(lGroup, strCodes, (long)m_hWnd);
			}
		}
		break;
// --> [Edit]  강지원 2008/10/15
	case UMSG_CHE_HOGA_VALUE:
		{
			
		}
		break;
	case WM_ENTERIDLE:
		{
			int n = 0;
		}
		break;
// <-- [Edit]  강지원 2008/10/15
	}

	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CExtInputBarDlg::ChangeButtonColor() 
{
//	CDrMLib_CBaseInfo2* pBaseInfo = NULL;
//	pBaseInfo = ((CStdDialog*)GetParent())->GetpMainBaseInfo();
//	if(pBaseInfo)
//	{
//		//	CClrButton m_btnFontSet;
//		//	m_baseInfo.GetButtonColor(m_btnFontSet.m_arrColor);
//		//	m_btnFontSet.ChangeSetColor();
//	}
}

void CExtInputBarDlg::OnBtinitsplit() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage(UMSG_SETONECHART);
	ClearLink();
}


//==============================================================================
// 상단 툴바의 차트 List의 내용과 중복되어 제거한다.
// 2005. 05. 26 by sy,nam
//==============================================================================
//DEL void CExtInputBarDlg::OnBtchart() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL     CMenu menu;
//DEL 	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//DEL     menu.CreatePopupMenu();
//DEL 
//DEL 	CStringArray strArrayIndicator;
//DEL 	if(m_pChartMng->GetSafeHwnd())
//DEL 		m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)"특수지표",(LPARAM)&strArrayIndicator);
//DEL 
//DEL 	CString strIndicator;
//DEL 	BOOL bExistance = FALSE;
//DEL 	long lSize = strArrayIndicator.GetSize();
//DEL 	UINT nFlags = NULL;
//DEL 	for(long lPos=0;lPos<lSize;lPos++)
//DEL 	{
//DEL 		strIndicator = strArrayIndicator.GetAt(lPos);
//DEL 		if(m_pChartMng->GetSafeHwnd())
//DEL 			bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
//DEL 		if(bExistance)
//DEL 		{
//DEL 			nFlags = MF_CHECKED;
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			nFlags = MF_UNCHECKED;
//DEL 		}
//DEL 		   
//DEL 		menu.AppendMenu(nFlags,100+lPos,strIndicator);
//DEL 	}
//DEL 	
//DEL 	CRect rect;
//DEL 	GetDlgItem(IDC_BTCHART)->GetWindowRect(rect);
//DEL 	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
//DEL 	if(nRetValue)
//DEL 	{
//DEL 		if(m_pChartMng->GetSafeHwnd())
//DEL 			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicator.GetAt(nRetValue-100));
//DEL 	}	
//DEL     menu.DestroyMenu();
//DEL }

void CExtInputBarDlg::OnBtindex() 
{
	// TODO: Add your control notification handler code here
//	char* szIndexGroupName[] = { "기본지표",
//								 "이동평균",
//								 "주가지표",
//								 "기타지표",
//								 "첨부지표",
//								 "추세지표",
//								 "거래량지표",
//								 "변동성지표",
//								 "탄력성지표"};

//-------------------------------------------------------------------
// Han Hwa에 맞게 지표 메뉴를 구성한다.
//-------------------------------------------------------------------
	char* szIndexGroupName[] = {					
								 //"전체지표",
								 "가격지표",
								 "추세지표",
								 "변동성지표",
								 "거래량지표",
								 "2차파생지표",
								 "기타지표",
								 "시장지표"
								 };
//-------------------------------------------------------------------
	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
	//=============================================================
//#ifdef _DEBUG
	// 05.10.24
	// 주식만 시장 지표 추가 
	//=============================================================
	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
		lAllCnt -= 1;
	//=============================================================
//#endif

    CMenu menu;
    menu.CreatePopupMenu();

	CStringArray strArrayIndicator;	
	CStringArray strArrayIndicatorAll;	
	CStringArray strArrayMarket; //시장 지표리스트 따로 관리.
	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	CMenu *pMenuChild = NULL;
	UINT nFlagsChild = NULL;
	BOOL bExistance = FALSE;
	strArrayIndicatorAll.RemoveAll();	
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		pMenuChild = new CMenu;
		pMenuChild->CreatePopupMenu();
		menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szIndexGroupName[lIndex]);

		if(m_pChartMng->GetSafeHwnd())
			m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)szIndexGroupName[lIndex],(LPARAM)&strArrayIndicator);
		lSize = strArrayIndicator.GetSize();
		nFlagsChild = NULL;
		bExistance = FALSE;

		for(long lPos=0;lPos<lSize;lPos++)
		{
			strIndicator = strArrayIndicator.GetAt(lPos);
			if(m_pChartMng->GetSafeHwnd())
				bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
			if(bExistance)
			{
				nFlagsChild = MF_CHECKED;
			}
			else
			{
				nFlagsChild = MF_UNCHECKED;
			}
			   
			pMenuChild->AppendMenu(nFlagsChild,100+lPosAll,strIndicator);
			lPosAll++;
		}
		strArrayIndicatorAll.Append(strArrayIndicator);

		//==========================================================
		//시장지표 항목은 별도로 따로 저장 한번 더 한다. 05.11. 01
		//메뉴에서 눌린게 시장지표라는 것을 알기위해 
		//==========================================================
		if(lIndex == lAllCnt - 1)
		{
			strArrayMarket.Append(strArrayIndicator);
		}
	}

//	CString sArrMarket = strArrayIndicatorAll.GetAt(lAllCnt); //시장지표 항목이 추가되어 있는 CStringArray를 가져온다.


	int nCntAddedByFavorite = 0;

	CFavoriteThingsMng* pFavoriteMng = NULL;

	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;
	if(pFavoriteMng)
	{
		pFavoriteMng->LoadGrpInfo();
		DWORD wSizeGrp = pFavoriteMng->m_strArrayGrp.GetSize();
		DWORD wSizeItem = 0;
		HTREEITEM  hRootItem = NULL;
		HTREEITEM  hItem = NULL;
		CString strGrpName;
		CString strItemName;
		DWORD wIndexGrp = 0;
		DWORD wIndexItem = 0;
		stFvrData _stFvrData;
		BOOL	bCreateRoot = FALSE;
		for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
		{
			bCreateRoot = FALSE;
			strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
			pFavoriteMng->LoadItemInfo(strGrpName);
			wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
			{
				_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
				{
					if(!nCntAddedByFavorite)
					{
						menu.AppendMenu(MF_SEPARATOR);
						nCntAddedByFavorite++;
					}
					bCreateRoot = TRUE;
					if(m_pChartMng->GetSafeHwnd())
						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
					if(bExistance)
					{
						nFlagsChild = MF_CHECKED;
					}
					else
					{
						nFlagsChild = MF_UNCHECKED;
					}
					menu.AppendMenu(nFlagsChild,100+lPosAll,_stFvrData.m_chData1);
					nCntAddedByFavorite++;
					lPosAll++;
					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
				}
			}		
		}
	}
	
	CRect rect;
	GetDlgItem(IDC_BTINDEX)->GetWindowRect(rect);
	UINT nFlags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlags, rect.left, rect.bottom, this);
	if(nRetValue)
	{	
		CString sJipyo = strArrayIndicatorAll.GetAt(nRetValue-100);

		//==============================================================================
		BOOL	bMarketJipyo = FALSE;
		for(int k = 0; k < strArrayMarket.GetSize(); k++)
		{
			if( sJipyo.CompareNoCase(strArrayMarket.GetAt(k)) == 0 )
			{
				bMarketJipyo = TRUE;
				break;
			}
		}
		//==============================================================================

		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)sJipyo, bMarketJipyo ? FROM_EXTINPUTBAR_MARKET : 0) ;
	}
	
	for(long lIndexOfMenu=0;lIndexOfMenu<(lAllCnt+nCntAddedByFavorite);lIndexOfMenu++)
	{
		pMenuChild = menu.GetSubMenu(lIndexOfMenu);
		if(pMenuChild)
		{
			pMenuChild->DestroyMenu();
			delete pMenuChild;
		}
	}
	menu.DestroyMenu();
}


//void CExtInputBarDlg::OnChkext1() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(1);
//}

//void CExtInputBarDlg::OnChkext2() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(2);
//}
//
//void CExtInputBarDlg::OnChkext3() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(3);	
//}
//
//void CExtInputBarDlg::OnChkext4() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(4);	
//}
//
//void CExtInputBarDlg::OnChkext5() 
//{
//	// TODO: Add your control notification handler code here
//	SetExt(5);
//}

void CExtInputBarDlg::SetExt(int nAct)
{
	EnableWindow(FALSE);
//	BOOL bExt1 = m_chkExt1.GetCheck();
//	BOOL bExt2 = m_chkExt2.GetCheck();
//	BOOL bExt3 = m_chkExt3.GetCheck();
//	BOOL bExt4 = m_chkExt4.GetCheck();
//	BOOL bExt5 = m_chkExt5.GetCheck();

	int nCmdShow1 = SW_SHOW;
	int nCmdShow2 = SW_SHOW;
	int nCmdShow3 = SW_SHOW;
	int nCmdShow4 = SW_SHOW;
	int nCmdShow5 = SW_SHOW;
//	if(bExt1)
//	{
//		m_chkExt1.SetToolTipText("버튼그룹열기");
//		nCmdShow1 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt1.SetToolTipText("버튼그룹닫기");
//	}
//	if(bExt2)
//	{
//		m_chkExt2.SetToolTipText("버튼그룹열기");
//		nCmdShow2 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt2.SetToolTipText("버튼그룹닫기");
//	}
//	if(bExt3)
//	{
//		m_chkExt3.SetToolTipText("버튼그룹열기");
//		nCmdShow3 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt3.SetToolTipText("버튼그룹닫기");
//	}
//	if(bExt4)
//	{
//		m_chkExt4.SetToolTipText("버튼그룹열기");
//		nCmdShow4 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt4.SetToolTipText("버튼그룹닫기");
//	}
//	if(bExt5)
//	{
//		m_chkExt5.SetToolTipText("버튼그룹열기");
//		nCmdShow5 = SW_HIDE;
//	}
//	else
//	{
//		m_chkExt5.SetToolTipText("버튼그룹닫기");
//	}
	m_btSave.ShowWindow(nCmdShow1);
	m_btLoad.ShowWindow(nCmdShow1);
	m_btPrint.ShowWindow(nCmdShow1);

//	m_btChart.ShowWindow(nCmdShow2);
	m_btIndex.ShowWindow(nCmdShow2);
//	m_btAddIndex.ShowWindow(nCmdShow2);
//	m_btSetEnvir.ShowWindow(nCmdShow2);
//	m_btAnalysisType.ShowWindow(nCmdShow2);
//	m_btEtcMenu.ShowWindow(nCmdShow2);

	//S01 2006.09.07 {{
//@유진삭제		m_btnChangeTool.ShowWindow(nCmdShow3);
	//	m_stBy.ShowWindow(nCmdShow3);			// 1 X 1
	//	m_btShowMatrix.ShowWindow(nCmdShow3);	// 분할
	//	m_btInitSplit.ShowWindow(nCmdShow3);	// 분할복원
	m_stSetMatrix.ShowWindow(nCmdShow3);		
	m_btExPrev.ShowWindow(nCmdShow3);
	m_btExNext.ShowWindow(nCmdShow3);
	//	m_chkJongmokLink.ShowWindow(nCmdShow4);		// 종목연동
	//	m_chkTypeLink.ShowWindow(nCmdShow4);		// 주기연동
	//	m_chkUpdateIndex.ShowWindow(nCmdShow4);		// 지표연동

	m_btInitial.ShowWindow(nCmdShow5);
	m_chkShowScale.ShowWindow(nCmdShow5);
	m_chkFixPeriod.ShowWindow(nCmdShow5);		// 기간고정
	//S01 2006.09.07 }}

// for Pattern Test - SeoJeong - 
#ifdef _DEBUG
//	m_btAnalysisPattern.ShowWindow(nCmdShow5);
//	m_btChartGuideLine.ShowWindow(nCmdShow5);
//	GetDlgItem(IDC_BTN_ANALIZE_PERIOD)->ShowWindow(nCmdShow5);
#else
//	m_btAnalysisPattern.ShowWindow(SW_HIDE);
//	m_btAnalysisPattern.ShowWindow(nCmdShow5);
//	m_btChartGuideLine.ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_BTN_ODRTRACE)->ShowWindow(SW_HIDE);
#endif

	
	m_btHist.ShowWindow(nCmdShow5);
	m_btST.ShowWindow(nCmdShow5);


	CRect rect;
	int nShiftExt12 = 63;//63;
	int nShiftExt23 = 230;//210;//188;//142;//116;
	int nShiftExt34 = 208;//159;//117;//133;
	int nShiftExt45 = 140;//64;
	if(nAct==1)		
	{
//		if(bExt1)
//		{
//			nShiftExt12 *= -1;
//		}
//		SHIFTCONTROL(m_chkExt2,nShiftExt12,rect);
//		SHIFTCONTROL(m_btChart,nShiftExt12,rect);
		SHIFTCONTROL(m_btIndex,nShiftExt12,rect);
		SHIFTCONTROL(m_btAddIndex,nShiftExt12,rect);
//		SHIFTCONTROL(m_btSetEnvir,nShiftExt12,rect);
//		SHIFTCONTROL(m_btAnalysisType,nShiftExt12,rect);
//		SHIFTCONTROL(m_btEtcMenu,nShiftExt12,rect);

//		SHIFTCONTROL(m_chkExt3,nShiftExt12,rect);
		SHIFTCONTROL(m_stSetMatrix,nShiftExt12,rect);
		SHIFTCONTROL(m_stBy,nShiftExt12,rect);
		SHIFTCONTROL(m_btShowMatrix,nShiftExt12,rect);
		SHIFTCONTROL(m_btExPrev,nShiftExt12,rect);
		SHIFTCONTROL(m_btExNext,nShiftExt12,rect);
		SHIFTCONTROL(m_btInitSplit,nShiftExt12,rect);

//		SHIFTCONTROL(m_chkExt4,nShiftExt12,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt12,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt12,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt12,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt12,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt12,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt12,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt12,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt12,rect);
		SHIFTCONTROL(m_btHist,nShiftExt12,rect);
		SHIFTCONTROL(m_btST,nShiftExt12,rect);
	}
	else if(nAct==2)
	{
//		if(bExt2)
//		{
//			nShiftExt23 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt3,nShiftExt23,rect);
		SHIFTCONTROL(m_stSetMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_stBy,nShiftExt23,rect);
		SHIFTCONTROL(m_btShowMatrix,nShiftExt23,rect);
		SHIFTCONTROL(m_btExPrev,nShiftExt23,rect);
		SHIFTCONTROL(m_btExNext,nShiftExt23,rect);
		SHIFTCONTROL(m_btInitSplit,nShiftExt23,rect);

//		SHIFTCONTROL(m_chkExt4,nShiftExt23,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt23,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt23,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt23,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt23,rect);

//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt23,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt23,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt23,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt23,rect);
		SHIFTCONTROL(m_btHist,nShiftExt23,rect);
		SHIFTCONTROL(m_btST,nShiftExt23,rect);

	}
	else if(nAct==3)
	{
//		if(bExt3)
//		{
//			nShiftExt34 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt4,nShiftExt34,rect);
		SHIFTCONTROL(m_chkJongmokLink,nShiftExt34,rect);
		SHIFTCONTROL(m_chkTypeLink,nShiftExt34,rect);
		SHIFTCONTROL(m_chkUpdateIndex,nShiftExt34,rect);
		
//		SHIFTCONTROL(m_chkExt5,nShiftExt34,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt34,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt34,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt34,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt34,rect);
		SHIFTCONTROL(m_btHist,nShiftExt34,rect);
		SHIFTCONTROL(m_btST,nShiftExt34,rect);
	}
	else if(nAct==4)
	{
//		if(bExt4)
//		{
//			nShiftExt45 *= -1;
//		}
//
//		SHIFTCONTROL(m_chkExt5,nShiftExt45,rect);
//001 2006.06.22		SHIFTCONTROL(m_btInitial,nShiftExt45,rect);
		SHIFTCONTROL(m_chkShowScale,nShiftExt45,rect);
		SHIFTCONTROL(m_btAnalysisPattern,nShiftExt45,rect);
		SHIFTCONTROL(m_btChartGuideLine,nShiftExt45,rect);
		SHIFTCONTROL(m_btHist,nShiftExt45,rect);
		SHIFTCONTROL(m_btST,nShiftExt45,rect);

	}
	Invalidate();
	EnableWindow();
}

void CExtInputBarDlg::OnChkjongmoklink() 
{
	// TODO: Add your control notification handler code here
	if(m_chkJongmokLink.GetCheck())
	{
/*		m_chkJongmokLink.SetToolTipText("종목연동해제");
		m_chkTypeLink.SetCheck(0);
		m_chkTypeLink.SetToolTipText("시간연동지정");
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkTypeLink.GetCheck());
*/
	}
	else
	{
//		m_chkJongmokLink.SetToolTipText("종목연동지정");
	}	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETJONGMOKLINK,(WPARAM)m_chkJongmokLink.GetCheck());
}

void CExtInputBarDlg::OnChktypelink() 
{
	// TODO: Add your control notification handler code here
/*	if(m_chkTypeLink.GetCheck())
	{
		m_chkTypeLink.SetToolTipText("시간연동해제");
		m_chkJongmokLink.SetCheck(0);
		m_chkJongmokLink.SetToolTipText("종목연동지정");
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkJongmokLink.GetCheck());
	}
	else
	{
		m_chkTypeLink.SetToolTipText("시간연동지정");
	}
*/	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETTYPELINK,(WPARAM)m_chkTypeLink.GetCheck());	
}

void CExtInputBarDlg::OnBtload() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTLOAD)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.bottom;

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_LOADCHART,0,(LPARAM)&pt);	
}

void CExtInputBarDlg::OnBtsave() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTSAVE)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.bottom;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SAVECHART,0,(LPARAM)&pt);	
}


void CExtInputBarDlg::OnBtinitial() 
{
	m_bEndDate	= TRUE;
	m_strStartDate	= _T("");
	m_strEndDate	= _T("");
	
	// TODO: Add your control notification handler code here
	CString strMessage = "전체 차트의 환경이 초기화 됩니다. 계속 하시겠습니까?";
	if(MessageBox(strMessage, "차트 설정", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
			m_pChartMng->SendMessage(UMSG_RESETCHART);

		//StdDialog에도 차트 초기화 추가
		GetParent()->SendMessage(UMSG_RESETCHART);
	}	
}

//void CExtInputBarDlg::OnBtaddindex() 
//{
//	// TODO: Add your control notification handler code here
//	m_pChartMng->SendMessage(UMSG_SHOWINDICATORCONFIGDLG);
//}

void CExtInputBarDlg::OnDestroy() 
{	
	// TODO: Add your message handler code here
	if(m_MatrixSelecter.GetSafeHwnd() != NULL)
		m_MatrixSelecter.DestroyWindow();
	m_brBkColor.DeleteObject();
	
	if(((CStdDialog*)GetParent())->m_strConfig == m_strCfgFile)
	{
		CString strTemp;
/*
		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		//nOpenedOwner  = 0 : 메뉴/툴바에서 열림
		//nOpenedOwner  = 1 : 마우스 오른쪽 버튼에서 열림 (종목코드 포함)
		//nOpenedOwner  = 2 : 스크립트,dll에서 화면 요청 (OpenMapData 포함)
		if(nOpenedOwner==0&&m_nSave)
		{
			if(m_nSave==SAVE_TRUE)
			{
				strTemp = "1";
			}
			else if(m_nSave==SAVE_FALSE)
			{
				strTemp = "0";
			}
			::WritePrivateProfileString("ExtInputBar","Saved",strTemp,m_strCfgFile);
		}
*/
		strTemp.Format("%d", m_nDragModeType);
		::WritePrivateProfileString("ExtInputBar","DragModeType",strTemp,m_strCfgFile);

		strTemp.Format("%d", m_bChkSaveJipyoOneFile);
		::WritePrivateProfileString("ExtInputBar","SaveJipyoOneFile",strTemp,m_strCfgFile);	

		strTemp.Format("%d", m_bChkRegGwansimUp);
		::WritePrivateProfileString("ExtInputBar","RegGwansimUp",strTemp,m_strCfgFile);	

		// 분석툴 개별 저장 - ojtaso (20080723)
		strTemp.Format("%d", m_bChkSaveAnalTool);
		::WritePrivateProfileString("ExtInputBar","SaveAnalTool",strTemp,m_strCfgFile);	

		// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
		strTemp.Format("%d", m_bChkShareAnalTool);
		::WritePrivateProfileString("ExtInputBar","ShareAnalTool",strTemp,m_strCfgFile);	
		// 2011.01.12 by SYS 
	}

	CRscDlg::OnDestroy();	
}

void CExtInputBarDlg::OnChkshowscale() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWSCALE,m_chkShowScale.GetCheck());
}

/*
void CExtInputBarDlg::OnBtupdateindex() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_UPDATEINDEX,m_chkShowScale.GetCheck());
}*/

HBRUSH CExtInputBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
		return m_brBkColor;
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CExtInputBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch(pMsg->wParam) 
		{
		case VK_RETURN :
			{
// --> [Edit]  강지원 2008/11/02		( 시간컨트롤에서 엔터키 입력시 조회를 해준다. )
				if ( pMsg->hwnd == GetDlgItem(IDC_TIMEPICKER)->GetSafeHwnd() )
				{
//					GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
					((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();
				}
// <-- [Edit]  강지원 2008/11/02
				else if(m_edAccPW.GetSafeHwnd()==pMsg->hwnd)
				{
					m_edAccPW.GetWindowText(m_strPassWord);
					SendAccountToPlatform(m_strAccountNo, m_strPassWord);

					BOOL bOrderLine = m_chkOrderLine.GetCheck();
					if( bOrderLine )
					{
						((CStdDialog*)GetParent())->SetOrderLineInfo(bOrderLine);
						if(m_pChartMng->GetSafeHwnd())
							m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)bOrderLine);	
					}
				}

				return FALSE;
			}
			break;
		case VK_ESCAPE :
			{
				return FALSE;			
			}
			break;
		}		
		break;
	}
	if(pMsg->wParam == VK_HOME)
		return FALSE;
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);	
	
}

/*
void CExtInputBarDlg::OnChkrevised() 
{
	// TODO: Add your control notification handler code here
	m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_chkRevised.GetCheck());
}
*/

void CExtInputBarDlg::OnBtprint() 
{	
	::SendMessage(((CStdDialog*)GetParent())->m_hMainFrame,WM_COMMAND, (WPARAM)ID_FILE_PRINT, 0);
}

void CExtInputBarDlg::OnBtsetenvir() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SHOWENVIRDLG);	
}

void CExtInputBarDlg::OnBtshowmatrix() 
{
	// TODO: Add your control notification handler code here
// --> [Edit]  강지원 2008/10/15
	if(m_pChartMng->GetSafeHwnd())
	{
		short nType = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCheHogaChange();
		if ( nType != 0 )
		{
			MessageBox("호가 차트일경우 1 X 1 만 지원합니다.");
//			AfxMessageBox("호가 차트일경우 1 X 1 만 지원합니다.");
			return;
		}
	}
// <-- [Edit]  강지원 2008/10/15

	if(m_MatrixSelecter.GetSafeHwnd() == NULL)
	{
		m_MatrixSelecter.Create(CMatrixSelecterDlg::IDD,GetParent());
		m_MatrixSelecter.SetpFrameDlg(this);
		m_MatrixSelecter.SetBesideWnd(&m_btShowMatrix);
		m_MatrixSelecter.SetMatrix(4,3);
	}
	m_MatrixSelecter.ShowMatrix();
}

void CExtInputBarDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_MatrixSelecter.GetSafeHwnd() != NULL)
		m_MatrixSelecter.ShowWindow(SW_HIDE);	
	CRscDlg::OnLButtonDown(nFlags, point);
}

void CExtInputBarDlg::OnBtexnext() 
{
	// TODO: Add your control notification handler code here
	((CStdDialog*)GetParent())->SetJongListNext();	
}

void CExtInputBarDlg::OnBtexprev() 
{
	// TODO: Add your control notification handler code here
	((CStdDialog*)GetParent())->SetJongListPrev();	
}

void CExtInputBarDlg::OnBtanalysispattern() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_ANALYSISPATTERN);
}

void CExtInputBarDlg::OnBtchartguideline() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_CHARTGUIDELINE);	
}
/*
void CExtInputBarDlg::OnBtanalysisType() 
{
	// TODO: Add your control notification handler code here
    CMenu menu;
	char* szEtcMenuName[] = {	"확대/축소",
								"종목특징분석",
								"십자선표시"							
								};
	long lAllCnt = sizeof(szEtcMenuName)/sizeof(char*);
	
	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
    menu.CreatePopupMenu();
	UINT nFlags = NULL;
	
	for(int nPos = 0;nPos < lAllCnt;nPos++)
	{
		if(m_nDragModeType== nPos)
		{
			nFlags = MF_CHECKED;
		}
		else
		{
			nFlags = MF_UNCHECKED;
		}
		menu.AppendMenu(nFlags,100 + nPos,szEtcMenuName[nPos]);
	}

	CRect rect;
	m_btAnalysisType.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
    menu.DestroyMenu();	
	if(!nRetValue) return;
	m_nDragModeType = nRetValue%100;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETDRAGMODETYPE,m_nDragModeType);	
}*/

void CExtInputBarDlg::OnBthist() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_SETORDEREDINFO);	
	
}

void CExtInputBarDlg::OnBtst() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_LOADSYSTEMTRADING);
}

//DEL void CExtInputBarDlg::OnChkratevalue() 
//DEL {
//DEL 	BOOL bCheckRateValue = m_chkRateValue.GetCheck();
//DEL 	if(bCheckRateValue)
//DEL 	{
//DEL 		m_chkRateValue.SetWindowText("%");
//DEL 	}
//DEL 	else 
//DEL 	{
//DEL 		m_chkRateValue.SetWindowText("\\");
//DEL 	}	
//DEL 	m_pChartMng->SendMessage(UMSG_RATEVALUE,bCheckRateValue);
//DEL }

void CExtInputBarDlg::OnBtnChangeunit() 
{
	if( m_nYUnitType == WON_TICK)
	{
		m_nYUnitType = PERCENT_TICK;
		m_btYUnit.SetWindowText("%");
	}
	else if(m_nYUnitType == DOLLAR_TICK)
	{
		m_nYUnitType = WON_TICK;
		m_btYUnit.SetWindowText("\\");
	}
	else if(m_nYUnitType == PERCENT_TICK)
	{
		m_nYUnitType = DOLLAR_TICK;
		m_btYUnit.SetWindowText("$");
	}

	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_RATEVALUE, m_nYUnitType);	
}

void CExtInputBarDlg::OnBtnSdate() 
{	
// --> [Edit]  강지원 2008/11/06		//	시작 일자를 세팅하지 않는다.
	return;
// <-- [Edit]  강지원 2008/11/06

	m_bEndDate = FALSE;

//	CString strImageUp		= m_strImageChartPath + "\\btn_up.bmp";
//	CString strImageDown	= m_strImageChartPath + "\\btn_down.bmp";		
//	m_btnStartDate	.SetSkin(strImageDown,strImageDown,"","","","",0,0,0);
//	m_btnEndDate	.SetSkin(strImageUp,strImageUp,"","","","",0,0,0);

	//시작일 종료일
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 90, 90, TRUE, "▶", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "◀", TRUE);

	m_btnStartDate	.Invalidate();
	m_btnEndDate	.Invalidate();


	//================================================================================
	// 시작일 종료일을 가져온다. 05.09.06
	//================================================================================
	double dStartDate = 0, dEndDate = 0;
	
	((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

	if(dStartDate)
		m_strStartDate.Format("%d", (long)dStartDate);
	if(dEndDate)
		m_strEndDate.Format("%d", (long)dEndDate);
	//================================================================================

	

	if(!m_strStartDate.GetLength())
	{
		CTime currentTime = CTime::GetCurrentTime(); 	
		//m_datePicker.SetTime(&currentTime);	
//@유진삭제
//		CString strCurTime = currentTime.Format("%Y%m%d");
//		m_datePicker.SetDataText(strCurTime);
//@유진삭제
//@유진추가
		m_datePicker.SetTime(&currentTime);
//@유진추가
		return;
	}
	
	m_strStartDate.TrimRight();

	if(m_chType == TICK_DATA_CHART) return;
	if(m_chType == YEAR_DATA_CHART) return;
	
	int nYear	= atoi(m_strStartDate.Left(4)	);	
	int nMonth	= atoi(m_strStartDate.Mid(4,2)	);
	int nDay	= atoi(m_strStartDate.Right(2)	);	


	CTime myTime(nYear,nMonth, nDay, 0, 0, 0);      		
	//m_datePicker.SetTime(&myTime); 
//@유진삭제
//	m_datePicker.SetDataText(m_strStartDate);
//@유진삭제
//@유진추가
	m_datePicker.SetTime(&myTime);
//@유진추가

	if(m_pChartMng && m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strStartDate));
}

void CExtInputBarDlg::OnBtnEdate() 
{	

// --> [Edit]  강지원 2008/11/06		//	마지막 일자를 세팅하지 않는다.
	return;
// <-- [Edit]  강지원 2008/11/06

	m_bEndDate = TRUE;

	//시작일 종료일
	m_btnStartDate	.FP_vSetImage(m_pIL_W20H20, 80, 80, TRUE, "▶", TRUE);	
	m_btnEndDate	.FP_vSetImage(m_pIL_W20H20, 82, 82, TRUE, "◀", TRUE);
	m_btnStartDate	.Invalidate();
	m_btnEndDate	.Invalidate();


	//================================================================================
	// 시작일 종료일을 가져온다. 05.09.06
	//================================================================================
	double dStartDate = 0, dEndDate = 0;
	
	((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

	if(dStartDate)
		m_strStartDate.Format("%d", (long)dStartDate);
	if(dEndDate)
		m_strEndDate.Format("%d", (long)dEndDate);

	if(		(m_strStartDate	.GetLength() != 8) ||
			(m_strEndDate	.GetLength() != 8) ||	
			(atoi(m_strStartDate)<= 19700101 ) ||
			(atoi(m_strEndDate	)<= 19700101 ) 
	)
	{
		CTime currentTime = CTime::GetCurrentTime(); 
		if(m_datePicker.GetSafeHwnd()) m_datePicker.SetTime(&currentTime);
		m_strStartDate = m_strEndDate =currentTime.Format("%Y%m%d");
	}
	
	//================================================================================

	

	 /* 달력컨트롤의 날짜를 시스템 날짜로 설정하기 */ 
	if(!m_strEndDate.GetLength())
	{
		CTime currentTime = CTime::GetCurrentTime(); 	
		//m_datePicker.SetTime(&currentTime);	
		CString strCurTime = currentTime.Format("%Y%m%d");
//@유진삭제
//		m_datePicker.SetDataText(strCurTime);
//@유진삭제
//@유진추가
		m_datePicker.SetTime(&currentTime);
//@유진추가
		return;
	}

	m_strEndDate.TrimRight();	

	if(m_chType == TICK_DATA_CHART) return;
	if(m_chType == YEAR_DATA_CHART) return;


	int nYear	= atoi(m_strEndDate.Left(4)		);
	int nMonth	= atoi(m_strEndDate.Mid(4,2)	);
	int nDay	= atoi(m_strEndDate.Right(2)	);

	CTime myTime(nYear,nMonth, nDay, 0, 0, 0);      		
	//m_datePicker.SetTime(&myTime); 
//@유진삭제
//	m_datePicker.SetDataText(m_strEndDate);
//@유진삭제
//@유진추가
	m_datePicker.SetTime(&myTime);
//@유진추가

	if(m_pChartMng && m_pChartMng->GetSafeHwnd())
		m_pChartMng->SendMessage( UMS_SETCHART_DATEINFO, (WPARAM)m_bEndDate, (LPARAM)atoi(m_strEndDate));
}

//void CExtInputBarDlg::SetCheckRevised(BOOL bUseRevised)
//{
////	m_bRevised = bUseRevised;
//}

void CExtInputBarDlg::SetCheckRevised(BOOL bRevised)
{
	m_bChkRevised = bRevised;
	UpdateData(FALSE);
}

void CExtInputBarDlg::OnChkRevised() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_UPDATEREVISED, m_bChkRevised);
}


void CExtInputBarDlg::SetMarketType(int nMarketType)
{
	m_nMarketType = nMarketType;
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_btYUnit.EnableWindow(TRUE);
	}
	else
	{
		m_btYUnit.EnableWindow(FALSE);
		m_nYUnitType = WON_TICK;
		m_btYUnit.SetWindowText("\\");
	}
} 

void CExtInputBarDlg::ChangeStatusDateControl(int nDataType)
{
	m_nDataType = nDataType;
	BOOL bEnable = TRUE;
	char a = nDataType + '0';
	//char cType[1];
	//itoa(nDataType, cType, 10);

	if(
		( a == TICK_DATA_CHART) ||
		( a == SEC_DATA_CHART) ||
		( a == MIN_DATA_CHART )
		)

	m_btnStartDate	.EnableWindow(bEnable);
	m_btnEndDate	.EnableWindow(bEnable);
	//m_datePicker	.EnableWindow(bEnable);		
//@유진삭제
//	if(m_datePicker.GetEnabled() == !bEnable)
//		m_datePicker.SetEnabled(bEnable);		
//@유진삭제
//@유진추가
	if ( !m_bRealUpdate )
	{
		if(m_datePicker.IsWindowEnabled() == !bEnable)
			m_datePicker.EnableWindow(bEnable);		
	}

////////////////////////////////////////////////////////////////////////////////////////////
// --> [Edit]  강지원 2008/11/09	( 임시로 막기 )
//	m_datePicker.EnableWindow(FALSE);
//	GetDlgItem(IDC_REAL_CHECK)->EnableWindow(FALSE);
//	GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
// <-- [Edit]  강지원 2008/11/09
////////////////////////////////////////////////////////////////////////////////////////////

//@유진추가

	m_btnStartDate.SetWindowText("▶");	
	m_btnEndDate.SetWindowText("◀");
}

//void CExtInputBarDlg::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
//{	
//	LPNMDATETIMECHANGE lpChange = (LPNMDATETIMECHANGE)pNMHDR;	
//	if(m_bEndDate)
//	{
//		m_strEndDate.Format("%04d%02d%02d", lpChange->st.wYear, lpChange->st.wMonth, lpChange->st.wDay);
//		
//	}
//	else
//	{
//		m_strStartDate.Format("%04d%02d%02d", lpChange->st.wYear, lpChange->st.wMonth, lpChange->st.wDay);
//	}	
//	
//	*pResult = 0;
//}

void CExtInputBarDlg::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{	
//@유진삭제
//	CString strDate = m_datePicker.GetDataText();
//@유진삭제
//@유진추가
	CString strDate;
	CTime timeTime;
	DWORD dwResult = m_datePicker.GetTime(timeTime);
	if (dwResult == GDT_VALID)
		strDate = timeTime.Format(_T("%Y%m%d"));
//@유진추가
	if(m_bEndDate)
	{
		if (m_strEndDate != strDate)
		{
			m_strEndDate = strDate;
		}
		else
		{
			return;
		}
	}
	else
	{
		m_strStartDate = strDate;
	}	

// --> [Edit]  강지원 2008/11/02		( 여기서 조회를 해버리면 조회 완료후 마지막 일자로 다시 설정되기때문에 날짜 변경시에는 TR을 날리지 않는다. )
//	if(m_bEndDate) // 종료일 모드이면 TR을 날린다.
//	{
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();
//	}
// <-- [Edit]  강지원 2008/11/02

	*pResult = 0;

}

void CExtInputBarDlg::OnCloseupDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//2007.02.28 by LYH 달력선택시 무조건 조회
	//if(	atoi(m_strStartDate) > atoi(m_strEndDate)) return;
	//}}

	if(m_bEndDate) // 종료일 모드이면 TR을 날린다.
	{
		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
	}
	else
	{
		if(m_pChartMng)
		{
			m_pChartMng->SendMessage(UMSG_MOVE_CHART_VIEWRANGE,
								atoi(m_strStartDate),
								atoi(m_strEndDate));
		}
	}
	*pResult = 0;
}

//#ifdef __INFO
//	#define SELL_SCR_NO		"5000"
//	#define BUY_SCR_NO		"5001"
//#else
	#define SELL_SCR_NO		"8325"
	#define BUY_SCR_NO		"8326"
//#endif

void CExtInputBarDlg::OnBtnSellscr() 
{	
	
	//OpenMap(SELL_SCR_NO);
	//종목 공유 발생
}

void CExtInputBarDlg::OnBtnBuyscr() 
{	
	//OpenMap(BUY_SCR_NO);
	//종목 공유 발생
}


//-----------------------------------------------------------------------------
// Reason		: 화면 열기
//		- CMapCoreView::OpenMap(LPCTSTR szMapName, LPCTSTR szArg0, LPCTSTR szArg1) 
//			 함수를 참고로 함.
//-----------------------------------------------------------------------------
// start
long CExtInputBarDlg::OpenMap(LPCTSTR szMapName)
{	
	//m_pCommonBaseInfo = ((CStdDialog*)GetParent())->GetpMainBaseInfo();
	//if(m_pCommonBaseInfo && m_strCode.GetLength() == CODE_STOCK_LENGTH)
	//{		
	//	m_pCommonBaseInfo->SetSharedData2(0,0,_T("종목코드"),(BYTE*)(LPTSTR)(LPCTSTR)m_strCode,CODE_STOCK_LENGTH,0);	
	//}


//	CString m_strTempMapName = szMapName;
//	if( m_strTempMapName.GetLength() >= 6)
//		m_strTempMapName = m_strTempMapName.Mid(2, 4);
//	else if( m_strTempMapName.GetLength() >= 4)
//		m_strTempMapName = m_strTempMapName.Left(4);
//	else
//		m_strTempMapName.Format("%04d", atoi((LPSTR)szMapName));
//
//	CString	strName0 = "_Open_arg0_" + m_strTempMapName;
//	CWnd* pMainWnd = AfxGetMainWnd();
//
//	pMainWnd->SendMessage(WMU_SETVAR, (WPARAM)(LPSTR)(LPCTSTR)strName0, (LPARAM)(LPSTR)"");
//
//	CString	strName1 = "_Open_arg1_" + m_strTempMapName;
//	pMainWnd->PostMessage(WMU_SETVAR, (WPARAM)(LPSTR)(LPCTSTR)strName1, (LPARAM)(LPSTR)"");
//
//	pMainWnd->PostMessage(WMU_OPEN_MAP, -1, (LPARAM)(LPCSTR)m_strTempMapName);
	return 0;
}
void CExtInputBarDlg::OnBtfixperiod() 
{
	//CString strDate;
	//CTime tmItem;
	//m_datePicker.GetTime(tmItem);
	//strDate = tmItem.Format("%Y%m%d");	

	//InputBar 로 부터 현재 ZoomValue값을 가져온다.
	

//	CString strZoomValue;
//	long lZoomValue = 0;
//	m_editZoomValue.GetWindowText(strZoomValue);
//	lZoomValue = atol(strZoomValue);

//	m_bChkPeriod = m_btFixPeriod.GetCheck();
//	GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 1, m_bChkPeriod);
//	if(m_bChkPeriod)
//	{
		int lEndDate = atoi(m_strEndDate);

		SYSTEMTIME tm;
		::ZeroMemory(&tm,sizeof(tm));
		if(m_chType==DAY_DATA_CHART||m_chType==WEEK_DATA_CHART)
		{
			tm.wYear = lEndDate/10000;
			tm.wMonth = lEndDate%10000/100;
			tm.wDay = lEndDate%100;
		}
		else if(m_chType==MONTH_DATA_CHART)
		{
			tm.wYear = lEndDate/10000;
			tm.wMonth = lEndDate%10000/100;
			tm.wDay = 1;
		}

		if(m_bEndDate) // 종료일로 선택되어 있을 경우에만 달력 컨트롤에 값을 셋한다.
		{
			//m_datePicker.SetTime(&tm);
//@유진삭제
//			m_datePicker.SetDataText(m_strEndDate);
//@유진삭제
//@유진추가
			m_datePicker.SetTime(&tm);
//@유진추가
		}

		m_pChartMng->SendMessage(UMSG_FIXDATE,0, (LPARAM)(LPCTSTR)m_strEndDate);
//	}
}

void CExtInputBarDlg::OnChkEndsave() 
{
	// TODO: Add your control notification handler code here
	if( m_chkEndSave.GetCheck() )
		m_nSave = SAVE_TRUE	;
	else
		m_nSave = SAVE_FALSE;
}

void CExtInputBarDlg::OnChkupdateindex() 
{
	// TODO: Add your control notification handler code here
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_UPDATEINDEX,0);
//	if( m_chkUpdateIndex.GetCheck() )
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_UPDATEINDEX,m_chkShowScale.GetCheck());
//	}	
}

BOOL CExtInputBarDlg::GetTimeSyncFlag()
{
	return m_chkTypeLink.GetCheck();
}

void CExtInputBarDlg::SetTimeSyncFlag(BOOL bCheck)
{
	m_bReplayMode = bCheck;
	
	m_chkTypeLink.SetCheck(bCheck);	
	m_chkTypeLink.Invalidate();
}

void CExtInputBarDlg::EnableWindowExtBar(BOOL bEnable)
{
}

//void CExtInputBarDlg::OnBtnOdrtrace() 
//{		
//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//		m_pChartMng->SendMessage(UMSG_CHARTODRTRACE);
//}

void CExtInputBarDlg::OnBtnAnalizePeriod() 
{	
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_APPLY_ANALYZE_PERIOD_ADDIN);	
}

void CExtInputBarDlg::OnBtnPeriod() 
{
	// TODO: Add your control notification handler code here
	
}

void CExtInputBarDlg::OnBtetcmenu() 
{
	// TODO: Add your control notification handler code here
    CMenu menu;
	CMenu *pMenuChild = NULL;

	//char* szEtcMenuName[] = {	"관심지표", "저장", "불러오기", "수정주가", "종료시 상태저장", "허수봉 표시", "지표 전체적용"};
	//{{저장, 불러오기 삭제 by LYH 2007.01.05
	//Eugene은 무조건 허수봉

	//20080914 이문수 >>
	//우리선물 fx챠트에는 수정주가 개념이 없어서 제거되고 프린트 기능이 추가된다.	
	//char* szEtcMenuName[] = {	"관심지표", "수정주가", "지표 전체적용"};

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	//char* szEtcMenuName[] = {"관심지표", "지표 전체적용", "분석툴 저장", "차트인쇄", "차트그림저장", "미결제라인표시"};	
	char* szEtcMenuName[] = {"관심지표", "지표 전체적용", "분석툴 저장", "분석툴 공유", "차트인쇄", "차트그림저장", "미결제라인표시"};	
	// 2011.01.12 by SYS <<


	//20080914 이문수 <<

 //	char* szEtcMenuName[] = {	"수정주가", 
 //								"종료시 차트저장",
 //								"시간연동 수신", 
 //								"상하한가 표시", 
 //								"허수봉 표시", 
 //								"권리락 표시"};
 	

 	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
     menu.CreatePopupMenu();
 	UINT nFlags = NULL;
 
 	//menu.AppendMenu(MF_UNCHECKED,100,szEtcMenuName[0]);
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();
	menu.AppendMenu(MF_POPUP,(UINT)pMenuChild->GetSafeHmenu(),szEtcMenuName[0]);


	CFavoriteThingsMng* pFavoriteMng = NULL;
	BOOL bExistance;
	int nFlagsChild;
	int lPosAll = 0;
	CStringArray strArrayIndicatorAll;	

	pFavoriteMng = &((CStdDialog*)GetParent())->m_FavoriteMng;

	stFvrData _stFvrData;
	BOOL	bCreateRoot = FALSE;
	if(pFavoriteMng)
	{
		pFavoriteMng->LoadItemInfo("JipyoFile");
		int wSizeItem = pFavoriteMng->m_ArrayFvrItem.GetSize();
		for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
		{
			//_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
			//treeItem->InsertItem(_stData.m_chData1);
			_stFvrData = pFavoriteMng->m_ArrayFvrItem.GetAt(wIndexItem);
				//if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX&&_stFvrData.m_lSubData1==0)
				if(_stFvrData.m_lTabNum==TABNO_ASSISTINDEX)
				{
					bCreateRoot = TRUE;
					if(m_pChartMng->GetSafeHwnd())
						bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)_stFvrData.m_chData1);
					if(bExistance)
					{
						nFlagsChild = MF_CHECKED;
					}
					else
					{
						nFlagsChild = MF_UNCHECKED;
					}
					pMenuChild->AppendMenu(nFlagsChild,200+lPosAll,_stFvrData.m_chData1);
					lPosAll++;
					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
				}

		}		

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
//		//for(wIndexGrp=0;wIndexGrp<wSizeGrp;wIndexGrp++)
//		//{
//			//bCreateRoot = FALSE;
//			//strGrpName  = pFavoriteMng->m_strArrayGrp.GetAt(wIndexGrp);
//			pFavoriteMng->LoadItemInfo("JipyoFile");
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
//					pMenuChild->AppendMenu(nFlagsChild,200+lPosAll,_stFvrData.m_chData1);
//					nCntAddedByFavorite++;
//					lPosAll++;
//					strArrayIndicatorAll.Add(_stFvrData.m_chData1);
//				}
//			}		
		//}
	}

	//{{저장, 불러오기 삭제 by LYH 2007.01.05
	//menu.AppendMenu(MF_UNCHECKED,101,szEtcMenuName[1]);
	//menu.AppendMenu(MF_UNCHECKED,102,szEtcMenuName[2]);
	//}}


	//20080914 이문수 >>
	//우리선물 fx챠트에는 수정주가 개념이 없어서 제거
 	//if(m_bChkRevised)
 	//{
 	//	nFlags = MF_CHECKED;
 	//}
 	//else
 	//{
 	//	nFlags = MF_UNCHECKED;
 	//}
	//menu.AppendMenu(nFlags,101,szEtcMenuName[1]);
	//20080914 이문수 <<
 

// 	if(m_nSave)
//	{
// 		if(m_nSave == 1)
//		{
// 			nFlags = MF_CHECKED;
// 		}
// 		else if(m_nSave == 2)
// 		{
// 			nFlags = MF_UNCHECKED;
// 		}		
// 	}
// 	else
// 	{
// 		nFlags = MF_GRAYED;
// 	}
// 	menu.AppendMenu(nFlags,102,szEtcMenuName[2]);		
 
 	//if(m_bUseNormalizedMinuteTimeRule)
 	//{
 	//	if(m_bNormalizedMinuteTimeRule)
 	//	{
 	//		nFlags = MF_CHECKED;
 	//	}
 	//	else
 	//	{
 	//		nFlags = MF_UNCHECKED;
 	//	}
 	//	menu.AppendMenu(nFlags,105,szEtcMenuName[5]);
 	//}


	//{{JS.Kim_20100903 Solomon 지표전체적용으로 하면 추세선이 저장이 안되는 오류가 있어 일단 메뉴에서 없앰
	/*
	if(m_bChkSaveJipyoOneFile)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}

// --> [Edit]  강지원 2008/11/08		( 2종목 이상일경우 지표 전체적용 메뉴를 Disable로 해달라는 요청 )
	int nChartCnt = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount();
	if ( nChartCnt > 1 || GetTimeSyncFlag() )
		nFlags = MF_DISABLED | MF_GRAYED;
// <-- [Edit]  강지원 2008/11/08

	//20080914 이문수 >>
	//우리선물 fx챠트에는 수정주가 개념이 없어서 제거하고 메뉴를 하나씩 위로 올린다.
 	//menu.AppendMenu(nFlags,102,szEtcMenuName[2]);
	menu.AppendMenu(nFlags,102,szEtcMenuName[1]);
	//20080914 이문수 <<
	*/

	// 분석툴 개별 저장 - ojtaso (20080723)
	if(m_bChkSaveAnalTool)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}
 	menu.AppendMenu(nFlags,103,szEtcMenuName[2]);


	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	//20080914 이문수 >>
	//nFlags = MF_UNCHECKED;
	//menu.AppendMenu(nFlags,106,szEtcMenuName[3]);//챠트인쇄
	//menu.AppendMenu(nFlags,107,szEtcMenuName[4]);//챠트그림저장
	//20080914 이문수 <<

	// 분석툴 공유
	if (m_bChkSaveAnalTool)
	{
		if(m_bChkShareAnalTool)
			nFlags = MF_CHECKED;
		else
			nFlags = MF_UNCHECKED;
	}
	else
	{
		// 분석툴 저장 기능이 체크가 안되어 있으면 disable 한다.
		nFlags = MF_UNCHECKED | MF_DISABLED;
	}
	// 2010.01.18일 기존 기능과 동일하게 하기 위해 분석툴 공유 기능 막음.
	menu.AppendMenu(nFlags,104,szEtcMenuName[3]);

	//20080914 이문수 >>
	nFlags = MF_UNCHECKED;
	menu.AppendMenu(nFlags,106,szEtcMenuName[4]);//챠트인쇄
	menu.AppendMenu(nFlags,107,szEtcMenuName[5]);//챠트그림저장
	//20080914 이문수 <<
	// 2011.01.12 by SYS <<



	/* solomon
	CStdDialog* pStdDialog = (CStdDialog*)GetParent();
	if (pStdDialog->m_bMigyulcheDraw)
	{
 		nFlags = MF_CHECKED;
 	}
 	else
 	{
 		nFlags = MF_UNCHECKED;
 	}
 	menu.AppendMenu(nFlags,108,szEtcMenuName[5]);
	*/

// 	if(m_bShowLock)
// 	{
// 		nFlags = MF_CHECKED;
// 	}
// 	else
// 	{
// 		nFlags = MF_UNCHECKED;
// 	}
// 	menu.AppendMenu(nFlags,105,szEtcMenuName[5]);
 
 	CRect rect;
 	m_btEtcMenu.GetWindowRect(rect);
 	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);

	pMenuChild = menu.GetSubMenu(0);
	if(pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}
    menu.DestroyMenu();	
	if(nRetValue >= 200)
	{
		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strArrayIndicatorAll.GetAt(nRetValue-200));
		return;
	}

 	switch(nRetValue)
 	{
//	case 101:
//		OnBtsave();
//		break;
//	case 102:
//		OnBtload();
//		break;
 	case 101:
		m_bChkRevised = !m_bChkRevised;
 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 			m_pChartMng->SendMessage(UMSG_UPDATEREVISED,m_bChkRevised);
 		break;
// 	case 104:
// 		if(m_nSave==1)
// 		{
// 			m_nSave = 2;
// 		}
// 		else if(m_nSave==2)
// 		{
// 			m_nSave = 1;
// 		}
// 		break;
 	//case 105:
 	//	m_bNormalizedMinuteTimeRule = !m_bNormalizedMinuteTimeRule;
 	//	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 	//		m_pChartMng->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,m_bNormalizedMinuteTimeRule);
 	//	break;
	case 102:
 		m_bChkSaveJipyoOneFile = !m_bChkSaveJipyoOneFile;
 		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 			m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveJipyoOneFile);
 		break;

	// 분석툴 개별 저장 - ojtaso (20080723)
	case 103:
		// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
		{
// 			BOOL bShareChange = FALSE;
// 
// 			// 이전상태가 : 추세선저장 및 추세선공유 기능이 체크된 상태이면,
// 			// 추세선공유가 해제되므로, 
// 			// 기존, 추세선공유 기능을 통해 그려진 추세선 정보를 지우고, 
// 			// 현재 선택된 추기의 추세선정보를 그린다.
// 			if (m_bChkSaveAnalTool && m_bChkShareAnalTool)
// 			{
// 				bShareChange = TRUE;
// 
// 				// 현재 그려진 추세선정보 저장..
// 				ChangeAnalToolFile(TRUE);
// 
// 				// 기존 추세선 모두 지우기...
// 				((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
// 			}			
// 			
			//
 			m_bChkSaveAnalTool = !m_bChkSaveAnalTool;
 			if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
 				m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveAnalTool, 100);

			if (!m_bChkSaveAnalTool)
			{
				// 분석툴 저장 기능이 체크 해지면 => 분석툴 공유 기능도 자동 체크 해지한다.
				m_bChkShareAnalTool = FALSE;
				if(m_pChartMng->GetSafeHwnd()) 
					m_pChartMng->SendMessage(UMSG_SET_ALALTOOL_SHAREFILE,m_bChkShareAnalTool, 0);
			}

// 			if (bShareChange)
// 			{
// 				// 현재 주기의 추세선파일로 추세선을 다시 그린다.
// 				ChangeAnalToolFile(FALSE);
// 			}
		}
 		break;

	case 104:
		{
			// 현재 그려진 추세선정보 저장..
// 			ChangeAnalToolFile(TRUE);
// 
// 			// 기존 추세선 모두 지우기...
// 			((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
// 			
			m_bChkShareAnalTool = !m_bChkShareAnalTool;
			if(m_pChartMng->GetSafeHwnd()) 
				m_pChartMng->SendMessage(UMSG_SET_ALALTOOL_SHAREFILE,m_bChkShareAnalTool, 0);
			
			if (m_bChkShareAnalTool)
			{
				// 분석툴 공유 기능이 체크 상태면 => 분석툴 저장 기능도 자동 체크한다.
				m_bChkSaveAnalTool = TRUE;
				if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
					m_pChartMng->SendMessage(UMSG_SAVEJIPYOONEFILE,m_bChkSaveAnalTool, 100);
			}

			// 추세선공유 파일의 추세선을 그린다.
//			ChangeAnalToolFile(FALSE);
		}
		break;
	// 2011.01.31 by SYS 

	//20080914 이문수 챠트인쇄, 챠트그림저장>>
	case 106:
		{
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_CONTAINER_PRINT);
		}
		break;
	case 107:
		{
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_IMG_SAVE);
		}
		break;
	/* solomon
	//20080914 이문수 <<
	case 108:
		{
			((CStdDialog*)GetParent())->m_bMigyulcheDraw = !((CStdDialog*)GetParent())->m_bMigyulcheDraw;
			if(m_pChartMng && IsWindow(m_pChartMng->GetSafeHwnd()))
				m_pChartMng->SendMessage(UMSG_CHART_MIGYULCHE, ((CStdDialog*)GetParent())->m_bMigyulcheDraw);
		}
		break;
	*/
  	}
}


//@유진삭제	
//void CExtInputBarDlg::OnBtSub1() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bCheck = m_btSub1.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//}
//@유진삭제	

void CExtInputBarDlg::OnBtSub2() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_btSub2.GetCheck();
	if(bCheck)
	{
		//((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
		//m_btSub1.SetCheck(FALSE);
		//m_btSub3.SetCheck(FALSE);
		//m_btSub8.SetCheck(FALSE);
		//m_btSub9.SetCheck(FALSE);
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, TRUE);
	}
	else
	{
		//((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, FALSE);
	}
}

//@유진삭제	
//void CExtInputBarDlg::OnBtSub3() 
//{
//	// TODO: Add your control notification handler code here
//	BOOL bCheck = m_btSub3.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_HORZ, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//}
//
//void CExtInputBarDlg::OnBtSub4() 
//{
//	// TODO: Add your control notification handler code here
//	m_btSub1.SetCheck(FALSE);
//	m_btSub2.SetCheck(FALSE);
//	m_btSub3.SetCheck(FALSE);
//
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL, TRUE);
//}
//
//void CExtInputBarDlg::OnBtSub5() 
//{
//	// TODO: Add your control notification handler code here
//	// TODO: Add your control notification handler code here
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	m_btSub1.SetCheck(FALSE);
//	m_btSub2.SetCheck(FALSE);
//	m_btSub3.SetCheck(FALSE);
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CHART_INVERT, TRUE);
//}
//
//void CExtInputBarDlg::OnBtSub6() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);	
//	BOOL bCheck = m_btSub6.GetCheck();
//	::SendMessage(GetParent()->m_hWnd, USMG_SHOW_DATAWND, 0, (LPARAM)bCheck);
//}
//@유진삭제	


void CExtInputBarDlg::OnChkfixperiod() 
{
	// TODO: Add your control notification handler code here
	if(m_chkFixPeriod.GetCheck())
	{
		//================================================================================
		// 시작일 종료일을 가져온다. 05.09.06
		//================================================================================
		double dStartDate = 0, dEndDate = 0;
		
		((CStdDialog*)GetParent())->GetStartEndDateAtStdDialog(dStartDate, dEndDate);

		if(dStartDate)
			m_strStartDate.Format("%d", (long)dStartDate);
		if(dEndDate)
			m_strEndDate.Format("%d", (long)dEndDate);
		//================================================================================OnBtnEdate();
//		CString strDate;
//		CTime tmItem;
//		m_datePicker.GetTime(tmItem);
//		strDate = tmItem.Format("%Y%m%d");
		CString strZoomValue;
		//long lZoomValue = 0;
		int lZoomValue = m_pChartMng->SendMessage(UMSG_GETZOOMVALUE, m_nDataType);
	//	m_editZoomValue.GetWindowText(strZoomValue);
		//lZoomValue = atol(strZoomValue);
		m_pChartMng->SendMessage(UMSG_FIXDATE,(WPARAM)lZoomValue, (LPARAM)(LPCTSTR)m_strEndDate);
	}
}

//@유진삭제
//void CExtInputBarDlg::OnBtinitialPeriod() 
//{
//	// TODO: Add your control notification handler code here
//	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DISPLAY_RESTORE, TRUE);
//	m_pChartMng->SendMessage(UMSG_SETZOOMVALUE,120);
//}
//@유진삭제

//001 2006.06.22 {{ EUGENE컨트롤의 이벤트를 받는다
BOOL CExtInputBarDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_BTETCMENU)			///> Button
	{
		switch (HIWORD(wParam))
		{
		case BN_CLICKED :
			TRACE("ExtInputBarDlg : BUTTON BN_CLICKED") ;
//			OnBtetcmenu();
			break;
		case BN_SETFOCUS :
			TRACE("ExtInputBarDlg : BUTTON BN_SETFOCUS") ;
			OnBtetcmenu();
			break;
		case BN_KILLFOCUS :
			TRACE("ExtInputBarDlg : BUTTON BN_KILLFOCUS") ;
			break;
		}
	} 
	
	return CDialog::OnCommand(wParam, lParam);
}
//001 2006.06.22 }}

//@유진삭제	
//S01 2006.09.07 {{ Tool 변경
//void CExtInputBarDlg::OnBtnChangeTool() 
//{
//	BOOL bInterlockShow;
//	BOOL bToolShow;
//	if(m_bChangeTool)	// Tool Button 을 보여줌
//	{
//		bInterlockShow = TRUE;
//		m_btSub1.ShowWindow(bInterlockShow);
//		m_btSub2.ShowWindow(bInterlockShow);
//		m_btSub3.ShowWindow(bInterlockShow);
//		m_btSub4.ShowWindow(bInterlockShow);
//		m_btSub5.ShowWindow(bInterlockShow);
//		m_btSub6.ShowWindow(bInterlockShow);
//		m_btSub7.ShowWindow(bInterlockShow);
//		m_btSub8.ShowWindow(bInterlockShow);
//		m_btSub9.ShowWindow(bInterlockShow);
//		m_btSub10.ShowWindow(bInterlockShow);
//		m_btSub11.ShowWindow(bInterlockShow);
//		m_btSub12.ShowWindow(bInterlockShow);
//		m_btSub13.ShowWindow(bInterlockShow);
//		m_btSub14.ShowWindow(bInterlockShow);
//		//		m_btYUnit.ShowWindow(bInterlockShow);
//		bToolShow = FALSE;
//		m_stBy.ShowWindow(bToolShow);				// 1 X 1
//		m_btShowMatrix.ShowWindow(bToolShow);		// 분할
//		m_btInitSplit.ShowWindow(bToolShow);		// 분할복원
//		m_chkJongmokLink.ShowWindow(bToolShow);		// 종목연동
//		m_chkTypeLink.ShowWindow(bToolShow);		// 주기연동
//		m_btFixPeriod.ShowWindow(bToolShow);		// 기간연동
//		m_chkUpdateIndex.ShowWindow(bToolShow);		// 지표연동	
//		m_bChangeTool = FALSE;
//	}
//	else	// 연동 버튼을 보여줌
//	{
//		bInterlockShow = FALSE;
//		m_btSub1.ShowWindow(bInterlockShow);
//		m_btSub2.ShowWindow(bInterlockShow);
//		m_btSub3.ShowWindow(bInterlockShow);
//		m_btSub4.ShowWindow(bInterlockShow);
//		m_btSub5.ShowWindow(bInterlockShow);
//		m_btSub6.ShowWindow(bInterlockShow);
//		m_btSub7.ShowWindow(bInterlockShow);
//		m_btSub8.ShowWindow(bInterlockShow);
//		m_btSub9.ShowWindow(bInterlockShow);
//		m_btSub10.ShowWindow(bInterlockShow);
//		m_btSub11.ShowWindow(bInterlockShow);
//		m_btSub12.ShowWindow(bInterlockShow);
//		m_btSub13.ShowWindow(bInterlockShow);
//		m_btSub14.ShowWindow(bInterlockShow);
//		//		m_btYUnit.ShowWindow(bInterlockShow);
//		bToolShow = TRUE;
//		m_stBy.ShowWindow(bToolShow);				// 1 X 1
//		m_btShowMatrix.ShowWindow(bToolShow);		// 분할
//		m_btInitSplit.ShowWindow(bToolShow);		// 분할복원
//		m_chkJongmokLink.ShowWindow(bToolShow);		// 종목연동
//		m_chkTypeLink.ShowWindow(bToolShow);		// 주기연동
//		m_btFixPeriod.ShowWindow(bToolShow);		// 기간연동
//		m_chkUpdateIndex.ShowWindow(bToolShow);		// 지표연동	
//		m_bChangeTool = TRUE;
//	}
//}
//S01 2006.09.07 }}
//
//void CExtInputBarDlg::OnBtSub8() 
//{
//	BOOL bCheck = m_btSub8.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub9.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_LINE_VERT, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}	
//}
//
//void CExtInputBarDlg::OnBtSub9() 
//{
//	BOOL bCheck = m_btSub8.GetCheck();
//	if(bCheck)
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//		m_btSub1.SetCheck(FALSE);
//		m_btSub2.SetCheck(FALSE);
//		m_btSub3.SetCheck(FALSE);
//		m_btSub8.SetCheck(FALSE);
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL_WITH_NIDLG, TRUE);
//	}
//	else
//	{
//		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
//	}
//
//}
//@유진삭제	

/*
void CExtInputBarDlg::OnBtnGwansim() 
{
	// TODO: Add your control notification handler code here
	CMenu menu;

	UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	menu.CreatePopupMenu();

	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(!pGateMng)
		return;

	IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
	if(!pMng) return;

	/////////////////////////
	CUIntArray arrIndex;
	CStringArray arrName;
	// 5. 관심 매니저의 멤버함수 부재로 인한 에러 수정필요
//	pMng->GetGwansimGroupList(arrIndex, arrName);

	int nSize = arrName.GetSize();
	char szNo[3] = {0, };

	LPCSTR szGroupName;
	UINT nGroupIndex;
	for(int nIndx = 0; nIndx < nSize; ++nIndx)
	{
		szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
		nGroupIndex = (UINT)arrIndex.GetAt(nIndx);	//관심그룹인덱스.
		menu.AppendMenu(MF_STRING,200+nGroupIndex,szGroupName);
	}

	CRect rect;
	m_btnGwanSim.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
	menu.DestroyMenu();	

	if(nRetValue >= 200)
	{
		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
		if(!pGateMng)
			return;

		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
		if(!pMng) return;

		/////////////////////////
		CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
		CStringArray strCodes;
		strCodes.Add(strCode);
		UINT nGroup = nRetValue-200;
		// 4. 관심 매니저의 멤버함수 부재로 인한 에러 수정필요
//		pMng->AddGwansimtem_New(nGroup, strCodes, (long)m_hWnd);
		return;
	}
}
*/
void CExtInputBarDlg::OnBtnGwansim() 
{
//@SolomonGNF
/*		// TODO: Add your control notification handler code here
		CMenu menu, pMenuChild;

 		UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_STRING,101,"관심종목 관리자");
		menu.AppendMenu(MF_SEPARATOR, (UINT)110, "");
		if(m_bChkRegGwansimUp)
		{
			menu.AppendMenu(MF_CHECKED,102,"  맨위로 추가");
			menu.AppendMenu(MF_UNCHECKED,103,"  맨아래로 추가");
		}
		else
		{
			menu.AppendMenu(MF_UNCHECKED,102,"  맨위로 추가");
			menu.AppendMenu(MF_CHECKED,103,"  맨아래로 추가");
		}

		menu.AppendMenu(MF_SEPARATOR, (UINT)110, "");
		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
		if(!pGateMng)
			return;

		CStringArray arrKey, arrName;
		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
		if(!pMng) return;

		//기존엔 그룹이름만 가져왔는데, 이제는 Key도 가져온다.
		pMng->GetGwansimGroupName(arrKey, arrName);

		CString strGroup;
		if(pMng)
		{
			for(int wIndexItem=0;wIndexItem<arrName.GetSize();wIndexItem++)
			{
				strGroup = arrName.GetAt(wIndexItem);
				menu.AppendMenu(MF_STRING,200+wIndexItem,strGroup);
			}		
		}
		
 		CRect rect;
 		m_btnGwanSim.GetWindowRect(rect);
 		int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);

		menu.DestroyMenu();	
		if(nRetValue == 101)
		{
	//		// 기존버전.
	//		//[[
	//		pMng->ShowGwansimDlg();
	//		//]]
	//
			// 수정한 버전. 
			//[[
			//2007.05.08 by LYH 관심 등록창 띄울 때 현재 코드 선택 할 수 있도록 종목코드 넘김(종목코드 마지막에 '|'구분자로 함)
			//pMng->ShowGwansimDlg();
			CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
	//		if(strCode && strCode.GetLength() > 0)
	//			pMng->ShowGwansimDlg(strCode+"|");
	//		else
	//			pMng->ShowGwansimDlg();
			//등록할 수 있는 코드인지 체크
			if (!strCode.IsEmpty() )
			{
				//BYTE cType = dll_GetJongMarket(strCode);
				//CString strType = dll_GetChangeMarketCode(cType, false);  // 시간외 거래 TRUE
	//@유진삭제	
	//			IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager);
	//			CString strType = pDataManager->GetCodeInfo(NULL, 2, strCode);
	//			BYTE ucMarketType = pDataManager->GetMarketTypeFromCode(strCode);
	//@유진삭제	
	//@유진은 선택된 코드 넘기지 않는다고 함.
	//			if( strType == "J" || strType == "Q" || strType == "F" || strType == "S" || strType == "O" || strType == "Z"
	//				|| strType == "T" || strType == "L"  || strType == "B")
	//			{			
	//				strCode += "|" ;				
	//			}
	//			else
				{
					strCode = "";
				}
				pMng->ShowGwansimDlg(this, strCode);
			}
			else
			{
				pMng->ShowGwansimDlg(this);
			}
			//]]

			//}}
		}
		else if(nRetValue == 102)
			m_bChkRegGwansimUp = TRUE;
		else if(nRetValue == 103)
			m_bChkRegGwansimUp = FALSE;
		else if(nRetValue >= 200)
		{
			CString strCode = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetDataCode();
			CStringArray arrCodes;
			arrCodes.Add(strCode+"!");
			//arrCodes.Add(strCode);
			CString strKey, strGroup;

			strGroup = arrName.GetAt(nRetValue-200);
			strKey = pMng->GetGwanKeyFromName(strGroup);
			if(strKey.GetLength() == 0)
				return;

			if(m_bChkRegGwansimUp)
				pMng->ADDGwasimItem(strKey, arrCodes, TRUE);
			else
				pMng->ADDGwasimItem(strKey, arrCodes, FALSE);
		}
*/
}

void CExtInputBarDlg::OnBtScrollPre() 
{
	m_nScrollState = SCROLL_LEFT;

	// TODO: Add your control notification handler code here
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 106, 104);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();

	//배속
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT, nSpeed);
}

void CExtInputBarDlg::OnBtScrollStop() 
{
	m_nScrollState = SCROLL_STOP;

	// TODO: Add your control notification handler code here
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 68, 70);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();
	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_STOP , TRUE);
}

void CExtInputBarDlg::OnBtScrollNext() 
{
	m_nScrollState = SCROLL_RIGHT;

	// TODO: Add your control notification handler code here
	m_btScrollNext.FP_vSetImage(m_pIL_W20H20, 70, 68);
	m_btScrollPre.FP_vSetImage(m_pIL_W20H20, 104, 106);
	m_btScrollNext.Invalidate();
	m_btScrollPre.Invalidate();

		//배속
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT , nSpeed);
}

//@유진추가
void CExtInputBarDlg::OnSelchangeCmbScrollSpeed() 
{
	//배속
	int nIdx = m_cmbScrollSpeed.GetCurSel();
	if(nIdx == -1)
	{
		nIdx = 0;
	}
	int nSpeed = (int)pow(2.0, (double)nIdx);

	if(m_nScrollState == SCROLL_LEFT)
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT, nSpeed);
	else if(m_nScrollState == SCROLL_RIGHT)
		((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT, nSpeed);
}
//@유진추가

BOOL CExtInputBarDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CExtInputBarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, m_crBk);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CExtInputBarDlg::ClearLink()
{
	m_chkJongmokLink.SetCheck(0);
	OnChkjongmoklink();
	m_chkTypeLink.SetCheck(0);
	OnChktypelink();
}

// 복수종목에서 선택된 차트 정보 표시 : 복수종목 - ojtaso (20070223)
void CExtInputBarDlg::InitChartMulitItem(ST_SELCHART* pSelChart)
{
	m_nYUnitType = pSelChart->m_nChartNumericValueType;
	switch(m_nYUnitType)
	{
	case WON_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
		break;
	case PERCENT_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 20, 21, FALSE, "%", TRUE, 23);
		break;
	case DOLLAR_TYPE:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 16, 17, FALSE, "$", TRUE, 19);
		break;
	default:
		m_btYUnit.FP_vSetImage(m_pIL_W20H20_Tool, 12, 13, FALSE, "\\", TRUE, 15);
	}

// --> [Edit]  강지원 2008/12/04		( 분일경우 시간컨트롤을 활성화 시킨다. )
	if ( !m_bRealUpdate )
	{
//		if ( pSelChart )
//		{
////			SetExInputBarDlgTimeIndex(pSelChart->m_nOrgTimeIndex);
//			m_nTimeIndex = pSelChart->m_nOrgTimeIndex;
//		}

		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART || m_chType == MIN_DATA_CHART)
		{
			m_datePicker.EnableWindow(TRUE);
			if ( m_chType == MIN_DATA_CHART )
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
			else
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
		}
		else
			m_datePicker.EnableWindow(FALSE);
	}
	else
	{
		m_nTimeIndex = -1;
		m_datePicker.EnableWindow(FALSE);
		GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
	}
// <-- [Edit]  강지원 2008/12/04

	m_btYUnit.Invalidate();
}

void CExtInputBarDlg::OnChkQueryOpt() 
{
	UpdateData(TRUE);
	
	if(m_pChartMng)
		m_pChartMng->SendMessage(UMSG_REQUEST_FORQUERYOPT);
}

int CExtInputBarDlg::GetQueryOpt()
{
	return m_chkQueryOpt.GetCheck();
}

void CExtInputBarDlg::EnableQueryOpt(BOOL bEnable)
{
	m_chkQueryOpt.EnableWindow(bEnable);
}

void CExtInputBarDlg::ChangeSkinColor()
{
	m_crBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);
	Invalidate();
}

void CExtInputBarDlg::SaveExtInputBar()
{
	if(((CStdDialog*)GetParent())->m_strConfig == m_strCfgFile)
	{
		CString strTemp;
/*		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		if(nOpenedOwner==0&&m_nSave)
		{
			if(m_nSave==SAVE_TRUE)
			{
				strTemp = "1";
			}
			else if(m_nSave==SAVE_FALSE)
			{
				strTemp = "0";
			}
			::WritePrivateProfileString("ExtInputBar","Saved",strTemp,m_strCfgFile);
		}
*/
		strTemp.Format("%d", m_nDragModeType);
		::WritePrivateProfileString("ExtInputBar","DragModeType",strTemp,m_strCfgFile);

		strTemp.Format("%d", m_bChkSaveJipyoOneFile);
		::WritePrivateProfileString("ExtInputBar","SaveJipyoOneFile",strTemp,m_strCfgFile);	

		strTemp.Format("%d", m_bChkRegGwansimUp);
		::WritePrivateProfileString("ExtInputBar","RegGwansimUp",strTemp,m_strCfgFile);	

		// 분석툴 개별 저장 - ojtaso (20080723)
		strTemp.Format("%d", m_bChkSaveAnalTool);
		::WritePrivateProfileString("ExtInputBar","SaveAnalTool",strTemp,m_strCfgFile);	

		// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
		strTemp.Format("%d", m_bChkShareAnalTool);
		::WritePrivateProfileString("ExtInputBar","ShareAnalTool",strTemp,m_strCfgFile);	
		// 2011.01.12 by SYS 
	}
}

void CExtInputBarDlg::OnChkRealUpdate() 
{
	UpdateData(TRUE);
	
	if ( m_chkRealUpdate )
	{
//		//	종목이 2종목 이상일겨우 과거데이터 조회를 막는다. 
//		if (((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetChartItemCount() > 1)
//		{
//			m_chkRealUpdate = FALSE;
//			UpdateData(FALSE);
//
//			AfxMessageBox("차트에서 2종목 이상일 경우 과거데이터 조회를 하실수 없습니다.");
//
//			return;
//		}

		// 리얼체크 알림을 보여줄지 결정
		CString strTemp = "";
		char szTemp[64];
		memset(szTemp, 0x00, sizeof(szTemp));
		::GetPrivateProfileString("ExtInputBar","RealCheck","0",szTemp,sizeof(szTemp),m_strCfgFile);
		strTemp = szTemp;

		if ( strTemp == "0" )
		{
			int nResult = AfxMessageBox("시간을 지정하시면 조회시 실시간 데이터를 받으실수 없습니다. \n\r알림을 다시 보지 않으시려면 확인을 눌러주세요.", MB_OKCANCEL );
			if ( nResult == IDOK )
				::WritePrivateProfileString( "ExtInputBar", "RealCheck", "1" , m_strCfgFile);
		}

		UpdateTimeCtrl();

// --> [Edit]  강지원 2008/12/08	( 선택된 종목의 시간대를 설정한다. )
		ST_SELCHART* pSelChart = NULL;
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
		if ( pSelChart )
		{
//			SetExInputBarDlgTimeIndex(pSelChart->m_nOrgTimeIndex);
			if ( pSelChart->m_nTimeIndex <= 0 )
				m_nTimeIndex = pSelChart->m_nOrgTimeIndex;
			else
				m_nTimeIndex = pSelChart->m_nTimeIndex;
		}
// <-- [Edit]  강지원 2008/12/08

		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART || m_chType == MIN_DATA_CHART)
		{
			m_datePicker.EnableWindow(TRUE);

			if( m_chType == MIN_DATA_CHART )
				GetDlgItem(IDC_TIMEPICKER)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
			m_datePicker.EnableWindow(FALSE);
		}

		m_bRealUpdate = FALSE;
	}
	else
	{
		GetDlgItem(IDC_TIMEPICKER)->EnableWindow(FALSE);
		m_datePicker.EnableWindow(FALSE);
		m_bRealUpdate = TRUE;

// --> [Edit]  강지원 2008/12/03
		((CChartMng*)m_pChartMng)->GetpChartItemSelected()->AllChartItemCodeRequest();

		//	체크 해제시 재조회를 요청한다.
//		CTime currentTime = CTime::GetCurrentTime(); 	
//		m_datePicker.SetTime(&currentTime);	
//		m_strEndDate = currentTime.Format("%Y%m%d");
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
// <-- [Edit]  강지원 2008/12/03
	}

//	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
//	if ( pSelChart )
//	{
//		pSelChart->m_nRealCheck = m_bRealUpdate
//	}

}

void CExtInputBarDlg::UpdateTimeCtrl() 
{
	CTime currentTime = CTime::GetCurrentTime(); 	
	CString strTime = "";
	strTime = currentTime.Format("%H%M%S");

	ST_SELCHART* pSelChart = NULL;
	((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);

	if ( pSelChart )
	{
		int nGab(0), nSeoulGab(0);
		nSeoulGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(22, 1));

		if ( pSelChart->m_nTimeIndex <= 0 )
			nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(pSelChart->m_nOrgTimeIndex, 1));
		else
			nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(pSelChart->m_nTimeIndex, 1));

		CString strValue = "";
		strTime = ((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetCalcTimeGab(strValue, strTime, (nGab - nSeoulGab));
		strTime.Replace(":", "");

		int nHour	= atoi(strTime.Left(2)	);	
		int nMin	= atoi(strTime.Mid(2,2)	);
		int nSec	= atoi(strTime.Right(2)	);	

		m_ctlTime = CTime(currentTime.GetYear(), currentTime.GetMonth(), currentTime.GetDay(), nHour, nMin, nSec);
		m_datePicker.SetTime(&currentTime);	
	}
	else
		m_ctlTime = currentTime;

	UpdateData(FALSE);

/*
	CString strTime = ((CChartMng*)m_pChartMng)->GetSelectTime();
	strTime.Remove(':');
	if ( strTime >= 6 )
	{
		CTime myTime;
		m_datePicker.GetTime(myTime);

		int nHour	= atoi(strTime.Left(2)	);	
		int nMin	= atoi(strTime.Mid(2,2)	);
		int nSec	= atoi(strTime.Right(2)	);	

		m_ctlTime = CTime(myTime.GetYear(), myTime.GetMonth(), myTime.GetDay(), nHour, nMin, nSec);
		UpdateData(FALSE);
	}
*/
}

CString CExtInputBarDlg::GetCtlTime() 
{
	UpdateData(TRUE);
	
	CString strResult = "";
	CTime myTime;
	m_datePicker.GetTime(myTime);
	
	if ( m_chkRealUpdate )
	{
		if( m_chType == DAY_DATA_CHART || m_chType == WEEK_DATA_CHART || m_chType == MONTH_DATA_CHART )
		{
			strResult = myTime.Format("%Y%m%d");
			strResult += "9999999999";
		}
		else if ( m_chType == MIN_DATA_CHART )
		{
			strResult = myTime.Format("%Y%m%d");
			strResult += m_ctlTime.Format("%H%M00");
		}
		else
		{
			//	리얼 체크가 풀려있을경우 현재 일자를 조회한다.
			myTime = CTime::GetCurrentTime();
			strResult = myTime.Format("%Y%m%d");
			strResult += "9999999999";
		}
	}
	else
	{
		//	리얼 체크가 풀려있을경우 현재 일자를 조회한다.
		myTime = CTime::GetCurrentTime();
		strResult = myTime.Format("%Y%m%d");
		strResult += "9999999999";
	}

	
	return strResult;
}

void CExtInputBarDlg::OnTimepickerchange(NMHDR* pNMHDR, LRESULT* pResult) 
{	
//	CString strDate;
//	CTime timeTime;
//	DWORD dwResult = m_datePicker.GetTime(timeTime);
//	if (dwResult == GDT_VALID)
//		strDate = timeTime.Format(_T("%Y%m%d"));
////@유진추가
//	if(m_bEndDate)
//	{
//		if (m_strEndDate != strDate)
//		{
//			m_strEndDate = strDate;
//		}
//		else
//		{
//			return;
//		}
//	}
//	else
//	{
//		m_strStartDate = strDate;
//	}	
//
//// --> [Edit]  강지원 2008/11/02		( 여기서 조회를 해버리면 조회 완료후 마지막 일자로 다시 설정되기때문에 날짜 변경시에는 TR을 날리지 않는다. )
////	if(m_bEndDate) // 종료일 모드이면 TR을 날린다.
////	{
//		GetParent()->SendMessage(UMSG_EXTINPUT_CHANGEDATE, 0, atoi(m_strEndDate));
////	}
//// <-- [Edit]  강지원 2008/11/02

	*pResult = 0;

}

void CExtInputBarDlg::SetExInputBarDlgTimeIndex(int nIndex)
{
	UpdateData(TRUE);

	if ( !m_bRealUpdate )
	{
		int nGab(0), nGab2(0);
		nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(nIndex, 1));

		if ( m_nTimeIndex > 0 )
			nGab2 = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(m_nTimeIndex, 1));
		else
		{
			ST_SELCHART* pSelChart = NULL;
			((CChartMng*)m_pChartMng)->GetpChartItemSelected()->GetSelChart( NULL, pSelChart);
			if ( pSelChart )
				m_nTimeIndex = pSelChart->m_nOrgTimeIndex;

			if ( pSelChart->m_nOrgTimeIndex == -1 )
				return;

			nGab2 = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(m_nTimeIndex, 1));
		}


		CTimeSpan cts = CTimeSpan(0, ( nGab - nGab2 ), 0, 0);     // 1일
	
		CString strDate1 = "", strDate2 = "";
		CTime myTime;
		m_datePicker.GetTime(myTime);

		strDate1 = m_ctlTime.Format("%Y%m%d");
		m_ctlTime = m_ctlTime + cts;
		strDate2 = m_ctlTime.Format("%Y%m%d");

		int nGabDate(0);
		if ( atol(strDate1) > atol(strDate2) )
			nGabDate = -1;
		else if ( atol(strDate1) < atol(strDate2) )
			nGabDate = 1;

		cts = CTimeSpan(nGabDate, 0, 0, 0);     // 1일


		myTime = myTime + cts;
		m_datePicker.SetTime(&myTime);	

		m_nTimeIndex = nIndex; 

		UpdateData(FALSE);

	}
}

void CExtInputBarDlg::OnBtOrder() 
{
	CString strCode = ((CChartMng*)m_pChartMng)->GetCurrentCode();
	CChartItem* pChartItem = NULL;
	pChartItem = ((CChartMng*)m_pChartMng)->GetpChartItemSelected();

	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(!pChartItem->m_mapChartItemRealData.Lookup(strCode, (LPVOID&)pChartItemRealData))
		return;

	int nLog(1);
	int nLogDispSize(0);
	int nPrecision(0);
	int nTickSize(1);
	int nValidPoint(0);
	int nMask(0);

	CString strPrice = "";
	strCode = CStdDialog::GetRealItemCode( ( LPCTSTR)strCode , TRUE );

	pChartItem->GetCodeConvInfo( strCode, nLog, nLogDispSize, nPrecision, nValidPoint, nMask, nTickSize);
	if ( nLog == 1 )
		strPrice = pChartItem->Gl_Format32String(atol(pChartItemRealData->m_strPrice), nLog, nLogDispSize, nPrecision, nMask);
	else if ( nLog == 8 || nLog == 32 )
		strPrice = pChartItem->Gl_Format32String(atol(pChartItemRealData->m_strPrice) * nTickSize, nLog, nLogDispSize, nPrecision, nMask);

	strPrice.Remove(',');	
	strPrice.Remove('.');
	strPrice.Remove('\'');

	int nOrderType = 0;
	char *pData = new char[100];
	memset(pData, 0x00, 100);
	sprintf(pData, "%s,%s,%d", strCode, strPrice, nOrderType);
	((CStdDialog*)GetParent())->SendMessage(UMSG_CREATESCREEN_LINK, 3, (LPARAM)pData);
	delete []pData;
}

void CExtInputBarDlg::SetOrderButtonEnable(BOOL bEnable)
{
	m_btOrder.EnableWindow(bEnable);
}

//<<< 20091111 by kmlee
void CExtInputBarDlg::OnCheckOrderLink ()
{
//<<20100308_JS.Kim 솔로몬 aaaaaaaaaaaaaaaaaaaaaaaa
	// FX차트 주문 연동 - onlyojt (20091204)
	BOOL bOrderLine = m_chkOrderLine.GetCheck();

	if(m_pChartMng->GetSafeHwnd())
	{
		if(bOrderLine == TRUE)
		{
			//AfxMessageBox("주문기능 사용시 차트영역에서 마우스 기능이 제한됩니다.");
			// FX차트 주문 연동 - onlyojt (20091204)
			((CStdDialog*)GetParent())->SetOrderLineInfo(bOrderLine);
		}

		m_pChartMng->SendMessage(UMSG_SETORDERLINK,(WPARAM)bOrderLine);	
	}
//>>
}
//>>>

// KTB 체결잔고표시 - ojtaso (20091019)
BOOL CExtInputBarDlg::GetChkOrder ()
{
	return m_chkOrderLine.GetCheck();
}


void CExtInputBarDlg::OnSelendokAccCtrl() 
{	
	m_strAccountNo = m_pAccountCtrl->GetAccNo();
	m_strAccountName = m_pAccountCtrl->GetAccName();
	m_strPassWord = m_pAccountCtrl->GetPswd();

	m_edAccPW.SetWindowText(m_strPassWord);

	SendAccountToPlatform(m_strAccountNo, m_strPassWord);

/*
	CString strAccount = m_strAccountNo;
	strAccount.Insert(5, "-");
	strAccount.Insert(3, "-");	
	m_pEditPass->SetText(m_strPassWord);
	
	SendCheckPassword();
	m_pEditPass->SetFocus();
	
	m_pStatic[STATIC_NAME]->SetText(m_strAccountName);
*/
}

void CExtInputBarDlg::GetAccountInfo(CString& strAccNo, CString& strAccName, CString& strAccPW) 
{
	strAccNo = m_pAccountCtrl->GetAccNo();
	strAccName = m_pAccountCtrl->GetAccName();
	m_edAccPW.GetWindowText(strAccPW);
}

void CExtInputBarDlg::SendAccountToPlatform(CString strAccountNo, CString strPassWord) 
{
	int nMajor = (int)::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, majorCC), (LPARAM)(LPCTSTR)"");

	CString strCode = ((CChartMng*)m_pChartMng)->GetCurrentCode();
	strCode = CStdDialog::GetRealItemCode(strCode);

	CString strTemp;
	strTemp.Format("%s\t%d\t%s\t%s\t%s", "SetAccount9065", nMajor, strAccountNo, strPassWord, strCode);
	::SendMessage(((CStdDialog*)GetParent())->m_hPlatform, WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)strTemp.operator LPCTSTR());
}

void CExtInputBarDlg::InitAccountCtrl(WPARAM wParam, LPARAM lParam)
{
	if(m_pAccountCtrl)
	{
		if( wParam == 0 && lParam == 0 )
			m_pAccountCtrl->Init();
		else
			m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
	}
}

// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
void CExtInputBarDlg::ChangeAnalToolFile(BOOL bSave)
{
	if(m_pChartMng->GetSafeHwnd())
	{
		if (bSave)
			m_pChartMng->SendMessage(UMSG_ALALTOOL_FILE_CHANGE, 0, 1);
		else
			m_pChartMng->SendMessage(UMSG_ALALTOOL_FILE_CHANGE, 0, 0);
	}
}
// 2011.01.31 by SYS 
