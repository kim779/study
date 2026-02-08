// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MainFrmWnd.h"
#include "QuickMakeDlg.h"
#include "ScreenWnd.h"
//BOOL g_bSSIsHTS = TRUE;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WMU_SET_MESSAGE			(WM_USER + 101)
#define NO_EXPEND_SIZE			820

HWND g_hMainFrmWnd;
extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd

CMainFrmWnd::CMainFrmWnd(CString strFormCode, CWnd* pScreenWnd)
{
	m_bIndustry = FALSE;
//	m_bIsFnUser = FALSE;
	m_bIsFnUser = TRUE;	//삼성에서는 m_bIsFnUser처리를 했다. 다른 사이트는 필요에 따라 처리할 것.
	m_bCreateWnd = FALSE;
	m_bPortAdd = FALSE;
	
	LOGFONT	logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	//m_pctrlSplitter = NULL;
	m_pviewConditionList = NULL;
	m_pviewCondition = NULL;
	
	m_bFirstState = TRUE; //# 0227추가	
	m_hLanguageMng = NULL;
	m_nSplitterPos = 188;	//0000145: 조건검색 좌측탭 Split 조정위치 기억기능 추가

	m_pScreenWnd = pScreenWnd;
	
	SetInitFormCode(strFormCode);
}

CMainFrmWnd::~CMainFrmWnd()
{
	m_listPortInfo.RemoveAll();
	m_mapPortItem.RemoveAll();

	m_brushStatic.DeleteObject();

	m_fontDefault.DeleteObject();
//	if (m_pctrlSplitter != NULL)
//	{
//		if(m_pctrlSplitter->m_hWnd && IsWindow(m_pctrlSplitter->m_hWnd))
//			m_pctrlSplitter->DestroyWindow();
//		delete m_pctrlSplitter;
//		m_pctrlSplitter = NULL;
//	}
	if(m_ctrlSplitter.GetSafeHwnd() != NULL)
	{
		m_ctrlSplitter.DestroyWindow();
	}
}


BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TTN_NEEDTEXT, 0, OnDisplayToolTip)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_USERQUICK_BTN, OnBtnMakeQuick)

	ON_MESSAGE(WM_CONDITIONRESULT_WND, OnNotifyConditionResult)
	ON_MESSAGE(WM_CONDITIONMAKE_WND, OnNotifyConditionMake)
	ON_MESSAGE(WM_NOTIFY_TAB, OnNotifyTabCtrl)
	ON_MESSAGE(WM_NOTIFY_QUICKTAB, OnNotifyQuick)
	ON_MESSAGE(WM_XMLTREE_CTRL, OnNotifyXMLTree)	
//	ON_MESSAGE(UM_CHANGE_REGKW, OnNotify_PortPolio)		-hidden-
	ON_MESSAGE(WMU_REFRESH_INDEXTREE, OnNotify_Language)
	ON_MESSAGE(RMSG_CHECKMENU, OnRmsgCheckMenu)
END_MESSAGE_MAP()

//void CMainFrmWnd::OnNotifyConditionResult(WPARAM wParam, LPARAM lParam)
LRESULT CMainFrmWnd::OnNotifyConditionResult(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_SCREEN_EXP)
	{
		m_bTopExp = (LPARAM)lParam;
		/*m_pviewCondition->SetTopExpend(m_bTopExp);*/
		
		m_nTopExp = (LPARAM)lParam;
		m_pviewCondition->SetTopExpend(m_nTopExp);
	}
	else if (wParam == NM_LOAD_PORTFOLIO)
		m_pviewConditionList->m_treeCommonCtrl.SetReloadPortPolio();
	else if (wParam == NM_UPDATE_FIXFIELD)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetMatchLogic_UpDate();
	else if (wParam == NM_ROLL_SEARCH)
	{
		BOOL bState = (LPARAM)lParam;
		
		SetControlEnable(bState);
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetControlEnable(bState);
	}
	else if (wParam == NM_SET_WORKDATE)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetWorkData((CStringArray*)lParam);
	else if (wParam == NM_SET_MSTCODE)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetMSTCode((CStringArray*)lParam);
	else if (wParam == NM_SHOW_LANGUAGE)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetShow_LanguageEdit();
	else if (wParam == NM_SENDDATA_SHARD)
	{
		ShardData *pstShardData = (ShardData*)lParam;
		SetSharedDataToSend(pstShardData);
	}

	return TRUE;
}

void CMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	CString strScreenNo(pstShardData->strScreenNo);

	if(pstShardData->saCode.GetSize() > 0)
	{
		pScreenWnd->OpenScreen(strScreenNo, &pstShardData->saCode, &pstShardData->saName);
	}
	else
	{
		pScreenWnd->OpenScreen(strScreenNo);
	}
}

BOOL CMainFrmWnd::SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode)
{	
	return TRUE;
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	if (strCode.IsEmpty() == FALSE)
	{
		//# Screen Link Data!
		
		/*
		CMainView *pviewMain = (CMainView*)GetParent();
		pviewMain->PutLinkData("&STOCK_CODE", (LPCSTR)strCode);
		*/
	}
}
void CMainFrmWnd::SetControlEnable(BOOL bState)
{
	bState = !bState;
	
	/*
	m_btnMakeQuick.EnableWindow(bState);				
	m_tabQuick.EnableWindow(bState);					
	m_ctrlMakeQuickBar.EnableWindow(bState);
	*/
	
	/*
	m_pviewConditionList->EnableWindow(bState);
	m_pviewConditionList->SetControlEnable(bState);
	m_pviewConditionList->Invalidate();
	*/
	
	/*
	m_btnMakeQuick.Invalidate();				
	m_tabQuick.Invalidate();					
	m_ctrlMakeQuickBar.Invalidate();
	*/
}

LRESULT CMainFrmWnd::OnNotifyConditionMake(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_INIT)
	{
		int nSelectGroup = (int)lParam;
		
		m_pviewConditionList->SetTreeView();
		((CConditionResultWnd*)GetWnd("RESULT"))->SetClearAll();
	}
	else if (wParam == NM_MAPSHOW)
	{
		BOOL bState = (LPARAM)lParam;
		
		SetControlEnable(bState);
		((CConditionResultWnd*)GetWnd("RESULT"))->SetControlEnable(bState);
	}
	else if (wParam == NM_ADD_DATA)
	{
		//# 자료 구분 (Condition / Language / PortPolio / File)
		if (m_nNowDataType == -1)					//# Nothing! 
		{
			if(m_nSeparateForm == 0)
				SetMessageBox("입력할 지표가 없습니다.", (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
			else
				SetMessageBox("입력할 지표가 없습니다.", (CString)PROGRAM_TITLE_BRIGHT, MB_ICONWARNING | MB_OK);
		}
		else if (m_nNowDataType == 0) //# Condition
			((CConditionMakeWnd*)GetWnd("MAKE"))->SetCondition(TRUE, &m_stNowXMLTreeInfo);
		else if (m_nNowDataType == 1)			//# File
			((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile);
	}
	else if (wParam == NM_TAB_CHANGE)
	{
		CUIntArray *psiData = (CUIntArray *)lParam;
		
		int nViewType = m_pviewConditionList->m_nViewType_Tree;
		m_pviewConditionList->SetSelectTab(nViewType, psiData->GetAt(1));
	}
	else if (wParam == NM_LOAD_FINENAME)
	{
		XMLTreeInfoFile	*pstXMLTreeInfoFile = (XMLTreeInfoFile*)lParam;
		
		((CConditionResultWnd*)GetWnd("RESULT"))->SetLoadedFile(pstXMLTreeInfoFile);
		if (pstXMLTreeInfoFile == NULL)
		{
			((CConditionResultWnd*)GetWnd("RESULT"))->SetClearAll();
			((CConditionResultWnd*)GetWnd("RESULT"))->SetSearchResultState();
		}
	}
	else if (wParam == NM_LOAD_FINENAME_TEMP)
	{
		CString strFileName = *(CString*)lParam;
		((CConditionResultWnd*)GetWnd("RESULT"))->SetLoadedFile(strFileName);
	}
	else if (wParam == NM_SEARCH)
		((CConditionResultWnd*)GetWnd("RESULT"))->SetSearch();
	else if (wParam == NM_FILE_SYNC)
	{
		//	m_pShareManager->SetShared_FileSync();
	}
	else if (wParam == NM_FILE_SAVE)
	{
		SetSaveTreeXml_User(TRUE);
	}
	else if (wParam == NM_SCREEN_EXP)
	{
		m_bLeftExp = (LPARAM)lParam;
		SetLeftExpend(m_bLeftExp);		
	}
	else if (wParam == NM_CHANGE_CONDITION)
	{
		int nSelectGroup = (LPARAM)lParam;
		if (nSelectGroup == 0)
			m_pviewConditionList->SetSelectTab(0, -1);
		else
			m_pviewConditionList->SetSelectTab(1, -1);
	}

	return TRUE;
}

void CMainFrmWnd::SetLeftExpend(BOOL bState)
{
	if(m_ctrlSplitter.GetSafeHwnd()==NULL) return;
	
	m_ctrlSplitter.SetShow_Col(!bState, 0);

	CRect reClient;
	m_pviewConditionList->GetClientRect(reClient);
	
	if(bState)
	{
		m_pviewConditionList->SetShowTree(TRUE);
	}
	else
	{
		m_pviewConditionList->SetShowTree(FALSE);
	}
}

void CMainFrmWnd::OnBtnMakeQuick()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CQuickMakeDlg dlgQuickMake(this);
	if (dlgQuickMake.DoModal() == IDOK)
		SetQuickInfo_UpDate();

	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::SetQuickInfo_UpDate()
{
	/*m_tabQuick.SetInitTab(&m_listQuickInfo);*/
	
	//# Result Xml File Save!
	SetSaveTreeXml("USER", &m_listUserInfoFolder);
// 아래 2줄 막아놓음. 사용자조건의 폴더를 삭제하면 추천조건이 사라지는 현상 수정.
//	SetSaveTreeXml("SYS", &m_listSysInfoFolder);
//	SetSaveQuickXml(&m_listQuickInfo);
	
// 	if (m_listQuickInfo.GetCount() == 0)
// 		m_tabQuick.ShowWindow(SW_HIDE);
// 	else
// 		m_tabQuick.ShowWindow(SW_SHOW);
}

void CMainFrmWnd::SetQuickInfo_Delete(CString strType, CString strFolderPath, CString strFilePath)
{
	POSITION pos;
	QuickList		stQuickList;
	BOOL bDeleteState;
	for (int nIndex = 0; nIndex < m_listQuickInfo.GetCount(); nIndex++)
	{
		bDeleteState = FALSE;
		
		pos = m_listQuickInfo.FindIndex(nIndex);
		stQuickList = m_listQuickInfo.GetAt(pos);
		
		if (strType == "FILE")
		{
			if ((strFolderPath == stQuickList.strFolderPath) && (strFilePath == stQuickList.strFilePath))
				bDeleteState = TRUE;
		}
		else if (strType == "FOLDER")
		{
			if (strFolderPath == stQuickList.strFolderPath)
				bDeleteState = TRUE;
		}
		
		if (bDeleteState == TRUE)
		{
			m_listQuickInfo.RemoveAt(pos);
			
			if (strType == "FILE")
				break;
		}
	}
	
	SetQuickInfo_UpDate();
}

LRESULT CMainFrmWnd::OnNotifyQuick(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_QUICK_CLICK)
	{
		QuickList *pstQuickList = (QuickList*)lParam;
		
		m_stNowXMLTreeInfoFile.strType = pstQuickList->strType;		//# USER, SYS
		m_stNowXMLTreeInfoFile.strFolderPath = pstQuickList->strFolderPath;
		m_stNowXMLTreeInfoFile.strFilePath = pstQuickList->strFilePath;
		m_stNowXMLTreeInfoFile.strFileName = pstQuickList->strFileName;
		m_stNowXMLTreeInfoFile.bQuickReg = TRUE;
		m_stNowXMLTreeInfoFile.strAlertReg = (pstQuickList->bAlertReg == TRUE)?"TRUE":"FALSE";
		
		m_nNowDataType = 2;
		char buf[3] = "10";
		CString strMsg;
		if (((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile, TRUE, TRUE) == TRUE)
		{
			strMsg = "퀵버튼/" + m_stNowXMLTreeInfoFile.strFileName;
			((CConditionResultWnd*)GetWnd("RESULT"))->SetSearch();
		}
		else
			strMsg = m_stNowXMLTreeInfoFile.strFileName + " Loading 중 Error!";
		
		
// 		CWnd *pParent = GetParent();		//# EzFinderMainView
// 		pParent = pParent->GetParent();	//# CWnd
		//pParent->SendMessage(WMU_DISP_MSG, (WPARAM)buf, (LPARAM)(LPCTSTR)strMsg);
		
		
		/*
		//# Tree Root View!
		
		  HTREEITEM hRoot, hChild, hSelItem, hTemp;
		  CString strCompare;
		  hRoot = m_pviewConditionList->m_treeUserCtrl.GetRootItem();
		  
			hChild = m_pviewConditionList->m_treeUserCtrl.GetChildItem(hRoot);
			strCompare = m_pviewConditionList->m_treeUserCtrl.GetItemText(hChild);
			if (strCompare.Compare(NOMINATION_INDEX_NAME) == 0)
			hChild = m_pviewConditionList->m_treeUserCtrl.GetChildItem(hChild);
			}
			else if( stQuickList.strType.Compare("USER") == 0 )
			hChild = m_pviewConditionList->m_treeUserCtrl.GetChildItem(hRoot);
			
			  while (hChild != NULL)
			  {
			  strCompare = m_pviewConditionList->m_treeUserCtrl.GetItemText(hChild);
			  if (strFileName.Compare(strCompare) == 0)
			  {
			  hSelItem = hChild;
			  break;
			  }
			  hChild = m_pviewConditionList->m_treeUserCtrl.GetNextItem(hChild, TVGN_NEXT);
			  }
		*/
	}

	return TRUE;
}

LRESULT CMainFrmWnd::OnNotifyTabCtrl(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_CHANGETAB)
	{
		int nTabIndex = (LPARAM)lParam;
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetTabChange(nTabIndex);
	}
	else if (wParam == NM_ADDFILE)
	{
		m_stNowXMLTreeInfoFile = *(XMLTreeInfoFile*)lParam;
		
		if (m_stNowXMLTreeInfoFile.bUnitCan == TRUE)
			((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile, FALSE);
		else
		{
			CString strMessage;
			strMessage = "조합할 수 없는 조건식을 선택하였습니다.\n\n순차조건이 포함된 조건식은 조합이 불가능합니다.";
			SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		}
	}

	return TRUE;
}

LRESULT CMainFrmWnd::OnNotifyXMLTree(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_SELECT_CLASS)
	{
		m_pviewConditionList->SetTextExplain("");
		m_nNowDataType = -1;
	}
	else if (wParam == NM_SELECT_ITEM)
	{
		m_stNowXMLTreeInfo = *(XMLTreeInfo*)lParam;
		
		if(m_nSeparateForm)
		{
			((CConditionMakeWnd*)GetWnd("MAKE"))->EuBtnInit(TRUE);
			((CConditionMakeWnd*)GetWnd("MAKE"))->SetCondition(TRUE, &m_stNowXMLTreeInfo);
			((CConditionResultWnd*)GetWnd("RESULT"))->OnClickSearchBtn();
		}
		
		m_pviewConditionList->SetItemExplain(m_stNowXMLTreeInfo);

		m_nNowDataType = 0;

	}
	// (2011/1/10 - Seung-Won, Bae) Do not support NM_ADD_ITEM on [020X].
	else if (wParam == NM_ADD_ITEM && 0 == m_nSeparateForm)
	{
// 		if(m_nSeparateForm)
// 		{
// 			((CConditionMakeWnd*)GetWnd("MAKE"))->EuBtnInit();
// 		}
		
		m_stNowXMLTreeInfo = *(XMLTreeInfo*)lParam;
	
		BOOL bResult = ((CConditionMakeWnd*)GetWnd("MAKE"))->SetCondition(TRUE, &m_stNowXMLTreeInfo);
		//KHD : 죽는 현상 수정
		if(bResult)
			((CConditionMakeWnd*)GetWnd("MAKE"))->m_ctrlMapLoad.SetBase();

		if(!bResult)
			return 0L;		
		
		m_nNowDataType = 0;
		
		//강제로 Update 재무지표 순위/범위 구분
		((CConditionMakeWnd*)GetWnd("MAKE"))->m_ctrlMapLoad.EuBtnUpdate();
		m_pviewConditionList->SetItemExplain(m_stNowXMLTreeInfo);

// 		if(m_nSeparateForm)
// 		{
// 			((CConditionResultWnd*)GetWnd("RESULT"))->OnClickSearchBtn();
// 		}
	}
	else if (wParam == NM_MODIFY_ITEM)
	{
		
	}
	else if (wParam == NM_SELECT_FILE)
	{
		m_stNowXMLTreeInfoFile = *(XMLTreeInfoFile*)lParam;
		m_nNowDataType = 1;
		
		m_pviewConditionList->SetFileExplain(m_stNowXMLTreeInfoFile);
	}
	else if (wParam == NM_LOAD_FILE)
	{
		m_stNowXMLTreeInfoFile = *(XMLTreeInfoFile*)lParam;
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile);
		
		/*
		if (((CConditionMakeWnd*)GetWnd("MAKE"))->m_DataAdmin.GetConditionCnt() == 0)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile);
		else
		{
		if (m_stNowXMLTreeInfoFile.bUnitCan == TRUE)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile, FALSE);
		else
		{
		CString strMessage;
		strMessage = "조합할 수 없는 조건식을 선택하였습니다.\n\n순차조건이 포함된 조건식은 조합이 불가능합니다.\n\n선택조건으로 초기화하겠습니까?";
		if (SetMessageBox(strMessage, (CString)PROGRAM_TITLE, MB_ICONWARNING | MB_OKCANCEL) == IDOK)
		((CConditionMakeWnd*)GetWnd("MAKE"))->SetLoadFile(&m_stNowXMLTreeInfoFile);
		}
		}
		*/
		m_nNowDataType = 1;
		
		m_pviewConditionList->SetFileExplain(m_stNowXMLTreeInfoFile);
	}
	else if (wParam == NM_ADD_MARK)
	{
		XMLTreeInfo stXMLTreeInfo = *(XMLTreeInfo*)lParam;
		m_pviewConditionList->SetAddTreeMark(stXMLTreeInfo);
		m_nNowDataType = 0;
	}
	else if (wParam == NM_DEL_MARK)
	{
		XMLTreeInfo stXMLTreeInfo = *(XMLTreeInfo*)lParam;
		m_pviewConditionList->SetDelTreeMark(stXMLTreeInfo);
	}
	else if (wParam == NM_GET_PORT)
	{
		GetPortData(m_listPortInfo);
		
		CXmlTreeCtrl *ptreeXml = (CXmlTreeCtrl *)lParam;
		ptreeXml->m_plistPortInfo = &m_listPortInfo;
	}
	else if (wParam == NM_GET_LANGUAGE)
	{
		if(m_bIsFnUser)
		{
			SetInit_LibLanguage(TRUE);	// SetInit()으로 변경한다.
							// 2008.02.28목 SetInit()으로 변경해 놔서 랭귀지 정보를 로딩하지 못하고 있음. 그래서 수정함.
			GetData_LanguageList(m_listLanguageInfo);
		
			CXmlTreeCtrl *ptreeXml = (CXmlTreeCtrl *)lParam;
			ptreeXml->m_plistLanguageInfo = &m_listLanguageInfo;
		}
	}
	// 사용자 조건삭제.
	else if (wParam == NM_USERCON_DEL)
	{
		CXmlTreeCtrl *ptreeXml = (CXmlTreeCtrl *)lParam;
		HTREEITEM hSelItem = ptreeXml->GetSelectedItem();
		//if(!hSelItem)return;
		if(!hSelItem)return FALSE;

		CString strFilePath = ptreeXml->GetItemText(hSelItem);
		//if(!hSelItem) return;
		if(!hSelItem) return FALSE;

		CString strFolderPath = ptreeXml->GetItemText(ptreeXml->GetParentItem(hSelItem));

		if(FnUserConDel(strFolderPath, strFilePath))
			ptreeXml->DeleteItem(hSelItem);
	}

	return TRUE;
}

BOOL CMainFrmWnd::FnUserConDel(LPCSTR strFolderPath, LPCSTR strFilePath)
{
	BOOL bResult = FALSE;

	{
		CString szMsg;
		szMsg.Format("조건(%s)을 삭제하려 합니다.\n삭제하시겠습니까?", strFilePath);
		
		if(IDYES != ::MessageBox(m_hWnd, szMsg,PROGRAM_TITLE, MB_YESNO))
 			return FALSE;
		
	}

	CString			strUserDataPath = GetFolderPath("USER") + "\\" ;

	CString strMessage;	
	CString szFilePath = strFilePath;

	if (szFilePath.IsEmpty() == FALSE)	szFilePath += ".xml";

	POSITION			posFolder, posFile;
	XMLTreeInfoFile		stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;

	int nCount = m_listUserInfoFolder.GetCount();
	for (int nFolderIndex = 0; nFolderIndex < nCount; nFolderIndex++)
	{
		posFolder = m_listUserInfoFolder.FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_listUserInfoFolder.GetAt(posFolder);
		
		if (stXMLTreeInfoFolder.strPath == strFolderPath)
		{
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				if (szFilePath.Compare(stXMLTreeInfoFile.strFilePath) == 0)
				{
					CFile::Remove(strUserDataPath + strFolderPath + "\\" + szFilePath);
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAt(posFile);
					bResult = TRUE;
					break;
				}
			}
			
			stXMLTreeInfoFolder.nFileCnt--;
			m_listUserInfoFolder.SetAt(posFolder, stXMLTreeInfoFolder);
		}
		
		if (bResult == TRUE)
			break;
	}
	return bResult;
}

void NotifySystemTradingLib(CPtrList* pList, long lResv)
{
	POSITION pos = pList->GetHeadPosition();
	while(pos)
	{
		// 전략 스크립트 정보
		//ST_SCRIPTINFO* pScriptInfo = (ST_SCRIPTINFO*)pList->GetNext(pos);
	}
	
	SendMessage(g_hMainFrmWnd, WMU_REFRESH_INDEXTREE, 0, 0);
}

void CMainFrmWnd::SetInit_LibLanguage(BOOL bInit)
{
	if (bInit == TRUE)
	{	
		CString strAppPath;
		strAppPath.Format("%s", Variant(homeCC));

		CString strLibPath;
		strLibPath = strAppPath + "\\dev\\SystemTradingExtLib.dll";

		if(m_hLanguageMng==NULL)
		{
 			m_hLanguageMng = ::LoadLibrary(strLibPath);
			
			if (m_hLanguageMng != NULL)
			{
				Lib_InitExtLib = (EX_InitExtLib)::GetProcAddress(m_hLanguageMng, "EX_InitExtLib");
				Lib_ExitExtLib = (EX_ExitExtLib)::GetProcAddress(m_hLanguageMng, "EX_ExitExtLib");
				Lib_GetScriptList = (EX_GetScriptList)::GetProcAddress(m_hLanguageMng, "EX_GetScriptList");
				Lib_GetScriptInfo = (EX_GetScriptInfo)::GetProcAddress(m_hLanguageMng, "EX_GetScriptInfo");
				Lib_ReloadScriptInfo = (EX_ReloadScriptInfo)::GetProcAddress(m_hLanguageMng, "EX_ReloadScriptInfo");			
				
				if (Lib_InitExtLib(NotifySystemTradingLib, (LPSTR)(LPCTSTR)strAppPath) == FALSE)
				{
					if(m_nSeparateForm == 0)
						::MessageBox(m_hWnd, "SystemTradingExtLib.lib 실패!, 고객센터로 문의하십시오.", PROGRAM_TITLE, MB_OK);
					else
						::MessageBox(m_hWnd, "SystemTradingExtLib.lib 실패!, 고객센터로 문의하십시오.", PROGRAM_TITLE_BRIGHT, MB_OK);
				}
			}
		}
	}
	else
	{
		if (m_hLanguageMng != NULL)
		{
			Lib_ExitExtLib(NotifySystemTradingLib);
			
			FreeLibrary(m_hLanguageMng); 
			m_hLanguageMng = NULL;
		}
	}
}

CString CMainFrmWnd::GetData_LanguageScript(CString strLanguageName)
{
	CString strResult = "";
	BYTE *pstrResult = NULL;

// 	char szCurrentPath[MAX_PATH];
// 	char szTargetPath[MAX_PATH];
// 	GetCurrentDirectory(MAX_PATH, szCurrentPath);
// 	lstrcpy(szTargetPath, szCurrentPath);
// 	lstrcat(szTargetPath, "\\Chart");
// 	SetCurrentDirectory(szTargetPath);
// 	if (Lib_GetScriptInfo((LPSTR)(LPCTSTR)strLanguageName, pstrResult) == FALSE)
// 		strResult = "";
// 
// 	strResult = (LPCSTR)pstrResult;
// 	if(!pstrResult) 
// 		delete [] pstrResult;

// 	SetCurrentDirectory(szCurrentPath);
// 	CString strResult = "";
// 	char szCurrentPath[MAX_PATH];
// 	char szTargetPath[MAX_PATH];
// 	GetCurrentDirectory(MAX_PATH, szCurrentPath);
// 	lstrcpy(szTargetPath, szCurrentPath);
// 	lstrcat(szTargetPath, "\\Chart");
// 	SetCurrentDirectory(szTargetPath);
	if (Lib_GetScriptInfo((LPSTR)(LPCTSTR)strLanguageName, strResult) == FALSE)
		strResult = "";
	//SetCurrentDirectory(szCurrentPath);

	return strResult;
}

BOOL CMainFrmWnd::GetData_LanguageList(CList	<LanguageInfo, LanguageInfo&> &listLanguageInfo)
{
	BOOL bResult = TRUE;
	
	//# Language
	listLanguageInfo.RemoveAll();
	if (m_hLanguageMng != NULL)
	{
		CPtrList* pList = Lib_GetScriptList();
		if(pList)
		{
			POSITION pos = pList->GetHeadPosition();
			while (pos)
			{
				// 전략 스크립트 정보
				try
				{
					LanguageInfo stScriptInfo = *(LanguageInfo*)pList->GetNext(pos);
					listLanguageInfo.AddTail(stScriptInfo);
				}
				catch(...)
				{
					return FALSE;
				}
			}
		}
		else
		{
			TRACE("CMainFrmWnd::GetData_LanguageList(..)에서 Lib_GetScriptList가 NULL인경우 발생\n");;
		}
	}
	else
		bResult = FALSE;
	
	return bResult;
}

void CMainFrmWnd::GetPortData(CList <PortGroupInfo, PortGroupInfo&> &listPortInfo)
{
	if (listPortInfo.GetCount() > 0)
		return;
	
	listPortInfo.RemoveAll();
	
	CString strTemp;
	int nType = 0, nItemCnt;
	LPCSTR szGroup=NULL;
	CStringArray arrKey, arrName, arrCode, arrItemName;

	GetData_PortGroup(arrKey, arrName);

	for(int nGroupCnt = 0; nGroupCnt < arrKey.GetSize(); ++nGroupCnt)
	{
		arrCode.RemoveAll();
		arrItemName.RemoveAll();
		PortGroupInfo stPortGroupInfo;
		stPortGroupInfo.nNo = nGroupCnt;
		strTemp = arrName.GetAt(nGroupCnt);
		stPortGroupInfo.strName.Format("%s %s", arrKey.GetAt(nGroupCnt), strTemp);	//[0000052:] 관심그룹 관리를 위해 키값도 이름에 포함시킴.
		strTemp = strTemp.Mid(0, strTemp.Find(' '));
#if(0)
		GetData_PortItem(arrCode, arrItemName, _ttoi(arrKey.GetAt(nGroupCnt)));

		if (arrCode.GetSize() >= (int)MAX_PORT_ITEM)
			nItemCnt = (int)MAX_PORT_ITEM;
		else
			nItemCnt = arrCode.GetSize();

		stPortGroupInfo.nItemCnt = nItemCnt;

		for(int j = 0; j < nItemCnt; ++j)
		{
			strTemp = arrCode.GetAt(j);
			stPortGroupInfo.strCode[j] = strTemp;
		}

		listPortInfo.AddTail(stPortGroupInfo);
#endif
	}
}

void CMainFrmWnd::OnNotify_PortPolio(WPARAM wParam, LPARAM lParam)
{
	//# Language List ReLoad! => List Sync
	m_pviewConditionList->m_treeCommonCtrl.SetReloadLanguage();
	m_pviewConditionList->m_treeCommonCtrl_Sort.SetReloadLanguage();
	
	//# Alert => X Language!

	// 관심그룹 다시 Load
	m_pviewConditionList->m_treeCommonCtrl.SetReloadPortPolio();
}

//extern int Dll_SetRegKwCombo(CWnd* pWnd, int nType);  //# nType 1-추가, 0-삭제	-hidden
void CMainFrmWnd::SetInit_LibPortPolio(BOOL bInit)
{
//	Dll_SetRegKwCombo(this, (bInit == TRUE)?1:0);
}

LRESULT CMainFrmWnd::OnNotify_Language(WPARAM wParam, LPARAM lParam)
{
	//# Language List ReLoad! => List Sync
	m_pviewConditionList->m_treeCommonCtrl.SetReloadLanguage();
	m_pviewConditionList->m_treeCommonCtrl_Sort.SetReloadLanguage();
	
	//# Alert => X Language!

	return TRUE;
}

void CMainFrmWnd::SetReloadUserPatten()
{
	//# Patten List ReLoad! => List Sync
	m_pviewConditionList->m_treeCommonCtrl.SetReloadUserPatten();
	m_pviewConditionList->m_treeCommonCtrl_Sort.SetReloadUserPatten();
	
	//# Alert => X Patten!
}

int CMainFrmWnd::GetFixFieldCnt()
{
	int nResult = 0;
	nResult = ((CConditionResultWnd*)GetWnd("RESULT"))->m_listFixHeadUser.GetCount();
	
	return nResult;
}

int CMainFrmWnd::GetMoveFieldCnt()
{
	int nResult = 0;
	nResult = ((CConditionMakeWnd*)GetWnd("MAKE"))->m_DataAdmin.GetMoveFieldTotCnt();		//# Common Group
	nResult += ((CConditionMakeWnd*)GetWnd("MAKE"))->m_DataAdmin.GetMoveFieldTotCnt(FALSE);		//# Order Group
	
	return nResult;
}

BOOL CMainFrmWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if(!m_ctrlSplitter.CreateStatic(this, 1, 2)) return FALSE;

	m_ctrlSplitter.CreateView(0, 0, RUNTIME_CLASS(CConditionListView), CSize(0, 0), pContext);
	m_ctrlSplitter.CreateView(0, 1, RUNTIME_CLASS(CConditionView), CSize(0, 0), pContext);

	m_pviewConditionList = (CConditionListView *)m_ctrlSplitter.GetPane(0, 0);
	m_pviewCondition = (CConditionView *)m_ctrlSplitter.GetPane(0, 1);

	m_ctrlSplitter.m_nResizeRow = -1;
	m_ctrlSplitter.m_bFirstSizing = true;
	m_ctrlSplitter.m_upBorder = 0;
	m_ctrlSplitter.SetSplitBarCx(3);
	m_ctrlSplitter.RecalcLayout();

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//--> HTS구분없앰. 사용하면 추후 다시
//	{
//		CString strHTS = KSLink_GetLoginInfo("ISHTS");
//		g_bSSIsHTS = (strHTS.Compare("1")==0)? TRUE : FALSE;
//	}
//<--

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_ctrlMakeQuickBar.Create(_T(""), WS_CHILD,// | WS_VISIBLE | WS_BORDER, 
		CRect(0, 0, 0, 0), this);
	
	COLORREF crBg = COLOR_BUTTON;
	COLORREF crText = COLOR_BLACK;
	
	m_btnMakeQuick.Create(_T("사용자조건 퀵버튼"), 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,//WS_BORDER, 
		CRect(0, 0, 0, 0), 
		this,	
		IDC_USERQUICK_BTN);

	//m_btnMakeQuick.SetFont(&m_fontDefault);
	
// 	m_tabQuick.Create(TCS_TABS | TCS_BUTTONS | WS_CHILD | WS_VISIBLE | TCS_TOOLTIPS, 
// 		CRect(0,0,0,0), this, IDC_QUICK_TAB);
// 	m_tabQuick.SetFont(&m_fontDefault);
// 	m_tabQuick.SetBkgndColor(COLOR_QUICK_CANVAS);
// 	m_tabQuick.ShowWindow(SW_HIDE);
	
	
	AfxSetResourceHandle(hInstSave);
	
	SetLoadXml();

	/*return 0;*/
	m_Draw3DRectangle.SetFontStyle();
	m_Draw3DRectangle.SetOuterLine(FALSE);
	m_Draw3DRectangle.SetTransParent(FALSE);
	m_Draw3DRectangle.SetGradation(FALSE);
	m_Draw3DRectangle.SetFlatOutput(FALSE);
	m_Draw3DRectangle.SetFrameType();
	m_Draw3DRectangle.SetBackColor(COLOR_QUICK_CANVAS);
	
	return 0;
}

void CMainFrmWnd::SetInitFormCode(CString szFormCode)
{
	m_szScreenKey = szFormCode;

	if(!(szFormCode.Compare("IB712700")))
	{
		m_nSeparateForm = 0;
	}
	else
	{
		m_nSeparateForm = 1;
	}
}

void CMainFrmWnd::SetInit(CString szFormCode)
{
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	
	SetInit_LibPortPolio(TRUE);
	if(m_bIsFnUser) SetInit_LibLanguage(TRUE);

	if (m_bFirstState == TRUE)
	{
		CString strFile;
		strFile = GetFilePath("FINDERINFO");
		GetLoadWindowState(strFile);
		m_bFirstState = FALSE;
	}
	
	CRect reClient;
	GetClientRect(reClient);
	int nSplitterPos = m_nSplitterPos;		//0000145: 조건검색 좌측탭 Split 조정위치 기억기능 추가
	OnSize_Splitter(reClient, nSplitterPos, TRUE);
	
	if (m_pviewConditionList != NULL)
		m_pviewConditionList->SetInit(m_nSeparateForm);

	if (m_bLeftExp == FALSE)
		SetLeftExpend(m_bLeftExp);		

// 	if (m_pviewConditionList != NULL)
// 		m_pviewConditionList->SetInit();
	
	if (m_ctrlSplitter.GetSafeHwnd() != NULL)
	{
		m_pviewCondition->SetInit();
		/*m_pviewCondition->SetTopExpend(m_bTopExp);*/
		m_pviewCondition->SetTopExpend(m_nTopExp);
	}

//	ChangeSkinColor();

	SetToolTip();
	
	if(m_nSeparateForm)
	{
 		SetSelectCondition();
// 		((CConditionResultWnd*)GetWnd("RESULT"))->OnClickSearchBtn();
	}

	((CConditionResultWnd*)GetWnd("RESULT"))->SetBtnExpandImage();
	((CConditionMakeWnd*)GetWnd("MAKE"))->SetBtnExpandImage();
}

void CMainFrmWnd::SetSelectCondition()
{
	m_pviewConditionList->SetSelectItem();
// 	
// 	
// 	HTREEITEM hTreeItem  = NULL;
// 	HTREEITEM hTreeChild = NULL;
// 	HTREEITEM hTreeTemp	 = NULL;
// 	int nIndex = 0;
// 	int j = 0;
// 	int k = 0;
// 
// 	switch(m_nSeparateForm)
// 	{
// 		case 200 :	nIndex = 3;	j = 0; k = 0;	break;
// 		case 201 :	nIndex = 3; j = 1; k = 1;	break;
// 		case 202 :	nIndex = 4;	j = 0; k = 0; 	break;
// 		case 203 :	nIndex = 4; j = 3; k = 0;	break;
// 		case 204 :	nIndex = 4;	j = 4; k = 0;	break;
// 		case 205 :	nIndex = 4;	j = 5; k = 0;	break;
// 		case 206 :	nIndex = 4; j = 6; k = 0;	break;
// 		case 207 :	nIndex = 4;	j = 7; k = 0;	break;
// 		case 208 :	nIndex = 5;	j = 0; k = 0;	break;
// 		case 209 :	nIndex = 6;	j = 0; k = 0;	break;
// 	}
// 
// 	hTreeItem = m_pviewConditionList->m_treeCommonCtrl.GetFirstVisibleItem();
// 	for(int i = 0; i < nIndex; i++)
// 	{
// 		hTreeItem = m_pviewConditionList->m_treeCommonCtrl.GetNextSiblingItem(hTreeItem);
// 	}
// 
// 	CString str = m_pviewConditionList->m_treeCommonCtrl.GetItemText(hTreeItem);
// 
// 	hTreeChild = m_pviewConditionList->m_treeCommonCtrl.GetChildItem(hTreeItem);
// 	for(i = 0; i < j; i++)
// 	{
// 		hTreeTemp = m_pviewConditionList->m_treeCommonCtrl.GetNextSiblingItem(hTreeChild);
// 		hTreeChild = hTreeTemp;
// 	}
// 	str = m_pviewConditionList->m_treeCommonCtrl.GetItemText(hTreeChild);
// 
// 	hTreeChild = m_pviewConditionList->m_treeCommonCtrl.GetChildItem(hTreeChild);
// 	for(i = 0; i < k; i++)
// 	{
// 		hTreeTemp = m_pviewConditionList->m_treeCommonCtrl.GetNextSiblingItem(hTreeChild);
// 		hTreeChild = hTreeTemp;
// 	}
// 	str = m_pviewConditionList->m_treeCommonCtrl.GetItemText(hTreeChild);
// 
// 	while(hTreeTemp = m_pviewConditionList->m_treeCommonCtrl.GetChildItem(hTreeChild))
// 	{
// 		hTreeChild = hTreeTemp;
// 	}
// 	str = m_pviewConditionList->m_treeCommonCtrl.GetItemText(hTreeChild);
// 
// 	m_pviewConditionList->m_treeCommonCtrl.SelectTreeItemCondition(hTreeChild);
// 	m_pviewConditionList->m_treeCommonCtrl.Invalidate();
}

void CMainFrmWnd::SetToolTip()
{	

}

void CMainFrmWnd::OnDestroy() 
{
	SetInit_LibPortPolio(FALSE);
	SetInit_LibLanguage(FALSE);
	
	//# Result Xml File Save!
	if (m_bLoadFile_User == TRUE)
		SetSaveTreeXml("USER", &m_listUserInfoFolder);
	
	if (m_bLoadFile_Sys == TRUE)
		SetSaveTreeXml("SYS", &m_listSysInfoFolder);
	
	SetSaveQuickXml(&m_listQuickInfo);
	
	SetDeleteFolderList(&m_listUserInfoFolder);
	SetDeleteFolderList(&m_listSysInfoFolder);
	m_listQuickInfo.RemoveAll();
	
	CString strFile;
	strFile = GetFilePath("FINDERINFO");
	SetSaveWindowState(strFile);

	if(m_nSeparateForm)
		SaveSelectedCondition();
	
//	if (m_pctrlSplitter != NULL)
//	{
//		m_pctrlSplitter->DestroyWindow();
//		delete m_pctrlSplitter;
//		m_pctrlSplitter = NULL;
//	}
	if (m_ctrlSplitter.GetSafeHwnd() != NULL)
	{
		m_pviewConditionList->SendMessage(WM_DESTROY, NULL, NULL);
		
		m_ctrlSplitter.DestroyWindow();
	}
	
	CFrameWnd::OnDestroy();
}

void CMainFrmWnd::SetSaveTreeXml_User(BOOL bTreeReload)
{
	SetSaveTreeXml("USER", &m_listUserInfoFolder);
	
	if (bTreeReload == TRUE)
		m_pviewConditionList->SerTreeReload();
}

void CMainFrmWnd::SetDeleteFolderList(CList	<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	POSITION posFolder;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	for (int nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
		
		stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
		delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
	}
	pFolderList->RemoveAll();
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
//	dc.FillSolidRect(reClient, COLOR_MAIN_CANVAS);
	dc.FillSolidRect(reClient, RGB(213, 213, 213));		// KB투자증권
	
	if (m_ctrlMakeQuickBar.GetSafeHwnd() != NULL)
	{
		CRect reQuick;
		m_ctrlMakeQuickBar.GetWindowRect(reQuick);
		ScreenToClient(reQuick);
		dc.FillSolidRect(reQuick, COLOR_QUICK_CANVAS);
		
		m_Draw3DRectangle.m_rcWnd = reQuick;
		m_Draw3DRectangle.DrawTitleFrame(&dc);
	}
}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
}

void CMainFrmWnd::OnSize_Splitter(CRect reClient, int nSplitterPos, BOOL bInvalidate)
{
	//if (m_pctrlSplitter == NULL)	return;
	if (m_ctrlSplitter.GetSafeHwnd() == NULL)	return;
	
	int nWidth = 188;
	if (nWidth > nSplitterPos)
		nSplitterPos = nWidth;
	
//	m_pctrlSplitter->SetColumnInfo(0, nSplitterPos, 0);
//	m_pctrlSplitter->SetColumnInfo(1, (reClient.right - reClient.left) - nSplitterPos, 0);
//	
//	if (bInvalidate == TRUE)
//		m_pctrlSplitter->RecalcLayout();

	m_ctrlSplitter.SetColumnInfo(0, nSplitterPos, 0);
	m_ctrlSplitter.SetColumnInfo(1, (reClient.right - reClient.left) - nSplitterPos , 0);
	
	if (bInvalidate == TRUE)
		m_ctrlSplitter.RecalcLayout();
}

void CMainFrmWnd::OnSize(CRect reClient)
{
	reClient.top += 3;
	reClient.bottom -= 4;
	reClient.left += 4;
	reClient.right -= 4;
	
	CRect reLeft, reTemp;
	reLeft = reClient;
	
	
	
	
	//OnSize_Splitter(reClient, 188, TRUE);
	/*
	m_ctrlMakeQuickBar;		//Quick Button Bar
	m_btnMakeQuick;			//사용자조건 퀵버튼
	m_tabQuick;				//사용자조건 QuickTab
	*/
}

BOOL CMainFrmWnd::SetSaveWindowState(CString strFile)
{
	BOOL bWrite = TRUE;
	
	TRY
	{
		CString strData;
		strData = (m_pviewConditionList->m_bTreeHelpExp == TRUE) ? "1" : "0";
		WritePrivateProfileString("WindowState", "TreeHelpExp", strData, strFile);
		
		// 20100320 박찬모 >>
		/*strData = (m_bTopExp == TRUE) ? "1" : "0";*/
		strData.Format("%d", m_nTopExp);
		if(!m_nSeparateForm)
			WritePrivateProfileString("WindowState", "TopExp", strData, strFile);
		else
			WritePrivateProfileString("WindowState_0200", "TopExp", strData, strFile);
		// 20100320 박찬모 <<

		/*strData.Format("%d", m_nTopExp);*/
		strData = (m_bLeftExp == TRUE) ? "1" : "0";
		if(!m_nSeparateForm)
			WritePrivateProfileString("WindowState", "LeftExp", strData, strFile);
		else
			WritePrivateProfileString("WindowState_0200", "LeftExp", strData, strFile);
		
		/*strData = "1";*/
		/*WritePrivateProfileString("WindowState", "LeftExp", strData, strFile);*/

		//>>0000145: 조건검색 좌측탭 Split 조정위치 기억기능 추가
		int cxCur,cxMin;
		m_ctrlSplitter.GetColumnInfo(0, cxCur,cxMin);
		if(m_bLeftExp == TRUE)
			m_nSplitterPos = cxCur;
		strData.Format("%d", m_nSplitterPos);
		
		if(!m_nSeparateForm)
			WritePrivateProfileString("WindowState", "SplitterPos", strData, strFile);
		else
			WritePrivateProfileString("WindowState_0200", "SplitterPos", strData, strFile);
		//<<0000145
	}
	CATCH (CFileException, pEx)
	{
		SetSendMainMsg("화면 설정 Data 저장 오류!");
		bWrite = FALSE;
	}
	END_CATCH
		
		return bWrite;
}

BOOL CMainFrmWnd::GetLoadWindowState(CString strFile)
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	char chData[10];
	
	bRead = cFind.FindFile(strFile);
	if (!bRead)
	{
		CString strMsg = strFile + "을 찾지 못했습니다.";
		SetSendMainMsg(strMsg);
	}
	else
	{
		GetPrivateProfileString("WindowState", "TreeHelpExp", "0", chData, sizeof(chData), strFile);
		m_pviewConditionList->SetHelpExp((atoi(chData) == 1) ? TRUE : FALSE);
		
		if(!m_nSeparateForm)
			GetPrivateProfileString("WindowState", "TopExp", "0", chData, sizeof(chData), strFile);
		else
			GetPrivateProfileString("WindowState_0200", "TopExp", "0", chData, sizeof(chData), strFile);
		/*m_bTopExp = (atoi(chData) == 1) ? TRUE : FALSE;*/
		m_nTopExp = atoi(chData);
		
		if(!m_nSeparateForm)
			GetPrivateProfileString("WindowState", "LeftExp", "0", chData, sizeof(chData), strFile);
		else
			GetPrivateProfileString("WindowState_0200", "LeftExp", "0", chData, sizeof(chData), strFile);
		
		/*GetPrivateProfileString("WindowState", "LeftExp", "0", chData, sizeof(chData), strFile);*/
		m_bLeftExp = (atoi(chData) == 1) ? TRUE : FALSE;

		//>>0000145: 조건검색 좌측탭 Split 조정위치 기억기능 추가
		if(!m_nSeparateForm)
			GetPrivateProfileString("WindowState", "SplitterPos", "0", chData, sizeof(chData), strFile);
		else
			GetPrivateProfileString("WindowState_0200", "SplitterPos", "0", chData, sizeof(chData), strFile);
		m_nSplitterPos = atoi(chData);
		if(m_nSplitterPos<0) m_nSplitterPos=0;
		//<<0000145
	}
	
	return bRead;
}

void CMainFrmWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
}

void CMainFrmWnd::OnDisplayToolTip(NMHDR *pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT *pToolText = (TOOLTIPTEXT *)pNMHDR;
	
	if(pToolText == NULL)
		return;
	
// 	if(m_tabQuick.m_strToolMsg == "")
// 		return;
		
// 	UpdateData();
// 	::lstrcpy(pToolText->szText, m_tabQuick.m_strToolMsg);
}

void CMainFrmWnd::SetControlState(BOOL bState)
{
	return;
	
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

void CMainFrmWnd::SetLoadXml()
{
	SetJobFolder();
	
	if(m_nSeparateForm == 0)
	{
		m_bLoadFile_User = SetLoadTreeXml("USER");
		SetLoadTreeXml("EXCAVATION");
	}
		
	else if(m_nSeparateForm == 1)
		SetLoadTreeXml("EXCAVATION");

	//m_bLoadFile_Sys = SetLoadTreeXml("SYS");
	//SetLoadQuickXml();
	
	//m_tabQuick.SetInitTab(&m_listQuickInfo);
}

void CMainFrmWnd::SetJobFolder()
{
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	
	CFileFind finder;
	CString strUserFolder = GetFolderPath("USER");
	CString strFindFolder;
	for (int nIndex = strUserFolder.GetLength() - 1; nIndex > 0; nIndex--)
	{
		if (strUserFolder[nIndex] == '\\')
		{
			strFindFolder = strUserFolder.Mid(0, nIndex);
			break;
		}
	}      
	
	BOOL bFoldMake = TRUE;
	BOOL bFoldMake_Temp = TRUE;
	BOOL bWorking = finder.FindFile(strFindFolder + "\\*.*");
	CString strGetPath = _T("");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		
		if (finder.IsDots())
			continue;
		
		if (finder.IsDirectory())
		{
			strGetPath = finder.GetFilePath();
			if (strGetPath == strUserFolder)
				bFoldMake = FALSE;
			
			strGetPath = finder.GetFilePath();
			if (strGetPath == strUserFolder + "\\" + (CString)FOLDER_TEMPFILE)
			{
				bFoldMake_Temp = FALSE;
				break;
			}
		}
		continue;
	}  

	if (bFoldMake == TRUE)
	{
		CreateDirectory(strFindFolder, 0);
		CreateDirectory(strUserFolder, 0);
	}
	
	if (bFoldMake_Temp == TRUE)
		CreateDirectory(strUserFolder + "\\" + (CString)FOLDER_TEMPFILE, 0);
}

BOOL CMainFrmWnd::SetLoadQuickXml()
{	
	SetControlState(FALSE);
	
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	CString strXmlFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + (CString)QUICK_FILE;
	
	char msg[500];
	CString strDebug;
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	pXmlDom->async = false;
	if (!pXmlDom->load (strXmlFilePath.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		strDebug.Format("[%s] 파일 읽기 오류", strXmlFilePath);
		SetSendMainMsg(strDebug);
		
		SetControlState(TRUE);
		return FALSE;
	}
	
	SyncQuickList(strXmlFilePath);
	
	SetControlState(TRUE);
	return TRUE;
}

void CMainFrmWnd::SyncQuickList(CString strXmlFilePath)
{
	CList<QuickList, QuickList> listQuickInfo;
	SetLoadQuickList(strXmlFilePath, listQuickInfo);
	
	POSITION pos;
	QuickList	stQuickList;
	CString strFolderPath, strFilePath;
	for (int nIndex = 0; nIndex < listQuickInfo.GetCount(); nIndex++)
	{
		pos = listQuickInfo.FindIndex(nIndex);
		stQuickList = listQuickInfo.GetAt(pos);
		
		strFolderPath = stQuickList.strFolderPath;
		strFilePath = stQuickList.strFilePath;
		if (stQuickList.strType.Compare("USER") == 0)
		{
			if (GetFindFile(strFolderPath, strFilePath, &m_listUserInfoFolder) == TRUE)
				m_listQuickInfo.AddTail(stQuickList);
		}
		else if (stQuickList.strType.Compare("SYS") == 0)
		{
			if (GetFindFile(strFolderPath, strFilePath, &m_listSysInfoFolder) == TRUE)
				m_listQuickInfo.AddTail(stQuickList);
		}
	}
	
	SetSaveQuickXml(&m_listQuickInfo);
	listQuickInfo.RemoveAll();
}

BOOL CMainFrmWnd::SetLoadQuickList(CString strXmlFilePath, 
								   CList<QuickList, QuickList> &listQuickInfo)
{
	BOOL bResult = TRUE;
	listQuickInfo.RemoveAll();
	
	MSXML::IXMLDOMNodePtr					pNIndex;
	MSXML::IXMLDOMNodeListPtr			pNSCondition;
	MSXML::IXMLDOMNodePtr					pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	VARIANT_BOOL bLoadedXMLFile;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;

	//20120112 이문수 파일존재여부 선행검사 >>	
	CFileFind ff;
	if(!ff.FindFile(strXmlFilePath))		
		return FALSE;	
	//20120112 이문수 파일존재여부 선행검사 <<

	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strXmlFilePath));
	
	pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
	if(pNIndex == NULL) return FALSE;

	//try 
	{
		pNSCondition = pNIndex->selectNodes("CONDITION");
		if(pNSCondition == NULL) return FALSE;

		int nItemCnt = pNSCondition->length;
		QuickList  stQuickList;
		for (int nIndex = 0; nIndex < nItemCnt; nIndex++)
		{
			pNCondition = pNSCondition->item[nIndex];
			if(pNCondition == NULL) return FALSE;

			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNACondition = pNCondition->attributes;
			if(pNACondition == NULL) return FALSE;

			pNAttributeTemp = pNACondition->getNamedItem("NAME");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.strFileName = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("QUICK_NAME");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.strQuickName = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("FILE_PATH");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.strFilePath = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("FOLDER_PATH");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.strFolderPath = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("TYPE");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.strType = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNACondition->getNamedItem("REGIST_CHECK");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.bCheck = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			pNAttributeTemp = pNACondition->getNamedItem("ALERTREG");
			if(pNAttributeTemp == NULL) return FALSE;
			stQuickList.bAlertReg = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			listQuickInfo.AddTail(stQuickList);
		}
	}
	//catch(_com_error e)
	//{
	//	bResult = FALSE;
	//}
	
	return TRUE;
}

BOOL CMainFrmWnd::SetSaveQuickXml(CList<QuickList, QuickList> *plistQuickInfo)
{
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	CString strXmlFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + (CString)QUICK_FILE;
	
	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr						pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAFolder;
	
	MSXML::IXMLDOMNodePtr						pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr		pNACondition;
	
	CString strCheck;
	POSITION pos;
	QuickList stQuickList;
	
	BOOL bResult = TRUE;
	
	try 
	{
		MSXML::IXMLDOMDocumentPtr pDOMSysIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr		pPI;
		MSXML::IXMLDOMNamedNodeMapPtr						pNAPI;
		MSXML::IXMLDOMNodePtr										pNEncoding;
		
		pPI = pDOMSysIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMSysIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		pDOMSysIndex->appendChild(pPI);
		
		pNIndex = pDOMSysIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttribute;
		pNAttribute = pDOMSysIndex->createAttribute("VERSION");
		pNAttribute->text = "1.0";
		pNAIndex->setNamedItem(pNAttribute);
		
		pDOMSysIndex->appendChild(pNIndex);
		
		QuickList  stQuickList;
		int nCnt = plistQuickInfo->GetCount();
		for(int nItemNo = 0; nItemNo < nCnt; nItemNo++)
		{
			pos = plistQuickInfo->FindIndex(nItemNo);
			stQuickList = plistQuickInfo->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;
			
			pNCondition = pDOMSysIndex->createElement("CONDITION");
			pNIndex->appendChild(pNCondition);
			pNACondition = pNCondition->attributes;
			
			pNAttribute = pDOMSysIndex->createAttribute("NAME");
			pNAttribute->text = (LPCTSTR)stQuickList.strFileName;
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("QUICK_NAME");
			pNAttribute->text = (LPCTSTR)stQuickList.strQuickName;
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("FILE_PATH");
			pNAttribute->text = (LPCTSTR)stQuickList.strFilePath;
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("FOLDER_PATH");
			pNAttribute->text = (LPCTSTR)stQuickList.strFolderPath;
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("TYPE");
			pNAttribute->text = (LPCTSTR)stQuickList.strType;
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("REGIST_CHECK");
			pNAttribute->text = (stQuickList.bCheck == TRUE)?"TRUE":"FALSE";
			pNACondition->setNamedItem(pNAttribute);
			
			pNAttribute = pDOMSysIndex->createAttribute("ALERTREG");
			pNAttribute->text = (stQuickList.bAlertReg == TRUE)?"TRUE":"FALSE";
			pNACondition->setNamedItem(pNAttribute);
		}
		pDOMSysIndex->save(_bstr_t(strXmlFilePath));
	}
	catch(_com_error e) 
	{
		bResult = FALSE;
	}
	
	return bResult;
}

CString CMainFrmWnd::GetUserFilePath(CString strOnlyFileName)
{
	CString strFilePath = "";
	CString strUserPath;

	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));

	strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strOnlyFileName;
	return strFilePath;
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";
	
	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;
	
	if (strLoadType == "USER") //# User
	{
		strFile = (CString)TREE_USER_FILE;
		
		bUserFolder = TRUE;
	}
	else if (strLoadType == "SYS") //# Recommend
		strFile = (CString)TREE_RECOMMEND_FILE;
	else if (strLoadType == "FIX_USER")
	{
		if(m_nSeparateForm == 0)
			strFile = (CString)FIXFIELD_USER_FILE;
		else
			strFile = (CString)FIXFIELD_USER_EXCAVATION;
		bUserFolder = TRUE;
	}
	else if (strLoadType == "FIX_SYS")
		strFile = (CString)FIXFIELD_SYS_FILE;
	else if (strLoadType == "MOVE_USER")
		strFile = (CString)MOVEFIELD_USER_FILE;
	else if (strLoadType == "MOVE_SYS")
		strFile = (CString)MOVEFIELD_SYS_FILE;
	else if (strLoadType == "EXCAVATION")
		strFile = (CString)TREE_EXCAVATION_FILE;
	else if (strLoadType == "CNADLE")
	{
		strFile = (CString)CANDLE_INDEX_FILE;
		bSetIndustry = FALSE;
	}
	else if (strLoadType == "FINDERINFO")
		strFile = (CString)FINDERINFO_INIFILE;
	
	if ((m_bIndustry == TRUE) && (bSetIndustry == TRUE))
		strFile.Replace(".", "_Industry.");
	
	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
	{
		CString strPath;
		strPath.Format("%s", Variant(homeCC));
		strFilePath = strPath + (CString)FOLDER_DATA + "\\" + strFile;
	}
	
	return strFilePath;
}

CString CMainFrmWnd::GetFolderPath(CString strLoadType)
{
	CString strFolderPath = "", strFolder;
	
	BOOL bUserFolder = FALSE;
	if (strLoadType == "SYS")			//# 2 : Recommend
	{
		strFolder = (CString)FOLDER_SYSFILE; 
		/*
		if (m_bIndustry == TRUE)
		strFolder += "_Industry";
		*/
	}
	else if (strLoadType == "USER")		//# 3 : user
	{
		strFolder = (CString)FOLDER_USERFILE;

		if (m_bIndustry == TRUE)
			strFolder += "_Industry";
		
		bUserFolder = TRUE;
	}
	else if (strLoadType == "LANGUAGE")		//# 4 : Language
		strFolder = (CString)FOLDER_LANGUAGEFILE;
	else if	(strLoadType == "EXCAVATION")
		strFolder = (CString)FOLDER_EXCAVATION;
	
	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		strFolderPath = (CString)strUserPath + strFolder;
	}
	else
	{
		CString strPath;
		strPath.Format("%s", Variant(homeCC));
		strFolderPath = strPath + strFolder;
	}
	
	return strFolderPath;
}

BOOL CMainFrmWnd::SetLoadTreeXml(CString strType)
{	
	
//	SetControlState(FALSE);
	
	CString strFilePath;
	strFilePath = GetFilePath(strType);
	
	char msg[500];
	CString strDebug;
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	CString strErrorMsg;
	pXmlDom->async = false;
	if (!pXmlDom->load (strFilePath.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		if (strType == "SYS")
		{
			CString strTemp(BSTR(pParsingErr->reason));	
			strDebug = strTemp + CString(msg) + "\n" + strFilePath;
			
			strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
			SetSendMainMsg(strErrorMsg);
			
//			SetControlState(TRUE);
			return FALSE;
		}
		else
			SetSaveTreeXml("USER", &m_listUserInfoFolder);
	}
	
	if (strType == "USER")
		SyncTreeListFile(strFilePath);
	else if (strType == "SYS")
	{
		if (GetXMLToList(strFilePath, strType, &m_listSysInfoFolder) == FALSE)
		{
			strErrorMsg.Format("%s Tree List File Load 실패!", strType);
			SetSendMainMsg(strErrorMsg);
		}
	}
	else if(strType == "EXCAVATION")
	{
		SyncExcavationTreeListFile(strFilePath);
	}
	
//	SetControlState(TRUE);
	return TRUE;
}

void CMainFrmWnd::SyncTreeListFile(CString strFilePath)
{
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	
	CString strErrorMsg;
	if (GetXMLToList(strFilePath, "USER", &m_listUserInfoFolder) == FALSE)
	{
		strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
		SetSendMainMsg(strErrorMsg);
	}
	
	//# Get Real File list #
	CList<XMLTreeInfoFolder, XMLTreeInfoFolder&>    listFolderList;
	CString strFolderPath = GetFolderPath("USER");
	
	GetPathToList(strFolderPath, &listFolderList);
	ProcessDeleteFolder(&m_listUserInfoFolder, &listFolderList);
	ProcessInsertFolder(&listFolderList, &m_listUserInfoFolder);
	
	SetDeleteFolderList(&listFolderList);
	listFolderList.RemoveAll();
	
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	POSITION posFolder;
	for (int nFolderIndex = 0; nFolderIndex < m_listUserInfoFolder.GetCount(); nFolderIndex++)
	{
		posFolder = m_listUserInfoFolder.FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_listUserInfoFolder.GetAt(posFolder);
		
		//# File Count ReSet!
		stXMLTreeInfoFolder.nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		
		if (stXMLTreeInfoFolder.strName == (CString)FOLDER_TEMPFILE)
			stXMLTreeInfoFolder.bLock = TRUE;
		
		m_listUserInfoFolder.SetAt(posFolder, stXMLTreeInfoFolder);
	}
	
	SetSaveTreeXml("USER", &m_listUserInfoFolder);
}

void CMainFrmWnd::GetPathToList(CString strRootFolderPath, 
								CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	CFileFind			ffDirectory;
	CFileFind			ffFile;
	
	BOOL	bFindFolder, bFindFile;
	CString strFileName, strFileTitle, strTemp;
	CString strFolderPath, strFolderName;
	BOOL bLock;
	int nFileCnt;
	
	bFindFolder = ffDirectory.FindFile(strRootFolderPath + "\\" + "*.*");
	while (bFindFolder)
	{
		bFindFolder = ffDirectory.FindNextFile();
		
		strFolderPath = ffDirectory.GetFileName();
		strFolderName = ffDirectory.GetFileTitle();
		if (ffDirectory.IsDirectory() && strFolderPath != "." && strFolderPath != "..")
		{
			bLock = (strFolderPath.CompareNoCase((CString)FOLDER_TEMPFILE) == 0)?TRUE:FALSE;
			nFileCnt = 0;
			
			stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
			bFindFile = ffFile.FindFile(strRootFolderPath + "\\" + strFolderPath + "\\" + "*.*");
			while (bFindFile)
			{
				bFindFile = ffFile.FindNextFile();
				strFileName = ffFile.GetFileName();
				
				if (!ffFile.IsDirectory())
				{
					strFileTitle = ffFile.GetFileTitle();
					strTemp = strFileName;
					strTemp.Replace(strFileTitle, "");
					strTemp.MakeUpper();
					if (strTemp.CompareNoCase(".XML") == 0 )
					{
						stXMLTreeInfoFile.strFolderPath = strFolderPath;
						stXMLTreeInfoFile.strFilePath = strFileName;
						stXMLTreeInfoFile.strFileName = strFileTitle;
						
						GetFileSetState("USER", strFolderPath, strFileName, 
							stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);
						
						stXMLTreeInfoFile.bQuickReg = FALSE;
						
						stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
						
						nFileCnt++;
					}
				}
			}
			
			stXMLTreeInfoFolder.bLock = bLock;
			stXMLTreeInfoFolder.strName = strFolderName;
			stXMLTreeInfoFolder.strPath = strFolderPath;
			stXMLTreeInfoFolder.nFileCnt = nFileCnt;
			
			pFolderList->AddTail(stXMLTreeInfoFolder);
		}
	}
}

BOOL CMainFrmWnd::GetXMLToList(CString strFilePath,	CString strType,
							   CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	MSXML::IXMLDOMNodePtr					pNIndex;
	
	MSXML::IXMLDOMNodeListPtr			pNSFolder;
	MSXML::IXMLDOMNodePtr					pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAFolder;
	
	MSXML::IXMLDOMNodeListPtr			pNSCondition;
	MSXML::IXMLDOMNodePtr					pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr	pNACondition;
	
	BOOL bResult = true;
	VARIANT_BOOL bLoadedXMLFile;
	
	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;

	//20120112 이문수 파일존재여부 선행검사 >>	
	CFileFind ff;
	if(!ff.FindFile(strFilePath))		
		return FALSE;	
	//20120112 이문수 파일존재여부 선행검사 <<

	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFilePath));
	
	// XML 파일을 로드하는데 실패하였다면
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	
	//try {
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		if(pNIndex == NULL) return FALSE;
		pNSFolder = pNIndex->selectNodes("FOLDER");
		if(pNSFolder == NULL) return FALSE;
		
		CString strFolderName, strFolderPath, strLock;
		int nLength_Folder = pNSFolder->length;
		for (int nFolderIndex = 0; nFolderIndex < nLength_Folder; nFolderIndex++)
		{
			pNFolder = pNSFolder->item[nFolderIndex];
			if(pNFolder == NULL) return FALSE;

			pNAFolder = pNFolder->attributes;
			if(pNAFolder == NULL) return FALSE;

			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			
			pNAttributeTemp = pNAFolder->getNamedItem("NAME");
			if(pNAttributeTemp == NULL) return FALSE;
			stXMLTreeInfoFolder.strName = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNAFolder->getNamedItem("PATH");
			if(pNAttributeTemp == NULL) return FALSE;
			stXMLTreeInfoFolder.strPath = CString((PCHAR)pNAttributeTemp->text);
			
			pNAttributeTemp = pNAFolder->getNamedItem("LOCK");
			if(pNAttributeTemp == NULL) return FALSE;
			stXMLTreeInfoFolder.bLock = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			pNSCondition = pNFolder->selectNodes("CONDITION");
			if(pNSCondition == NULL)	return FALSE;

			int nFileCnt = pNSCondition->length;
			stXMLTreeInfoFolder.nFileCnt = nFileCnt;
			
			stXMLTreeInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
			for (int nFileIndex = 0; nFileIndex < nFileCnt; nFileIndex++)
			{
				pNCondition = pNSCondition->item[nFileIndex];
				if(pNCondition == NULL) 
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				// Folder에 아이템 생성하기 - Start
				MSXML::IXMLDOMNodePtr	pNAttributeTemp;
				pNACondition = pNCondition->attributes;
				if(pNACondition == NULL) 
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				stXMLTreeInfoFile.strFolderPath = stXMLTreeInfoFolder.strPath;
				
				pNAttributeTemp = pNACondition->getNamedItem("NAME");
				if(pNAttributeTemp == NULL) 
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				stXMLTreeInfoFile.strFileName = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNACondition->getNamedItem("PATH");
				if(pNAttributeTemp == NULL)
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				stXMLTreeInfoFile.strFilePath = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNACondition->getNamedItem("ALERTREG");
				if(pNAttributeTemp == NULL)
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				stXMLTreeInfoFile.strAlertReg = CString((PCHAR)pNAttributeTemp->text);
				
				pNAttributeTemp = pNACondition->getNamedItem("UNITCAN");
				if(pNAttributeTemp == NULL)
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}

				stXMLTreeInfoFile.bUnitCan = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;
				
				pNAttributeTemp = pNACondition->getNamedItem("QUICKREG");
				if(pNAttributeTemp == NULL)
				{
					stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
					delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
					return FALSE;
				}
				stXMLTreeInfoFile.bQuickReg = (CString((PCHAR)pNAttributeTemp->text) == "TRUE") ? TRUE : FALSE;
				
				stXMLTreeInfoFile.strType = strType;
				if (strType == "SYS")
				{
					pNAttributeTemp = pNACondition->getNamedItem("EXPLANATION");
					if(pNAttributeTemp == NULL)
					{
						stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
						delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
						return FALSE;
					}

					stXMLTreeInfoFile.strExplain = CString((PCHAR)pNAttributeTemp->text);
				}
				else
				{
					pNAttributeTemp = pNACondition->getNamedItem("EXPLANATION");
					if(pNAttributeTemp == NULL)
					{
						stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
						delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
						return FALSE;
					}

					if (pNAttributeTemp == NULL)
						stXMLTreeInfoFile.strExplain = "";
					else
						stXMLTreeInfoFile.strExplain = CString((PCHAR)pNAttributeTemp->text);
				}
				
				stXMLTreeInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
			}
			
			pFolderList->AddTail(stXMLTreeInfoFolder);
		}
	//} 
	//catch(_com_error e)
	//{
	//	bResult = FALSE;
	//}	
	//return bResult;
	return TRUE;
}

void CMainFrmWnd::ProcessDeleteFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
									  CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList)
{
	XMLTreeInfoFile	stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	XMLTreeInfoFolder	stSourceInfoFolder, stDestInfoFolder;
	
	POSITION posFolder;
	int nFolderCnt = pSourceList->GetCount();
	
	int nFolderIndexPos = 0;
	for (int nFolderIndex = 0; nFolderIndex < nFolderCnt; nFolderIndex++)
	{
		posFolder = pSourceList->FindIndex(nFolderIndexPos);
		stXMLTreeInfoFolder = pSourceList->GetAt(posFolder);
		
		if (GetFindFolder(stXMLTreeInfoFolder.strPath, pDestList) == FALSE)
		{
			stXMLTreeInfoFolder.plistXMLTreeInfoFile->RemoveAll();
			delete stXMLTreeInfoFolder.plistXMLTreeInfoFile;
			pSourceList->RemoveAt(posFolder);
		}
		else
		{
			posFolder = GetPosFolder(pSourceList, stXMLTreeInfoFolder.strPath);
			stSourceInfoFolder = pSourceList->GetAt(posFolder);
			
			posFolder = GetPosFolder(pDestList, stXMLTreeInfoFolder.strPath);
			stDestInfoFolder = pDestList->GetAt(posFolder);
			
			if ((stSourceInfoFolder.plistXMLTreeInfoFile == NULL) ||
				(stDestInfoFolder.plistXMLTreeInfoFile == NULL))
			{
				
			}
			else
				ProcessDeleteFile(stSourceInfoFolder.plistXMLTreeInfoFile,
				stDestInfoFolder.plistXMLTreeInfoFile);
			
			nFolderIndexPos++;
		}
	}
}

void CMainFrmWnd::ProcessInsertFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pSourceList, 
									  CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pDestList)
{
	POSITION posFolder;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	XMLTreeInfoFolder	stSourceInfoFolder, stDestInfoFolder;
	
	for (int nFolderIndex = 0; nFolderIndex < pSourceList->GetCount(); nFolderIndex++)
	{
		posFolder = pSourceList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pSourceList->GetAt(posFolder);
		if (GetFindFolder(stXMLTreeInfoFolder.strPath, pDestList) == FALSE)
		{
			XMLTreeInfoFolder		stAddInfoFolder;
			
			stAddInfoFolder.bLock = stXMLTreeInfoFolder.bLock;
			stAddInfoFolder.strName = stXMLTreeInfoFolder.strName;
			stAddInfoFolder.strPath = stXMLTreeInfoFolder.strPath;
			
			stAddInfoFolder.plistXMLTreeInfoFile = new CList <XMLTreeInfoFile, XMLTreeInfoFile&>;
			
			POSITION posFile;
			XMLTreeInfoFile	stXMLTreeInfoFile;
			for (int nFileIndex = 0; nFileIndex < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nFileIndex++)
			{
				posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
				
				stAddInfoFolder.plistXMLTreeInfoFile->AddTail(stXMLTreeInfoFile);
			}
			stAddInfoFolder.nFileCnt = stAddInfoFolder.plistXMLTreeInfoFile->GetCount();
			pDestList->AddTail(stAddInfoFolder);
		}
		else
		{
			posFolder = GetPosFolder(pSourceList, stXMLTreeInfoFolder.strPath);
			stSourceInfoFolder = pSourceList->GetAt(posFolder);
			
			posFolder = GetPosFolder(pDestList, stXMLTreeInfoFolder.strPath);
			stDestInfoFolder = pDestList->GetAt(posFolder);
			
			if ((stSourceInfoFolder.plistXMLTreeInfoFile == NULL) ||
				(stDestInfoFolder.plistXMLTreeInfoFile == NULL))
			{
				
			}
			else
				ProcessInsertFile(stSourceInfoFolder.plistXMLTreeInfoFile, 
				stDestInfoFolder.plistXMLTreeInfoFile);
		}
	}
}

void CMainFrmWnd::ProcessDeleteFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
									CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList)
{
	POSITION pos;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	
	int nCount = pSourceList->GetCount();
	int nFileIndexPos = 0;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pSourceList->FindIndex(nFileIndexPos);
		stXMLTreeInfoFile = pSourceList->GetAt(pos);
		
		if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
			pSourceList->RemoveAt(pos);
		else
			nFileIndexPos++;
	}
	
	/*
	pos = pSourceList->GetHeadPosition();
	while (pos != NULL)
	{
	stXMLTreeInfoFile = pSourceList->GetNext(pos);
	
	  if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
	  {
	  //pos = GetPosFile(pSourceList, stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath);
	  pSourceList->RemoveAt(pos);	
	  }
	  }
	*/
}

void CMainFrmWnd::ProcessInsertFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pSourceList, 
									CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pDestList)
{
	POSITION pos;
	XMLTreeInfoFile	stXMLTreeInfoFile;
	
	for (int nIndex = 0; nIndex < pSourceList->GetCount(); nIndex++)
	{
		pos = pSourceList->FindIndex(nIndex);
		stXMLTreeInfoFile = pSourceList->GetAt(pos);
		
		if (GetFindFile(stXMLTreeInfoFile.strFilePath, pDestList) == FALSE)
		{
			GetFileSetState("USER", stXMLTreeInfoFile.strFolderPath, stXMLTreeInfoFile.strFilePath, 
				stXMLTreeInfoFile.strAlertReg, stXMLTreeInfoFile.bUnitCan);
			
			stXMLTreeInfoFile.bQuickReg = FALSE;
			
			pDestList->AddTail(stXMLTreeInfoFile);
		}
	}
}


BOOL CMainFrmWnd::GetFindFile(CString strFilePath,
							  CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList)
{
	BOOL bResult = FALSE;
	
	POSITION pos;
	XMLTreeInfoFile      stXMLTreeInfoFile;
	for (int nIndex = 0; nIndex < pFileList->GetCount(); nIndex++)
	{
		pos = pFileList->FindIndex(nIndex);
		stXMLTreeInfoFile = pFileList->GetAt(pos);
		
		if (strFilePath == stXMLTreeInfoFile.strFilePath)
		{
			bResult = TRUE;
			break;
		}
	}
	
	return bResult;
}

BOOL CMainFrmWnd::GetFindFile(CString strFolderPath, CString strFilePath,
							  CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;
	
	if (strFolderPath.IsEmpty() || strFilePath.IsEmpty())
		return bResult;
	
	POSITION pos;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	for (int nIndex = 0; nIndex < pFolderList->GetCount(); nIndex++)
	{
		pos = pFolderList->FindIndex(nIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(pos);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
			bResult = GetFindFile(strFilePath, stXMLTreeInfoFolder.plistXMLTreeInfoFile);
		
		if (bResult == TRUE)
			break;
	}
	
	return bResult;
}

BOOL CMainFrmWnd::GetFindFolder(CString strFolderPath, 
								CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = FALSE;
	
	if (strFolderPath.IsEmpty())
		return bResult;
	
	POSITION pos;
	XMLTreeInfoFolder			stXMLTreeInfoFolder;
	for (int nIndex = 0; nIndex < pFolderList->GetCount(); nIndex++)
	{
		pos = pFolderList->FindIndex(nIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(pos);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			bResult = TRUE;
			break;
		}
	}
	
	/*
	pos = pFolderList->GetHeadPosition();
	while(pos != NULL)
	{
	stXMLTreeInfoFolder = pFolderList->GetNext(pos);
	if (strFolderPath == stXMLTreeInfoFolder.strPath)
	{
	bResult = TRUE;
	break;
	}
	}
	*/
	
	return bResult;
}


BOOL CMainFrmWnd::SetSaveTreeXml(CString strType,
								 CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList)
{
	BOOL bResult = TRUE;
	
	CString strFilePath;
	strFilePath = GetFilePath(strType);
	
	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr						pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAFolder;
	
	MSXML::IXMLDOMNodePtr						pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr		pNACondition;
	
	try 
	{
		POSITION pos;
		XMLTreeInfoFile stFileInfo;
		XMLTreeInfoFolder stFolderInfo;
		
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr		pPI;
		MSXML::IXMLDOMNamedNodeMapPtr						pNAPI;
		MSXML::IXMLDOMNodePtr										pNEncoding;
		
		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		pDOMIndex->appendChild(pPI);
		
		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)XML_VERSION_USERFILE;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pDOMIndex->appendChild(pNIndex);
		int nFolderCnt = pFolderList->GetCount();
		for (int nFolderIndex = 0; nFolderIndex < nFolderCnt; nFolderIndex++)
		{
			pos = pFolderList->FindIndex(nFolderIndex);
			stFolderInfo = pFolderList->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;			
			pNFolder = pDOMIndex->createElement("FOLDER");
			pNAFolder = pNFolder->attributes;
			
			pNAttributeTemp = pDOMIndex->createAttribute("NAME");
			pNAttributeTemp->text = (LPCTSTR)stFolderInfo.strName;
			pNAFolder->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("PATH");
			pNAttributeTemp->text = (LPCTSTR)stFolderInfo.strPath;
			pNAFolder->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("LOCK");
			if (stFolderInfo.strName == (CString)FOLDER_TEMPFILE)
				pNAttributeTemp->text = "TRUE";
			else
				pNAttributeTemp->text = (LPCTSTR)(stFolderInfo.bLock == TRUE)?"TRUE":"FALSE";
			pNAFolder->setNamedItem(pNAttributeTemp);
			
			pNIndex->appendChild(pNFolder);
			
			if ((stFolderInfo.plistXMLTreeInfoFile == NULL) || (stFolderInfo.nFileCnt == 0))
				continue;
			
			int nFileCnt = stFolderInfo.plistXMLTreeInfoFile->GetCount();

			for(int nFileIndex = 0; nFileIndex < nFileCnt; nFileIndex++)
			{
				pos = stFolderInfo.plistXMLTreeInfoFile->FindIndex(nFileIndex);
				stFileInfo = stFolderInfo.plistXMLTreeInfoFile->GetAt(pos);
				
				if (stFolderInfo.strPath == stFileInfo.strFolderPath)
				{
					MSXML::IXMLDOMNodePtr pNAttributeTemp;
					
					pNCondition = pDOMIndex->createElement("CONDITION");
					pNFolder->appendChild( pNCondition );
					pNACondition = pNCondition->attributes;
					
					pNAttributeTemp = pDOMIndex->createAttribute("NAME");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strFileName;
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMIndex->createAttribute("PATH");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strFilePath;
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMIndex->createAttribute("ALERTREG");
					pNAttributeTemp->text = (LPCTSTR)(stFileInfo.strAlertReg);
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMIndex->createAttribute("UNITCAN");
					pNAttributeTemp->text = (LPCTSTR)(stFileInfo.bUnitCan == TRUE) ? "TRUE" : "FALSE";
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMIndex->createAttribute("QUICKREG");
					pNAttributeTemp->text = (LPCTSTR)(stFileInfo.bQuickReg == TRUE) ? "TRUE" : "FALSE";
					pNACondition->setNamedItem(pNAttributeTemp);
					
					pNAttributeTemp = pDOMIndex->createAttribute("EXPLANATION");
					pNAttributeTemp->text = (LPCTSTR)stFileInfo.strExplain;
					pNACondition->setNamedItem(pNAttributeTemp);
				}
			}
		}
		
		pDOMIndex->save(_bstr_t(strFilePath));
		
	}
	catch(_com_error e) 
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CMainFrmWnd::GetFileSetState(CString strType, CString strFolderPath, CString strFilePath,
								  CString &strAlertReg, BOOL &bUnitCan)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNodePtr			    pNConditions;
	MSXML::IXMLDOMNamedNodeMapPtr	pNAConditions;
	
	CString strPath = GetFolderPath(strType) + "\\" + strFolderPath + "\\" + strFilePath;
	
	//try {
		MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		pDOMConditionIndex->async = false;

		//20120112 이문수 파일존재여부 선행검사 >>
		CFileFind ff;
		if(!ff.FindFile(strPath))		
		{
			strAlertReg = "FALSE";
			bUnitCan = FALSE;
			return FALSE;
		}
		//20120112 이문수 파일존재여부 선행검사 <<

		pDOMConditionIndex->load(_bstr_t(strPath));
		
		pNConditions = pDOMConditionIndex->selectSingleNode("CONDITIONS");
		if(pNConditions == NULL) 
		{		
			strAlertReg = "FALSE";
			bUnitCan = FALSE;
			return FALSE;
		}
		pNAConditions = pNConditions->attributes;
		if(pNConditions == NULL) 
		{		
			strAlertReg = "FALSE";
			bUnitCan = FALSE;
			return FALSE;
		}
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		
		pNAttributeTemp = pNAConditions->getNamedItem("ALERT");
		if(pNAttributeTemp == NULL) 
		{		
			strAlertReg = "FALSE";
			bUnitCan = FALSE;
			return FALSE;
		}
		strAlertReg = (atoi((PCHAR)pNAttributeTemp->text) == 1)?"TRUE":"FALSE";
		
		pNAttributeTemp = pNAConditions->getNamedItem("GROUP_CNT");
		if(pNAttributeTemp == NULL) 
		{		
			strAlertReg = "FALSE";
			bUnitCan = FALSE;
			return FALSE;
		}
		bUnitCan = (atoi((PCHAR)pNAttributeTemp->text) == 1)?TRUE:FALSE;
	//} 
	//catch (_com_error e)
	//{
	//	bResult = FALSE;		
	//	strAlertReg = "FALSE";
	//	bUnitCan = FALSE;
	//}	
	//return bResult;
	return TRUE;
}

POSITION CMainFrmWnd::GetPosFolder(CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> *pFolderList, 
								   CString strFolderPath)
{
	POSITION posResult = NULL;
	
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	
	POSITION posFolder;
	int nFolderIndex;
	for (nFolderIndex = 0; nFolderIndex < pFolderList->GetCount(); nFolderIndex++)
	{
		posFolder = pFolderList->FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = pFolderList->GetAt(posFolder);
		
		if (strFolderPath == stXMLTreeInfoFolder.strPath)
		{
			posResult = posFolder;
			break;
		}
	}
	
	return posResult;
}

POSITION CMainFrmWnd::GetPosFile(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList,
								 CString strFolderPath, CString strFilePath)
{
	POSITION posResult = NULL;
	
	XMLTreeInfoFile	stXMLTreeInfoFile;
	POSITION posFile;
	for (int nFileIndex = 0; nFileIndex < pFileList->GetCount(); nFileIndex++)
	{
		posFile = pFileList->FindIndex(nFileIndex);
		stXMLTreeInfoFile = pFileList->GetAt(posFile);
		
		if ((strFolderPath == stXMLTreeInfoFile.strFolderPath) &&
			(strFilePath == stXMLTreeInfoFile.strFilePath))
		{
			posResult = posFile;
			break;
		}
	}
	
	return posResult;
}

int CMainFrmWnd::GetMatchLogic(CList <HeadInfoType, HeadInfoType&>	*plistFieldInfo, 
							   CString &strMatchLogic)
{
	CList <MatchInfo, MatchInfo&>	*plistMatchInfo;
	plistMatchInfo = &((CConditionMakeWnd*)GetWnd("MAKE"))->m_listMatchInfo;
	
	CString strTemp;
	POSITION pos, posFiledCol;
	MatchInfo	stMatchInfo;
	HeadInfoType	stHeadInfoType;
	int nIndex, nFiledCol;
	for (nIndex = 0; nIndex < plistMatchInfo->GetCount(); nIndex++)
	{
		pos = plistMatchInfo->FindIndex(nIndex);
		stMatchInfo = plistMatchInfo->GetAt(pos);
		
		CString strMatchFieldname;
		if (!stMatchInfo.strElement.IsEmpty())
			strMatchFieldname.Format("%s(%s)", stMatchInfo.strField, stMatchInfo.strElement);
		else
			strMatchFieldname = stMatchInfo.strField;
		
		for (nFiledCol = 0; nFiledCol < plistFieldInfo->GetCount(); nFiledCol++)
		{
			posFiledCol = plistFieldInfo->FindIndex(nFiledCol);
			stHeadInfoType = plistFieldInfo->GetAt(posFiledCol);
			
			if (strMatchFieldname == stHeadInfoType.strHeadName)
				break;
		}
		
		strTemp.Format("%d", nFiledCol);
		if (nIndex < plistMatchInfo->GetCount() - 1)
			strTemp = strTemp + (CString)MATCHVAL_OPT_LIST[stMatchInfo.nOptIndex];
		
		strMatchLogic = strMatchLogic + strTemp;
	}
	
	return plistMatchInfo->GetCount();
}

void CMainFrmWnd::SetSendMainMsg(CString strMsg)
{
	//((CConditionResultWnd*)GetWnd("RESULT"))->m_ctrlMsg.SetWindowText(strMsg);


//	CConditionSearchView *pView = (CConditionSearchView*)GetParent();
//	pView->SendMessage(WMU_SET_MESSAGE, (WPARAM)0, (LPARAM)(LPCSTR)strMsg);
}

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(COLOR_MAIN_CANVAS);
		hbr = m_brushStatic;
	}
	
	return hbr;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	m_clrMainSkin = clrBkgnd;

	m_clrBkgnd = m_clrMainSkin;
	m_stTabColor = stTabColor;

	m_ctrlSplitter.SetColorSplitBar(m_clrMainSkin);
	m_ctrlSplitter.Invalidate();

	if(m_brushStatic.DeleteObject())
		m_brushStatic.CreateSolidBrush(m_clrMainSkin);	

	if(m_pviewCondition)
		m_pviewCondition->ChangeSkinColor(m_clrMainSkin, stTabColor);
	if(m_pviewConditionList)
		m_pviewConditionList->ChangeSkinColor(m_clrMainSkin, stTabColor);

	Invalidate();
}

CWnd* CMainFrmWnd::GetWnd(CString strType)
{
	CWnd* pwndResult = NULL;
	
	if (strType == "MAKE")
		pwndResult = (CWnd*)m_pviewCondition->m_pwndConditionMake;
	else if (strType == "RESULT")
		pwndResult = (CWnd*)m_pviewCondition->m_pwndConditionResult;
	
	return pwndResult;
}

//#include "../../Contents/inc/IMainInfoMng.h"
//void CMainFrmWnd::LoadlanguageEditor(CString _szScriptName)
//{
//	CString szScriptName(_szScriptName);
//	szScriptName += ".sts";
//
//	//m_pwndMsgTarget
//	//CopyBaseScriptFile();
//
//	IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	CString szRootPath = pMainMng->GetRootDir();
//
//	CString szSystemTradingPath;
//	szSystemTradingPath = szRootPath + "\\Chart\\SystemTrading.exe";
//	szSystemTradingPath.MakeLower();
//
//#ifdef _DEBUG	
//	// 디버그 버전용 언어편집기가 없으므로 Release용이 실행되도록 한다.
//	szSystemTradingPath.Replace("windebug", "winrel");
//#endif
//
//// 	CString szScriptPath;
//// 	{
//// 		char szUserPath[BUFSIZ];
//// 		::GetEnvironmentVariable(KEY_USERDIRECTORY, szUserPath, BUFSIZ);
//// 		
//// 		::sprintf(szUserPath, "/U\"%s\" /S\"%s.sts\"", (CString)szUserPath, szScriptName);
//// 		szScriptPath = szUserPath;
//// 	}
//
//	CWnd* pWndCompiler = FindWindow(NULL, _T("SystemTrading"));
//	COPYDATASTRUCT copyData;
//	if(pWndCompiler)
//	{
//		copyData.dwData = 1000;
//		copyData.cbData = ::lstrlen(szScriptName) + sizeof(DWORD);
//		copyData.lpData = (void*)(LPCSTR)szScriptName;
//		//copyData.lpData = (void*)(LPCSTR)szScriptPath;
//		
//		pWndCompiler->SendMessage(WM_COPYDATA, 0,(LPARAM)&copyData);
//	}
//	else
//	{
//		HWND h_pctr = NULL;
//		h_pctr = AfxGetPctrMainHandle();
//
//		LPCSTR szBaseDir = "Data\\STData";
//		char szConfigFile[MAX_PATH] = {NULL, };
//		char szIP[16] = {NULL, };
//		::wsprintf(szConfigFile, _T("%s\\%s\\STConfig.cfg"), szRootPath, szBaseDir);
//
//		CString szScriptPath;
//		CString szUserDir = pMainMng->GetEncUserDir();
//		if(szScriptName.IsEmpty())
//			szScriptPath.Format(_T("/U\"%s\""), szUserDir);
//		else
//			szScriptPath.Format(_T("/U\"%s\" /S\"%s\""), szUserDir, szScriptName);
//
//		CString szOption;
//		szOption.Format("%s|%08X",szScriptPath,h_pctr);
//
//		::ShellExecute(NULL, _T("open"), szSystemTradingPath, szOption, NULL, SW_SHOWMAXIMIZED);
//	}
//}

LRESULT CMainFrmWnd::OnRmsgCheckMenu(WPARAM wp, LPARAM lp)
{
	if(wp==100)
	{
		//XML TREE에서 보낸다.
		LPCSTR szParam = (LPCTSTR)lp;
//		LoadlanguageEditor(szParam);

#ifdef _DEBUG
		CString szMsg;
		szMsg.Format("언어편집기 디버그(%s)에서는 실행 안합니다.", szParam);
		::MessageBox(m_hWnd, szMsg, PROGRAM_TITLE, MB_OK);
		return 0L;
#endif

		((CConditionMakeWnd*)GetWnd("MAKE"))->SetShow_LanguageEdit(szParam);
	}
	return 0L;
}

void CMainFrmWnd::PutLinkData(CString strType, CString strCode)
{
	m_strLinkCode = strCode;
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->ShareCode(strCode);
}

BOOL CMainFrmWnd::GetBackDateAndTime(CString& strDate, CString& strTime)
{
	return ((CConditionMakeWnd*)GetWnd("MAKE"))->GetBackDateAndTime(strDate, strTime);
}

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrBkgnd;
	*stTabColor = m_stTabColor;
}

void CMainFrmWnd::OnDBClickResultGrid(CString strCode)
{
	m_strLinkCode = strCode;
	
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->ShareCode(strCode);
}

CFont* CMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	
	return pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	
	return pScreenWnd->Variant(nComm, strData);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

void CMainFrmWnd::SendTR(CString strTRCode, char* szSendData, int nDataLen, int nKey, char cStat)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->SendTR(strTRCode, szSendData, nDataLen, nKey, cStat);
}

void CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey )
{
	if ((CString)szTR == TRNO_FAVORITE) //tour2k 관심종목 그룹
	{
		char* pReceiveBuff = new char[dwTRDateLen + 1];
		memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
		memcpy(pReceiveBuff, aTRData, dwTRDateLen);

		if (nKey == TRKEY_GWANSIMGROUP)
		{
			CStringArray arrName, arrKey;
			ParseGroup(pReceiveBuff, arrKey, arrName);
			
			m_pviewConditionList->m_treeCommonCtrl.m_plistPortInfo = &m_listPortInfo;
			m_pviewConditionList->m_treeCommonCtrl.SetReloadPortPolio(FALSE);

		}
		else if (nKey == TRKEY_GWANSIMITEM)
		{
            m_arrInitCode.RemoveAll();
			int ncnt = ParseList(pReceiveBuff, m_arrInitCode);

			if (ncnt > 0)
				if (m_bCreateWnd) if(!m_bPortAdd) ((CConditionResultWnd*)GetWnd("RESULT"))->SendHaveItem(&m_arrInitCode);
				else ((CConditionResultWnd*)GetWnd("RESULT"))->SetEditSubjectTarGetBit(TRUE);

			m_bCreateWnd = TRUE;//
			m_bPortAdd = FALSE;
		}
		delete[] pReceiveBuff; pReceiveBuff = nullptr;
	}
	else
	((CConditionResultWnd*)GetWnd("RESULT"))->ReceiveData(szTR, aTRData, dwTRDateLen, nKey );
}

int CMainFrmWnd::ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName)
{
	int cnt = atoi(CString(pReceiveBuff, 4));
	int parseL = 4;

	struct _grSearch
	{
		char ngrs[2];
		char grseq[2];
		char gname[30];
	};

	struct _grSearch* stgr;
	char* pSeq, * pgame;
	pSeq = new char[sizeof(stgr->grseq) + 1];
	pgame = new char[sizeof(stgr->gname) + 1];

	m_listPortInfo.RemoveAll();
	m_GwanSimGroup.strKey.RemoveAll();
	m_GwanSimGroup.strGroupName.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pReceiveBuff[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		m_GwanSimGroup.strKey.Add(pSeq);
		arrKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		m_GwanSimGroup.strGroupName.Add(pgame);
		arrName.Add(pgame);

		parseL += sizeof(struct _grSearch);

		PortGroupInfo stPortGroupInfo;
		stPortGroupInfo.nNo = ii ;
		stPortGroupInfo.strName.Format("%02d %s", ii+1 , pgame);	//[0000052:] 관심그룹 관리를 위해 키값도 이름에 포함시킴.

		m_listPortInfo.AddTail(stPortGroupInfo);
	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return cnt;
}

int CMainFrmWnd::ParseList(char* pReceiveBuff, CStringArray& arrName)
{
	CString strGroup = CString(pReceiveBuff, 2);
	CString strgname = CString(&pReceiveBuff[2], 20).Trim();
	int cnt = atoi(CString(&pReceiveBuff[22], 4));
	int parseL = 26;

	struct _jinfo {
		char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char code[12]; // 종목코드
		char xprc[10]; // 보유단가
		char xnum[10]; // 보유수량
	};

	if (cnt <= 0) return 0;

	struct _jinfo* jinfo;
	char* pgubn, * pCode, * pxprc, * pxnum;
	pgubn = new char[sizeof(jinfo->gubn) + 1];
	pCode = new char[sizeof(jinfo->code) + 1];

	PortGroupInfo stPortGroupInfo;

	for (int ii = 0; ii < cnt; ii++)
	{
		jinfo = (struct _jinfo*)&pReceiveBuff[parseL];
		memset(pgubn, 0x00, sizeof(jinfo->gubn) + 1);
		memcpy(pgubn, jinfo->gubn, sizeof(jinfo->gubn));  // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		memset(pCode, 0x00, sizeof(jinfo->code) + 1);
		memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

		arrName.Add(CString(pCode).Trim());
		stPortGroupInfo.strCode[ii] = CString(pCode).Trim();

		parseL += sizeof(struct _jinfo);
	}

	stPortGroupInfo.strName = strgname;
	stPortGroupInfo.nItemCnt = cnt;
	m_mapPortItem.SetAt(strGroup, stPortGroupInfo);

	delete[] pgubn; pgubn = nullptr;
	delete[] pCode; pCode = nullptr;

	return cnt;
}

BOOL CMainFrmWnd::IsExistPortItem(CString strKey)
{
	PortGroupInfo stPort;
	if (m_mapPortItem.Lookup(strKey, stPort))
		return TRUE;
	return FALSE;
}

CWnd* CMainFrmWnd::GetModuleMainWnd()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	return pScreenWnd->GetModuleMainWnd();
}

BOOL CMainFrmWnd::GetKSPUpMst(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	return pScreenWnd->GetKSPUpMst(pastrJongCode, pastrJongName, pastrJongEngName, nOption);
}

BOOL CMainFrmWnd::GetKSQUpMst(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	return pScreenWnd->GetKSQUpMst(pastrJongCode, pastrJongName, pastrJongEngName, nOption);
}

void CMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
#if(0)//tour2k
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	//	sprintf( sSend,"%s%80s%s", "MYUg", " ", "00001U");

	int sendL = 0;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMGROUP, US_PASS);
#endif
}

BOOL CMainFrmWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
#if(0)//tour2k
	return pScreenWnd->GetData_PortItem(saCode, saName, nKey);
#else
	//	sprintf(sSend, "%s%80s%s%40s%02d", "MYUj", " ", "00001U", " ", nKey);

	int sendL = 0;
	CString stmp;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';
	stmp.Format("%02d", nKey);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMITEM, US_PASS);
	return true;
#endif
}

void CMainFrmWnd::RegPortItem(CStringArray* pSaItemCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pScreenWnd;
	pScreenWnd->RegPortItem(pSaItemCode);
}

void CMainFrmWnd::SyncExcavationTreeListFile(CString strFilePath)
{
	CString strAppPath;
	strAppPath.Format("%s", Variant(homeCC));
	
	CString strErrorMsg;
	if (GetXMLToList(strFilePath, "EXCAVATION", &m_listExcavationFolder) == FALSE)
	{
		strErrorMsg.Format("[%s] Tree List File이 존재하지 않습니다.", strFilePath);
		SetSendMainMsg(strErrorMsg);
	}

	CList<XMLTreeInfoFolder, XMLTreeInfoFolder&> listExcavationFolder;
	CString strFolderPath = GetFolderPath("EXCAVATION");

	GetPathToList(strFolderPath, &listExcavationFolder);
	ProcessDeleteFolder(&m_listExcavationFolder, &listExcavationFolder);
	ProcessInsertFolder(&listExcavationFolder, &m_listExcavationFolder);

	SetDeleteFolderList(&listExcavationFolder);
	listExcavationFolder.RemoveAll();

	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	POSITION posFolder;
	for (int nFolderIndex = 0; nFolderIndex < m_listExcavationFolder.GetCount(); nFolderIndex++)
	{
		posFolder = m_listExcavationFolder.FindIndex(nFolderIndex);
		stXMLTreeInfoFolder = m_listExcavationFolder.GetAt(posFolder);
		
		stXMLTreeInfoFolder.nFileCnt = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount();
		
		if (stXMLTreeInfoFolder.strName == (CString)FOLDER_TEMPFILE)
			stXMLTreeInfoFolder.bLock = TRUE;
		
		m_listExcavationFolder.SetAt(posFolder, stXMLTreeInfoFolder);
	}
}

BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

CString CMainFrmWnd::GetMarketNameToCode(CString strMaketCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetMarketNameToCode(strMaketCode);
}

void CMainFrmWnd::OpenScreen(CString strScreenNo)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->OpenScreen(strScreenNo);
}

int CMainFrmWnd::SetUserMaster(LPCSTR szKey, int nCount, CStringArray *pArrCode, CStringArray *pArrName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->SetUserMaster(szKey, nCount, pArrCode, pArrName);
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

COLORREF CMainFrmWnd::GetMainBackColor()
{
	return m_clrMainSkin;
}

void CMainFrmWnd::GetAccNo(CStringArray* psaAccNo)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->GetAccNo(psaAccNo);
}

void CMainFrmWnd::RequestHaveItem(CString strAccNo)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->RequestHaveItem(strAccNo);
}

void CMainFrmWnd::SendHaveItem(CStringArray* psaHaveItem)
{
	((CConditionResultWnd*)GetWnd("RESULT"))->SendHaveItem(psaHaveItem);
}

CString CMainFrmWnd::GetSelectedCondition()
{
	CString strSavePath;
	strSavePath.Format("%s%s%s%s\\%s", Variant(homeCC), USRDIR, Variant(nameCC), FOLDER_FINDDATA, "8416_ConditionInfo.dat");
	
	CString strConditionName;
	
	CFileFind ff;
	if(ff.FindFile(strSavePath))
	{
		char chData[256];
		GetPrivateProfileString("SelectedCondition", "ConditionName", "", chData, sizeof(chData), strSavePath);
		strConditionName.Format("%s", chData);
	}

	if(strConditionName == "")
		strConditionName = "분봉 N자형 상승패턴";
	
	return strConditionName;
}

void CMainFrmWnd::SaveSelectedCondition()
{
	CString strSavePath;
	strSavePath.Format("%s%s%s%s\\%s", Variant(homeCC), USRDIR, Variant(nameCC), FOLDER_FINDDATA, "8416_ConditionInfo.dat");
	
	CString strConditionName = m_pviewConditionList->m_treeExcavation.m_strSelectItemText;

	TRY 
	{
		WritePrivateProfileString("SelectedCondition", "ConditionName", strConditionName, strSavePath);
	}
	CATCH (CFileException, pEx)
	{
		
	}
	END_CATCH
}

void CMainFrmWnd::OnLanguageReload()
{
	SetInit_LibLanguage(FALSE);
	SetInit_LibLanguage(TRUE);
		
	m_pviewConditionList->m_treeCommonCtrl.SetReloadLanguage();
	m_pviewConditionList->m_treeCommonCtrl_Sort.SetReloadLanguage();	

	m_pviewConditionList->m_treeCommonCtrl.SetExpandLanguage();
}

BOOL CMainFrmWnd::GetMemberCompany(CStringArray &arrCode, CStringArray &arrName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetMemberCompany(arrCode, arrName);
}

void CMainFrmWnd::ListViewSearchEditReset()
{
	m_pviewConditionList->m_comboSearch.SetWindowText("");
}