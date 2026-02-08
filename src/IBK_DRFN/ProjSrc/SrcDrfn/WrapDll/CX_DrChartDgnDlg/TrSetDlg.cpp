// TrSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wDrChartDgnDlg.h"
#include "TrSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrSetDlg dialog

#define		ID_SUBMENU_START		24580
#define		ID_SUBMENU_COUNT		10

#define		MARGIN_CONTROL			8

CTrSetDlg::CTrSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrSetDlg)
	m_bDirectQry = FALSE;
	//}}AFX_DATA_INIT
	m_pSubMenu = NULL;
	m_pSubMenu2 = NULL;
	m_pSelGrid = NULL;

	m_strFile = _T("");

	m_nSelIndex = -1;
	m_pSelTrPack = NULL;

	m_bDirectSend = FALSE;
}

CTrSetDlg::~CTrSetDlg()
{
	m_arrQuery.RemoveAll();
}

void CTrSetDlg::CopyArrQry(CArrTrQuery* pArr)
{
	CTrQuery qry;
	int nSize = pArr->GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = pArr->GetAt(i);
		m_arrQuery.Add(qry);
	}
}

void CTrSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrSetDlg)
	DDX_Control(pDX, IDC_EDIT_TRREALSYMBOL, m_editRealSymbol);
	DDX_Control(pDX, IDC_GROUP_TRQUERY, m_grpTRQuery);
	DDX_Control(pDX, IDC_LIST_TRQUERY, m_listQuery);
	DDX_Control(pDX, IDC_CHECK_DIRECTQUERY, m_chkDirectQry);
	DDX_Control(pDX, IDC_EDIT_TRCODE, m_editTRCode);
	DDX_Check(pDX, IDC_CHECK_DIRECTQUERY, m_bDirectQry);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrSetDlg, CDialog)
	//{{AFX_MSG_MAP(CTrSetDlg)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_DIRECTQUERY, OnCheckDirectquery)
	ON_BN_CLICKED(IDC_BTN_ADDTRQUERY, OnBtnAddtrquery)
	ON_BN_CLICKED(IDC_BTN_DELTRQUERY, OnBtnDeltrquery)
	ON_BN_CLICKED(IDC_BTN_RENAME, OnBtnRename)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRQUERY, OnItemchangedListTrquery)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SUBMENU_START, ID_SUBMENU_START+ID_SUBMENU_COUNT, OnSubMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrSetDlg message handlers

BOOL CTrSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listQuery.InsertColumn(0, _T(""), LVCFMT_LEFT, 120);
//--------------------------------------------------------------

	CTrQuery qry;
	int nSize = m_arrQuery.GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = m_arrQuery.GetAt(i);
		m_listQuery.InsertItem(i, qry.m_strID);
	}

	if (nSize > 0)
		m_listQuery.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);

	ReSizeCtrl();

	EnableControl();

	return TRUE;
}

void CTrSetDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pSubMenu != NULL)
	{
		delete m_pSubMenu;
		m_pSubMenu = NULL;
	}

	if (m_pSubMenu2 != NULL)
	{
		delete m_pSubMenu2;
		m_pSubMenu2 = NULL;
	}
}

int CTrSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_SINGLESEL|LVS_SHAREIMAGELISTS|LVS_OWNERDRAWFIXED|LVS_SHOWSELALWAYS;
	if (m_GridOut.Create(dwStyle, CRect(0,0,0,0), this, IDC_GRID_FORMAT_OUT) == -1)
		return -1;

	if (m_GridIn.Create(dwStyle, CRect(0,0,0,0), this, IDC_GRID_FORMAT_IN) == -1)
		return -1;

	CFont* pFont = GetFont();
	m_GridOut.SetFont(pFont);
	m_GridIn.SetFont(pFont);

	m_GridOut.Initialize();
	m_GridIn.Initialize();

	m_GridIn.ChangeRootName(_T("Input TR Data"), RGB(255, 255, 255));
	m_GridOut.ChangeRootName(_T("Output TR Data"), RGB(255, 255, 255));


	if (!m_pSubMenu)
	{
		m_pSubMenu = new CMenuXP;
		m_pSubMenu->CreatePopupMenu();
		m_pSubMenu->SetBackColor(RGB(244, 246, 249));
		m_pSubMenu->SetSelectedBarColor(RGB(107, 157, 233));
		m_pSubMenu->SetIconAreaColor(RGB(165, 201, 250));
		m_pSubMenu->SetMenuStyle(CMenuXP::STYLE_XP);
	}

	if (!m_pSubMenu2)
	{
		m_pSubMenu2 = new CMenuXP;
		m_pSubMenu2->CreatePopupMenu();
		m_pSubMenu2->SetBackColor(RGB(244, 246, 249));
		m_pSubMenu2->SetSelectedBarColor(RGB(107, 157, 233));
		m_pSubMenu2->SetIconAreaColor(RGB(165, 201, 250));
		m_pSubMenu2->SetMenuStyle(CMenuXP::STYLE_XP);
	}

	LPTSTR lpszMenu[] = { _T("폴더생성"), _T("아이템생성"), _T("폴더추가"), _T("아이템추가"), _T("삭제"), NULL };

	int i, nIndex=ID_SUBMENU_START;
	for (i=0; lpszMenu[i]!=NULL; i++)
	{
		if (m_pSubMenu)
			m_pSubMenu->AppendODMenu(0, new CMenuXPText(nIndex, lpszMenu[i]));

		if ((i > 1) && (m_pSubMenu2))
			m_pSubMenu2->AppendODMenu(0, new CMenuXPText(nIndex, lpszMenu[i]));

		nIndex++;
	}

	return 0;
}

LRESULT CTrSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

void CTrSetDlg::QueryDataToGrid(CTrPack* pTrPack, CQueryGrid* pGrid)
{
	if ((pTrPack == NULL) || (pGrid == NULL))
		return;

	m_pSelTrPack = pTrPack;
	m_pSelGrid = pGrid;

	m_pSelGrid->DeleteAll();
	m_pSelGrid->SetInitData();
	m_pSelGrid->LockWindowUpdate();
	if (m_pSelTrPack->m_bInput)
		m_pSelGrid->ChangeRootName(_T("Input TR Data"), RGB(255, 255, 255));
	else
		m_pSelGrid->ChangeRootName(_T("Output TR Data"), RGB(255, 255, 255));

	CTrBlock *pBlock = NULL;
	int nSize = m_pSelTrPack->m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_pSelTrPack->m_arrBlock.GetAt(i);
		if (pBlock)
			LoadQueryData(pBlock, m_pSelGrid->GetRootItem(0));
	}

	m_pSelGrid->UnlockWindowUpdate();
}

void CTrSetDlg::LoadQueryData(CTrBlock *pPdataBlock, CSuperGridCtrl::CTreeItem *pPItem)
{
	CString strTemp;
	CSuperGridCtrl::CTreeItem *pNewBlock = NULL, *pNewItem = NULL;
	CItemInfo *pInfo = NULL;

	pNewBlock = m_pSelGrid->InsertNewNode(pPItem);
	pInfo = m_pSelGrid->GetData(pNewBlock);
	pInfo->SetItemText(pPdataBlock->m_strBlockName);
	strTemp = CTypeData::GetTypeName(DATA_BOOL, pPdataBlock->IsArray());
	pInfo->SetSubItemText(COLIDX_ARRAY-1, strTemp);
	pInfo->SetSubItemText(COLIDX_CNTNAME-1, pPdataBlock->m_strCntItemID);

	m_pSelGrid->UpdateData(pNewBlock, pInfo, TRUE);

	if (pPdataBlock)
	{
		CTrItem *pDataItem = NULL;
		CString strTemp;
		int nIndex = 0;
		int nSize = pPdataBlock->m_TrItemArr.GetSize();
		for (int i=0; i<nSize; i++)
		{
			pDataItem = pPdataBlock->m_TrItemArr.GetAt(i);

			pNewItem = m_pSelGrid->InsertNewItem(pNewBlock);
			pInfo = m_pSelGrid->GetData(pNewItem);
			pInfo->SetItemText(pDataItem->GetItemID());

			strTemp = CTypeData::GetTypeName(DATA_TYPE, pDataItem->GetDataType());
			pInfo->SetSubItemText(COLIDX_TYPE-1, strTemp);
			strTemp.Format(_T("%d"), pDataItem->GetSize());
			pInfo->SetSubItemText(COLIDX_SIZE-1, strTemp);
			strTemp = CTypeData::GetTypeName(DATA_BOOL, pDataItem->GetABS());
			pInfo->SetSubItemText(COLIDX_BYTEORD-1, strTemp);

			m_pSelGrid->UpdateData(pNewItem, pInfo, TRUE);
		}
	}
}

BOOL CTrSetDlg::GridToQueryData(CTrPack* pTrPack, CQueryGrid* pGrid)
{
	if ((pTrPack == NULL) || (pGrid == NULL))
		return FALSE;

	m_pSelTrPack = pTrPack;
	m_pSelGrid = pGrid;

	m_pSelTrPack->RemoveAll();

	int nRet = MakeQueryData(NULL, m_pSelGrid->GetRootItem(0));
	if (nRet > 0)
		return FALSE;

	return TRUE;
}

int CTrSetDlg::MakeQueryData(CTrBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem)
{
	int nRet = 0;
	int nValue = 0;
	CString strTemp;
	CTrItem *pNewItem = NULL;
	CTrBlock *pBlock = NULL;
	CSuperGridCtrl::CTreeItem *pChildItem = NULL;
	CItemInfo* pItemInfo = NULL;
	POSITION pos = m_pSelGrid->GetHeadPosition(pPItem);
	while (pos)
	{
		pChildItem = m_pSelGrid->GetNextChild(pPItem, pos);
		if (pChildItem == NULL)
			continue;

		pItemInfo = m_pSelGrid->GetData(pChildItem);
		if (m_pSelGrid->ItemHasChildren(pChildItem))	// folder면
		{
			pBlock = new CTrBlock;
			pBlock->m_strBlockName = pItemInfo->GetItemText();

			strTemp = pItemInfo->GetSubItem(COLIDX_ARRAY-1);
			nValue = CTypeData::GetTypeID(DATA_BOOL, strTemp);
			pBlock->m_bArray = (BOOL)nValue;

			pBlock->m_strCntItemID = pItemInfo->GetSubItem(COLIDX_CNTNAME-1);

			m_pSelTrPack->AddBlock(pBlock);

			nRet = MakeQueryData(pBlock, pChildItem);
			if (nRet > 0)
				return nRet;
		}
		else
		{
			if (pPBlock && pPBlock->m_bArray)
				pNewItem = new CTrItemArr;
			else
				pNewItem = new CTrItem;

			strTemp = pItemInfo->GetSubItem(COLIDX_TYPE-1);
			nValue = CTypeData::GetTypeID(DATA_TYPE, strTemp);

			pNewItem->SetItemID(pItemInfo->GetItemText());
			pNewItem->SetDataType(nValue);

			strTemp = pItemInfo->GetSubItem(COLIDX_SIZE-1);
			pNewItem->SetSize(atoi(strTemp));

			if (pBlock)
				pNewItem->SetIsArray(pBlock->m_bArray);

			strTemp = pItemInfo->GetSubItem(COLIDX_BYTEORD-1);
			nValue = CTypeData::GetTypeID(DATA_BOOL, strTemp);
			pNewItem->SetABS((BOOL)nValue);

			nRet = m_pSelTrPack->AddItem(pPBlock->m_strBlockName, pNewItem);
			if (nRet > 0)
				return nRet;
		}
	}

	return 0;
}

void CTrSetDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	BOOL bPopupMenu = FALSE;
	CRect rect, rectIn;

	m_GridOut.GetWindowRect(rect);
	m_GridIn.GetWindowRect(rectIn);

	if (rect.PtInRect(point))
	{
		bPopupMenu = TRUE;
		m_pSelGrid = &m_GridOut;
	}
	else if (rectIn.PtInRect(point))
	{
		bPopupMenu = TRUE;
		m_pSelGrid = &m_GridIn;
	}

	if (bPopupMenu)
	{
		CSuperGridCtrl::CTreeItem *pParentItem = m_pSelGrid->GetSelectedTreeItem();
		if (pParentItem == NULL)
			return;

		if (!m_pSelGrid->ItemHasChildren(pParentItem))
		{
			if (m_pSubMenu2)
				m_pSubMenu2->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_VERTICAL, point.x, point.y, this);
		}
		else
		{
			if (m_pSubMenu)
				m_pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_VERTICAL, point.x, point.y, this);
		}
	}
}

void CTrSetDlg::OnSubMenu(UINT nID)
{
	int nMenuNum = nID - ID_SUBMENU_START;
	switch (nMenuNum)
	{
	case 0:	// 폴더
		OnNodeAdd(1);
		break;
	case 1:	// 아이템
		OnItemAdd(1);
		break;
	case 2:	// 폴더
		OnNodeAdd(2);
		break;
	case 3:	// 아이템
		OnItemAdd(2);
		break;
	case 4:	// 삭제
		OnMenuDelete();
		break;
	}
}

void CTrSetDlg::OnNodeAdd(int nType)
{
	if (!m_pSelGrid)
		return;

	CSuperGridCtrl::CTreeItem *pParentItem = NULL, *pRoot = NULL;
	int nIndex = m_pSelGrid->GetSelectedItem();
	if (nIndex == -1)
		return;

	BOOL bAdd = FALSE;

	CSuperGridCtrl::CTreeItem *pItem = (CSuperGridCtrl::CTreeItem *)m_pSelGrid->GetItemData(nIndex);
	pParentItem = m_pSelGrid->GetParentItem(pItem);

	pRoot = m_pSelGrid->GetRootItem(0);
	if ((nType == 1) && !pParentItem)
	{
		bAdd = TRUE;
		pParentItem = pRoot;
	}
	else
	{
		if ((nType == 2) && (pParentItem == pRoot))
		{
			bAdd = TRUE;
			pParentItem = pItem;
		}
	}

	if (!bAdd)
		return;

	if (pParentItem)
	{
		m_pSelGrid->InsertNewNode(pParentItem, (nType == 1));
		m_pSelGrid->SetRedraw(TRUE);
		m_pSelGrid->InvalidateRect(NULL);
		m_pSelGrid->UpdateWindow();
	}
}

// item 추가
void CTrSetDlg::OnItemAdd(int nType)
{
	if (!m_pSelGrid)
		return;

	CSuperGridCtrl::CTreeItem *pParentItem = NULL, *pRoot = NULL;
	pParentItem = (nType == 1)?m_pSelGrid->GetSelectedNodeItem():m_pSelGrid->GetSelectedTreeItem();

	pRoot = m_pSelGrid->GetRootItem(0);
	if (nType == 2)
	{
		if ((pParentItem == pRoot) || (m_pSelGrid->GetParentItem(pParentItem) == pRoot))
			return;
	}

	if (pParentItem) {
		m_pSelGrid->InsertNewItem(pParentItem, (nType == 1));
		
		m_pSelGrid->SetRedraw(TRUE);
		m_pSelGrid->InvalidateRect(NULL);
		m_pSelGrid->UpdateWindow();
	}
}

// 선택 항목(folder or item) 삭제
void CTrSetDlg::OnMenuDelete()
{
	if (!m_pSelGrid)
		return;

	m_pSelGrid->SendMessage(WM_KEYDOWN, (WPARAM)VK_DELETE, 0);
}


void CTrSetDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void CTrSetDlg::ReSizeCtrl()
{
	CRect rect, rcGrid;
/*
	CWnd* pWnd = GetDlgItem(IDC_BTN_RENAME);
	if (pWnd)
	{
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
	}
*/

	m_listQuery.GetWindowRect(rect);
	ScreenToClient(rect);

	int nTop = rect.top;

	m_grpTRQuery.GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint point;
	point.x = rect.left + MARGIN_CONTROL;

	int nWidth = rect.Width() - MARGIN_CONTROL*2;
	int nHeight = rect.bottom - nTop - MARGIN_CONTROL*2;

	point.y = nTop;
	if (m_bDirectSend)
	{
		if (m_GridIn.GetSafeHwnd())
			m_GridIn.SetWindowPos(0, point.x, point.y, nWidth, nHeight/3, SWP_SHOWWINDOW);

		point.y = nTop + nHeight/3 + MARGIN_CONTROL;
		if (m_GridOut.GetSafeHwnd())
			m_GridOut.SetWindowPos(0, point.x, point.y, nWidth, nHeight*2/3, SWP_SHOWWINDOW);
	}
	else
	{
		if (m_GridOut.GetSafeHwnd())
			m_GridOut.SetWindowPos(0, point.x, point.y, nWidth, nHeight + MARGIN_CONTROL, SWP_SHOWWINDOW);
	}
}

void CTrSetDlg::OnCheckDirectquery()
{
	if (m_bDirectQry)
	{
		m_GridIn.EnableWindow(FALSE); 
	}
	else
	{
		m_GridIn.EnableWindow(TRUE);
	}
}

void CTrSetDlg::OnBtnAddtrquery() 
{
	CAddQueryDlg	dlg;
	dlg.SetQueryName(_T("QryName"));
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetQueryName();

		int nIndex = m_listQuery.GetItemCount();
		m_listQuery.InsertItem(nIndex, strName);

		CTrQuery	qry;
		qry.m_strID = strName;
		m_arrQuery.Add(qry);

		m_listQuery.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CTrSetDlg::OnBtnDeltrquery() 
{
	POSITION pos = m_listQuery.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_listQuery.GetNextSelectedItem(pos);
	char szName[1024];
	memset(szName, 0x00, sizeof(szName));
	m_listQuery.GetItemText(nIndex, 0, szName, sizeof(szName));

	m_listQuery.DeleteItem(nIndex);

	CString strName;
	strName.Format(_T("%s"), szName);

	nIndex = GetQueryAt(strName);
	if (nIndex >= 0)
		m_arrQuery.RemoveAt(nIndex);
}

void CTrSetDlg::OnBtnRename()
{
	int nSelIndex = -1;
	POSITION pos = m_listQuery.GetFirstSelectedItemPosition();
	if (pos)
		nSelIndex = m_listQuery.GetNextSelectedItem(pos);

	if (nSelIndex < 0)
		return;

	CString strName;
	char szName[1024];
	memset(szName, 0x00, sizeof(szName));
	m_listQuery.GetItemText(nSelIndex, 0, szName, sizeof(szName));
	strName.Format(_T("%s"), szName); 

	CAddQueryDlg	dlg;
	dlg.m_bRename = TRUE;
	dlg.SetQueryName(strName);
	if (dlg.DoModal() == IDOK)
	{
		int nQueryIndex = GetQueryAt(strName);
		if (nQueryIndex < 0)
			return;

		strName = dlg.GetQueryName();

		m_listQuery.SetItemText(nSelIndex, 0, strName);

		m_selQry.RemoveAll();
		m_selQry = m_arrQuery.GetAt(nQueryIndex);
		m_selQry.m_strID = strName;
		m_arrQuery.SetAt(nQueryIndex, m_selQry);

		m_listQuery.SetItemState(nSelIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CTrSetDlg::OnItemchangedListTrquery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	char szName[1024];
	memset(szName, 0x00, sizeof(szName));
	m_listQuery.GetItemText(pNMListView->iItem, pNMListView->iSubItem, szName, sizeof(szName));

	CString strName;
	strName.Format(_T("%s"), szName);

	//	기존에 만들어 놓은 것을 저장한다.
	SaveCurrentQry();

	//	새로 선택된 Query를 가져 온다.
	int nIndex = GetQueryAt(strName);
	if (nIndex < 0)
		return;

	m_selQry.RemoveAll();
	m_selQry = m_arrQuery.GetAt(nIndex);
	m_nSelIndex = nIndex;

	ResetQueryData();

	*pResult = 0;
}

int CTrSetDlg::GetQueryAt(CString strName)
{
	CTrQuery qry;
	int nSize = m_arrQuery.GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = m_arrQuery.GetAt(i);
		if (strName.Compare(qry.m_strID) == 0)
			return i;
	}

	return -1;
}

void CTrSetDlg::ResetQueryData()
{
	QueryDataToGrid(&(m_selQry.m_InPack), &m_GridIn);
	QueryDataToGrid(&(m_selQry.m_OutPack), &m_GridOut);

	m_GridIn.ChangeRootName(_T("Input TR Data"), RGB(255, 255, 255));
	m_GridOut.ChangeRootName(_T("Output TR Data"), RGB(255, 255, 255));

	m_editTRCode.SetWindowText(m_selQry.m_strTRCode);
	m_editRealSymbol.SetWindowText(m_selQry.m_strRealSymbol);

	CheckDlgButton(IDC_CHECK_TRSORT, m_selQry.m_OutPack.GetSortType());
	CheckDlgButton(IDC_CHECK_TRREALABS, m_selQry.m_OutPack.GetRealABS());
}

void CTrSetDlg::GetTRQry(CArrTrQuery* pArr)
{
	if (pArr == NULL)		return;

	pArr->RemoveAll();

	CTrQuery qry;
	int nSize = m_arrQuery.GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = m_arrQuery.GetAt(i);
		pArr->Add(qry);
	}
}

void CTrSetDlg::SetTRQry(CArrTrQuery* pArr)
{
	if (pArr == NULL)		return;

	m_arrQuery.RemoveAll();

	CTrQuery qry;
	int nSize = pArr->GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = pArr->GetAt(i);
		m_arrQuery.Add(qry);
	}
}

void CTrSetDlg::EnableControl()
{
	if (!m_bDirectSend)
	{
		m_listQuery.EnableWindow(FALSE);
		m_GridIn.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ADDTRQUERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELTRQUERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRREALSYMBOL)->EnableWindow(FALSE);
	}
}

void CTrSetDlg::SaveCurrentQry()
{
	if (m_nSelIndex >= 0)
	{
		m_selQry.RemoveAll();

		m_editTRCode.GetWindowText(m_selQry.m_strTRCode);
		m_editRealSymbol.GetWindowText(m_selQry.m_strRealSymbol);

		m_selQry.m_OutPack.SetSortType(IsDlgButtonChecked(IDC_CHECK_TRSORT));
		m_selQry.m_OutPack.SetRealABS(IsDlgButtonChecked(IDC_CHECK_TRREALABS));

		GridToQueryData(&(m_selQry.m_InPack), &m_GridIn);
		GridToQueryData(&(m_selQry.m_OutPack), &m_GridOut);

		m_arrQuery.SetAt(m_nSelIndex, m_selQry);
	}
}

void CTrSetDlg::OnOK()
{
	SaveCurrentQry();

	CDialog::OnOK();
}

BOOL CTrSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return 1;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
