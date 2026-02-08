// DlgCompSetup.cpp : implementation file
//

#include "stdafx.h"
#include "ChartUtil.h"
#include "DlgCompSetup.h"
#include "../Chart_Common/Grid\ColorTable.h"
#include "../../inc/IMainInfoMng.h"
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_ITEM_CNT		10    //최대 10개만 추가가 가능하다.
								//비교차트의 UsrAddListDlg.h 에 동일하게 정의해서 사용한다.

#define ROW_HEIGHT 15
#define GRID_CALLPUT_WIDTH	33
#define GRID_CENTER_WIDTH	100
#define COL_CENTER 1
#define TITLE_ROW 1
enum	{EXERCISE, FIRST, SECOND, THIRD, FOURTH};	// 콜 옵션 종목 


/////////////////////////////////////////////////////////////////////////////
// CDlgCompSetup dialog


CDlgCompSetup::CDlgCompSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCompSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCompSetup)
	m_nTime = 0;
	m_bCurShow = FALSE;
	m_nPeriodType = 0;
	m_nRComp = -1;
	//}}AFX_DATA_INIT

	m_dwInData=NULL;
	m_dwOutData=NULL;
	m_rgbText = GetSysColor(COLOR_WINDOWTEXT);
	m_pTreeListMng = NULL;

	m_pCurGroup = NULL;
	m_pCurSubItem = NULL;

	m_nListItemSel = -1;
	m_nListCodeSel = -1;

	m_bModifyFlag=FALSE;

	m_nCtrlType = 0;
	m_pGridCtrl = NULL;
}


void CDlgCompSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCompSetup)
	DDX_Control(pDX, IDC_LISTCODES, m_ListCodes);
	DDX_Control(pDX, IDC_LISTITEM, m_ListItem);
	DDX_Control(pDX, IDC_LISTGRP, m_ListRP);
	DDX_Control(pDX, IDC_TREESEARCH, m_treeSearch);
	DDX_Control(pDX, IDC_TEXT_COLOR, m_btnTextColor);
	DDX_Control(pDX, IDC_EDTCODE2, m_edtCode);
	DDX_Control(pDX, IDC_EDTCODE, m_edtCodeName);
	DDX_Control(pDX, IDC_THICKNESS, m_Thickness_Combo);
	DDX_Control(pDX, IDC_DRAWSTYLE, m_DrawStyle_Combo);
	DDX_Control(pDX, IDC_CMBSEL, m_CmbSel_Combo);
	DDX_Text(pDX, IDC_EDTTIME, m_nTime);
	DDV_MinMaxInt(pDX, m_nTime, 0, 3600);
	DDX_Check(pDX, IDC_CHKSHOW, m_bCurShow);
	DDX_Text(pDX, IDC_EDTPERIOD, m_nPeriodType);
	DDV_MinMaxInt(pDX, m_nPeriodType, 1, 300);
	DDX_Radio(pDX, IDC_RCOMP1, m_nRComp);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_TEXT_COLOR, m_rgbText);
}


BEGIN_MESSAGE_MAP(CDlgCompSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgCompSetup)
	ON_BN_CLICKED(IDC_BTNGRPNEW, OnBtngrpnew)
	ON_BN_CLICKED(IDC_BTNGRPDEL, OnBtngrpdel)
	ON_BN_CLICKED(IDC_BTNGRPCHANGE, OnBtngrpchange)
	ON_BN_CLICKED(IDC_ARROW1, OnArrow1)
	ON_BN_CLICKED(IDC_ARROW2, OnArrow2)
	ON_BN_CLICKED(IDC_ARROW3, OnArrow3)
	ON_BN_CLICKED(IDC_ARROW4, OnArrow4)
	ON_BN_CLICKED(IDC_BTNLEFT, OnBtnleft)
	ON_BN_CLICKED(IDC_BTNRIGHT, OnBtnright)
	ON_BN_CLICKED(IDC_BTNLEFT3, OnBtnleft3)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LISTGRP, OnSelchangeListgrp)
	ON_NOTIFY(HDN_ITEMCHANGED, IDC_LISTITEM, OnItemchangedListitem)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREESEARCH, OnSelchangedTreesearch)
	ON_NOTIFY(NM_CLICK, IDC_LISTITEM, OnClkListItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTITEM, OnDblclkListItem)
	ON_NOTIFY(NM_CLICK, IDC_LISTCODES, OnClkListcodes)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCODES, OnDblclkListcodes)
	ON_CBN_SELCHANGE(IDC_DRAWSTYLE, OnSelchangeDrawstyle)
	ON_CBN_SELCHANGE(IDC_THICKNESS, OnSelchangeThickness)
	ON_CBN_SELCHANGE(IDC_CMBSEL, OnSelchangeCmbsel)
	ON_MESSAGE(CPN_SELCHANGE, OnColorSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCompSetup message handlers

BOOL CDlgCompSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pMainInfo)
	{
		m_szRootPath = pMainInfo->GetRootDir();
	}
	// TODO: Add extra initialization here
	m_edtCodeName.SetPromptText(_T("<종목명>"));
	m_edtCode.SetPromptText(_T("<종목코드>"));

	fnInitList();
	fnTreeLoad();

	Initial_ThicknessComboBox( 0, 0, m_Thickness_Combo);	
	Initial_DrawStyleComboBox( 0, m_DrawStyle_Combo);

	InitFromLoadData();

	UpdateData(FALSE);
	fnCheckNoGroup();
	fnInitSelctGroup(m_dwInData->m_szSelectGroup);	// 그룹선택처리

	InitGrid();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#include "./DlgGroupName.h"
void CDlgCompSetup::OnBtngrpnew() 
{
	CDlgGroupName dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_bModifyFlag = TRUE;	//변경정보 셋팅

		dlg.m_szName.TrimLeft();
		dlg.m_szName.TrimRight();

		if(dlg.m_szName.GetLength()>0)
		{
			// 같은 그룹이름이 있는지 확인한다. 있으면 처리안함.			
			{
				int nCnt = m_ListRP.GetCount();
				CString rString;
				for(int i=0; i<nCnt; i++)
				{
					m_ListRP.GetText(i, rString);
					if(rString.Compare(dlg.m_szName)==0)
					{
						AfxMessageBox("이미 같은 그룹명이 존재합니다.");
						return;
					}
				}
			}

			int nNSel = m_ListRP.AddString(dlg.m_szName);
			m_ListRP.SetCurSel(nNSel);
			m_ListItem.DeleteAllItems();

			CDlgCompGroupData* pGroupData = new CDlgCompGroupData;
			pGroupData->m_szGroupName = dlg.m_szName;
			m_GroupList.AddTail(pGroupData);
			m_pCurGroup = pGroupData;
		}
	}
}

void CDlgCompSetup::OnBtngrpdel() 
{
	int nSel = m_ListRP.GetCurSel();
	if(nSel<0) return;

	CString rString;
	m_ListRP.GetText(nSel, rString);

	CString szMsg;
	szMsg.Format("%s 그룹을(를) 삭제하시겠습니까?", rString);
	if(IDYES!=AfxMessageBox(szMsg, MB_YESNO))
		return;

	m_bModifyFlag = TRUE;	//변경정보 셋팅

	m_ListRP.DeleteString(nSel);
	m_ListItem.DeleteAllItems();

	// 해당 그룹정보도 Clear해야함.
	POSITION pos=NULL;
	CDlgCompGroupData* pGroupData = FindGroupInfo(nSel, pos);
	if(pGroupData)
	{
		for(POSITION tPos=pGroupData->m_ItemList.GetHeadPosition(); tPos;)
		{
			delete pGroupData->m_ItemList.GetNext(tPos);
		}
		pGroupData->m_ItemList.RemoveAll();
		m_GroupList.RemoveAt(pos);
	}

	if(m_ListRP.GetCount()==0) return;

	int nIndex = nSel - 1;
	if(nIndex<0) nIndex = 0;
	m_ListRP.SetCurSel(nIndex);

	fnSelectGroup(nIndex);
}

void CDlgCompSetup::OnBtngrpchange() 
{
	int nSel = m_ListRP.GetCurSel();
	if(nSel<0) return;

	CString rString;
	m_ListRP.GetText(nSel, rString);

	CDlgGroupName dlg;
	dlg.m_szName = rString;
	if(dlg.DoModal()==IDOK)
	{
		// 변경이 없으면 아무처리 않함.
		if(rString.Compare(dlg.m_szName)==0) return;

		m_bModifyFlag = TRUE;	//변경정보 셋팅
		
		if(dlg.m_szName.GetLength()>0)
		{
			m_ListRP.DeleteString(nSel);
			m_ListRP.InsertString(nSel, dlg.m_szName);

			// 그룹명 변경
			POSITION pos=NULL;
			CDlgCompGroupData* pGroupData = FindGroupInfo(nSel, pos);
			if(pGroupData)
			{
				pGroupData->m_szGroupName = dlg.m_szName;
			}
		}
	}
}

void CDlgCompSetup::OnArrow1() 
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅
}

void CDlgCompSetup::OnArrow2() 
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅
}

void CDlgCompSetup::OnArrow3() 
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅
}

void CDlgCompSetup::OnArrow4() 
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅
}

// 비교지수설정의 아이템으로 추가
void CDlgCompSetup::OnBtnleft() 
{
	if(m_nAddType==1)
		DoGridAddToItem(m_szGridSelCode, m_szGridSelCodeName);
	else
	{
		//DoAddToItem(m_nListCodeSel);
//-->@.080624-001
		POSITION pos = m_ListCodes.GetFirstSelectedItemPosition();
		if (pos == NULL)
			TRACE0("No items were selected!\n");
		else
		{
			std::vector<int> nSelList;
			while (pos)
			{
				int nItem = m_ListCodes.GetNextSelectedItem(pos);
				nSelList.push_back(nItem);			
			}
			std::vector<int>::iterator it = nSelList.begin();
			while(it != nSelList.end())
			{
				m_nListCodeSel = *it;
				DoAddToItem(m_nListCodeSel);
				it++;
			}			
			nSelList.clear();
		}
//<--
	}
}

// 비교지수설정의 아이템에서 삭제
void CDlgCompSetup::OnBtnright() 
{
//	DoDeleteItem(m_nListItemSel);
//	m_nListItemSel--;
//	DoCurItemChange();

	int nFistSel = -1;
	POSITION pos = m_ListItem.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
//-->@.080624-001
		std::vector<int> nSelList;
		while (pos)
		{
			int nItem = m_ListItem.GetNextSelectedItem(pos);
			nSelList.push_back(nItem);			
		}

		int nCnt = nSelList.size();
		for(int i=nCnt-1; i>=0; i--)
		{
			DoDeleteItem(nSelList.at(i));
		}

		nFistSel = nSelList.at(0);
		m_nListItemSel = nFistSel;
		m_nListItemSel--;
		DoCurItemChange();
		fnSelectItem(m_pCurGroup, m_nListItemSel);

		nSelList.clear();
//<--
	}
}

// 전체삭제
void CDlgCompSetup::OnBtnleft3() 
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅
	
	m_ListItem.DeleteAllItems();

	List_CDlgCompSubItem* pSubItemList = GetCurSubItemList();
	if(pSubItemList)
	{
		POSITION tpos=NULL;
		for(POSITION pos=pSubItemList->GetHeadPosition(); pos;)
		{
			tpos = pos;
			CDlgCompSubItem* pItem = pSubItemList->GetNext(pos);
			delete pItem;

			if(tpos) pSubItemList->RemoveAt(tpos);
		}
	}
}

void CDlgCompSetup::OnOk() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	// 아무 변경이 없으면 그냥 빠져나간다.
	if(	m_dwInData->m_nBase	!=m_nRComp ||
		m_dwInData->m_nTimer != m_nTime ||
		m_dwInData->m_nTypeUnit	!= m_nPeriodType ||
		m_dwInData->m_bCurDisp	!= m_bCurShow)
	m_bModifyFlag = TRUE;

	if(m_bModifyFlag==FALSE && m_pCurGroup)
	{
		if(m_dwInData->m_szSelectGroup != m_pCurGroup->m_szGroupName)
			m_bModifyFlag = TRUE;
	}

	if(m_bModifyFlag==FALSE)
	{
		CDialog::OnCancel();
		return;
	}
	if(m_dwInData)
	{
		m_dwInData->m_nBase		= m_nRComp;
		m_dwInData->m_nTimer	= m_nTime;
		m_dwInData->m_nTypeUnit	= m_nPeriodType;
		m_dwInData->m_bCurDisp	= m_bCurShow;

		if(m_pCurGroup)
			m_dwInData->m_szSelectGroup = m_pCurGroup->m_szGroupName;

		//m_dwInData->m_pFn->SaveData((long)m_dwInData);
		m_dwInData->m_pFn->CopyGroupList((long)&m_GroupList, (long)&m_dwInData->m_GroupList);
		m_dwInData->m_pFn->SaveData((long)m_dwInData);
	}	

	CDialog::OnOK();
}


void CDlgCompSetup::Initial_ThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. BitMap을 Load한다.
	if( !Thickness_Combo.m_bitmap.GetSafeHandle()) Thickness_Combo.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
	AddLineType( &Thickness_Combo, 5 );

	// 2. 값을 setting한다.
	for( int i = -1 ; i < Thickness_Combo.GetCount() ; i++ ) 
		Thickness_Combo.SetItemHeight( i, 14 );

//	Thickness_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphPenThickness( p_nSubGraphIndex, 1) - 1);
	Thickness_Combo.SetCurSel(0);

//	// 3. 선형이고, static 아이디가 들어오지 않았을때는 그냥 나간다.
//	if( idStatic == 0 || m_pIGraphInfo->GetSubGraphType( p_nSubGraphIndex) == CGraphBaseData::Line_Type )
//		return;
//
//	// 4. 인자로 받은 Static과 Thickness_Combo를 숨겨둔다.
//	CStatic *pStatic = ( CStatic * )GetDlgItem( idStatic );
//	pStatic->ShowWindow( SW_HIDE );
//	Thickness_Combo.ShowWindow( SW_HIDE );
}

void CDlgCompSetup::AddLineType( CComboBox *pCombo, int Type )
{
	TCHAR *szType[] = 
	{
		_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
		_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
		_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
		_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
		_T("24")
	};

	ASSERT( pCombo != NULL );
	
	// 1. Combo를 초기화 시킨다.
	pCombo->ResetContent();

	// 2. LineType을 지정한다.
	for( int i = 0 ; i < Type ; i++ )
		pCombo->AddString( szType[ i ] );
}

void CDlgCompSetup::Initial_DrawStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. BitMap을 Load한다.
	LoadBitmapToDrawStyleCombo( p_nSubGraphIndex, Drawsytle_Combo );
	
	// 2. 값을 setting한다.
	for( int i = -1 ; i < Drawsytle_Combo.GetCount() ; i++ ) 
		Drawsytle_Combo.SetItemHeight( i, 14 );

	//Drawsytle_Combo.SetCurSel( m_pIGraphInfo->GetSubGraphDrawStyle( p_nSubGraphIndex));
	Drawsytle_Combo.SetCurSel( 0 );
}

void CDlgCompSetup::LoadBitmapToDrawStyleCombo( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo )
{
	if( p_nSubGraphIndex < 0) return;

	// 1. Graph의 type에 따라 해당되는 bitMap을 load한다.
	RealLoadBitmap( 5, IDB_LINE_STYLE, Drawsytle_Combo );
}

void CDlgCompSetup::RealLoadBitmap(const int nCount, const int nIDBitmap, CBitmapItemComboBox& Drawsytle_Combo)
{
	if( Drawsytle_Combo.m_bitmap.GetSafeHandle()) Drawsytle_Combo.m_bitmap.DeleteObject();
	
	if (!Drawsytle_Combo.m_bitmap.LoadBitmap(nIDBitmap))
		return;   // need this one image
	
	AddLineType(&Drawsytle_Combo, nCount);
}

void CDlgCompSetup::SetInOutData(long dwInData, long dwOutData)
{
	m_dwInData = (STDlgCompData*)dwInData;
	m_dwOutData = dwOutData;

	//if(m_dwInData->m_GroupList.GetCount()>0)
		m_dwInData->m_pFn->CopyGroupList((long)&m_dwInData->m_GroupList, (long)&m_GroupList);
	//m_dwInData->m_pFn->CopyGroupList((long)&m_dwInData->m_GroupList, (long)&m_GroupList);
}

void CDlgCompSetup::InitFromLoadData()
{
	m_nRComp	= m_dwInData->m_nBase;
	m_nTime		= m_dwInData->m_nTimer;
	m_nPeriodType = m_dwInData->m_nTypeUnit;
	m_bCurShow	= m_dwInData->m_bCurDisp;

	for(POSITION pos=m_GroupList.GetHeadPosition(); pos;)
	{
		CDlgCompGroupData* pGroupData = m_GroupList.GetNext(pos);
		m_ListRP.AddString(pGroupData->m_szGroupName);
	}
}

#ifndef LVS_EX_LABELTIP
	#define LVS_EX_LABELTIP 0x00004000
#endif
void CDlgCompSetup::fnInitList()
{
	m_ListItem.InsertColumn(0, "종목명"	, LVCFMT_LEFT	, 140);
	m_ListItem.InsertColumn(1, "코드"	, LVCFMT_CENTER	, 70);
	m_ListItem.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP);

	m_ListCodes.InsertColumn(0, "종목명", LVCFMT_LEFT	, 100);
	m_ListCodes.InsertColumn(1, "코드"	, LVCFMT_CENTER	, 90);
//	m_ListCodes.Init_ListCtrl();
	m_ListCodes.SetRowHeight(15);
	m_ListCodes.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP);
}

void CDlgCompSetup::fnTreeLoad()
{
	m_pTreeListMng = (ITreeListManager2*)AfxGetPctrInterface(UUID_ITreeListManager2);

	CInputCodeListEx inData;
	inData.m_pTreeCtrl = &m_treeSearch;
	inData.m_pListCtrl = NULL;
	inData.m_szSection = "등락률차트";

	if(m_pTreeListMng)
	{
		m_dwTreeKey = m_pTreeListMng->LoadTreeList(&inData, "등락률차트");
		m_pTreeListMng->SetBaseData("SCREENCODE", "4602");
	}
}

void CDlgCompSetup::OnDestroy() 
{
	if(m_pGridCtrl)
	{
		HWND hGrid = m_pGridCtrl->GetSafeHwnd();
		TRACE("Grid Hanle=%08X\n", hGrid);
		if( hGrid&& IsWindow(hGrid))
		{
			//AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
			m_pGridCtrl->DestroyWindow();
		}

		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	}
	
	if(m_dwInData)m_dwInData->m_pFn->ClearGroupList((long)&m_GroupList);
	m_GroupList.RemoveAll();
	if(m_pTreeListMng) m_pTreeListMng->UnLoadTreeList(m_dwTreeKey);

	CDialog::OnDestroy();	
	// TODO: Add your message handler code here
	
}

List_CDlgCompSubItem* CDlgCompSetup::GetCurSubItemList()
{
	if(m_pCurGroup)
		return &m_pCurGroup->m_ItemList;
	return NULL;
}

CDlgCompGroupData* CDlgCompSetup::FindGroupInfo(int nIndex, POSITION &pos)
{
	pos = m_GroupList.FindIndex(nIndex);
	if(pos)
		return m_GroupList.GetAt(pos);

	return NULL;
}


void CDlgCompSetup::fnSelectGroup(int nSel)
{
	m_ListItem.DeleteAllItems();

	POSITION pos=NULL;
	CDlgCompGroupData* pGroupData = FindGroupInfo(nSel, pos);
	if(!pGroupData) return;

	m_pCurGroup = pGroupData;
	int nItem=0;
	//CDlgCompSubItem* pItem0=NULL;
	for(POSITION tPos=pGroupData->m_ItemList.GetHeadPosition(); tPos; nItem++)
	{
		CDlgCompSubItem* pItem = pGroupData->m_ItemList.GetNext(tPos);
		//if(nItem==0)	pItem0 = pItem;

		nItem = m_ListItem.InsertItem(nItem, pItem->m_szCodeName);
		m_ListItem.SetItemText(nItem, 1, pItem->m_szCode);
		//m_ListItem.InsertColumn(nItem)
	}

	//if(pItem0) fnSelectItem(pItem0);
}

void CDlgCompSetup::fnSelectItem(CDlgCompGroupData* pGroupData, int nItemSel)
{
	if(pGroupData==NULL)
	{
		int nGroup = m_ListRP.GetCurSel();
		if(nGroup<0) return;

		POSITION pos = m_GroupList.FindIndex(nGroup);
		pGroupData = m_GroupList.GetAt(pos);
	}
	if(pGroupData==NULL) return;

	POSITION pos = pGroupData->m_ItemList.FindIndex(nItemSel);
	if(!pos) return;

	CDlgCompSubItem* pItem = pGroupData->m_ItemList.GetAt(pos);
	fnSelectItem(pItem);
}

void CDlgCompSetup::fnSelectItem(CDlgCompSubItem* pItem)
{
	if(!pItem) return;

	// 현재 선택된 아이템으로 설정한다.
	m_pCurSubItem = pItem;
	fnUpdateToControl(pItem);
//	m_btnTextColor.SetColor(pItem->m_clr);
//	m_DrawStyle_Combo.SetCurSel(pItem->m_nDrawStyle);
//	m_Thickness_Combo.SetCurSel(pItem->m_nThickness);
//	SetDlgItemText(IDC_TXTTITLE, pItem->m_szCodeName);
}


void CDlgCompSetup::OnSelchangeListgrp() 
{
	// 이 루틴은 불리지 않는 듯함.
	int nSel = m_ListRP.GetCurSel();
	if(nSel<0) return;

	CDlgCompGroupData* pGroupData = m_GroupList.GetAt(m_GroupList.FindIndex(nSel));
	m_pCurGroup = pGroupData;

	fnSelectGroup(nSel);
	fnSelectItem(m_pCurGroup, 0);
}

void CDlgCompSetup::OnItemchangedListitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	int nItem = pNMListView->iItem;
	if(nItem>=0)
	{
		m_pCurSubItem = m_pCurGroup->m_ItemList.GetAt(m_pCurGroup->m_ItemList.FindIndex(nItem));

		fnUpdateToControl(m_pCurSubItem);
	}

	*pResult = 0;
}

//LRESULT CDlgCompSetup::DoUdmTreeClick(WPARAM wParam, LPARAM lParam)
//{
//	CItemLongData* pLData = (CItemLongData*)lParam;
////	m_pQueryData = pLData;
//	if(pLData)
//	{
//		TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
//		
//		ChangeListOrGridCtrl(pLData->m_szSection);
//		long dwReserved=0;
//		if(m_pTreeListMng)
//			m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
//
//	}
//	return TRUE;
//}

void CDlgCompSetup::OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = m_treeSearch.GetSelectedItem();
	if(hItem == NULL)
		return;

	//실제로는 CBaseLongData 타입임
	CItemLongData* pLData = (CItemLongData*)m_treeSearch.GetItemData(hItem);
	if(pLData)
	{
		TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
		ChangeListOrGridCtrl(pLData->m_szSection);

		if(pLData->m_nType==BaseLongData_OneClick)
		{
			long dwReserved=0;
			if(m_pTreeListMng)
				m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
		}
		else if(pLData->m_szMethod.CompareNoCase("CFG")!=0)
		{
			long dwReserved=0;
			if(m_pTreeListMng)
				m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
		}
	}

	*pResult = 0;
}

void CDlgCompSetup::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nListItemSel = pNMListView->iItem;

	fnSelectItem(m_pCurGroup, m_nListItemSel);
	*pResult = 0;
}

void CDlgCompSetup::OnDblclkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_nListItemSel = pNMListView->iItem;
	DoDeleteItem(m_nListItemSel);
	m_nListItemSel--;
	DoCurItemChange();

	*pResult = 0;
}

void CDlgCompSetup::OnClkListcodes(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nListCodeSel = pNMListView->iItem;

	*pResult = 0;
}

void CDlgCompSetup::OnDblclkListcodes(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_nListCodeSel = pNMListView->iItem;
	DoAddToItem(m_nListCodeSel);

	*pResult = 0;
}

void CDlgCompSetup::CTLMCallback::ReceiveData(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData)
{
	//TRACE("CDlgCompSetup::CTLMCallback::ReceiveData\n");

	if(aDataType!='A') return; // 현재는 A만처리.
	METHOD_PROLOGUE(CDlgCompSetup, TLMCallback);

	if(	pThis->m_nCtrlType == 1)		// Kospi200옵션
	{
		LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
		if(pListTypeA)
		{
			pThis->SetMasterData(pListTypeA);
		}
	}
	else if( pThis->m_nCtrlType == 2)	// 주식옵션
	{
		LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
		if(pListTypeA)
		{
			pThis->SetMasterData(pListTypeA);
		}
	}
	else								// 그외
	{
		LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
		if(pListTypeA)
		{
			//트리밑의 종목리스트를 모두 삭제한다.
			pThis->m_ListCodes.DeleteAllItems();
			pThis->m_ListCodes.fnUsingMemoryClear();
			
			long	lUpLimit = 0;
			long	lUp = 0;
			long	lSteady = 0;
			long	lDown = 0;
			long	lDownLimit = 0;
			long	lIndexCnt = 0;
			double dDaebiRateTotal = 0;
			
			for(POSITION pos=pListTypeA->GetHeadPosition(); pos; )
			{
				CCodeTypeA TypeA = pListTypeA->GetNext(pos);
				
				MASTERCODE* pData = new MASTERCODE;
				pData->strCode = TypeA.m_szcode;
				pData->strKor = TypeA.m_szhname;
				pData->strEng = _T("");	
				pThis->m_ListCodes.m_AllArray.Add(pData);
			}
			pThis->m_ListCodes.SetItemCount(pListTypeA->GetCount());
		}
	}
}

void CDlgCompSetup::DoAddToItem(int nItem)
{
	if(nItem<0) return;
	if(!m_pCurGroup)
	{
		AfxMessageBox("선택된 그룹이 없음.");
		return;
	}

	List_CDlgCompSubItem* pCurSubItem = GetCurSubItemList();
	if(!pCurSubItem)
	{
		AfxMessageBox("그룹이 선택되지 않음");
		return;
	}

	CString szCode, szCodeName;
	szCodeName = m_ListCodes.GetItemText(nItem, 0);
	szCode = m_ListCodes.GetItemText(nItem, 1);

	// 추가 가능한지 확인한다.
	int nRet = fnCheckCode(szCode, szCodeName);
	if(nRet==-1)
	{
		AfxMessageBox("이미 추가되어 있는 종목입니다.");
		return;
	}
	else if(nRet==-2)
	{
		CString szMsg;
		szMsg.Format("최대 %d개 종목까지 가능합니다.", MAX_ITEM_CNT);
		AfxMessageBox(szMsg);
		return;
	}

	m_bModifyFlag = TRUE;	//변경정보 셋팅


	int nI = m_ListItem.GetItemCount();
	nI = m_ListItem.InsertItem(nI, szCodeName);
	nI = m_ListItem.SetItemText(nI, 1, szCode);

	CDlgCompSubItem* pNewItem = new CDlgCompSubItem;
	INIT_CDlgCompSubItemP(pNewItem);
	pNewItem->m_szCode = szCode;
	pNewItem->m_szCodeName = szCodeName;

	pCurSubItem->AddTail(pNewItem);
	m_pCurSubItem = pNewItem;
}

#include "../../inc/IMasterDataMng.h"
void CDlgCompSetup::DoGridAddToItem(LPCSTR szCode, LPCSTR _szCodeName)
{
	if(!m_pCurGroup)
	{
		AfxMessageBox("선택된 그룹이 없음.");
		return;
	}

	List_CDlgCompSubItem* pCurSubItem = GetCurSubItemList();
	if(!pCurSubItem)
	{
		AfxMessageBox("그룹이 선택되지 않음");
		return;
	}
	
	// 추가 가능한지 확인한다.
	int nRet = fnCheckCode(szCode, _szCodeName);
	if(nRet==-1)
	{
		AfxMessageBox("이미 추가되어 있는 종목입니다.");
		return;
	}
	else if(nRet==-2)
	{
		CString szMsg;
		szMsg.Format("최대 %d개 종목까지 가능합니다.", MAX_ITEM_CNT);
		AfxMessageBox(szMsg);
		return;
	}

	m_bModifyFlag = TRUE;	//변경정보 셋팅

	IMasterDataManagerLast* pMasterMng = ( IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	CString szCodeName = _szCodeName;
	if(pMasterMng)
		szCodeName = pMasterMng->GetCodeName(szCode);

	int nI = m_ListItem.GetItemCount();
	nI = m_ListItem.InsertItem(nI, szCodeName);
	nI = m_ListItem.SetItemText(nI, 1, szCode);

	CDlgCompSubItem* pNewItem = new CDlgCompSubItem;
	INIT_CDlgCompSubItemP(pNewItem);
	pNewItem->m_szCode = szCode;
	pNewItem->m_szCodeName = szCodeName;

	pCurSubItem->AddTail(pNewItem);
	m_pCurSubItem = pNewItem;
}


void CDlgCompSetup::DoDeleteItem(int nItem)
{
	m_bModifyFlag = TRUE;	//변경정보 셋팅

	m_ListItem.DeleteItem(nItem);

	List_CDlgCompSubItem* pSubItemList = GetCurSubItemList();
	if(pSubItemList)
	{
		POSITION pos = pSubItemList->FindIndex(nItem);
		CDlgCompSubItem* pItem = pSubItemList->GetAt(pos);
		delete pItem;
		pSubItemList->RemoveAt(pos);
	}
}

void CDlgCompSetup::DoCurItemChange()
{
	int nCnt = m_ListItem.GetItemCount();
	if(nCnt==0)
	{
		m_nListItemSel = -1;
		return;
	}

	// 08.05.31 작업 준비만 해놓음.
	if(m_nListItemSel>=0)
	{
//		m_pCurSubItem = ;
	}
}

void CDlgCompSetup::fnUpdateToControl(CDlgCompSubItem* pCurSubItem)
{
	SetDlgItemText(IDC_TXTTITLE, pCurSubItem->m_szCodeName);
	m_btnTextColor.SetColor(pCurSubItem->m_clr);
	m_DrawStyle_Combo.SetCurSel(pCurSubItem->m_nDrawStyle);
	m_Thickness_Combo.SetCurSel(pCurSubItem->m_nThickness);
}

void CDlgCompSetup::fnUpdateToMemory(CDlgCompSubItem* pCurSubItem)
{
	if(!pCurSubItem)
	{
		AfxMessageBox("현재 아이템 처리가 되어있지 않음");
		return;
	}
	m_bModifyFlag = TRUE;	//변경정보 셋팅
	
	//pCurSubItem->m_szCodeName
	pCurSubItem->m_clr = m_btnTextColor.GetColor();
	pCurSubItem->m_nDrawStyle = m_DrawStyle_Combo.GetCurSel();
	pCurSubItem->m_nThickness = m_Thickness_Combo.GetCurSel();
}

LONG CDlgCompSetup::OnColorSelChange(WPARAM wParam, LPARAM)
{
	fnUpdateToMemory(m_pCurSubItem);
	return TRUE;
}


void CDlgCompSetup::OnSelchangeDrawstyle() 
{
	fnUpdateToMemory(m_pCurSubItem);
}

void CDlgCompSetup::OnSelchangeThickness() 
{
	fnUpdateToMemory(m_pCurSubItem);
}

void CDlgCompSetup::OnSelchangeCmbsel()
{
	int nSel = m_CmbSel_Combo.GetCurSel();
	CString rString;
	m_CmbSel_Combo.GetLBText(nSel, rString);

	DoFindEdtName(rString);
}

// 아무 그룹도 없으면 기본그룹을 생성한다.
void CDlgCompSetup::fnCheckNoGroup()
{
	if(m_GroupList.GetCount()>0) return;

	CString szGroup("기본그룹");

	int nNSel = m_ListRP.AddString(szGroup);
	m_ListRP.SetCurSel(nNSel);

	CDlgCompGroupData* pGroupData = new CDlgCompGroupData;
	pGroupData->m_szGroupName = szGroup;
	m_GroupList.AddTail(pGroupData);
	m_pCurGroup = pGroupData;
}

// 주어진 그룹이름을 select한다. OnInit~에서 호출.
void CDlgCompSetup::fnInitSelctGroup( LPCSTR szGroupName)
{
	int nCnt = m_ListRP.GetCount();
	CString rString;
	for(int i=0; i<nCnt; i++)
	{
		m_ListRP.GetText(i, rString);
		if(rString.Compare(szGroupName)==0)
		{
			m_ListRP.SetCurSel(i);
			fnSelectGroup(i);
			fnSelectItem(m_pCurGroup, 0);
			return;
		}
	}
}

// 1. 리스트에 코드가 존재하는지 확인한다.
// 2. 최대 허용갯수 확인
// Return값 0이상: 정상, -1:이미존재, -2:허용횟수초과. 

int CDlgCompSetup::fnCheckCode(LPCSTR _szCode, LPCSTR _szCodeName)
{
	int nCnt = m_ListItem.GetItemCount();
	if(nCnt==MAX_ITEM_CNT) return -2;

	CString szCode;
	for(int i=0; i<nCnt; i++)
	{
		szCode = m_ListItem.GetItemText(i, 1);
		if(szCode.Compare(_szCode)==0) return -1;
	}
	return nCnt;
}

void CDlgCompSetup::InitGrid()
{
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_pGridCtrl = new CExGridCtrl;
		if (!m_pGridCtrl) return;

		m_pGridCtrl->Create(CRect(0,0,0,0), this, ID_GRIDCTRL);
		HWND hGrid = m_pGridCtrl->GetSafeHwnd();
		TRACE("Grid Hanle=%08X\n", hGrid);

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

		//위치조정
		{
			CRect rect;
			m_ListCodes.GetWindowRect(rect);
			ScreenToClient(rect);
			m_pGridCtrl->MoveWindow(rect);
		}

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


void CDlgCompSetup::InitGridRowCol(int nRowCount/*=0*/)
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

void CDlgCompSetup::ChangeGridRowCol()
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

void CDlgCompSetup::ChangeListOrGridCtrl(CString szSection)
{
	if(szSection == "PMASTER")			// Kospi200옵션
	{
		m_nCtrlType = 1;
		m_ListCodes.ShowWindow(SW_HIDE);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_SHOW);
	}
	else if(szSection == "JPMASTER")	// 주식옵션
	{
		m_nCtrlType = 2;
		m_ListCodes.ShowWindow(SW_HIDE);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_SHOW);
	}
	else								// 그외
	{
		m_nCtrlType = 0;
		m_ListCodes.ShowWindow(SW_SHOW);
		if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
			m_pGridCtrl->ShowWindow(SW_HIDE);
	}
}

void CDlgCompSetup::InsertGridData()
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

CString CDlgCompSetup::GetInsertGridDataText(int row, int col)
{
	if (row != 0 )		return "";

	if (col == COL_CENTER)	return "행사가";
	else if (col < COL_CENTER)	return "콜";
	else if (col > COL_CENTER)	return "풋";

	return "";
}

BOOL CDlgCompSetup::GetCodeInfo(int nType, int& row, int& col, CString& strCode)
{
	int nRowTotal = m_pGridCtrl->GetRowCount();
	if (row < TITLE_ROW || row > nRowTotal || col == COL_CENTER)
		return FALSE;

	strCode = (LPCSTR)m_pGridCtrl->GetItemData(row, col);
	return TRUE;
}

void CDlgCompSetup::SetMasterData(LIST_CCodeTypeA* pCodeType)
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
	strPath.Format("%s\\%s\\%s",  m_szRootPath, "mst", "master\\atm.dat");
	char szAtm[100]; memset(szAtm,0x00,sizeof(szAtm));
	::GetPrivateProfileString("ATM", "PRICE", "", szAtm, 100, strPath);
	strATM = (LPCSTR)(LPSTR)szAtm;

	int nRow = 1;
	CString strCenter;
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


LRESULT CDlgCompSetup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
		case WM_NOTIFY:
			{
				NM_GRIDVIEW *pNmgv = (NM_GRIDVIEW *)lParam;
				int nCode = pNmgv->hdr.code;
				
				if(nCode == GVN_SELCHANGING)			// Left button click
				{
					if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
					{
						CString str;
						str = m_pGridCtrl->GetItemText(pNmgv->iClickRow, pNmgv->iClickCol);

						// 거래대상종목이 아닌경우 return
						if (str.Find("X") >= 0)						
							return TRUE;

						CString strCodeSelected;
						GetCodeInfo(1, pNmgv->iClickRow, pNmgv->iClickCol, strCodeSelected);
						if(!strCodeSelected.IsEmpty())
						{
							m_szGridSelCode = strCodeSelected;
						}
						return TRUE;
					}
				}
				else if(nCode == NM_DBLCLK)			// Left button click
				{
					if(pNmgv->hdr.hwndFrom == m_pGridCtrl->m_hWnd) // 옵션 잔고 그리드에서 마우스 클릭시 종목코드 처리
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
							m_szGridSelCode = strCodeSelected;
							DoGridAddToItem(m_szGridSelCode, m_szGridSelCodeName);
						}
						return TRUE;
					}
				}
			}
			break;
		default:
			break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgCompSetup::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		{
			if(pMsg->hwnd)
			{
				//switch (pMsg->wParam)			case VK_RETURN:
				CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
				if(pWnd)
				{
					HWND hFocus = pWnd->GetSafeHwnd();
					//콤보의 Edit가 선택되었을 경우대비.
					if(hFocus==m_edtCode.GetSafeHwnd())
					{
						DoFindEdtCode();
						if(pMsg->wParam==VK_RETURN)
						{
							OnBtnleft();
							return 1L;
						}
						else
						{
							CDialog::PreTranslateMessage(pMsg);
							return 1L;
						}
					}
					else if(hFocus==m_edtCodeName.GetSafeHwnd())
					{
						DoFindEdtName();
						if(pMsg->wParam==VK_RETURN)
						{
							OnBtnleft();
							return 1L;
						}
						else
						{
							CDialog::PreTranslateMessage(pMsg);
							return 1L;
						}
					}
				}
			}
		}
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgCompSetup::DoFindEdtCode()
{
	CString szIn;
	m_edtCode.GetWindowText(szIn);

	if(szIn.GetLength()==0) return;

	// TODO: Add your control notification handler code here
	int nCnt = m_ListCodes.GetItemCount();
	CString rString;
	for(int i=0; i<nCnt; i++)
	{
		rString = m_ListCodes.GetItemText(i, 1);
		if(rString.Find(szIn)==0)
		{
			m_ListCodes.EnsureVisible(i, FALSE);
			UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
			DoSelectionClear();
			m_ListCodes.SetItemState(i, flag, flag);
			break;
		}
	}
}


void CDlgCompSetup::DoFindEdtName(LPCSTR szInput)
{
	CString szIn = szInput;
	if(szIn.GetLength()==0)
		m_edtCodeName.GetWindowText(szIn);

	if(szIn.GetLength()==0) return;

	// TODO: Add your control notification handler code here
//	LVFINDINFO info;
//	info.flags = LVFI_PARTIAL|LVFI_STRING;
//	info.psz = szIn;
//	int nIndex = m_ListCodes.FindItem(&info, -1);
//	if(nIndex>=0)
//	{
//		m_ListCodes.EnsureVisible(nIndex, FALSE);
//	}
	szIn.MakeUpper();

	int nCnt = m_ListCodes.GetItemCount();
	CString rString;
	for(int i=0; i<nCnt; i++)
	{
		rString = m_ListCodes.GetItemText(i, 0);
		if(rString.Find(szIn)==0)
		{
			m_ListCodes.EnsureVisible(i, FALSE);
			UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
			DoSelectionClear();
			m_ListCodes.SetItemState(i, flag, flag);
			break;
		}
	}	
}

void CDlgCompSetup::DoSelectionClear()
{
	if(m_ListCodes.GetSelectedCount()==0) return;

	POSITION pos = m_ListCodes.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			int nItem = m_ListCodes.GetNextSelectedItem(pos);
			UINT flag = LVIS_SELECTED;
			m_ListCodes.SetItemState(nItem, 0, flag);
		}
	}
}