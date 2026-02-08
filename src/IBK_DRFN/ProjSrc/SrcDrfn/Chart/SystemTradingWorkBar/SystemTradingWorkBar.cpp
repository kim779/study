// SystemTradingWorkBar.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemTradingWorkBar.h"

#include "../Include_ST/MessageDefine.h"
#include "../include_ST/DeclareDefine.h"

#include "WndTreeCtrlOwner.h"
#include "TreeCtrlSysTrading.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		WORKSPACE_WIDTH			200
#define		IDC_WORKSPACE			3000

#define		IDC_WNDFUNCTION		3110
#define		IDC_WNDSTRATEGY		3111
#define		IDC_WNDFUNCSTGY		3112

#define		IDC_TREESTRATEGY	3011

#define COLOR_MASK	RGB(0x00,0x80,0x80)

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingWorkBarApp

BEGIN_MESSAGE_MAP(CSystemTradingWorkBarApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingWorkBarApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingWorkBarApp construction

CSystemTradingWorkBarApp::CSystemTradingWorkBarApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
//	m_pTreeStrategy = NULL;
	m_pFlatTabCtrl = NULL;

	m_pWndFunction = NULL;
	m_pWndStrategy = NULL;
	m_pWndFunctionInStrategy = NULL;
}

BOOL CSystemTradingWorkBarApp::CreateWorkspace(CFrameWnd* p_pMainWnd, LPSTR p_lpszRootPath, VT_SCRIPTINFO* p_pVtStartegyInfo, VT_SCRIPTINFO* p_pVtUserFuncInfo)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_pMainWnd = p_pMainWnd;
	m_lpszRootPath = p_lpszRootPath;
	m_pVtStrategyInfo = p_pVtStartegyInfo;
	m_pVtUserFuncInfo = p_pVtUserFuncInfo;

	KEY_GETENVVARIABLE();
	if(xRead>0) theApp.m_szUserPath = xUserEncDir;
	else m_szUserPath.Format("%s%s", p_lpszRootPath, "\\Usr");

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_CLASSTREE);

	m_ImageList.Create(IDB_CLASSTREE, 16, 1, COLOR_MASK);
//	m_ImageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 2, 1);
	
	m_ImageList.Add(&bitmap, RGB(0,255,0));

	bitmap.DeleteObject();
	bitmap.LoadBitmap(IDB_OUTPUT);

	m_ImageListOutput.Create(IDB_OUTPUT, 16, 1, COLOR_MASK);
	m_ImageListOutput.Add(&bitmap, RGB(0,255,0));
	
	return TRUE;
}

void CSystemTradingWorkBarApp::DeleteWorkspace()
{
	if(m_pWndFunction)
	{
		m_pWndFunction->DestroyWindow();
		delete m_pWndFunction;
		m_pWndFunction = NULL;
	}

	if(m_pWndStrategy)
	{
		m_pWndStrategy->DestroyWindow();
		delete m_pWndStrategy;
		m_pWndStrategy = NULL;
	}

	if(m_pWndFunctionInStrategy)
	{
		delete m_pWndFunctionInStrategy;
		m_pWndFunctionInStrategy = NULL;
	}

	if(m_pFlatTabCtrl)
	{
		m_pFlatTabCtrl->SetCurSel(0);
		delete m_pFlatTabCtrl;
		m_pFlatTabCtrl = NULL;
	}
}

CWnd* CSystemTradingWorkBarApp::GetStategyWnd()
{
	if(m_pWndStrategy)
		return m_pWndStrategy->GetTreeCtrl();

	m_pWndStrategy = new CWndTreeCtrlOwner(FT_STRATEGY);
	if(!m_pWndStrategy->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), m_pMainWnd, IDC_WNDSTRATEGY))
	{
		delete m_pWndStrategy;
		m_pWndStrategy = NULL;
	}
	else
	{
		m_pWndStrategy->InitTreeCtrl(&m_ImageList);
	}
	
	return m_pWndStrategy->GetTreeCtrl();
}

void CSystemTradingWorkBarApp::CreateTabs()
{
	m_pFlatTabCtrl = new CXTExcelTabCtrl;

	int dwNewStyle = FTS_XT_BOTTOM | FTS_XT_HSCROLL | FTS_XT_TOOLTIPS;
	
	// Create  the flat tab control.
	if (!m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|dwNewStyle,
		CRect(0, 0, 0, 0), m_pMainWnd, IDC_FLAT_TABCTRL))
	{
		TRACE0( "Failed to create flattab control\n" );
		return;
	}

	// set the tab control to have a static edge border.
	m_pFlatTabCtrl->ModifyStyleEx(NULL, WS_EX_STATICEDGE);

	CreateViews();

	// Insert tabs into the flat tab control.
	m_pFlatTabCtrl->InsertItem(0, _T("검증"), &m_sheet1);
	m_pFlatTabCtrl->InsertItem(1, _T("찾기"), &m_sheet2);
	m_pFlatTabCtrl->InsertItem(2, _T("메세지"), &m_sheet3);
	
	m_pFlatTabCtrl->SetCurSel(0);

	m_pFlatTabCtrl->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, 
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	m_sheet1.InsertColumn(0, _T("Icon"), LVCFMT_LEFT, 18);
	m_sheet1.InsertColumn(1, _T("Column"), LVCFMT_LEFT, 512);

	m_sheet2.InsertColumn(0, _T("Find"), LVCFMT_LEFT, 512);

//	m_sheet3.InsertColumn(0, _T("Message"), LVCFMT_LEFT, 512);
}

BOOL CSystemTradingWorkBarApp::CreateViews()
{
	CRect rectDummy(0, 0, 0, 0);
	// Create the sheet1 list box.
	if ( ::IsWindow( m_sheet1.m_hWnd ) )
	{
		m_sheet1.DestroyWindow();
	}
	if (!m_sheet1.Create(WS_CHILD |  
		LVS_REPORT | LVS_NOCOLUMNHEADER, rectDummy, m_pFlatTabCtrl, IDC_SHEET1 ))
	{
		TRACE0( "Failed to create sheet1.\n" );
		return -1;
	}
	ListView_SetExtendedListViewStyleEx(m_sheet1.m_hWnd,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	m_sheet1.SetImageList(&m_ImageListOutput, LVSIL_SMALL);

	// Create the sheet2 list box.
	if ( ::IsWindow( m_sheet2.m_hWnd ) )
	{
		m_sheet2.DestroyWindow();
	}
	if (!m_sheet2.Create(WS_CHILD |  
		LVS_REPORT | LVS_NOCOLUMNHEADER, rectDummy, m_pFlatTabCtrl, IDC_SHEET2 ))
	{
		TRACE0( "Failed to create sheet2.\n" );
		return -1;
	}
	ListView_SetExtendedListViewStyleEx(m_sheet2.m_hWnd,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// Create the sheet3 list box.
	if ( ::IsWindow( m_sheet3.m_hWnd ) )
	{
		m_sheet3.DestroyWindow();
	}
	if (!m_sheet3.Create(WS_CHILD | WS_VSCROLL |
		LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | LBS_OWNERDRAWFIXED , rectDummy, m_pFlatTabCtrl, IDC_SHEET3 ))
	{
		TRACE0( "Failed to create sheet3.\n" );
		return -1;
	}
	
	return TRUE;
}

CWnd* CSystemTradingWorkBarApp::GetOutputWnd()
{
	if(m_pFlatTabCtrl)
		return m_pFlatTabCtrl;

	m_sheet1.SetBackColor(XTAuxData().clrWindow);
	m_sheet2.SetBackColor(XTAuxData().clrWindow);
	m_sheet3.SetBackColor(XTAuxData().clrWindow);

	CreateTabs();

	return m_pFlatTabCtrl;
}

CWnd* CSystemTradingWorkBarApp::GetFunctionWnd()
{
	if(m_pWndFunction)
		return m_pWndFunction->GetTreeCtrl();

	m_pWndFunction = new CWndTreeCtrlOwner(FT_USERFUNCTION);
	if(!m_pWndFunction->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), m_pMainWnd, IDC_WNDFUNCTION))
	{
		delete m_pWndFunction;
		m_pWndFunction = NULL;
	}
	else
	{
		m_pWndFunction->InitTreeCtrl(&m_ImageList);
	}
	
	return m_pWndFunction->GetTreeCtrl();
}

CWnd* CSystemTradingWorkBarApp::GetFunctionInStrategyWnd()
{
	if(m_pWndFunctionInStrategy)
		return m_pWndFunctionInStrategy->GetTreeCtrl();

	m_pWndFunctionInStrategy = new CWndTreeCtrlOwner(FT_ALLFILE);
	if(!m_pWndFunctionInStrategy->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), m_pMainWnd, IDC_WNDFUNCSTGY))
	{
		delete m_pWndFunctionInStrategy;
		m_pWndFunctionInStrategy = NULL;
	}
	else
	{
		m_pWndFunctionInStrategy->InitTreeCtrl(&m_ImageList);
	}
	
	return m_pWndFunctionInStrategy->GetTreeCtrl();
}

void CSystemTradingWorkBarApp::AddListFunction(CScriptInfomation* pFunction)
{
	CTreeCtrlSysTrading* pTreeFunction = (CTreeCtrlSysTrading*)GetFunctionWnd();
	if(!pTreeFunction)
		return;

	HTREEITEM hItem = pTreeFunction->CXTTreeCtrl::FindItem(_T("모든 사용자함수"));
	
	int nImage = pFunction->GetCompiledFlag();

	HTREEITEM hSubItem = pTreeFunction->InsertItem(pFunction->GetFunctionName(), nImage, nImage, hItem);
	pTreeFunction->SetItemData(hSubItem, pFunction->GetUniqueID());

	pTreeFunction->Select(hSubItem, TVGN_CARET);
	pTreeFunction->EnsureVisible(hSubItem);
}

void CSystemTradingWorkBarApp::AddListStrategy(CScriptInfomation* pStrategy)
{
	CTreeCtrlSysTrading* pTreeStrategy = (CTreeCtrlSysTrading*)GetStategyWnd();
	if(!pTreeStrategy)
		return;

	HTREEITEM hItem = pTreeStrategy->CXTTreeCtrl::FindItem(_T("모든 전략"));

	int nImage = pStrategy->GetCompiledFlag();

	HTREEITEM hSubItem = pTreeStrategy->InsertItem(pStrategy->GetFunctionName(), nImage, nImage, hItem);
	pTreeStrategy->SetItemData(hSubItem, pStrategy->GetUniqueID());

	pTreeStrategy->Select(hSubItem, TVGN_CARET);
	pTreeStrategy->EnsureVisible(hSubItem);
}

void CSystemTradingWorkBarApp::AddListFunctionInStrategy(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
{

	CTreeCtrlSysTrading* pTreeFunctionInStrategy = (CTreeCtrlSysTrading*)GetFunctionInStrategyWnd();
	if(!pTreeFunctionInStrategy)
		return;

/*
	HTREEITEM hItem = NULL;
	HTREEITEM hRootItem = pTreeFunctionInStrategy->GetRootItem();
	while(TRUE)
	{
		hItem = pTreeFunctionInStrategy->FindItem(lpszName, NULL, FALSE);
		if(pTreeFunctionInStrategy->GetParentItem(hItem) == hRootItem)
			break;
	}
*/

	HTREEITEM hItem = pTreeFunctionInStrategy->FindItem(lpszName, NULL, FALSE);
	if(!hItem)
	{
		CScriptFind findScript(lpszName);
		VT_SCRIPTINFO* pVtStrategyInfo = theApp.GetStrategyInfo();
		VT_SCRIPTINFO::iterator it = std::find_if(pVtStrategyInfo->begin(), pVtStrategyInfo->end(), findScript);

		if(it == pVtStrategyInfo->end())
			return;

		int nImage = (*it)->GetCompiledFlag();
		hItem = pTreeFunctionInStrategy->InsertItem(lpszName, nImage, nImage);
	}
	else
	{
		HTREEITEM hSubItem = pTreeFunctionInStrategy->GetChildItem(hItem);
		while(hSubItem)
		{
			pTreeFunctionInStrategy->DeleteItem(hSubItem);
			hSubItem = pTreeFunctionInStrategy->GetChildItem(hItem);
		}
	}

	VT_DECLDATA::iterator it = pVtUsedUserFunction->begin();
	while(it != pVtUsedUserFunction->end())
	{
		InsertFunctionItem(hItem, (*it), pTreeFunctionInStrategy);
		it++;
	}

	pTreeFunctionInStrategy->Expand(hItem, TVE_EXPAND);
}

void CSystemTradingWorkBarApp::AddListFunctionInFunction(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
{
	CTreeCtrlSysTrading* pTreeFunctionInStrategy = (CTreeCtrlSysTrading*)GetFunctionInStrategyWnd();
	if(!pTreeFunctionInStrategy)
		return;

	HTREEITEM hRootItem = pTreeFunctionInStrategy->GetRootItem();
	HTREEITEM hItem = NULL;
	char szName[65] = {NULL, };
	TVITEM item;
	item.mask = TVIF_HANDLE|TVIF_TEXT;
	while(hRootItem)
	{
		item.hItem = hRootItem;
		item.pszText = szName;
		item.cchTextMax = sizeof(szName) - 1;
		if(pTreeFunctionInStrategy->GetItem(&item))
		{
			if(::lstrcmp(item.pszText, lpszName) == 0)
			{
				hItem = hRootItem;
				break;
			}
		}

		hRootItem = pTreeFunctionInStrategy->GetNextItem(hRootItem, TVGN_NEXT);
		if(!hRootItem)
			break;

		::memset(szName, 0x00, sizeof(szName));
	}

	if(!hItem)
	{
		CScriptFind findScript(lpszName);
		VT_SCRIPTINFO* pVtFunctionInfo = theApp.GetUserFuncInfo();
		VT_SCRIPTINFO::iterator it = std::find_if(pVtFunctionInfo->begin(), pVtFunctionInfo->end(), findScript);

		if(it == pVtFunctionInfo->end())
			return;

		int nImage = (*it)->GetCompiledFlag();
		hItem = pTreeFunctionInStrategy->InsertItem(lpszName, nImage, nImage);
	}
	else
	{
		HTREEITEM hSubItem = pTreeFunctionInStrategy->GetChildItem(hItem);
		while(hSubItem)
		{
			pTreeFunctionInStrategy->DeleteItem(hSubItem);
			hSubItem = pTreeFunctionInStrategy->GetChildItem(hItem);
		}
	}

	VT_DECLDATA::iterator it = pVtUsedUserFunction->begin();
	while(it != pVtUsedUserFunction->end())
	{
		InsertFunctionItem(hItem, (*it), pTreeFunctionInStrategy);
		it++;
	}

	pTreeFunctionInStrategy->Expand(hItem, TVE_EXPAND);
}

void CSystemTradingWorkBarApp::InsertFunctionItem(HTREEITEM hItem, CDeclareData* pFunctionItem, CTreeCtrlSysTrading* pTreeFunctionInStrategy)
{
	CScriptFind findScript(pFunctionItem->GetDeclareOriginString());
	VT_SCRIPTINFO* pVtUserFuncInfo = theApp.GetUserFuncInfo();
	VT_SCRIPTINFO::iterator it = std::find_if(pVtUserFuncInfo->begin(), pVtUserFuncInfo->end(), findScript);

	if(it == pVtUserFuncInfo->end())
		return;

	LPCSTR lpszSubItemText = (*it)->GetFunctionName();
	if(pTreeFunctionInStrategy->GetItemText(hItem).CompareNoCase(lpszSubItemText) == 0)
		return;
	
	HTREEITEM hSubItem = 0;
	int nImage = (*it)->GetCompiledFlag();
	
	hSubItem = pTreeFunctionInStrategy->InsertItem(lpszSubItemText, nImage, nImage, hItem);

	if(hSubItem)
		pTreeFunctionInStrategy->SetItemData(hSubItem, (*it)->GetUniqueID());
}

void CSystemTradingWorkBarApp::DeleteListFunctionInStrategy(LPCSTR lpszName)
{
	CTreeCtrlSysTrading* pTreeFunctionInStrategy = (CTreeCtrlSysTrading*)GetFunctionInStrategyWnd();
	if(!pTreeFunctionInStrategy)
		return;

	HTREEITEM hRootItem = pTreeFunctionInStrategy->GetRootItem();
	char szName[65] = {NULL, };
	TVITEM item;
	item.mask = TVIF_HANDLE|TVIF_TEXT;
	while(hRootItem)
	{
		item.hItem = hRootItem;
		item.pszText = szName;
		item.cchTextMax = sizeof(szName) - 1;
		if(pTreeFunctionInStrategy->GetItem(&item))
		{
			if(::lstrcmp(item.pszText, lpszName) == 0)
			{
				pTreeFunctionInStrategy->DeleteItem(hRootItem);
				break;
			}
		}

		hRootItem = pTreeFunctionInStrategy->GetNextItem(hRootItem, TVGN_NEXT);
		if(!hRootItem)
			break;

		::memset(szName, 0x00, sizeof(szName));
	}
}

void CSystemTradingWorkBarApp::SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo)
{
	if(!m_pFlatTabCtrl)
		return;

	if(m_pFlatTabCtrl->GetCurSel() != 0)
		m_pFlatTabCtrl->SetCurSel(0);

	int nImage = GetCompileResultImageIndex(nMessage);
	int nIndex = m_sheet1.InsertItem(m_sheet1.GetItemCount(), lpszText, nImage);

	if(pErrorInfo)
		m_sheet1.SetItemData(nIndex, (DWORD)pErrorInfo->GetLineNumber());
	m_sheet1.EnsureVisible(nIndex, TRUE);
}

void CSystemTradingWorkBarApp::SetFindText(LPCSTR lpszFindScriptName, UINT nFileType)
{
	if(!m_pFlatTabCtrl)
		return;

	if(m_pFlatTabCtrl->GetCurSel() != 1)
		m_pFlatTabCtrl->SetCurSel(1);

	int nIndex = m_sheet2.InsertItem(m_sheet2.GetItemCount(), lpszFindScriptName);

	m_sheet2.SetItemData(nIndex, (DWORD)nFileType);
	m_sheet2.EnsureVisible(nIndex, TRUE);
}

void CSystemTradingWorkBarApp::SetServerMessage(LPCSTR lpszMessage, UINT nLevel)
{
	if(!m_pFlatTabCtrl)
		return;

	if(m_pFlatTabCtrl->GetCurSel() != 2)
		m_pFlatTabCtrl->SetCurSel(2);

	CString strMessage(_T(""));
	CTime tm = CTime::GetCurrentTime();
	CString strTime = tm.Format(_T("[%H:%M:%S] "));
	strMessage = strTime + lpszMessage;

	int nIndex = m_sheet3.InsertString(0, strMessage);

	switch(nLevel)
	{
	case SVRMSG_ALERT:
		m_sheet3.SetItemData(nIndex, (DWORD)RGB(255, 80, 80));
		m_sheet3.SetCurSel(nIndex);
		break;
	case SVRMSG_WARNING:
		m_sheet3.SetItemData(nIndex, (DWORD)RGB(189, 147, 0));
		m_sheet3.SetCurSel(nIndex);
		break;
	case SVRMSG_SPECIAL:
		m_sheet3.SetItemData(nIndex, (DWORD)RGB(0, 217, 0));
		break;
	default:
		m_sheet3.SetItemData(nIndex, (DWORD)0);
	}

}

int CSystemTradingWorkBarApp::GetCompileResultImageIndex(int nMessage)
{
	int nImage = 0;

	nMessage = (int)(nMessage / 10000);
	switch(nMessage)
	{
	case 4:
		nImage = 2;
		break;
	case 6:
		nImage = 1;
		break;
	}

	return nImage;
}

void CSystemTradingWorkBarApp::ChangeCompiledOption(UINT nFileType, LPCSTR szName, UINT nCompiled)
{
	CTreeCtrlSysTrading* pTreeFunctionInStrategy = (CTreeCtrlSysTrading*)GetFunctionInStrategyWnd();

	if(nFileType == FT_STRATEGY)
	{
		VT_SCRIPTINFO* pVtStrategy = theApp.GetStrategyInfo();

		CScriptFind find(szName);
		VT_SCRIPTINFO::iterator it = std::find_if(pVtStrategy->begin(), pVtStrategy->end(), find);
		if(it != pVtStrategy->end())
			(*it)->SetCompileCheck(nCompiled);

		CTreeCtrlSysTrading* pTreeStrategy = (CTreeCtrlSysTrading*)GetStategyWnd();
//		HTREEITEM hItem = pTreeStrategy->FindItem(szName, NULL, FALSE);
		HTREEITEM hItem = pTreeStrategy->GetRootItem();
		HTREEITEM hItemFS = NULL;

		while(hItem)
		{
			hItemFS = pTreeStrategy->FindItemInBranch(szName, FALSE, FALSE, hItem);

			pTreeStrategy->SetItemImage(hItemFS, nCompiled, nCompiled);

			hItem = pTreeStrategy->GetNextItem(hItem, TVGN_NEXT);
		}

		if(!pTreeFunctionInStrategy)
			return;

		hItemFS = pTreeFunctionInStrategy->FindItem(szName, NULL, FALSE);
		if(hItemFS)
			pTreeFunctionInStrategy->SetItemImage(hItemFS, nCompiled, nCompiled);
	}
	else if(nFileType == FT_USERFUNCTION)
	{
		VT_SCRIPTINFO* pVtFunction = theApp.GetUserFuncInfo();

		CScriptFind find(szName);
		VT_SCRIPTINFO::iterator it = std::find_if(pVtFunction->begin(), pVtFunction->end(), find);
		if(it != pVtFunction->end())
			(*it)->SetCompileCheck(nCompiled);

		CTreeCtrlSysTrading* pTreeFunction = (CTreeCtrlSysTrading*)GetFunctionWnd();
//		HTREEITEM hItem = pTreeFunction->FindItem(szName, NULL, FALSE);
		HTREEITEM hItem = pTreeFunction->GetRootItem();
		HTREEITEM hItemFS = NULL;

		while(hItem)
		{
			hItemFS = pTreeFunction->FindItemInBranch(szName, FALSE, FALSE, hItem);

			pTreeFunction->SetItemImage(hItemFS, nCompiled, nCompiled);

			hItem = pTreeFunction->GetNextItem(hItem, TVGN_NEXT);
		}

		if(!pTreeFunctionInStrategy)
			return;

		hItem = pTreeFunctionInStrategy->GetRootItem();
		while(hItem)
		{
			hItemFS = pTreeFunctionInStrategy->FindItem(szName, hItem, FALSE);
			if(hItemFS)
				pTreeFunctionInStrategy->SetItemImage(hItemFS, nCompiled, nCompiled);

			hItem = pTreeFunctionInStrategy->GetNextItem(hItem, TVGN_NEXT);
		}
	}
}

void CSystemTradingWorkBarApp::EnsureVisibleFunctionInStrategy(LPCSTR lpszName)
{
	CTreeCtrlSysTrading* pTreeFunctionInStrategy = (CTreeCtrlSysTrading*)GetFunctionInStrategyWnd();
	if(!pTreeFunctionInStrategy)
		return;

	HTREEITEM hItem = pTreeFunctionInStrategy->FindItem(lpszName, NULL, FALSE);
	if(hItem)
	{
		pTreeFunctionInStrategy->Select(hItem, TVGN_CARET);
		pTreeFunctionInStrategy->EnsureVisible(hItem);
	}
}

void CSystemTradingWorkBarApp::ClearOutputText()
{
	if(!m_sheet1.GetSafeHwnd())
		return;

	m_sheet1.DeleteAllItems();
}

void CSystemTradingWorkBarApp::ClearFindText()
{
	if(!m_sheet2.GetSafeHwnd())
		return;

	m_sheet2.DeleteAllItems();
}

void CSystemTradingWorkBarApp::ClearMessageText()
{
	if(!m_sheet3.GetSafeHwnd())
		return;

	int nCount = m_sheet3.GetCount();
	for(int i = nCount - 1; i >= 0; i--)
		m_sheet3.DeleteString(i);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingWorkBarApp object

CSystemTradingWorkBarApp theApp;

extern "C" __declspec(dllexport) BOOL EX_CreateWorkspace(CFrameWnd* p_pMainWnd, LPSTR p_lpszRootPath, VT_SCRIPTINFO* p_pVtStartegyInfo, VT_SCRIPTINFO* p_pVtUserFuncInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.CreateWorkspace(p_pMainWnd, p_lpszRootPath,p_pVtStartegyInfo, p_pVtUserFuncInfo);
}

extern "C" __declspec(dllexport) void EX_DeleteWorkspace()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.DeleteWorkspace();
}

extern "C" __declspec(dllexport) CWnd* EX_GetStategyWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetStategyWnd();
}

extern "C" __declspec(dllexport) CWnd* EX_GetFunctionWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetFunctionWnd();
}

extern "C" __declspec(dllexport) CWnd* EX_GetFunctionInStrategyWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetFunctionInStrategyWnd();
}

extern "C" __declspec(dllexport) CWnd* EX_GetOutputWnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetOutputWnd();
}

extern "C" __declspec(dllexport) void EX_SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.SetOutputText(nMessage, lpszText, pErrorInfo);
}

extern "C" __declspec(dllexport) void EX_SetFindText(LPCSTR lpszFindScriptName, UINT nFileType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.SetFindText(lpszFindScriptName, nFileType);
}

extern "C" __declspec(dllexport) void EX_SetServerMessage(LPCSTR lpszMessage, UINT nLevel)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.SetServerMessage(lpszMessage, nLevel);
}


extern "C" __declspec(dllexport) void EX_AddListStrategy(CScriptInfomation* pStrategy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.AddListStrategy(pStrategy);
}

extern "C" __declspec(dllexport) void EX_AddListFunction(CScriptInfomation* pFunction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.AddListFunction(pFunction);
}

extern "C" __declspec(dllexport) void EX_AddListFunctionInStrategy(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.AddListFunctionInStrategy(lpszName, pVtUsedUserFunction);
}

extern "C" __declspec(dllexport) void EX_AddListFunctionInFunction(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.AddListFunctionInFunction(lpszName, pVtUsedUserFunction);
}

extern "C" __declspec(dllexport) void EX_ChangeCompiledOption(UINT nFileType, LPCSTR szName, UINT nCompiled)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.ChangeCompiledOption(nFileType, szName, nCompiled);
}

extern "C" __declspec(dllexport) void EX_DeleteListFunctionInStrategy(LPCSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.DeleteListFunctionInStrategy(lpszName);
}

extern "C" __declspec(dllexport) void EX_EnsureVisibleFunctionInStrategy(LPCSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.EnsureVisibleFunctionInStrategy(lpszName);
}

extern "C" __declspec(dllexport) void EX_ClearAllOutputText()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.ClearOutputText();
	theApp.ClearFindText();
//	theApp.ClearMessageText();
}

extern "C" __declspec(dllexport) void EX_ClearAllFindText()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.ClearFindText();
}
