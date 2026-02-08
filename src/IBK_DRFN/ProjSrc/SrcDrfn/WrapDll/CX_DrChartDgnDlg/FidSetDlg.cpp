// FidSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wdrchartdgndlg.h"
#include "FidSetDlg.h"

#include "../../ForSite/FS_Env.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		ID_SUBMENU_START		24580
#define		ID_SUBMENU_COUNT		10

#define		MARGIN_CONTROL			8

int g_nIndex = 0;
char g_szHideKey[20];

/////////////////////////////////////////////////////////////////////////////
// CFidSetDlg property page

CFidSetDlg::CFidSetDlg(CWnd *pParent)
	: CDialog(CFidSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFidSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSubMenu = NULL;
	m_pSubMenu2 = NULL;
	m_pSelGrid = NULL;

	m_strFile = _T("");

	m_nSelIndex = -1;
	m_pSelFidPack = NULL;

	m_bDirectSend = FALSE;
	m_bUseEstEge = FALSE;

	memset(g_szHideKey, 0x00, sizeof(g_szHideKey));
}

CFidSetDlg::~CFidSetDlg()
{
	m_arrQuery.RemoveAll();
}

void CFidSetDlg::CopyArrQry(CArrFidQuery* pArr)
{
	CFidQuery qry;
	int nSize = pArr->GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = pArr->GetAt(i);
		m_arrQuery.Add(qry);
	}
}

void CFidSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFidSetDlg)
	DDX_Control(pDX, IDC_EDIT_REALSYMBOL, m_editRealSymbol);
	DDX_Control(pDX, IDC_BTN_FIDOPEN, m_btnFidOpen);
	DDX_Control(pDX, IDC_BTN_FIDSAVEAS, m_btnFidSaveAs);
	DDX_Control(pDX, IDC_GROUP_FIDQUERY, m_grpFIDQuery);
	DDX_Control(pDX, IDC_LIST_FIDQUERY, m_listQuery);
	DDX_Control(pDX, IDC_EDIT_FIDTRCODE, m_editFIDTrCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFidSetDlg, CDialog)
	//{{AFX_MSG_MAP(CFidSetDlg)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADDFIDQUERY, OnBtnAddfidquery)
	ON_BN_CLICKED(IDC_BTN_DELFIDQUERY, OnBtnDelfidquery)
	ON_BN_CLICKED(IDC_BTN_RENAME, OnBtnRename)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FIDQUERY, OnItemchangedListFidquery)
	ON_BN_CLICKED(IDC_BTN_FIDSAVEAS, OnBtnFidsaveas)
	ON_BN_CLICKED(IDC_BTN_FIDOPEN, OnBtnFidopen)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SUBMENU_START, ID_SUBMENU_START+ID_SUBMENU_COUNT, OnSubMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFidSetDlg message handlers

void CFidSetDlg::GetFIDQry(CArrFidQuery* pArr)
{
	if (pArr == NULL)		return;

	pArr->RemoveAll();

	CFidQuery qry;
	int nSize = m_arrQuery.GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = m_arrQuery.GetAt(i);
		pArr->Add(qry);
	}
}

void CFidSetDlg::SetFIDQry(CArrFidQuery* pArr)
{
	if (pArr == NULL)		return;

	m_arrQuery.RemoveAll();

	CFidQuery qry;
	int nSize = pArr->GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = pArr->GetAt(i);
		m_arrQuery.Add(qry);
	}
}


BOOL CFidSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	m_btnFidSaveAs.ShowWindow(SW_HIDE);
//	m_btnFidOpen.ShowWindow(SW_HIDE);

	m_listQuery.InsertColumn(0, _T(""), LVCFMT_LEFT, 120);
//--------------------------------------------------------------

	CFidQuery qry;
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

void CFidSetDlg::OnDestroy()
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

int CFidSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

	m_GridIn.ChangeRootName(_T("Input FID Data"), RGB(255, 255, 255));
	m_GridOut.ChangeRootName(_T("Output FID Data"), RGB(255, 255, 255));


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

LRESULT CFidSetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

void CFidSetDlg::QueryDataToGrid(CFidPack* pFidPack, CFidQueryGrid* pGrid)
{
	if ((pFidPack == NULL) || (pGrid == NULL))
		return;

	m_pSelFidPack = pFidPack;
	m_pSelGrid = pGrid;

	m_pSelGrid->DeleteAll();
	m_pSelGrid->SetInitData();
	m_pSelGrid->LockWindowUpdate();
	if (m_pSelFidPack->m_bInput)
		m_pSelGrid->ChangeRootName(_T("Input TR Data"), RGB(255, 255, 255));
	else
		m_pSelGrid->ChangeRootName(_T("Output TR Data"), RGB(255, 255, 255));

	CFidBlock *pBlock = NULL;
	int nSize = m_pSelFidPack->m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_pSelFidPack->m_arrBlock.GetAt(i);
		if (pBlock)
			LoadQueryData(pBlock, m_pSelGrid->GetRootItem(0));
	}

	m_pSelGrid->UnlockWindowUpdate();
}

void CFidSetDlg::LoadQueryData(CFidBlock *pPdataBlock, CSuperGridCtrl::CTreeItem *pPItem)
{
	CString strTemp;
	CSuperGridCtrl::CTreeItem *pNewBlock = NULL, *pNewItem = NULL;
	CItemInfo *pInfo = NULL;

	pNewBlock = m_pSelGrid->InsertNewNode(pPItem);
	pInfo = m_pSelGrid->GetData(pNewBlock);
	pInfo->SetItemText(pPdataBlock->m_strBlockName);
	strTemp = _T("FALSE");
	if (pPdataBlock->IsArray())		strTemp = _T("TRUE");
	pInfo->SetSubItemText(0, strTemp);
	pInfo->SetSubItemText(1, pPdataBlock->m_strGridSym);
	strTemp.Format(_T("%d"), pPdataBlock->m_nReqRow);
	pInfo->SetSubItemText(2, strTemp);

	m_pSelGrid->UpdateData(pNewBlock, pInfo, TRUE);

	if (pPdataBlock)
	{
		CFidItem *pDataItem = NULL;
		int nIndex = 0;
		int nSize = pPdataBlock->m_arrItem.GetSize();
		for (int i=0; i<nSize; i++)
		{
			pDataItem = pPdataBlock->m_arrItem.GetAt(i);

			pNewItem = m_pSelGrid->InsertNewItem(pNewBlock);
			pInfo = m_pSelGrid->GetData(pNewItem);
			pInfo->SetItemText(pDataItem->GetItemID());

			pInfo->SetSubItemText(0, pDataItem->GetChartID());
			pInfo->SetSubItemText(1, pDataItem->GetRealID());
			strTemp = _T("FALSE");
			if (pDataItem->GetABS())	strTemp = _T("TRUE");
			pInfo->SetSubItemText(2, strTemp);

			m_pSelGrid->UpdateData(pNewItem, pInfo, TRUE);
		}
	}
}

BOOL CFidSetDlg::GridToQueryData(CFidPack* pFidPack, CFidQueryGrid* pGrid)
{
	if ((pFidPack == NULL) || (pGrid == NULL))
		return FALSE;

	m_pSelFidPack = pFidPack;
	m_pSelGrid = pGrid;

	m_pSelFidPack->RemoveAll();

	int nRet = MakeQueryData(NULL, m_pSelGrid->GetRootItem(0));
	if (nRet > 0)
		return FALSE;

	return TRUE;
}

int CFidSetDlg::MakeQueryData(CFidBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem)
{
	int nRet = 0;
	int nValue = 0;
	CString strTemp;
	CFidItem *pNewItem = NULL;
	CFidBlock *pBlock = NULL;
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
			pBlock = new CFidBlock;
			pBlock->m_strBlockName = pItemInfo->GetItemText();

			strTemp = pItemInfo->GetSubItem(0);
			nValue = 1;
			if (strTemp.Compare(_T("FALSE")) == 0)
				nValue = 0;

			pBlock->m_bArray = (BOOL)nValue;

			pBlock->m_strGridSym = pItemInfo->GetSubItem(1);
			pBlock->m_nReqRow = atoi(pItemInfo->GetSubItem(2));

			m_pSelFidPack->AddBlock(pBlock);

			nRet = MakeQueryData(pBlock, pChildItem);
			if (nRet > 0)
				return nRet;
		}
		else
		{
			if (pPBlock && pPBlock->m_bArray)
				pNewItem = new CFidItemArr;
			else
				pNewItem = new CFidItem;

			pNewItem->SetItemID(pItemInfo->GetItemText());
			pNewItem->SetChartID(pItemInfo->GetSubItem(0));
			pNewItem->SetRealID(pItemInfo->GetSubItem(1));

			strTemp = pItemInfo->GetSubItem(2);
			if (strTemp.Compare(_T("FALSE")) == 0)
				pNewItem->SetABS(FALSE);
			else
				pNewItem->SetABS(TRUE);

			nRet = m_pSelFidPack->AddItem(pPBlock->m_strBlockName, pNewItem);
			if (nRet > 0)
				return nRet;
		}
	}

	return 0;
}

void CFidSetDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
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

void CFidSetDlg::OnSubMenu(UINT nID)
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

void CFidSetDlg::OnNodeAdd(int nType)
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
void CFidSetDlg::OnItemAdd(int nType)
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
void CFidSetDlg::OnMenuDelete()
{
	if (!m_pSelGrid)
		return;

	m_pSelGrid->SendMessage(WM_KEYDOWN, (WPARAM)VK_DELETE, 0);
}


void CFidSetDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void CFidSetDlg::ReSizeCtrl()
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

	m_grpFIDQuery.GetWindowRect(rect);
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

void CFidSetDlg::OnBtnAddfidquery()
{
	CAddQueryDlg	dlg;
	dlg.SetQueryName(_T("QryName"));
	if (dlg.DoModal() == IDOK)
	{
		CString strName = dlg.GetQueryName();

		int nIndex = m_listQuery.GetItemCount();
		m_listQuery.InsertItem(nIndex, strName);

		CFidQuery	qry;
		qry.m_strID = strName;
		m_arrQuery.Add(qry);

		m_listQuery.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CFidSetDlg::OnBtnRename()
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

void CFidSetDlg::OnBtnDelfidquery() 
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

void CFidSetDlg::OnItemchangedListFidquery(NMHDR* pNMHDR, LRESULT* pResult) 
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

int CFidSetDlg::GetQueryAt(CString strName)
{
	CFidQuery qry;
	int nSize = m_arrQuery.GetSize();
	for (int i=0; i<nSize; i++)
	{
		qry = m_arrQuery.GetAt(i);
		if (strName.Compare(qry.m_strID) == 0)
			return i;
	}

	return -1;
}

void CFidSetDlg::ResetQueryData()
{
	if (m_bUseEstEge)
	{
		QueryDataToGrid(&(m_outQuery.m_InPack), &m_GridIn);
		QueryDataToGrid(&(m_outQuery.m_OutPack), &m_GridOut);
	}
	else
	{
		QueryDataToGrid(&(m_selQry.m_InPack), &m_GridIn);
		QueryDataToGrid(&(m_selQry.m_OutPack), &m_GridOut);
	}

	m_GridIn.ChangeRootName(_T("Input TR Data"), RGB(255, 255, 255));
	m_GridOut.ChangeRootName(_T("Output TR Data"), RGB(255, 255, 255));

	m_editFIDTrCode.SetWindowText(m_selQry.m_strOOPCode);
	m_editRealSymbol.SetWindowText(m_selQry.m_strRealSymbol);

	CheckDlgButton(IDC_CHECK_FIDSORT, m_selQry.m_OutPack.GetSortType());
	CheckDlgButton(IDC_CHECK_FIDREALABS, m_selQry.m_OutPack.GetRealABS());
}

void CFidSetDlg::EnableControl()
{
	if (!m_bDirectSend)
	{
		m_listQuery.EnableWindow(FALSE);
		m_GridIn.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_ADDFIDQUERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELFIDQUERY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_RENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FIDTRCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REALSYMBOL)->EnableWindow(FALSE);
	}
}

void CFidSetDlg::SaveCurrentQry()
{
	if (m_nSelIndex >= 0)
	{
		m_selQry.RemoveAll();

		m_editFIDTrCode.GetWindowText(m_selQry.m_strOOPCode);
		m_editRealSymbol.GetWindowText(m_selQry.m_strRealSymbol);

		m_selQry.m_OutPack.SetSortType(IsDlgButtonChecked(IDC_CHECK_FIDSORT));
		m_selQry.m_OutPack.SetRealABS(IsDlgButtonChecked(IDC_CHECK_FIDREALABS));

		GridToQueryData(&(m_selQry.m_InPack), &m_GridIn);
		GridToQueryData(&(m_selQry.m_OutPack), &m_GridOut);

		m_arrQuery.SetAt(m_nSelIndex, m_selQry);
	}
}

void CFidSetDlg::OnOK()
{
	if (!m_bUseEstEge)
		SaveCurrentQry();

	CDialog::OnOK();
}

BOOL CFidSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if (pMsg->wParam == VK_RETURN)
			{
				if ((strcmp(g_szHideKey, _T("openmind")) == 0) ||
					(strcmp(g_szHideKey, _T("OPENMIND")) == 0))
				{
					m_btnFidSaveAs.ShowWindow(SW_SHOW);
					m_btnFidOpen.ShowWindow(SW_SHOW);
					m_bDirectSend = FALSE;
					EnableControl();
					m_bUseEstEge = TRUE;
					ReSizeCtrl();
				}
			}

			g_nIndex = 0;
			memset(g_szHideKey, 0x00, sizeof(g_szHideKey));

			return 1;
		}
		else
		{
			if (g_nIndex >= 20)
			{
				g_nIndex = 0;
				memset(g_szHideKey, 0x00, sizeof(g_szHideKey));
			}
			else
				g_szHideKey[g_nIndex++] = (char)pMsg->wParam;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CFidSetDlg::OnBtnFidsaveas()
{
	m_outQuery.RemoveAll();

	GridToQueryData(&(m_outQuery.m_InPack), &m_GridIn);
	GridToQueryData(&(m_outQuery.m_OutPack), &m_GridOut);

	// 파일 대화상자.
	char ext[] = "qry";
	DWORD	dwFlag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_LONGNAMES|OFN_NONETWORKBUTTON;
	char szFilter[] = "Query Files (*.qry)|*.qry||";

	CFileDialog writeDlg(FALSE, ext, NULL, dwFlag, szFilter, NULL);
	CString strInitDir;
	GetDir_DrfnRoot(strInitDir);
	strInitDir += "\\Icss\\";
	writeDlg.m_ofn.lpstrInitialDir = (LPCTSTR)strInitDir;
	if (writeDlg.DoModal() != IDOK)
		return;

	m_strFile = writeDlg.GetPathName();	// Write file path

	m_outQuery.WriteQueryFile(m_strFile);
}

void CFidSetDlg::OnBtnFidopen()
{
	char ext[] = "qry";
	DWORD	dwFlag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_LONGNAMES|OFN_NONETWORKBUTTON;
	char szFilter[] = "Query Files (*.qry)|*.qry||";

	CFileDialog openDlg(TRUE, ext, NULL, dwFlag, szFilter, NULL);
	CString strInitDir;
	GetDir_DrfnRoot(strInitDir);
	strInitDir += "\\Icss\\";
	openDlg.m_ofn.lpstrInitialDir = (LPCTSTR)strInitDir;
	if (openDlg.DoModal() != IDOK)
		return;

	m_strFile = openDlg.GetPathName();	// Open file path

	m_outQuery.ReadQueryFile(m_strFile);

	ResetQueryData();
}
