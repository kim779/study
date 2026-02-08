// TabIndices.cpp : implementation file
//

#include "stdafx.h"
#include "TabIndices.h"
//#include "../../COMMON_BASE\drds\DRDSDEF.h"
#include "LeftBarDlg.h"
#include "./define/DragNDropDefine.h"
#include "./define/ChartMsgDef.h"
#include "FavoriteThingsMng.h"
#include "../../inc/IAUGDIMng.h"
#include ".\tabindices.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language

#include "stdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define HEIGHTOFINDICES		80
#define HEIGHTOFSELECTED	50

#define TFI_JIPYOITEM_TITLE	1		// 타이틀검색
#define TFI_JIPYOITEM_ID	2		// Tree ID검색

/////////////////////////////////////////////////////////////////////////////
// CTabIndices dialog
#define ID_SRND_TIMER 0x100

#define UDM_MAP_OPEN_SEARCH			WM_USER+611 // 4Byte Code Enter
#define UDM_TREE_RBUTTON			WM_USER+613 // 4Byte Code Enter
#define MENU_ID_ADD								100
#define MENU_ID_DEL								200
#define MENU_TITLE_ADD						"즐겨찾기 추가"
#define MENU_TITLE_DEL						"즐겨찾기 삭제"

char* szIndexGroupName[] = {
								 "특수지표",
								 "관심지표",
								 "사용지표",
								 "가격지표",
								 "추세지표",
								 "변동성지표",
// (2008/9/13 - Seung-Won, Bae) Remove Volume Index
								 "거래량지표",
								 "2차파생지표",
								 "기타지표",
// (2008/9/13 - Seung-Won, Bae) Remove Market Index
//								 "시장지표",
								 "전체지표"
								 };

CTabIndices::CTabIndices(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabIndices::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabIndices)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nHeightOfIndices = 120;
	m_nIndexSelected = 0;
	m_bLBtnDown = FALSE;
	m_bDragNDrop = FALSE;

//	m_pListU = NULL;
//	m_pListKU = NULL;
//	m_pListUP = NULL;
//	m_pListUD = NULL;

	m_bShowJipyoItem = FALSE; //지표 리스트 를 보여줄지 여부.
	m_bBack = FALSE; //지표 돌려보기 뒤로 가는 것인지 여부
	m_strOldJipyo = _T("");
	m_nOldSelJipyoIndex = -1; //기존에 선택된 리스트의 항목 Index
	m_posPrev = NULL;
	m_posNext = NULL;
	m_posCur  = NULL;	
	m_nNextJipyoIndex	= -1;

	m_bRun = FALSE;

	m_bFindExtend = FALSE;
	
}


void CTabIndices::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIndices)
	DDX_Control(pDX, IDC_STATIC_TITLE2, m_stTitle);
	DDX_Control(pDX, IDC_RADIO_AUTO, m_RadioAuto);
	DDX_Control(pDX, IDC_RADIO_MANUAL, m_RadioManual);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_CMB_TIME, m_cmbTime);
	DDX_Control(pDX, IDC_BTN_SHOW_REPEAT, m_btnShowList);
	DDX_Control(pDX, IDC_BTN_DELITEM, m_btnDelItem);
	DDX_Control(pDX, IDC_BTN_ADDITEM, m_btnAddItem);
	DDX_Control(pDX, IDC_BTSEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTAPPLY, m_btnApply);
	DDX_Control(pDX, IDC_TREEITEMS, m_treeItems);
	DDX_Control(pDX, IDC_LISTITEMS, m_listItems);	
	DDX_Control(pDX, IDC_EDIT_FIND, m_editFind);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_BTN_EXTEND, m_btnExtend);
	DDX_Control(pDX, IDC_LIST_FIND, m_listFind);
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_LISTINDICES, m_listIndices);
}


BEGIN_MESSAGE_MAP(CTabIndices, CTabDlg)
	//{{AFX_MSG_MAP(CTabIndices)
	ON_WM_SIZE()
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTAPPLY, OnBtapply)
	ON_BN_CLICKED(IDC_BTSEARCH, OnBtsearch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREEITEMS, OnDblclkTreeitems)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREEITEMS, OnBegindragTreeitems)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()	
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTITEMS, OnBegindragListitems)	
	ON_NOTIFY(NM_DBLCLK, IDC_LISTITEMS, OnDblclkListitems)
	ON_BN_CLICKED(IDC_BTN_SHOW_REPEAT, OnBtnShowRepeat)	
	ON_BN_CLICKED(IDC_BTN_NEXT, OnBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREV, OnBtnPrev)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_CBN_SELCHANGE(IDC_CMB_TIME, OnSelchangeCmbTime)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_MANUAL, OnRadioManual)
	ON_BN_CLICKED(IDC_BTN_ADDITEM, OnBtnAdditem)
	ON_BN_CLICKED(IDC_BTN_DELITEM, OnBtnDelitem)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_BTN_EXTEND, OnBnClickedBtnExtend)
	ON_LBN_SELCHANGE(IDC_LIST_FIND, OnLbnSelchangeListFind)
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
	ON_COMMAND_RANGE(MENU_ID_ADD, MENU_ID_DEL, OnSendMenu)		//Modified by cyberahn@KOSCOM 2006.10.11.
END_MESSAGE_MAP()

//ON_NOTIFY(NM_CLICK, IDC_LISTINDICES, OnClickListindices)
//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTINDICES, OnItemchangedListindices)



/////////////////////////////////////////////////////////////////////////////
// CTabIndices message handlers

void CTabIndices::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RecalcLayout();
}

BOOL CTabIndices::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	Init_Button();

	m_listFind.ResetContent();
	m_listFind.ShowWindow(FALSE);

//	m_treeItems.SetHilightColor(RGB(0,0,0),CRSELECTED);
	
	
/*
	CString strImagePath = m_pParentDlg->m_szImageDir;//m_pParentDlg->m_strRootPath + "\\Image";
	CString strImageUp;
	CString strImageDown;
	CString strImageDis;

	strImageUp = strImagePath	+ "\\사용자_up.bmp";
	strImageDown = strImagePath + "\\사용자_down.bmp";		
	m_btnSearch.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);
	m_btnSearch.SetToolTipText("사용자");

	strImageUp = strImagePath + "\\적용_up.bmp";
	strImageDown = strImagePath + "\\적용_down.bmp";	
	strImageDis = strImagePath + "\\적용_dis.bmp";	
	m_btnApply.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);
	m_btnApply.SetToolTipText("적용");

	strImageUp		= strImagePath + "\\열기_up.bmp"	;
	strImageDown	= strImagePath + "\\열기_down.bmp"	;		
	m_btnShowList.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);

	strImageUp		= strImagePath + "\\btn_지표정지_up.bmp"	;
	strImageDown	= strImagePath + "\\btn_지표정지_down.bmp"	;	
	strImageDis		= strImagePath + "\\btn_지표정지_dis.bmp"	;
	m_btnStop.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);

	strImageUp		= strImagePath + "\\역방향플레이_up.bmp"	;
	strImageDown	= strImagePath + "\\역방향플레이_down.bmp"	;
	strImageDis		= strImagePath + "\\역방향플레이_dis.bmp"	;
	m_btnPrev.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);

	strImageUp		= strImagePath + "\\정방향플레이_up.bmp"	;
	strImageDown	= strImagePath + "\\정방향플레이_down.bmp"	;
	strImageDis		= strImagePath + "\\정방향플레이_dis.bmp"	;	
	m_btnNext.SetSkin(strImageUp,strImageDown,"",strImageDis,"","",0,0,0);

	strImageUp		= strImagePath + "\\돌려보기추가_up.bmp"	;
	strImageDown	= strImagePath + "\\돌려보기추가_down.bmp"	;	
	m_btnAddItem.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);

	strImageUp		= strImagePath + "\\선택삭제_up.bmp"	;
	strImageDown	= strImagePath + "\\선택삭제_down.bmp"	;	
	m_btnDelItem.SetSkin(strImageUp,strImageDown,"","","","",0,0,0);
*/

//-----------------------------------------------------------------------------
//	Reason : 1. 전체 지표 목록은 TRee로 구성하므로 List는 뺀다.
//			    Splitter Bar도 없앤다.
//           2. 상단 리스트 만 없앤다. 2005.05.11 화면 바뀜.^^'
//  Author : Sang-Yun, Nam
//  Date   : 2005-04-21 pm 04:37
//-----------------------------------------------------------------------------	
	// XListCtrl must have LVS_EX_FULLROWSELECT if combo boxes are used
	//m_listIndices.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//m_listIndices.SetHilightColor(RGB(0,0,0),CRSELECTED);
	m_listItems.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
	m_listItems.SetHilightColor(CLR_FONT_DEFAULT,CRSELECTED);
	
	CRect rect;
	LV_COLUMN lvcolumn;
	HDITEM hditem;
/*	m_listIndices.GetClientRect(&rect);

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Name";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = rect.Width();
	m_listIndices.InsertColumn(0, &lvcolumn);	

	// iterate through header items and attach the image list
	HDITEM hditem;
	hditem.mask = HDI_IMAGE | HDI_FORMAT;
	m_listIndices.m_HeaderCtrl.GetItem(0, &hditem);
	hditem.fmt |=  HDF_IMAGE;
	hditem.iImage = XHEADERCTRL_NO_IMAGE;
	m_listIndices.m_HeaderCtrl.SetItem(0, &hditem);

	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.iSubItem = 0;
	m_listIndices.SetColumn(0, &lvcolumn);
*/

	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.pszText = "Name";
	lvcolumn.iSubItem = 0;
	lvcolumn.cx = rect.Width();
	m_listItems.InsertColumn(0, &lvcolumn);	
	// iterate through header items and attach the image list
	hditem.mask = HDI_IMAGE | HDI_FORMAT;
	m_listItems.m_HeaderCtrl.GetItem(0, &hditem);
	hditem.fmt |=  HDF_IMAGE;
	hditem.iImage = XHEADERCTRL_NO_IMAGE;
	m_listItems.m_HeaderCtrl.SetItem(0, &hditem);
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.iSubItem = 0;
	m_listItems.SetColumn(0, &lvcolumn);


	
//	TCHAR *	lpszSubItem[] = { _T("기술적지표"),
//							  _T("업종지수"),
//							  _T("시장지표"),
//							  NULL };
//
//	
//	for(int i=0;;i++)
//	{
//		if (lpszSubItem[i] == NULL)
//			break;
//		m_listIndices.InsertItem(i, "");
//		m_listIndices.SetItemText(i, 0, lpszSubItem[i]);
//	}	


	
    // y-splitter
    m_ySplitterY1.BindWithControl(this, IDC_SPLITTERY1);
    m_ySplitterY1.SetMinHeight(30, 50);
    m_ySplitterY1.AttachAsAboveCtrl(IDC_TREEITEMS);
    m_ySplitterY1.AttachAsBelowCtrl(IDC_LISTITEMS);
	//m_ySplitterY1.AttachAsBelowCtrl(IDC_TREEITEMS);
    m_ySplitterY1.RecalcLayout();


//	SetConfig();
//	m_listIndices.SetCurSel(m_nIndexSelected);
//-----------------------------------------------------------------------*/


	// 지표 돌려보기 기본 수동
	m_RadioManual.SetCheck(1);
	m_RadioAuto.SetCheck(0);

	// 돌려보기 시간 
	m_cmbTime.AddString("3초");
	m_cmbTime.AddString("5초");
	m_cmbTime.AddString("15초");
	m_cmbTime.SetCurSel(0);

	SetItemForm(); //트리 구성 

	m_stTitle.SetStaticColor(m_crBk, m_crBk, CLR_FONT_DEFAULT);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabIndices::SetConfig(BOOL bLoad /*= TRUE*/)
{
	CString strTemp;
	char szTemp[64];
	if(bLoad)
	{
		::GetPrivateProfileString("TabIndices","HeightOfIndices","80",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_nHeightOfIndices = atoi(szTemp);
		::GetPrivateProfileString("TabIndices","IndexSelected","0",szTemp,sizeof(szTemp),m_pParentDlg->m_strConfig);
		m_nIndexSelected = atoi(szTemp);
	}
	else
	{
		strTemp.Format("%d",m_nHeightOfIndices);
		::WritePrivateProfileString("TabIndices","HeightOfIndices",strTemp,m_pParentDlg->m_strConfig);
		strTemp.Format("%d",m_nIndexSelected);
		::WritePrivateProfileString("TabIndices","IndexSelected",strTemp,m_pParentDlg->m_strConfig);
	}
}


CRect CTabIndices::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(2,4,0,0);
	switch(nID)
	{
//	case IDC_LISTINDICES:
//		rect.bottom = rect.top + m_nHeightOfIndices;
//		break;		
//	case IDC_LISTITEMS:
//	case IDC_TREEITEMS:
//		rect.top = rect.top + m_nHeightOfIndices + GAP_TABDLG;
//		rect.bottom = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG;			
//		break;	
	//007 {{ 지표검색
	case IDC_EDIT_FIND:
//		rect.top = rect.top + GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left ;
		rect.right = rect.left + 110; 
		break;
	case IDC_BTN_FIND:
//		rect.top = rect.top + GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 110 + GAP_TABDLG;
		rect.right = rect.left + 20;
		break;
	case IDC_BTN_EXTEND:
//		rect.top = rect.top + GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		rect.left = rect.left + 110 + GAP_TABDLG + 20 + 2;
		rect.right = rect.left + 20; 
		break;
	case IDC_LIST_FIND:
		rect.top = rect.top + HEIGHTOFBUTTON + GAP_TABDLG;
		rect.bottom = rect.top + HEIGHTOFBUTTON * 5;        
		break;
	//007 }}
	case 10102 : 
		if(m_bFindExtend)
			rect.top = rect.top + HEIGHTOFBUTTON * 6 + GAP_TABDLG * 2;
		else
			rect.top = rect.top + HEIGHTOFBUTTON + GAP_TABDLG;

		if(m_bShowJipyoItem)			
			rect.bottom = rect.top + m_nHeightOfIndices;			
		else
			rect.bottom = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG;
		break;
	case IDC_TREEITEMS:
		rect.top = rect.top + GAP_TABDLG;
		if(m_bShowJipyoItem)			
			rect.bottom = rect.top + m_nHeightOfIndices;			
		else
			rect.bottom = rect.bottom - HEIGHTOFBUTTON - GAP_TABDLG;			
		break;
	case IDC_LISTITEMS:
		rect.top	= rect.top + m_nHeightOfIndices + GAP_TABDLG + 2;
		rect.bottom = rect.bottom - HEIGHTOFBUTTON * 3 - GAP_TABDLG;			
		break;	
	case IDC_SPLITTERY1:
		rect.top = rect.top + m_nHeightOfIndices;
		rect.bottom = rect.top + GAP_TABDLG + 2;
		break;		
	case IDC_STATIC_TITLE2:
		rect.top = rect.bottom - HEIGHTOFBUTTON + 2;
		rect.left = rect.left + 5;
		//20080915 이문수 >>
		//rect.right = rect.left + 60;			
		rect.right = rect.left + 50;			
		//20080915 이문수 <<
		break;

	case IDC_BTSEARCH:
		rect.top = rect.bottom - HEIGHTOFBUTTON;
		rect.left = rect.right - 43 * 2 - 2;
		rect.right = rect.left + 43;
		break;
	case IDC_BTAPPLY:
		rect.top = rect.bottom - HEIGHTOFBUTTON;
		//rect.left = rect.right - 60;
		rect.left = rect.right - 43;
		break;	
	case IDC_BTN_SHOW_REPEAT:
		rect.top = rect.bottom - HEIGHTOFBUTTON;
		rect.left = rect.left + 1;
		rect.right = rect.left + 30;		
		break;	
	case IDC_BTN_ADDITEM:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2;
		rect.left = rect.left + 1;
		rect.right = rect.left + 88;		
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_BTN_DELITEM:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2;
		rect.left = rect.left + 1 + 89;
		rect.right = rect.left + 61;		
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_CMB_TIME:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 2;
		rect.left = rect.left + 1 + 89 + 62;
		rect.right = rect.left + 55;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;	
	case IDC_BTN_PREV:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 3;
		rect.left = rect.left + 1;
		rect.right  = rect.left + 33;
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_BTN_STOP:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 3;
		rect.left  = rect.left + 1 + 34;
		rect.right = rect.left + 33;		
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_BTN_NEXT:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 3;
		rect.left  = rect.left + 1 + 34 + 34;
		rect.right = rect.left + 33;	
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_RADIO_MANUAL:
		rect.top = rect.bottom - HEIGHTOFBUTTON * 3;
		rect.left  = rect.left + 1 + 34 + 34 + 34;
		rect.right = rect.left + 42;	
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	case IDC_RADIO_AUTO:	
		rect.top = rect.bottom - HEIGHTOFBUTTON * 3;
		rect.left  = rect.left + 1 + 34 + 34 + 34 + 47;
		rect.right = rect.left + 42;	
		rect.bottom = rect.top + HEIGHTOFBUTTON;
		break;
	default:
		break;
	}
	return rect;
}


void CTabIndices::RecalcLayout()
{
//	RecalcLayoutCtrl(m_listIndices);
	RecalcLayoutCtrl(m_listItems);
//	RecalcLayoutCtrl(m_treeItems);
	RecalcLayoutCtrl(m_ctrlOutBar);
	RecalcLayoutCtrl(m_ySplitterY1);
	
	RecalcLayoutCtrl(m_btnShowList);	
	RecalcLayoutCtrl(m_btnApply);	
	RecalcLayoutCtrl(m_btnSearch);

	RecalcLayoutCtrl(m_btnPrev);
	RecalcLayoutCtrl(m_btnStop);	
	RecalcLayoutCtrl(m_btnNext);	
	RecalcLayoutCtrl(m_RadioAuto);
	RecalcLayoutCtrl(m_RadioManual);
//
//	
	RecalcLayoutCtrl(m_btnAddItem);
	RecalcLayoutCtrl(m_btnDelItem);
	RecalcLayoutCtrl(m_cmbTime);

	//007 {{
	RecalcLayoutCtrl(m_editFind);
	RecalcLayoutCtrl(m_btnFind);
	RecalcLayoutCtrl(m_btnExtend);
	RecalcLayoutCtrl(m_listFind);
	//007 }}

	RecalcLayoutCtrl(m_stTitle);
	CRect rect;
	GetClientRect(rect);	
	rect.right -= GAP_TABDLG*4;
//	if(m_listIndices.GetSafeHwnd())	
//	{
//		m_listIndices.SetColumnWidth(0,rect.Width());
//	}
	if(m_listItems.GetSafeHwnd())	
	{		
		m_listItems.SetColumnWidth(0,rect.Width());
	}
}


LONG CTabIndices::OnWmSplitterMoved(UINT wParam, LONG lParam )
{
	if(IDC_SPLITTERY1==lParam)
	{
		m_nHeightOfIndices += wParam;
		RecalcLayout();
	}	
	return 1L;
}

//void CTabIndices::OnClickListindices(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	// TODO: Add your control notification handler code here
//	LPNMLISTVIEW ptreeview = (LPNMLISTVIEW)pNMHDR;
//	*pResult = 0;
//}

void CTabIndices::SetItemForm()
{
	SetTechnicalIndicator();

//////////////////////////////////////////////////////////////////////////////////////

	DWORD dwf = CGfxOutBarCtrl::fDragItems|CGfxOutBarCtrl::fEditGroups|CGfxOutBarCtrl::fEditItems|CGfxOutBarCtrl::fRemoveGroups|
				CGfxOutBarCtrl::fRemoveItems|CGfxOutBarCtrl::fAddGroups|CGfxOutBarCtrl::fAnimation|CGfxOutBarCtrl::fSelHighlight;
	//m_ctrlOutBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,100,100), this, 10102, dwf);
	m_ctrlOutBar.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,100,100), this, 10102, dwf);
	//2006.12.07 정유진 수정
	//LeftBar 사이즈 변경시 이미지 잘림현상 수정
	//m_ctrlOutBar.SetFolderImageList(m_pIL_W151H21, 0, 2, 3);
	m_ctrlOutBar.SetFolderImageList(&m_ImageOutbarIcon, 0, 1, 3);
	//2006.12.07 정유진 수정 end
	m_ctrlOutBar.SetOwner(this);

//	STUSERCOLORTABLE info;
//	info = theApp.m_userColorTable.GetAt(43);
//	m_ctrlOutBar.cr3dFace =  theApp.GetColor(theApp.m_skinInfo.m_szBKColor);
//	m_ctrlOutBar.cr3dUnFace = info.rgb;
	m_ctrlOutBar.cr3dFace =  RGB(191,211,233);
	m_ctrlOutBar.cr3dUnFace = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	//m_ctrlOutBar.cr3dUnFace = RGB(191,211,233);

	//-------------------------------------------------------------------
	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
	//=============================================================
	// 05.10.24
	// 주식만 시장 지표 추가 
	//=============================================================
//	if(m_nChartGubun != STOCK_CHART)
//		lAllCnt -= 1;
	//=============================================================

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
	//int nId = 4;
	
	//IMainResouceManager* pRscMng = (IMainResouceManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nId, 0);
	//IAUGDIManager* pGDIMng = pRscMng->CreateGDIManager();
//	CImageList* pImageList = (CImageList*)pGDIMng->AdviseImageList(strImageDir, 9, 9);

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

	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		CTrTreeCtrl* pTree = new CTrTreeCtrl;
		//CTreeCtrlEx* pTree = new CTreeCtrlEx;
		pTree->Create(WS_CHILD|TVS_SHOWSELALWAYS, CRect(0,0,0,0), &m_ctrlOutBar, 10100+(lIndex-1));
		pTree->SetImageList(&m_ImageListIcon, TVSIL_NORMAL);

		if(lIndex == 1)		//관심지표
		{
			strArrayIndicator.RemoveAll();
			CFavoriteThingsMng pFavoriteMng;
			DWORD wSizeItem = 0;
			CString strGrpName;
			DWORD wIndexItem = 0;
			stFvrData _stFvrData;
			pFavoriteMng.SetPath(m_pParentDlg->GetUserPath());
			pFavoriteMng.LoadItemInfo("JipyoFile");
			wSizeItem = pFavoriteMng.m_ArrayFvrItem.GetSize();
			for(wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
			{
				_stFvrData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
				strArrayIndicator.Add(_stFvrData.m_chData1);
			}		
		}
		else if(lIndex == 2)	//사용지표
		{
			strArrayIndicator.RemoveAll();
		}
		else
		{
			if(m_pChartMng->GetSafeHwnd())
				m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)szIndexGroupName[lIndex],(LPARAM)&strArrayIndicator);
		}
		lSize = strArrayIndicator.GetSize();
		nFlagsChild = NULL;
		bExistance = FALSE;

		for(long lPos=0;lPos<lSize;lPos++)
		{
			strIndicator = strArrayIndicator.GetAt(lPos);

			long lPos1 = strIndicator.Find("&&");
			if(lPos1>=0)
			{
				strIndicator.Delete(lPos);
			}	
			pTree->InsertItem(strIndicator);
			lPosAll++;
		}
		strArrayIndicatorAll.Append(strArrayIndicator);

		if(lIndex == 0)
			m_ctrlOutBar.AddFolderBar("차트", pTree);
		else
			m_ctrlOutBar.AddFolderBar(szIndexGroupName[lIndex], pTree);
		
		m_ListTree.AddTail(pTree);
	}

//	pUtil->SetGrade(cMenuGubun, theApp.m_pUserInfo->Level);
//	pUtil->MakeTreeAllNode(pTree);

	

//	HTREEITEM hItem;
//	HTREEITEM  huRootItem = pTree->InsertItem("KOSPI");
//	POSITION pos = m_pListU->GetHeadPosition();
//	while(pos)
	{
//		uMaster = m_pListU->GetNext(pos);
		//hItem = pTree->InsertItem("aaa",huRootItem);
		//pTree->SetItemData(hItem,10000);
		//pTree->Expand(huRootItem, TVE_EXPAND);
	}

	//m_ctrlOutBar.AddFolderBar((LPSTR)(LPCTSTR)"테스트입니당", pTree);

	//m_ctrlOutBar.AddFolderBar((LPSTR)(LPCTSTR)"테스트입니당111", &m_treeItems);
//	m_ListTree.AddTail(&m_treeItems);

//	m_ListTree.AddTail(&m_treeItems);

	m_ctrlOutBar.SetSelFolder(0);
//////////////////////////////////////////////////////////////////////////////////////

//	switch(m_nIndexSelected)
//	{
//	case 0:	// 기술적지표
//		SetTechnicalIndicator();
//		break;
//	case 1:	// 업종지수
//		SetUpjongDataTree();
//		break;
//	case 2:	// 시장지표
//		SetMarketIndicator();
//		break;
//	}
}


void CTabIndices::SetUpjongDataTree()
{
 	m_listItems.ShowWindow(SW_HIDE);
//	m_treeItems.ShowWindow(SW_SHOW);
	m_ctrlOutBar.ShowWindow(SW_SHOW);
	m_treeItems.DeleteAllItems();

	m_arKOSPIUpCode		.RemoveAll();
	m_arKOSPIUpName		.RemoveAll();	
	m_arKOSPI200UpCode	.RemoveAll();
	m_arKOSPI200UpName	.RemoveAll();	
	m_arKOSDAQUpCode	.RemoveAll();	
	m_arKOSDAQUpName	.RemoveAll();			
	m_arETCUpCode		.RemoveAll();
	m_arETCUpName		.RemoveAll();	

	//KOSPI 업종 
//	m_pParentDlg->GetCodeMasterArray(CODE_UPJONG, m_arKOSPIUpCode, m_arKOSPIUpName);

	//KOSDAQ 업종
//	m_pParentDlg->GetCodeMasterArray(CODE_KUPJONG, m_arKOSDAQUpCode, m_arKOSDAQUpName);

	//KOSPI200 / 기타업종 한화에서는 기타 업종은 KOSPI200업종에 포함되어 있다.
//	m_pParentDlg->GetCodeMasterArray(CODE_K200UPJONG, m_arKOSPI200UpCode, m_arKOSPI200UpName);

/*	//기타 업종
	m_pParentDlg->GetCodeMasterArray(, m_arETCUpCode, m_arETCUpName);

	if(!m_pListU)	
	{
		m_pListU = (CList<CUMaster, CUMaster>*)m_pParentDlg->GetCodePointer("UMASTER");
	}
	if(!m_pListKU)
	{
		m_pListKU = (CList<CKUMaster, CKUMaster>*)m_pParentDlg->GetCodePointer("UKMASTER");
	}
	if(!m_pListUP)	
	{
		m_pListUP = (CList<CUPMaster, CUPMaster>*)m_pParentDlg->GetCodePointer("UPMASTER");
	}
	if(!m_pListUD)	
	{
		m_pListUD = (CList<CUDMaster, CUDMaster>*)m_pParentDlg->GetCodePointer("UDMASTER");
	}
*/	
/*	if(m_pListU)
	{
		CUMaster uMaster;
		HTREEITEM hItem;
		HTREEITEM  huRootItem = m_treeItems.InsertItem("KOSPI");
		POSITION pos = m_pListU->GetHeadPosition();
		while(pos)
		{
			uMaster = m_pListU->GetNext(pos);
			hItem = m_treeItems.InsertItem(uMaster.m_szName,huRootItem);
			m_treeItems.SetItemData(hItem,atoi(uMaster.m_szGubun)*1000+atoi(uMaster.m_szCode));
		}
	}
	if(m_pListKU)
	{
		CKUMaster kuMaster;
		HTREEITEM hItem;
		HTREEITEM  hkuRootItem = m_treeItems.InsertItem("KOSDAQ");
		POSITION pos = m_pListKU->GetHeadPosition();
		while(pos)
		{			
			kuMaster = m_pListKU->GetNext(pos);
			hItem = m_treeItems.InsertItem(kuMaster.m_szName,hkuRootItem);
			m_treeItems.SetItemData(hItem,atoi(kuMaster.m_szGubun)*1000+atoi(kuMaster.m_szCode));
		}
	}
	if(m_pListUP)
	{
		CUPMaster upMaster;
		HTREEITEM hItem;
		HTREEITEM  hupRootItem = m_treeItems.InsertItem("KOSPI200");
		POSITION pos = m_pListUP->GetHeadPosition();
		while(pos)
		{
			upMaster = m_pListUP->GetNext(pos);
			hItem = m_treeItems.InsertItem(upMaster.m_szName,hupRootItem);
			m_treeItems.SetItemData(hItem,atoi(upMaster.m_szGubun)*1000+atoi(upMaster.m_szCode));
		}
	}
	if(m_pListUD)
	{
		CUDMaster udMaster;
		HTREEITEM hItem;
		HTREEITEM  hudRootItem = m_treeItems.InsertItem("기타 업종지수");
		POSITION pos = m_pListUD->GetHeadPosition();
		while(pos)
		{			
			udMaster = m_pListUD->GetNext(pos);
			hItem = m_treeItems.InsertItem(udMaster.m_szName,hudRootItem);
			m_treeItems.SetItemData(hItem,atoi(udMaster.m_szGubun)*1000+atoi(udMaster.m_szCode));
		}
	}
*/
}

void CTabIndices::SetTechnicalIndicator()
{
	m_listItems.ShowWindow(SW_HIDE);
	//m_treeItems.ShowWindow(SW_SHOW);
	if(m_ctrlOutBar)
		m_ctrlOutBar.ShowWindow(SW_SHOW);
	m_pParentDlg->GetIndicatorNameListIntoTreeCtrl(m_treeItems);
}

void CTabIndices::SetMarketIndicator()
{
/*	#include "arMarketIndicator.h"
	
// 	m_listItems.ShowWindow(SW_SHOW);
	m_treeItems.ShowWindow(SW_HIDE);
	if(m_listItems.GetItemCount()) return;
//	m_listItems.DeleteAllItems();	
	for(int i=0;;i++)
	{
		if (g_lpszInputMarketIndicator[i] == NULL)
			break;
		m_listItems.InsertItem(i, "");		
		if(i%2) // odd
		{
			m_listItems.SetItemText(i, 0, g_lpszInputMarketIndicator[i]);
		}
		else		// even
		{
			m_listItems.SetItemText(i, 0, g_lpszInputMarketIndicator[i],RGB(0,0,0),CRGRID);
		}				

	}
*/
}

void CTabIndices::OnDblclkListitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//OnBtapply();
	*pResult = 0;
}

void CTabIndices::OnBtapply() 
{

	//{{ 적용 로직 -> 삭제로 변결
//	POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
//	CTreeCtrl* treeItem  = m_ListTree.GetAt(pos);
//	HTREEITEM hItem = NULL;
//	hItem = treeItem->GetSelectedItem();
//
//	//시장지표와 아닌것의 구분
//	CString strText			= treeItem->GetItemText(hItem);
//
//	BOOL bMarket = FALSE;
//	if(szIndexGroupName[m_nIndexSelected] == "시장지표")		
//	{		
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText, FROM_TABJIPYO_MARKET);
//	}
//	else
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
//	}
	//}}

//	stFvrData _stData;
//	CString strItem;
//	CString strItemTitle;
//	::ZeroMemory(&_stData,sizeof(_stData));
//	strItemTitle = '[';
//	strItemTitle += TAB_ASSISTINDEX;
//	strItemTitle += "] ";
//
//	POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
//	CTreeCtrl* treeItem  = m_ListTree.GetAt(pos);
//	HTREEITEM hItem = NULL;
//	hItem = treeItem->GetSelectedItem();
//	//HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);
//	if(hItem==NULL) 
//	{
//		MessageBox("하나를 선택하십시오.",DLLTITLENAME);
//		return;
//	}		
//	
//	CString strPItem = szIndexGroupName[m_nIndexSelected];
//	CString sSelItemText = treeItem->GetItemText(hItem);
//	if(sSelItemText.GetLength())
//	{
//		strItemTitle += sSelItemText;
//		strItemTitle += '(';
//		strItemTitle += strPItem;
//		strItemTitle += ')';
//		DWORD wItemData = m_treeItems.GetItemData(hItem);
//
//
//		//업종 지수에서도 문자열로 저장해도 되는 지 확인이 필요한다.
//		//2005. 08. 10 by sy,nam
//		memcpy(_stData.m_chData1,sSelItemText,sSelItemText.GetLength());
//		//=============================================================
//	}
//
//	CRect rect;
//	CPoint pt;
//	GetDlgItem(IDC_BTSEARCH)->GetWindowRect(rect);
//	pt.x = rect.left;
//	pt.y = rect.top;
//	
//	memcpy(_stData.m_chTitle,strItemTitle,strItemTitle.GetLength());	
//	_stData.m_lTabNum = TABNO_ASSISTINDEX;
//	_stData.m_lSubData1 = m_nIndexSelected;
//
//	CFavoriteThingsMng pFavoriteMng;
//	pFavoriteMng.SetPath(m_pParentDlg->m_strRootPath+ "\\User\\Chart");
//	pFavoriteMng.SaveItemInfo(&_stData,"JipyoFile");
//
//	pos = m_ListTree.FindIndex(1);
//	treeItem  = m_ListTree.GetAt(pos);
//
//	treeItem->DeleteAllItems();
//	pFavoriteMng.LoadItemInfo("JipyoFile");
//	int wSizeItem = pFavoriteMng.m_ArrayFvrItem.GetSize();
//	for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
//	{
//		_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
//		treeItem->InsertItem(_stData.m_chData1);
//	}		
/////////////////////////
	if(m_nIndexSelected != 1)
		return;

	POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
	CTreeCtrl* treeMyFavorite  = m_ListTree.GetAt(pos);
	stFvrData _stData;

	CString strItemName;
	CString strMessage;

	HTREEITEM hItem = treeMyFavorite->GetSelectedItem();
	strItemName = treeMyFavorite->GetItemText(hItem);
	if(strItemName.GetLength() <= 0)
	{
		MessageBox("관심지표 중 삭제할 지표를 하나 선택하십시오.",DLLTITLENAME);
		return;
	}
	strMessage.Format("%s 를 삭제하시겠습니까?",strItemName);
	int nRetValue = MessageBox(strMessage,DLLTITLENAME,MB_YESNO|MB_ICONSTOP);
	if(nRetValue==IDYES)
	{
		CFavoriteThingsMng pFavoriteMng;
		pFavoriteMng.SetPath(m_pParentDlg->GetUserPath());
		pFavoriteMng.LoadItemInfo("JipyoFile");
		pFavoriteMng.DeleteItemInfo(strItemName, "JipyoFile");
		treeMyFavorite->DeleteAllItems();
		int wSizeItem = pFavoriteMng.m_ArrayFvrItem.GetSize();
		for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
		{
			_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
			treeMyFavorite->InsertItem(_stData.m_chData1);
		}		
		//m_pFavoriteMng->DeleteGrp(strGrpName);
		//LoadTreeData();
	}

//	HTREEITEM hItem = NULL;
//	hItem = m_treeItems.GetSelectedItem();
//	HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);			
//	if(hPItem==NULL) 
//	{
//		return;
//	}
//
//	
////	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
////	{
////		CString strText			= m_treeItems.GetItemText(hItem);
////		m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
////	}
//	
//	//시장지표와 아닌것의 구분
//	CString strPItemText	= m_treeItems.GetItemText(hPItem);
//	CString strText			= m_treeItems.GetItemText(hItem);
//
//	BOOL bMarket = FALSE;
//	if(strPItemText == "시장지표")		
//	{		
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText, FROM_TABJIPYO_MARKET);
//	}
//	else
//	{
//		if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
//	}
///*
//	switch(m_nIndexSelected)	
//	{
//	case 0:	// 기술적 지표
//		{
//			HTREEITEM hItem = NULL;
//			hItem = m_treeItems.GetSelectedItem();
//			HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);			
//			if(hPItem==NULL) 
//			{
//				return;
//			}
//			CString strText = m_treeItems.GetItemText(hItem);
//			m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
//		}
//	break;
//	case 1://업종지수
//		{
//			HTREEITEM hItem = NULL;
//			hItem = m_treeItems.GetSelectedItem();
//			HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);			
//			if(hPItem==NULL) 
//			{
//				return;
//			}
//			DWORD wItemData = m_treeItems.GetItemData(hItem);
//			m_pChartMng->SendMessage(UMSG_ADDUPJONGDATA,wItemData);
//		}
//		break;
//	case 2://시장지표
//		{
//			int nCurSel = m_listItems.GetCurSel();
//			char szItem[64];
//			m_listItems.GetItemText(nCurSel,0,szItem,sizeof(szItem));
//			m_pChartMng->SendMessage(UMSG_ADDMARKETDATA,(WPARAM)&szItem);
//		}
//		break;
//
//	}*/	
}

void CTabIndices::OnBtsearch() 
{
	// TODO: Add your control notification handler code here
	stFvrData _stData;
	CString strItem;
	CString strItemTitle;
	::ZeroMemory(&_stData,sizeof(_stData));
	strItemTitle = '[';
	strItemTitle += TAB_ASSISTINDEX;
	strItemTitle += "] ";

	POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
	CTreeCtrl* treeItem  = m_ListTree.GetAt(pos);
	HTREEITEM hItem = NULL;
	hItem = treeItem->GetSelectedItem();
	//HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);
	if(hItem==NULL) 
	{
		MessageBox("하나를 선택하십시오.",DLLTITLENAME);
		return;
	}		
	
	CString strPItem = szIndexGroupName[m_nIndexSelected];
	CString sSelItemText = treeItem->GetItemText(hItem);
	if(sSelItemText.GetLength())
	{
		strItemTitle += sSelItemText;
		strItemTitle += '(';
		strItemTitle += strPItem;
		strItemTitle += ')';
		DWORD wItemData = m_treeItems.GetItemData(hItem);


		//업종 지수에서도 문자열로 저장해도 되는 지 확인이 필요한다.
		//2005. 08. 10 by sy,nam
		memcpy(_stData.m_chData1,sSelItemText,sSelItemText.GetLength());
		//=============================================================
	}

	CRect rect;
	CPoint pt;
	GetDlgItem(IDC_BTSEARCH)->GetWindowRect(rect);
	pt.x = rect.left;
	pt.y = rect.top;
	
	memcpy(_stData.m_chTitle,strItemTitle,strItemTitle.GetLength());	
	_stData.m_lTabNum = TABNO_ASSISTINDEX;
	_stData.m_lSubData1 = m_nIndexSelected;

	CFavoriteThingsMng pFavoriteMng;
	pFavoriteMng.SetPath(m_pParentDlg->GetUserPath());
	pFavoriteMng.SaveItemInfo(&_stData,"JipyoFile");

	pos = m_ListTree.FindIndex(1);
	treeItem  = m_ListTree.GetAt(pos);

	treeItem->DeleteAllItems();
	pFavoriteMng.LoadItemInfo("JipyoFile");
	int wSizeItem = pFavoriteMng.m_ArrayFvrItem.GetSize();
	for(int wIndexItem=0;wIndexItem<wSizeItem;wIndexItem++)
	{
		_stData = pFavoriteMng.m_ArrayFvrItem.GetAt(wIndexItem);
		treeItem->InsertItem(_stData.m_chData1);
	}		

	
	//m_pParentDlg->SendMessage(UMSG_SHOWREGFAVORITEDLG,(WPARAM)&pt,(LPARAM)&_stData);	
//	stFvrData _stData;
//	CString strItem;
//	CString strItemTitle;
//	::ZeroMemory(&_stData,sizeof(_stData));
//	strItemTitle = '[';
//	strItemTitle += TAB_ASSISTINDEX;
//	strItemTitle += "] ";
//
//	HTREEITEM hItem = NULL;
//	hItem = m_treeItems.GetSelectedItem();
//	HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);
//	if(hPItem==NULL) 
//	{
//		MessageBox("하나를 선택하십시오.",DLLTITLENAME);
//		return;
//	}		
//
//	CString strPItem = m_treeItems.GetItemText(hPItem);
//	CString sSelItemText = m_treeItems.GetItemText(hItem);
//	if(sSelItemText.GetLength())
//	{
//		strItemTitle += sSelItemText;
//		strItemTitle += '(';
//		strItemTitle += strPItem;
//		strItemTitle += ')';
//		DWORD wItemData = m_treeItems.GetItemData(hItem);
//
//
//		//업종 지수에서도 문자열로 저장해도 되는 지 확인이 필요한다.
//		//2005. 08. 10 by sy,nam
//		memcpy(_stData.m_chData1,sSelItemText,sSelItemText.GetLength());
//		//=============================================================
//	}
//	
//
//			
///*
//	
//	switch(m_nIndexSelected)
//	{
//	case 0:	// 기술적지표
//	case 2:	// 시장지표
//		{
//			int nCurSel = -1;
//			nCurSel = m_listItems.GetCurSel();
//			if(nCurSel==-1) 
//			{
//				MessageBox("하나를 선택하십시오.",DLLTITLENAME);
//				return;
//			}
//			strItem = m_listItems.GetItemText(nCurSel,0);
//			strItemTitle += strItem;
//			memcpy(_stData.m_chData1,strItem,strItem.GetLength());
//		}
//		break;
//	case 1:	// 업종지수
//		{
//			HTREEITEM hItem = NULL;
//			hItem = m_treeItems.GetSelectedItem();
//			HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);
//			if(hPItem==NULL) 
//			{
//				MessageBox("하나를 선택하십시오.",DLLTITLENAME);
//				return;
//			}		
//			CString strPItem = m_treeItems.GetItemText(hPItem);
//			strItemTitle += m_treeItems.GetItemText(hItem);
//			strItemTitle += '(';
//			strItemTitle += strPItem;
//			strItemTitle += ')';
//			DWORD wItemData = m_treeItems.GetItemData(hItem);
////			if(strPItem=="KOSPI")
////			{
////				wItemData += 1000;
////			}
////			else if(strPItem=="KOSDAQ")
////			{
////				wItemData += 2000;
////			}
//			_stData.m_lSubData2 = wItemData;
//		}		
//		break;
//	default:
//		return;
//		break;
//	}
//*/
//	CRect rect;
//	CPoint pt;
//	GetDlgItem(IDC_BTSEARCH)->GetWindowRect(rect);
//	pt.x = rect.left;
//	pt.y = rect.top;
//	
//	memcpy(_stData.m_chTitle,strItemTitle,strItemTitle.GetLength());	
//	_stData.m_lTabNum = TABNO_ASSISTINDEX;
//	_stData.m_lSubData1 = m_nIndexSelected;
//	m_pParentDlg->SendMessage(UMSG_SHOWREGFAVORITEDLG,(WPARAM)&pt,(LPARAM)&_stData);	
}

void CTabIndices::OnDblclkTreeitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnBtapply();
	*pResult = 0;
}

void CTabIndices::OnDestroy() 
{
	CTabDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	SetConfig(FALSE);
	RemoveAllTree();
	m_ImageListIcon.DeleteImageList();
	m_ImageOutbarIcon.DeleteImageList();
}

void CTabIndices::OnBegindragTreeitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bLBtnDown = TRUE;
	m_treeItems.Select(pNMTreeView->itemNew.hItem,TVGN_CARET);	
	SetCapture();		
	*pResult = 0;
}

void CTabIndices::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLBtnDown)
	{
		// 드래그임을 표시
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
		m_pMultiChartWnd->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
	}	
	
	CTabDlg::OnMouseMove(nFlags, point);
}

void CTabIndices::OnLButtonUp(UINT nFlags, CPoint point) 
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
			CRect rectChartMng, rectList;
			m_pParentDlg->GetWindowRect(rectChartMng);
			m_listItems.GetWindowRect(rectList);
			if(!rectChartMng.PtInRect(ptDrag))
			{
				OnBtapply();
			}
			else if(rectList.PtInRect(ptDrag))
			{
				if(m_bRun == FALSE)
					OnBtnAdditem();	
			}
		}
		m_bDragNDrop = FALSE;
	}		
	CTabDlg::OnLButtonUp(nFlags, point);
}

void CTabIndices::OnBegindragListitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_bLBtnDown = TRUE;
	SetCapture();	
	*pResult = 0;
}

//void CTabIndices::OnItemchangedListindices(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
//	// TODO: Add your control notification handler code here
//	m_nIndexSelected = phdn->iItem;
//	SetItemForm();	
//	*pResult = 0;
//}


void CTabIndices::OnBtnShowRepeat() 
{	
	m_bShowJipyoItem = !m_bShowJipyoItem;
	CString strImagePath = m_pParentDlg->m_szImageDir;
	CString strImageUp, strImageDown;
			
	if(m_bShowJipyoItem)
	{
		//m_btnShowList.FP_vSetImage(m_pIL_W30H19, 2, 3);

		m_ySplitterY1	.ShowWindow(SW_SHOW);
		m_listItems		.ShowWindow(SW_SHOW);
		m_btnPrev		.ShowWindow(SW_SHOW);
		m_btnStop		.ShowWindow(SW_SHOW);
		m_btnNext		.ShowWindow(SW_SHOW);
		m_RadioManual	.ShowWindow(SW_SHOW);
		m_RadioAuto		.ShowWindow(SW_SHOW);				
		m_btnAddItem	.ShowWindow(SW_SHOW);
		m_btnDelItem	.ShowWindow(SW_SHOW);
		m_cmbTime		.ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_RADIO_MANUAL	)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_RADIO_AUTO	)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BTN_ADDITEM	)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_BTN_DELITEM	)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_CMB_TIME		)->ShowWindow(SW_SHOW);


	}
	else
	{
//		m_btnShowList.FP_vSetImage(m_pIL_W30H19, 0, 1);

		m_ySplitterY1	.ShowWindow(SW_HIDE);
		m_listItems		.ShowWindow(SW_HIDE);
		m_btnPrev		.ShowWindow(SW_HIDE);
		m_btnStop		.ShowWindow(SW_HIDE);
		m_btnNext		.ShowWindow(SW_HIDE);
		m_RadioManual	.ShowWindow(SW_HIDE);
		m_RadioAuto		.ShowWindow(SW_HIDE);				
		m_btnAddItem	.ShowWindow(SW_HIDE);
		m_btnDelItem	.ShowWindow(SW_HIDE);
		m_cmbTime		.ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_RADIO_MANUAL	)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_RADIO_AUTO	)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BTN_ADDITEM	)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_BTN_DELITEM	)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_CMB_TIME		)->ShowWindow(SW_HIDE);
	}
	RecalcLayout();


	
//	m_btnShowList.Invalidate();
}

void CTabIndices::OnBtnNext() 
{
	
	
	// TODO: Add your control notification handler code here	
//	MakeCheckedJipyoList();
//	if(m_RadioAuto.GetCheck()) // 자동
//	{
//		OnBtnStop();
//		
//		m_bRun = TRUE;
//		m_listItems.SetAutoRunRecycle(m_bRun);		
//
//		if(m_cmbTime.GetCurSel() != CB_ERR)
//		{	
//			//m_listItems		.EnableWindow(FALSE);
//			m_btnStop		.EnableWindow(TRUE);
//			m_RadioAuto		.EnableWindow(FALSE);
//			m_RadioManual	.EnableWindow(FALSE);
//			m_cmbTime		.EnableWindow(FALSE);
//			m_btnAddItem	.EnableWindow(FALSE);
//			m_btnDelItem	.EnableWindow(FALSE);
//			m_btnNext		.EnableWindow(FALSE);
//			m_btnPrev		.EnableWindow(FALSE);
//
//			CString strText;
//			m_cmbTime.GetLBText(m_cmbTime.GetCurSel(), strText);
//			strText.Replace("초", "");
//			strText.Remove(' ');
//			SetTimer(ID_SRND_TIMER, atoi(strText) * 1000, NULL);
//			m_bBack = FALSE;	
//		}
//	}
//	else
//	{
//		m_bBack = FALSE;		
//		Move_Selected_JipyoItem(m_nOldSelJipyoIndex);
//	}
}

void CTabIndices::OnBtnPrev() 
{
	
	
	// TODO: Add your control notification handler code here
//	MakeCheckedJipyoList();
//	if(m_RadioAuto.GetCheck())
//	{
//		OnBtnStop();
//
//		m_bRun = TRUE;
//		m_listItems.SetAutoRunRecycle(m_bRun);		
//
//		if(m_cmbTime.GetCurSel() != CB_ERR)
//		{	
//			//m_listItems		.EnableWindow(FALSE);
//			m_btnStop		.EnableWindow(TRUE);
//			m_RadioAuto		.EnableWindow(FALSE);
//			m_RadioManual	.EnableWindow(FALSE);
//			m_cmbTime		.EnableWindow(FALSE);
//			m_btnAddItem	.EnableWindow(FALSE);
//			m_btnDelItem	.EnableWindow(FALSE);
//			m_btnNext		.EnableWindow(FALSE);
//			m_btnPrev		.EnableWindow(FALSE);
//
//
//			CString strText;
//			m_cmbTime.GetLBText(m_cmbTime.GetCurSel(), strText);
//			strText.Replace("초", "");
//			strText.Remove(' ');
//			SetTimer(ID_SRND_TIMER, atoi(strText) * 1000, NULL);
//			m_bBack = TRUE;
//		}	
//	}
//	else if(m_RadioManual.GetCheck())
//	{
//		m_bBack = TRUE;
//		
//		Move_Selected_JipyoItem(m_nOldSelJipyoIndex);
//	}
}

void CTabIndices::OnBtnStop() 
{

	// TODO: Add your control notification handler code here
	KillTimer(ID_SRND_TIMER);

	m_bRun = FALSE;	
//	m_listItems.SetAutoRunRecycle(m_bRun);		

	m_btnStop		.EnableWindow(FALSE);
	m_RadioAuto		.EnableWindow(TRUE);
	m_RadioManual	.EnableWindow(TRUE);
	m_cmbTime		.EnableWindow(TRUE);
	m_btnAddItem	.EnableWindow(TRUE);
	m_btnDelItem	.EnableWindow(TRUE);
	m_btnNext		.EnableWindow(TRUE);
	m_btnPrev		.EnableWindow(TRUE);
	m_listItems		.EnableWindow(TRUE);

	m_nOldSelJipyoIndex = -1;	
}

void CTabIndices::OnSelchangeCmbTime() 
{
	// TODO: Add your control notification handler code here
	OnBtnStop();
}

void CTabIndices::OnRadioAuto() 
{
	// TODO: Add your control notification handler code here
	
}

void CTabIndices::OnRadioManual() 
{
	// TODO: Add your control notification handler code here
	
}

void CTabIndices::OnBtnAdditem() 
{	
	HTREEITEM hItem = NULL;
	hItem = m_treeItems.GetSelectedItem();
	HTREEITEM hPItem = m_treeItems.GetParentItem(hItem);			
	if(hPItem==NULL) 
	{
		return;
	}
	CString strText = m_treeItems.GetItemText(hItem);

	if(m_listItems.GetSafeHwnd()) // 2005. 04. 22 by Nam
	{
		int nTot = m_listItems.GetItemCount();
		for(int i = 0; i < nTot; i++)
		{
			CString strValue = m_listItems.GetItemText(i,0);
			if(strValue.CompareNoCase(strText) == 0)
				return;
		}
		
		m_listItems.InsertItem(nTot, strText);
		m_listItems.SetCheckbox(nTot, 0, 0); //체크 박스 형태로 추가한다.
		m_listItems.SetCurSel(nTot);
		m_nOldSelJipyoIndex = -1;
	}
}

void CTabIndices::OnBtnDelitem() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listItems.GetFirstSelectedItemPosition();
	if(pos)
	{
		int nItem = m_listItems.GetNextSelectedItem(pos);
		m_listItems.DeleteItem(nItem);
		(nItem > 0) ? nItem-- : nItem = 0;
		m_listItems.SetCurSel(nItem);
		m_nOldSelJipyoIndex = -1;
	}
	else
		AfxMessageBox("삭제할 항목을 선택해 주십시오");
}

void CTabIndices::OnTimer(UINT nIDEvent) 
{	
	if(m_bRun == FALSE) return;

	Move_Selected_JipyoItem(m_nOldSelJipyoIndex);
	CTabDlg::OnTimer(nIDEvent);
}

void CTabIndices::Move_Selected_JipyoItem(int nRecvItem)
{
//	int nStartIndex = - 1;
//	if(m_nOldSelJipyoIndex < 0)
//	{
//		int nItem = -1;
//		POSITION pos = m_listItems.GetFirstSelectedItemPosition();
//
//		//선택된 항목이 없는 경우 강제로 첫번째 항목으로 선택을 시킨다.
//		if(pos == NULL)
//		{
//			m_listItems.SetCurSel(0);
//			pos = m_listItems.GetFirstSelectedItemPosition();
//		}
//
//		if(pos)
//		{			
//			//현재 마우스로 선택되어진 항목의 Zero Base 기반 Index를 가져온다.
//			nItem = m_listItems.GetNextSelectedItem(pos);
//			if( m_listItems.GetCheckbox(nItem, 0) == CHECKED )
//			{
//				nStartIndex = nItem;	
//			}
//			else
//			{
//				nStartIndex = LookUpStartIndex(nItem); // 시작할 위치를 구한다.
//			}
//			
//			if(nStartIndex == -1)
//			{
//				// 신규로 추가 되었으나 체크가 안된 상태인 경우 가장 다음에 선택된 항목을  
//				// 선택한것으로 설정해 본다.
//				nStartIndex = LookUpStartIndex(0);
//				//m_nOldSelJipyoIndex = 
//				//Move_Selected_JipyoItem(0);
//			}
//			
//			m_nOldSelJipyoIndex = nStartIndex;
//		}
//	}
//	else
//	{		
//		POSITION posCompare = m_listItems.GetFirstSelectedItemPosition();
//		if(posCompare)
//		{				
//			//현재 마우스로 선택되어진 항목의 Zero Base 기반 Index를 가져온다.
//			int nItem = m_listItems.GetNextSelectedItem(posCompare);
//			if( m_listItems.GetCheckbox(nItem, 0) == CHECKED )
//			{
//				if(m_nOldSelJipyoIndex != nItem)
//				{
//					m_nOldSelJipyoIndex = -1;
//					Move_Selected_JipyoItem(0);
//				}
//			}
//		}
//
//		POSITION pos = m_listCheckedJipyo.GetHeadPosition();
//		CString strIndex;
//		while( pos != NULL )
//		{
//			strIndex = _T("");
//			strIndex = (CString)m_listCheckedJipyo.GetAt(pos);
//			if(strIndex.GetLength())
//			{					
//				if(atoi(strIndex) == m_nOldSelJipyoIndex)
//					break;
//			}
//			m_listCheckedJipyo.GetNext(pos);
//		}
//
//		if(m_bBack)
//		{			
//			if(pos != NULL)
//				m_listCheckedJipyo.GetPrev(pos);
//
//			if(pos == NULL)
//				pos = m_listCheckedJipyo.GetTailPosition();
//			
//			strIndex = (CString)m_listCheckedJipyo.GetAt(pos);
//		}
//		else
//		{
//			if(pos != NULL)
//				m_listCheckedJipyo.GetNext(pos);
//			
//			if(pos == NULL)
//				pos = m_listCheckedJipyo.GetHeadPosition();
//			
//			strIndex = (CString)m_listCheckedJipyo.GetAt(pos);
//		}
//		m_nOldSelJipyoIndex = nStartIndex = atoi(strIndex);
//	}
//
//	m_listItems.SetCurSel(nStartIndex);				
//	CString strText = m_listItems.GetItemText(nStartIndex, 0);
//
//	if(m_strOldJipyo.GetLength())
//	{			
//		//( m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)m_strOldJipyo) );
//		//
//			m_pChartMng->SendMessage(UMSG_DEL_CHART_BLOCK,(WPARAM)(LPCTSTR)m_strOldJipyo);
//		//	m_strOldJipyo = _T("");
//		//
//	}
//
//	m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
//	m_strOldJipyo = strText;


//	if(strText.GetLength())
//		AfxMessageBox(strText);

/*
	int nItem = nRecvItem;
	POSITION pos = NULL;
	if(nItem == -1)
	{	
		pos = m_listItems.GetFirstSelectedItemPosition();	
		if(pos)
		{
			//현재 마우스로 선택되어진 항목의 Zero Base 기반 Index를 가져온다.
			nItem = m_listItems.GetNextSelectedItem(pos);
			m_listItems.GetCheckbox(nItem, 0);
		}
		else
		{
			nItem = 0;
			//OnBtnStop();
			//AfxMessageBox("돌려보기를 시작할 항목을 선택해 주십시오");		
			//return;
		}
	}


	int nStartIndex = LookUpStartIndex(nItem);	
	if(m_bBack)
	{
		m_nOldSelJipyoIndex = nStartIndex - 1;
		if(m_nOldSelJipyoIndex < 0)
			m_nOldSelJipyoIndex = m_listItems.GetItemCount() - 1;			
	}
	else
	{
		m_nOldSelJipyoIndex = nStartIndex + 1;
		if(m_nOldSelJipyoIndex == (m_listItems.GetItemCount() ))
			m_nOldSelJipyoIndex = 0;
	}

	CString sTemp;
	CString strText;
	sTemp.Format("nStartIndex : %d, m_nOldSelJipyoIndex : %d, 텍스트 : %s", \
					nStartIndex, m_nOldSelJipyoIndex,strText);
	OutputDebugString(sTemp);

	m_listItems.SetCurSel(nStartIndex);				
	strText = m_listItems.GetItemText(nStartIndex, 0);
	//AfxMessageBox(strText);
//===========================================================================================
/*	if(m_bBack)
	{	
		CString strText;
		int nCnt = 0;
		for(int i = nItem; i >= 0; i--)
		{			
			if(m_listItems.GetCheckbox(i, 0))
			{
				// 다음 체크 항목이 어디인지 미리 기억해 둬야 하므로.
				//반복문을 2번째 돌릴때 멈춘다. =============================================
				nCnt++;
				if(nCnt == 2)
				{
					m_nOldSelJipyoIndex = i;
					break;
				}//============================================================================
				else if( (nCnt == 1) && ( (m_nOldSelJipyoIndex == 0) || (m_nOldSelJipyoIndex == -1)))
				{
					//첫번째 체크되었을 때 이미 항목의 가장 위라면
                    //다음 시작 위치는 리스트의 가장 마지막으로 옮겨서 검색해야 하기에 
					//리스트 전체 갯수의 값을 m_nOldSelJipyoIndex에 저장한다.
					nItem = m_listItems.GetItemCount() - 1;
					m_nOldSelJipyoIndex = nItem;
				}

				m_listItems.SetCurSel(i);				
				strText = m_listItems.GetItemText(i, 0);
				OutputDebugString(strText);
				/*
				if(m_pChartMng->GetSafeHwnd() && strText.GetLength()) // 2005. 05. 13 by Nam
				{
					// 예전 지표 내용이 있고, 현재 추가하려고 하는 지표항목이 기존 항목과
					// 같지 않은 경우에 삭제한다.
					if( m_strOldJipyo.GetLength() && (strText != m_strOldJipyo))
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)m_strOldJipyo);

					if(m_strOldJipyo != strText)
					{	
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
						m_strOldJipyo = strText;
					}
				}			
			}
		}
	}
	else if(m_bBack == FALSE)
	{
		int nCnt = 0;
		int nTot = m_listItems.GetItemCount();
		CString strText, strOldText;
		for(int i = nItem; i < nTot ; i++)
		{
			if(m_listItems.GetCheckbox(i, 0))
			{
				nCnt++;
				// 다음 체크 항목이 어디인지 미리 기억해 둬야 하므로.
				//반복문을 2번째 돌릴때 멈춘다. =============================================
				if(nCnt == 2)
				{
					m_nOldSelJipyoIndex = i;
					break;
				}//============================================================================
				else if( (nCnt == 1) && ( (m_nOldSelJipyoIndex == (nTot - 1)) || (m_nOldSelJipyoIndex == -1)))
				{
					//첫번째 체크되었을 때 이미 항목의 가장 마지막 이라면
                    //다음 시작 위치는 리스트의 가장 처음으로 옮겨서 검색해야 하기에 
					//리스트 전체 갯수의 값을 m_nOldSelJipyoIndex에 저장한다.
					nItem = 0;
					m_nOldSelJipyoIndex = nItem;
				}

				m_listItems.SetCurSel(i);				
				strText = m_listItems.GetItemText(i, 0);
				OutputDebugString(strText);
				/*if(m_pChartMng->GetSafeHwnd() && strText.GetLength()) // 2005. 05. 13 by Nam
				{
					if(strOldText.GetLength())
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strOldText);

					m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
					strOldText = strText;
				}
			}
		}
	}*/
	
/*	if(m_bBack)
	{
		CString strText, strOldText;
		for(int i = nItem; i >= 0; i--)
		{			
			if(m_listItems.GetCheckbox(i, 0))
			{
				m_listItems.SetCurSel(i);				
				strText = m_listItems.GetItemText(i, 0);
				OutputDebugString(strText);
				if(m_pChartMng->GetSafeHwnd() && strText.GetLength()) // 2005. 05. 13 by Nam
				{
					if(strOldText.GetLength())
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strOldText);

					m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
					strOldText = strText;
				}
			}
		}		
	}
	else
	{
		int nTot = m_listItems.GetItemCount();
		CString strText, strOldText;
		for(int i = nItem; i < nTot ; i++)
		{
			if(m_listItems.GetCheckbox(i, 0))
			{
				m_listItems.SetCurSel(i);				
				strText = m_listItems.GetItemText(i, 0);
				OutputDebugString(strText);
				if(m_pChartMng->GetSafeHwnd() && strText.GetLength()) // 2005. 05. 13 by Nam
				{
					if(strOldText.GetLength())
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strOldText);

					m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)strText);
					strOldText = strText;
				}
			}
		}
	}
	*/	
}

void CTabIndices::ClearCheckJipyoList()
{	
	POSITION pos = NULL;
	CString p;
	while( (pos = m_listCheckedJipyo.GetHeadPosition()) != NULL)
	{
		p = _T("");
		p = m_listCheckedJipyo.GetAt(pos);
		if(p.GetLength())
		{			
			m_listCheckedJipyo.RemoveAt(pos);    
		}
		//m_listCheckedJipyo.GetNext(pos);
	}


/*	POSITION pos;	
	CString* pa = NULL;
	CString  strKey;	
	
	for(pos = m_listCheckedJipyo.GetHeadPosition(); pos != NULL;)
	{
		m_listCheckedJipyo.GetNextAssoc(pos, strKey, (void*&)pa);
		delete pa;
		pa = NULL;
	}
	m_listCheckedJipyo.RemoveAll();		
*/
}

void CTabIndices::MakeCheckedJipyoList()
{
	ClearCheckJipyoList();
	for(int i = 0; i < m_listItems.GetItemCount(); i++)
	{			
		if(m_listItems.GetCheckbox(i, 0))
		{
			CString sKey, sValue;
			sKey.Format("%d", i);			
			m_listCheckedJipyo.AddTail(sKey);			
		}
	}
}

//지표 돌려보기를 시작할 Index를 넘긴다.
int CTabIndices::LookUpStartIndex(int  nSelIndex)
{
	if(m_bBack)
	{
		POSITION pos = m_listCheckedJipyo.GetTailPosition();
		CString p;
		while( pos != NULL )
		{
			p = _T("");
			p = (CString)m_listCheckedJipyo.GetAt(pos);
			if(p.GetLength())
			{			
				if(atoi(p) <= nSelIndex)
					return atoi(p);			
			}
			m_listCheckedJipyo.GetPrev(pos);
		}		
	}
	else
	{
		POSITION pos = m_listCheckedJipyo.GetHeadPosition();
		CString p;
		while( pos != NULL )
		{
			p = _T("");
			p = (CString)m_listCheckedJipyo.GetAt(pos);
			if(p.GetLength())
			{					
				if(atoi(p) >= nSelIndex)
					return atoi(p);				
			}
			m_listCheckedJipyo.GetNext(pos);
		}
	}

	return -1;	
}

void CTabIndices::OnKillFocus(CWnd* pNewWnd) 
{
	CTabDlg::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	m_bRun = FALSE;
}

LRESULT CTabIndices::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case UMSG_STOP_JIPYOCYCLE:
			{
				m_bRun = FALSE;//AfxMessageBox("지표 멈춰라");
				OnBtnStop();
			}
			break;
		case UDM_MAP_OPEN_SEARCH:
			{
				//if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
				//	m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)lParam, FROM_TABJIPYO_MARKET);
				if(szIndexGroupName[m_nIndexSelected] == "시장지표")		
				{		
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)lParam, FROM_TABJIPYO_MARKET);
				}
				else
				{
					if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
						m_pChartMng->SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)lParam);
				}
				//요청에 의해 뺌(Bold처리) 2006.12.08
				//RefreshIndex();
				//OnBtapply();
				break;
			}
		case WMU_OUTBAR_NOTIFY:
			{
				if(wParam == NM_FOLDERCHANGE)
				{
					m_nIndexSelected = lParam;
					if(m_nIndexSelected == 2)
					{
						POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
						CTreeCtrl* pTreeCtrl  = m_ListTree.GetAt(pos);
						HTREEITEM hmyItem = NULL;

						BOOL bExistance = FALSE;
						CString strIndicator;

						CStringArray strArrayIndicator;	
						if(m_pChartMng->GetSafeHwnd())
						{
							m_pChartMng->SendMessage(UMSG_GETNAMEINDICATOR,(WPARAM)"전체지표",(LPARAM)&strArrayIndicator);
							int lSize = strArrayIndicator.GetSize();

							pTreeCtrl->DeleteAllItems();

							for(long lPos=0;lPos<lSize;lPos++)
							{
								strIndicator = strArrayIndicator.GetAt(lPos);

								long lPos1 = strIndicator.Find("&&");
								if(lPos1>=0)
								{
									strIndicator.Delete(lPos);
								}	
								bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
								if(bExistance)
								{
									// (2008/9/13 - Seung-Won, Bae) for Multi-Language
									_MHWND( NULL);
									strIndicator = _MTOL2( strIndicator);
									pTreeCtrl->InsertItem(strIndicator);
								}
							}
						}
					}
					//요청에 의해 뺌(Bold처리) 2006.12.08
					//else
					//	RefreshIndex();
				}
				//AfxMessageBox("WMU_OUTBAR_NOTIFY");
			}
			break;
		case UDM_TREE_RBUTTON:
			{
				CMenu menu;
				menu.CreatePopupMenu();

				if(m_nIndexSelected == 1)	//관심지표일경우 Del메뉴추가
					menu.AppendMenu(MF_STRING, MENU_ID_DEL, (LPCTSTR)MENU_TITLE_DEL); 
				else						//그밖 Add메뉴추가
					menu.AppendMenu(MF_STRING, MENU_ID_ADD, (LPCTSTR)MENU_TITLE_ADD); 

				CPoint pPoint = (CPoint)wParam;
				GetCursorPos(&pPoint);
				menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pPoint.x, pPoint.y, this);
				menu.DestroyMenu();
			}
			break;
	}
	if(message == RMSG_DRAGSTART)
	{
		m_pMultiChartWnd->SendMessage(RMSG_DRAGSTART, wParam, lParam);
	}
	else if(message == RMSG_DRAGEND)
	{
		CDragNDrop* pDnD = (CDragNDrop*)lParam;				
		CRect rectChartMng;
		m_pParentDlg->GetWindowRect(rectChartMng);
		if(!rectChartMng.PtInRect(*pDnD->GetCursorPoint()))
		{
			m_pMultiChartWnd->PostMessage(RMSG_DRAGEND, wParam, lParam);
		}
	}
	
	return CTabDlg::WindowProc(message, wParam, lParam);
}

void CTabIndices::Init_Button()
{	
//	m_btnSearch.FP_vSetImage(m_pIL_W93H20, 0, 2);
//	m_btnSearch.FP_vSetToolTipText("사용자");
	
//	m_btnApply.FP_vSetImage(m_pIL_W60H19, 32, 34);
//	m_btnApply.FP_vSetToolTipText("적용");

	//지표 돌려보기 열기	
//	m_btnShowList.FP_vSetImage(m_pIL_W30H19, 0, 1);

	//지표정지
//	m_btnStop.FP_vSetImage(m_pIL_W33H19, 3, 4, FALSE, "", FALSE, 5);

	//역방향플레이
//	m_btnPrev.FP_vSetImage(m_pIL_W33H19, 9, 10, FALSE, "", FALSE, 11);

	//정방향플레이
//	m_btnNext.FP_vSetImage(m_pIL_W33H19, 0, 1, FALSE, "", FALSE, 2);

	//돌려보기 추가
//	m_btnAddItem.FP_vSetImage(m_pIL_W88H19, 0, 1);

	//선택삭제	
//	m_btnDelItem.FP_vSetImage(m_pIL_W60H19, 3, 4);

	//관심지표등록
//@유진삭제 m_btnSearch	.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "등록", FALSE);
	m_btnSearch	.FP_vSetImage(m_pIL_W43H20, 0, 2, TRUE, "등록", FALSE);

	//삭제
//@유진삭제 m_btnApply	.FP_vSetImage(m_pIL_W43H19, 0, 2, TRUE, "삭제", FALSE);
	m_btnApply	.FP_vSetImage(m_pIL_W43H20, 0, 2, TRUE, "삭제", FALSE);

	//지표찾기
	m_btnFind	.FP_vSetImage(m_pIL_W20H20, 76, 78, TRUE, "", FALSE);

	//찾기영역 확장축소
	m_btnExtend	.FP_vSetImage(m_pIL_W20H20, 48, 50, TRUE, "", FALSE);
}

void CTabIndices::RemoveAllTree()
{
	CTrTreeCtrl* pTree = NULL;
	POSITION pos = m_ListTree.GetHeadPosition();
	for (;pos;)
	{
		pTree = m_ListTree.GetNext(pos);
		if (pTree)
		{
			pTree->DestroyWindow();
			delete pTree;
		}
	}
	
	m_ListTree.RemoveAll();
}

void CTabIndices::RefreshIndex()
{
	POSITION pos = m_ListTree.FindIndex(m_nIndexSelected);
	CTreeCtrl* pTreeCtrl  = m_ListTree.GetAt(pos);
	HTREEITEM hmyItem = NULL;

	BOOL bExistance = FALSE;
	// Show all of the children of hmyItem in bold.
	if (pTreeCtrl->ItemHasChildren(hmyItem))
	{
		HTREEITEM hItem = pTreeCtrl->GetChildItem(hmyItem);

		while (hItem != NULL)
		{
			CString strIndicator = pTreeCtrl->GetItemText(hItem);
			if(m_pChartMng)
				bExistance = m_pChartMng->SendMessage(UMSG_HASGRAPH,(WPARAM)(LPCTSTR)strIndicator);
			if(bExistance)
				pTreeCtrl->SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
			else
				pTreeCtrl->SetItemState(hItem, ~TVIS_BOLD, TVIS_BOLD);
			hItem = pTreeCtrl->GetNextSiblingItem(hItem);
		}
	}
}

//007 지표찾기
void CTabIndices::OnBnClickedBtnFind()
{
	CString strFindData;
	CStringArray strResult;

	m_listFind.ResetContent();
	m_editFind.GetWindowText(strFindData);

	if(!m_bFindExtend)
	{
		m_listFind.ShowWindow(TRUE);
		m_bFindExtend = TRUE;
		m_btnExtend	.FP_vSetImage(m_pIL_W20H20, 44, 46, TRUE, "", FALSE);
		Invalidate();
		RecalcLayout();		
	}	
	
	 // 리스트박스에 지표항목 채우기
	GetFindItemList(TFI_JIPYOITEM_TITLE, strFindData, strResult);
	for (int i = 0; i < strResult.GetSize(); i++)
		m_listFind.AddString(strResult[i]);
}

//007 지표검색{{
void CTabIndices::GetFindItemList(int nFindType, CString strFindData, CStringArray &strResult)
{

	CString strIndicator, strTemp;
	strFindData.MakeUpper();

	int iIndex = 0;

	for (POSITION pos=m_ListTree.GetHeadPosition(); pos != m_ListTree.GetTailPosition(); ) 
	{
		CTreeCtrl* pTreeCtrl = m_ListTree.GetNext(pos);
		HTREEITEM hmyItem = NULL;

		BOOL bExistance = FALSE;
		// Show all of the children of hmyItem in bold.
		if (pTreeCtrl->ItemHasChildren(hmyItem))
		{
			HTREEITEM hItem = pTreeCtrl->GetChildItem(hmyItem);

			while (hItem != NULL)
			{
				strIndicator = pTreeCtrl->GetItemText(hItem);
				strTemp = strIndicator;
				strTemp.MakeUpper();
				switch(nFindType) 
				{
				case TFI_JIPYOITEM_ID:			// 선택된 데이터를 Select시킴
					if(strTemp.Find(strFindData) != -1)
					{

						m_ctrlOutBar.SetSelFolder(iIndex);
						pTreeCtrl->SelectItem(hItem);
						strResult.Add(strIndicator);
						return;
					}
					break;
				case TFI_JIPYOITEM_TITLE:		// 해당데이터를 찾는다
					if(strTemp.Find(strFindData) != -1)
						strResult.Add(strIndicator);
					break;
				}
				
				hItem = pTreeCtrl->GetNextSiblingItem(hItem);
			}
		}
		iIndex++;
	}
}
// }}

//007 {{ 지표검색 리스트확장
void CTabIndices::OnBnClickedBtnExtend()
{
	if(m_bFindExtend)
	{
		// 리스트 닫기
		m_bFindExtend = FALSE;
		m_listFind.ShowWindow(FALSE);
		m_btnExtend	.FP_vSetImage(m_pIL_W20H20, 48, 50, TRUE, "", FALSE);
		Invalidate();
		RecalcLayout();
		//m_btnExtend.FP_vSetImage();
	}
	else
	{
		// 리스트 열기
		m_bFindExtend = TRUE;
		m_listFind.ShowWindow(TRUE);
		m_btnExtend	.FP_vSetImage(m_pIL_W20H20, 44, 46, TRUE, "", FALSE);
		Invalidate();
		RecalcLayout();		
	}
}
//007 }}

void CTabIndices::OnLbnSelchangeListFind()
{
	CString strData;
	int nSel = m_listFind.GetCurSel();
	m_listFind.GetText(nSel, strData);
	CStringArray strResult;
	GetFindItemList(TFI_JIPYOITEM_ID, strData, strResult);
}

BOOL CTabIndices::PreTranslateMessage(MSG* pMsg)
{
	UINT nKeyCode = pMsg->wParam;

	HWND hwndEditFind;
	hwndEditFind = m_editFind.GetSafeHwnd();
	if(pMsg->hwnd == hwndEditFind)
	{
		if (nKeyCode == VK_RETURN)
		{
			OnBnClickedBtnFind();
			return TRUE;
		}
	}
	return CTabDlg::PreTranslateMessage(pMsg);	
}

void CTabIndices::OnSendMenu(UINT nIndex)
{
	if (nIndex == MENU_ID_ADD)		//# Add
	{
		OnBtsearch();
	}
	else if (nIndex == MENU_ID_DEL)		//# Del
	{
		OnBtapply();
	}
}

//HBRUSH CTabIndices::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO: Change any attributes of the DC here
//	switch(nCtlColor) 
//	{
//	case CTLCOLOR_STATIC:
//		return m_brBkColor;
//	}
//
//	// TODO: Return a different brush if the default is not desired
//	return hbr;
//}

void CTabIndices::ChangeSkinColor(COLORREF clrBkgrnd)
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
	}
	
	m_stTitle.SetStaticColor(m_crBk, m_crBk, CLR_FONT_DEFAULT);
	Invalidate();
}