// TabCodeSearch.cpp : implementation file
//

#include "stdafx.h"
#include "TabCodeSearch.h"
#include "./define/ChartMsgDef.h"
#include "./define/DragNDropDefine.h"

#include "ChartItem.h"
#include "StdDialog.h"

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

// 솔로몬	const UINT RMSG_GWANSIMREGISTER		=   ::RegisterWindowMessage("RMSG_GWANSIMREGISTER");
#define UDM_TREE_CLICK			WM_USER+612			// 4Byte Code Enter
#define UWM_RECEIVE_ITRQUERY   ( WM_USER+826 )
#define UM_CHANGE_REGKW		   ( WM_USER+525 )		// 관심종목 그룹변경 처리용

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
	m_nHeightOfSearch			= DEFAULT_SPLITTER_HEIGHT;
	m_hRootItemUsr				= NULL;		// 사용자 저장조건
	m_hRootItemProposeSpecial	= NULL;		// 추천조건
	m_hRootItemProposeBuy		= NULL;		// 추천조건
	m_hRootItemProposeSell		= NULL;		// 추천조건
	m_hRootItemTheme			= NULL;		// 시장테마
	m_hRootItemSise				= NULL;		// 시세동향
	m_hRootItemIndicator		= NULL;
	m_hRootItemInvestor			= NULL;
	m_hRootItemFavorite			= NULL;		// 관심종목
	m_hRootItemAccount			= NULL;		// 보유종목
	m_hRootItemFromOutside		= NULL;
	m_hRootItemCurrentTemp		= NULL;
	m_hRootItemCurrent			= NULL;
	m_bLBtnDown					= FALSE;
	m_bRBtnDown					= FALSE;
	m_bDragNDrop				= FALSE;
	m_bDeletingTreeData			= FALSE;
	m_bInsertingInListCode		= FALSE;
	m_bUseTimer					= FALSE;
	m_bIgnoreEditVolume			= FALSE;
	m_nSortIndexOfList			= 0;
	m_bSortTypeOfList			= TRUE;
	m_lQueryStatus				= 0;
	m_bSkipSelChangedTreeEvent	= FALSE;
	m_bQueryList				= FALSE;
	m_pTreeListMng				= NULL;

	m_dwTreeKey_gwan			= 0; 
	m_dwTreeKey_master_stock	= 0; 
	m_dwTreeKey_master_elw		= 0; 
	m_dwTreeKey_master_jisu		= 0; 
	m_dwTreeKey_master_future	= 0;
	m_dwTreeKey_master_foreign	= 0;
	m_dwTreeKey_oneclick		= 0; 
	m_dwTreeKey_FindSearch		= 0; 
	m_pGwanTree					= NULL;
	m_pMasterTree_stock			= NULL;
	m_pMasterTree_elw			= NULL;
	m_pMasterTree_Jisu			= NULL;
	m_pMasterTree_future		= NULL;
	m_pMasterTree_foreign		= NULL;
	m_pOneClickTree				= NULL;
	m_pFindSearchTree			= NULL;

	m_bAdvise					= FALSE;
	m_bPlay						= FALSE;
	m_pQueryData				= NULL;
	m_pMasterDataManager		= NULL;
	m_pGridCtrl					= NULL;

}


void CTabCodeSearch::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearch)
	DDX_Control(pDX, IDC_CHK_REPEAT, m_chkRepeat);
	DDX_Control(pDX, IDC_BTRESETOCX, m_btnResetOcx);
	DDX_Control(pDX, IDC_BTPREV, m_btPrev);
	DDX_Control(pDX, IDC_BTNEXT, m_btNext);
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BTPLAY, m_btPlay);
	DDX_Control(pDX, IDC_CMB_TIMER, m_cmbTime);
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
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )
	ON_REGISTERED_MESSAGE( RMSG_INTREQUEST, OnIntRequest )
	ON_REGISTERED_MESSAGE( RMSG_CONCERNHWND, OnResetAll )
	ON_BN_CLICKED(IDC_BTPLAY, OnBnClickedBtplay)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCODE, OnLvnItemchangedListcode)
	ON_MESSAGE( WMU_OUTBAR_NOTIFY, OnOutBarNotify )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearch message handlers
#define TIMEID_UPDATEWND 10
BOOL CTabCodeSearch::OnInitDialog() 
{
	CTabDlg::OnInitDialog();
	Init_Button();
	SetConfig();
	UpdateTreeSearch();	

	// y-splitter
	m_ySplitterY1.BindWithControl(this, IDC_SPLITTERY1);
	m_ySplitterY1.SetMinHeight(30, 50);
	m_ySplitterY1.AttachAsBelowCtrl(IDC_LISTCODE);
	m_ySplitterY1.RecalcLayout();

	// Grid 초기화
	InitMasterData();
	InitGrid();

	m_listCode.Init(m_pParentDlg,0);
	m_listCode.RemoveAll();

	m_listCode.ShowWindow(SW_HIDE);
	SetTimer(TIMEID_UPDATEWND, 10, NULL);

	m_cmbTime.SetCurSel(0);		//004 돌려보기 default값 3초

	int nID = 9;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		IAUGwanManager* pMng = (IAUGwanManager*)pGateMng->GetInterface(nID);
// 솔로몬		pMng->SetRegGwansim(this, 1);
	}

	nID = 5;
	if(pGateMng)
		m_pMasterDataManager = (IMasterDataManager*)pGateMng->GetInterface(nID);
	
	SetFXMasterData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


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
// 솔로몬		pMng->SetRegGwansim(this, 0);
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
	case 10102 : 
		{
			rect.top = rect.top;// + HEIGHTOFBUTTON*2;
			rect.bottom = rect.top + m_nHeightOfSearch;
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
		rect.top = rect.top + GAP_TABDLG;// + Y_GAP;
		rect.bottom = rect.bottom - GAP_TABDLG * 6;
		break;		
	case IDC_SPLITTERY1:
		{
			rect.top = 0;
			rect.bottom = 0;
		}
		break;
	case IDC_BTRESETOCX:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2 - GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 64 + 34 + GAP_TABDLG*2;
		rect.right = rect.left + 34;
		break;

	case IDC_BTPREV:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.right = rect.left + 20;
		break;
	case IDC_BTNEXT:
		rect.top = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG/2;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 20;
		rect.right = rect.left + 20;
		break;

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

	RecalcLayoutCtrl(m_ySplitterY1);
	RecalcLayoutCtrl(m_btPrev);
	RecalcLayoutCtrl(m_btNext);
	RecalcLayoutCtrl(m_btnResetOcx);
	RecalcLayoutCtrl(m_chkRepeat);
	RecalcLayoutCtrl(m_btPlay);
	RecalcLayoutCtrl(m_cmbTime);
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

}


void CTabCodeSearch::ClearAllTreeSearch()
{
	m_hRootItemUsr				= NULL;		// 사용자 저장조건
	m_hRootItemProposeSpecial	= NULL;		// 추천조건
	m_hRootItemProposeBuy		= NULL;		// 추천조건
	m_hRootItemProposeSell		= NULL;		// 추천조건
	m_hRootItemTheme			= NULL;		// 시장테마
	m_hRootItemSise				= NULL;		// 시세동향
}

void CTabCodeSearch::DeleteChildItem(HTREEITEM hItem)
{
return;
}

void CTabCodeSearch::RequestJongmokCodesInList(HTREEITEM hItemGrp)
{

}

void CTabCodeSearch::OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
}



void CTabCodeSearch::GetCodeData20001(CString *pStr)
{
}

void CTabCodeSearch::GetCodeData20011(CString *pStr)
{
}


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
	}
	else if(strTRCode=="05010")
	{
	}
	else if(strTRCode=="00410")
	{
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


void CTabCodeSearch::OnBtportfolio() 
{
}

void CTabCodeSearch::OnBtregfavor() 
{
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	UINT nSel = 3;
}

void CTabCodeSearch::OnBtsearch() 
{
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
		m_pMultiChartWnd->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
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
	CString		strTemp;
	CString		strTemp1;
	CString		strTemp2;
	CString		strTemp3;
	char		szTemp[1024];

	if(bLoad)
	{
		::GetPrivateProfileString("TabCodeSearch","HeightOfSearch","300",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_nHeightOfSearch = atoi(szTemp);
		if(m_nHeightOfSearch<180)
			m_nHeightOfSearch = DEFAULT_SPLITTER_HEIGHT;

		::GetPrivateProfileString("TabCodeSearch","SelectedFavoriteItemCode","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strFavoriteGrp = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedItemName","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strSelectedItemName = szTemp;
		::GetPrivateProfileString("TabCodeSearch","SelectedParentItemName","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_strSelectedParentItemName = szTemp;
		::GetPrivateProfileString("TabCodeSearch","UserCodeOrder","",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_srtCodeOrder = szTemp;
	}
	else
	{
		strTemp.Format("%d",m_nHeightOfSearch);
		::WritePrivateProfileString("TabCodeSearch","HeightOfSearch",strTemp,m_pParentDlg->m_strConfig);
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

	CString strCodeSelected = m_listCode.GetCodeSelected();
	if(!strCodeSelected.IsEmpty())
	{
		((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
		if (g_pPtUtilMng)
			g_pPtUtilMng->AddToHistory(((CStdDialog*)m_pMultiChartWnd)->m_hPlatform, strCodeSelected);

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
	{
		CString strCodeSelected = m_listCode.GetCodeSelected();
		if(!strCodeSelected.IsEmpty())
		{
			((CStdDialog*)m_pMultiChartWnd)->ChangeCode(strCodeSelected);
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
		m_listCode.ShowWindow(SW_SHOW);
	}



	
	CTabDlg::OnTimer(nIDEvent);
}



BOOL CTabCodeSearch::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)

	
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
				int nMarketType = FX_CHART;
				for(POSITION pos=pListTypeA->GetHeadPosition(); pos; )
				{
					CCodeTypeA typeA = pListTypeA->GetNext(pos);

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
					break;
				case DOWN_LIMIT:	// DownLimit
					lDownLimit++;
					break;	
				}
				
				
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


			pThis->m_bInsertingInListCode = FALSE;
			//}
		}
	}
}

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

	char* KeyName = (LPSTR)(LPCTSTR)szKey;
	UINT	nID = 200;

	pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);

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
	char* KeyName = (LPSTR)(LPCTSTR)szKey;
	UINT nID = 200;
	pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
	m_bAdvise = FALSE;
	pDrdsLib->DrdsClose(DataName);		
	return TRUE;
}

void CTabCodeSearch::SetRealPriceData(void *pData)
{

}

void CTabCodeSearch::OnBtnRetry() 
{
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
		m_listCode.ModifyStyle(0, LVS_SINGLESEL);
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
			//long lCntOfChart = m_pChartMng->SendMessage(UMSG_GETCNTOFCHART);
			m_listCode.ModifyStyle(LVS_SINGLESEL, 0);
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

	if(m_pFindSearchTree)
	{
		m_pFindSearchTree->DestroyWindow();
		delete m_pFindSearchTree;
	}
}

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
			if(((CStdDialog*)m_pMultiChartWnd)->m_nMarketType == FX_CHART)
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
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode (FX_CHART);
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
					((CStdDialog*)m_pMultiChartWnd)->ChangeStockMode (FX_CHART);
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
		}
	}

	return 0L;
}

void CTabCodeSearch::ChangeSkinColor(COLORREF clrBkgrnd)
{
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.DeleteObject();
	m_brBkColor.CreateSolidBrush(m_crBk);

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
		m_pGridCtrl->SetEditable(FALSE);					// CInplaceEdit 설정
		m_pGridCtrl->SetGridColor(COLOR_GRIDHEADER);		// Grid Color 설정
		m_pGridCtrl->SetHighlightColor(COLOR_BACK_LABEL);
		m_pGridCtrl->SetBkColor(COLOR_WHITE);				// Grid Back Color 설정
		m_pGridCtrl->EnableColumnHide();					// Column 숨길 수 있게 설정
		m_pGridCtrl->EnableRowHide();						// Row 숨길 수 있게 설정
		m_pGridCtrl->EnableTitleTips(FALSE);				// ToolTip 설정
		m_pGridCtrl->SetGridLineColor(COLOR_GRIDLINE);		// Grid Line color
		
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

	int nRowTotal = m_pGridCtrl->GetRowCount();
	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)
		return FALSE;

	strCode = (LPCSTR)m_pGridCtrl->GetItemData(row, col);
	return TRUE;
}


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
// 솔로몬	
		CStringArray *arrCode, *arrName;
		arrCode = new CStringArray;
		arrName = new CStringArray;
		m_pMasterDataManager->GetFXMst(arrCode, arrName);

		for ( int i = 0 ; i < arrCode->GetSize() ; i++ )
		{
			m_listCode.InsertCodeData (arrCode->GetAt (i), arrCode->GetAt (i), 
									"", "", "", "", '3');
		}
		delete arrCode;
		delete arrName;

	/*
	// [TabCodeSearch]
	// UserCodeOrder=
	// 저장된 정보가 있으면..
	if (m_srtCodeOrder.GetLength())
	{
		int i;
		CString strCode;
		int nCount = m_srtCodeOrder.GetLength() / 7;

		CString strOrgCode;
		CString strInsertCode;
		
		CStringArray arrCode, arrName;
		m_pMasterDataManager->GetMasterData("", arrCode, arrName);

		for ( i = 0 ; i < arrCode.GetSize() ; i++ )
		{
			strOrgCode += arrCode.GetAt(i);
		}

		// 마스터에 있는 종목만 찾는다.
		for ( i = 0; i < nCount; i++)
		{
			strCode = m_srtCodeOrder.Mid(i*7, 7);
			
			if (strOrgCode.Find(strCode) >= 0)
			{
				m_listCode.InsertCodeData( strCode, strCode,
										"", "", "", "", '3'	);
				strInsertCode += strCode;
			}
		}
		// 마스터에만 있는 종목만 찾는다.
		for ( i = 0; i < arrCode.GetSize() ; i++ )
		{
			strCode = arrCode.GetAt(i);
			
			if (strInsertCode.Find(strCode) < 0)
			{
				m_listCode.InsertCodeData( strCode, strCode,
										"", "", "", "", '3'	);
				strInsertCode += strCode;
			}
		}
	}
	// 저장된 정보가 없으면 master table에서 load
	else
	{
		CStringArray arrCode, arrName;
		m_pMasterDataManager->GetMasterData("", arrCode, arrName);

		for ( int i = 0 ; i < arrCode.GetSize() ; i++ )
		{
			m_listCode.InsertCodeData (arrCode.GetAt (i), arrCode.GetAt (i), 
									"", "", "", "", '3');
		}
	}
	*/
}

void CTabCodeSearch::SetCurListIndex(CString strCode) 
{
	int nIndex = m_listCode.FindCode(strCode);
	m_listCode.SetCurSel(nIndex);
}