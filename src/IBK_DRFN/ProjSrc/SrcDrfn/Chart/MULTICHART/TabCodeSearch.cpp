// TabCodeSearch.cpp : implementation file
//

#include "stdafx.h"
#include "MultiChart.h"
#include "TabCodeSearch.h"
#include "./define/ChartMsgDef.h"
//#include "COMMON_BASE/WMUtil.h"
//#include "./define/IntDef.h"
#include "./define/DragNDropDefine.h"

#include "StdDialog.h"
#include "ChartMng.h"

//#include "../CommonTR/DrdsS31.h"
#include "../../inc/RealUpdate.h"
#include "./control/TrTreeCtrl.h"
#include "../../inc/IAUGwanMng.h"

#include "../chart_common/Grid/ColorTable.h"
#define ROW_HEIGHT 15
#define GRID_CALLPUT_WIDTH	31
#define GRID_CENTER_WIDTH	87
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

#define		IDC_TREEITEM_GWAN		10111
#define		IDC_TREEITEM_STOCK		10112
#define		IDC_TREEITEM_ELW		10113
#define		IDC_TREEITEM_JISU		10114
#define		IDC_TREEITEM_FUTURE		10115
#define		IDC_TREEITEM_FOJIPYO	10116
#define		IDC_TREEITEM_FOREIGN	10117
#define		IDC_TREEITEM_COMMODITY	10118
#define		IDC_TREEITEM_ONECLICK	10119
#define		IDC_TREEITEM_FINDSEARCH	10120

//const UINT RMSG_GWANSIMREGISTER		=   ::RegisterWindowMessage("RMSG_GWANSIMREGISTER");
const UINT RMSG_ADDACCOUNTCODE		=   ::RegisterWindowMessage("RMSG_ADDACCOUNTCODE");
const UINT RMSG_CHECKAVGPRICE		=   ::RegisterWindowMessage("RMSG_CHECKAVGPRICE");
const UINT RMSG_GETCURCHARTINFO		=   ::RegisterWindowMessage("RMSG_GETCURCHARTINFO");
const UINT RMSG_ADDACCOUNTCONC		=   ::RegisterWindowMessage("RMSG_ADDACCOUNTCONC");
const UINT RMSG_RESETCONCDATA		=   ::RegisterWindowMessage("RMSG_RESETCONCDATA");
const UINT RMSG_GETINTERFACE		=	::RegisterWindowMessage(_T("RMSG_GETINTERFACE"));
const UINT RMSG_WMSIZE				=	::RegisterWindowMessage(_T("RMSG_WMSIZE"));
const UINT RMSG_TABCODESEARCH		=	::RegisterWindowMessage(_T("RMSG_TABCODESEARCH"));

#define UDM_TREE_CLICK			WM_USER+612 // 4Byte Code Enter
//#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )

//#define UM_CHANGE_REGKW		   ( WM_USER+525 )		// 관심종목 그룹변경 처리용

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

	m_pGwanTree = NULL;
	m_pUserAccountList = NULL;

	m_bAdvise = FALSE;

	m_bPlay = FALSE;
	m_pQueryData = NULL;
//@유진삭제
//	m_bGridExtend = FALSE;
//@유진삭제

	m_pMasterDataManager = NULL;
	m_pGridCtrl = NULL;

//	m_listOptionInfo.RemoveAll();
	m_pSelectedTree = NULL;
	m_nSelAccount = -1;

	m_pGwanMng = NULL;

	//	HTSMODE_FOPRO : 선옵전용, HTSMODE_HTS : 일반 HTS.
	m_nHtsMode = HTSMODE_HTS;
}


void CTabCodeSearch::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearch)
	DDX_Control(pDX, IDC_CHK_AVGPRICE, m_chkAvgPrice);
	DDX_Control(pDX, IDC_CHK_CONCPRICE, m_chkConcPrice);
	DDX_Control(pDX, IDC_CHK_REPEAT, m_chkRepeat);
	DDX_Control(pDX, IDC_BTRESETOCX, m_btnResetOcx);
	DDX_Control(pDX, IDC_BTPREV, m_btPrev);
	DDX_Control(pDX, IDC_BTNEXT, m_btNext);
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
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
	ON_BN_CLICKED(IDC_BTPORTFOLIO, OnBtportfolio)
	ON_BN_CLICKED(IDC_BTSEARCH, OnBtsearch)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCODE, OnBegindragListcode)
	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LISTCODE, OnBeginRdragListcode)
	ON_NOTIFY(NM_CLICK, IDC_LISTCODE, OnClickListcode)
	ON_NOTIFY(NM_RCLICK, IDC_LISTCODE, OnRclickListcode)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNEXT, OnBtnext)
	ON_BN_CLICKED(IDC_BTPREV, OnBtprev)
	ON_BN_CLICKED(IDC_BTRESETOCX, OnBtresetocx)
	ON_BN_CLICKED(IDC_CHK_AVGPRICE, OnChkAvgprice)
	ON_BN_CLICKED(IDC_CHK_CONCPRICE, OnChkConcprice)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCODE, OnItemchangedListcode)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LISTCODE, OnItemchangingListcode)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )
	ON_REGISTERED_MESSAGE( RMSG_INTREQUEST, OnIntRequest )
	ON_REGISTERED_MESSAGE( RMSG_CONCERNHWND, OnResetAll )
	ON_BN_CLICKED(IDC_BTPLAY, OnBnClickedBtplay)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCODE, OnLvnItemchangedListcode)
	ON_REGISTERED_MESSAGE( RMSG_ADDACCOUNTCODE, OnAddAccountCodeItem )
	ON_REGISTERED_MESSAGE( RMSG_CHECKAVGPRICE, OnCheckAvgPrice )
	ON_REGISTERED_MESSAGE( RMSG_ADDACCOUNTCONC, OnMakeAccountConcData )
	ON_REGISTERED_MESSAGE( RMSG_RESETCONCDATA, OnResetConcData )
	ON_MESSAGE( WMU_OUTBAR_NOTIFY, OnOutBarNotify )

	//@Solomon관심-->
	ON_MESSAGE( WM_NOTIFY_LOAD		, OnWinixNotifyLoad)
	ON_MESSAGE( WM_NOTIFY_SAVE		, OnWinixNotifySave)
	ON_MESSAGE( WM_NOTIFY_GROUP		, OnWinixNotifyGroup)
	ON_MESSAGE( WM_NOTIFY_GROUPLIST	, OnWinixNotifyGroupList)
	ON_MESSAGE( WM_NOTIFY_SAVECODE	, OnWinixNotifySaveCode)
	//@Solomon관심<--

	ON_REGISTERED_MESSAGE(RMSG_WMSIZE, OnRmsgWMSize)
	ON_REGISTERED_MESSAGE( RMSG_TABCODESEARCH , OnRmsgTabCodeSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch message handlers
#define TIMEID_UPDATEWND 10
#define TIMEID_SELECTTREE 20
BOOL CTabCodeSearch::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	IMainInfoManager0002* pMng = (IMainInfoManager0002*)AfxGetPctrInterface(UUID_IMainInfoManager);
	m_pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);;
	if (!pMng)
		return 0;
	
	m_nHtsMode = pMng->GetHtsMode();

	CStdDialog* pStdParent = (CStdDialog*)m_pMultiChartWnd;
	m_pGwanMng = (IWinixGwanManager*)pStdParent->GetParent()->SendMessage(RMSG_GETINTERFACE, 0, 0);

	if(m_pGwanMng)
	{	
		HWND hPlatform = pStdParent->m_hPlatform;
		HWND hOnUser = pStdParent->GetParent()->GetParent()->GetSafeHwnd();
		HWND hMsgReceiver = GetSafeHwnd();
		CString szKey;
		szKey.Format("%08X\t%08X\t%08X\t", hPlatform, hOnUser, hMsgReceiver);
		m_pGwanMng->Advise(szKey);
		
		pStdParent->m_pGwanMng = m_pGwanMng;
	}

	Init_Button();

	SetConfig();

	InitTreeSearch();
	UpdateTreeSearch();	

	// y-splitter
	m_ySplitterY1.BindWithControl(this, IDC_SPLITTERY1);
	m_ySplitterY1.SetMinHeight(30, 50);
	m_ySplitterY1.AttachAsAboveCtrl(10102);
	m_ySplitterY1.AttachAsBelowCtrl(IDC_LISTCODE);
	m_ySplitterY1.RecalcLayout();

	// Grid 초기화
	InitMasterData();
	InitGrid();

	m_listCode.Init(m_pParentDlg,0);

	m_listCode.RemoveAll();
	m_listCode.ShowWindow(SW_HIDE);
	SetTimer(TIMEID_UPDATEWND, 10, NULL);

	m_cmbTime.SetParent(this);
	m_cmbTime.SetCurSel(1);		//004 돌려보기 default값 3초

	if(m_cmbTime.GetCount()==0)
	{
		m_cmbTime.AddString("5초");
		m_cmbTime.AddString("10초");
		m_cmbTime.AddString("15초");
		m_cmbTime.AddString("20초");
		m_cmbTime.AddString("25초");
		m_cmbTime.AddString("30초");
		m_cmbTime.AddString("60초");
		m_cmbTime.SetCurSel(1);
	}
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

// void CTabCodeSearch::OnSize(UINT nType, int cx, int cy) 
// {
// 	CTabDlg::OnSize(nType, cx, cy);	
// 	// TODO: Add your message handler code here
// 	RecalcLayout();
// }

//윈7 64비트에서의 Resize 오류건때문에 이런 식으로 수정.
void CTabCodeSearch::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	PostMessage(RMSG_WMSIZE, 0, 0);
}

LRESULT CTabCodeSearch::OnRmsgWMSize(WPARAM wp, LPARAM lp)
{
	RecalcLayout();
	return 0L;
}

void CTabCodeSearch::OnDestroy() 
{
	CStdDialog* pStdParent = (CStdDialog*)m_pMultiChartWnd;
//@Solomon-091106	if(pStdParent->m_pGwanMng)	pStdParent->m_pGwanMng->SetRegGwansim(this, 0);
	//	pMng->SetRegGwansim(this, 0);
	if(m_pGwanMng)
	{
		m_pGwanMng->UnAdvise();
	}

	if(m_pTreeListMng)
	{
		DWORD dwKey = 0;
		int nSize = m_ctrlOutBar.GetFolderCount();
		for (int i=0; i<nSize; i++)
		{
			dwKey = m_ctrlOutBar.GetFolderData(i);
			if (dwKey)
			{
				m_pTreeListMng->UnLoadTreeList(dwKey);
				m_ctrlOutBar.SetFolderData(i, 0);
			}
		}
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
	m_ImageOutbarIcon.DeleteImageList();

	InitMasterData();
	//AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	 if(m_pGridCtrl)
	 {
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	 } 

	DoClearItemLongDataList();
	CTabDlg::OnDestroy();
}	


CRect CTabCodeSearch::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
//	rect.DeflateRect(2,0,0,0);
	int nMarket = ((CStdDialog*)m_pMultiChartWnd)->m_nMarketType;
	int nHeight = 0;
	if(nMarket == CMasterTypeInfo::UPJONG_CHART && ((CStdDialog*)m_pMultiChartWnd)->m_nChartType == SINGLE_CHART)//KHD
		nHeight = 120;
	switch(nID)
	{
	case 10102 : 
		{
			rect.top = rect.top;// + HEIGHTOFBUTTON*2;
			rect.bottom = rect.top + (m_nHeightOfSearch -nHeight)- GAP_TABDLG;
		}
		break;		
	case IDC_BTGRID_EXTEND:
		rect.top = rect.top + m_nHeightOfSearch + GAP_TABDLG + Y_GAP;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.right  - 20;
		rect.right = rect.right;
		break;	
	case IDC_LISTCODE:
	case ID_GRIDCTRL:
		rect.top = rect.top + (m_nHeightOfSearch -nHeight) + GAP_TABDLG;// + Y_GAP;
		if(m_pSelectedTree == m_pUserAccountList)
			rect.bottom = rect.bottom - GAP_TABDLG * 12 - 2;
		else
			rect.bottom = rect.bottom - GAP_TABDLG * 6 - 2;

		break;		
	case IDC_SPLITTERY1:
		{
			rect.top = rect.top + (m_nHeightOfSearch -nHeight) - 2;
			rect.bottom = rect.top + GAP_TABDLG;
		}
		break;
	case IDC_BTRESETOCX:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 64 + 34 + GAP_TABDLG*2;
		rect.right = rect.left + 34;
		break;

	case IDC_BTPREV:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2 - 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON - 1;
		rect.right = rect.left + 20;
		break;
	case IDC_BTNEXT:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2 - 1;
//		rect.bottom = rect.top + HEIGHTOFBUTTON - 1;
		rect.left = rect.left + 20;
		rect.right = rect.left + 20;
		break;
	case IDC_CMB_TIMER:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2 - 1;
		rect.bottom = rect.top + HEIGHTOFBUTTON - 1;
		rect.left = rect.left + 20 + 2 + 20;
		rect.right = rect.left + 48;
		break;
	case IDC_BTPLAY:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2 - 1;
		rect.left = rect.left + 20 + 2 + 20 + 48 + 2;
		rect.right = rect.left + 20;
		break;
	case IDC_CHK_REPEAT:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2 - 2;
		rect.left = rect.left + 20 + 2 + 20 + 48 + 20 + 4;
		rect.right = rect.left + 50;
		break;
	case IDC_CHK_AVGPRICE:
		rect.top = rect.bottom - ((HEIGHTOFBUTTON * 2) + (GAP_TABDLG * 2));
		rect.bottom = rect.top + HEIGHTOFBUTTON - 1;
		rect.left = rect.left + 6;
		rect.right = rect.left + 70;
		break;
	case IDC_CHK_CONCPRICE:
		rect.top = rect.bottom - ((HEIGHTOFBUTTON * 2) + (GAP_TABDLG * 2));
		rect.bottom = rect.top + HEIGHTOFBUTTON - 1;
		rect.left = rect.left + 86 + 6;
		rect.right = rect.left + 70;
		break;
	default:
		break;
	}
	return rect;
}


void CTabCodeSearch::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listCode);
	if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
		RecalcLayoutCtrl(*m_pGridCtrl);
	//2006.08.26 by LYH UI 변경{{
	//RecalcLayoutCtrl(m_treeSearch);
	RecalcLayoutCtrl(m_ctrlOutBar);
	//2006.08.26 by LYH UI 변경}}
	RecalcLayoutCtrl(m_ySplitterY1);
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
	RecalcLayoutCtrl(m_cmbTime);
	RecalcLayoutCtrl(m_btPlay);

	RecalcLayoutCtrl(m_chkConcPrice);
	RecalcLayoutCtrl(m_chkAvgPrice);
	
	if(m_cmbTime.GetSafeHwnd())
		m_cmbTime.Invalidate();
//004 }}
//@유진삭제
//	RecalcLayoutCtrl(m_btnGridExtend);
//	//20061212 정유진수정
//	//grid column 확장btn 추가 
//	RecalcLayoutCtrl(m_chkColumnExtend);
//	//20061212 수정 end
//@유진삭제
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
	strImageDir = "";
	strImageDir.Format("%s\\outbar_icon.bmp", m_pParentDlg->m_szImageDir);
	bitmap.Attach((HBITMAP)LoadImage(NULL, (LPSTR)(LPCTSTR)strImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
	m_ImageOutbarIcon.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1);
	m_ImageOutbarIcon.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
//@유진삭제 
	//20061213 수정 end

	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		m_pTreeListMng = (ITreeListManager2*)pGateMng->GetInterface(UUID_ITreeListManager2);
		if(!m_pTreeListMng)
			return;

		m_pTreeListMng->SetSendKey(((CStdDialog*)m_pMultiChartWnd)->m_hPlatform, ((CStdDialog*)m_pMultiChartWnd)->GetParent()->GetParent()->GetSafeHwnd());
		m_pTreeListMng->SetGwanInterface((long)m_pGwanMng);

// 		{
// 			m_pTreeListMng->SetKeyData("WrapperWnd", (long)((CStdDialog*)m_pMultiChartWnd)->GetParent()->GetParent()->GetSafeHwnd());
// 			m_pTreeListMng->SetKeyData("WinixWnd", (long)((CStdDialog*)m_pMultiChartWnd)->m_hPlatform);
// 		}

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
		//m_ctrlOutBar.SetFolderImageList(m_pIL_W151H21, 0, 2, 3);
		m_ctrlOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 1, 3);
		//2006.12.07 정유진 수정end
		m_ctrlOutBar.SetOwner(this);

		m_ctrlOutBar.cr3dFace =  RGB(191,211,233);
		m_ctrlOutBar.cr3dUnFace = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
		m_ctrlOutBar.crBackGroundColor1 = RGB(233,234,241);

		int nID = 10111;
		if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == MULTI_CHART)
		{
			m_pGwanTree = CreateTreeCtrl("관심그룹", nID++);
			CreateTreeCtrl("주식", nID++);
			CreateTreeCtrl("ELW", nID++);
			CreateTreeCtrl("업종", nID++);
			CreateTreeCtrl("선물/옵션", nID++);
			if (m_nHtsMode == HTSMODE_FOPRO)
				CreateTreeCtrl("FOMASTER", nID++);
//	20130515 이상신 : 해외지수 삭제.(issue No - 0006748)
//			CreateTreeCtrl("해외", nID++);
			CreateTreeCtrl("상품선물", nID++);
			CreateTreeCtrl("빠른종목검색", nID++);
			CreateTreeCtrl("사용자조건검색", nID++);
		}
		else if(((CStdDialog*)m_pMultiChartWnd)->m_nChartType == SINGLE_CHART)
		{
			int nMarket = ((CStdDialog*)m_pMultiChartWnd)->m_nMarketType;
			switch (nMarket)
			{
			case CMasterTypeInfo::STOCK_CHART:
				m_pGwanTree = CreateTreeCtrl("관심그룹", nID++);

				if (((CStdDialog*)m_pMultiChartWnd)->m_nScreenID != 1115)
					CreateTreeCtrl("주식", nID++);

				CreateTreeCtrl("ELW", nID++);
				CreateTreeCtrl("빠른종목검색", nID++);
				CreateTreeCtrl("사용자조건검색", nID++);
				break;
			case CMasterTypeInfo::UPJONG_CHART:
				CreateTreeCtrl("업종", nID++);
				break;
			case CMasterTypeInfo::FUTOPT_CHART:
				if (((CStdDialog*)m_pMultiChartWnd)->IsChartScreen("OldFuture")==TRUE)	//@과거선물조회화면인지.
					CreateTreeCtrl("과거선물", nID++);
				else
					CreateTreeCtrl("선물/옵션", nID++);
				break;
			case CMasterTypeInfo::STOCKFUTOPT_CHART:
				CreateTreeCtrl("주식선물", nID++);
				break;
			case CMasterTypeInfo::FUOPT_JIPYO_CHART:
				if (m_nHtsMode == HTSMODE_FOPRO)
					CreateTreeCtrl("FOMASTER", nID++);
				break;
			case CMasterTypeInfo::FOREIGN_CHART:
//	20130515 이상신 : 해외지수 삭제.(issue No - 0006748)
//				CreateTreeCtrl("해외", nID++);
				break;
			case CMasterTypeInfo::COMMODITY_CHART:
				CreateTreeCtrl("상품선물", nID++);
				break;
			}
		}

		{	//StdDialog에 등록.
			CString szData;
			szData.Format("%s\t%08X", "TabCodeSearch", m_hWnd);
			WPARAM wp = MAKEWPARAM(drbt_REGWINDOW, 0);
			
			m_pMultiChartWnd->SendMessage(RMSG_DRUSER, wp, (LPARAM)(LPCSTR)szData);
		}

		m_ctrlOutBar.SetSelFolder(0);
	}
}

CTrTreeCtrl* CTabCodeSearch::CreateTreeCtrl(CString strName, UINT nID)
{
	CTrTreeCtrl *pTree = new CTrTreeCtrl;
	pTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, nID);
	pTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);
	m_ctrlOutBar.AddFolderBar(strName, pTree);

//	m_ctrlOutBar.GetItemCount();
//	m_ctrlOutBar.GetItemData()

	return pTree;
}

void CTabCodeSearch::UpdateTreeSearch()
{
	CString strTemp;
	int nPosBuy = -1;
	int nPosSell = -1;
	HTREEITEM hItem = NULL;
	CString *pstr = NULL;	
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
	case UMSG_SENDJCODSYNHIGHLIGHTED:
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
			CString szInputData = pLData->m_szInputData;
			//@Solomon 관심 별도처리.
			if(szInputData.Find("DRFNGWAN:")==0)
			{
				int nSize = strlen("DRFNGWAN:");
				if(m_pGwanMng)
				{
					CStringArray arrCode, arrName;
					m_pGwanMng->GetGwansimItem(szInputData.Mid(nSize, 2), arrCode, arrName);
				}
			}
			else
			{
				m_pQueryData = pLData;
				if(pLData)
				{
					TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
					
					ChangeListOrGridCtrl(pLData->m_szSection);
					long dwReserved=0;
					CString strTitle = m_ctrlOutBar.GetFolderText();
					if (strTitle.Compare(_T("사용자조건검색")) == 0)
					{
						CGetBackData *pData = m_CondTarget.SetGetBackData(((CItemOneClickData*)pLData)->m_szOCPath);
						dwReserved = (long)pData;
					}

					if(m_pTreeListMng)
					{
						m_pTreeListMng->SetSendKey(((CStdDialog*)m_pMultiChartWnd)->m_hPlatform, ((CStdDialog*)m_pMultiChartWnd)->GetParent()->GetParent()->GetSafeHwnd());
						m_pTreeListMng->SetGwanInterface((long)m_pGwanMng);
						m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
					}
				}
			}
			break;
		}
	case UMSG_RUN_AUTOTIMER:
		{
			RunAutoTimer();
		}
		break;
	case UMSG_IS_PLAYSTATE:
		{
			BOOL* pbIsPlayState = (BOOL*)wParam;
			*pbIsPlayState = IsPlayState();
		}
		break;
	}

	if(message == WM_NOTIFY)
	{
		NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
		int nCode = pNmgv->hdr.code;
		
		if(nCode == NM_CLICK)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
//				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
//				if(!bInsertMode)
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
						m_listCode.SetFocus();
					}
					return TRUE;
				}	
			}
		}
		else if(nCode == NM_DBLCLK)			// Left button click
		{
			if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
			{
//				BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
//				if(bInsertMode)
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

	if (strTRCode.Compare(TRNO_MSTCODE) == 0)
	{
		m_CondTarget.ReceiveData(aTRData, dwTRDateLen);
	}
	else
	{
		CStringList listStrJongmokCodeChecked;
		CStringList listStrJongmokCodeHighlighted;
		m_pChartMng->SendMessage(UMSG_GETJCODECHECKEDFROMCHART, (WPARAM)&listStrJongmokCodeChecked, (LPARAM)&listStrJongmokCodeHighlighted);
		m_listCode.CheckJongmokCode(&listStrJongmokCodeChecked,&listStrJongmokCodeHighlighted);
		m_listCode.Invalidate();
		this->SendMessage(UMSG_SETINITIALCHARTDATA);
		m_hRootItemCurrent = m_hRootItemCurrentTemp;
	}
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
	char szTemp[64];
//	CTimerData timerData;	

	CString strCfgFile = ((CStdDialog*)m_pMultiChartWnd)->m_strConfig;
	if(bLoad)
	{
		::GetPrivateProfileString("TabCodeSearch","HeightOfSearch","300",szTemp,sizeof(szTemp), strCfgFile);
		m_nHeightOfSearch = atoi(szTemp);
		if(m_nHeightOfSearch<180)
			m_nHeightOfSearch = DEFAULT_SPLITTER_HEIGHT;	

		::GetPrivateProfileString("TabCodeSearch","SelectedFavoriteItemCode","",szTemp,sizeof(szTemp), strCfgFile);
		m_strFavoriteGrp = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedItemName","",szTemp,sizeof(szTemp),strCfgFile);
		m_strSelectedItemName = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedParentItemName","",szTemp,sizeof(szTemp),strCfgFile);
		m_strSelectedParentItemName = szTemp;

//		m_listCode.SetAlarmColor(m_bUseAlarmBkClr);
//		m_listCode.SetAlarmSound(m_bUseAlarmSound,m_strSoundFile);
//		m_listCode.SetAlarmPeriod(m_nAlarmFrom,m_nAlarmTo);
	}
	else
	{
		strTemp.Format("%d",m_nHeightOfSearch);
		::WritePrivateProfileString("TabCodeSearch","HeightOfSearch",strTemp,strCfgFile);

		::WritePrivateProfileString("TabCodeSearch","SelectedFavoriteItemCode",m_strFavoriteGrp,strCfgFile);
		::WritePrivateProfileString("TabCodeSearch","SelectedItemName",m_strSelectedItemName,strCfgFile);
		::WritePrivateProfileString("TabCodeSearch","SelectedParentItemName",m_strSelectedParentItemName,strCfgFile);
	}
}

void CTabCodeSearch::SetTimerInfo(BOOL bLoad /*= TRUE*/)
{
	CString strTemp;
	char szTemp[64];
	long lCntOfTimer = 0;

	CString strCfgFile = ((CStdDialog*)m_pMultiChartWnd)->m_strConfig;
	if(bLoad)
	{
		::GetPrivateProfileString("TabCodeSearch","CntOfUnit","0",szTemp,sizeof(szTemp),strCfgFile);
		lCntOfTimer = atoi(szTemp);
	}
	else
	{
		::WritePrivateProfileString("TabCodeSearch","CntOfUnit",strTemp,strCfgFile);
	}
}

void CTabCodeSearch::OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bLBtnDown = TRUE;
	SetCapture();	
	*pResult = 0;
}

void CTabCodeSearch::OnBeginRdragListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bRBtnDown = TRUE;
	SetCapture();	
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

void CTabCodeSearch::RequestMstCode(LPCTSTR lpBuffer,long lLength)
{
	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)lpBuffer;
	st.m_lLength= lLength;
	st.m_strTRCode = TRNO_MSTCODE;
	m_pMultiChartWnd->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	
}

void CTabCodeSearch::OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	

	((CStdDialog*)m_pMultiChartWnd)->ChangeCode(m_listCode.GetCodeSelected());

	*pResult = 0;
}

void CTabCodeSearch::OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	{

		CString strCodeSelected = m_listCode.GetCodeSelected();
		if(!strCodeSelected.IsEmpty())
		{
			((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
		}
	}
	*pResult = 0;
}

void CTabCodeSearch::OnBtall() 
{
	// TODO: Add your control notification handler code here
	int nItemCount = m_listCode.GetItemCount();
	for(int nIndex=0;nIndex<nItemCount;nIndex++)
	{
		m_listCode.SetCurSel(nIndex);
	}	
}

void CTabCodeSearch::OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CString strCodeSelected = m_listCode.GetCodeSelected();
	if(!strCodeSelected.IsEmpty())
	{
		((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
	}

	*pResult = 0;
}

void CTabCodeSearch::OnTimer(UINT nIDEvent) 
{
//004 {{
	if(AUTO_TIMER == nIDEvent)
	{
		long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
		BOOL bRet = m_listCode.SetCursorNext(1, m_chkRepeat.GetCheck());
		if(bRet)
		{
			KillTimer(nIDEvent);
			ApplySelectedCodes();
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
	else if(TIMEID_SELECTTREE == nIDEvent)
	{
		KillTimer(nIDEvent);

		if(m_pSelectedTree)
			m_pSelectedTree->ClickDefault();
	}
	
	CTabDlg::OnTimer(nIDEvent);
}

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

void CTabCodeSearch::OnBtnext() 
{
	// TODO: Add your control notification handler code here
	if(m_listCode.GetItemCount() < 1)
		return;
//	m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);
	long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
	m_listCode.SetCursorNext(lCntOfChart, m_chkRepeat.GetCheck());
	ApplySelectedCodes();
}

void CTabCodeSearch::OnBtprev() 
{
	// TODO: Add your control notification handler code here
	if(m_listCode.GetItemCount() < 1)
		return;
//	m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);
	long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
	m_listCode.SetCursorPrev(lCntOfChart, m_chkRepeat.GetCheck());
	ApplySelectedCodes();
}

void CTabCodeSearch::OnBtresetocx() 
{
	// TODO: Add your control notification handler code here
	m_pChartMng->SendMessage(UMSG_RESETOCX);
}


void  CTabCodeSearch::DeleteFavoriteChild()
{
}



LRESULT CTabCodeSearch::OnIntRequest( UINT wParam, LONG lParam )
{
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
}


void CTabCodeSearch::AdviseMain(int nAdivise)
{
}


void CTabCodeSearch::SetAccountInfo(LPCTSTR lpAccountInfo)
{
}

void CTabCodeSearch::RequestSavedQuery()
{
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
}

void CTabCodeSearch::Init_Button()
{
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
}

void CTabCodeSearch::CTLMCallback::ReceiveData(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData)
{
	if (pData == NULL)
		return ;

	METHOD_PROLOGUE(CTabCodeSearch, TLMCallback);

	CString szScreenNo = ((CStdDialog*)pThis->m_pMultiChartWnd)->GetScreenNo();
	BOOL bChkCode = FALSE;
	if (szScreenNo.Compare("7112")==0)	//KHD : 주식종합에서 다른 시장 정보 나오지 않게
		bChkCode = TRUE;

	if (aDataType=='A')
	{
		if (pThis->m_nCtrlType == 1 || pThis->m_nCtrlType == 2 || pThis->m_nCtrlType == 3)	// 1:Kospi200옵션, 2:주식옵션, 3:달러옵션
		{
			LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
			if (pListTypeA)
			{
				pThis->m_bInsertingInListCode = FALSE;
				pThis->SetMasterData(pListTypeA);
				pThis->m_bInsertingInListCode = FALSE;
			}
		}
		else														// 그외
		{
			LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
			pThis->m_listCode.SetRedraw(FALSE);

			if (nLength >= 0)
			{
				pThis->m_listCode.RemoveAll();
				pThis->m_listCode.m_nIndex = 0;
			}

			if (bChkCode)
				_DoFilterStockCode(pListTypeA);

			BOOL bUserSearch = FALSE;
			if (dwReserved != NULL)	// 사용자 조건검색.
			{
				CGetBackData *pData = (CGetBackData*)dwReserved;
				pThis->m_CondTarget.LoadTdfData(pData);
				bUserSearch = TRUE;
			}

			if (pListTypeA)
			{
				pThis->m_bInsertingInListCode = TRUE;
				CRect rc;
				pThis->m_listCode.GetClientRect(rc);

				int nIndex = 0;
				int nCnt = rc.Height()/17;
				int nMarketType = CMasterTypeInfo::STOCK_CHART;

				for (POSITION pos=pListTypeA->GetHeadPosition(); pos; )
				{
					CCodeTypeA typeA = pListTypeA->GetNext(pos);
					typeA.m_szhname.Replace("&", "&&");

					if (bUserSearch)	// 사용자 조건검색.
					{
						if (pThis->m_CondTarget.CheckTargetResult(typeA.m_szcode) == FALSE)
							continue;
					}

					pThis->m_listCode.InsertCodeData(typeA.m_szcode, typeA.m_szhname, "", "", "", "", '3');

					if (pListTypeA->GetCount()>0 && pListTypeA->GetCount()<=200)
					{
						pThis->m_strCodes += ";";
						pThis->m_strCodes +=typeA.m_szcode;
					}

					nIndex++;
					if (nIndex == nCnt)
					{
						pThis->m_listCode.SetRedraw(TRUE);
						pThis->m_listCode.SetItemCountEx(nCnt);
					}
				}
				pThis->m_listCode.SetRedraw(TRUE);
//				pThis->m_listCode.SetItemCountEx(pListTypeA->GetCount());
				pThis->m_listCode.SetItemCountEx(nIndex);
				pThis->m_bInsertingInListCode = FALSE;
			}
		}
	}
	else
	{
		LIST_CCodeTypeB *pListTypeB = (LIST_CCodeTypeB*)pData;
		pThis->m_listCode.SetRedraw(FALSE);

		if (nLength >= 0)
		{
			if (pThis->m_bAdvise)
			{
				if (pThis->m_strCodes.GetLength() > 0)
					pThis->MakeDrdsUnConnectUnAdvise(pThis->m_strCodes);
				pThis->m_strCodes = "";
			}
			pThis->m_listCode.RemoveAll();
			pThis->m_listCode.m_nIndex = 0;
		}
		if (pListTypeB)
		{
			long	lUpLimit = 0;
			long	lUp = 0;
			long	lSteady = 0;
			long	lDown = 0;
			long	lDownLimit = 0;
			long	lIndexCnt = 0;
			double	dDaebiRateTotal = 0;

			pThis->m_bInsertingInListCode = TRUE;
			CRect rc;
			pThis->m_listCode.GetClientRect(rc);
			int nCnt = rc.Height()/17;
			int nIndex = 0;

			for (POSITION pos=pListTypeB->GetHeadPosition(); pos; )
			{
				CCodeTypeB typeB = pListTypeB->GetNext(pos);
				if (typeB.m_szsign.GetLength()==0)
					typeB.m_szsign = EQUAL;

				switch(typeB.m_szsign[0])
				{
					case UP_LIMIT:		lUpLimit++;		break;	// UpLimit
					case UP:			lUp++;			break;	// Up
					case EQUAL:			lSteady++;		break;	// Steady
					case DOWN:			lDown++;		break;	// Down
					case DOWN_LIMIT:	lDownLimit++;	break;	// DownLimit
				}
				pThis->m_listCode.InsertCodeData(typeB.m_szcode, typeB.m_szhname, typeB.m_szprice, typeB.m_szchange,
										typeB.m_szchrate, typeB.m_szvolume, typeB.m_szsign[0], typeB.m_cGb - '0');

				if (pListTypeB->GetCount()>0 && pListTypeB->GetCount()<=200)
				{
					pThis->m_strCodes += ";";
					pThis->m_strCodes +=typeB.m_szcode;
				}

				nIndex++;
				if (nIndex == nCnt)
				{
					pThis->m_listCode.SetRedraw(TRUE);
					pThis->m_listCode.SetItemCountEx(nCnt);
				}
			}
			pThis->m_listCode.SetRedraw(TRUE);
			pThis->m_listCode.SetItemCountEx(pListTypeB->GetCount());

			//{{20070312 by LYH 종합차트 여러개 띄웠을 경우 관심리스트 리얼 못 받는 버그 수정
			ITrComm* pTrComm = (ITrComm*)pThis->m_pQueryData->m_pITrComm;
			if (pTrComm)
			{
				CString strCompare;
				strCompare.Format("%s", pThis->m_pQueryData->m_szMethod);

				if(strCompare.Compare("USERTR") == 0)
					pTrComm->ChangeCallback(&pThis->m_xMyITrCommSite);
			}
			//}}

			pThis->m_bInsertingInListCode = FALSE;
		}
	}
}

int CTabCodeSearch::CTLMCallback::_DoFilterStockCode(LIST_CCodeTypeA *pListTypeA)
{
	int nCount = 0;
	
	POSITION tPos;
	int nSkip = 0;
	for(POSITION pos=pListTypeA->GetHeadPosition(); pos; )
	{
		tPos = pos;
		CCodeTypeA typeA = pListTypeA->GetNext(pos);
		if(typeA.m_szcode.Trim().GetLength()!=6)
		{
			pListTypeA->RemoveAt(tPos);
			nCount++;
		}
	}
	
	return nCount;
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
		
		pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
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
		
		pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
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
/*
void CTabCodeSearch::OnBtnRetry() 
{
	if(m_pQueryData)
	{
		long dwReserved=0;
		if(m_pTreeListMng) 
		{
			m_pTreeListMng->SetSendKey(((CStdDialog*)m_pMultiChartWnd)->m_hPlatform, ((CStdDialog*)m_pMultiChartWnd)->GetParent()->GetParent()->GetSafeHwnd());
			m_pTreeListMng->SetGwanInterface((long)m_pGwanMng);
			m_pTreeListMng->RequestData(m_pQueryData, &m_xTLMCallback, 'B', '1', dwReserved);
		}
	}
}
*/
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
		//>>20110210_alzioyes:[A00000221]차트스크롤기능과 종목돌려보기 동시에 안되도록.
		if(m_pChartMng->SendMessage(RMSG_GETINFO, 1, 0) != 0)	//SCROLL_STOP
		{
			AfxMessageBox("차트스크롤 기능이 동작중입니다.");
			return;
		}
		//<<20110210_alzioyes:[A00000221]차트스크롤기능과 종목돌려보기 동시에 안되도록.

		// 플레이
//		m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);

		long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);

		//>>20110210_alzioyes:[A00000222]차트가 N*N으로 보고 있을 경우 돌려보기 기능 작동은 막기.(솔로몬요처사항)
		if(lCntOfChart>0)
		{		
			
			if( lCntOfChart>1 && !m_pChartMng->SendMessage( UMSG_RELAY_POSSSIBLE_CHECK ) )				
			{				
				AfxMessageBox("N*N 상태에서는 돌려보기 기능이 제한됩니다.");
				return;
			}
			else if(  lCntOfChart>1 && m_pMultiChartWnd && !m_pMultiChartWnd->SendMessage( UMSG_GETJONGMOKLINK, 0, 0 ) )
			{				
				AfxMessageBox("N*N 상태에서는 돌려보기 기능이 제한됩니다.");
				return;
			}
			
			long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
			//2007.01.27 by LYH 처음 버튼 눌렸을때는 마지막일경우 처음부터 시작
			//m_listCode.SetCursorNext(lCntOfChart, m_chkRepeat.GetCheck());
			m_listCode.SetCursorNext(1, TRUE);
			//}}
			ApplySelectedCodes();
		}		

		RunAutoTimer();
//		if(lCntOfChart == 1)
//		{
//			int nCur = m_cmbTime.GetCurSel();
//			switch(nCur) 
//			{
//			case 0:		// 2sec
//				SetTimer(AUTO_TIMER, 2000, NULL);
//				break;
//			case 1:		// 3sec
//				SetTimer(AUTO_TIMER, 3000, NULL);
//				break;
//			case 2:		// 4sec
//				SetTimer(AUTO_TIMER, 4000, NULL);
//				break;
//			case 3:		// 6sec
//				SetTimer(AUTO_TIMER, 6000, NULL);
//				break;
//			case 4:		// 8sec
//				SetTimer(AUTO_TIMER, 8000, NULL);
//				break;
//			case 5:		// 10sec
//				SetTimer(AUTO_TIMER, 10000, NULL);
//				break;
//			default:
//				break;
//			}
//		}
//		else if(lCntOfChart > 1)
//		{
//			double dTime = lCntOfChart * 1.5 * 1000;
//			int nTime = dTime;
//			SetTimer(AUTO_TIMER, nTime, NULL);
//		}
		m_btPlay.FP_vSetImage(m_pIL_W20H20, 64, 66);
		m_btPlay.FP_vSetToolTipText("정지");
		m_btPlay.Invalidate();
		//Invalidate(TRUE);
		m_bPlay = TRUE;	
	}

	m_pChartMng->SendMessage(RMSG_SETINFO, 1, (LPARAM)m_bPlay);	//20110210_alzioyes:[A00000221]
}

void CTabCodeSearch::RemoveAllTree()
{
	CTrTreeCtrl *pTree = NULL;
	int nSize = m_ctrlOutBar.GetFolderCount();
	for (int i=0; i<nSize; i++)
	{
		pTree = (CTrTreeCtrl *)m_ctrlOutBar.GetFolderChild(i);
		if (pTree)
		{
			pTree->DestroyWindow();
			delete pTree;
			pTree = NULL;
		}
	}

	m_pGwanTree = NULL;

/*
	if(m_pUserAccountList)
	{
		m_pUserAccountList->DestroyWindow();
		delete m_pUserAccountList;
	}
*/
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
	//@Solomon:091113SM039
	{
		CChartMng* pChartMng = (CChartMng*)m_pChartMng;
		CInputBarDlg* pInputBarDlg = (CInputBarDlg*)pChartMng->GetpInputBarDlg();
		pInputBarDlg->m_strCode = "";
	}

	// TODO: Add your control notification handler code here
	UINT nCntSelected = m_listCode.GetSelectedCount();
	m_pMultiChartWnd->SendMessage(UMSG_CHANGEMODEBYINPUTBAR,0,0);
	if(nCntSelected==1)
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = m_listCode.GetCodeSelected();
		stInputCodeData.m_strTime = m_listCode.GetTimeSelected();
		stInputCodeData.m_nDefault = MODE_DEFAULT;
		stInputCodeData.m_nMode = CHANGE_ITEM;

		((CStdDialog*)m_pMultiChartWnd)->ChangeCode(m_listCode.GetCodeSelected(), MODE_USER, CHANGE_ITEM);
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

UINT THREAD_PROCEEDSELECTTREE(LPVOID lpVoid)
{
	CTrTreeCtrl* pSelectedTree = (CTrTreeCtrl*)lpVoid;
	if(pSelectedTree)
		pSelectedTree->ClickDefault();

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
	if (wParam==NM_FOLDERCHANGE)
		fnFolderChange(((CStdDialog*)m_pMultiChartWnd)->m_nChartType, (int)lParam);

	return 0L;
}

void CTabCodeSearch::fnFolderChange(int nChartType, int nSel)
{
	CTrTreeCtrl* pTree = NULL;
	pTree = (CTrTreeCtrl*)m_ctrlOutBar.GetFolderChild(nSel);
	if (pTree)
	{
		DWORD dwKey = m_ctrlOutBar.GetFolderData(nSel);
		CString strTitle = m_ctrlOutBar.GetFolderText(nSel);
		dwKey = ChangeTreeFolder(strTitle, pTree, dwKey, FALSE);
		m_ctrlOutBar.SetFolderData(nSel, dwKey);
	}

	ShowAccountOperator(FALSE);

	AfxBeginThread(THREAD_PROCEEDSELECTTREE, m_pSelectedTree);
}

long CTabCodeSearch::ChangeTreeFolder(CString strName, CTrTreeCtrl *pTree, long lTreeKey, BOOL bChangeMode)
{
	if (lTreeKey == 0)
	{
		CInputCodeListEx inData;
		inData.m_pTreeCtrl = pTree;
		inData.m_szSection = strName;
		if(m_pTreeListMng) m_pTreeListMng->SetGwanInterface((long)m_pGwanMng);
		m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pMultiChartWnd)->GetScreenNo());
		lTreeKey = m_pTreeListMng->LoadTreeList(&inData, strName);
	}

	if (bChangeMode)
		((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType);

	m_pSelectedTree = pTree;

	return lTreeKey;
}

void CTabCodeSearch::ChangeSkinColor(COLORREF clrBkgrnd)
{
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);

	STOutBarColor OLColor;
	if(m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)2, (LPARAM)&OLColor))
	{
		m_ctrlOutBar.cr3dFace = OLColor.clrOLOutLine;
		m_ctrlOutBar.cr3dUnFace = OLColor.clrOLOutLine;
		m_ctrlOutBar.crBackGroundColor1 = OLColor.clrOLBkgrnd;
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
	else if(szSection == "달러옵션")	// 달러옵션
	{
		m_nCtrlType = 3;
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
		//AFX_MANAGE_STATE(AfxGetStaticModuleState()); //0004814: [7111] 종합차트 - 특정 고객 PC에서 차트를 오픈하면 죽는 현상

		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		if(m_pGridCtrl->Create(CRect(0,0,0,0), this, ID_GRIDCTRL)==TRUE)
		{		
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
	CString strKey, strMonth, strWeek;
	POSITION jp;
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

				strMonth = (pMonthData->m_szKey).Mid(2, 2);
				strWeek = (pMonthData->m_szKey).Mid(4, 2);

				if ((pItem->m_strCallCode).Mid(1, 2) == "AF" && (pItem->m_strPutCode).Mid(1, 2) == "AF")
					strKey.Format("%s.M%s", strMonth, strWeek);
				else
					strKey.Format("%s.%s", strMonth, strWeek);

				pItem->m_szPrice.Remove(' ');
				strCenter.Format("%s %s", strKey, pItem->m_szPrice);
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

void CTabCodeSearch::RunAutoTimer()
{
	int nCur = m_cmbTime.GetCurSel();
	if(nCur<0) return;

	//20110124_alzioyes: 5, 10 ,15, 20, 25, 30, 60 솔로몬에서 요청한 값.
	CString rString;
	m_cmbTime.GetLBText(nCur, rString);
	int nPos = rString.Find("초");
	if(nPos<0) return;

	rString = rString.Left(nPos);
	rString.TrimRight();

	UINT nTimerVal = (1000)* atoi(rString);
	SetTimer(AUTO_TIMER, nTimerVal, NULL);

// 	switch(nCur) 
// 	{
// 	case 0:		// 3sec
// 		SetTimer(AUTO_TIMER, 3000, NULL);
// 		break;
// 	case 1:		// 5sec
// 		SetTimer(AUTO_TIMER, 5000, NULL);
// 		break;
// 	case 2:		// 6sec
// 		SetTimer(AUTO_TIMER, 6000, NULL);
// 		break;
// 	case 3:		// 7sec
// 		SetTimer(AUTO_TIMER, 7000, NULL);
// 		break;
// 	case 4:		// 8sec
// 		SetTimer(AUTO_TIMER, 8000, NULL);
// 		break;
// 	case 5:		// 10sec
// 		SetTimer(AUTO_TIMER, 10000, NULL);
// 		break;
// 	default:
// 		break;
// 	}
}

BOOL CTabCodeSearch::IsPlayState()
{
	return m_bPlay;
}

HRESULT CTabCodeSearch::OnAddAccountCodeItem(WPARAM wParam, LPARAM lParam)
{
	char szCount[6] = {NULL, };
	::memcpy(szCount, (char*)lParam, 5);

	int nCount = atoi(szCount);
	if(nCount >= 0)
	{
		m_listCode.RemoveAll();
		m_listCode.m_nIndex = 0;
	}

	m_bInsertingInListCode = TRUE;
	CRect rc;
	m_listCode.GetClientRect(rc);
	
	int nIndex = 0;
	int nCnt = rc.Height()/17;

	CString strCode, strExtCode, strName, strPrice;

	for(int i = 0; i < nCount; i++)
	{
		LPa4017Out2 pCodeData = (LPa4017Out2)(lParam + 5 + (sizeof(a4017Out2) * i));
		strExtCode.Format("%12.12s", pCodeData->expcode);
		strCode.Format("%6.6s", pCodeData->expcode + 3);
		strName.Format("%20.20s", pCodeData->codename);
		strPrice.Format("%9.9s", pCodeData->pamt);
		
		m_listCode.InsertCodeData(
			strCode,
			strName,
			strPrice, "", "", strExtCode, '3'
			);
		
		if(nCount > 0 && nCount <= 200)
		{
			m_strCodes += ";";
			m_strCodes +=strCode;
		}
		
		nIndex++;
		if(nIndex == nCnt) {
			m_listCode.SetRedraw(TRUE);
			m_listCode.SetItemCountEx(nCnt);
		}
	}
	m_listCode.SetRedraw(TRUE);
	m_listCode.SetItemCountEx(nCount);
	m_bInsertingInListCode = FALSE;

	return 0L;
}

void CTabCodeSearch::ShowAccountOperator(BOOL bShow)
{
	if(bShow)
	{
		m_chkAvgPrice.ShowWindow(SW_SHOW);
		m_chkConcPrice.ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkAvgPrice.SetCheck(FALSE);
		m_chkConcPrice.SetCheck(FALSE);

		OnChkAvgprice();	
		OnChkConcprice();
		
		m_nSelAccount = -1;

		m_chkAvgPrice.ShowWindow(SW_HIDE);
		m_chkConcPrice.ShowWindow(SW_HIDE);
	}

	RecalcLayout();
}

void CTabCodeSearch::OnChkAvgprice() 
{
	BOOL bCheck = m_chkAvgPrice.GetCheck();
	
	if(m_nSelAccount == LB_ERR)
		return;
	
	CString strPrice, strTemp;
	char cTemp;
	m_listCode.GetCodeData(m_nSelAccount, strPrice, strTemp, strTemp, strTemp, cTemp);
	
	CString strCode = m_listCode.GetCodeSelected();

	if(bCheck)
	{
		strPrice.Replace(",", "");
		((CChartMng*)m_pChartMng)->ShowAvgLine(strCode, strPrice);	
	}
	else
		((CChartMng*)m_pChartMng)->ShowAvgLine(strCode, "");	
}

HRESULT CTabCodeSearch::OnCheckAvgPrice(WPARAM wParam, LPARAM lParam)
{
	if(m_nSelAccount == LB_ERR)
		return 0L;

	CString strCode = (LPTSTR)(LPCTSTR)lParam;
	CString strSelCode = m_listCode.GetCodeSelected(m_nSelAccount);

	if(strCode == strSelCode)
		OnChkAvgprice();

	OnChkConcprice();
	
	return 0L;
}

void CTabCodeSearch::OnChkConcprice() 
{
	// 주식만
	if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType != 0)
		return;

//	if(m_nSelAccount == LB_ERR)
//		return;

	CChartMng* pChartMng = (CChartMng*)m_pChartMng;
	if(m_chkConcPrice.GetCheck())
	{
		HTREEITEM hItem = m_pUserAccountList->GetSelectedItem();
		if(!hItem)
			return;

		CString strExtCode(_T(""));
		CChartMng* pChartMng = (CChartMng*)m_pChartMng;
		if(pChartMng)
		{
			CInputBarDlg* pInputBarDlg = (CInputBarDlg*)pChartMng->GetpInputBarDlg();
			if(pInputBarDlg)
			{
				CString strCode = pInputBarDlg->GetCurrentCode();
				strExtCode = ((CStdDialog*)m_pMultiChartWnd)->GetItemExpCode(strCode);
			}
		}

		if(strExtCode.IsEmpty())
			return;
/*
		CString strPrice, strTemp, strExtCode;
		m_listCode.GetCodeData(m_nSelAccount, strPrice, strTemp, strTemp, strExtCode, cTemp);
*/
		
		a4018In sta4018In;
		::memset(&sta4018In, 0x20, sizeof(sta4018In));
		
		CString strAccount = m_pUserAccountList->GetItemText(hItem);
		strAccount.Replace("-", "");

		::memcpy(sta4018In.accno, strAccount, sizeof(sta4018In.accno));
		::memcpy(sta4018In.expcode, strExtCode, sizeof(sta4018In.expcode));
		
		CString strNumber, strDate, strTime;
		long lCount = 0;
		
		char cTemp;
		pChartMng->GetCurStatusInfo(cTemp, strNumber, strDate, strTime, lCount);
		
		CString strTemp;
		if(cTemp == '1')		// 분
		{
			sta4018In.gubun[0] = '2';

			strTime.Replace(":", "");
			strTemp.Format("%s00", strTime.Right(4));
			::memcpy(sta4018In.basetm, strTemp, sizeof(sta4018In.basetm));
		}
		else if(cTemp == '2')	// 일
		{
			sta4018In.gubun[0] = '1';
			if(atoi(strNumber) != 1)
			{
				pChartMng->RemoveConcPacketData();
				return;
			}
		}
		else 
			return;
		
// 		::memcpy(sta4018In.term, strNumber, sizeof(sta4018In.term));
// 		::memcpy(sta4018In.basedt, strDate, sizeof(sta4018In.basedt));
// 		
// 		strTemp.Format("%ld", lCount);
// 		::memcpy(sta4018In.cnt, strTemp, sizeof(sta4018In.cnt));
// 		
// 		((CStdDialog*)m_pMultiChartWnd)->RequestAccountData(NAME_a4018, (char*)&sta4018In, sizeof(sta4018In));
	}
	else
	{
		pChartMng->RemoveConcPacketData();
	}
}

HRESULT CTabCodeSearch::OnMakeAccountConcData(WPARAM wParam, LPARAM lParam)
{
	CChartMng* pChartMng = (CChartMng*)m_pChartMng;

	char szCount[6] = {NULL, };
	::memcpy(szCount, (char*)lParam, 5);

	int nCount = atoi(szCount);
	long nPacketCount = pChartMng->GetPacketCount("자료일자");

//	CString strConcTime;
//	int nConcTimeBufferLength = 8 * nPacketCount + 1;
//	char* szConcTimeChartData = strConcTime.GetBuffer(nConcTimeBufferLength);
	
//	::memset(szConcTimeChartData, 0x20, nConcTimeBufferLength);

	CString strSellCountData;
	int nSellCountBufferLength = 5 * nPacketCount + 1;
	char* szSellCountChartData = strSellCountData.GetBuffer(nSellCountBufferLength);

	::memset(szSellCountChartData, 0x20, nSellCountBufferLength);
	
	CString strSellPriceData;
	int nSellPriceBufferLength = 9 * nPacketCount + 1;
	char* szSellPriceChartData = strSellPriceData.GetBuffer(nSellPriceBufferLength);

	::memset(szSellPriceChartData, 0x20, nSellPriceBufferLength);

	CString strBuyCountData;
	int nBuyCountBufferLength = 5 * nPacketCount + 1;
	char* szBuyCountChartData = strBuyCountData.GetBuffer(nBuyCountBufferLength);

	::memset(szBuyCountChartData, 0x20, nBuyCountBufferLength);
	
	CString strBuyPriceData;
	int nBuyPriceBufferLength = 9 * nPacketCount + 1;
	char* szBuyPriceChartData = strBuyPriceData.GetBuffer(nBuyPriceBufferLength);

	::memset(szBuyPriceChartData, 0x20, nBuyPriceBufferLength);

	char cTemp;
	CString strNumber, strDate, strTime;
	long lCount = 0;

	pChartMng->GetCurStatusInfo(cTemp, strNumber, strDate, strTime, lCount);

	char szDate[9];
	for(int i = 0; i < nCount; i++)
	{
		LPa4018Out2 pCodeData = (LPa4018Out2)(lParam + 5 + (sizeof(a4018Out2) * i));
		
		::memset(szDate, 0x00, sizeof(szDate));
		::memcpy(szDate, pCodeData->procdt, sizeof(pCodeData->procdt));

		int nIndex = -1;
		if(cTemp == '1')
		{
			strDate = szDate;
			strDate.TrimRight();

			::memset(szDate, 0x00, sizeof(szDate));
			::memcpy(szDate, pCodeData->hhmm, sizeof(pCodeData->hhmm));

			strNumber = szDate;
			strNumber.TrimRight();

			strTime.Format("%s%s", strDate.Right(4), strNumber);
			::memcpy(szDate, strTime, sizeof(pCodeData->procdt));
			
			//nIndex = pChartMng->GetPacketNearIndex("자료일자", atof(szDate));
			nIndex = pChartMng->GetPacketNearIndex("_DateTime_", atof(szDate));
		}
		else
			//nIndex = pChartMng->GetPacketIndex("자료일자", atof(szDate));
			nIndex = pChartMng->GetPacketIndex("_DateTime_", atof(szDate));

		if(nIndex < 0)
			continue;

//		memcpy(szConcTimeChartData + (sizeof(pCodeData->procdt) * nIndex), pCodeData->procdt, sizeof(pCodeData->procdt));
		memcpy(szSellCountChartData + (sizeof(pCodeData->scnt) * nIndex), pCodeData->scnt, sizeof(pCodeData->scnt));
		memcpy(szSellPriceChartData + (sizeof(pCodeData->sconcprc) * nIndex), pCodeData->sconcprc, sizeof(pCodeData->sconcprc));
		memcpy(szBuyCountChartData + (sizeof(pCodeData->bcnt) * nIndex), pCodeData->bcnt, sizeof(pCodeData->bcnt));
		memcpy(szBuyPriceChartData + (sizeof(pCodeData->bconcprc) * nIndex), pCodeData->bconcprc, sizeof(pCodeData->bconcprc));
	}

//	strConcTime.ReleaseBuffer();
	strSellCountData.ReleaseBuffer();
	strSellPriceData.ReleaseBuffer();
	strBuyCountData.ReleaseBuffer();
	strBuyPriceData.ReleaseBuffer();

	pChartMng->AddConcPacketData(NULL, nPacketCount,
								strSellCountData, nPacketCount,
								strSellPriceData, nPacketCount,
								strBuyCountData, nPacketCount,
								strBuyPriceData, nPacketCount);
	return 0L;
}

HRESULT CTabCodeSearch::OnResetConcData(WPARAM wParam, LPARAM lParam)
{
	m_chkAvgPrice.SetCheck(FALSE);
	m_chkConcPrice.SetCheck(FALSE);

	OnChkAvgprice();	
	OnChkConcprice();
	
	m_nSelAccount = -1;
	
	return 0L;
}

void CTabCodeSearch::OnItemchangedListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if( !pNMListView) return;
//	if( !( pNMListView->uNewState & LVIS_SELECTED) || ( pNMListView->uOldState & LVIS_SELECTED)) return;
	
/*
	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = m_listCode.GetCodeSelected(pNMListView->iItem);
	stInputCodeData.m_nDefault = MODE_DEFAULT;
	stInputCodeData.m_nMode = CHANGE_ITEM;
	if(stInputCodeData.m_strCode.GetLength()>0)
		m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
*/

	*pResult = 0;
}

void CTabCodeSearch::OnItemchangingListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//이름으로 Outbar폴더 선택되어 열리도록.
int CTabCodeSearch::SetSelFolder(LPCSTR szFolderName)
{
	return m_ctrlOutBar.SetSelFolder(szFolderName);
}


LRESULT CTabCodeSearch::OnRmsgTabCodeSearch(WPARAM wp, LPARAM lp)
{
	if(wp==10)
	{
		SetSelFolder((LPCSTR)lp);
	}
	return 0L;
}
