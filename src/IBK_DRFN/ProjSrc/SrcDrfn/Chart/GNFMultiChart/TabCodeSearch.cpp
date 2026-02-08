// TabCodeSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TabCodeSearch.h"
#include "./define/ChartMsgDef.h"
//#include "COMMON_BASE/WMUtil.h"
//#include "./define/IntDef.h"
#include "./define/DragNDropDefine.h"

#include "ChartItem.h"
#include "StdDialog.h"

//#include "../CommonTR/DrdsS31.h"
#include "../../inc/RealUpdate.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/IAUGwanMng.h"

#include "../chart_common/Grid/ColorTable.h"
#define ROW_HEIGHT 15
#define GRID_CALLPUT_WIDTH	33
#define GRID_CENTER_WIDTH	83
#define COL_CENTER 1
#define TITLE_ROW 1
enum	{EXERCISE, FIRST, SECOND, THIRD, FOURTH};	// 콜 옵션 종목 

#include <Winsock2.h>
#include ".\tabcodesearch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//@SolomonGNF  const UINT RMSG_GWANSIMREGISTER		=   ::RegisterWindowMessage("RMSG_GWANSIMREGISTER");
#define UDM_TREE_CLICK			WM_USER+612 // 4Byte Code Enter
#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )
#define UM_CHANGE_REGKW		   ( WM_USER+525 )		// 관심종목 그룹변경 처리용
//const UINT RMSG_CREATEOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_CREATEOPENSHAREDATA"));
/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch dialog
#define FIELD_PRICE			 "price"
#define FIELD_SIGN			 "sign"
#define FIELD_CHRATE		 "chrate"
#define FIELD_CHANGE		 "change"
#define TYPE_PRICE		0
#define TYPE_CODE		1
#define TYPE_CHGRATE	2
#define TYPE_CHANGE		3

#define AUTO_TIMER		4	// 돌려보기 Timer ID
#define Y_GAP			2
#define DEFAULT_SPLITTER_HEIGHT		300

#define IDC_GRIDCODE	6021

//////////////////////////////////////////////////////////////////////////
// COptionInfo
//COptionInfo::COptionInfo()
//{
//	m_strCode.Empty();
//	m_nCallPut = 0;
//	m_strEvtPrice.Empty();
//}

//////////////////////////////////////////////////////////////////////////
// CTabCodeSearch
CTabCodeSearch::CTabCodeSearch(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCodeSearch)
	//}}AFX_DATA_INIT
	m_nHeightOfSearch = DEFAULT_SPLITTER_HEIGHT;
	m_hRootItemUsr = NULL;		// 사용자 저장조건
	m_hRootItemProposeSpecial = NULL;	// 추천조건
	m_hRootItemProposeBuy = NULL;	// 추천조건
	m_hRootItemProposeSell = NULL;	// 추천조건
	m_hRootItemTheme = NULL;	// 시장테마
	m_hRootItemSise = NULL;		// 시세동향
	m_hRootItemIndicator = NULL;
	m_hRootItemInvestor = NULL;
	m_hRootItemFavorite = NULL;		// 관심종목
	m_hRootItemAccount = NULL;		// 보유종목
	m_hRootItemFromOutside = NULL;
	m_hRootItemCurrentTemp = NULL;
	m_hRootItemCurrent = NULL;
	m_bLBtnDown = FALSE;
	m_bRBtnDown = FALSE;
	m_bDragNDrop = FALSE;
	m_bDeletingTreeData = FALSE;

	m_bInsertingInListCode = FALSE;
	m_bUseTimer = FALSE;
	m_bIgnoreEditVolume = FALSE;
	m_nSortIndexOfList = 0;
	m_bSortTypeOfList = TRUE;
//	m_nAlarmFrom = 0;
//	m_nAlarmTo = 0;
//	m_bUseAlarmSound = FALSE;
//	m_bUseAlarmBkClr = FALSE;
	m_lQueryStatus = 0;
	m_bSkipSelChangedTreeEvent = FALSE;
	m_bQueryList = FALSE;
	m_pTreeListMng = NULL;

	m_dwTreeKey_gwan = 0; 
	m_dwTreeKey_master_stock = 0; 
	m_dwTreeKey_master_elw = 0; 
	m_dwTreeKey_master_jisu = 0; 
	m_dwTreeKey_master_future = 0;
	m_dwTreeKey_master_foreign = 0;
	m_dwTreeKey_oneclick = 0; 
//	m_dwTreeKey_userFind = 0; 
	m_dwTreeKey_FindSearch = 0; 
	m_pGwanTree = NULL;
	m_pMasterTree_stock = NULL;
	m_pMasterTree_elw = NULL;
	m_pMasterTree_Jisu = NULL;
	m_pMasterTree_future = NULL;
	m_pMasterTree_foreign = NULL;
	m_pOneClickTree = NULL;
//	m_pUserFindTree = NULL;
	m_pFindSearchTree = NULL;

	m_bAdvise = FALSE;

	m_bPlay = FALSE;
	m_pQueryData = NULL;
//@유진삭제
//	m_bGridExtend = FALSE;
//@유진삭제

	m_pMasterDataManager = NULL;
	m_pGridCtrl = NULL;

// --> [Edit]  강지원 2008/11/04
	m_nCurrIndex	= -1;
// <-- [Edit]  강지원 2008/11/04

//	m_listOptionInfo.RemoveAll();
}


void CTabCodeSearch::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearch)
//@유진삭제
//	DDX_Control(pDX, IDC_BTGRID_EXTEND, m_btnGridExtend);
//@유진삭제
	DDX_Control(pDX, IDC_CHK_REPEAT, m_chkRepeat);
	DDX_Control(pDX, IDC_BTRESETOCX, m_btnResetOcx);
	DDX_Control(pDX, IDC_BTPREV, m_btPrev);
	DDX_Control(pDX, IDC_BTNEXT, m_btNext);
//@유진삭제
//	DDX_Control(pDX, IDC_BTALL, m_btnAll);
//	DDX_Control(pDX, IDC_BTREGFAVOR, m_btnRegFavorite);
//	DDX_Control(pDX, IDC_BTN_RETRY, m_btnRetry);
//	DDX_Control(pDX, IDC_BTN_ADDAPPLY, m_btnAddApply);
//	DDX_Control(pDX, IDC_BTAPPLY, m_btnApply);
//@유진삭제
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
	//DDX_Control(pDX, IDC_TREESEARCH, m_treeSearch);	
//@유진삭제
//	DDX_Control(pDX, IDC_CMB_COLUMN_TYPE, m_cmbColumnType);
//@유진삭제
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BTPLAY, m_btPlay);
	DDX_Control(pDX, IDC_CMB_TIMER, m_cmbTime);
//@유진삭제
//	DDX_Control(pDX, IDC_CHK_COLUMN_EXTEND, m_chkColumnExtend);
//	DDX_Control(pDX, IDC_CMB_MULTI, m_cmbMultiItem);
//@유진삭제
}


BEGIN_MESSAGE_MAP(CTabCodeSearch, CTabDlg)
	//{{AFX_MSG_MAP(CTabCodeSearch)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREESEARCH, OnSelchangedTreesearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCODE, OnDblclkListcode)
//@유진삭제
//	ON_BN_CLICKED(IDC_BTAPPLY, OnBtapply)
//@유진삭제
	ON_BN_CLICKED(IDC_BTPORTFOLIO, OnBtportfolio)
//@유진삭제
//	ON_BN_CLICKED(IDC_BTREGFAVOR, OnBtregfavor)
//@유진삭제
	ON_BN_CLICKED(IDC_BTSEARCH, OnBtsearch)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCODE, OnBegindragListcode)
	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LISTCODE, OnBeginRdragListcode)
	ON_NOTIFY(NM_CLICK, IDC_LISTCODE, OnClickListcode)
//@유진삭제 ON_BN_CLICKED(IDC_BTALL, OnBtall)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCODE, OnRclickListcode)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNEXT, OnBtnext)
	ON_BN_CLICKED(IDC_BTPREV, OnBtprev)
	ON_BN_CLICKED(IDC_BTRESETOCX, OnBtresetocx)
//@유진삭제
//	ON_CBN_SELCHANGE(IDC_CMB_COLUMN_TYPE, OnSelchangeColumnType)
//	ON_BN_CLICKED(IDC_BTN_RETRY, OnBtnRetry)
//	ON_BN_CLICKED(IDC_BTN_ADDAPPLY, OnBtnAddapply)
//	ON_BN_CLICKED(IDC_BTGRID_EXTEND, OnBtgridExtend)
//@유진삭제
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )
	ON_REGISTERED_MESSAGE( RMSG_INTREQUEST, OnIntRequest )
	ON_REGISTERED_MESSAGE( RMSG_CONCERNHWND, OnResetAll )
	ON_BN_CLICKED(IDC_BTPLAY, OnBnClickedBtplay)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCODE, OnLvnItemchangedListcode)
//@유진삭제
//	ON_BN_CLICKED(IDC_CHK_COLUMN_EXTEND, OnBtnColumnExtend)
//@유진삭제
	ON_MESSAGE( WMU_OUTBAR_NOTIFY, OnOutBarNotify )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch message handlers
#define TIMEID_UPDATEWND 10
BOOL CTabCodeSearch::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

//@유진삭제
//	CString strImagePath = m_pParentDlg->m_strRootPath + "\\Image";
//	CString strImageUp;
//	CString strImageDown;
//	CString strImageDis;
//@유진삭제

	Init_Button();

	SetConfig();

// --> [Edit]  강지원 2008/07/31			( 관심종목등 종목 분류는 일딴 뺌.. )
//	InitTreeSearch();
// <-- [Edit]  강지원 2008/07/31
	UpdateTreeSearch();	

	// y-splitter
	m_ySplitterY1.BindWithControl(this, IDC_SPLITTERY1);
	m_ySplitterY1.SetMinHeight(30, 50);
//	20080709 ( 일단막음 )
//	m_ySplitterY1.AttachAsAboveCtrl(10102);
	m_ySplitterY1.AttachAsBelowCtrl(IDC_LISTCODE);
	m_ySplitterY1.RecalcLayout();

	// Grid 초기화
	InitMasterData();
	InitGrid();

	m_listCode.Init(m_pParentDlg,0);

//@유진삭제
//	m_JongStatusInTab.Create(CJongStatusInTab::IDD,this);
//	m_JongStatusInTab.ShowWindow(SW_SHOW);
//@유진삭제

//	IntRequest();
//	AdviseMain(1);
	
	m_listCode.RemoveAll();
//@유진삭제
//	m_JongStatusInTab.InitStatus();
//@유진삭제

	//Tree repaint 문제 때문에 추가
//	m_treeSearch.ShowWindow(SW_HIDE);
	m_listCode.ShowWindow(SW_HIDE);
	SetTimer(TIMEID_UPDATEWND, 10, NULL);

//@유진삭제
//	m_cmbColumnType.SetCurSel(0);
//
//	SetChartTypeControl();
//@유진삭제

	m_cmbTime.SetCurSel(0);		//004 돌려보기 default값 3초
	m_nCurrIndex = 0;

	int nID = 9;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(nID);
//@SolomonGNF 		pMng->SetRegGwansim(this, 1);
	}

	nID = 5;
	if(pGateMng)
		m_pMasterDataManager = (IMasterDataManager*)pGateMng->GetInterface(nID);
	
// --> [Edit]  강지원 2008/08/06		( 종목코드를 가져오기위해서... 일단 )
	SetFXMasterData();
// <-- [Edit]  강지원 2008/08/06

	m_listCode.SetHilightColor(RGB(255,255,255), RGB(151, 28, 30));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//@유진삭제
//// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//void CTabCodeSearch::SetChartTypeControl()
//{
//	if(((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//	{
//		m_cmbMultiItem.SetCurSel(0);
//	}
//	else
//	{
//		m_cmbMultiItem.ShowWindow(SW_HIDE);
//
//		m_btnApply.ShowWindow(TRUE);
//
//		if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == MULTI_CHART)
//			m_btnAddApply.ShowWindow(SW_SHOW);
//		else
//			m_btnAddApply.ShowWindow(SW_HIDE);
//	}
//}
//@유진삭제

void CTabCodeSearch::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	RecalcLayout();
}

void CTabCodeSearch::OnDestroy() 
{
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
//@SolomonGNF 		pMng->SetRegGwansim(this, 0);
	}

	if(m_pTreeListMng)
	{
		if(m_dwTreeKey_gwan)			m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_gwan);
		if(m_dwTreeKey_master_stock)	m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_master_stock);
		if(m_dwTreeKey_master_elw)		m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_master_elw);
		if(m_dwTreeKey_master_jisu)		m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_master_jisu);
		if(m_dwTreeKey_master_future)	m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_master_future);
		if(m_dwTreeKey_master_foreign)	m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_master_foreign);
		if(m_dwTreeKey_oneclick)		m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_oneclick);
//		if(m_dwTreeKey_userFind)		m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_userFind);
		if(m_dwTreeKey_FindSearch)		m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_FindSearch);
	}

	// TODO: Add your message handler code here
	KillTimer(100);
	SetConfig(FALSE);
	m_bDeletingTreeData = TRUE;
	DeleteChildItem(m_hRootItemUsr);
	DeleteChildItem(m_hRootItemProposeSpecial);
	DeleteChildItem(m_hRootItemProposeBuy);
	DeleteChildItem(m_hRootItemProposeSell);
	DeleteChildItem(m_hRootItemTheme);
	DeleteChildItem(m_hRootItemSise);
	DeleteChildItem(m_hRootItemInvestor);
	DeleteChildItem(m_hRootItemIndicator);
	DeleteChildItem(m_hRootItemFavorite);
	DeleteChildItem(m_hRootItemAccount); 
	m_bDeletingTreeData = FALSE;
//	AdviseMain(0);
	
	m_listCode.RemoveAll();

	if(m_strCodes.GetLength() > 0)
		MakeDrdsUnConnectUnAdvise(m_strCodes);

	RemoveAllTree();
	m_ImageListIcon.DeleteImageList();

	InitMasterData();
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	 if(m_pGridCtrl)
	 {
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	 } 

	CTabDlg::OnDestroy();
}	


CRect CTabCodeSearch::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(2,0,0,0);
	switch(nID)
	{
		//	case IDC_TREESEARCH:
		//		rect.top = rect.top;// + HEIGHTOFBUTTON*2;
		////		rect.top = rect.top;
		//		rect.bottom = rect.top + m_nHeightOfSearch;
		//		break;		
	case 10102 : 
//@유진삭제
//		if(m_bGridExtend)
//		{
//			rect.top = 0;
//			rect.bottom = 0;
//		}
//		else
//@유진삭제
		{
			rect.top = rect.top;// + HEIGHTOFBUTTON*2;
			rect.bottom = rect.top + m_nHeightOfSearch;
		}
		break;		
//@유진삭제
//	case IDC_BTALL:
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		rect.right = rect.left + 0;
//		break;	
//// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//	case IDC_BTAPPLY:
//		if(((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//			break;
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		rect.right = rect.left + 43;
//		break;
//	case IDC_CMB_MULTI:
//		if(!((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//			break;
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		rect.right = rect.left + 43 + 43 + 3;
//		break;		
//	case IDC_BTN_ADDAPPLY:
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		rect.left = rect.left + 43 + 3;
//		rect.right = rect.left + 43;
//		break;		
//	case IDC_BTREGFAVOR:
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		//rect.left = rect.left + 34 + 34 + 3*2;
//		rect.left = rect.right - (43 +1*2 + 20);
//		rect.right = rect.left + 43;
//		break;	
//@유진삭제
	case IDC_BTGRID_EXTEND:
//@유진삭제
//		if(m_bGridExtend)
//			rect.top = rect.top;
//		else
//@유진삭제
			rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.right  - 20;
		rect.right = rect.right;
		break;	
	case IDC_LISTCODE:
	case ID_GRIDCTRL:
//@유진삭제
//		if(m_bGridExtend)
//			rect.top = rect.top + HEIGHTOFBUTTON * 1 + GAP_TABDLG * 1;
//		else
//			rect.top = rect.top + m_nHeightOfSearch + HEIGHTOFBUTTON + GAP_TABDLG + Y_GAP * 3;
//@유진삭제
// --> [Edit]  강지원 2008/07/31	( 관심종목 트리 사이즈를 없앤다. )
//		rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG;// + Y_GAP;
//		rect.bottom = rect.bottom - GAP_TABDLG * 6;
		rect.top = rect.top + GAP_TABDLG;// + Y_GAP;
		// (2008/9/28 - Seung-Won, Bae) Hide Rotate UI
		//	rect.bottom = rect.bottom - GAP_TABDLG * 6;
// <-- [Edit]  강지원 2008/07/31
		break;		
	case IDC_SPLITTERY1:
//@유진삭제
//		if(m_bGridExtend)
//		{
//			rect.top = 0;
//			rect.bottom = 0;
//		}
//		else
//@유진삭제
// --> [Edit]  강지원 2008/07/31	( 스플릿터도 감추기 )
		{
//			rect.top = rect.top + m_nHeightOfSearch;
//			rect.bottom = rect.top + GAP_TABDLG;
			rect.top = 0;
			rect.bottom = 0;
		}
// <-- [Edit]  강지원 2008/07/31
		break;
//@유진삭제
//	case CJongStatusInTab::IDD:
//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG * 2 - HEIGHTOFJONGSTATUS ;
//		rect.bottom = rect.top + HEIGHTOFJONGSTATUS;
//		break;
//	case IDC_BTN_RETRY:
//		rect.left = rect.left + 64 + 43 + 3*2;
//		rect.right = rect.left + 43;
//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG * 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		break;
//@유진삭제
//	case IDC_BTREGFAVOR:
//		rect.left = rect.left + 64 + 3*1;
//		rect.right = rect.left + 43;
//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG * 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		break;
		/////////////////////////////////////////////////////////////////
//@유진삭제
//	case IDC_CMB_COLUMN_TYPE:
//		rect.left = rect.left;
//		rect.right = rect.left + 64;
//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG * 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		break;
//@유진삭제
		//	case IDC_RADIO_PRICE:
		//		rect.left = rect.left + GAP_TABDLG;
		//		rect.right = rect.left + 55;
		//		rect.top = rect.bottom - HEIGHTOFBUTTON * 3 - GAP_TABDLG * 2;
		//		rect.bottom = rect.top + HEIGHTOFBUTTON;
		//		break;
		//	case IDC_RADIO_CHANGE:	
		//		rect.left = rect.left + 55 + GAP_TABDLG;
		//		rect.right = rect.left + 55;
		//		rect.top = rect.bottom - HEIGHTOFBUTTON * 3 - GAP_TABDLG * 2;
		//		rect.bottom = rect.top + HEIGHTOFBUTTON;
		//		break;
		//	case IDC_RADIO_CHGRATE:	
		//		rect.left = rect.left + 55 * 2 + GAP_TABDLG;
		//		rect.right = rect.left + 55;
		//		rect.top = rect.bottom - HEIGHTOFBUTTON * 3 - GAP_TABDLG * 2;
		//		rect.bottom = rect.top + HEIGHTOFBUTTON;
		//		break;
		/////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////
		//	case IDC_BTSEARCH:
		//		rect.top = rect.bottom - HEIGHTOFBUTTON;
		//		rect.left = rect.right - 20 - 60  - 55;
		//		rect.right = rect.right - 60  - 55;
		//		break;		
		//	case IDC_BTPORTFOLIO:
		//		rect.top = rect.bottom - HEIGHTOFBUTTON;
		//		rect.left = rect.right - 34 - 55 - 55;
		//		rect.right = rect.right - 34 - 55;
		//		break;		
		//	case IDC_BTREGFAVOR:
	case IDC_BTRESETOCX:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 64 + 34 + GAP_TABDLG*2;
		rect.right = rect.left + 34;
		break;
		//case IDC_BTALL://	case IDC_BTREGFAVOR:			
		//		rect.top = rect.bottom - HEIGHTOFBUTTON;
		//		rect.left = rect.right - 34 - 20 - 53 - 34 - 10;
		//		rect.right = rect.right - 34 - 20 - 53 - 10;
		//		break;

	case IDC_BTPREV:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		//		rect.left = 5;
		rect.right = rect.left + 20;
		break;
	case IDC_BTNEXT:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 20;
		rect.right = rect.left + 20;
		break;
		//004 {{
	case IDC_CMB_TIMER:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 20 + 2 + 20;
		rect.right = rect.left + 48;
		break;
	case IDC_BTPLAY:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		rect.left = rect.left + 20 + 2 + 20 + 48 + 2;
		rect.right = rect.left + 20;
		break;
	case IDC_CHK_REPEAT:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		rect.left = rect.left + 20 + 2 + 20 + 48 + 20 + 4;
		rect.right = rect.left + 50;
		break;
		//004 }}
//@유진삭제
//	case IDC_CHK_COLUMN_EXTEND:							
//		//20061212 정유진수정
//		//grid column 확장btn 추가
//		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG * 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON;
//		rect.left = rect.left + 48 + 20;
//		rect.right = rect.left + 20;
//		break;
//		//20061212 수정 end
//@유진삭제
	default:
		break;
	}
	return rect;
}


void CTabCodeSearch::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listCode);
	
//	CRect rect;
//	if (m_listCode.GetSafeHwnd())
//	{
//		m_listCode.GetClientRect(&rect);
//		m_listCode.SetColumnWidth(0, rect.Width());
//	}

	if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
		RecalcLayoutCtrl(*m_pGridCtrl);
	//2006.08.26 by LYH UI 변경{{
	//RecalcLayoutCtrl(m_treeSearch);
// --> [Edit]  강지원 2008/07/31		( 관심종목등 종목 분류는 일딴 뺌.. )
//	RecalcLayoutCtrl(m_ctrlOutBar);

	//2006.08.26 by LYH UI 변경}}
	RecalcLayoutCtrl(m_ySplitterY1);
// <-- [Edit]  강지원 2008/07/31

//@유진삭제
//	RecalcLayoutCtrl(m_JongStatusInTab,CJongStatusInTab::IDD);	
//@유진삭제
	//RecalcLayoutCtrl(*(GetDlgItem(IDC_BTREGFAVOR)));
//@유진삭제
//	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//	RecalcLayoutCtrl(m_btnRegFavorite);
//	RecalcLayoutCtrl(m_cmbMultiItem);
//	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//	if(!((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//	{
//		RecalcLayoutCtrl(m_btnAddApply);
//		RecalcLayoutCtrl(m_btnApply);
//	}
//	RecalcLayoutCtrl(m_btnRetry);
//	RecalcLayoutCtrl(m_btnAll);
//@유진삭제
	RecalcLayoutCtrl(m_btPrev);
	RecalcLayoutCtrl(m_btNext);
	RecalcLayoutCtrl(m_btnResetOcx);
//@유진삭제
//	RecalcLayoutCtrl(m_cmbColumnType);
//@유진삭제
	RecalcLayoutCtrl(m_chkRepeat);
//004 {{
	RecalcLayoutCtrl(m_btPlay);
	RecalcLayoutCtrl(m_cmbTime);
//004 }}
//@유진삭제
//	RecalcLayoutCtrl(m_btnGridExtend);
//	//20061212 정유진수정
//	//grid column 확장btn 추가 
//	RecalcLayoutCtrl(m_chkColumnExtend);
//	//20061212 수정 end
//@유진삭제

//	if( m_listCode.GetSafeHwnd())
//	{
//		m_listCode.Invalidate();
//		m_listCode.UpdateWindow();
//	}
}


LONG CTabCodeSearch::OnWmSplitterMoved( UINT wParam, LONG lParam )
{
	if(IDC_SPLITTERY1==lParam)
	{
		m_nHeightOfSearch += wParam;
		RecalcLayout();
	}
	return 1L;
}

void CTabCodeSearch::InitTreeSearch()
{
// --> [Edit]  강지원 2008/11/28	( 메모리가 샘... World-On에서는 사용안하는 컨트롤 )
/*
	CBitmap bitmap;
	CString strImageDir;
	strImageDir.Format("%s\\folder.bmp", m_pParentDlg->m_szImageDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageListIcon.Create(18, 18, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageListIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();

	//20061213 정유진 수정 
	//LeftBar 트리뷰헤더 이미지 수정
//@유진삭제 
//	strImageDir = "";
//	strImageDir.Format("%s\\outbar_icon.bmp", m_pParentDlg->m_szImageDir);
//	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
//	m_ImageOutbarIcon.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1);
//	m_ImageOutbarIcon.Add(&bitmap, RGB(255, 0, 255));
//	bitmap.DeleteObject();
//@유진삭제 
	//20061213 수정 end

	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		m_pTreeListMng = (ITreeListManager2*)pGateMng->GetInterface(UUID_ITreeListManager2);
		if(!m_pTreeListMng)
			return;

		CInputCodeList inData;
		//inData.m_pTreeCtrl = &m_treeSearch;
		//inData.m_szSection = "CHARTTREELIST";

		//m_dwTreeKey = m_pTreeListMng->LoadTreeList(&inData, "CHARTTREELIST");
		//m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);


		DWORD dwf = CGfxOutBarCtrl::fDragItems|CGfxOutBarCtrl::fEditGroups|CGfxOutBarCtrl::fEditItems|CGfxOutBarCtrl::fRemoveGroups|
			CGfxOutBarCtrl::fRemoveItems|CGfxOutBarCtrl::fAddGroups|CGfxOutBarCtrl::fAnimation|CGfxOutBarCtrl::fSelHighlight;
		//m_ctrlOutBar.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0,0,100,100), this, 10102, dwf);
		m_ctrlOutBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,100,100), this, 10102, dwf);
		//2006.12.07 정유진 수정
		//LeftBar 사이즈 변경시 이미지 잘림현상 수정
		m_ctrlOutBar.SetFolderImageList(m_pIL_W151H21, 0, 2, 3);
		//m_ctrlOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 1, 3);
		//2006.12.07 정유진 수정end
		m_ctrlOutBar.SetOwner(this);

		m_ctrlOutBar.cr3dFace =  RGB(191,211,233);
		m_ctrlOutBar.cr3dUnFace = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);

		if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == MULTI_CHART)
		{
			m_pGwanTree = new CTrTreeCtrl;
			m_pGwanTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10111);
			m_pGwanTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("관심그룹", m_pGwanTree);

			m_pMasterTree_stock = new CTrTreeCtrl;
			m_pMasterTree_stock->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10112);
			m_pMasterTree_stock->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("주식", m_pMasterTree_stock);

			m_pMasterTree_elw = new CTrTreeCtrl;
			m_pMasterTree_elw->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10113);
			m_pMasterTree_elw->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("ELW", m_pMasterTree_elw);

			m_pMasterTree_Jisu = new CTrTreeCtrl;
			m_pMasterTree_Jisu->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10114);
			m_pMasterTree_Jisu->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("업종", m_pMasterTree_Jisu);

			m_pMasterTree_future = new CTrTreeCtrl;
			m_pMasterTree_future->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10115);
			m_pMasterTree_future->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("선물/옵션", m_pMasterTree_future);

			m_pMasterTree_foreign = new CTrTreeCtrl;
			m_pMasterTree_foreign->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10116);
			m_pMasterTree_foreign->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("해외", m_pMasterTree_foreign);

			m_pOneClickTree = new CTrTreeCtrl;
			m_pOneClickTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10117);
			m_pOneClickTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("빠른종목검색", m_pOneClickTree);

//			m_pUserFindTree = new CTrTreeCtrl;
//			m_pUserFindTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10118);
//			m_pUserFindTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
//			m_ctrlOutBar.AddFolderBar("종목검색", m_pUserFindTree);

			m_pFindSearchTree = new CTrTreeCtrl;
			m_pFindSearchTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10119);
			m_pFindSearchTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
			m_ctrlOutBar.AddFolderBar("사용자조건검색", m_pFindSearchTree);
		}
		else if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == SINGLE_CHART)
		{
//			m_pGwanTree = new CTrTreeCtrl;
//			m_pGwanTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10111);
//			m_pGwanTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
//			m_ctrlOutBar.AddFolderBar("관심그룹", m_pGwanTree);

			if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == STOCK_CHART)
			{
				m_pMasterTree_stock = new CTrTreeCtrl;
				m_pMasterTree_stock->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10112);
				m_pMasterTree_stock->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("주식", m_pMasterTree_stock);

				m_pMasterTree_elw = new CTrTreeCtrl;
				m_pMasterTree_elw->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10113);
				m_pMasterTree_elw->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("ELW", m_pMasterTree_elw);

				m_pOneClickTree = new CTrTreeCtrl;
				m_pOneClickTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10117);
				m_pOneClickTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("빠른종목검색", m_pOneClickTree);
				
				m_pFindSearchTree = new CTrTreeCtrl;
				m_pFindSearchTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10119);
				m_pFindSearchTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("사용자조건검색", m_pFindSearchTree);
			}

			if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == UPJONG_CHART)
			{
				m_pMasterTree_Jisu = new CTrTreeCtrl;
				m_pMasterTree_Jisu->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10114);
				m_pMasterTree_Jisu->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("업종", m_pMasterTree_Jisu);
			}
			
			if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == FUTOPT_CHART)
			{
				m_pMasterTree_future = new CTrTreeCtrl;
				m_pMasterTree_future->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10115);
				m_pMasterTree_future->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
				m_ctrlOutBar.AddFolderBar("선물/옵션", m_pMasterTree_future);
			}

//			m_pOneClickTree = new CTrTreeCtrl;
//			m_pOneClickTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10117);
//			m_pOneClickTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
//			m_ctrlOutBar.AddFolderBar("빠른종목검색", m_pOneClickTree);
//
////			m_pUserFindTree = new CTrTreeCtrl;
////			m_pUserFindTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10118);
////			m_pUserFindTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
////			m_ctrlOutBar.AddFolderBar("종목검색", m_pUserFindTree);
//
//			m_pFindSearchTree = new CTrTreeCtrl;
//			m_pFindSearchTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10119);
//			m_pFindSearchTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
//			m_ctrlOutBar.AddFolderBar("사용자조건검색", m_pFindSearchTree);
		}

		m_ctrlOutBar.SetSelFolder(0);
	}
	else
	{
		//		AfxMessageBox("InitTreeSearch pGate NULL");
	}
*/
// <-- [Edit]  강지원 2008/11/28
}

void CTabCodeSearch::UpdateTreeSearch()
{
//	char szConditionItem[100];
//	char szConditionItemPath[256];
//	char *pszConditionList;	
//	char *pszConditionListPath;	
	CString strTemp;
	int nPosBuy = -1;
	int nPosSell = -1;
	HTREEITEM hItem = NULL;
	CString *pstr = NULL;

	// nType => 0:추천, 1:사용자, 2:Language
//	int nCnt = m_pParentDlg->MyNetFindLibGetConditionList(1,FALSE,pszConditionList,pszConditionListPath);
//	for(int i=0;i<nCnt;i++)
//	{
//		memcpy(szConditionItem,pszConditionList + 100*i,100);
//		memcpy(szConditionItemPath,pszConditionListPath + 256*i,256);
//		hItem = m_treeSearch.InsertItem(szConditionItem,m_hRootItemUsr);		
//		pstr = new CString;
//		pstr->Format("%s",szConditionItemPath);		
//		m_treeSearch.SetItemData(hItem,(DWORD)pstr);
//	}
//	if(nCnt)
//	{
//		delete [] pszConditionList;
//		delete [] pszConditionListPath;
//	}

	// nType => 0:추천, 1:사용자, 2:Language
//	nCnt = m_pParentDlg->MyNetFindLibGetConditionList(0,FALSE,pszConditionList,pszConditionListPath);
//	for(i=0;i<nCnt;i++)
//	{
//		memcpy(szConditionItem,pszConditionList + 100*i,100);
//		memcpy(szConditionItemPath,pszConditionListPath + 256*i,256);
//		strTemp = szConditionItemPath;
//		nPosBuy = strTemp.Find("\\Buy\\");
//		nPosSell = strTemp.Find("\\Sell\\");
//		if(nPosBuy!=-1)
//		{
//			hItem = m_treeSearch.InsertItem(szConditionItem,m_hRootItemProposeBuy);
//		}
//		else if(nPosSell!=-1)
//		{
//			hItem = m_treeSearch.InsertItem(szConditionItem,m_hRootItemProposeSell);
//		}
//		else
//		{
//			hItem = m_treeSearch.InsertItem(szConditionItem,m_hRootItemProposeSpecial);
//		}		
//		pstr = new CString;
//		pstr->Format("%s",szConditionItemPath);		
//		m_treeSearch.SetItemData(hItem,(DWORD)pstr);
//	}
//	if(nCnt)
//	{
//		delete [] pszConditionList;
//		delete [] pszConditionListPath;
//	}
	
//	m_lQueryStatus++; // to receive account info By Window message UMSG_SETACCOUNTINFO..
//	UpdateItemTheme();
//	UpdateItemSise('1');
//	UpdateItemSise('2');
//	UpdateItemSise('3');
	
}

void CTabCodeSearch::UpdateItemSise(char chType)
{
/*
	//========= m_hRootItemSise
	D20010_I st20010;
	memset(&st20010,0x20,sizeof(st20010));
	st20010.dtGb[0] = '0';	// 0:전체 1:검색 2:종목 
	st20010.Gb[0] = chType;	// 1:시세 2:투자자 3:지표신호 	
	stSend2Server st;
	st.m_pWnd = this;
	st.m_pBuffer = &st20010;
	st.m_lLength= sizeof(st20010);
	st.m_strTRCode = "20010";	
	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
	m_lQueryStatus++;
*/
}


void CTabCodeSearch::ClearAllTreeSearch()
{
	//m_treeSearch.DeleteAllItems();
	m_hRootItemUsr = NULL;		// 사용자 저장조건
	m_hRootItemProposeSpecial = NULL;	// 추천조건
	m_hRootItemProposeBuy = NULL;	// 추천조건
	m_hRootItemProposeSell = NULL;	// 추천조건
	m_hRootItemTheme = NULL;	// 시장테마
	m_hRootItemSise = NULL;		// 시세동향
}

void CTabCodeSearch::DeleteChildItem(HTREEITEM hItem)
{
return;
}

void CTabCodeSearch::RequestJongmokCodesInList(HTREEITEM hItemGrp)
{
//	m_listCode.RemoveAll();
//	
//	m_hRootItemCurrentTemp = hItemGrp;
//	m_JongStatusInTab.InitStatus();
//
//	if(!hItemGrp) return;
//
//	m_strFavoriteGrp = "";
//	CString strTreeItem;
//
//	CString *pstr = (CString *)m_treeSearch.GetItemData(hItemGrp);
//	GetCodeData00410(pstr);
}

void CTabCodeSearch::OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
////	if(!m_bDeletingTreeData)
////	{
////		m_strSelectedItemName = m_treeSearch.GetItemText(pNMTreeView->itemNew.hItem);
////		HTREEITEM hParentItem = m_treeSearch.GetParentItem(pNMTreeView->itemNew.hItem);
////		m_strSelectedParentItemName = m_treeSearch.GetItemText(hParentItem);
////		RequestJongmokCodesInList(pNMTreeView->itemNew.hItem);
////		m_bDeletingTreeData = TRUE;
////		//return;
////	}
//
//	HTREEITEM hItem = m_treeSearch.GetSelectedItem();// .m_hSelTreeItem;
//	if(hItem == NULL)
//		return;
//
//	CItemLongData* pLData = (CItemLongData*)m_treeSearch.GetItemData(hItem);
//	if(pLData)
//	{
//		TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
//		long dwReserved=0;
//		if(m_pTreeListMng)
//			m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'B', '1', dwReserved);
//	}
//
//	*pResult = 0;
}

/*
void CTabCodeSearch::GetCodeData80000(CString *pStr)
{
	char *pszFindDefine;	
	User_Finder *pStUser_Finder;
	int nIndex = -1;
	CString strFullFilePath = m_pParentDlg->m_strRootPath;
	strFullFilePath.TrimRight('\\');
	strFullFilePath  = strFullFilePath + *pStr;
	int szData = m_pParentDlg->MyNetFindLibGetFindDefine_TR((char*)(LPCTSTR)strFullFilePath,strFullFilePath.GetLength(),nIndex, pszFindDefine);
	if(szData>0)
	{
		pStUser_Finder = (User_Finder*)pszFindDefine;
		pStUser_Finder->Sort_flag = 1;
		//0 : 사용안함
		//+1 : 종목코드(오름)
		//+2 : 종목명(오름)
		//+3 : 등락률(오름)
		//+4 : 대비(오름)
		//+5 : 거래량(오름)
		//+6 : 현재가(오름)
		//-1 : 종목코드(내림)
		//-2 : 종목명(내림)
		//-3 : 등락률(내림)
		//-4 : 대비(내임)
		//-5 : 거래량(내임)
		//-6 : 현재가(내임)
		stSend2Server st;
		st.m_pWnd = this;
		st.m_pBuffer = pszFindDefine;
		st.m_lLength= szData;
		st.m_strTRCode = "80000";
		m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
		delete [] pszFindDefine;
	}
}*/


void CTabCodeSearch::GetCodeData20001(CString *pStr)
{
//	CString strGrptmcd;
//	strGrptmcd.Format("%2d",dwCode);
/*
	D20001_I st20001_i;
	memset(st20001_i.grptmcd,0x20,sizeof(st20001_i.grptmcd));
	memcpy(st20001_i.grptmcd,(LPCTSTR)*pStr,sizeof(st20001_i.grptmcd));
	st20001_i.filtercode[0] = '1';
	memset(st20001_i.limitvol,0x20,sizeof(st20001_i.limitvol));
	st20001_i.limitvol[0] = '0';
	
	stSend2Server st;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)&st20001_i;
	st.m_lLength= sizeof(st20001_i);
	st.m_strTRCode = "20001";
	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
*/
}

void CTabCodeSearch::GetCodeData20011(CString *pStr)
{
//	CString strGrptmcd;
//	strGrptmcd.Format("%2d",dwCode);
/*
	D20011_I st20011_i;
	memcpy(st20011_i.grptmcd,(LPCTSTR)*pStr,sizeof(st20011_i.grptmcd));
	st20011_i.filtercode[0] = '1';
	memset(st20011_i.limitvol,0x20,sizeof(st20011_i.limitvol));
	st20011_i.limitvol[0] = '0';
	
	stSend2Server st;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)&st20011_i;
	st.m_lLength= sizeof(st20011_i);
	st.m_strTRCode = "20011";
	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
*/
}

/*
void CTabCodeSearch::SetRealPriceData(char *pData, int nSize)
{
	char szTemp0[16];
	char szTemp1[16];
	char szTemp2[16];
	char szTemp3[16];
	char szTemp4[16];
	char chSign;
	CString strPriceOld;
	CString strChangeOld;
	CString strChgrateOld;
	CString strVolumeOld;
	char	chSignOld;
	double dChgRateOld = 0;
	double dChgRateNew = 0;
	STPriceData* Op = (STPriceData*) pData;
	MEMCPYWITHNULL(szTemp0,Op->shcode);		// 종목코드
	MEMCPYWITHNULL(szTemp1,Op->price);		// 현재가
	MEMCPYWITHNULL(szTemp2,Op->change);		// 대비
	MEMCPYWITHNULL(szTemp3,Op->chgrate);	// 대비율
	MEMCPYWITHNULL(szTemp4,Op->volume);	// 거래량	
	chSign = Op->sign[0];
	int nIndex = m_listCode.FindCode(szTemp0);
	if(nIndex>=0)
	{
		m_listCode.GetCodeData(nIndex,strPriceOld,strChangeOld,strChgrateOld,strVolumeOld,chSignOld);
//		m_listCode.UpdateCodeData(nIndex,szTemp0,szTemp1,szTemp2,szTemp3,szTemp4,chSign);		
		dChgRateOld = atof(strChgrateOld);
		dChgRateNew = atof(szTemp3);
		m_JongStatusInTab.UpdateStatus(chSign,chSignOld,dChgRateNew-dChgRateOld);
	}
}*/


LRESULT CTabCodeSearch::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_RECEIVEFROMSERVER:
		{
			stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;
			ReceiveData(pStR->m_strTRCode,pStR->m_pBuffer,pStR->m_dwLength);
		}
		break;
//@유진삭제
//	case UMSG_SENDJCODECHECKED:
//		{
//			m_listCode.CheckJongmokCode(wParam,(LPCTSTR)lParam);
//		}
//		break;
//@유진삭제
	case UMSG_GETJCODECHECKEDFROMLEFTBAR:
		{
			BOOL* pbCheck = (BOOL*)wParam;
			*pbCheck = m_listCode.GetCheckByJongmokCode((LPCTSTR)lParam);
		}
		break;
	case UMSG_SENDJCODEUNHIGHLIGHTED:
		{
			m_listCode.UnHighlightByJongmokCode((LPCTSTR)wParam);
		}
		break;
	case UMSG_GETJCODECHECKEDFROMCODESRC:
		{
			m_listCode.GetListOfChecked((CStringList *)wParam);
		}
		break;
	case UMSG_SETACCOUNTINFO:
		{
/*
			CString strAccountInfo = (LPCTSTR)wParam;
			if(strAccountInfo.GetLength())
			{
				SetAccountInfo(strAccountInfo);
			}	
			m_lQueryStatus--;
			RequestSavedQuery();
*/
		}
		break;
	case UMSG_SETINITIALCHARTDATA:
		{
			CString strComment = m_listCode.GetCommentAboutHighlight();
			m_pMultiChartWnd->SendMessage(UMSG_SETHIGHLIGHTINFO,(WPARAM)(LPCTSTR)strComment);
		}
		break;
	case UDM_TREE_CLICK:
		{
			CItemLongData* pLData = (CItemLongData*)lParam;
			m_pQueryData = pLData;
			if(pLData)
			{
				TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
				
				ChangeListOrGridCtrl(pLData->m_szSection);
				long dwReserved=0;
				if(m_pTreeListMng)
					m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);

//@유진삭제
//				//20061218 정유진 수정
//				//관심종목 선택인 경우만 실시간 버튼 활성화 나머지 항목 선택시 재조회 버튼으로 활성화
//				CString strCompare;
//				strCompare.Format("%s", pLData->m_szMethod);
//
//				if(strCompare.Compare("USERTR") == 0)
//				{
//					m_btnRetry.SetBtnTitle("실시간", "실시간");
//					m_btnRetry.EnableWindow(FALSE);
//				}
//				else
//				{
//					m_btnRetry.SetBtnTitle("재조회", "재조회");
//					m_btnRetry.EnableWindow(TRUE);
//				}
//				//20061218 수정 END
//@유진삭제
			}
			break;
		}
	case UWM_RECEIVE_ITRQUERY:
		{
			if(wParam == 2)
			{
				IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
				IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
				long lGroup = (long)lParam;

				CStringArray saCode;
				CUIntArray iaMarketGb;
				m_listCode.GetCheckedCodes(&saCode, &iaMarketGb);

				CStringArray strCodes;
				for (int nIndex = 0; nIndex < saCode.GetSize(); nIndex++)
				{
					strCodes.Add(saCode.GetAt(nIndex));
				}
//@유진삭제
//				pMng->AddGwansimtem_New(lGroup, strCodes, (long)m_hWnd);
//@유진삭제
			}
			break;
		}
	case UM_CHANGE_REGKW:
		{
			CInputCodeListEx inData;
			if(m_dwTreeKey_gwan != 0)
			{
				m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_gwan);
				m_pGwanTree->DeleteAllItems();
			}

			inData.m_pTreeCtrl = m_pGwanTree;
			inData.m_szSection = "CHARTTREELIST";
			m_dwTreeKey_gwan = m_pTreeListMng->LoadTreeList(&inData, "관심그룹");
			m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
			break;
		}
	}

	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;
		
		if(nCode == GVN_SELCHANGING)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(!bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// 거래대상종목이 아닌경우 return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
					}
					return TRUE;
				}	
			}
		}
		else if(nCode == NM_DBLCLK)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
				if(bInsertMode)
				{
					CString str;
					str = m_pGridCtrl->GetItemText(pNmgv->iClickRow,pNmgv->iClickCol);

					// 거래대상종목이 아닌경우 return
					if (str.Find("X") >= 0)						
						return TRUE;

					CString strCodeSelected;
					GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
					if(!strCodeSelected.IsEmpty())
					{
						((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
					}
					return TRUE;
				}	
			}
		}
	}
	return CTabDlg::WindowProc(message, wParam, lParam);
}

void CTabCodeSearch::ReceiveData(LPCSTR szTR, LPVOID aTRData, DWORD dwTRDateLen)
{
	if(!dwTRDateLen)	return;
	LPTSTR pData = (LPTSTR)aTRData;
	CString strTRCode = szTR;
	if(strTRCode=="80000")
	{
//		if(SZ_OutPacket>=dwTRDateLen) return;
//		OutPacket			stOutPacket;
//		OutListPacket		stOutListPacket;
//		memcpy(&stOutPacket, pData, SZ_OutPacket);
//	
//		stOutPacket.TotalJongCnt = ntohl(stOutPacket.TotalJongCnt);
//		stOutPacket.OutFieldCnt = ntohl(stOutPacket.OutFieldCnt);
//		stOutPacket.OutListPacketSize = ntohl(stOutPacket.OutListPacketSize);
//		stOutPacket.FindTime = ntohl(stOutPacket.FindTime);			
//
//		long lOutFieldCnt = stOutPacket.OutFieldCnt;			  
//		long lOutListPacketSize = stOutPacket.OutListPacketSize;  
//		long lFindTime = stOutPacket.FindTime;					  
//		
//
//		pData+=SZ_OutPacket;
//		dwTRDateLen -= SZ_OutPacket;
//		CString strResultField;
//		CString strItemCode, strItemName, strItemPrice, strItemDaebi, strItemDaebiRate, strItemVolume;
//		long	lUpLimit = 0;
//		long	lUp = 0;
//		long	lSteady = 0;
//		long	lDown = 0;
//		long	lDownLimit = 0;
//		long	lIndexCnt = 0;
//		double dDaebiRateTotal = 0;
//		CString strVolumeLimitation;
////		m_editVolume.GetWindowText(strVolumeLimitation);
//		double dVolumeLimitation = atof(strVolumeLimitation)*1000;
//		m_bInsertingInListCode = TRUE;
//		for (int nIndex = 0; nIndex < stOutPacket.TotalJongCnt; nIndex++) 
//		{
//			memcpy(&stOutListPacket, pData + stOutPacket.OutListPacketSize*nIndex, stOutPacket.OutListPacketSize);
//			stOutListPacket.ResultField[0] = ntohl(stOutListPacket.ResultField[0]);
//			stOutListPacket.ResultField[1] = ntohl(stOutListPacket.ResultField[1]);
//			stOutListPacket.ResultField[2] = ntohl(stOutListPacket.ResultField[2]);
//			stOutListPacket.ResultField[3] = ntohl(stOutListPacket.ResultField[3]);	// volume
//
//			if(stOutListPacket.ResultField[3]<dVolumeLimitation) continue;
//
//			switch(stOutListPacket.Daebi)
//			{
//			case '1':	// UpLimit
//				lUpLimit++;
//				break;
//			case '2':	// Up
//				lUp++;
//				break;
//			case '3':	// Steady
//				lSteady++;
//				break;
//			case '5':	// Down
//				lDown++;
//				break;
//			case '4':	// DownLimit
//				lDownLimit++;
//				break;	
//			}
//
//			// Code
//			strItemCode = stOutListPacket.Code;
//			strItemCode = strItemCode.Left(6);
//
//			// Name
//			strItemName = stOutListPacket.KoreanName;
//			strItemName.TrimRight(" ");
//
//			// Price			
//			strItemPrice.Format("%d",stOutListPacket.ResultField[0]);
//
//			//			
//			strItemDaebi.Format("%d",stOutListPacket.ResultField[1]);
//			strItemDaebi.TrimRight(" ");
//
//			
//			strItemDaebiRate.Format("%d.%02d",abs(stOutListPacket.ResultField[2]/100),abs(stOutListPacket.ResultField[2]%100));
//			if(stOutListPacket.ResultField[2]<0)
//			{
//				strItemDaebiRate = '-' + strItemDaebiRate;
//			}
//			
//			dDaebiRateTotal += (double)stOutListPacket.ResultField[2];
//
//			strItemVolume.Format("%d",stOutListPacket.ResultField[3]);
//			strItemVolume.TrimRight(" ");
//
//			if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//			{
//				m_listCode.UpdateCodeData(strItemCode,strItemName,strItemPrice,strItemDaebi,strItemDaebiRate,strItemVolume,stOutListPacket.Daebi,lIndexCnt);					
//			}
//			else
//			{
//				m_listCode.InsertCodeData(strItemCode,strItemName,strItemPrice,strItemDaebi,strItemDaebiRate,strItemVolume,stOutListPacket.Daebi);
//			}
//			
//			lIndexCnt++;
////			pData = &pData[stOutPacket.OutListPacketSize];
////			dwTRDateLen -= stOutPacket.OutListPacketSize;
//		}
//		if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//		{
//			m_listCode.RemoveNoUpdateItem();					
//		}
//		else
//		{
//			m_listCode.InitSortIndex();
//		}
//		m_bInsertingInListCode = FALSE;
//
//		m_JongStatusInTab.SetStatus(lUpLimit,lUp,lSteady,lDown,lDownLimit,lIndexCnt,dDaebiRateTotal/100);
	}
	else if(strTRCode=="05010")
	{
//		TR_05010_O	stTr05010O;
//		TR_05010_OS	stTr05010Os;
//		long lOffset = 0;
//		long lCnt = 0;
//		char szTemp[128];
//		char szCode[64];
//		char szName[64];
//		char chSign;
//		CString strPrice;
//		CString strChange;
//		CString strChgRate;
//		CString strVolume;
//
//		long	lUpLimit = 0;
//		long	lUp = 0;
//		long	lSteady = 0;
//		long	lDown = 0;
//		long	lDownLimit = 0;
//		long	lIndexCnt = 0;
//		double dDaebiRateTotal = 0;
//
//		MEMCPYBYTARSIZE2(stTr05010O,pData,lOffset);
//		MEMCPYWITHNULL(szTemp,stTr05010O.cnt);
//		lCnt = atol(szTemp);
//		CString strVolumeLimitation;
////		m_editVolume.GetWindowText(strVolumeLimitation);
//		double dVolumeLimitation = atof(strVolumeLimitation)*1000;
//		m_bInsertingInListCode = TRUE;
//		for(long lPos=0;lPos<lCnt;lPos++)
//		{
//			MEMCPYBYTARSIZE2(stTr05010Os,pData,lOffset);
//			MEMCPYWITHNULL(szCode,stTr05010Os.jg_code);
//			MEMCPYWITHNULL(szName,stTr05010Os.jg_name);
//			MEMCPYWITHNULL(szTemp,stTr05010Os.price);
//			strPrice = szTemp;
//			MEMCPYWITHNULL(szTemp,stTr05010Os.change);
//			strChange = szTemp;
//			MEMCPYWITHNULL(szTemp,stTr05010Os.change_rate);
//			strChgRate = szTemp;
//			MEMCPYWITHNULL(szTemp,stTr05010Os.volume);
//			strVolume = szTemp;
//			chSign = stTr05010Os.sign[0];
//			
//			if(atof(szTemp)<dVolumeLimitation) continue;
//
//			switch(chSign)
//			{
//			case '1':	// UpLimit
//				lUpLimit++;
//				break;
//			case '2':	// Up
//				lUp++;
//				break;
//			case '3':	// Steady
//				lSteady++;
//				break;
//			case '5':	// Down
//				lDown++;
//				strChange = '-' + strChange;
//				break;
//			case '4':	// DownLimit
//				lDownLimit++;
//				strChange = '-' + strChange;
//				break;	
//			}
//			dDaebiRateTotal += (double)atof(strChgRate);
//			
//			if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//			{
//				m_listCode.UpdateCodeData(	szCode,
//											szName,
//											strPrice,
//											strChange,
//											strChgRate,
//											strVolume,
//											chSign,
//											lIndexCnt);
//			}
//			else
//			{
//				m_listCode.InsertCodeData(
//					szCode,
//					szName,
//					strPrice,
//					strChange,
//					strChgRate,
//					strVolume,
//					chSign);
//			}
//			lIndexCnt++;
//		}
//		if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//		{
//			m_listCode.RemoveNoUpdateItem();					
//		}
//		else
//		{
//			m_listCode.InitSortIndex();
//		}
//
//		m_bInsertingInListCode = FALSE;
//		m_JongStatusInTab.SetStatus(lUpLimit,lUp,lSteady,lDown,lDownLimit,lIndexCnt,dDaebiRateTotal/100);
	}
	else if(strTRCode=="00410")
	{
//		TR_00410_O st_00410_o;
//		memcpy(&st_00410_o, pData, dwTRDateLen);
//
//		char chTemp[64];
//		CString strItemCode, strItemName, strItemPrice, strItemDaebi, strItemDaebiRate, strItemVolume;
//		long	lUpLimit = 0;
//		long	lUp = 0;
//		long	lSteady = 0;
//		long	lDown = 0;
//		long	lDownLimit = 0;
//		long	lIndexCnt = 0;
//		double dDaebiRateTotal = 0;
//		MEMCPYWITHNULL(chTemp,st_00410_o.grid_len);
//		long lCnt = atol(chTemp);
//		
//		HTREEITEM hItem = NULL;
//		CString strVolumeLimitation;
////		m_editVolume.GetWindowText(strVolumeLimitation);
//		double dVolumeLimitation = atof(strVolumeLimitation)*1000;
//		m_bInsertingInListCode = TRUE;
//		for(long lPos=0;lPos < lCnt;lPos++)
//		{
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].code);
//			strItemCode = chTemp;
//			strItemCode.TrimRight();
//			if(strItemCode.IsEmpty()) continue;
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].name);
//			strItemName = chTemp;
//			strItemName.TrimRight();
//			if(strItemName.IsEmpty()) continue;
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].price);
//			strItemPrice = chTemp;
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].change);
//			strItemDaebi = chTemp;
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].rate);
//			strItemDaebiRate = chTemp;
//			dDaebiRateTotal += atof(strItemDaebiRate);
//			MEMCPYWITHNULL(chTemp,st_00410_o.grid[lPos].volume);
//			strItemVolume = chTemp;
//			if(atof(chTemp)<dVolumeLimitation) continue;
//			
//			switch(st_00410_o.grid[lPos].sign[0])
//			{
//			case '1':	// UpLimit
//				lUpLimit++;
//				strItemDaebi.TrimLeft('0');
//				break;
//			case '2':	// Up
//				lUp++;
//				strItemDaebi.TrimLeft('0');
//				break;
//			case '3':	// Steady
//				lSteady++;
//				strItemDaebi = '0';
//				break;
//			case '5':	// Down
//				lDown++;
//				strItemDaebi.TrimLeft('-');
//				strItemDaebi.TrimLeft('0');
//				strItemDaebi = '-' + strItemDaebi;
//				break;
//			case '4':	// DownLimit
//				lDownLimit++;
//				strItemDaebi.TrimLeft('-');
//				strItemDaebi.TrimLeft('0');
//				strItemDaebi = '-' + strItemDaebi;
//				break;	
//			case '6':
//			case '7':
//			case '8':
//			case '9':
//			case '0':
//				if(strItemDaebi[0]=='-')
//				{
//					strItemDaebi.TrimLeft('-');
//					strItemDaebi.TrimLeft('0');
//					strItemDaebi = '-' + strItemDaebi;
//				}
//				else
//				{
//					strItemDaebi.TrimLeft('0');
//				}
//
//				break;
//			}
//			if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//			{
//				m_listCode.UpdateCodeData(strItemCode,strItemName,strItemPrice,strItemDaebi,strItemDaebiRate,strItemVolume,st_00410_o.grid[lPos].sign[0],lIndexCnt);
//			}
//			else
//			{
//				m_listCode.InsertCodeData(strItemCode,strItemName,strItemPrice,strItemDaebi,strItemDaebiRate,strItemVolume,st_00410_o.grid[lPos].sign[0]);
//			}
//			lIndexCnt++;
//		}
//		if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
//		{
//			m_listCode.RemoveNoUpdateItem();					
//		}
//		else
//		{
//			m_listCode.InitSortIndex();
//		}
//
//		m_bInsertingInListCode = FALSE;
//		m_JongStatusInTab.SetStatus(lUpLimit,lUp,lSteady,lDown,lDownLimit,lCnt,dDaebiRateTotal);
	}

	CStringList listStrJongmokCodeChecked;
	CStringList listStrJongmokCodeHighlighted;
	m_pChartMng->SendMessage(UMSG_GETJCODECHECKEDFROMCHART
		,(WPARAM)&listStrJongmokCodeChecked
		,(LPARAM)&listStrJongmokCodeHighlighted);
	m_listCode.CheckJongmokCode(&listStrJongmokCodeChecked,&listStrJongmokCodeHighlighted);
	m_listCode.Invalidate();
	this->SendMessage(UMSG_SETINITIALCHARTDATA);
	m_hRootItemCurrent = m_hRootItemCurrentTemp;
}

//@유진삭제
//void CTabCodeSearch::OnBtapply() 
//{
//	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//	BOOL bAddMode = FALSE;
//	UINT nSel = m_cmbMultiItem.GetCurSel();
//	if(nSel == 2)
//		bAddMode = TRUE;
//
//	// TODO: Add your control notification handler code here
//	UINT nCntSelected = m_listCode.GetCheckedCount();
//	if(nCntSelected==1)
//	{
//		STINPUTCODEDATA stInputCodeData;
//		stInputCodeData.m_strCode = m_listCode.GetCheckedCodes();
//		stInputCodeData.m_strCode.Remove(';');
//		stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
//		stInputCodeData.m_nDefault = MODE_DEFAULT;
//		stInputCodeData.m_nMode = CHANGE_ITEM;
//		m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
//	}
//	else if(nCntSelected>1)
//	{
//		m_pChartMng->SendMessage(UMSG_MULTICODES,bAddMode,(LPARAM)(LPCTSTR)m_listCode.GetCheckedCodeList());
//	}
//}
//@유진삭제

void CTabCodeSearch::OnBtportfolio() 
{
	// TODO: Add your control notification handler code here
//	m_pMultiChartWnd->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG1);	
}

void CTabCodeSearch::OnBtregfavor() 
{
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	UINT nSel = 3;
//@유진삭제
//	if(((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//		nSel = m_cmbMultiItem.GetCurSel();
//
//	switch(nSel)
//	{
//	case 0:
//		OnBtapply();
//		break;
//	case 1:
//		OnBtnAddapply();
//		break;
//	case 2:
//		OnBtapply();
//		break;
//	case 3:
//		{
//			CStringArray saCode;
//			CUIntArray iaMarketGb;
//			m_listCode.GetCheckedCodes(&saCode, &iaMarketGb);
//
//			if(saCode.GetSize()<1)
//			{
//				m_pMultiChartWnd->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG3);
//				return;
//			}
//
//			CStringArray strCodes;
//			for (int nIndex = 0; nIndex < saCode.GetSize(); nIndex++)
//			{
//				strCodes.Add(saCode.GetAt(nIndex));
//			}
//			IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
//			if(!pGateMng)
//				return;
//
//			/////////////////////////
//			IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(9);
//			CUIntArray arrIndex;
//			CStringArray arrName;
//			pMng->GetGwansimGroupList(arrIndex, arrName);
//
//			CMenu menu;
//
//			UINT flags = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//			menu.CreatePopupMenu();
//
//			int nSize = arrName.GetSize();
//			char szNo[3] = {0, };
//
//			LPCSTR szGroupName;
//			UINT nGroupIndex;
//			for(int nIndx = 0; nIndx < nSize; ++nIndx)
//			{
//				szGroupName = (LPCSTR)arrName.GetAt(nIndx);	//관심그룹이름.
//				nGroupIndex = (UINT)arrIndex.GetAt(nIndx);	//관심그룹인덱스.
//				menu.AppendMenu(MF_STRING,200+nGroupIndex,szGroupName);
//			}
//
//			CRect rect;
//			m_btnRegFavorite.GetWindowRect(rect);
//			int nRetValue = menu.TrackPopupMenu(flags, rect.left, rect.bottom, this);
//			menu.DestroyMenu();	
//
//			if(nRetValue >= 200)
//			{
//				UINT nGroup = nRetValue-200;
//				pMng->AddGwansimtem_New(nGroup, strCodes, (long)m_hWnd);
//			}
//		}
//		break;
//	}
//@유진삭제

/*
  CStringArray saCode;
  CUIntArray iaMarketGb;
  m_listCode.GetCheckedCodes(&saCode, &iaMarketGb);
  
  if(saCode.GetSize()<1)
  {
	  m_pMultiChartWnd->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG3);
	  return;
  }

  LIST_TR_CODEINFO listCodeInfo;
  TR_CODEINFO stCodeInfo;
  for (int nIndex = 0; nIndex < saCode.GetSize(); nIndex++)
  {
    stCodeInfo.gb = (iaMarketGb.GetAt(nIndex) == 1)?'1':'2';
    memcpy(stCodeInfo.code, saCode.GetAt(nIndex), 6);
    listCodeInfo.AddTail(stCodeInfo);
  }

  IGateManager* pGateMng;
  AfxGetIGateManager(pGateMng);

  ITreeListManager* pMng = (ITreeListManager*)pGateMng->GetInterface(26);
	pMng->ImsiRegist(NULL, (long)&listCodeInfo);
  

	INT_TOTAL		data;
	memset(&data,0x00,sizeof(INT_TOTAL));
	strcpy(data.Igubun,"2");
	strcpy(data.IopenDlg,"1");
	strcpy(data.Icnt,"1");

	if(m_pMultiChartWnd)
	{
		HWND hMainWnd; AfxGetHanaroMainWnd(hMainWnd);
		CString strPathImageDir = (LPCTSTR)(LPCSTR)::SendMessage(hMainWnd, RMSG_INTREQUEST, (WPARAM)GetSafeHwnd(),(LPARAM)&data);
	}	
*/


//	CPtrArray arrGroup;
//	data.Idata = (void*)&arrGroup;
//
//	INT_GROUP_ST Group;
//	memset(&Group,0x00,sizeof(INT_GROUP_ST));
//	sprintf(Group.Icnt,"2");
//	sprintf(Group.Ino,"0");
//	sprintf(Group.Ititle,"JJimTemp");
//	arrGroup.Add(&Group);
//	
//	if(m_pMultiChartWnd)
//	{
//		HWND hMainWnd; AfxGetHanaroMainWnd(hMainWnd);
//		CString strPathImageDir = (LPCTSTR)(LPCSTR)::SendMessage(hMainWnd, RMSG_INTREQUEST, (WPARAM)GetSafeHwnd(),(LPARAM)&data);
//	}	
//  return TRUE;

	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
//	CString strCodes = m_listCode.GetCheckedCodes();
//	CString strCodeSingle;
//	long lPos = -1;
//	if(strCodes.IsEmpty()) 
//	{
//		m_pMultiChartWnd->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG3);
//		return;
//	}
//	if(strCodes.GetLength()==6)
//	{
//		strCodes += ';';
//	}
//
//	INT_TOTAL		data;
//	memset(&data,0x00,sizeof(INT_TOTAL));
//	strcpy(data.Igubun,"2");
//	strcpy(data.IopenDlg,"1");
//	strcpy(data.Icnt,"1");
//
//	CPtrArray arrGroup;
//	data.Idata = (void*)&arrGroup;
//
//	INT_GROUP_ST Group;
//	memset(&Group,0x00,sizeof(INT_GROUP_ST));
//	sprintf(Group.Icnt,"2");
//	sprintf(Group.Ino,"0");
//	sprintf(Group.Ititle,"JJimTemp");
//	arrGroup.Add(&Group);
//	CPtrArray ArrItem;
//	INT_ITEM_ST* pItem = NULL;
//	Group.Idata = (void*)&ArrItem;
//	while(1)
//	{
//		lPos = strCodes.Find(';');
//		if(lPos==-1) break;
//		strCodeSingle = strCodes.Left(lPos);
//		strCodes.Delete(0,lPos+1);
//		pItem = new INT_ITEM_ST;
//		memset(pItem,0x20,sizeof(INT_ITEM_ST));
//		memcpy(pItem->Icode,(LPCTSTR)strCodeSingle,strCodeSingle.GetLength());
//		ArrItem.Add(pItem);
//	}	
//	
//	if(m_pMultiChartWnd)
//	{
//		HWND hMainWnd; AfxGetHanaroMainWnd(hMainWnd);
//		CString strPathImageDir = (LPCTSTR)(LPCSTR)::SendMessage(hMainWnd, RMSG_INTREQUEST, (WPARAM)GetSafeHwnd(),(LPARAM)&data);
//		//((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_INTREQUEST,(WPARAM)GetSafeHwnd(),(LPARAM)&data);	
//	}	
//
//	long lSize = ArrItem.GetSize();
//	for(lPos=0;lPos<lSize;lPos++)
//	{
//		delete (INT_ITEM_ST*)ArrItem.GetAt(lPos);
//	}
//	ArrItem.RemoveAll();
}

void CTabCodeSearch::OnBtsearch() 
{
	// TODO: Add your control notification handler code here
//	m_pMultiChartWnd->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG1);
}

void CTabCodeSearch::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		m_bLBtnDown = FALSE;
		ReleaseCapture();

		if(m_bDragNDrop)	// 드래그 중이었으면
		{
			CPoint ptDrag(point);
			ClientToScreen(&ptDrag);
			CRect rectChartMng;
			m_pParentDlg->GetWindowRect(rectChartMng);
			if(!rectChartMng.PtInRect(ptDrag))
			{
				// 데이터 생성
				CDragNDrop* pDnD = new CDragNDrop;				
				UINT nCnt = m_listCode.GetSelectedCount();			
				if(nCnt>1)
				{
					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그리스트"), (LPTSTR)(LPCTSTR)m_listCode.GetSelectedCodes());
				}
				else
				{
					CString strPacketShared;
					strPacketShared = m_listCode.GetCodeSelected();
					strPacketShared += ';';
//					strPacketShared += m_listCode.GetTimeSelected();
					strPacketShared += "L";
					strPacketShared += ';';
					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그"), (LPTSTR)(LPCTSTR)strPacketShared);
				}
				//{{ modify		: {{{ 이영희 }}} Date : 2006/7/7 메인 모듈 옮겨움
				//m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->PostMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
				m_pMultiChartWnd->PostMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
				//}}
			}
			CRect rectItemCode;
			m_listCode.GetWindowRect(rectItemCode);
			if (rectItemCode.PtInRect(ptDrag))
			{
				m_listCode.ScreenToClient(&ptDrag);
				int nIndex = m_listCode.HitTest(ptDrag);
				if (nIndex != -1 && m_listCode.GetSelectedCount() == 1)
				{
					int nOrgIndex = m_listCode.GetCurSel();
					DWORD dwOrg = m_listCode.GetItemData(nOrgIndex);
					DWORD dwDes = m_listCode.GetItemData(nIndex);

					m_listCode.SetItemData(nIndex, dwOrg);
					m_listCode.SetItemData(nOrgIndex, dwDes);

					m_listCode.Invalidate();
				
				}
			}
		}
		m_bDragNDrop = FALSE;
	}
	
	CTabDlg::OnLButtonUp(nFlags, point);
}

void CTabCodeSearch::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bRBtnDown)
	{
		m_bRBtnDown = FALSE;
		ReleaseCapture();

		if(m_bDragNDrop)	// 드래그 중이었으면
		{
			CPoint ptDrag(point);
			ClientToScreen(&ptDrag);
			CRect rectChartMng;
			m_pParentDlg->GetWindowRect(rectChartMng);
			if(!rectChartMng.PtInRect(ptDrag))
			{
				// 데이터 생성
				CDragNDrop* pDnD = new CDragNDrop;				
				UINT nCnt = m_listCode.GetSelectedCount();			
				if(nCnt>1)
				{
					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그리스트"), (LPTSTR)(LPCTSTR)m_listCode.GetSelectedCodes());
				}
				else
				{
					CString strPacketShared;
					strPacketShared = m_listCode.GetCodeSelected();
					strPacketShared += ';';
//					strPacketShared += m_listCode.GetTimeSelected();
					strPacketShared += "R";
					strPacketShared += ';';
					pDnD->SetData(&ptDrag, m_hWnd, _T("코드-드래그"), (LPTSTR)(LPCTSTR)strPacketShared);
				}
				//{{ modify		: {{{ 이영희 }}} Date : 2006/7/7 메인 모듈 옮겨움
				//m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->PostMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
				m_pMultiChartWnd->PostMessage(RMSG_DRAGEND, 0, (LPARAM)pDnD);
				//}}
			}
		}
		m_bDragNDrop = FALSE;
	}
	
	CTabDlg::OnRButtonUp(nFlags, point);
}

void CTabCodeSearch::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		// 드래그임을 표시
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
//		m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
		
		//{{ modify		: {{{ 이영희 }}} Date : 2006/7/7 메인 모듈 옮겨움
		//m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
		m_pMultiChartWnd->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
		//}}
	}
	
	if(m_bRBtnDown)
	{
		if(((CStdDialog*)m_pMultiChartWnd)->IsMultiChartMode())
		{
			// 드래그임을 표시
			m_bDragNDrop = TRUE;
			CPoint ptDrag(point);
			ClientToScreen(&ptDrag);
			m_pMultiChartWnd->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
		}
	}

	CTabDlg::OnMouseMove(nFlags, point);
}

void CTabCodeSearch::SetConfig(BOOL bLoad /*= TRUE*/)
{
	CString strTemp;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	char szTemp[1024];
//	CTimerData timerData;	
	if(bLoad)
	{
		::GetPrivateProfileString("TabCodeSearch","HeightOfSearch","300",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_nHeightOfSearch = atoi(szTemp);
		if(m_nHeightOfSearch<180)
			m_nHeightOfSearch = DEFAULT_SPLITTER_HEIGHT;
//		::GetPrivateProfileString("TabCodeSearch","CntOfTime","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		int nCnt = atoi(szTemp);
//		for(int nIndex=0;nIndex<nCnt;nIndex++)
//		{
//			strTemp.Format("StartTime%02d",nIndex);
//			::GetPrivateProfileString("TabCodeSearch",strTemp,"0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//			timerData.m_lStartTime = atoi(szTemp);
//			strTemp.Format("Volume%02d",nIndex);
//			::GetPrivateProfileString("TabCodeSearch",strTemp,"0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//			timerData.m_lVolumeLimitation = atoi(szTemp);
//			if(timerData.IsAvailable())
//			{
//				m_listTimerData.Add(timerData);
//			}
//		}
//		::GetPrivateProfileString("TabCodeSearch","AlarmFrom","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_nAlarmFrom = atoi(szTemp);
//		::GetPrivateProfileString("TabCodeSearch","AlarmTo","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_nAlarmTo = atoi(szTemp);
//		::GetPrivateProfileString("TabCodeSearch","UseAlarmSound","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_bUseAlarmSound = atoi(szTemp);
//		::GetPrivateProfileString("TabCodeSearch","UseAlarmBkClr","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_bUseAlarmBkClr = atoi(szTemp);

//		CString strTempFile = m_pParentDlg->m_strRootPath + "Data\\sound1.wav";
//		::GetPrivateProfileString("TabCodeSearch","SoundFile",strTempFile,szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
//		m_strSoundFile = szTemp;		

		::GetPrivateProfileString("TabCodeSearch","SelectedFavoriteItemCode","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strFavoriteGrp = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedItemName","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strSelectedItemName = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedParentItemName","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strSelectedParentItemName = szTemp;

//		m_listCode.SetAlarmColor(m_bUseAlarmBkClr);
//		m_listCode.SetAlarmSound(m_bUseAlarmSound,m_strSoundFile);
//		m_listCode.SetAlarmPeriod(m_nAlarmFrom,m_nAlarmTo);
		::GetPrivateProfileString("TabCodeSearch","UserCodeOrder","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_srtCodeOrder = szTemp;
	}
	else
	{
		strTemp.Format("%d",m_nHeightOfSearch);
		::WritePrivateProfileString("TabCodeSearch","HeightOfSearch",strTemp,m_pParentDlg->m_strConfig);
//		int nCntInList = m_listTimerData.GetSize();
//		strTemp.Format("%d",nCntInList);
//		::WritePrivateProfileString("TabCodeSearch","CntOfTime",strTemp,m_pParentDlg->m_strConfig);
//		for(int nIndex=0;nIndex<nCntInList;nIndex++)
//		{
//			timerData =	m_listTimerData.GetAt(nIndex);
//			if(!timerData.IsAvailable())
//			{
//				continue;
//			}
//			strTemp.Format("StartTime%02d",nIndex);
//			strTemp1.Format("%2d",timerData.m_lStartTime);
//			::WritePrivateProfileString("TabCodeSearch",strTemp,strTemp1,m_pParentDlg->m_strConfig);
//			strTemp.Format("Volume%02d",nIndex);
//			strTemp1.Format("%2d",timerData.m_lVolumeLimitation);
//			::WritePrivateProfileString("TabCodeSearch",strTemp,strTemp1,m_pParentDlg->m_strConfig);
//		}
//		strTemp.Format("%d",m_nAlarmFrom);
//		::WritePrivateProfileString("TabCodeSearch","AlarmFrom",strTemp,m_pParentDlg->m_strConfig);
//		strTemp.Format("%d",m_nAlarmTo);
//		::WritePrivateProfileString("TabCodeSearch","AlarmTo",strTemp,m_pParentDlg->m_strConfig);
//		strTemp.Format("%d",m_bUseAlarmSound);
//		::WritePrivateProfileString("TabCodeSearch","UseAlarmSound",strTemp,m_pParentDlg->m_strConfig);
//		strTemp.Format("%d",m_bUseAlarmBkClr);
//		::WritePrivateProfileString("TabCodeSearch","UseAlarmBkClr",strTemp,m_pParentDlg->m_strConfig);
//		::WritePrivateProfileString("TabCodeSearch","SoundFile",m_strSoundFile,m_pParentDlg->m_strConfig);
		::WritePrivateProfileString("TabCodeSearch","SelectedFavoriteItemCode",m_strFavoriteGrp,m_pParentDlg->m_strConfig);
		::WritePrivateProfileString("TabCodeSearch","SelectedItemName",m_strSelectedItemName,m_pParentDlg->m_strConfig);
		::WritePrivateProfileString("TabCodeSearch","SelectedParentItemName",m_strSelectedParentItemName,m_pParentDlg->m_strConfig);

		strTemp = "";
		for (int i = 0; i < m_listCode.GetItemCount(); i++)
		{
			strTemp += m_listCode.GetCodeSelected(i);
		}
		::WritePrivateProfileString("TabCodeSearch","UserCodeOrder",strTemp,m_pParentDlg->m_strConfig);
	}
}

void CTabCodeSearch::SetTimerInfo(BOOL bLoad /*= TRUE*/)
{
	CString strTemp;
	char szTemp[64];
	long lCntOfTimer = 0;
	if(bLoad)
	{
		::GetPrivateProfileString("TabCodeSearch","CntOfUnit","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		lCntOfTimer = atoi(szTemp);
	}
	else
	{
		::WritePrivateProfileString("TabCodeSearch","CntOfUnit",strTemp,m_pParentDlg->m_strConfig);
	}
}

void CTabCodeSearch::OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
// --> [Edit]  강지원 2008/10/20
	//	시간탭에 있는 항목이 종목이 아니기때문에 드래그 앤 드랍을 막는다.
//	m_bLBtnDown = TRUE;
//	SetCapture();	
// <-- [Edit]  강지원 2008/10/20
	*pResult = 0;
}

void CTabCodeSearch::OnBeginRdragListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
// --> [Edit]  강지원 2008/10/20
	//	시간탭에 있는 항목이 종목이 아니기때문에 드래그 앤 드랍을 막는다.
//	m_bRBtnDown = TRUE;
//	SetCapture();	
// <-- [Edit]  강지원 2008/10/20
	*pResult = 0;
}
void CTabCodeSearch::RequestCurrentPrice(LPCTSTR lpBuffer,long lLength)
{
	stSend2Server st;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)lpBuffer;
	st.m_lLength= lLength;
	st.m_strTRCode = "05010";
	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
} 

void CTabCodeSearch::OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	
// --> [Edit]  강지원 2008/10/13		( 일단 막기.. 종목이 아니라 각 도시의 시간이기때문에 종목이 바뀌면 안됨 )
//@유진삭제	if(pNMListView->iSubItem>0)
//	{
////@유진삭제
////		BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
////		if(bInsertMode&&!m_bInsertingInListCode)
////		{
////			STINPUTCODEDATA stInputCodeData;
////			stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
////			stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
////			stInputCodeData.m_nDefault = MODE_DEFAULT;
////			stInputCodeData.m_nMode = CHANGE_ITEM;
////			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
////@유진삭제
////@유진추가
//			((CStdDialog*)m_pMultiChartWnd)->ChangeCode(m_listCode.GetCodeSelected());
////@유진추가
////@유진삭제
////		}	
////@유진삭제
//	}
// <-- [Edit]  강지원 2008/10/13
	*pResult = 0;
}

void CTabCodeSearch::OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
// --> [Edit]  강지원 2008/10/13		( 일단 막기.. 종목이 아니라 각 도시의 시간이기때문에 종목이 바뀌면 안됨 )

//@유진삭제
//	if(pNMListView->iSubItem==0)
//	{
//		m_listCode.UpdateCheck(pNMListView->iItem,m_pChartMng);
//	}
//	else
//@유진삭제
	{
//@유진삭제
//		BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
//		if(!bInsertMode&&!m_bInsertingInListCode)
//		{
//@유진삭제
			CString strCodeSelected = m_listCode.GetCodeSelected();
			if(!strCodeSelected.IsEmpty())
			{
//@유진삭제
//				STINPUTCODEDATA stInputCodeData;
//				stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
//				stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
//				stInputCodeData.m_nDefault = MODE_DEFAULT;
//				stInputCodeData.m_nMode = CHANGE_ITEM;
//				// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//				m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
//@유진삭제
//@유진추가
// --> [Edit]  강지원 2008/10/21
//				((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);

//				CString strTitle, strGmt, strSummerTime, strTemp;
//				strTemp = strCodeSelected;
//
//				int nCnt = strTemp.Find(",");
//				strTitle= strTemp.Left(nCnt);	
//				strTemp = strTemp.Mid(nCnt+1);	
//				nCnt	= strTemp.Find(",");
//				strGmt	= strTemp.Left(nCnt);
//				strSummerTime	= strTemp.Mid(nCnt+1);	
				m_nCurrIndex = pNMListView->iItem;
				((CStdDialog*)m_pMultiChartWnd)->ChangeTimeSet(pNMListView->iItem + 1, TRUE);
// <-- [Edit]  강지원 2008/10/21
//@유진추가
			}
//@유진삭제
//		}	
//@유진삭제
	}

// <-- [Edit]  강지원 2008/10/13
	*pResult = 0;
}

void CTabCodeSearch::OnBtall() 
{
	// TODO: Add your control notification handler code here
	int nItemCount = m_listCode.GetItemCount();
	for(int nIndex=0;nIndex<nItemCount;nIndex++)
	{
		m_listCode.SetCurSel(nIndex);
		m_nCurrIndex = nIndex;
	}	
}

void CTabCodeSearch::OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//@유진삭제
//	if(pNMListView->iSubItem==0)
//	{
//		m_listCode.UpdateCheck(pNMListView->iItem, m_pChartMng);
//	}
//	else
//@유진삭제
	{
		CString strCodeSelected = m_listCode.GetCodeSelected();
		if(!strCodeSelected.IsEmpty())
		{
// --> [Edit]  강지원 2008/10/22
//			((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);

//			CString strTitle, strGmt, strSummerTime, strTemp;
//			strTemp = strCodeSelected;
//
//			int nCnt = strTemp.Find(",");
//			strTitle= strTemp.Left(nCnt);	
//			strTemp = strTemp.Mid(nCnt+1);	
//			nCnt	= strTemp.Find(",");
//			strGmt	= strTemp.Left(nCnt);
//			strSummerTime	= strTemp.Mid(nCnt+1);	
			m_nCurrIndex = pNMListView->iItem;
			((CStdDialog*)m_pMultiChartWnd)->ChangeTimeSet(pNMListView->iItem + 1, TRUE);
// <-- [Edit]  강지원 2008/10/22
		}
	}
	*pResult = 0;
}

void CTabCodeSearch::OnTimer(UINT nIDEvent) 
{
//004 {{
	int nID = nIDEvent;
	if(AUTO_TIMER == nID)
	{
		long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
		BOOL bRet = m_listCode.SetCursorNext(lCntOfChart, m_chkRepeat.GetCheck());
		if(bRet)
		{
			KillTimer(nIDEvent);
			ApplySelectedCodes();
			if(m_bPlay)
			{
				double dTime = lCntOfChart * 1000;
				int nTime = dTime;
				int nCur = m_cmbTime.GetCurSel();
				switch(nCur) 
				{
				case 0:		// 2sec
					nTime *=  2; //SetTimer(AUTO_TIMER, 2000, NULL);
					break;
				case 1:		// 3sec
					nTime *=  3; //SetTimer(AUTO_TIMER, 3000, NULL);
					break;
				case 2:		// 4sec
					nTime *=  4; //SetTimer(AUTO_TIMER, 4000, NULL);
					break;
				case 3:		// 6sec
					nTime *=  6; //SetTimer(AUTO_TIMER, 6000, NULL);
					break;
				case 4:		// 8sec
					nTime *=  8; //SetTimer(AUTO_TIMER, 8000, NULL);
					break;
				case 5:		// 10sec
					nTime *=  10; //SetTimer(AUTO_TIMER, 10000, NULL);
					break;
				default:
					break;
				}
				
				SetTimer(AUTO_TIMER, nTime, NULL);
			}
		}
		else
		{
			KillTimer(nIDEvent);
			m_btPlay.FP_vSetImage(m_pIL_W20H20, 68, 70);
			m_btPlay.FP_vSetToolTipText("재생");
			m_btPlay.Invalidate(TRUE);
			m_bPlay = FALSE;
		}
	}
//004 }}
	else
	{
		KillTimer(nIDEvent);
		//m_treeSearch.ShowWindow(SW_SHOW);
		m_listCode.ShowWindow(SW_SHOW);
	}

/*
	switch(nIDEvent)
	{
	case 100:
		{
			if(m_bUseTimer)
			{
				BOOL bEnable = m_pMultiChartWnd->SendMessage(UMSG_GETSTATUSENABLE);
				if(bEnable)
				{
					SetVolumeInEdit();
					RequestJongmokCodesInList(m_treeSearch.GetSelectedItem());
				}
			}
			else
			{
				CString strImagePath = m_pParentDlg->m_strRootPath + "\\Image";
				CString strImageUp;
				CString strImageDown;
				CString strImageDis;
				KillTimer(100);
				strImageUp = strImagePath + "\\[9000]btn_정지_up.bmp";
				strImageDown = strImagePath + "\\[9000]btn_정지_down.bmp";	
				strImageDis = strImagePath + "\\[9000]btn_정지_dis.bmp";	
				m_btActTimer.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);
				m_btActTimer.SetToolTipText("정지");
				m_btActTimer.Invalidate();
			}
		}
		break;
	case 200:
		{
			KillTimer(200);
			m_hRootItemFromOutside = m_treeSearch.InsertItem("조건검색결과");		
			m_treeSearch.SelectItem(m_hRootItemFromOutside);
		}
		break;
	}
*/

	
	CTabDlg::OnTimer(nIDEvent);
}

/*
void CTabCodeSearch::SetVolumeInEdit()
{
	BOOL bChkVolTimer = m_chkVolTimer.GetCheck();
	long lVolumeLimitation = GetVolumeLimitation();
	if(lVolumeLimitation&&bChkVolTimer)
	{
		CString strVolume;
		strVolume.Format("%d",lVolumeLimitation);
		m_bIgnoreEditVolume = TRUE;
		m_editVolume.SetWindowText(strVolume);
		m_bIgnoreEditVolume = FALSE;
	}
}
*/

/*
void CTabCodeSearch::SetTimeFilter()
{
	CString strImagePath = m_pParentDlg->m_strRootPath + "\\Image";
	CString strImageUp;
	CString strImageDown;
	CString strImageDis;
	if(m_bUseTimer)
	{
		strImageUp = strImagePath + "\\[9000]btn_시작_up.bmp";
		strImageDown = strImagePath + "\\[9000]btn_시작_down.bmp";	
		strImageDis = strImagePath + "\\[9000]btn_시작_dis.bmp";	
		m_btActTimer.SetToolTipText("시작");
		CString strTimeInterval;
		long lTimeInterval = 0;
		int nCurSel = m_cbTimeInterval.GetCurSel();
		if(nCurSel!=LB_ERR)
		{
			m_cbTimeInterval.EnableWindow(FALSE);
			m_cbTimeInterval.GetLBText(nCurSel,strTimeInterval);
			int nPos = strTimeInterval.Find("초");
			if(nPos!=-1)
			{
				lTimeInterval = atol(strTimeInterval.Left(nPos));
			}
			SetVolumeInEdit();
			RequestJongmokCodesInList(m_treeSearch.GetSelectedItem());
			SetTimer(100,lTimeInterval*1000,NULL);
		}
	}
	else
	{
		m_cbTimeInterval.EnableWindow();
		strImageUp = strImagePath + "\\[9000]btn_정지_up.bmp";
		strImageDown = strImagePath + "\\[9000]btn_정지_down.bmp";	
		strImageDis = strImagePath + "\\[9000]btn_정지_dis.bmp";	
		m_btActTimer.SetToolTipText("정지");		
	}
	m_btActTimer.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);		
	m_btActTimer.Invalidate();
}
*/

/*
void CTabCodeSearch::OnBtacttimer() 
{
	// TODO: Add your control notification handler code here
	if(m_bUseTimer)
	{
		m_bUseTimer = FALSE;
	}
	else
	{
		m_bUseTimer = TRUE;
	}
	SetTimeFilter();
}
*/

/*
void CTabCodeSearch::OnBtsettimer() 
{
	// TODO: Add your control notification handler code here
	CSettingerDlg dlg;
	CRect rect;
	m_btnSetTimer.GetWindowRect(rect);
	dlg.m_ptButtomLeftOfBtn.x = rect.left;
	dlg.m_ptButtomLeftOfBtn.y = rect.bottom;
	dlg.SetData(this);
	dlg.DoModal();
	m_listCode.SetAlarmColor(m_bUseAlarmBkClr);
	m_listCode.SetAlarmSound(m_bUseAlarmSound,m_strSoundFile);
	m_listCode.SetAlarmPeriod(m_nAlarmFrom,m_nAlarmTo);
}
*/


/*
long CTabCodeSearch::GetVolumeLimitation()
{
	CTime tm = CTime::GetCurrentTime();
	long lHour = tm.GetHour();
	CTimerData timerDataFrom;
	CTimerData timerDataTo;
	long lSizeOfList = m_listTimerData.GetSize();
	for(int nPos=0;nPos<lSizeOfList;nPos++)
	{
		timerDataTo = m_listTimerData.GetAt(nPos);
		if(timerDataTo.m_lStartTime==100)
		{
			timerDataTo.m_lStartTime = 0;
		}
		if(lSizeOfList>1)
		{
			if(timerDataFrom.m_lStartTime<=lHour&&timerDataTo.m_lStartTime>lHour)
			{
				return timerDataFrom.m_lVolumeLimitation;
			}
		}
		if(nPos==(lSizeOfList-1))
		{
			if(timerDataTo.m_lStartTime<=lHour)
			{
				return timerDataTo.m_lVolumeLimitation;
			}
		}
		timerDataFrom = timerDataTo;
	}
	return 0;
}*/


/*
void CTabCodeSearch::OnChangeEditvolume() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTabDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!m_bIgnoreEditVolume)	
	{
		m_bUseTimer = FALSE;
		m_chkVolTimer.SetCheck(FALSE);
		SetTimeFilter();
	}
}
*/

BOOL CTabCodeSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam) 
			{
			case VK_RETURN:
				{
					if(m_editVolume.GetSafeHwnd()==pMsg->hwnd)
					{
						RequestJongmokCodesInList(m_treeSearch.GetSelectedItem());
						return 1L;
					}
				}
				break;
			}
		}
		break;
	}	
*/
	return CTabDlg::PreTranslateMessage(pMsg);
}

HBRUSH CTabCodeSearch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		{
			pDC->SetBkColor(m_crBk);
			return m_brBkColor;
		}		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*
void CTabCodeSearch::OnSelchangeCombotimeinterval() 
{
	// TODO: Add your control notification handler code here
	m_bUseTimer = FALSE;
	SetTimeFilter();
}
*/

void CTabCodeSearch::OnBtnext() 
{
	// TODO: Add your control notification handler code here
	if(m_listCode.GetItemCount() < 1)
		return;
	m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);
	long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
	m_listCode.SetCursorNext(lCntOfChart, m_chkRepeat.GetCheck());
	ApplySelectedCodes();
}

void CTabCodeSearch::OnBtprev() 
{
	// TODO: Add your control notification handler code here
	if(m_listCode.GetItemCount() < 1)
		return;
	m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);
	long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
	m_listCode.SetCursorPrev(lCntOfChart, m_chkRepeat.GetCheck());
	ApplySelectedCodes();
}

/*
void CTabCodeSearch::OnChkautosort() 
{
	// TODO: Add your control notification handler code here
	m_listCode.SetAutoSort(m_chkAutoSort.GetCheck());
}
*/

/*
void CTabCodeSearch::OnBttreeup() 
{
	// TODO: Add your control notification handler code here
	m_nHeightOfSearch = 30;
	RecalcLayout();	
	HTREEITEM hItem = m_treeSearch.GetSelectedItem();
	m_treeSearch.EnsureVisible(hItem);
}
*/

/*
void CTabCodeSearch::OnBtvolumecombo() 
{
	// TODO: Add your control notification handler code here

    CMenu menu;
    menu.CreatePopupMenu();

	char szTemp[64];
	long lPosAll = 0;
	long lSize = 0;
	long lValueOfCombo[7] = {0, 1, 5, 10, 30, 50, 100};
	
	
	for(int nIndex=0;nIndex<7;nIndex++)
	{
		sprintf(szTemp,"%d",lValueOfCombo[nIndex]);
		menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,szTemp);
	}

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	CRect rect;
	m_btnCbVolume.GetWindowRect(rect);
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, rect.left, rect.bottom, this);
	if(!nRetValue) return;
	menu.DestroyMenu();
	sprintf(szTemp,"%d",lValueOfCombo[nRetValue-1000]);
//	m_editVolume.SetWindowText(szTemp);
}

*/
void CTabCodeSearch::OnBtresetocx() 
{
	// TODO: Add your control notification handler code here
	m_pChartMng->SendMessage(UMSG_RESETOCX);
}


void  CTabCodeSearch::DeleteFavoriteChild()
{
//	// Delete all of the children of RootItemFavorite.
//	m_bDeletingTreeData = TRUE;
//	if (m_treeSearch.ItemHasChildren(m_hRootItemFavorite))
//	{
//		HTREEITEM hNextItem;
//		HTREEITEM hChildItem = m_treeSearch.GetChildItem(m_hRootItemFavorite);
//		CString strChildItem;
//		while (hChildItem != NULL)
//		{
//			hNextItem = m_treeSearch.GetNextItem(hChildItem, TVGN_NEXT);
//			strChildItem = m_treeSearch.GetItemText(hNextItem);
//			m_treeSearch.DeleteItem(hChildItem);
//			hChildItem = hNextItem;
//		}
//	}
//	m_bDeletingTreeData = FALSE;
}



LRESULT CTabCodeSearch::OnIntRequest( UINT wParam, LONG lParam )
{

//	if(GetSafeHwnd() == (HWND)wParam)
//	{		
//		CPtrArray *ptrArrayIntGroupSt = (CPtrArray *)((INT_TOTAL*)lParam)->Idata;
//		if(!ptrArrayIntGroupSt) 
//		{			
//			DeleteFavoriteChild();
//			return 1L;
//		}
//		DWORD wSize = ptrArrayIntGroupSt->GetSize();
//		INT_GROUP_ST *pIntGroupSt = NULL;
//		INT_ITEM_ST *pIntItemSt = NULL;
//		HTREEITEM hItem = NULL;
//		CString *pstr = NULL;
//		CString strFavoriteGrp;
//		char szBuffer[64];
//		if(!m_bQueryList)
//		{
//			DeleteFavoriteChild();
//			CString strTitle;
//			for (DWORD wIndex = 0; wIndex < wSize; wIndex++)
//			{
//				pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(wIndex);
//				memcpy(szBuffer,pIntGroupSt->Ino,sizeof(pIntGroupSt->Ino));
//				szBuffer[sizeof(pIntGroupSt->Ino)] = NULL;
//				strFavoriteGrp = szBuffer;
//				memcpy(szBuffer,pIntGroupSt->Ititle,sizeof(pIntGroupSt->Ititle));
//				szBuffer[sizeof(pIntGroupSt->Ititle)] = NULL;
//				strTitle = szBuffer;
//				strTitle.TrimLeft();
//				strTitle.TrimRight();	
//				hItem = m_treeSearch.InsertItem(strTitle,m_hRootItemFavorite);
//				pstr = new CString;
//				pstr->Format("%s",strFavoriteGrp);		
//				m_treeSearch.SetItemData(hItem,(DWORD)pstr);
//			}
//			
//			if(m_strSelectedParentItemName=="관심종목")
//			{
//				HTREEITEM hChildItem = m_treeSearch.GetChildItem(m_hRootItemFavorite);
//				HTREEITEM hNextItem;
//				while (hChildItem)
//				{
//					if(!hChildItem) break;
//					CString *pStrCode = (CString *)m_treeSearch.GetItemData(hChildItem);
//					if(*pStrCode==m_strFavoriteGrp)
//					{
//						m_treeSearch.SelectItem(hChildItem);
//						return 1L;
//						break;
//					}				
//					hNextItem = m_treeSearch.GetNextItem(hChildItem, TVGN_NEXT);
//					hChildItem = hNextItem;
//				}
//				m_treeSearch.SelectItem(m_hRootItemFavorite);
//				m_listCode.RemoveAll();	
//			}
//		}
//		m_bQueryList = FALSE;
//		if(m_strSelectedParentItemName!="관심종목")
//		{
//			return 1L;				
//		}	
//		char Buffer[4096];
//		char szTemp[16];
//		char    jg_code         [15];   // 종목코드   
//		long lOffsetOfBuffer = 0;
//		TR_05010_I stTr05010i;
//		TR_05010_IS stTr05010is;
//		
//		//pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(m_nPosGroup);
//		BOOL bFindGroupSt = FALSE;
//		CString strTitleInGroupSt;
//		long lSizeOfGroupSt = ptrArrayIntGroupSt->GetSize();
//		for(long lPos=0;lPos<lSizeOfGroupSt;lPos++)
//		{
//			pIntGroupSt = (INT_GROUP_ST *)ptrArrayIntGroupSt->GetAt(lPos);
//			memcpy(szBuffer,pIntGroupSt->Ino,sizeof(pIntGroupSt->Ino));
//			szBuffer[sizeof(pIntGroupSt->Ino)] = NULL;
//			strFavoriteGrp = szBuffer;
////				strTitleInGroupSt = pIntGroupSt->Ititle;
////				strTitleInGroupSt.TrimRight();
//			if(strFavoriteGrp==m_strFavoriteGrp)
//			{
//				bFindGroupSt = TRUE;
//				break;
//			}
//		}
//		if(!bFindGroupSt) 
//		{
////			CString strMessage;
////			strMessage = m_strFavoriteGrp + "를 찾을수 없습니다.";
////			MessageBox(strMessage);
//			return 0L;
//		}
//		CPtrArray *ptrArrayIntItemSt = (CPtrArray *)(pIntGroupSt->Idata);
//		DWORD wSizeItems = ptrArrayIntItemSt->GetSize();
//		sprintf(szTemp,"%4d",wSizeItems);
//
//		MEMCPYBYTARSIZE(stTr05010i.winid,"0000");
//		MEMCPYBYTARSIZE(stTr05010i.jgcnt,szTemp);
//
//		MEMCPYBYSRCSIZE(Buffer,stTr05010i,lOffsetOfBuffer);
//		for (DWORD wIndex = 0; wIndex < wSizeItems; wIndex++)
//		{
//			pIntItemSt = (INT_ITEM_ST *)ptrArrayIntItemSt->GetAt(wIndex);
//		
//			memset(jg_code,0x20,sizeof(jg_code));
//			memcpy(jg_code,pIntItemSt->Icode,sizeof(pIntItemSt->Icode)-1);
//			stTr05010is.jg_tp[0] = pIntItemSt->Igubun[0];
//			MEMCPYBYTARSIZE(stTr05010is.jg_code,jg_code);
//			MEMCPYBYSRCSIZE(Buffer,stTr05010is,lOffsetOfBuffer);
//		}
//		if(!m_bUseTimer)
//		{
//			m_listCode.RemoveAll();	
//		}			
//		RequestCurrentPrice(Buffer,lOffsetOfBuffer);
//	}	

	return 1L;
}

LRESULT CTabCodeSearch::OnResetAll( UINT wParam, LONG lParam )
{	
	NMHDR *nmhdr;
	nmhdr = (NMHDR*)lParam;
	
	if ((nmhdr->code == 3) || (nmhdr->code == 6))
	{
		IntRequest();
	}	
	return 1L;
}

void CTabCodeSearch::IntRequest()
{	
	INT_TOTAL		data;
	memset(&data,0x00,sizeof(INT_TOTAL));
	memcpy(data.Igubun,"1",1);
	memcpy(data.IopenDlg,"0",1);
//	((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_INTREQUEST,(WPARAM)GetSafeHwnd(),(LPARAM)&data);	
}


void CTabCodeSearch::AdviseMain(int nAdivise)
{
//	NMHDR nmhdr;
//	::memset(&nmhdr, 0x00, sizeof(NMHDR));
//	nmhdr.hwndFrom = m_hWnd;
//	nmhdr.idFrom = 0;		
//	nmhdr.code = 0;			// 3,6일때 Update			
//	WPARAM wParam = nAdivise;	// 1: 등록, 0 : 삭제, 3:Notify 		
//	((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CONCERNHWND, wParam, (LPARAM)&nmhdr);
}


void CTabCodeSearch::SetAccountInfo(LPCTSTR lpAccountInfo)
{
//	CString strAccountInfoAll = lpAccountInfo;
//	CString strAccountInfo;
//	CString strAccountNumber;
//	HTREEITEM hItem = NULL;
//	CString *pstr = NULL;
//	if(!m_hRootItemAccount)
//	{
//		MessageBox("Account Error!");
//	}
//	while(1)
//	{
//		strAccountInfo = strAccountInfoAll.Left(65);
//		strAccountInfoAll.Delete(0,65);
//		strAccountNumber = strAccountInfo.Left(11);
//		hItem = m_treeSearch.InsertItem(strAccountNumber,m_hRootItemAccount);		
//		pstr = new CString;
//		pstr->Format("%s",strAccountNumber);		
//		//2006.01.14m_treeSearch.SetItemData(hItem,(DWORD)pstr);
//		if(strAccountInfoAll.IsEmpty()) break;
//	}

}

void CTabCodeSearch::RequestSavedQuery()
{
//	if(m_lQueryStatus==0)
//	{
//		HTREEITEM hSubRootItem = NULL;
//		HTREEITEM hChildItem = NULL;
//		CString strSubRootItem;
//		CString strChildItem;
//		while(TRUE)
//		{
//			if(!hSubRootItem)
//			{			
//				hSubRootItem = m_treeSearch.GetRootItem();
//			}
//			else
//			{
//				hSubRootItem = m_treeSearch.GetNextSiblingItem(hSubRootItem);
//			}
//			if(!hSubRootItem) break;
//			strSubRootItem = m_treeSearch.GetItemText(hSubRootItem);
//			if(strSubRootItem==m_strSelectedParentItemName)
//			{
//				hChildItem = NULL;
//				while(TRUE)
//				{
//					if(!hChildItem)
//					{
//						hChildItem = m_treeSearch.GetChildItem(hSubRootItem);
//					}
//					else
//					{
//						hChildItem = m_treeSearch.GetNextSiblingItem(hChildItem);
//					}
//					if(!hChildItem) break;
//					strChildItem = m_treeSearch.GetItemText(hChildItem);
//					if(strChildItem==m_strSelectedItemName)
//					{
//						m_treeSearch.Expand(hSubRootItem,TVE_EXPAND);
//						m_treeSearch.SelectItem(hChildItem);
//						break;
//					}
//				}
//				break;
//			}
//			else if(strSubRootItem==m_strSelectedItemName)
//			{
//				m_treeSearch.SelectItem(hSubRootItem);
//				break;
//			}
//		}
//	}
}


void CTabCodeSearch::ReceiveDataFromOutSide()
{
	if(!m_pParentDlg->GetPacketFromOutSide().IsEmpty()&&!m_hRootItemFromOutside)
	{
		SetTimer(200,50,NULL);
	}	
}


void CTabCodeSearch::GetCodeData00410(CString *pStr)
{
//	CString strGrptmcd;
//	strGrptmcd.Format("%2d",dwCode);


//	TR_00410_I st00410_i;
//	memcpy(st00410_i.theme_code,(LPCTSTR)*pStr,sizeof(st00410_i.theme_code));
//	stSend2Server st;
//	st.m_pWnd = this;
//	st.m_pBuffer = (void*)&st00410_i;
//	st.m_lLength= sizeof(st00410_i);
//	st.m_strTRCode = "00410";
//	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);		
}
void CTabCodeSearch::Init_Button()
{
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)

	//m_btnRegFavorite.InitKoscomCtrl(this, 9, IDC_BTREGFAVOR, NULL);
	//m_btnRegFavorite.SetDataText("관심등록");

//@유진삭제
//	m_btnRetry.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "재조회", FALSE);
//	m_btnRetry.FP_vSetToolTipText("시세재조회");
//
//	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//	if(((CStdDialog*)m_pMultiChartWnd)->IsMultiItemChart())
//	{
//		m_btnRegFavorite.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "확인", FALSE);
//		m_btnRegFavorite.FP_vSetToolTipText("확인");	
//	}
//	else
//	{
//		m_btnRegFavorite.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "등록", FALSE);
//		m_btnRegFavorite.FP_vSetToolTipText("관심등록");	
//
//		m_btnAddApply.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "추가", FALSE);
//		m_btnAddApply.FP_vSetToolTipText("종목추가");
//
//		m_btnApply.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "변경", FALSE);
//		m_btnApply.FP_vSetToolTipText("종목변경");
//	}
//
//	m_btnAll.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "전체", FALSE);
//	m_btnAll.FP_vSetToolTipText("전체");		
//@유진삭제
	
	//이전	
	m_btPrev.FP_vSetImage(m_pIL_W20H20, 32, 34);
	m_btPrev.FP_vSetToolTipText("이전");

	//이후	
	m_btNext.FP_vSetImage(m_pIL_W20H20, 28, 30);
	m_btNext.FP_vSetToolTipText("이후");		
	
	//차트 초기화 
	m_btnResetOcx.FP_vSetImage(m_pIL_W20H20, 52, 53);
	m_btnResetOcx.FP_vSetToolTipText("차트 초기화");

	//재생	
	m_btPlay.FP_vSetImage(m_pIL_W20H20, 68, 70);
	m_btPlay.FP_vSetToolTipText("재생");

//@유진삭제
//	//그리드확장	
//	m_btnGridExtend.FP_vSetImage(m_pIL_W20H20, 112, 114);
//	m_btnGridExtend.FP_vSetToolTipText("그리드확장");
//
//	//20061212 정유진수정
//	//grid column 확장btn 추가
//	m_chkColumnExtend.FP_vSetImage(m_pIL_W20H20, 56, 58);
//	m_chkColumnExtend.FP_vSetToolTipText("그리드 Column 확장");
//	//20061212 수정 end
//@유진삭제

	// (2008/9/28 - Seung-Won, Bae) Hide Rotate UI
	m_btPrev.ShowWindow( SW_HIDE);
	m_btNext.ShowWindow( SW_HIDE);
	m_btnResetOcx.ShowWindow( SW_HIDE);
	m_btPlay.ShowWindow( SW_HIDE);
	m_chkRepeat.ShowWindow( SW_HIDE);
	m_cmbTime.ShowWindow( SW_HIDE);
}

void CTabCodeSearch::CTLMCallback::ReceiveData(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData)
{
//	TRACE("CTabCodeSearch::CTLMCallback::ReceiveData\n");

	METHOD_PROLOGUE(CTabCodeSearch, TLMCallback);
	// pThis->

	if(aDataType=='A')
	{
		if(	pThis->m_nCtrlType == 1)		// Kospi200옵션
		{
			LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
			if(pListTypeA)
			{
				pThis->m_bInsertingInListCode = FALSE;
				pThis->SetMasterData(pListTypeA);
				pThis->m_bInsertingInListCode = FALSE;
			}
		}
		else if( pThis->m_nCtrlType == 2)	// 주식옵션
		{
			LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
			if(pListTypeA)
			{
				pThis->m_bInsertingInListCode = FALSE;
				pThis->SetMasterData(pListTypeA);
				pThis->m_bInsertingInListCode = FALSE;
			}
		}
		else								// 그외
		{
			LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
			pThis->m_listCode.SetRedraw(FALSE);

			if(nLength >= 0)
			{
				pThis->m_listCode.RemoveAll();
				pThis->m_listCode.m_nIndex = 0;
			}
			if(pListTypeA)
			{
				pThis->m_bInsertingInListCode = TRUE;
				CRect rc;
				pThis->m_listCode.GetClientRect(rc);

				int nIndex = 0;
				int nCnt = rc.Height()/17;
				int nMarketType = CMasterTypeInfo::STOCK_CHART;
				for(POSITION pos=pListTypeA->GetHeadPosition(); pos; )
				{
					CCodeTypeA typeA = pListTypeA->GetNext(pos);

	//@유진수정가능
	//				if(((CStdDialog*)pThis->m_pMultiChartWnd)->m_nChartType == SINGLE_CHART)
	//				{
	//					nMarketType = ((CStdDialog*)pThis->m_pMultiChartWnd)->GetMarketTypeFromCode(typeA.m_szcode);
	//					if(nMarketType != ((CStdDialog*)pThis->m_pMultiChartWnd)->m_nMarketType)
	//						continue;
	//				}
	//@유진수정가능
					pThis->m_listCode.InsertCodeData(
							typeA.m_szcode,
							typeA.m_szhname,
							"", "", "", "", '3'
							);

					if(pListTypeA->GetCount()>0 && pListTypeA->GetCount()<=200)
					{
						pThis->m_strCodes += ";";
						pThis->m_strCodes +=typeA.m_szcode;
					}

						nIndex++;
						if(nIndex == nCnt) {
							pThis->m_listCode.SetRedraw(TRUE);
							pThis->m_listCode.SetItemCountEx(nCnt);
						}
				}
				pThis->m_listCode.SetRedraw(TRUE);
				pThis->m_listCode.SetItemCountEx(pListTypeA->GetCount());
	//@유진수정가능		pThis->m_listCode.SetItemCountEx(pThis->m_listCode.GetItemCount());

	//			ITrComm* pTrComm = (ITrComm*)pThis->m_pQueryData->m_pITrComm;
	//			if(pTrComm)
	//			{
	//				CString strCompare;
	//				strCompare.Format("%s", pThis->m_pQueryData->m_szMethod);
	//
	//				if(strCompare.Compare("USERTR") == 0)
	//					pTrComm->ChangeCallback(&pThis->m_xMyITrCommSite);
	//			}
				pThis->m_bInsertingInListCode = FALSE;
			}
		}
	}
	else
	{
		LIST_CCodeTypeB *pListTypeB = (LIST_CCodeTypeB*)pData;
		pThis->m_listCode.SetRedraw(FALSE);

		if(nLength >= 0)
		{
			if(pThis->m_bAdvise)
			{
				if(pThis->m_strCodes.GetLength() > 0)
					pThis->MakeDrdsUnConnectUnAdvise(pThis->m_strCodes);
				pThis->m_strCodes = "";
			}
			pThis->m_listCode.RemoveAll();
			pThis->m_listCode.m_nIndex = 0;
		}
		if(pListTypeB)
		{
			long	lUpLimit = 0;
			long	lUp = 0;
			long	lSteady = 0;
			long	lDown = 0;
			long	lDownLimit = 0;
			long	lIndexCnt = 0;
			double dDaebiRateTotal = 0;

			//CString strVolumeLimitation;
			//double dVolumeLimitation = atof(strVolumeLimitation)*1000;
			pThis->m_bInsertingInListCode = TRUE;
			CRect rc;
			pThis->m_listCode.GetClientRect(rc);
			int nCnt = rc.Height()/17;
			int nIndex = 0;

			for(POSITION pos=pListTypeB->GetHeadPosition(); pos; )
			{
				CCodeTypeB typeB = pListTypeB->GetNext(pos);
				if(typeB.m_szsign.GetLength()==0)
					typeB.m_szsign = EQUAL;
				//if(atof(szTemp)<dVolumeLimitation) continue;

				switch(typeB.m_szsign[0])
				{
				case UP_LIMIT:	// UpLimit
					lUpLimit++;
					break;
				case UP:	// Up
					lUp++;
					break;
				case EQUAL:	// Steady
					lSteady++;
					break;
				case DOWN:	// Down
					lDown++;
					//strChange = '-' + strChange;
					break;
				case DOWN_LIMIT:	// DownLimit
					lDownLimit++;
					//strChange = '-' + strChange;
					break;	
				}
				//dDaebiRateTotal += (double)atof(strChgRate);
				
	//			if(m_bUseTimer&&m_hRootItemCurrent==m_hRootItemCurrentTemp)
	//			{
	//				m_listCode.UpdateCodeData(	typeB.m_szcode,
	//											szName,
	//											typeB.m_szprice,
	//											typeB.m_szchange,
	//											typeB.m_szchrate,
	//											typeB.m_szvolume,
	//											typeB.m_szsign,
	//											lIndexCnt);
	//			}
	//			else
	//			{
					pThis->m_listCode.InsertCodeData(
						typeB.m_szcode,
						typeB.m_szhname,
						typeB.m_szprice,
						typeB.m_szchange,
						typeB.m_szchrate,
						typeB.m_szvolume,
						typeB.m_szsign[0],
						typeB.m_cGb - '0'
						);

					if(pListTypeB->GetCount()>0 && pListTypeB->GetCount()<=200)
					{
						pThis->m_strCodes += ";";
						pThis->m_strCodes +=typeB.m_szcode;
					}
					//}
					//	lIndexCnt++;
					nIndex++;
					if(nIndex == nCnt) {
						pThis->m_listCode.SetRedraw(TRUE);
						pThis->m_listCode.SetItemCountEx(nCnt);
					}
			}
			pThis->m_listCode.SetRedraw(TRUE);
			pThis->m_listCode.SetItemCountEx(pListTypeB->GetCount());

			//{{20070312 by LYH 종합차트 여러개 띄웠을 경우 관심리스트 리얼 못 받는 버그 수정
			ITrComm* pTrComm = (ITrComm*)pThis->m_pQueryData->m_pITrComm;
			if(pTrComm)
			{
				CString strCompare;
				strCompare.Format("%s", pThis->m_pQueryData->m_szMethod);

				if(strCompare.Compare("USERTR") == 0)
					pTrComm->ChangeCallback(&pThis->m_xMyITrCommSite);
			}
			//}}

			//20061218 정유진 수정
			//관심종목 선택인 경우만 실시간 버튼 활성화 나머지 항목 선택시 재조회 버튼으로 활성화 
			//아래 코드 WndProc로 코드 옮김
	/*	
			if(pListTypeB->GetCount()>0 && pListTypeB->GetCount()<=200)
			{
				pThis->m_btnRetry.SetBtnTitle("실시간", "실시간");
				pThis->m_btnRetry.EnableWindow(FALSE);
			}
			else
			{
				pThis->m_btnRetry.SetBtnTitle("재조회", "재조회");
				pThis->m_btnRetry.EnableWindow(TRUE);
			}

			if(pThis->m_strCodes.GetLength() > 0)
			{
				if(pThis->m_strCodes.GetAt(0)==',')
					pThis->m_strCodes = pThis->m_strCodes.Mid(1);
				pThis->MakeDrdsConnectAdvise(pThis->m_strCodes);
			}
	*/
			//20061218 수정 END

			pThis->m_bInsertingInListCode = FALSE;
//@유진삭제
//			pThis->m_JongStatusInTab.SetStatus(lUpLimit,lUp,lSteady,lDown,lDownLimit,lIndexCnt,dDaebiRateTotal/100);
//@유진삭제
			//}
		}
	}
}

//void CTabCodeSearch::OnRadioPrice() 
//{
//	// TODO: Add your control notification handler code here
//	m_listCode.SetColumnWidth(2, 47);
//	m_listCode.SetColumnWidth(3, 0);
//	m_listCode.SetColumnWidth(4, 0);
//	m_listCode.Invalidate();
//}
//
//void CTabCodeSearch::OnRadioChange() 
//{
//	// TODO: Add your control notification handler code here
//	m_listCode.SetColumnWidth(2, 0);
//	m_listCode.SetColumnWidth(3, 47);
//	m_listCode.SetColumnWidth(4, 0);
//	m_listCode.Invalidate();
//}
//
//void CTabCodeSearch::OnRadioChgrate() 
//{
//	// TODO: Add your control notification handler code here
//	m_listCode.SetColumnWidth(2, 0);
//	m_listCode.SetColumnWidth(3, 0);
//	m_listCode.SetColumnWidth(4, 47);	
//	m_listCode.Invalidate();
//}

///////////////////////////////////////////////////////////////
// WMLGT_NEW 0801 START
// DRDS 두개의 필드(PRICE,SIGN)에 대해서 Advise
///////////////////////////////////////////////////////////////////
BOOL CTabCodeSearch::MakeDrdsConnectAdvise(CString szKey)
{
	IDrdsLib* pDrdsLib = ((CStdDialog*)m_pMultiChartWnd)->GetDrdsLib();
	if(!pDrdsLib)
		return FALSE;
	pDrdsLib->DrdsConnect("S31");

	char* DataName = (LPSTR)(LPCTSTR)"S31";
	char* FieldName = "*";
	//for(int i =0 ;i <4; i++)
	//{
	//	if(i==0) FieldName = "price";
	//	else if(i==1) FieldName = "change";
	//	else if(i==2) FieldName = "chgrate";
	//	else FieldName = "sign";
		char* KeyName = (LPSTR)(LPCTSTR)szKey;
		UINT	nID = 200;
		
//@SolomonGNF 		pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName, m_hWnd);
	//}

	m_bAdvise = TRUE;
	return TRUE;
}

///////////////////////////////////////////////////////////////
// WMLGT_NEW 0801 START
// DRDS 두개의 필드(PRICE,SIGN)에 대해서 UnAdvise
///////////////////////////////////////////////////////////////////
BOOL CTabCodeSearch::MakeDrdsUnConnectUnAdvise(CString szKey)
{
	IDrdsLib* pDrdsLib = ((CStdDialog*)m_pMultiChartWnd)->GetDrdsLib();
	if(!m_bAdvise)
		return TRUE;
	char* DataName = (LPSTR)(LPCTSTR)"S31";
	char* FieldName = "*";
//	for(int i=0;i<4;i++)
//	{
//		if(i==0) FieldName = "price";
//		else if(i==1) FieldName = "change";
//		else if(i==2) FieldName = "chgrate";
//		else FieldName = "sign";

		char* KeyName = (LPSTR)(LPCTSTR)szKey;
		UINT nID = 200;
		
//@SolomonGNF 		pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName, m_hWnd);
//	}
	m_bAdvise = FALSE;
	pDrdsLib->DrdsClose(DataName);		
	return TRUE;
}

void CTabCodeSearch::SetRealPriceData(void *pData)
{
//@유진삭제 : 유진에서는 사이드바에서의 리얼처리는 하지 않는다.
/*	
	if(m_listCode.GetSafeHwnd() == NULL)
		return;
	CString strCode, strPrice, strChange, strChgrate, strVolume, strSign;
	char chSign;
	CString strPriceOld;
	CString strChangeOld;
	CString strChgrateOld;
	CString strVolumeOld;
	char	chSignOld;
	double dChgRateOld = 0;
	double dChgRateNew = 0;
	CRealUpdateData* cReal = (CRealUpdateData*)pData;
	BOOL bRet;
	int nIndex;

	strCode = cReal->m_strMainKeyVal;
	if((nIndex = m_listCode.FindCode(strCode)) < 0)
		return;

	m_listCode.GetCodeData(nIndex,strPriceOld,strChangeOld,strChgrateOld,strVolumeOld,chSignOld);
	strPriceOld.Remove(',');

	bRet = cReal->GetDataString(DI_PRICE, strPrice);// 현재가
	if(bRet)
	{
		strPrice.TrimLeft('0');
		strPrice.TrimLeft();
	}
	else
		strPrice = strPriceOld;

	bRet = cReal->GetDataString(DI_CHANGE, strChange);// 대비
	if(bRet)
	{
		strChange.TrimLeft('0');
		strChange.TrimLeft();
	}
	else
		strChange = strChangeOld;

	bRet = cReal->GetDataString(DI_CHGRATE, strChgrate);// 대비율
	if(!bRet)
		strChgrate = strChgrateOld;
	else
	{
		double dRate = atof(strChgrate)/100.;
		strChgrate.Format("%.2f", dRate);
	}

	//bRet = cReal->GetDataString(DI_VOLUME, strVolume);// 거래량
	//if(bRet)
	//{
	//	strVolume.TrimLeft();
	//}

	bRet = cReal->GetDataString(DI_SIGN, strSign);// 등락부호
	if(bRet)
		chSign = strSign.GetAt(0);
	else
		chSign = chSignOld;

	if(atoi(strPriceOld) == atoi(strPrice))
		return;

	m_listCode.UpdateCodeData(strCode,strCode,strPrice,strChange,strChgrate,"",chSign, nIndex);
	if(chSignOld != chSign)
	{
		dChgRateOld = atof(strChgrateOld);
		dChgRateNew = atof(strChgrate);
		m_JongStatusInTab.UpdateStatus(chSign,chSignOld,dChgRateNew-dChgRateOld);
	}

	//char szTemp0[16];
	//char szTemp1[16];
	//char szTemp2[16];
	//char szTemp3[16];
	//char szTemp4[16];
	//char chSign;
	//CString strPriceOld;
	//CString strChangeOld;
	//CString strChgrateOld;
	//CString strVolumeOld;
	//char	chSignOld;
	//double dChgRateOld = 0;
	//double dChgRateNew = 0;
	//DRDS_S31_DATA* Op = (DRDS_S31_DATA*) pData;
	//MEMCPYWITHNULL(szTemp0,Op->shcode);		// 종목코드
	//MEMCPYWITHNULL(szTemp1,Op->shcode);		// 종목코드
	//MEMCPYWITHNULL(szTemp2,Op->price);		// 현재가
	//MEMCPYWITHNULL(szTemp3,Op->change);		// 대비
	//MEMCPYWITHNULL(szTemp4,Op->chgrate);	// 대비율
	//
	//CString strChrate = szTemp4;
	//strChrate.TrimLeft();
	//strChrate.TrimLeft('0');
	//
	//if(strChrate[0]=='.')
	//{
	//	strChrate = '0' + strChrate;
	//}

	//chSign = Op->sign[0];
	//int nIndex = m_listCode.FindCode(szTemp0);
	//if(nIndex>=0)
	//{
	//	m_listCode.GetCodeData(nIndex,strPriceOld,strChangeOld,strChgrateOld,strVolumeOld,chSignOld);
	//	//m_listCode.UpdateCodeData(nIndex,szTemp0,szTemp1,szTemp2,szTemp3,szTemp4,chSign);		
	//	strPriceOld.Remove(',');
	//	
	//	if(atoi(strPriceOld) == atoi(szTemp2))
	//		return;

	//	OutputDebugString("TabCodeSearch\n");
	//	m_listCode.UpdateCodeData(szTemp0,szTemp1,szTemp2,szTemp3,strChrate,"",chSign, nIndex);
	//	if(chSignOld != chSign)
	//	{
	//		dChgRateOld = atof(strChgrateOld);
	//		dChgRateNew = atof(szTemp3);
	//		m_JongStatusInTab.UpdateStatus(chSign,chSignOld,dChgRateNew-dChgRateOld);
	//	}
	//}
*/
}

void CTabCodeSearch::OnBtnRetry() 
{
	// TODO: Add your control notification handler code here
	//HTREEITEM hItem = m_treeSearch.GetSelectedItem();// .m_hSelTreeItem;
	//if(hItem == NULL)
	//	return;

	//CItemLongData* pLData = (CItemLongData*)m_treeSearch.GetItemData(hItem);
	//if(pLData)
	//{
	//	TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
	//	long dwReserved=0;
	//	if(m_pTreeListMng)
	//		m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'B', '1', dwReserved);
	//}

	if(m_pQueryData)
	{
		long dwReserved=0;
		if(m_pTreeListMng) 
			m_pTreeListMng->RequestData(m_pQueryData, &m_xTLMCallback, 'B', '1', dwReserved);
	}
}

//멀티종목 추가하기
void CTabCodeSearch::OnBtnAddapply() 
{
	// TODO: Add your control notification handler code here
	//UINT nCntSelected = m_listCode.GetSelectedCount();
	UINT nCntSelected = m_listCode.GetCheckedCount();
	if(nCntSelected==1)
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = m_listCode.GetCheckedCodes();
		stInputCodeData.m_strCode.Remove(';');
		stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
		m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,2,(LPARAM)&stInputCodeData);
	}
	else if(nCntSelected>1)
	{
		m_pChartMng->SendMessage(UMSG_RESETOCX);
		// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
		m_pChartMng->SendMessage(UMSG_MULTICODES,2,(LPARAM)(LPCTSTR)m_listCode.GetCheckedCodeList());
	}
}


//004 돌려보기 Play버튼
void CTabCodeSearch::OnBnClickedBtplay()
{
	if(m_bPlay)		 
	{
		// 일시정지
		KillTimer(AUTO_TIMER);
		m_btPlay.FP_vSetImage(m_pIL_W20H20, 68, 70);
		m_btPlay.FP_vSetToolTipText("재생");
		m_btPlay.Invalidate();
		m_bPlay = FALSE;
	}
	else
	{
		// 플레이
		m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);

		long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
		if(lCntOfChart>0)
		{
			long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
			//2007.01.27 by LYH 처음 버튼 눌렸을때는 마지막일경우 처음부터 시작
			//m_listCode.SetCursorNext(lCntOfChart, m_chkRepeat.GetCheck());
			m_listCode.SetCursorNext(lCntOfChart, TRUE);
			//}}
			ApplySelectedCodes();
		}
		if(lCntOfChart == 1)
		{
			int nCur = m_cmbTime.GetCurSel();
			switch(nCur) 
			{
			case 0:		// 2sec
				SetTimer(AUTO_TIMER, 2000, NULL);
				break;
			case 1:		// 3sec
				SetTimer(AUTO_TIMER, 3000, NULL);
				break;
			case 2:		// 4sec
				SetTimer(AUTO_TIMER, 4000, NULL);
				break;
			case 3:		// 6sec
				SetTimer(AUTO_TIMER, 6000, NULL);
				break;
			case 4:		// 8sec
				SetTimer(AUTO_TIMER, 8000, NULL);
				break;
			case 5:		// 10sec
				SetTimer(AUTO_TIMER, 10000, NULL);
				break;
			default:
				break;
			}
		}
		else if(lCntOfChart > 1)
		{
			double dTime = lCntOfChart * 1.5 * 1000;
			int nTime = dTime;
			SetTimer(AUTO_TIMER, nTime, NULL);
		}
		m_btPlay.FP_vSetImage(m_pIL_W20H20, 64, 66);
		m_btPlay.FP_vSetToolTipText("정지");
		m_btPlay.Invalidate();
		//Invalidate(TRUE);
		m_bPlay = TRUE;	
	}
}

void CTabCodeSearch::RemoveAllTree()
{
	if(m_pGwanTree)
	{
		m_pGwanTree->DestroyWindow();
		delete m_pGwanTree;
	}

	if(m_pMasterTree_stock)
	{
		m_pMasterTree_stock->DestroyWindow();
		delete m_pMasterTree_stock;
	}

	if(m_pMasterTree_elw)
	{
		m_pMasterTree_elw->DestroyWindow();
		delete m_pMasterTree_elw;
	}

	if(m_pMasterTree_Jisu)
	{
		m_pMasterTree_Jisu->DestroyWindow();
		delete m_pMasterTree_Jisu;
	}

	if(m_pMasterTree_future)
	{
		m_pMasterTree_future->DestroyWindow();
		delete m_pMasterTree_future;
	}

	if(m_pMasterTree_foreign)
	{
		m_pMasterTree_foreign->DestroyWindow();
		delete m_pMasterTree_foreign;
	}

	if(m_pOneClickTree)
	{
		m_pOneClickTree->DestroyWindow();
		delete m_pOneClickTree;
	}

//	if(m_pUserFindTree)
//	{
//		m_pUserFindTree->DestroyWindow();
//		delete m_pUserFindTree;
//	}

	if(m_pFindSearchTree)
	{
		m_pFindSearchTree->DestroyWindow();
		delete m_pFindSearchTree;
	}
}

//@유진삭제
//void CTabCodeSearch::OnSelchangeColumnType() 
//{
//	// TODO: Add your control notification handler code here
//
//	if(m_cmbColumnType.GetCurSel()==0)
//	{
//		m_listCode.SetColumnWidth(2, 47);
//		m_listCode.SetColumnWidth(3, 0);
//		m_listCode.SetColumnWidth(4, 0);
//		m_listCode.Invalidate();
//	}
//	else if(m_cmbColumnType.GetCurSel()==1)
//	{
//		m_listCode.SetColumnWidth(2, 0);
//		m_listCode.SetColumnWidth(3, 47);
//		m_listCode.SetColumnWidth(4, 0);
//		m_listCode.Invalidate();
//	}
//	else if(m_cmbColumnType.GetCurSel()==2)
//	{
//		m_listCode.SetColumnWidth(2, 0);
//		m_listCode.SetColumnWidth(3, 0);
//		m_listCode.SetColumnWidth(4, 47);	
//		m_listCode.Invalidate();
//	}
//}
//
//void CTabCodeSearch::OnBtgridExtend() 
//{
//	// TODO: Add your control notification handler code here
//	if(m_bGridExtend)
//	{
//		m_btnGridExtend.FP_vSetImage(m_pIL_W20H20, 112, 114);
//		m_btnGridExtend.FP_vSetToolTipText("그리드확장");
//		m_bGridExtend = FALSE;
//	}
//	else
//	{
//		m_btnGridExtend.FP_vSetImage(m_pIL_W20H20, 116, 118);
//		m_btnGridExtend.FP_vSetToolTipText("그리드축소");
//		m_bGridExtend = TRUE;
//	}
//	RecalcLayout();
//}
//@유진삭제

//돌려보기 다른함수로 뺌
void CTabCodeSearch::ApplySelectedCodes() 
{
	// TODO: Add your control notification handler code here
	UINT nCntSelected = m_listCode.GetSelectedCount();
	if(nCntSelected==1)
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
		stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
		stInputCodeData.m_nDefault = MODE_DEFAULT;
		stInputCodeData.m_nMode = CHANGE_ITEM;

		((CStdDialog*)m_pMultiChartWnd)->ChangeCode(m_listCode.GetCodeSelected());
	}
	else if(nCntSelected>1)
	{
		m_pChartMng->SendMessage(UMSG_MULTICODES,NULL,(LPARAM)(LPCTSTR)m_listCode.GetSelectedCodes());
	}
}

void CTabCodeSearch::OnLvnItemchangedListcode(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


//@유진삭제
//void CTabCodeSearch::OnBtnColumnExtend()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//20061212 정유진수정
//	//grid column 확장btn 추가
//
//	CStdDialog *pStddlg;
//	pStddlg = ((CStdDialog*)m_pMultiChartWnd);
//
//	if (pStddlg->m_nExtendState == 0)
//	{
//		m_chkColumnExtend.FP_vSetImage(m_pIL_W20H20, 60, 62);
//		m_chkColumnExtend.FP_vSetToolTipText("그리드 Column 축소");
//
//		m_listCode.SetColumnWidth(2, 47);
//		m_listCode.SetColumnWidth(3, 40);
//		m_listCode.SetColumnWidth(4, 40);
//		m_cmbColumnType.EnableWindow(FALSE);
//		//	m_listCode.Invalidate();
//	}
//	else
//	{
//		m_chkColumnExtend.FP_vSetImage(m_pIL_W20H20, 56, 58);
//		m_chkColumnExtend.FP_vSetToolTipText("그리드 Column 확장");
//
//		if (m_cmbColumnType.GetCurSel()==0)
//		{
//			m_listCode.SetColumnWidth(2, 47);
//			m_listCode.SetColumnWidth(3, 0);
//			m_listCode.SetColumnWidth(4, 0);
//		}
//		else if (m_cmbColumnType.GetCurSel()==1)
//		{
//			m_listCode.SetColumnWidth(2, 0);
//			m_listCode.SetColumnWidth(3, 40);
//			m_listCode.SetColumnWidth(4, 0);
//		}
//		else if (m_cmbColumnType.GetCurSel()==2)
//		{
//			m_listCode.SetColumnWidth(2, 0);
//			m_listCode.SetColumnWidth(3, 0);
//			m_listCode.SetColumnWidth(4, 40);
//		}
//		m_cmbColumnType.EnableWindow(TRUE);
//	}
//
//	m_listCode.Invalidate();
//	pStddlg->GridColumnExtend();
//}
//@유진삭제

HRESULT CTabCodeSearch::CTLMCallback::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	//	TRACE("CTabCodeSearch::CTLMCallback::ReceiveData\n");

	METHOD_PROLOGUE(CTabCodeSearch, TLMCallback);
	//{{20070312 by LYH 종합차트 여러개 띄웠을 경우 관심리스트 리얼 못 받는 버그 수정
	ITrComm* pTrComm = (ITrComm*)pThis->m_pQueryData->m_pITrComm;
	if(pTrComm)
	{
		CString strCompare;
		strCompare.Format("%s", pThis->m_pQueryData->m_szMethod);

		if(strCompare.Compare("USERTR") != 0)
			return 0;
	}
	//}}
	pThis->SetRealPriceData(pRealData);
	return 0;
}

//*****************************************************************************
// 함  수  명  : long CTabCodeSearch::OnOutBarNotify(WPARAM wParam, LPARAM lParam)
// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-13 오후 2:55:04
//*****************************************************************************
long CTabCodeSearch::OnOutBarNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam==NM_FOLDERCHANGE)
	{
		int nSel = lParam;
		CInputCodeListEx inData;
		if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == MULTI_CHART)
		{
			if(nSel==codesearch_sel_gwan)	
			{
				if(m_dwTreeKey_gwan != 0)
				{
					m_pTreeListMng->UnLoadTreeList(m_dwTreeKey_gwan);
					m_pGwanTree->DeleteAllItems();
				}

				inData.m_pTreeCtrl = m_pGwanTree;
				inData.m_szSection = "관심그룹";
				m_dwTreeKey_gwan = m_pTreeListMng->LoadTreeList(&inData, "관심그룹");
				m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
			}
			else if(nSel==codesearch_sel_master_stock)	
			{
				if(m_dwTreeKey_master_stock == 0)
				{
					inData.m_pTreeCtrl = m_pMasterTree_stock;
					inData.m_szSection = "주식";
					m_dwTreeKey_master_stock = m_pTreeListMng->LoadTreeList(&inData, "주식");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
//				((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(STOCK_CHART);
			}
			else if(nSel==codesearch_sel_master_elw)	
			{
				if(m_dwTreeKey_master_elw == 0)
				{
					inData.m_pTreeCtrl = m_pMasterTree_elw;
					inData.m_szSection = "ELW";
					m_dwTreeKey_master_elw = m_pTreeListMng->LoadTreeList(&inData, "ELW");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
//				((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(STOCK_CHART);
			}
			else if(nSel==codesearch_sel_master_jisu)	
			{
				if(m_dwTreeKey_master_jisu == 0)
				{
					inData.m_pTreeCtrl = m_pMasterTree_Jisu;
					inData.m_szSection = "업종";
					m_dwTreeKey_master_jisu = m_pTreeListMng->LoadTreeList(&inData, "업종");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
//				((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(UPJONG_CHART);
			}
			else if(nSel==codesearch_sel_master_future)	
			{
				if(m_dwTreeKey_master_future == 0)
				{
					inData.m_pTreeCtrl = m_pMasterTree_future;
					inData.m_szSection = "선물/옵션";
					m_dwTreeKey_master_future = m_pTreeListMng->LoadTreeList(&inData, "선물/옵션");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
//				((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(FUTOPT_CHART);
			}
			else if(nSel==codesearch_sel_master_foreign)	
			{
				if(m_dwTreeKey_master_foreign == 0)
				{
					inData.m_pTreeCtrl = m_pMasterTree_foreign;
					inData.m_szSection = "해외";
					m_dwTreeKey_master_foreign = m_pTreeListMng->LoadTreeList(&inData, "해외");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
//				((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(FOREIGN_CHART);
			}
			else if(nSel==codesearch_sel_oneclick)	
			{
				if(m_dwTreeKey_oneclick == 0)
				{
					inData.m_pTreeCtrl = m_pOneClickTree;
					inData.m_szSection = "빠른종목검색";
					m_dwTreeKey_oneclick = m_pTreeListMng->LoadTreeList(&inData, "빠른종목검색");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
			}
			else if(nSel==codesearch_sel_imsi)	
			{
				if(m_dwTreeKey_FindSearch == 0)
				{
					inData.m_pTreeCtrl = m_pFindSearchTree;
					inData.m_szSection = "사용자조건검색";
					m_dwTreeKey_FindSearch = m_pTreeListMng->LoadTreeList(&inData, "사용자조건검색");
					m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
				}
			}
		}
		else if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == SINGLE_CHART)
		{
			if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			{
				if(nSel==codesearch_stock_sel_master_stock)
				{
					if(m_dwTreeKey_master_stock == 0)
					{
						inData.m_pTreeCtrl = m_pMasterTree_stock;
						inData.m_szSection = "주식";
						m_dwTreeKey_master_stock = m_pTreeListMng->LoadTreeList(&inData, "주식");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(CMasterTypeInfo::STOCK_CHART);
				}
				else if(nSel==codesearch_stock_sel_master_elw)	
				{
					if(m_dwTreeKey_master_elw == 0)
					{
						inData.m_pTreeCtrl = m_pMasterTree_elw;
						inData.m_szSection = "ELW";
						m_dwTreeKey_master_elw = m_pTreeListMng->LoadTreeList(&inData, "ELW");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(CMasterTypeInfo::STOCK_CHART);
				}
				else if(nSel==codesearch_stock_sel_oneclick)	
				{
					if(m_dwTreeKey_oneclick == 0)
					{
						inData.m_pTreeCtrl = m_pOneClickTree;
						inData.m_szSection = "빠른종목검색";
						m_dwTreeKey_oneclick = m_pTreeListMng->LoadTreeList(&inData, "빠른종목검색");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
				}
				else if(nSel==codesearch_stock_sel_imsi)	
				{
					if(m_dwTreeKey_FindSearch == 0)
					{
						inData.m_pTreeCtrl = m_pFindSearchTree;
						inData.m_szSection = "사용자조건검색";
						m_dwTreeKey_FindSearch = m_pTreeListMng->LoadTreeList(&inData, "사용자조건검색");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
				}
			}
			else if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
			{
				if(nSel==codesearch_upjong_sel_master_jisu)
				{
					if(m_dwTreeKey_master_jisu == 0)
					{
						inData.m_pTreeCtrl = m_pMasterTree_Jisu;
						inData.m_szSection = "업종";
						m_dwTreeKey_master_jisu = m_pTreeListMng->LoadTreeList(&inData, "업종");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(CMasterTypeInfo::UPJONG_CHART);
				}
			}
			else if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)
			{
				if(nSel==codesearch_future_sel_master_future)
				{
					if(m_dwTreeKey_master_future == 0)
					{
						inData.m_pTreeCtrl = m_pMasterTree_future;
						inData.m_szSection = "선물/옵션";
						m_dwTreeKey_master_future = m_pTreeListMng->LoadTreeList(&inData, "선물/옵션");
						m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->m_strScreenNo);
					}
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(CMasterTypeInfo::FUTOPT_CHART);
				}
			}
		}
	}

	return 0L;
}

void CTabCodeSearch::ChangeSkinColor(COLORREF clrBkgrnd)
{
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);

	STOutBarColor OLColor;
	if(m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)2, (LPARAM)&OLColor))
	{
//		m_ctrlOutBar.cr3dFace = OLColor.clrOLOutLine;
//		m_ctrlOutBar.cr3dUnFace = OLColor.clrOLOutLine;
//		m_ctrlOutBar.crBackGroundColor1 = OLColor.clrOLBkgrnd;
		m_listCode.SetHeaderColor(OLColor.clrOLBkgrnd, OLColor.clrOLOutLine);
	}
	
	Invalidate();
}

void CTabCodeSearch::ChangeListOrGridCtrl(CString szSection)
{
	if(szSection == "PMASTER")			// Kospi200옵션
	{
		m_nCtrlType = 1;
		m_listCode.ShowWindow(SW_HIDE);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_SHOW);
	}
	else if(szSection == "JPMASTER")	// 주식옵션
	{
		m_nCtrlType = 2;
		m_listCode.ShowWindow(SW_HIDE);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_SHOW);
	}
	else								// 그외
	{
		m_nCtrlType = 0;
		m_listCode.ShowWindow(SW_SHOW);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_HIDE);
	}
}

void CTabCodeSearch::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		m_pGridCtrl->Create(CRect(0,0,0,0), this, ID_GRIDCTRL);
		
		///////////////////////////////////////////////////
		// 각종 설정
		m_pGridCtrl->SetEditable(FALSE);				// CInplaceEdit 설정
//		m_pGridCtrl->SetFont(GetParent()->GetFont());
		m_pGridCtrl->SetGridColor(COLOR_GRIDHEADER);		// Grid Color 설정
		m_pGridCtrl->SetHighlightColor(COLOR_BACK_LABEL);
		m_pGridCtrl->SetBkColor(COLOR_WHITE);			// Grid Back Color 설정
		m_pGridCtrl->EnableColumnHide();				// Column 숨길 수 있게 설정
		m_pGridCtrl->EnableRowHide();					// Row 숨길 수 있게 설정
		m_pGridCtrl->EnableTitleTips(FALSE);			// ToolTip 설정
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);	// Grid Line color

		///////////////////////////////////////////////////
		// 그리드 모양을 Default로 한다
		InitGridRowCol();
	
		// 그리드의 모양을 바꾼다
		ChangeGridRowCol();

		// 그리드에 데이터를 추가한다.
		InsertGridData();

		ChangeListOrGridCtrl("");
	}
}

void CTabCodeSearch::InitGridRowCol(int nRowCount/*=0*/)
{	
	try {
		int nRow, nCol;

		nRow = nRowCount + TITLE_ROW;
		nCol = 3;
		
		m_pGridCtrl->SetRowCount(nRow);				// Row ¼³A¤
		m_pGridCtrl->SetColumnCount(nCol);			// Column ¼³A¤
		m_pGridCtrl->SetFixedColumnCount(0);		// Fixed Column ¼³A¤
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
}

void CTabCodeSearch::ChangeGridRowCol()
{
	int nCol = m_pGridCtrl->GetColumnCount();		// Column Count 구하기
	int nRow = m_pGridCtrl->GetRowCount();			// Row Count 구하기

	// fill rows/cols with text
	for (int row = 0; row < nRow; row++)
		m_pGridCtrl->SetRowHeight(row, ROW_HEIGHT);
	
	for (int col = 0; col < nCol; col++)
		m_pGridCtrl->SetColumnWidth(col, GRID_CALLPUT_WIDTH);

	m_pGridCtrl->SetColumnWidth(COL_CENTER, GRID_CENTER_WIDTH);		
	m_pGridCtrl->Refresh();
}

// 빈 데이터 설정 및 추가
// 나중에 SetItemText(CString) 만으로 데이터를 추가할 수 있게 미리 설정
void CTabCodeSearch::InsertGridData()
{
	int nCol = m_pGridCtrl->GetColumnCount();
	int nRow = m_pGridCtrl->GetRowCount();
	
	// fill rows/cols with text
	int row, col;	
	for (row = 0; row < nRow; row++)
	{
		for (col = 0; col < nCol; col++)
		{ 

			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_BKCLR|GVIF_STATE|GVIF_PARAM;
			Item.nState = GVIS_READONLY | GVIS_MODIFIED ;//| GVIS_DROPHILITED;
			Item.row = row;
			Item.col = col;
			
			Item.nFormat = DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER;

			Item.strText = GetInsertGridDataText(row, col);
			Item.crBkClr = RGB(238,239,243);
			if(row == 0 && col < COL_CENTER)			Item.crBkClr = RGB(244, 180, 180);
			else if(row == 0 && col > COL_CENTER)		Item.crBkClr = RGB(180, 207, 244);
			Item.crFgClr = COLOR_BLACK;
			
			m_pGridCtrl->SetItem(&Item);
		}
	}	
}

CString CTabCodeSearch::GetInsertGridDataText(int row, int col)
{
	if (row != 0 )		return "";

	if (col == COL_CENTER)	return "행사가";
	else if (col < COL_CENTER)	return "콜";
	else if (col > COL_CENTER)	return "풋";

	return "";
}

void CTabCodeSearch::InitMasterData()
{
//	COptionInfo* pOptionInfo = NULL;
//	POSITION pos = m_listOptionInfo.GetHeadPosition();
//	while(pos)
//	{
//		if(pOptionInfo = m_listOptionInfo.GetNext(pos))
//			delete pOptionInfo;
//		pOptionInfo = NULL;
//	}
//	m_listOptionInfo.RemoveAll();
//	return TRUE;
}

void CTabCodeSearch::SetMasterData(LIST_CCodeTypeA* pCodeType)
{
	m_cpHelper.ClearList();
	m_cpHelper.Parsing(pCodeType, m_nCtrlType);

	//CCallPutIMonthData* pMonthData = m_cpHelper.FindItemData("200806");
	int nCount = 0;
	for(POSITION jp=m_cpHelper.m_ListData.GetHeadPosition(); jp;)
	{
		CCallPutIMonthData* pMonthData = m_cpHelper.m_ListData.GetNext(jp);
		TRACE("\n\n월물=%s\n", pMonthData->m_szKey);
		if(pMonthData)
		{
			for(POSITION pos=pMonthData->m_ListData.GetHeadPosition(); pos;)
			{
				CCallPutItemData* pItem = pMonthData->m_ListData.GetNext(pos);
				TRACE("%s [%d] [%d]\n", pItem->m_szPrice, pItem->m_isCall, pItem->m_isPut);
				nCount++;
			}
		}
	}

	InitGridRowCol(nCount);

	// ATM
	CString strPath, strATM;
	strPath.Format("%s\\%s\\%s",  m_pParentDlg->m_strRootPath, "mst", "master\\atm.dat");
	char szAtm[100]; memset(szAtm,0x00,sizeof(szAtm));
	::GetPrivateProfileString("ATM", "PRICE", "", szAtm, 100, strPath);
	strATM = (LPCSTR)(LPSTR)szAtm;

	int nRow = 1;
	CString strCenter;
	for(jp=m_cpHelper.m_ListData.GetHeadPosition(); jp;)
	{
		CCallPutIMonthData* pMonthData = m_cpHelper.m_ListData.GetNext(jp);
		TRACE("\n\n월물=%s\n", pMonthData->m_szKey);
		if(pMonthData)
		{
			for(POSITION pos=pMonthData->m_ListData.GetHeadPosition(); pos;)
			{
				CCallPutItemData* pItem = pMonthData->m_ListData.GetNext(pos);
				TRACE("%s [%d] [%d]\n", pItem->m_szPrice, pItem->m_isCall, pItem->m_isPut);

				pItem->m_szPrice.Remove(' ');
				strCenter.Format("%s %s", pMonthData->m_szKey.Mid(2, 4), pItem->m_szPrice);
				m_pGridCtrl->SetItemFormat(nRow, 0, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER);
				m_pGridCtrl->SetItemText(nRow, 0, pItem->m_isCall ? "O" : "X");
				m_pGridCtrl->SetItemData(nRow, 0, (LPARAM)(LPCSTR)pItem->m_strCallCode);
				m_pGridCtrl->SetItemBkColour(nRow, 0, COLOR_WHITE);
				m_pGridCtrl->SetItemFormat(nRow, 1, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER);
				m_pGridCtrl->SetItemFgColour(nRow, 1, (strATM == pItem->m_szPrice) ? COLOR_RED : COLOR_BLACK);
				m_pGridCtrl->SetItemText(nRow, 1, strCenter);
				m_pGridCtrl->SetItemBkColour(nRow, 1, COLOR_WHITE);
				m_pGridCtrl->SetItemFormat(nRow, 2, DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX|DT_CENTER);
				m_pGridCtrl->SetItemText(nRow, 2, pItem->m_isPut ? "O" : "X");
				m_pGridCtrl->SetItemData(nRow, 2, (LPARAM)(LPCSTR)pItem->m_strPutCode);
				m_pGridCtrl->SetItemBkColour(nRow, 2, COLOR_WHITE);
				nRow++;
			}
		}
	}

	m_pGridCtrl->Refresh();
}

BOOL CTabCodeSearch::GetCodeInfo(int nType, int& row, int& col, CString& strCode)
{
//	int nCallPut, nYear, nMonth, nEvtPrice;

//	int nRowTotal = m_pGridCtrl->GetRowCount();
//	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)
//		return FALSE;
//
//	if (col < COL_CENTER)	{ nCallPut = 2; }
//	else					{ nCallPut = 3; }

//	nYear = m_pOptionItem[row]->m_nYear;
//	nMonth = m_pOptionItem[row]->m_nMonth; 
//	nEvtPrice = m_pOptionItem[row]->m_nEvtPrice;

//	strCode.Format("%1d%02d%1c%1X%03d", nCallPut, 1, nYear, nMonth, nEvtPrice);

	int nRowTotal = m_pGridCtrl->GetRowCount();
	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)
		return FALSE;

	strCode = (LPCSTR)m_pGridCtrl->GetItemData(row, col);
	return TRUE;
}

// row에 해당하는 행사가 찾기
//int  CTabCodeSearch::GetGridExercise(int row)
//{
//	long lPrice = m_lMaxLimit - (row - TITLE_ROW)* 250; 
//
//	return (int)(lPrice / 100);
//}

// 행사가로 set
long CTabCodeSearch::GetRealExercise(int nPrice)
{ 
	long lPrice = nPrice * 100; 
	if (lPrice % 500) lPrice += 50;	
	return lPrice;
}

//*******************************************************************/
/*! Function Name : SetFXMasterData
/*! Function      : FX 종목코드를 리스트에 세팅한다. 
/*! Param         : 
/*! Return        : void 
/*! Create        : 강지원 , 2008/08/06
/*! Comment       : 
//******************************************************************/
void CTabCodeSearch::SetFXMasterData()
{
// --> [Edit]  강지원 2008/10/13
	CStringArray arrCode, arrName;

//	CString strFileName = m_pParentDlg->m_strRootPath + "/system/timesetup.ini";
	CString strFileName = m_pParentDlg->m_strRootPath + "\\Icss\\timesetup.ini";
	

	CString	strTemp, strSection, strKey, strData;
	CString	strTitle, strGmt, strSummerTime, strCityName;

	char	szItem[MAX_PATH];
	int nCount;

	strSection = "TIME";
	nCount = GetPrivateProfileInt (strSection, "COUNT", 0, strFileName); 
	int nCnt;
	for(int i = 0; i < nCount; i++)
	{
		strKey.Format("%d", i+1);
		memset(szItem, 0, MAX_PATH);
		GetPrivateProfileString(strSection, strKey, "", szItem, MAX_PATH, strFileName); 
		strData = strTemp = szItem;
		nCnt = strTemp.Find(",");
		strTitle= strTemp.Left(nCnt);	
		strTemp = strTemp.Mid(nCnt+1);	
		nCnt	= strTemp.Find(",");
		strGmt	= strTemp.Left(nCnt);	
		strSummerTime	= strTemp.Mid(nCnt+1);	
		
		// 도시명만 취득한다.
		nCnt = strTitle.Find('(');	
		strCityName = strTitle.Left(nCnt);	
//		m_stuOptionValue.m_TmstrTitle.Add(strCityName);

//		m_stuOptionValue.m_TmstrGmt.Add(strGmt);
//		nSummer = atoi(strSummerTime);
//		m_stuOptionValue.m_TmdSummerTime.Add(nSummer);

		if ( strSummerTime == "1" )
			strCityName = strTitle + " 섬머타임적용";

		m_listCode.InsertCodeData( strData, strTitle,
								"", "", "", "", '3'	);

	}

//	m_pMasterDataManager->GetMasterData("", arrCode, arrName);

//	for ( int i = 0 ; i < arrCode.GetSize() ; i++ )
//	{
//		m_listCode.InsertCodeData( arrCode.GetAt(i), arrCode.GetAt(i),
//								"", "", "", "", '3'	);
//	}
// <-- [Edit]  강지원 2008/10/13



//// --> [Edit]  이도영 2008/09/23
//// 
//	if (m_srtCodeOrder.GetLength())
//	{
//		CString strCode;
//		int nCount = m_srtCodeOrder.GetLength() / 7;
//
//		for (int i = 0; i < nCount; i++)
//		{
//			strCode = m_srtCodeOrder.Mid(i*7, 7);
//			m_listCode.InsertCodeData( strCode, strCode,
//									"", "", "", "", '3'	);
//		}
//// <-- [Edit]  이도영 2008/09/23
//	}
//	else
//	{
//		CStringArray arrCode, arrName;
//		m_pMasterDataManager->GetMasterData("", arrCode, arrName);
//
//		for ( int i = 0 ; i < arrCode.GetSize() ; i++ )
//		{
//			m_listCode.InsertCodeData( arrCode.GetAt(i), arrCode.GetAt(i),
//									"", "", "", "", '3'	);
//		}
//	}
}


void CTabCodeSearch::SetCurListIndex(int nIndex) 
{
	//	1 베이스이기때문에 1을 빼준다. ( 나중에는 1베이스로 바꾸기때문에 바꿔줌 )
	if ( nIndex < 1 )
	{
		int nPreIndex = m_listCode.GetCurSel();
		m_listCode.SetItemState(nPreIndex, 0, LVIS_SELECTED);
		m_nCurrIndex = -1;
	}
	else
	{
		m_nCurrIndex = nIndex;
		m_listCode.SetCurSel(nIndex - 1);
	}
}


int	 CTabCodeSearch::GetCurListIndex()
{
	return m_listCode.GetCurSel() + 1;
}