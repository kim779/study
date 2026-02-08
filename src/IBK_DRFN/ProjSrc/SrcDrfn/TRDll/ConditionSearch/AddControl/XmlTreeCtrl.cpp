// XmlTreeCtrl.cpp : implementation file
//

/**********************************************************************/
/*																																		*/
/*												XML Load  Control														*/
/*													(Mouse Control)														*/
/*																																		*/
/*												 작성일 : 2003. 11~													*/
/*							       작성자 : 우동우 (TEXEN SYSTEM)									*/
/*																																		*/
/**********************************************************************/

#include "stdafx.h"
#include "../resource.h"
#include "XmlTreeCtrl.h"
#include "../ConditionListView.h"
#include "../MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE hDllInstance;

#define RECOMMEND_ROOTTITLE				"추천"
#define USER_ROOTTITLE			  		"사용자조건"
//#define LANGUAGE_ROOTTITLE				"랭귀지조건"
#define LANGUAGE_ROOTTITLE				"랭귀지"
#define ITEMEXCAVATION_ROOTTITLE		"유망종목발굴외"

/*
#define NEWS_SUBROOTTITLE			  	"뉴스분석"
#define PORTPOLIO_SUBROOTTITLE		"관심그룹"
#define LANGUAGE_SUBROOTTITLE			"랭귀지"
#define PATTEN_ROOTTITLE				  "사용자패턴"
#define ACCOUNT_SUBROOTTITLE			"보유종목그룹"
*/
#define BOOKMARK_SUBROOTTITLE			"즐겨찾기"


#define MENU_ID_ADD								100
#define MENU_ID_DEL								200
#define	MENU_ID_USERCONDDEL						300
#define	MENU_ID_EXECLANGUAGE					400
#define	MENU_ID_EXECLANGUAGE_SCRIPT				401
#define MENU_TITLE_ADD						"즐겨찾기 추가"
#define MENU_TITLE_DEL						"즐겨찾기 삭제"
#define MENU_USERCOND_DEL					"사용자조건 삭제"
#define MENU_EXECLANGUAGE					"언어편집기 실행"


#define OFFSET_PORT								-100
#define OFFSET_ACCOUNT						-300
#define OFFSET_LANGUAGE						-400
#define OFFSET_USERPATTEN					-2000
#define OFFSET_NEWS               -3000
#define OFFSET_MARK								-5000			//<= [Last]

#define OFFSET_SYSFILE						10000
#define OFFSET_USERFILE						20000
#define OFFSET_EXCAVATION					30000


const int m_nTreeIconIndex0St = 0;
const int m_nTreeIconIndex0St_File = 2;   //# Add : 2006.09
const int m_nTreeIconIndex1St = 4;
const int m_nTreeIconIndex2St = 6;
const int m_nTreeIconIndexNormal = 8;		//#<= Normal
const int m_nTreeIconIndexNonAlert = 9;	//#<= Alert
const int m_nTreeIconIndexPort = 10;			//# PortPolio

const int m_nTreeIconIndex_NormalFile = 11;				//#<= Normal File
const int m_nTreeIconIndex_NormalFile_Unit = 12;	//#<= Unit Normal File
const int m_nTreeIconIndex_AlertFile = 13;				//#<= Alert File
const int m_nTreeIconIndex_AlertFile_Unit = 14;		//#<= Alert Unit File


/*
#define TREE_SELECT_TYPE_ITEM         1
#define TREE_SELECT_TYPE_CLASS        2

#define TREE_SELECT_TYPE_USERFILE     3
#define TREE_SELECT_TYPE_SYSFILE      4

#define TREE_SELECT_TYPE_PORTPOLIO    5
#define TREE_SELECT_TYPE_ACCOUNT      6
#define TREE_SELECT_TYPE_LANGUAGE     7
#define TREE_SELECT_TYPE_USERPATTEN   8
#define TREE_SELECT_TYPE_NEWS         9
*/

extern HINSTANCE hDllInstance;
/////////////////////////////////////////////////////////////////////////////
// CXmlTreeCtrl

CXmlTreeCtrl::CXmlTreeCtrl()
{
	m_bIndustry = FALSE;
	m_bIsFnUser = TRUE;

	m_strLoadType = ""; //# 0 : Common, 01 : Common Sort, 02 : Common Mark
					  //# 1 : Order, 11 : Order Sort, 12 : Order Mark,
					  //# 2 : Recommend, 3 : user, 4 : Language, 5 : Recommend + user

	m_bLoadFlag = FALSE;
		
	m_nXmlListIndex = 0;
	m_nXmlMarkListIndex = 0;
	m_nXmlUserListIndex = 0;
	m_nXmlSysListIndex = 0;

	m_nSeleteType = -1;
	m_bAllItemView = FALSE;

	m_strSelectItemText = "XXX";

	m_htiRoot = NULL;
	m_htiPortPolio = NULL;
	m_htiAccount = NULL;
	m_htiLanguage = NULL;
	m_htiPatten = NULL;
	m_htiNews = NULL;
	m_htiBookMark = NULL;

	m_htiRecomm = NULL;
	m_htiUser = NULL;

	m_plistPortInfo = NULL;
	m_plistLanguageInfo = NULL;
	m_bLDragging = FALSE;
}

CXmlTreeCtrl::~CXmlTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXmlTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXmlTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(MENU_ID_ADD, MENU_EXECLANGUAGE, OnSendMenu)	
//	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CXmlTreeCtrl::OnTvnBegindrag)
ON_WM_ERASEBKGND()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

	//ON_COMMAND_RANGE(MENU_ID_ADD, MENU_ID_USERCONDDEL, OnSendMenu)

/////////////////////////////////////////////////////////////////////////////
// CXmlTreeCtrl message handlers

void CXmlTreeCtrl::SetIconLoad()
{

}


CString CXmlTreeCtrl::GetPath(int nComm)
{
	CString strAppPath;
	
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMsgTarget;
	strAppPath = pMainWnd->Variant(nComm);
		
	return strAppPath;
}

int CXmlTreeCtrl::Create(CWnd* pParentWnd, UINT nID, CImageList *pListImage,
              CString strLoadType,
							BOOL bSendMsgThis/* = TRUE*/, 
              BOOL bJustAlert/* = FALSE*/, 
              int nUseBookMarkType/* = 0*/, //# 0 : Not User, 1 : Read, 2 : Read/Write) 
              BOOL bIndustry/* = FALSE*/,
              BOOL bIsFnUser /*= TRUE*/)
{
	int nResult = -1;

	if (bSendMsgThis == TRUE)
		m_pwndMsgTarget = pParentWnd;
	else
	{
		pParentWnd = pParentWnd->GetParent();
		m_pwndMsgTarget = pParentWnd->GetParent();	//# CMainFrmWnd
	}
	
	m_strAppPath = GetPath(homeCC);
	
	DWORD dwStyle;
	dwStyle = WS_CHILD | WS_CLIPCHILDREN | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS;

	if (strLoadType == "3")
		dwStyle += TVS_EDITLABELS;
	else if(strLoadType != "0")
		dwStyle += TVS_DISABLEDRAGDROP;

	nResult = CTreeCtrl::Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd,	nID);
	CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		 	
	m_strLoadType = strLoadType;  //# 0 : Common, 01 : Common Sort, 02 : Common Mark
                                //# 1 : Order, 11 : Order Sort, 12 : Order Mark,
                                //# 2 : Recommend, 3 : user, 4 : Language, 5 : Recommend + user
								//# 7 : 유망종목발굴 외
  
	m_bSendMsgThis = bSendMsgThis; 
	m_bJustAlert = bJustAlert;
	m_nUseBookMarkType = nUseBookMarkType;

	m_bIndustry = bIndustry;
	m_bIsFnUser = bIsFnUser;
	
	SetImageList(pListImage, TVSIL_NORMAL);

	SetInit();

	return nResult;
}

void CXmlTreeCtrl::SetInit()
{
	if ((m_strLoadType == "0") || (m_strLoadType == "01"))
	{
		SetLoadPortPolio();		//# PortPolio
		SetLoadNews();
		SetLoadAccount();

		SetLoadLanguage();

		if (m_nUseBookMarkType > 0)
			SetLoadXmlMark(m_strLoadType);
	}
	else if ((m_strLoadType == "1") || (m_strLoadType == "11"))
	{
		if (m_nUseBookMarkType > 0)
	 		SetLoadXmlMark(m_strLoadType);
	}
	else if (m_strLoadType == "5")
		m_bLoadFlag = SetLoadXml_File();
	else if(m_strLoadType == "7")
	{

	}

	SetDefaultCursor();
}

void CXmlTreeCtrl::OnDestroy() 
{
//	CTreeCtrl::OnDestroy();
		
	if ((m_strLoadType == "0") || (m_strLoadType == "01"))
	{
		if (m_nUseBookMarkType == 2)		//# Read/Write
			SetSaveXmlMark(m_strLoadType, &m_listXMLTreeMarkInfo);

		m_listNewsInfo.RemoveAll();
		
		m_saAccountInfo.RemoveAll();
	}
	else if ((m_strLoadType == "1") || (m_strLoadType == "11"))
	{
		if (m_nUseBookMarkType == 2)		//# Read/Write
			SetSaveXmlMark(m_strLoadType, &m_listXMLTreeMarkInfo);
	}
	else if (m_strLoadType == "5")
	{
		m_listUserInfoFile.RemoveAll();
		m_listSysInfoFile.RemoveAll();
	}
	else if(m_strLoadType == "7")
		m_listItemExcavationInfoFile.RemoveAll();

	/*
	else if (m_strLoadType == "3")
		SetSaveXmlFile(m_strLoadType);
	*/

	m_listXMLTreeInfo.RemoveAll();
	m_listXMLTreeMarkInfo.RemoveAll();
	m_listXMLTreeInfoFile.RemoveAll();

	CTreeCtrl::OnDestroy();
}

BOOL CXmlTreeCtrl::SetTreeView()
{
	BOOL bResult = -1;

 	if (m_bLoadFlag == FALSE)
	{
		if (m_strLoadType == "5")
			bResult = SetLoadXml_File();
		else
			bResult = SetLoadXml();

		m_bLoadFlag = bResult;
    /*
		if ((m_strLoadType > 1) && (m_strLoadType != 5))
		{
			m_bAllItemView = FALSE;
			SetItemAllView(TRUE);
		}
    */
	}

 	return bResult;
}

void CXmlTreeCtrl::SetControlState(BOOL bState)
{
	return;
	
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

BOOL CXmlTreeCtrl::SetLoadXml_File()
{
	SetControlState(FALSE);

	BOOL bResult = FALSE;

	DeleteAllItems();
	
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr))
		return bResult;

	m_htiRoot = GetRootItem();
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	CString strFileRoot = GetFilePath("3");
	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		m_htiUser = SetTreeItem(m_htiRoot, USER_ROOTTITLE, 
                  m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);
		SetInitTreeFile(pXmlDom->childNodes, m_htiUser, "3");
	}
	else
	{

	}

	strFileRoot = GetFilePath("2");
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
// 		m_htiRecomm = SetTreeItem(m_htiRoot, RECOMMEND_ROOTTITLE, 
// 									m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);
// 		SetInitTreeFile(pXmlDom->childNodes, m_htiRecomm, "2");
	}
	else
	{

	}
	
	SetControlState(TRUE);

	return TRUE;
}

BOOL CXmlTreeCtrl::SetLoadXml_File(CString strLoadType, HTREEITEM htiParent)
{
	if (htiParent == NULL)
		return FALSE;
	
	SetControlState(FALSE);
	
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	CString strFileRoot = GetFilePath(strLoadType);
	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
		SetInitTreeFile(pXmlDom->childNodes, htiParent, strLoadType);
	else
	{
		
	}
	
	SetControlState(TRUE);
	
	return FALSE;
}

int CXmlTreeCtrl::GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan)
{
	int nResult = m_nTreeIconIndex_NormalFile;

	if (bAlertReg == TRUE)
	{
		if (bUnitCan == TRUE)
			nResult = m_nTreeIconIndex_AlertFile_Unit;
		else
			nResult = m_nTreeIconIndex_AlertFile;
	}
	else
	{
		if (bUnitCan == TRUE)
			nResult = m_nTreeIconIndex_NormalFile_Unit;
		else
			nResult = m_nTreeIconIndex_NormalFile;
	}

	return nResult;
}

void CXmlTreeCtrl::SetLoadUserPatten(CStringArray &saUserPatten)
{
	/*
	CString strPath_UserPatten = m_strAppPath + "\\UserPattern.ini";

	char szBuff[2048] = {0,}, szItem[256] = {0,};
	if (::GetPrivateProfileSectionNames(szBuff, 2048, strPath_UserPatten) == 0) 
		return;
	
	int nLen = 0;
	while(szBuff[nLen])
	{
		strcpy(szItem, szBuff + nLen);  
		nLen += (strlen(szItem) + 1);		// 1 = NULL

		saUserPatten.Add(szItem);
	}
	*/
}

XMLTreeInfo CXmlTreeCtrl::GetPattenToTreeInfo(CString strName)
{
	XMLTreeInfo stXMLTreeInfo;
	
	stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_USERPATTEN;		//# Item : Common[0] / Order[1] / Language[4] / UserPatten[5]
	
	stXMLTreeInfo.strName = strName;
	stXMLTreeInfo.strTag = (CString)TAG_TYPE_USERPATTEN;
	
	stXMLTreeInfo.nNonAlert = 1;					//# Deawoo [NEW]
	stXMLTreeInfo.nCondiType = 0;
	stXMLTreeInfo.nTargetBase_Type = 0;
	
	stXMLTreeInfo.nGubun = 0;
	stXMLTreeInfo.nGubunDefVal = 0;
	
	stXMLTreeInfo.nNotUser = 0;
	stXMLTreeInfo.nNotDefVal = 0;
	
	stXMLTreeInfo.strResultViewID = "5311/";
	stXMLTreeInfo.nResultViewDefVal = 0;
	
	stXMLTreeInfo.nChartValSet = 2;					//# Deawoo [NEW]
	stXMLTreeInfo.strMapFile = (CString)MAP_OTHER_FILE;
	
	return stXMLTreeInfo;
}

void CXmlTreeCtrl::SetReloadUserPatten()
{
	if (m_htiPatten == NULL)
		return;

	HTREEITEM hItem, hDelete;
	hItem = GetChildItem(m_htiPatten);
	while (hItem != NULL)
	{
		hDelete = hItem;
		hItem = GetNextItem(hItem, TVGN_NEXT);
		DeleteItem(hDelete);
	}

	CStringArray saUserPatten;
	SetLoadUserPatten(saUserPatten);
	for (int nIndex = 0; nIndex < saUserPatten.GetSize(); nIndex++)
		SetTreeItem(m_htiPatten, saUserPatten.GetAt(nIndex), m_nTreeIconIndexNonAlert, m_nTreeIconIndexNonAlert, OFFSET_USERPATTEN - nIndex);
	saUserPatten.RemoveAll();
}

void CXmlTreeCtrl::SetReloadUserFile(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistUserInfoFolder)
{
	if (m_htiUser == NULL)
		return;

	HTREEITEM hItem, hDelete;
	hItem = GetChildItem(m_htiUser);
	while (hItem != NULL)
	{
		hDelete = hItem;
		hItem = GetNextItem(hItem, TVGN_NEXT);
		DeleteItem(hDelete);
	}

	m_listUserInfoFile.RemoveAll();
	m_nXmlUserListIndex = 0;

	HTREEITEM htiFolder;
	int nIndexFolder, nIndexFile;
	int nTreeIconIndex;

	POSITION posFolder, posFile;
	XMLTreeInfoFile stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	for (nIndexFolder = 0; nIndexFolder < plistUserInfoFolder->GetCount(); nIndexFolder++)
	{
		posFolder = plistUserInfoFolder->FindIndex(nIndexFolder);
		stXMLTreeInfoFolder = plistUserInfoFolder->GetAt(posFolder);

		htiFolder	= SetTreeItem(m_htiUser, stXMLTreeInfoFolder.strName, m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);
		
		CList<XMLTreeInfoFile, XMLTreeInfoFile&> listTempUserInfoFile;
		listTempUserInfoFile.RemoveAll();
		for (nIndexFile = 0; nIndexFile < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nIndexFile++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndexFile);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
			
			if(stXMLTreeInfoFile.strType == "")
				stXMLTreeInfoFile.strType = "USER";
			listTempUserInfoFile.AddTail(stXMLTreeInfoFile);
		}

		TreeInfoSort(&listTempUserInfoFile);
		
		for(int i = 0; i < listTempUserInfoFile.GetCount(); i++)
		{
			posFile = listTempUserInfoFile.FindIndex(i);
			stXMLTreeInfoFile = listTempUserInfoFile.GetAt(posFile);

			nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
				stXMLTreeInfoFile.bUnitCan);

			SetTreeItem(htiFolder, stXMLTreeInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex, OFFSET_USERFILE + m_nXmlUserListIndex);
			m_nXmlUserListIndex++;

			m_listUserInfoFile.AddTail(stXMLTreeInfoFile);
		}
	}

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent()->GetParent();
	CConditionListView* pConditionListViewWnd = (CConditionListView*)pMainFrmWnd->m_pviewConditionList;
	if(!pConditionListViewWnd)
		return;

	if(pConditionListViewWnd->GetSaveFlag())
	{
		CString strSaveFileName = pConditionListViewWnd->GetSaveFileName();

		hItem = GetChildItem(m_htiUser);
		HTREEITEM hSelecteItem = hItem;

		while(hItem != NULL)
		{
			hSelecteItem = GetChildItem(hItem);

			while(hSelecteItem != NULL)
			{
				CString strItemText = GetItemText(hSelecteItem);
				if(strItemText == strSaveFileName)
				{
					SelectItem(hSelecteItem);
					break;
				}

				hSelecteItem = GetNextItem(hSelecteItem, TVGN_NEXT);
			}

			hItem = GetNextItem(hItem, TVGN_NEXT);
		}
	}

	Invalidate();
}

void CXmlTreeCtrl::SetLoadExcavationFile(CList <XMLTreeInfoFolder, XMLTreeInfoFolder&> *plistItemExcavationInfoFolder)
{
	HTREEITEM hItem, hDelete;
	hItem = GetChildItem(m_htiRoot);
	while (hItem != NULL)
	{
		hDelete = hItem;
		hItem = GetNextItem(hItem, TVGN_NEXT);
		DeleteItem(hDelete);
	}
	
	m_listItemExcavationInfoFile.RemoveAll();
	m_nXmlItemExcavationIndex = 0;
	
	HTREEITEM htiFolder;
	int nIndexFolder, nIndexFile;
	int nTreeIconIndex;
	
	POSITION posFolder, posFile;
	XMLTreeInfoFile stXMLTreeInfoFile;
	XMLTreeInfoFolder	stXMLTreeInfoFolder;
	for (nIndexFolder = 0; nIndexFolder < plistItemExcavationInfoFolder->GetCount(); nIndexFolder++)
	{
		posFolder = plistItemExcavationInfoFolder->FindIndex(nIndexFolder);
		stXMLTreeInfoFolder = plistItemExcavationInfoFolder->GetAt(posFolder);
		
		htiFolder = SetTreeItem(m_htiRoot, stXMLTreeInfoFolder.strName, m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);
		
		CList<XMLTreeInfoFile, XMLTreeInfoFile&> listTempItemExcavationInfoFile;
		listTempItemExcavationInfoFile.RemoveAll();
		for (nIndexFile = 0; nIndexFile < stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetCount(); nIndexFile++)
		{
			posFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->FindIndex(nIndexFile);
			stXMLTreeInfoFile = stXMLTreeInfoFolder.plistXMLTreeInfoFile->GetAt(posFile);
			
			if(stXMLTreeInfoFile.strType == "")
				stXMLTreeInfoFile.strType = "EXCAVATION";
			listTempItemExcavationInfoFile.AddTail(stXMLTreeInfoFile);
		}
		
		TreeInfoSort(&listTempItemExcavationInfoFile);
		
		for(int i = 0; i < listTempItemExcavationInfoFile.GetCount(); i++)
		{
			posFile = listTempItemExcavationInfoFile.FindIndex(i);
			stXMLTreeInfoFile = listTempItemExcavationInfoFile.GetAt(posFile);
			
			nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
				stXMLTreeInfoFile.bUnitCan);
			
			CString strFileName;
			strFileName = stXMLTreeInfoFile.strFileName;
			strFileName.Replace("_", " ");
			
			SetTreeItem(htiFolder, strFileName, nTreeIconIndex, nTreeIconIndex, OFFSET_EXCAVATION + m_nXmlItemExcavationIndex);
			m_nXmlItemExcavationIndex++;
			
			m_listItemExcavationInfoFile.AddTail(stXMLTreeInfoFile);
		}
	}
	
// 	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent()->GetParent();
// 	CConditionListView* pConditionListViewWnd = (CConditionListView*)pMainFrmWnd->m_pviewConditionList;
// 	if(!pConditionListViewWnd)
// 		return;
// 	
// 	if(pConditionListViewWnd->GetSaveFlag())
// 	{
// 		CString strSaveFileName = pConditionListViewWnd->GetSaveFileName();
// 		
// 		hItem = GetChildItem(m_htiUser);
// 		HTREEITEM hSelecteItem = hItem;
// 		
// 		while(hItem != NULL)
// 		{
// 			hSelecteItem = GetChildItem(hItem);
// 			
// 			while(hSelecteItem != NULL)
// 			{
// 				CString strItemText = GetItemText(hSelecteItem);
// 				if(strItemText == strSaveFileName)
// 				{
// 					SelectItem(hSelecteItem);
// 					break;
// 				}
// 				
// 				hSelecteItem = GetNextItem(hSelecteItem, TVGN_NEXT);
// 			}
// 			
// 			hItem = GetNextItem(hItem, TVGN_NEXT);
// 		}
// 	}
	
	Invalidate();
}

// 20110222 박찬모 사용자 조건 오름차순 정렬	>> 
void CXmlTreeCtrl::TreeInfoSort(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *plistUserInfoFile)
{
	return;
	
	for(int i = 0; i < plistUserInfoFile->GetCount(); i++)
	{
		for(int j = 0; j < plistUserInfoFile->GetCount() - 1 - i; j++)
		{
			POSITION posFirst = plistUserInfoFile->FindIndex(j);
			XMLTreeInfoFile stTreeInfoFIleFirst = plistUserInfoFile->GetAt(posFirst);

			POSITION posSecond = plistUserInfoFile->FindIndex(j + 1);
			XMLTreeInfoFile stTreeInfoFileSecond = plistUserInfoFile->GetAt(posSecond);

			if(stTreeInfoFIleFirst.strFileName > stTreeInfoFileSecond.strFileName)
			{
				XMLTreeInfoFile stTreeInfoTemp = stTreeInfoFileSecond;
				plistUserInfoFile->SetAt(posSecond, stTreeInfoFIleFirst);
				plistUserInfoFile->SetAt(posFirst, stTreeInfoTemp);
			}
		}
	}
}
// 20110222 박찬모 사용자 조건 오름차순 정렬	<<

void CXmlTreeCtrl::SetReloadUserFile(CList <XMLTreeInfoFile, XMLTreeInfoFile&> *plistUserInfoFile)
{
	if (m_htiUser == NULL)
		return;

	HTREEITEM hItem, hDelete;
	hItem = GetChildItem(m_htiUser);
	while (hItem != NULL)
	{
		hDelete = hItem;
		hItem = GetNextItem(hItem, TVGN_NEXT);
		DeleteItem(hDelete);
	}

	m_listUserInfoFile.RemoveAll();
	m_nXmlUserListIndex = 0;
	
	int nTreeIconIndex;
	POSITION posFile;
	XMLTreeInfoFile stXMLTreeInfoFile;

	BOOL nAddFlag;
	for (int nIndexFile = 0; nIndexFile < plistUserInfoFile->GetCount(); nIndexFile++)
	{
		posFile = plistUserInfoFile->FindIndex(nIndexFile);
		stXMLTreeInfoFile = plistUserInfoFile->GetAt(posFile);

    nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
                                            stXMLTreeInfoFile.bUnitCan);

		nAddFlag = TRUE;
		if (m_bJustAlert == TRUE)
    {
      if (stXMLTreeInfoFile.strAlertReg = "TRUE")
        nAddFlag = TRUE;
      else if (stXMLTreeInfoFile.strAlertReg = "FALSE")
        nAddFlag = FALSE;
      else if (stXMLTreeInfoFile.strAlertReg.IsEmpty() == FALSE)
        nAddFlag = TRUE;
    }

		if (nAddFlag == TRUE)
		{
			SetTreeItem(m_htiUser, stXMLTreeInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex, OFFSET_USERFILE + m_nXmlUserListIndex);
			m_nXmlUserListIndex++;

			m_listUserInfoFile.AddTail(stXMLTreeInfoFile);
		}
	}

	Invalidate();
}

BOOL CXmlTreeCtrl::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;

	pXmlDom->async = false;
	if (!pXmlDom->load (strFileRoot.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
						pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug = strTemp + CString(msg) + "\n" + strFileRoot;


#ifdef _DEBUG
//		SetMessageBox(strDebug, "[XML Files Error]", MB_OK);		
#endif
		return FALSE;
	}

	return TRUE;
}

BOOL CXmlTreeCtrl::SetLoadXml()
{
	SetControlState(FALSE);

	BOOL bResult = FALSE;

	DeleteAllItems();
	HRESULT hr = CoInitialize(NULL);
	if( !SUCCEEDED(hr) )
		return bResult;

	CString strFilePath = GetFilePath(m_strLoadType);
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	if (GetLoadXml(strFilePath, pXmlDom) == FALSE)
		return bResult;
	
	m_htiRoot = GetRootItem();
	if ((m_strLoadType == "0") || (m_strLoadType == "1") ||	//# 0 : Common, 1 : Order
		(m_strLoadType == "01") || (m_strLoadType == "11") ||
		(m_strLoadType == "7"))
	{
		SetInitTree(pXmlDom->childNodes, m_htiRoot);
		
		if (m_strLoadType == "0")
		{	
			SetPortPolioTree();
			SetAccountTree();
			SetLanguageTree();
			SetNewsTree();
			
			SetReloadUserPatten();
		}
		
		if (m_nUseBookMarkType > 0)
			SetInitTreeMark(&m_listXMLTreeMarkInfo);
		
		//# Add : 2006.09
		if (m_htiUser != NULL)
			SetLoadXml_File("3", m_htiUser);
		
		if (m_htiRecomm != NULL)
			SetLoadXml_File("2", m_htiRecomm);
	}
	else if (m_strLoadType == "3")	//# 3 : user
		SetInitTreeFile(pXmlDom->childNodes, m_htiRoot);
	else if (m_strLoadType == "2")	//# 2 : Recommend, 4 : Language
		SetInitTreeFile(pXmlDom->childNodes, m_htiRoot);
	
	Expand(m_htiRoot, TVE_EXPAND);
	HTREEITEM hti = GetChildItem(m_htiRoot);
	bResult = TRUE;
	
	SetControlState(TRUE);
	return bResult;
}

void CXmlTreeCtrl::SetLoadPortPolio()
{
	m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_GET_PORT, (LPARAM)this);
}

void CXmlTreeCtrl::SetReloadPortPolio(BOOL bClear)
{
  //tour2k
	if (bClear)
	{
		// 기존리스트를 삭제하고 다시 로드한다.
		if (m_plistPortInfo) m_plistPortInfo->RemoveAll();
		SetLoadPortPolio();
	}

	SetPortPolioTree();
}

void CXmlTreeCtrl::SetPortPolioTree()
{
	if ((m_htiPortPolio == NULL) || (m_plistPortInfo == NULL))
		return;

	HTREEITEM hItem, hDelete;
	hItem = GetChildItem(m_htiPortPolio);
	while (hItem != NULL)
	{
		hDelete = hItem;
		hItem = GetNextItem(hItem, TVGN_NEXT);
		DeleteItem(hDelete);
	}

	POSITION pos;
	PortGroupInfo stPortGroupInfo;
	int nIndex, nItemIndex;
	CString strPortName;
	nItemIndex = 0;
	for (nIndex = 0; nIndex < m_plistPortInfo->GetCount(); nIndex++)
	{
		pos = m_plistPortInfo->FindIndex(nIndex);
		stPortGroupInfo = m_plistPortInfo->GetAt(pos);

#if(0)
		if (stPortGroupInfo.nNo < 10)
			strPortName.Format("%s[%d]", stPortGroupInfo.strName, stPortGroupInfo.nItemCnt);
		else
			strPortName.Format("%s[%d]", stPortGroupInfo.strName, stPortGroupInfo.nItemCnt);
#else
		strPortName.Format("%s", stPortGroupInfo.strName.Trim());
#endif
		SetTreeItem(m_htiPortPolio, strPortName, m_nTreeIconIndexPort, m_nTreeIconIndexPort, OFFSET_PORT - nItemIndex);
		nItemIndex++;
	}

	Invalidate();
}

XMLTreeInfo CXmlTreeCtrl::GetPortToTreeInfo(PortGroupInfo stPortGroupInfo)
{
	XMLTreeInfo stXMLTreeInfo;

	stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_PORTPOLIO;			//# Item : Common[0] / Order[1] / Language[4] / User Patten[5]

	stXMLTreeInfo.strName = stPortGroupInfo.strName;
	stXMLTreeInfo.strTag = (CString)TAG_TYPE_PORTPOLIO;
	
	stXMLTreeInfo.nNonAlert = 1; // KHD : 한투 Alert용이다.
	stXMLTreeInfo.nCondiType = 0;
	stXMLTreeInfo.nTargetBase_Type = 1;

	stXMLTreeInfo.nGubun = 0;
	stXMLTreeInfo.nGubunDefVal = 0;

	stXMLTreeInfo.nNotUser = 0;
	stXMLTreeInfo.nNotDefVal = 0;
		
	stXMLTreeInfo.strResultViewID = "";
	stXMLTreeInfo.nResultViewDefVal = 0;

	stXMLTreeInfo.nChartValSet = 0;		//# Deawoo [NEW]
	stXMLTreeInfo.strMapFile = (CString)MAP_OTHER_FILE;

	return stXMLTreeInfo;
}

void CXmlTreeCtrl::SetLoadLanguage()
{
	m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_GET_LANGUAGE, (LPARAM)this);
}

void CXmlTreeCtrl::SetLanguageTree(BOOL bUpDate/* = FALSE*/)
{
	if (m_bJustAlert == TRUE)
		return;

	if ((m_htiLanguage == NULL) || (m_plistLanguageInfo == NULL))
		return;

	if (bUpDate == TRUE)
	{
		HTREEITEM hItem, hDelete;
		hItem = GetChildItem(m_htiLanguage);
		while (hItem != NULL)
		{
			hDelete = hItem;
			hItem = GetNextItem(hItem, TVGN_NEXT);
			DeleteItem(hDelete);
		}
	}
	
	POSITION pos;
	LanguageInfo  stLanguageInfo;
	for (int nIndex = 0; nIndex < m_plistLanguageInfo->GetCount(); nIndex++)
	{
		pos = m_plistLanguageInfo->FindIndex(nIndex);
		stLanguageInfo = m_plistLanguageInfo->GetAt(pos);
		
		SetTreeItem(m_htiLanguage, (LPCTSTR)stLanguageInfo.lpszName, 
			m_nTreeIconIndexNonAlert, 
			m_nTreeIconIndexNonAlert, OFFSET_LANGUAGE - nIndex);
	}
	
	Invalidate();
}

void CXmlTreeCtrl::SetReloadLanguage()
{
	SetLoadLanguage();
	SetLanguageTree(TRUE);
}

XMLTreeInfo CXmlTreeCtrl::GetLanguageToTreeInfo(LanguageInfo stLanguageInfo)
{
	XMLTreeInfo stXMLTreeInfo;
	
	stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_LANGUAGE;			//# Item : Common[0] / Order[1] / Language[4] / User Patten[5]
	
	stXMLTreeInfo.strName = (CString)stLanguageInfo.lpszName;
	stXMLTreeInfo.strTag = (CString)TAG_TYPE_LANGUAGE;
	
	stXMLTreeInfo.nNonAlert = 1;
	stXMLTreeInfo.nCondiType = 0;
	stXMLTreeInfo.nTargetBase_Type = 0;
	
	stXMLTreeInfo.nGubun = 0;
	stXMLTreeInfo.nGubunDefVal = 0;
	
	stXMLTreeInfo.nNotUser = 0;
	stXMLTreeInfo.nNotDefVal = 0;
	
	stXMLTreeInfo.strResultViewID = "";
	stXMLTreeInfo.nResultViewDefVal = 0;
	
	stXMLTreeInfo.nChartValSet = 0;		//# Deawoo [NEW]
	stXMLTreeInfo.strMapFile = (CString)MAP_OTHER_FILE;

  /*
	stXMLTreeInfo.nChartValSet = stLanguageInfo.nType;
	stXMLTreeInfo.strMapFile = (CString)stLanguageInfo.lpszName + ".sts";
  */

	return stXMLTreeInfo;
}


void CXmlTreeCtrl::SetLoadNews()
{
	m_listNewsInfo.RemoveAll();

	/*
	int nIndex;
	NewsInfo	stInfoNews;

	CString strRoot;
	strRoot = m_strAppPath + FOLDER_NEWS + NEWS_KEYWORD_FILE;

	CString strTemp;
	CStdioFile fNewsKeyWord(strRoot, CFile::modeRead);

	try {
		int nNewsIndex = 0;
		while (fNewsKeyWord.ReadString(strTemp))
		{
			int nFindPos, nStartPos;
			CString strResult;

			nStartPos = 0;
			nFindPos = 0;
			nIndex = 0;
			while (nFindPos > -1)
			{
				nFindPos = strTemp.Find(":", nStartPos);
				strResult = strTemp.Mid(nStartPos, nFindPos - nStartPos);
				nStartPos = nFindPos + 1;

				if (nIndex == 0)	//# code
					stInfoNews.nCode = atoi(strResult);
				else if (nIndex == 1)		//# Type
				{
					if (strResult.Compare("{S}") < 0)
						break;
					else
						stInfoNews.strType = strResult;
				}
				else if (nIndex == 3)		//# keyword
					stInfoNews.strstrKeyWord = strResult;

				nIndex++;
			}

			if (stInfoNews.strType == "{S}")
			{
				stInfoNews.nIndex = nNewsIndex;
				m_listNewsInfo.AddTail(stInfoNews);
				//TRACE(_T("%s\n"), strTemp);
				nNewsIndex++;
			}
		}
	}
	catch(...) { }
	
	fNewsKeyWord.Close();
	*/
}

void CXmlTreeCtrl::SetNewsTree(BOOL bUpDate/* = FALSE*/)
{
	if (m_htiNews == NULL)
		return;

	if (bUpDate == TRUE)
	{
		HTREEITEM hItem, hDelete;
		hItem = GetChildItem(m_htiNews);
		while (hItem != NULL)
		{
			hDelete = hItem;
			hItem = GetNextItem(hItem, TVGN_NEXT);
			DeleteItem(hDelete);
		}
	}

  //# Read News Info Insert!
	POSITION pos;
	NewsInfo	stNewsInfo;
	for (int nIndex = 0; nIndex < m_listNewsInfo.GetCount(); nIndex++)
	{
		pos = m_listNewsInfo.FindIndex(nIndex);
		stNewsInfo = m_listNewsInfo.GetAt(pos);

    SetTreeItem(m_htiNews, stNewsInfo.strKeyWord, m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, OFFSET_NEWS - nIndex);
	}	
}

XMLTreeInfo CXmlTreeCtrl::GetNewsToTreeInfo(NewsInfo	stNewsInfo)
{
	XMLTreeInfo stXMLTreeInfo;

	stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_NEWS;

	stXMLTreeInfo.strName = (CString)stNewsInfo.strKeyWord;
	stXMLTreeInfo.strTag = (CString)TAG_TYPE_NEWS;

	stXMLTreeInfo.nNonAlert = 1;
	stXMLTreeInfo.nCondiType = 0;
	stXMLTreeInfo.nTargetBase_Type = 1;

	stXMLTreeInfo.nGubun = 0;
	stXMLTreeInfo.nGubunDefVal = 0;

	stXMLTreeInfo.nNotUser = 0;
	stXMLTreeInfo.nNotDefVal = 0;
		
	stXMLTreeInfo.strResultViewID = "";
	stXMLTreeInfo.nResultViewDefVal = 0;

	stXMLTreeInfo.nChartValSet = 0;		//# Deawoo [NEW]
	stXMLTreeInfo.strMapFile = (CString)MAP_OTHER_FILE;

	return stXMLTreeInfo;
}


HTREEITEM CXmlTreeCtrl::SetTreeItem(HTREEITEM htiParent, CString strItem, 
										int nTreeIconIndex, int nImgSelIndex, int lParam) 
{
	TV_INSERTSTRUCT tvstruct;
	
	tvstruct.hParent = htiParent;
	tvstruct.hInsertAfter = TVI_LAST;
		
	tvstruct.item.pszText = (LPTSTR)(LPCSTR)strItem;	
		
	tvstruct.item.lParam = (long)lParam;
	tvstruct.item.iImage = nTreeIconIndex;
	tvstruct.item.iSelectedImage = nImgSelIndex;
	tvstruct.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	
	HTREEITEM hItem = InsertItem(&tvstruct);
	Expand(hItem, TVE_COLLAPSE);
	UpdateWindow();
		
	return hItem; 
}

BOOL CXmlTreeCtrl::SetTreeItem_Update(HTREEITEM htiItem, CString strItem, 
										int nTreeIconIndex, int nImgSelIndex)
{
	BOOL bResult = FALSE;

	TVITEM item;

	item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	item.hItem = htiItem;
	item.pszText = strItem.GetBuffer(0);
	item.iImage = nTreeIconIndex;
	item.iSelectedImage = nImgSelIndex;
	
	GetItem(&item);

	bResult = SetItem(&item);
	Expand(htiItem, TVE_COLLAPSE);
	UpdateWindow();
		
	return bResult; 
}

void CXmlTreeCtrl::SetInitTree(MSXML::IXMLDOMNodeList *pNodes, HTREEITEM hti)
{
	if (pNodes == NULL) 
		return; 
	
	BOOL bTreeInsertFlag = FALSE;
	int nTreeIconIndex = 0;
	
	CString strNodeName, strAttName;
	CString strNodeVal;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;	
	
	try {
		for (long idx = 0; idx < pNodes->length; idx++) 
		{
			MSXML::IXMLDOMNodePtr pNode = pNodes->item[idx];
			if (!wcscmp((const wchar_t *)pNode->nodeTypeString,L"element"))
			{
				strNodeName = BSTR(pNode->nodeName);			
				if (SUCCEEDED(pNode->get_attributes(&pattrs))) 
				{
					MSXML::IXMLDOMNodePtr pDomNode = pattrs->item[0];
					if ((pattrs->length) > 0) 
					{
						_bstr_t bstrName = pDomNode->nodeName;
						strAttName = (LPTSTR)(LPCTSTR)bstrName;
						
						//# 2004. 10. 25
						bTreeInsertFlag = TRUE;
						
						if (!strAttName.Compare("NAME"))
						{
							nTreeIconIndex = m_nTreeIconIndex1St;
							
							_variant_t variantValue = pDomNode->nodeValue;
							_bstr_t bstrValue(variantValue);
							strNodeVal = (LPTSTR)(LPCTSTR)bstrValue;
							
							MSXML::IXMLDOMNodePtr	pDomNode1St;
							CString			strAttName1St;
							CString			strNodeVal1St;
							XMLTreeInfo		stXMLTreeInfo;
							
							if ((m_strLoadType == "0") || (m_strLoadType == "01"))
								stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_COMM;
							else if ((m_strLoadType == "1") || (m_strLoadType == "11"))
								stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_ORDER;
							
							stXMLTreeInfo.strName = strNodeVal; 
							stXMLTreeInfo.strTag = strNodeName;
							
							if (pDomNode1St = pattrs->item[1])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								
								if (!strAttName1St.Compare("NON_ALERT"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									
									stXMLTreeInfo.nNonAlert = atoi((LPCTSTR)bstrValue1St);
									
									if ((m_bJustAlert == TRUE) && (stXMLTreeInfo.nNonAlert == 1))
										continue;
									
									if (stXMLTreeInfo.nNonAlert == 1)	//#<= Non Alert
										nTreeIconIndex = m_nTreeIconIndexNonAlert;
									else
										nTreeIconIndex = m_nTreeIconIndexNormal;
								}
							}
							
							if ((nTreeIconIndex == m_nTreeIconIndexNonAlert) || (nTreeIconIndex == m_nTreeIconIndexNormal))
							{
								int nIndex = 2;
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("CONDI_TYPE"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nCondiType = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("TARGETBASE_TYPE"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nTargetBase_Type = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("GUBUN"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nGubun = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("GUBUN_DEFVAL"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nGubunDefVal = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("NOT_USER"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nNotUser = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("NOT_DEFVAL"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nNotDefVal = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("RESULT_VIEW_ID"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.strResultViewID = (LPCTSTR)bstrValue1St;
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("RESULT_VIEW_DEFVAL"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nResultViewDefVal = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("CHART_VALSET"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.nChartValSet = atoi((LPCTSTR)bstrValue1St);
									}
									nIndex++;
								}
								
								if (pDomNode1St = pattrs->item[nIndex])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									if (!strAttName1St.Compare("FILE_NAME"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfo.strMapFile = (LPTSTR)(LPCTSTR)bstrValue1St;
									}
								}
								
								m_listXMLTreeInfo.AddTail(stXMLTreeInfo);
							}					
						}
						else if (!strAttName.Compare("TYPE"))
						{
							strNodeVal = strNodeName;
							nTreeIconIndex = m_nTreeIconIndex0St;
							
							if (strNodeVal == PORTPOLIO_SUBROOTTITLE)
							{
								if (m_bIndustry == FALSE)
									m_htiPortPolio = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == ACCOUNT_SUBROOTTITLE)
							{
								m_htiAccount = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == LANGUAGE_SUBROOTTITLE)
							{
								if (m_bIndustry == FALSE)
								{
									if (m_bIsFnUser == TRUE)
										m_htiLanguage = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								}
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == NEWS_SUBROOTTITLE)
							{
								m_htiNews = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == PATTEN_ROOTTITLE)
							{
								m_htiPatten = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == BOOKMARK_SUBROOTTITLE)
							{
								m_htiBookMark = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == USER_ROOTTITLE)
							{
								nTreeIconIndex = m_nTreeIconIndex0St_File;
								
								m_htiUser = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
								bTreeInsertFlag = FALSE;
							}
							else if (strNodeVal == RECOMMEND_ROOTTITLE)
							{
// 								nTreeIconIndex = m_nTreeIconIndex0St_File;
// 								
// 								m_htiRecomm = SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
// 								bTreeInsertFlag = FALSE;
							}
						}
						//# 2004. 10. 25
						//bTreeInsertFlag = TRUE;
					}
					else		//# Main Root
						bTreeInsertFlag = FALSE;
				}
				
				if (bTreeInsertFlag == TRUE && strNodeVal != RECOMMEND_ROOTTITLE 
					&& strNodeVal != LANGUAGE_ROOTTITLE)
				{
					HTREEITEM	hti2;
					if ((nTreeIconIndex == m_nTreeIconIndexNormal) || (nTreeIconIndex == m_nTreeIconIndexNonAlert))
					{
						hti2	= SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex, m_nXmlListIndex);
						m_nXmlListIndex++;
					}
					else
						hti2	= SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
					
					if ((nTreeIconIndex != m_nTreeIconIndexNormal) && (nTreeIconIndex != m_nTreeIconIndexNonAlert))
						SetInitTree(pNode->childNodes, hti2);
				}
				else
					SetInitTree(pNode->childNodes, hti);
			}
			pNode = NULL;
		}
		pattrs = NULL;
	}
	catch(...){ }
}

void CXmlTreeCtrl::SetInitTreeFile(MSXML::IXMLDOMNodeList *pNodes, HTREEITEM hti, CString strLoadType, CString strFoldPath)
{
	if(pNodes == NULL) 
		return; 
	
	int nXmlListIndex;
	BOOL bTreeInsertFlag = FALSE;
	int nTreeIconIndex = 0;
	
	CString strNodeName, strAttName;
	CString strNodeVal;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;	
	
	for (long idx = 0; idx < pNodes->length; idx++) 
	{
		MSXML::IXMLDOMNodePtr pNode = pNodes->item[idx];
		
		if (!wcscmp((const wchar_t *)pNode->nodeTypeString,L"element"))
		{
			strNodeName = BSTR(pNode->nodeName);			
			if( SUCCEEDED(pNode->get_attributes(&pattrs)) ) 
			{
				bTreeInsertFlag = FALSE;
				
				if (!strNodeName.Compare("FOLDER"))
					nTreeIconIndex = m_nTreeIconIndex0St_File;
				else if (!strNodeName.Compare("CONDITION"))
					nTreeIconIndex = m_nTreeIconIndex_NormalFile;
				
				MSXML::IXMLDOMNodePtr pDomNode = pattrs->item[0];
				if( (pattrs->length) > 0 ) 
				{
					_bstr_t bstrName = pDomNode->nodeName;
					strAttName = (LPTSTR)(LPCTSTR)bstrName;
					
					if (!strAttName.Compare("NAME"))
					{
						_variant_t variantValue = pDomNode->nodeValue;
						_bstr_t bstrValue(variantValue);
						strNodeVal = (LPTSTR)(LPCTSTR)bstrValue;
						
						MSXML::IXMLDOMNodePtr	pDomNode1St;
						CString					strAttName1St;
						CString					strNodeVal1St;
						
						XMLTreeInfoFile		stXMLTreeInfoFile;
						stXMLTreeInfoFile.strFolderPath = strFoldPath;
						stXMLTreeInfoFile.strFileName = strNodeVal;
						if (pDomNode1St = pattrs->item[1])
						{
							_bstr_t bstrAttName1St = pDomNode1St->nodeName;
							strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
							if (!strAttName1St.Compare("PATH"))
							{
								_variant_t variantValue1St = pDomNode1St->nodeValue;
								_bstr_t bstrValue1St(variantValue1St);
								stXMLTreeInfoFile.strFilePath = (LPCTSTR)bstrValue1St;
							}
						}
						
						if (nTreeIconIndex == m_nTreeIconIndex0St_File)
							strFoldPath = stXMLTreeInfoFile.strFilePath;
						else
						{
							if (nTreeIconIndex == m_nTreeIconIndex_NormalFile)
							{
								if (pDomNode1St = pattrs->item[2])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									
									if (!strAttName1St.Compare("ALERTREG"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfoFile.strAlertReg = CString((PCHAR)bstrValue1St);
									}
								}
								
								if (pDomNode1St = pattrs->item[3])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									
									if (!strAttName1St.Compare("UNITCAN"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfoFile.bUnitCan = (CString((PCHAR)bstrValue1St) == "TRUE") ? TRUE : FALSE;
									}
								}
								
								nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE,
									stXMLTreeInfoFile.bUnitCan);
								
								if (pDomNode1St = pattrs->item[4])
								{
									_bstr_t bstrAttName1St = pDomNode1St->nodeName;
									strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
									
									if (!strAttName1St.Compare("QUICKREG"))
									{
										_variant_t variantValue1St = pDomNode1St->nodeValue;
										_bstr_t bstrValue1St(variantValue1St);
										stXMLTreeInfoFile.bQuickReg = (CString((PCHAR)bstrValue1St) == "TRUE") ? TRUE : FALSE;
									}
								}
								
								if (strLoadType == "2")
								{
									if (pDomNode1St = pattrs->item[5])
									{
										_bstr_t bstrAttName1St = pDomNode1St->nodeName;
										strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
										
										if (!strAttName1St.Compare("EXPLANATION"))
										{
											_variant_t variantValue1St = pDomNode1St->nodeValue;
											_bstr_t bstrValue1St(variantValue1St);
											stXMLTreeInfoFile.strExplain = CString((PCHAR)bstrValue1St);
										}
									}
								}
								else
									stXMLTreeInfoFile.strExplain = "";
							}
							
							stXMLTreeInfoFile.strType = (strLoadType == "2")?"SYS":"USER";
							if (strLoadType == "2")
							{
								m_listSysInfoFile.AddTail(stXMLTreeInfoFile);
								
								m_nXmlSysListIndex++;
								nXmlListIndex = OFFSET_SYSFILE + m_nXmlSysListIndex;
							}
							else if (strLoadType == "3")
							{
								m_listUserInfoFile.AddTail(stXMLTreeInfoFile);
								
								m_nXmlUserListIndex++;
								nXmlListIndex = OFFSET_USERFILE + m_nXmlUserListIndex;
							}
							else
							{
								m_listXMLTreeInfoFile.AddTail(stXMLTreeInfoFile);
								
								m_nXmlListIndex++;
								nXmlListIndex = m_nXmlListIndex;
							}
						}
						
						bTreeInsertFlag = TRUE;
					}
				}
			}
			
			if (bTreeInsertFlag == TRUE)
			{
				HTREEITEM	hti2;
				if (nTreeIconIndex == m_nTreeIconIndex0St_File)
					hti2	= SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
				else if (nTreeIconIndex == m_nTreeIconIndex1St)
					hti2	= SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex + 1, -1);
				else
					hti2	= SetTreeItem(hti, strNodeVal, nTreeIconIndex, nTreeIconIndex, nXmlListIndex - 1);
				
				SetInitTreeFile(pNode->childNodes, hti2, strLoadType, strFoldPath);
			}
			else
				SetInitTreeFile(pNode->childNodes, hti, strLoadType, strFoldPath);
		}
		pNode = NULL;
	}
	pattrs = NULL;	
}


void CXmlTreeCtrl::SetItme(BOOL bModify, CString strItem, BOOL bAlertReg, BOOL bUnitCan)
{
	CString strFolderPath = GetFolderPath(m_strLoadType);
	
	int nTreeIconIndex;
	nTreeIconIndex = GetTreeIconIndex_File(bAlertReg, bUnitCan);
	
	XMLTreeInfoFile		stXMLTreeInfoFile;
	if (bModify == TRUE)
	{
		CArray <HTREEITEM, HTREEITEM>		arFindItem;
		GetFindTreeItem(m_htiRoot, strItem, arFindItem);
		if (arFindItem.GetSize() > 0)
		{
			DeleteItem(arFindItem.GetAt(0));
			
			POSITION pos;
			for (int nIndex = 0; nIndex < m_listXMLTreeInfoFile.GetCount(); nIndex++)
			{
				pos = m_listXMLTreeInfoFile.FindIndex(nIndex);
				stXMLTreeInfoFile = m_listXMLTreeInfoFile.GetAt(pos);
				
				if (stXMLTreeInfoFile.strFileName == strItem)
				{
					m_listXMLTreeInfoFile.RemoveAt(pos);
					break;
				}
			}
		}
	}
	
	stXMLTreeInfoFile.strFolderPath = strFolderPath;
	stXMLTreeInfoFile.strFileName = strItem;
	stXMLTreeInfoFile.strFilePath = strItem + ".xml";
	stXMLTreeInfoFile.bQuickReg = FALSE;
	stXMLTreeInfoFile.strAlertReg = (bAlertReg == TRUE)?"TRUE":"FALSE";
	stXMLTreeInfoFile.bUnitCan = bUnitCan;
	m_listXMLTreeInfoFile.AddTail(stXMLTreeInfoFile);
	
	SetTreeItem(m_htiRoot, strItem, nTreeIconIndex, nTreeIconIndex, m_listXMLTreeInfoFile.GetCount() - 1);
	if (bModify == TRUE)
		Invalidate();
}


void CXmlTreeCtrl::SetItme_User(BOOL bModify, XMLTreeInfoFile	stXMLTreeInfoFile)
{
	int nTreeIconIndex;
	nTreeIconIndex = GetTreeIconIndex_File((stXMLTreeInfoFile.strAlertReg == "TRUE")?TRUE:FALSE, 
		stXMLTreeInfoFile.bUnitCan);
	
	POSITION pos;
	XMLTreeInfoFile		stInfoFile;
	HTREEITEM	htiFolder, htiFile;
	if (GetFindTreeItem(m_htiUser, stXMLTreeInfoFile.strFolderPath, htiFolder) == FALSE)
		htiFolder= SetTreeItem(m_htiUser, stXMLTreeInfoFile.strFolderPath, m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, -1);
	
	Expand(m_htiUser, TVE_EXPAND);
	Expand(htiFolder, TVE_EXPAND);
	
	HTREEITEM	htiSelect;
	if (bModify == TRUE)
	{
		for (int nIndex = 0; nIndex < m_listUserInfoFile.GetCount(); nIndex++)
		{
			pos = m_listUserInfoFile.FindIndex(nIndex);
			stInfoFile = m_listUserInfoFile.GetAt(pos);
			
			if ((stInfoFile.strFolderPath == stXMLTreeInfoFile.strFolderPath) && 
				(stInfoFile.strFileName == stXMLTreeInfoFile.strFileName))
			{
				m_listUserInfoFile.SetAt(pos, stXMLTreeInfoFile);
				break;
			}
		}
		
		GetFindTreeItem(htiFolder, stXMLTreeInfoFile.strFileName, htiFile);
		SetTreeItem_Update(htiFile, stInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex);
		htiSelect = htiFile;
	}
	else
	{
		m_listUserInfoFile.AddTail(stXMLTreeInfoFile);
		htiSelect = SetTreeItem(htiFolder, stXMLTreeInfoFile.strFileName, nTreeIconIndex, nTreeIconIndex, OFFSET_USERFILE + m_nXmlUserListIndex);
		m_nXmlUserListIndex++;
	}
	
	m_stSelectInfoFile = stXMLTreeInfoFile;
	m_nSeleteType = TREE_SELECT_TYPE_USERFILE;
	
	SelectItem(htiSelect);
	Invalidate();
}


void CXmlTreeCtrl::SetDeleteItme(CString strItem)
{
	BOOL bState = FALSE;
	
	strItem.TrimLeft(); 
	strItem.TrimRight();
	
	if (SetMessageBox(CString("조건(") + strItem + ") 을 삭제하려 합니다.\n삭제하시겠습니까?", (CString)PROGRAM_TITLE, MB_YESNO) == IDYES)
	{
		POSITION pos;
		XMLTreeInfoFile		stInfoFile;
		int nCount = m_listXMLTreeInfoFile.GetCount();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pos = m_listXMLTreeInfoFile.FindIndex(nIndex);
			stInfoFile = m_listXMLTreeInfoFile.GetAt(pos);
			
			if (strItem.Compare(stInfoFile.strFileName) == 0)
			{
				if ((SetDeleteTreeItem(strItem) == TRUE) && (SetDeleteFile(strItem) == TRUE))
				{
					m_listXMLTreeInfoFile.RemoveAt(pos);
					bState = TRUE;
				}
				break;
			}
		}
	}
	
	if (bState == FALSE)
	{
		CString strErrorMsg;
		
		strErrorMsg.Format("조건파일(%s)을 삭제하지 못하였습니다.", strItem + ".xml");
		SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
	}
}

BOOL CXmlTreeCtrl::SetDeleteTreeItem(CString strItem)
{
	BOOL bResult = FALSE;
	
	CString strFolderPath = GetFolderPath(m_strLoadType);
	
	CString strLoadType = m_strLoadType;
	CString strRootName;
	if (strLoadType == "2"){}			//# 2 : Recommend
		/*strRootName = (CString)RECOMMEND_ROOTTITLE; */
	else if (strLoadType == "3") //# 3 : user
		strRootName = (CString)USER_ROOTTITLE;
	else if (strLoadType == "4"){}		//# 4 : Language
		/*strRootName = (CString)LANGUAGE_ROOTTITLE;*/
	
	HTREEITEM hRoot, hChild, hTemp;
	CString strDel;
	
	hTemp = GetRootItem();
	while(hTemp != NULL)
	{
		strRootName = GetItemText(hTemp);
		if (strRootName.Compare(strRootName) == 0)
		{
			hRoot = hTemp;
			break;
		}
		
		hTemp = GetNextSiblingItem(hTemp);
	}
	
	hTemp = GetChildItem(hRoot);
	while (hTemp != NULL)
	{
		strDel = GetItemText(hTemp);
		if (strItem.Compare(strDel) == 0)
		{
			if (!ItemHasChildren(hTemp))
			{
				hChild = hTemp;
				DeleteItem(hChild);
				bResult = TRUE;
				break;
			}
		}
		hTemp = GetNextItem(hTemp, TVGN_NEXT);
	}
	
	return bResult;
}

BOOL CXmlTreeCtrl::SetDeleteFile(CString strItem)
{
	BOOL bResult = FALSE;
	
	try 
	{
		CString strFolderPath = GetFolderPath(m_strLoadType);
		CFile::Remove(strFolderPath + "\\" + strItem + ".xml");
		
		bResult = TRUE;
	} 
	catch(...) { }
	
	return bResult;
}

void CXmlTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE("\n# CXmlTreeCtrl => OnLButtonDown");
	
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
			GetSelectItem(point);

// 		HTREEITEM hItem = GetParentItem(hSelItem);                         
// 		hItem = GetParentItem(hItem);
// 		if(GetItemText(hItem) == "사용자조건" || GetItemText(hItem) == "즐겨찾기")
// 			DragItem(hSelItem, point);
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXmlTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	TRACE("\n# CXmlTreeCtrl => OnLButtonUp");
	
	
// 	if(((CConditionListView*)GetParent())->m_nSeparateForm)
// 	{
// 		OnLButtonDblClk(1, point);
// 	}
	

	if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		if(m_pDragImage != NULL) 
		{ 
			delete m_pDragImage; 
			m_pDragImage = NULL; 
		} 

		HTREEITEM hSelItem = HitTest(point, &nFlags);

		if(!(GetItemText(GetParentItem(hSelItem)) != USER_ROOTTITLE || GetItemText(GetParentItem(hSelItem)) != BOOKMARK_SUBROOTTITLE))
		{	
			Invalidate();
			return;
		}

		SelectDropTarget(NULL);

		if(m_hitemDrag == m_hitemDrop)
			return;

		
		HTREEITEM hParent = NULL;
		
		if(m_nCaptureItem == 0)
			hParent = GetParentItem(GetParentItem(m_hitemDrop));
		else if(m_nCaptureItem == 1)
		{
			hParent = GetParentItem(m_hitemDrop);
			if(hParent != m_htiBookMark)
				return;
		}
			

		HTREEITEM	hitem;
		if (((hitem = HitTest(point, &nFlags)) == NULL)) return ;
		
		/*if (GetParentItem(hitem) != NULL) return ;*/

		HTREEITEM htiParent = m_hitemDrop;
		while((htiParent = GetParentItem(htiParent)) != NULL)
		{
			if( htiParent == m_hitemDrag ) return;
		}

		Expand(m_hitemDrop, TVE_EXPAND) ;
		
		if(m_nCaptureItem == 0)
		{
			if(MoveUserXmlFile(GetItemText(GetParentItem(m_hitemDrag)), 
				GetItemText(hSelItem), GetItemText(m_hitemDrag)))
			{
				HTREEITEM htiNew = MoveChildItem(m_hitemDrag, m_hitemDrop, TVI_LAST);
				DeleteItem(m_hitemDrag);
				SelectItem(htiNew);
			}
		}
		else if(m_nCaptureItem)
		{
			ChangeMarkSort(GetItemText(m_hitemDrag), GetItemText(hSelItem), m_htiBookMark);
			SelectItem(hSelItem);
		}
		
	
		Invalidate();
	}
	
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CXmlTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	TRACE("\n# CXmlTreeCtrl => OnLButtonDblClk");

	if (m_nSeleteType > -1)
	{
		UINT uFlags;
		HTREEITEM hSelItem = HitTest(point, &uFlags);
		if (hSelItem != NULL)
		{
			if (TVHT_ONITEM & uFlags)
				TRACE("\n# TVHT_ONITEM");		//# item
			else
				return;
			SelectItem(hSelItem);
		}
		else
			return;
		
		if (GetChildItem(hSelItem) == NULL)
			SetSelectData_Send("ADD", point);
		else
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_CLASS, (LPARAM)NULL);
	}
	else
		SetSelectData_Send("ADD", point);
	
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CXmlTreeCtrl::SetSelectData_Send(CString strMode, CPoint point)
{
	if (strMode == "SELECT")
	{
		m_nSeleteClass = SetSelectData(point);
		if (m_nSeleteClass == 0)
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_CLASS, (LPARAM)NULL);
		else if (m_nSeleteClass == 1)
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_ITEM, (LPARAM)&m_stSelectInfo);
		else if (m_nSeleteClass == 2)
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_FILE, (LPARAM)&m_stSelectInfoFile);
		else
			TRACE("\n# CXmlTreeCtrl -> SetSelectData Error!");
	}
	else if (strMode == "ADD")
	{
		m_nSeleteClass = SetSelectData(point);
		if (m_nSeleteClass == 0)
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_CLASS, (LPARAM)NULL);
		else if (m_nSeleteClass == 1)
		{
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_ADD_ITEM, (LPARAM)&m_stSelectInfo);
		}
		else if (m_nSeleteClass == 2)
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_LOAD_FILE, (LPARAM)&m_stSelectInfoFile);
		else
			TRACE("\n# CXmlTreeCtrl -> SetSelectData Error!");
	}
	else if (strMode == "MODIFY")
	{
		
	}
}

int CXmlTreeCtrl::SetSelectData(CPoint point)
{
	int nResult = -1;
	
	POSITION pos;
	int nItemIndex;
	CString strItem = _T("");
	
	m_nSeleteType = GetSelectItem(strItem, nItemIndex, point);
	if ((m_nSeleteType == TREE_SELECT_TYPE_USERFILE) || (m_nSeleteType == TREE_SELECT_TYPE_SYSFILE))
	{
		m_nSeleteType = GetSelectFile(m_stSelectInfoFile, point);
		nResult = 2;
	}
	else if (m_nSeleteType == (int)TREE_SELECT_TYPE_CLASS)
		nResult = 0;
	else
	{
		if ((m_nSeleteType == (int)TREE_SELECT_TYPE_ITEM_COMM) ||
			(m_nSeleteType == (int)TREE_SELECT_TYPE_ITEM_ORDER))
		{
			if (GetSelectItem(m_stSelectInfo, point) == TRUE)		//# Item, Mark Item
				nResult = 1;
			else
				return nResult;
		}
		else
		{
			if (m_nSeleteType == TREE_SELECT_TYPE_PORTPOLIO)		//# Port Polio
			{
				if (m_plistPortInfo == NULL)
					return nResult;
				
				PortGroupInfo stPortGroupInfo;
				pos = m_plistPortInfo->FindIndex(nItemIndex);
				if (pos != NULL)
				{
					stPortGroupInfo = m_plistPortInfo->GetAt(pos);
					m_stSelectInfo = GetPortToTreeInfo(stPortGroupInfo);
				}
				else
					return nResult;
			}
			else if (m_nSeleteType == TREE_SELECT_TYPE_ACCOUNT)		//# Account
			{
				
			}
			else if (m_nSeleteType == TREE_SELECT_TYPE_LANGUAGE)		//# Language
			{
				if (m_plistLanguageInfo == NULL)
					return nResult;
				
				LanguageInfo    stLanguageInfo;
				pos = m_plistLanguageInfo->FindIndex(nItemIndex);
				if (pos != NULL)
				{
					stLanguageInfo = m_plistLanguageInfo->GetAt(pos);
					m_stSelectInfo = GetLanguageToTreeInfo(stLanguageInfo);
				}
				else
					return nResult;
			}
			else if (m_nSeleteType == TREE_SELECT_TYPE_NEWS)		//# News
			{
				NewsInfo	stNewsInfo;
				pos = m_listNewsInfo.FindIndex(nItemIndex);
				if (pos != NULL)
				{
					stNewsInfo = m_listNewsInfo.GetAt(pos);
					m_stSelectInfo = GetNewsToTreeInfo(stNewsInfo);
				}
				else
					return nResult;
			}
			else if (m_nSeleteType == TREE_SELECT_TYPE_USERPATTEN)		//# User Patten
				m_stSelectInfo = GetPattenToTreeInfo(strItem);
			else
				return nResult;
			
			nResult = 1;
		}
	}
	
	return nResult;
}

void CXmlTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void CXmlTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	BringWindowToTop();
	
	*pResult = 0;
}

void CXmlTreeCtrl::GetSelectItem(CPoint point)
{
	if ((point.x == 0) && (point.y == 0))
		TRACE("Not Select Item!");
	else
		SetSelectData_Send("SELECT", point);
    
	if (m_bSendMsgThis == FALSE)
	{
		//# Send to CWndFrame Class : 선택한 Item의 Tree경로
		CWnd* pParentWnd = m_pwndMsgTarget->GetParent();			//CEzFinderMainView
		pParentWnd = pParentWnd->GetParent();			//CWndFrame
		
														/*
														CString strItemPath = GetItemPath(GetSelectedItem());
														char buf[3] = "10";
														pParentWnd->SendMessage(WMU_DISP_MSG, 
														(WPARAM)buf, (LPARAM)(LPCTSTR)strItemPath);
		*/
	}
}

void CXmlTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//return; // 트리에서는 메인에서의 우버튼 메뉴가 나오지 않도록 Pretrans에서 처리
	
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
		{
			TRACE("\n# TVHT_ONITEM");		//# item
		}
		else
		{
			CTreeCtrl::OnRButtonDown(nFlags, point);
			return;
		}
		
		if (TVHT_ONITEM & uFlags)
		{
			GetSelectItem(point);
			SelectItem(hSelItem);
		}
	}
	else
	{
		CTreeCtrl::OnRButtonDown(nFlags, point);
		return;
	}
	
	// 랭귀지이면 메인쪽에 메시지 보낸다. 메인에서 팝업메시지 보낸다.
	if(m_htiLanguage==hSelItem)
		SetSendPopupMenu(3, point);

	if ((m_strLoadType == "0") || (m_strLoadType == "1") ||
		(m_strLoadType == "01") || (m_strLoadType == "11"))
	{
		if (GetChildItem(hSelItem) == NULL)
		{
			TVITEM item;
			TCHAR szText[1024];
			item.hItem = hSelItem;
			item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
			item.pszText = szText;
			item.cchTextMax = 1024;
			

			BOOL bWorked = GetItem(&item);
			if (bWorked == TRUE)
			{
				if ((item.iImage == m_nTreeIconIndexNormal) || 
					(item.iImage == m_nTreeIconIndexNonAlert))	//#<= Normal, Alert
				{
					if (m_htiBookMark == GetParentItem(hSelItem))
						SetSendPopupMenu(1, point);
					else 
					{
						if ((m_htiPortPolio != GetParentItem(hSelItem)) && 
							(m_htiPortPolio != hSelItem) && (m_htiBookMark != hSelItem))
							SetSendPopupMenu(0, point, hSelItem);
					}
				}
				else if((item.iImage == m_nTreeIconIndex_NormalFile) ||
					(item.iImage == m_nTreeIconIndex_NormalFile_Unit) ||
					(item.iImage == m_nTreeIconIndex_AlertFile_Unit) )
				{
					HTREEITEM hRoot1, hRoot0;
					hRoot1 = GetParentItem(hSelItem);
					if(hRoot1)
					{
						hRoot0 = GetParentItem(hRoot1);
						if(hRoot0)
						{
							CString szText = GetItemText(hRoot0);
							if(szText.Compare(USER_ROOTTITLE)==0)
								SetSendPopupMenu(2, point);
						}
					}
				}
			}
		}
	}
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CXmlTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
		{
			TRACE("\n# TVHT_ONITEM");		//# item
		}
		else
		{
			CTreeCtrl::OnRButtonUp(nFlags, point);
			return ;
		}
		
		if (TVHT_ONITEM & uFlags)
		{
			GetSelectItem(point);
			SelectItem(hSelItem);
		}
	}
	else
	{
		CTreeCtrl::OnRButtonUp(nFlags, point);
		return;
	}

	CTreeCtrl::OnRButtonUp(nFlags, point);
}


void CXmlTreeCtrl::SetSendPopupMenu(int nType, CPoint ptMenu, HTREEITEM hSelItem)
{
	CMenu menu;
	menu.CreatePopupMenu();
	
	if (nType == 0)
	{
		menu.AppendMenu(MF_STRING, MENU_ID_ADD, (LPCTSTR)MENU_TITLE_ADD); 

		// 랭귀지이면 언어편집기 실행루틴 추가
		if(IsParentLanguage(hSelItem))
			menu.AppendMenu(MF_STRING, MENU_ID_EXECLANGUAGE_SCRIPT, (LPCTSTR)MENU_EXECLANGUAGE); 
	}
	else if (nType == 1)
		menu.AppendMenu(MF_STRING, MENU_ID_DEL, (LPCTSTR)MENU_TITLE_DEL); 
	else if (nType ==2)
		menu.AppendMenu(MF_STRING, MENU_ID_USERCONDDEL, (LPCTSTR)MENU_USERCOND_DEL); 
	else if (nType ==3)
		menu.AppendMenu(MF_STRING, MENU_ID_EXECLANGUAGE, (LPCTSTR)MENU_EXECLANGUAGE); 

	//menu.SetOwnerDraw();
	
	GetCursorPos(&ptMenu);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);
	menu.DestroyMenu();
}


void CXmlTreeCtrl::OnSendMenu(UINT nIndex)
{
	if (nIndex == MENU_ID_ADD)		//# Add
	{
		if ((m_nSeleteType == TREE_SELECT_TYPE_ITEM_COMM) || (m_nSeleteType == TREE_SELECT_TYPE_ITEM_ORDER))
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_ADD_MARK, (LPARAM)&m_stSelectInfo);
	}
	else if (nIndex == MENU_ID_DEL)		//# Del
	{
		if ((m_nSeleteType == TREE_SELECT_TYPE_ITEM_COMM) || (m_nSeleteType == TREE_SELECT_TYPE_ITEM_ORDER))
			m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_DEL_MARK, (LPARAM)&m_stSelectInfo);
	}
	else if (nIndex == MENU_ID_USERCONDDEL)	// 사용자조건 삭제
	{
		m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_USERCON_DEL, (LPARAM)this);
	}
	else if (nIndex == MENU_ID_EXECLANGUAGE)	// 언어편집기 실행
	{
		// WPARAM 100:언어편집기실행
		// LPARAM 실행할 스크립트, 없으면 NULL
		m_pwndMsgTarget->SendMessage(RMSG_CHECKMENU, (WPARAM)100, (LPARAM)0);
	}
	else if (nIndex == MENU_ID_EXECLANGUAGE_SCRIPT)
	{
		// WPARAM 100:언어편집기실행
		// LPARAM 실행할 스크립트, 없으면 NULL
		HTREEITEM hSelectedItem = GetSelectedItem();

		CString szScript;
		if(hSelectedItem)
			szScript = GetItemText(hSelectedItem);
		m_pwndMsgTarget->SendMessage(RMSG_CHECKMENU, (WPARAM)100, (LPARAM)(LPCSTR)szScript);
	}
}

void CXmlTreeCtrl::SetAddMark(XMLTreeInfo stXMLTreeInfo)
{
	if ((m_nUseBookMarkType == 0) || (m_htiBookMark == NULL))
		return;
	
	if ((m_bJustAlert == TRUE) && (stXMLTreeInfo.nNonAlert == 1))
		return;
	
	POSITION pos;
	if (GetHaveTreeMarkInfo(stXMLTreeInfo, pos) == FALSE)
	{
		m_listXMLTreeMarkInfo.AddTail(stXMLTreeInfo);
		
		int nTreeIconIndex;
		if (stXMLTreeInfo.nNonAlert == 0)
			nTreeIconIndex = m_nTreeIconIndexNormal;
		else
			nTreeIconIndex = m_nTreeIconIndexNonAlert;
		
		SetTreeItem(m_htiBookMark, stXMLTreeInfo.strName, nTreeIconIndex, nTreeIconIndex, 
			OFFSET_MARK - m_nXmlMarkListIndex);

		m_nXmlMarkListIndex++;
	}
}

BOOL CXmlTreeCtrl::SetDelMark(XMLTreeInfo stXMLTreeInfo)
{
	BOOL bResult = FALSE;
	
	if (m_nUseBookMarkType == 0)
		return bResult;
	
	POSITION pos;
	if (GetHaveTreeMarkInfo(stXMLTreeInfo, pos) == TRUE)
	{
		CString strDel;
		HTREEITEM hTemp = GetChildItem(m_htiBookMark);
		while (hTemp != NULL)
		{
			strDel = GetItemText(hTemp);
			if (stXMLTreeInfo.strName.Compare(strDel) == 0)
			{
				if (!ItemHasChildren(hTemp))
				{
					DeleteItem(hTemp);
					bResult = TRUE;
					break;
				}
			}
			hTemp = GetNextItem(hTemp, TVGN_NEXT);
		}
		
		if (bResult == TRUE)
		{
			m_listXMLTreeMarkInfo.RemoveAt(pos);
			UpdateWindow();
		}
	}
	
	return bResult;
}


BOOL CXmlTreeCtrl::GetHaveTreeMarkInfo(XMLTreeInfo stXMLTreeInfo, POSITION &posMark)
{
	BOOL bResult = FALSE;
	
	POSITION pos;
	XMLTreeInfo stTargetXMLTreeInfo;
	for (int nIndex = 0; nIndex < m_listXMLTreeMarkInfo.GetCount(); nIndex++)
	{
		pos = m_listXMLTreeMarkInfo.FindIndex(nIndex);
		stTargetXMLTreeInfo = m_listXMLTreeMarkInfo.GetAt(pos);
		
		if ((stXMLTreeInfo.strTag == stTargetXMLTreeInfo.strTag) &&
			(stXMLTreeInfo.strName == stTargetXMLTreeInfo.strName))
		{
			posMark = pos;
			bResult = TRUE;
			break;
		}
	}
	
	return bResult;
}

int CXmlTreeCtrl::GetSelectItem(CString &strItem, int &nItemIndex, CPoint point)
{
	int nResult = -1;
	
	nItemIndex = -1;
	
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
		{
			SelectItem(hSelItem);
			
			if (GetChildItem(hSelItem) != NULL)
				return TREE_SELECT_TYPE_CLASS;
		}
		else
			return nResult;
	}
	else
		return nResult;
	
	
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;
	
	BOOL bWorked = GetItem(&item);
	if (bWorked == TRUE)
	{
		if (item.state == 2) 
		{
			strItem = item.pszText;
			
			if (item.lParam <= OFFSET_MARK)				//# Mark
			{
				if ((m_strLoadType == "0") || (m_strLoadType == "01"))    //# Comm
					nResult = (int)TREE_SELECT_TYPE_ITEM_COMM;
				else if ((m_strLoadType == "1") || (m_strLoadType == "11"))   //# Order
					nResult = (int)TREE_SELECT_TYPE_ITEM_ORDER;
			}
			else if (item.lParam <= OFFSET_NEWS)	//# News
			{
				nResult = (int)TREE_SELECT_TYPE_NEWS;
				nItemIndex = (int)OFFSET_NEWS - (int)item.lParam;
			}
			else if (item.lParam <= OFFSET_USERPATTEN)	//# User Patten
				nResult = (int)TREE_SELECT_TYPE_USERPATTEN;
			else if (item.lParam <= OFFSET_LANGUAGE)	//# Language
			{
				nResult = (int)TREE_SELECT_TYPE_LANGUAGE;
				nItemIndex = (int)OFFSET_LANGUAGE - (int)item.lParam;
			}
			else if (item.lParam <= OFFSET_ACCOUNT)		//# Accounts
				nResult = (int)TREE_SELECT_TYPE_ACCOUNT;
			else if (item.lParam <= OFFSET_PORT)		//# Port Polio
			{
				nResult = (int)TREE_SELECT_TYPE_PORTPOLIO;
				nItemIndex = (int)OFFSET_PORT - (int)item.lParam;
			}
			else if (item.lParam >= OFFSET_USERFILE)	//# User
				nResult = (int)TREE_SELECT_TYPE_USERFILE;
			else if (item.lParam >= OFFSET_SYSFILE)		//# Sys
				nResult = (int)TREE_SELECT_TYPE_SYSFILE;
			else
			{
				if ((m_strLoadType == "0") || (m_strLoadType == "01"))
					nResult = (int)TREE_SELECT_TYPE_ITEM_COMM;
				else if ((m_strLoadType == "1") || (m_strLoadType == "11"))
					nResult = (int)TREE_SELECT_TYPE_ITEM_ORDER;
			}
		}
	}
	
	return nResult;
}

BOOL CXmlTreeCtrl::GetSelectItem(XMLTreeInfo &stXMLTreeInfo, CPoint point)
{
	BOOL bResult = FALSE;
	
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
			SelectItem(hSelItem);
		else
			return bResult;
	}
	else
		return bResult;
	
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;
	
	BOOL bWorked = GetItem(&item);
	if (bWorked == TRUE)
	{
		if ((item.state == 2) && 
			((item.lParam > -1) || (item.lParam <= OFFSET_MARK)) )
		{
			m_strSelectItemText = item.pszText;
			
			CList	<XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo;
			int nIndex;
			POSITION pos;
			if (item.lParam > -1) 
			{
				plistXMLTreeInfo = &m_listXMLTreeInfo;
				nIndex = (int)item.lParam;
			}
			else if (item.lParam <= OFFSET_MARK)
			{
				plistXMLTreeInfo = &m_listXMLTreeMarkInfo;
				nIndex = ((int)item.lParam * -1) + OFFSET_MARK;
			}
			
			pos = plistXMLTreeInfo->FindIndex(nIndex);
			if (pos != NULL)
			{
				stXMLTreeInfo = plistXMLTreeInfo->GetAt(pos);
				if ((m_strLoadType == "0") || (m_strLoadType == "1") ||
					(m_strLoadType == "01") || (m_strLoadType == "11"))
					bResult = TRUE;
			}
		}
	}
	
	return bResult;
}

BOOL CXmlTreeCtrl::GetSelect_File(XMLTreeInfoFile &stXMLTreeInfoFile)
{
	stXMLTreeInfoFile = m_stSelectInfoFile;
	return m_nSeleteType;
}

BOOL CXmlTreeCtrl::SetSelectItemExcavation_Manual(HTREEITEM hItem)
{
	if(hItem == NULL)
		return FALSE;

	SelectItem(hItem);
	GetSelectFile(hItem);
	m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_LOAD_FILE, (LPARAM)&m_stSelectInfoFile);

	return TRUE;
}

int CXmlTreeCtrl::GetSelectFile(HTREEITEM hItem)
{
	BOOL nResult = -1;
	
	HTREEITEM hSelItem = hItem;
	if (hSelItem != NULL)
	{
		SelectItem(hSelItem);
	}
	else
		return nResult;
	
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;
	
	BOOL bWorked = GetItem(&item);
	if (bWorked == TRUE)
	{
		if ((item.state == 2) && (item.lParam > -1))
		{
			m_strSelectItemText = item.pszText;
			
			POSITION pos;
			CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *plistXMLTreeInfoFile;
			
			if	(item.lParam >= OFFSET_EXCAVATION)
			{
				CString strFileName;
				strFileName.Format("%s", szText);
				strFileName.Replace(" ", "_");
				item.pszText = (LPSTR)(LPCTSTR)strFileName;
				
				plistXMLTreeInfoFile = &m_listItemExcavationInfoFile;
			}
			else if (item.lParam >= OFFSET_USERFILE)
				plistXMLTreeInfoFile = &m_listUserInfoFile;
			else if (item.lParam >= OFFSET_SYSFILE)
				plistXMLTreeInfoFile = &m_listSysInfoFile;
			else
				plistXMLTreeInfoFile = &m_listXMLTreeInfoFile;
			
			for (int nIndex = 0; nIndex < plistXMLTreeInfoFile->GetCount(); nIndex++)
			{
				pos = plistXMLTreeInfoFile->FindIndex(nIndex);
				m_stSelectInfoFile = plistXMLTreeInfoFile->GetAt(pos);
				if (item.pszText == m_stSelectInfoFile.strFileName)
				{
					if (item.lParam >= OFFSET_USERFILE)
						nResult = TREE_SELECT_TYPE_USERFILE;
					else if (item.lParam >= OFFSET_SYSFILE)
						nResult = TREE_SELECT_TYPE_SYSFILE;
					break;
				}
			}
		}
	}
	
	return nResult;
}

int CXmlTreeCtrl::GetSelectFile(XMLTreeInfoFile &stXMLTreeInfoFile, CPoint point)
{
	BOOL nResult = -1;
	
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
		{
			SelectItem(hSelItem);
			
			if (GetChildItem(hSelItem) != NULL)
				return TREE_SELECT_TYPE_CLASS;
		}
		else
			return nResult;
	}
	else
		return nResult;
	
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;
	
	BOOL bWorked = GetItem(&item);
	if (bWorked == TRUE)
	{
		if ((item.state == 2) && (item.lParam > -1))
		{
			m_strSelectItemText = item.pszText;
			
			POSITION pos;
			CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *plistXMLTreeInfoFile;
			
			if	(item.lParam >= OFFSET_EXCAVATION)
			{
				CString strFileName;
				strFileName.Format("%s", szText);
				strFileName.Replace(" ", "_");
				item.pszText = (LPSTR)(LPCTSTR)strFileName;
				
				plistXMLTreeInfoFile = &m_listItemExcavationInfoFile;
			}
			else if (item.lParam >= OFFSET_USERFILE)
				plistXMLTreeInfoFile = &m_listUserInfoFile;
			else if (item.lParam >= OFFSET_SYSFILE)
				plistXMLTreeInfoFile = &m_listSysInfoFile;
			else
				plistXMLTreeInfoFile = &m_listXMLTreeInfoFile;
			
			CString strFolder = FOLDER_TEMPFILE;
			HTREEITEM hParent = GetParentItem(item.hItem);
			if(hParent)
				strFolder = GetItemText(hParent);
			
			for (int nIndex = 0; nIndex < plistXMLTreeInfoFile->GetCount(); nIndex++)
			{
				pos = plistXMLTreeInfoFile->FindIndex(nIndex);
				stXMLTreeInfoFile = plistXMLTreeInfoFile->GetAt(pos);
				if (item.pszText == m_stSelectInfoFile.strFileName && strFolder.CompareNoCase(m_stSelectInfoFile.strFolderPath) == 0)
				{
					if (item.lParam >= OFFSET_USERFILE)
						nResult = TREE_SELECT_TYPE_USERFILE;
					else if (item.lParam >= OFFSET_SYSFILE)
						nResult = TREE_SELECT_TYPE_SYSFILE;
					break;
				}
			}
		}
	}
	
	return nResult;
}

int CXmlTreeCtrl::GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, 
								  CArray<HTREEITEM, HTREEITEM> &saFindItemArray)
{
	if (strFindText.IsEmpty())
		return 0;
	strFindText.MakeUpper();
	
	HTREEITEM htChildNode = GetChildItem(htItemNode);
	while (htChildNode != NULL)
	{
		CString	strItemText =	GetItemText(htChildNode);
		strItemText.MakeUpper();
		if (strItemText.Find((LPCTSTR)strFindText) != -1)
			saFindItemArray.Add(htChildNode);
		
		if (ItemHasChildren(htChildNode))
			int ResultItem = GetFindTreeItem(htChildNode, strFindText, saFindItemArray);
		
		htChildNode = GetNextItem(htChildNode, TVGN_NEXT);
	}
	
	return saFindItemArray.GetSize();
}

BOOL CXmlTreeCtrl::GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, 
								   HTREEITEM &hItem)
{
	BOOL bResult = FALSE;
	
	if (strFindText.IsEmpty())
		return 0;
	strFindText.MakeUpper();
	
	HTREEITEM htChildNode = GetChildItem(htItemNode);
	while (htChildNode != NULL)
	{
		CString	strItemText =	GetItemText(htChildNode);
		strItemText.MakeUpper();
		if (strItemText.Find((LPCTSTR)strFindText) != -1)
		{
			hItem = htChildNode;
			bResult = TRUE;
			break;
		}
		
		htChildNode = GetNextItem(htChildNode, TVGN_NEXT);
	}
	
	return bResult;
}

void CXmlTreeCtrl::SetItemAllView(BOOL bAllItemView)
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();
	
	while (htiRoot != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(htiRoot, TVE_EXPAND);
		else
			Expand(htiRoot, TVE_COLLAPSE);
		
		SetItemChildView(htiRoot, bAllItemView);
		htiRoot = GetNextSiblingItem(htiRoot);
	}
	
	m_bAllItemView = bAllItemView;
}

void CXmlTreeCtrl::SetItemChildView(HTREEITEM hItem, BOOL bAllItemView)
{
	HTREEITEM hChild = GetChildItem(hItem);
	while (hChild != NULL)
	{
		if (bAllItemView == TRUE)
			Expand(hChild, TVE_EXPAND);
		else
			Expand(hChild, TVE_COLLAPSE);
		
		if (ItemHasChildren(hChild) == TRUE)
			SetItemChildView(hChild, bAllItemView);
		
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}

BOOL CXmlTreeCtrl::SetInitTreeList(CList<XMLTreeInfoFile, XMLTreeInfoFile&> *pFileList)
{
	BOOL bResult = TRUE;
	
	int nTreeIconIndex0St = 0;
	int nTreeIconIndex1St = 2;
	int nTreeIconIndex2St = 3;
	int nTreeIconIndex3St = 4;		//#<= Alert X
	int nTreeIconIndex4St = 5;		//#<= Alert O
	
	POSITION pos;
	XMLTreeInfoFile    stFileInfo;
	TRACE("\n\n [After Sync Folder File Info]");
	
	//int	m_strLoadType;		//# 0 : Common, 1 : order, 2 : Recommend, 3 : user, 4 : Language, 5 : Recommend + user
	int nTreeIconIndex = nTreeIconIndex0St;
	HTREEITEM	htiParent;
	if (m_strLoadType == "2"){}		//# 2 : Recommend
		/*htiParent = SetTreeItem(m_htiRoot, (CString)RECOMMEND_ROOTTITLE, nTreeIconIndex, nTreeIconIndex, -1);*/
	else if (m_strLoadType == "3")		//# 3 : user
		htiParent = SetTreeItem(m_htiRoot, (CString)USER_ROOTTITLE, nTreeIconIndex, nTreeIconIndex, -1);
	else if (m_strLoadType ==  "4"){}		//# 4 : Language
		/*htiParent = SetTreeItem(m_htiRoot, (CString)LANGUAGE_ROOTTITLE, nTreeIconIndex, nTreeIconIndex, -1);*/
	
// 	try {
// 		int nCount = pFileList->GetCount();
// 		for(int nIndex = 0; nIndex < nCount; nIndex++)
// 		{
// 			pos = pFileList->FindIndex(nIndex);
// 			stFileInfo = pFileList->GetAt(pos);
// 			
// 			if (stFileInfo.strAlertReg == "TRUE")
// 				nTreeIconIndex = nTreeIconIndex4St;
// 			else
// 				nTreeIconIndex = nTreeIconIndex2St;
// 			
// 			SetTreeItem(htiParent, stFileInfo.strFileName, nTreeIconIndex, nTreeIconIndex, nIndex);
// 		}
// 	} catch( CException e )
// 	{
// 		bResult = FALSE;
// 	}
	
	int nCount = pFileList->GetCount();
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pos = pFileList->FindIndex(nIndex);
		stFileInfo = pFileList->GetAt(pos);
		
		if (stFileInfo.strAlertReg == "TRUE")
			nTreeIconIndex = nTreeIconIndex4St;
		else
			nTreeIconIndex = nTreeIconIndex2St;
		
		SetTreeItem(htiParent, stFileInfo.strFileName, nTreeIconIndex, nTreeIconIndex, nIndex);
	}
	
	return bResult;
}

void CXmlTreeCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeCtrl::OnShowWindow(bShow, nStatus);
	
}

BOOL CXmlTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetEditControl())
		{
			BOOL bState = FALSE;
			if (pMsg->wParam == VK_RETURN)
			{
				bState = TRUE;
				m_bEditModify = TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE)
			{
				bState = TRUE;
				m_bEditModify = FALSE;
			}
			
			if (bState == TRUE)
			{
				SetFocus();
				return TRUE;
			}
		}
	}
	
	else if(pMsg->message == WM_RBUTTONDOWN)
	{
		CPoint point = (CPoint)pMsg->lParam;
		UINT nFlags = (UINT)pMsg->wParam;
		
		UINT uFlags;
		HTREEITEM hSelItem = HitTest(point, &uFlags);

		if (hSelItem != NULL)
		{
			if (TVHT_ONITEM & uFlags)
			{
				TRACE("\n# TVHT_ONITEM");		//# item
			}
			else
			{
				CTreeCtrl::OnRButtonDown(nFlags, point);
				return TRUE;
			}

			if (TVHT_ONITEM & uFlags)
			{
				GetSelectItem(point);
				SelectItem(hSelItem);
			}
		}
		else
		{
			CTreeCtrl::OnRButtonDown(nFlags, point);
			return TRUE;
		}

		// 랭귀지이면 메인쪽에 메시지 보낸다. 메인에서 팝업메시지 보낸다.
		if(m_htiLanguage==hSelItem)
			SetSendPopupMenu(3, point);

		if ((m_strLoadType == "0") || (m_strLoadType == "1") ||
			(m_strLoadType == "01") || (m_strLoadType == "11"))
		{
			if (GetChildItem(hSelItem) == NULL)
			{
				TVITEM item;
				TCHAR szText[1024];
				item.hItem = hSelItem;
				item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
				item.pszText = szText;
				item.cchTextMax = 1024;


				BOOL bWorked = GetItem(&item);
				if (bWorked == TRUE)
				{
					if ((item.iImage == m_nTreeIconIndexNormal) || 
						(item.iImage == m_nTreeIconIndexNonAlert))	//#<= Normal, Alert
					{
						if (m_htiBookMark == GetParentItem(hSelItem))
							SetSendPopupMenu(1, point);
						else 
						{
							if ((m_htiPortPolio != GetParentItem(hSelItem)) && 
								(m_htiPortPolio != hSelItem) && (m_htiBookMark != hSelItem))
								SetSendPopupMenu(0, point, hSelItem);
						}
					}
					else if((item.iImage == m_nTreeIconIndex_NormalFile) ||
						(item.iImage == m_nTreeIconIndex_NormalFile_Unit) ||
						(item.iImage == m_nTreeIconIndex_AlertFile) ||	//0000139: 조건검색 순차검색 설정후 저장시, 저장되지 않는 오류 개선
						(item.iImage == m_nTreeIconIndex_AlertFile_Unit) )
					{
						HTREEITEM hRoot1, hRoot0;
						hRoot1 = GetParentItem(hSelItem);
						if(hRoot1)
						{
							hRoot0 = GetParentItem(hRoot1);
							if(hRoot0)
							{
								CString szText = GetItemText(hRoot0);
								if(szText.Compare(USER_ROOTTITLE)==0)
									SetSendPopupMenu(2, point);
							}
						}
					}
				}
			}
		}
	}
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CXmlTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	HTREEITEM hSelectedItem = GetSelectedItem();
	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelectedItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;
	
	if (GetItem(&item) == TRUE)
	{
		if ((item.state == 2) && (item.lParam > -1))
		{
			
		}
		else
			PostMessage(WM_KEYDOWN, VK_ESCAPE, NULL);
	}
	
	*pResult = 0;
}

void CXmlTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	if (m_bEditModify == TRUE)
	{
		BOOL bState = FALSE;
		
		UpdateData(true);
		HTREEITEM hSelectedItem = GetSelectedItem();
		CEdit *label = GetEditControl();
		CString strChange;
		label->GetWindowText(strChange);
		
		
		//# Select Info Modify!
		TVITEM item;
		TCHAR szText[1024];
		item.hItem = hSelectedItem;
		item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
		item.pszText = szText;
		item.cchTextMax = 1024;
		
		if (GetItem(&item) == TRUE)
		{
			if ((item.state == 2) && (item.lParam > -1))
			{
				m_strSelectItemText = item.pszText;
				
				int nIndex;
				POSITION pos;
				XMLTreeInfoFile stXMLTreeInfoFile;
				
				BOOL bSameFileName = FALSE;
				for (nIndex = 0; nIndex < m_listXMLTreeInfoFile.GetCount(); nIndex++)
				{
					pos = m_listXMLTreeInfoFile.FindIndex(nIndex);
					stXMLTreeInfoFile = m_listXMLTreeInfoFile.GetAt(pos);
					
					if (stXMLTreeInfoFile.strFileName == strChange)
					{
						bSameFileName = TRUE;
						break;
					}
				}
				
				if (bSameFileName == TRUE)
				{
					CString strErrorMsg;
					strErrorMsg.Format("조건파일(%s)존재 합니다. \n 변경 파일명을 재확인하세요!", strChange + ".xml");
					SetMessageBox(strErrorMsg, (CString)PROGRAM_TITLE, MB_OK);
					SetFocus();
					return;
				}
				
				for (nIndex = 0; nIndex < m_listXMLTreeInfoFile.GetCount(); nIndex++)
				{
					pos = m_listXMLTreeInfoFile.FindIndex(nIndex);
					stXMLTreeInfoFile = m_listXMLTreeInfoFile.GetAt(pos);
					
					if (item.pszText == stXMLTreeInfoFile.strFileName)
					{
						m_stSelectInfoFile.strFileName = strChange;
						m_stSelectInfoFile.strFilePath = strChange + ".xml";
						
						m_listXMLTreeInfoFile.SetAt(pos, m_stSelectInfoFile);
						bState = TRUE;
						break;
					}
				}
			}
		}
		
		if (bState == FALSE)
			return;
		
		//# File ReName!
		CString strRoot = GetFolderPath("3") + "\\";
		CFile::Rename(strRoot + item.pszText + ".xml", strRoot + strChange + ".xml");
		
		//# Tree Item Change!
		SetItemText(hSelectedItem, strChange);
	}
	
	*pResult = 0;
}


void CXmlTreeCtrl::SetLoadAccount()
{
/*
01 : 위탁
06: 저축
13 : 선물
51 : 랩계좌
71 : 종금위탁
76 : 종금저축

  #define		SIZEFULLACCT	64  // 계좌(11) + 계좌명(40) + 비밀번호(13)
  // 공유처리는 SIZEFULLACCT 단위로
	*/
	
	m_saAccountInfo.RemoveAll();
	
	/*
	CString strPath;
	strPath = m_strAppPath + "\\Bin\\Comm.dll";
	HINSTANCE hComm = ::LoadLibrary(strPath);
	FpGetRemainCode	pGetRemainCode = (FpGetRemainCode)::GetProcAddress(hComm, "EX_GetRemainCodeByAccount");
	*/
	
	/*
	CWnd *pWndPerent = this;
	pWndPerent = pWndPerent->GetParent();	//# CConditionTabCtrl
	pWndPerent = pWndPerent->GetParent();	//# CSubMainLeftView
	pWndPerent = pWndPerent->GetParent();	//# CGfxSplitterWnd
	pWndPerent = pWndPerent->GetParent();	//# CMyNetFindFrmWnd
	pWndPerent = pWndPerent->GetParent();	//# CMyNetFindMain
	
	  CMyNetFindMain *pMyNetFindMain = (CMyNetFindMain*)pWndPerent;
	  
		CString strAccountData;
		int nAccountCnt = pMyNetFindMain->m_pAccHelp->GetKeyString("01", strAccountData);
		
		  
			CString strAccountInfo;
			CString strItem, strAccount, strName;
			CStringArray saHaveCode;
			for (int nIndex = 0; nIndex < nAccountCnt; nIndex++)
			{
			strAccountInfo = strAccountData.Mid(SIZEFULLACCT * nIndex, SIZEFULLACCT);
			
			  strAccount = strAccountInfo.Left(11);
			  strName = strAccountInfo.Mid(11, 40);
			  strName.TrimLeft(" ");
			  strName.TrimRight(" ");
			  
				strItem.Format("%s_%s", strAccount, strName);
				m_saAccountInfo.Add(strItem);
				}
	*/
	//::FreeLibrary(hComm);
}


void CXmlTreeCtrl::SetAccountTree(BOOL bUpDate/* = FALSE*/)
{
	if (m_htiAccount == NULL)
		return;
	
	if (bUpDate == TRUE)
	{
		HTREEITEM hItem, hDelete;
		hItem = GetChildItem(m_htiAccount);
		while (hItem != NULL)
		{
			hDelete = hItem;
			hItem = GetNextItem(hItem, TVGN_NEXT);
			DeleteItem(hDelete);
		}
	}
	
	CString strItemName;
	int nCnt = m_saAccountInfo.GetSize();
	for (int nIndex = 0; nIndex < nCnt; nIndex++)
	{
		strItemName = m_saAccountInfo.GetAt(nIndex);
		SetTreeItem(m_htiAccount, strItemName, m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, OFFSET_ACCOUNT - nIndex);
	}
}

CString CXmlTreeCtrl::GetItemPath(HTREEITEM hSelectItem)
{
	CString strPath;
	CString strParentPath;
	
	if (hSelectItem == NULL)
		return "";
	
	strPath = GetItemText(hSelectItem);
	HTREEITEM hParentItem = GetParentItem(hSelectItem);
	while (hParentItem != NULL)
	{
		strParentPath = GetItemText(hParentItem);
		strPath = strParentPath + "/" + strPath;
		
		hParentItem = GetParentItem(hParentItem);
	}
	
	return strPath;
}

BOOL CXmlTreeCtrl::SetSaveXmlMark(CString strLoadType, CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo)
{
	BOOL bResult = TRUE;
	
	SetControlState(FALSE);
	
	strLoadType += "2_";
	CString strFilePath = GetFilePath(strLoadType);
	
	MSXML::IXMLDOMNodePtr				pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr				pNFolder;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAFolder;
	
	MSXML::IXMLDOMNodePtr				pNCondition;
	MSXML::IXMLDOMNamedNodeMapPtr		pNACondition;
	
	try 
	{
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr		pPI;
		MSXML::IXMLDOMNamedNodeMapPtr				pNAPI;
		MSXML::IXMLDOMNodePtr						pNEncoding;
		
		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		//pPIUsr = pPI->cloneNode(VARIANT_TRUE);
		
		pDOMIndex->appendChild(pPI);

		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)XML_VERSION_USERFILE;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		
		pDOMIndex->appendChild(pNIndex);
		//MSXML::IXMLDOMNodePtr pNAttributeTemp;			
		pNFolder = pDOMIndex->createElement("BOOK_MARK");
		pNAFolder = pNFolder->attributes;
		
		/*
		pNAttributeTemp = pDOMIndex->createAttribute("NAME");
		pNAttributeTemp->text = "USER";//(LPCTSTR)m_strFolderName;
		pNAFolder->setNamedItem(pNAttributeTemp);
		*/
		
		pNIndex->appendChild(pNFolder);
		
		POSITION pos;
		XMLTreeInfo	stXMLTreeInfo;
		CString strTemp;
		for (int nItemIndex = 0; nItemIndex < plistXMLTreeInfo->GetCount(); nItemIndex++)
		{
			pos = plistXMLTreeInfo->FindIndex(nItemIndex);
			stXMLTreeInfo = plistXMLTreeInfo->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;
			
			pNCondition = pDOMIndex->createElement((LPCTSTR)stXMLTreeInfo.strTag);
			pNFolder->appendChild(pNCondition);
			pNACondition = pNCondition->attributes;
			
			pNAttributeTemp = pDOMIndex->createAttribute("NAME");
			pNAttributeTemp->text = (LPCTSTR)stXMLTreeInfo.strName;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("NOT_ALERT");
			strTemp.Format("%d", stXMLTreeInfo.nNonAlert);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("CONDI_TYPE");
			strTemp.Format("%d", stXMLTreeInfo.nCondiType);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("TARGETBASE_TYPE");
			strTemp.Format("%d", stXMLTreeInfo.nTargetBase_Type);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("GUBUN");
			strTemp.Format("%d", stXMLTreeInfo.nGubun);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("GUBUN_DEFVAL");
			strTemp.Format("%d", stXMLTreeInfo.nGubunDefVal);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("NOT_USER");
			strTemp.Format("%d", stXMLTreeInfo.nNotUser);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("NOT_DEFVAL");
			strTemp.Format("%d", stXMLTreeInfo.nNotDefVal);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("RESULT_VIEW_ID");
			pNAttributeTemp->text = (LPCTSTR)stXMLTreeInfo.strResultViewID;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("RESULT_VIEW_DEFVAL");
			strTemp.Format("%d", stXMLTreeInfo.nResultViewDefVal);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("CHART_VALSET");
			strTemp.Format("%d", stXMLTreeInfo.nChartValSet);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNACondition->setNamedItem(pNAttributeTemp);
			
			pNAttributeTemp = pDOMIndex->createAttribute("FILE_NAME");
			pNAttributeTemp->text = (LPCTSTR)stXMLTreeInfo.strMapFile;
			pNACondition->setNamedItem(pNAttributeTemp);
		}
		
		pDOMIndex->save(_bstr_t(strFilePath));
	}
	catch(_com_error e) 
	{
		bResult = FALSE;
	}
	
	SetControlState(TRUE);
	
	return bResult;
}


CString CXmlTreeCtrl::GetFilePath(CString strLoadType)
{
	CString strFile;
	CString strFilePath = "";
	
	BOOL bUserFolder = FALSE;
	if (strLoadType == "0") //# Common
		strFile = (CString)TREE_COMMON_FILE;
	else if (strLoadType == "1") //# Order
		strFile = (CString)TREE_ORDER_FILE;
	else if (strLoadType == "01") //# Common Sort
		strFile = (CString)TREE_COMMON_SORT_FILE;
	else if (strLoadType == "11") //# Order Sort
		strFile = (CString)TREE_ORDER_SORT_FILE;
	else if	(strLoadType == "7")
		strFile = (CString)TREE_EXCAVATION_FILE;
	else if (strLoadType == "02_") //# Common Mark
	{
		strFile = (CString)TREE_COMMON_MARK_FILE;
		bUserFolder = TRUE;
	}
	else if (strLoadType == "12_") //# Order Mark
	{
		strFile = (CString)TREE_ORDER_MARK_FILE;
		bUserFolder = TRUE;
	}
	else if (strLoadType == "4") //# Language
		strFile = (CString)TREE_LANGUAGE_FILE;
	else if (strLoadType == "3") //# User
	{
		strFile = (CString)TREE_USER_FILE;
		bUserFolder = TRUE;
	}
	else if (strLoadType == "2") //# Recommend
		strFile = (CString)TREE_RECOMMEND_FILE;
	
	if (m_bIndustry == TRUE)
		strFile.Replace(".", "_Industry.");
	
	if (bUserFolder == TRUE)
	{
		//0000263: 조건검색 화면에서 조건을 즐겨찾기로 추가하고 화면들 닫았다 열었을때 즐겨찾기 조건에 추가된 조건이 없는 현상
		CString strUserPath;
		strUserPath.Format("%s%s%s", GetPath(homeCC), USRDIR, GetPath(nameCC));
		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
		strFilePath = m_strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
	
	return strFilePath;
}

CString CXmlTreeCtrl::GetFolderPath(CString strLoadType)
{
	CString strFolderPath = "", strFolder;
	
	BOOL bUserFolder = FALSE;
	if (strLoadType == "2")			//# 2 : Recommend
	{
		strFolder = (CString)FOLDER_SYSFILE; 
		/*
		if (m_bIndustry == TRUE)
		strFolder += "_Industry";
		*/
	}
	else if (strLoadType == "3")		//# 3 : user
	{
		strFolder = (CString)FOLDER_USERFILE;
		if (m_bIndustry == TRUE)
			strFolder += "_Industry";
		
		bUserFolder = TRUE;
	}
	else if (strLoadType == "4")		//# 4 : Language
		strFolder = (CString)FOLDER_LANGUAGEFILE;
	
	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s", GetPath(homeCC), USRDIR, GetPath(nameCC));
		
		strFolderPath = strUserPath + strFolder;
	}
	else
		strFolderPath = m_strAppPath + "\\" + strFolder;
	
	return strFolderPath;
}

BOOL CXmlTreeCtrl::SetLoadXmlMark(CString strLoadType)
{
	BOOL bResult = TRUE;
	
	SetControlState(FALSE);
	
	strLoadType += "2_";
	CString strFilePath = GetFilePath(strLoadType);
	
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr))
		return bResult;
	
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pXmlDom->async = false;

	//20120112 이문수 >>
	CFileFind ff;
	if(!ff.FindFile(strFilePath))
	{
		SetControlState(TRUE);
		return FALSE;
	}
	//20120112 이문수 <<

	if (!pXmlDom->load (strFilePath.AllocSysString())) 
	{	
		char msg[500];
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		CString strTemp(BSTR(pParsingErr->reason));	
		CString strMsg = strTemp + CString(msg) + "\n" + strFilePath;
		//		SetMessageBox(strMsg, "[XML Files Error]", MB_OK);		
		SetControlState(TRUE);
		return FALSE;
	}
	
	SetLoadXmlMark(strLoadType, pXmlDom->childNodes, &m_listXMLTreeMarkInfo);
	SetControlState(TRUE);
	
	return bResult;
}

void CXmlTreeCtrl::SetLoadXmlMark(CString strLoadType, MSXML::IXMLDOMNodeList *pNodes, 
								  CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo)
{
	CString strNodeName, strAttName;
	CString strNodeVal;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;	
	try {
		for (long idx = 0; idx < pNodes->length; idx++) 
		{
			MSXML::IXMLDOMNodePtr pNode = pNodes->item[idx];
			if(pNode == NULL) return;

			if (!wcscmp((const wchar_t *)pNode->nodeTypeString,L"element"))
			{
				strNodeName = BSTR(pNode->nodeName);			
				if (SUCCEEDED(pNode->get_attributes(&pattrs))) 
				{
					MSXML::IXMLDOMNodePtr pDomNode = pattrs->item[0];

					if ((pattrs->length) > 0) 
					{
						_bstr_t bstrName = pDomNode->nodeName;
						strAttName = (LPTSTR)(LPCTSTR)bstrName;
						
						if (!strAttName.Compare("NAME"))
						{
							_variant_t variantValue = pDomNode->nodeValue;
							_bstr_t bstrValue(variantValue);
							strNodeVal = (LPTSTR)(LPCTSTR)bstrValue;
							
							MSXML::IXMLDOMNodePtr	pDomNode1St;
							CString					strAttName1St;
							CString					strNodeVal1St;
							XMLTreeInfo		stXMLTreeInfo;
							
							stXMLTreeInfo.strName = strNodeVal; 
							stXMLTreeInfo.strTag = strNodeName;
							
							int nIndex = 1;
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("NOT_ALERT"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nNonAlert = atoi((LPCTSTR)bstrValue1St);
									
									if ((m_bJustAlert == TRUE) && (stXMLTreeInfo.nNonAlert == 1))
										continue;
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("CONDI_TYPE"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nCondiType = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("TARGETBASE_TYPE"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nTargetBase_Type = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("GUBUN"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nGubun = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("GUBUN_DEFVAL"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nGubunDefVal = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("NOT_USER"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nNotUser = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("NOT_DEFVAL"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nNotDefVal = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("RESULT_VIEW_ID"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.strResultViewID = (LPCTSTR)bstrValue1St;
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("RESULT_VIEW_DEFVAL"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nResultViewDefVal = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("CHART_VALSET"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.nChartValSet = atoi((LPCTSTR)bstrValue1St);
									
									nIndex++;
								}
							}
							
							if (pDomNode1St = pattrs->item[nIndex])
							{
								_bstr_t bstrAttName1St = pDomNode1St->nodeName;
								strAttName1St = (LPTSTR)(LPCTSTR)bstrAttName1St;
								if (!strAttName1St.Compare("FILE_NAME"))
								{
									_variant_t variantValue1St = pDomNode1St->nodeValue;
									_bstr_t bstrValue1St(variantValue1St);
									stXMLTreeInfo.strMapFile = (LPTSTR)(LPCTSTR)bstrValue1St;
									
									nIndex++;
								}
							}
							
							if ((strLoadType == "0") || (strLoadType == "01"))   //# Comm
								stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_COMM;
							else if ((strLoadType == "1") || (strLoadType == "11"))    //# Order
								stXMLTreeInfo.nItemType = (int)TREE_SELECT_TYPE_ITEM_ORDER;
							
							plistXMLTreeInfo->AddTail(stXMLTreeInfo);
						}
					}
				}
				
				SetLoadXmlMark(strLoadType, pNode->childNodes, &m_listXMLTreeMarkInfo);
			}
			pNode = NULL;
		}
		pattrs = NULL;
	}
	catch(_com_error e) 
	{
		
	}
}

void CXmlTreeCtrl::SetInitTreeMark(CList <XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo)
{
	POSITION pos;
	XMLTreeInfo		stXMLTreeInfo;
	
	int nTreeIconIndex;
	for (int nIndex = 0; nIndex < plistXMLTreeInfo->GetCount(); nIndex++)
	{
		pos = plistXMLTreeInfo->FindIndex(nIndex);
		stXMLTreeInfo = plistXMLTreeInfo->GetAt(pos);
		
		if (stXMLTreeInfo.nNonAlert == 0)
			nTreeIconIndex = m_nTreeIconIndexNormal;
		else
			nTreeIconIndex = m_nTreeIconIndexNonAlert;
		
		SetTreeItem(m_htiBookMark, stXMLTreeInfo.strName, nTreeIconIndex, nTreeIconIndex, 
			OFFSET_MARK - m_nXmlMarkListIndex);
		
		m_nXmlMarkListIndex++;
	}
}

int CXmlTreeCtrl::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, MB_OK);
}

BOOL CXmlTreeCtrl::GetCheckComboLoad()
{
	BOOL bResult = FALSE;
	
	if ((m_strLoadType == "0") || (m_strLoadType == "1") || 
		(m_strLoadType == "01") || (m_strLoadType == "11"))
	{
		if ((m_htiRecomm != NULL) || (m_htiUser != NULL))
			bResult = TRUE;
	}
	
	return bResult;
}

BOOL CXmlTreeCtrl::IsParentLanguage(HTREEITEM hItem)
{
	if(!hItem) return FALSE;

	HTREEITEM hParent = GetParentItem(hItem);
	if(!hParent) return FALSE;

	CString szSubText = GetItemText(hParent);
	if(szSubText.CompareNoCase(LANGUAGE_ROOTTITLE)==0)
		return TRUE;

	return FALSE;
}

void CXmlTreeCtrl::SelectTreeItemCondition(HTREEITEM htree)
{
	m_nSeleteClass = SetSelectDataManual(htree);
	// (2011/1/10 - Seung-Won, Bae) Do not support NM_ADD_ITEM on [020X].
	m_pwndMsgTarget->SendMessage(WM_XMLTREE_CTRL, (WPARAM)NM_SELECT_ITEM, (LPARAM)&m_stSelectInfo);	
}

int CXmlTreeCtrl::SetSelectDataManual(HTREEITEM htree)
{	
	int nResult = -1;

// 	POSITION pos;
// 	int nItemIndex;
	CString strItem = _T("");	

	if (GetSelectItem(m_stSelectInfo, htree) == TRUE)		//# Item, Mark Item
		nResult = 1;
	else
		return nResult;

	return nResult;
}

BOOL CXmlTreeCtrl::GetSelectItem(XMLTreeInfo &stXMLTreeInfo, HTREEITEM htree)
{
	BOOL bResult = FALSE;

	/*UINT uFlags;*/
	HTREEITEM hSelItem = htree;
	if (hSelItem != NULL)
	{
		SelectItem(hSelItem);
	}
	else
		return bResult;

	TVITEM item;
	TCHAR szText[1024];
	item.hItem = hSelItem;
	item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
	item.pszText = szText;
	item.cchTextMax = 1024;

	BOOL bWorked = GetItem(&item);
	if (bWorked == TRUE)
	{
		if ((item.state == 2) && 
			((item.lParam > -1) || (item.lParam <= OFFSET_MARK)) )
		{
			m_strSelectItemText = item.pszText;

			CList	<XMLTreeInfo, XMLTreeInfo&> *plistXMLTreeInfo;
			int nIndex;
			POSITION pos;
			if (item.lParam > -1) 
			{
				plistXMLTreeInfo = &m_listXMLTreeInfo;
				nIndex = (int)item.lParam;
			}
			else if (item.lParam <= OFFSET_MARK)
			{
				plistXMLTreeInfo = &m_listXMLTreeMarkInfo;
				nIndex = ((int)item.lParam * -1) + OFFSET_MARK;
			}

			pos = plistXMLTreeInfo->FindIndex(nIndex);
			if (pos != NULL)
			{
				stXMLTreeInfo = plistXMLTreeInfo->GetAt(pos);
				if ((m_strLoadType == "0") || (m_strLoadType == "1") ||
					(m_strLoadType == "01") || (m_strLoadType == "11"))
					bResult = TRUE;
			}
		}
	}

	return bResult;
}

HTREEITEM CXmlTreeCtrl::DragItem(HTREEITEM hDragItem, CPoint point)
{
	return NULL;	
}

BOOL CXmlTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
}

void CXmlTreeCtrl::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	// So user cant drag root node
	HTREEITEM hParent = GetParentItem(pNMTreeView->itemNew.hItem);
	if(hParent == NULL) 
		return ; 

	if(GetItemText(hParent) != BOOKMARK_SUBROOTTITLE)
	{
		hParent = GetParentItem(hParent);	
		if(hParent == NULL) 
			return;

		if(GetItemText(hParent) != USER_ROOTTITLE)
			return;
	}
	
	// Item user started dragging ...
	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !m_pDragImage )
		return;
		
	if(GetItemText(hParent) == USER_ROOTTITLE)
		m_nCaptureItem = 0;
	else if(GetItemText(hParent) == BOOKMARK_SUBROOTTITLE)
		m_nCaptureItem = 1;
	
	m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}

void CXmlTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM	hitem;
	UINT		flags;
	
	if (m_bLDragging)
	{	
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		if ((hitem = HitTest(point, &flags)) != NULL)
		{
			CImageList::DragShowNolock(FALSE);

			HTREEITEM hParentFolder = GetParentItem(hitem);
			
			if(m_nCaptureItem == 0)
			{
				if(GetItemText(hParentFolder) != USER_ROOTTITLE && (cursor_no != ::GetCursor())) 
				{
					::SetCursor(cursor_no);
					SelectDropTarget(NULL);
				}
				
				if(GetItemText(hParentFolder) == USER_ROOTTITLE)
				{
					if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr); 
					SelectDropTarget(hitem);
				}
			}
			else if(m_nCaptureItem == 1)
			{
				if(GetItemText(hParentFolder) != BOOKMARK_SUBROOTTITLE && (cursor_no != ::GetCursor())) 
				{
					::SetCursor(cursor_no);
					SelectDropTarget(NULL);
				}
				
				if(GetItemText(hParentFolder) == BOOKMARK_SUBROOTTITLE)
				{
					if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr); 
					SelectDropTarget(hitem);
				}
			}

			m_hitemDrop = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}
	else 
	{
		// Set cursor to arrow if not dragged
		// Otherwise, cursor will stay hand or arrow depen. on prev setting
		::SetCursor(cursor_arr);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CXmlTreeCtrl::SetDefaultCursor()
{
	// Get the windows directory
	CString strWndDir;
	GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
	strWndDir.ReleaseBuffer();

	strWndDir += _T("\\winhlp32.exe");
	// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
	HMODULE hModule = LoadLibrary(strWndDir);
	if (hModule) {
		HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
		if (hHandCursor)
		{
			cursor_hand = CopyCursor(hHandCursor);
		}

	}
	FreeLibrary(hModule);

	cursor_arr	= ::LoadCursor(NULL, IDC_ARROW);
	cursor_no	= ::LoadCursor(NULL, IDC_NO) ;
}

HTREEITEM CXmlTreeCtrl::MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
	CString sText;

	// get information of the source item
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE |     TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	GetItem(&tvstruct.item);  
	sText = GetItemText(hItem);

	tvstruct.item.cchTextMax = sText.GetLength();
	tvstruct.item.pszText = sText.LockBuffer();

	//insert the item at proper location
	tvstruct.hParent = htiNewParent;
	tvstruct.hInsertAfter = htiAfter;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	hNewItem = InsertItem(&tvstruct);
	sText.ReleaseBuffer();

	//now copy item data and item state.
	SetItemData(hNewItem,GetItemData(hItem));
	SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	//now delete the old item
	DeleteItem(hItem);

	return hNewItem;
}

BOOL CXmlTreeCtrl::MoveUserXmlFile(CString strFromDir, CString strToDir, CString strFileName)
{
	CString strFolder = (CString)FOLDER_USERFILE;
	
	CString strUserPath;
	strUserPath.Format("%s%s%s", GetPath(homeCC), USRDIR, GetPath(nameCC));

	CString strFromFullPath = (CString)strUserPath + strFolder + "\\" + strFromDir + "\\" + strFileName + ".xml";
	CString strToFullPath = (CString)strUserPath + strFolder + "\\" + strToDir + "\\" + strFileName + ".xml";
	
// 	CFile file;
// 	file.Open(strFromFullPath + strFileName + ".xml", CFile::modeReadWrite | CFile::modeCreate, NULL);
// 	file.Close();
	if(!::MoveFile(strFromFullPath, strToFullPath))
	{
		::MessageBox(m_hWnd, "같은 이름의 사용자 조건이 이미 존재합니다.", PROGRAM_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CXmlTreeCtrl::GetCheckSameFile(CString strFolderName, CString strFileName)
{
	BOOL bResult = FALSE;

	strFileName += ".xml";

	CMainFrmWnd* pMainFrm = (CMainFrmWnd*) m_pwndMsgTarget;
	
	bResult = pMainFrm->GetFindFile(strFolderName, strFileName, &pMainFrm->m_listUserInfoFolder);

	return bResult;
}

void CXmlTreeCtrl::ChangeMarkSort(CString strTarget, CString strBefore, HTREEITEM hItemMarkRoot)
{
	HTREEITEM hItemChilde = GetChildItem(hItemMarkRoot);
	while(hItemChilde)
	{
		DeleteItem(hItemChilde);
		hItemChilde = GetChildItem(hItemMarkRoot);
	}

	POSITION posBefore;
	POSITION posTarget;
	POSITION posMemberList;
	POSITION pos;

	XMLTreeInfo stXmlTreeInfo;
	XMLTreeInfo stXmlTreeInfoTemp;

	CList<XMLTreeInfo, XMLTreeInfo&> listXMLTreeMarkInfo;
	int nIndex = 0;
	for( nIndex = 0; nIndex < m_listXMLTreeMarkInfo.GetCount(); nIndex++)
	{
		posMemberList = m_listXMLTreeMarkInfo.FindIndex(nIndex);
		stXmlTreeInfoTemp = m_listXMLTreeMarkInfo.GetAt(posMemberList);

		listXMLTreeMarkInfo.AddTail(stXmlTreeInfoTemp);
		pos = listXMLTreeMarkInfo.FindIndex(nIndex);
		stXmlTreeInfoTemp = listXMLTreeMarkInfo.GetAt(pos);

		if(stXmlTreeInfoTemp.strName == strTarget)
		{
			posTarget = pos;
			stXmlTreeInfo = stXmlTreeInfoTemp;
		}	
		else if(stXmlTreeInfoTemp.strName == strBefore)
			posBefore = pos;
	}
	
	listXMLTreeMarkInfo.RemoveAt(posTarget);
	listXMLTreeMarkInfo.InsertBefore(posBefore, stXmlTreeInfo);
	
	m_nXmlMarkListIndex = 0;
	m_listXMLTreeMarkInfo.RemoveAll();
	
	for(nIndex = 0; nIndex < listXMLTreeMarkInfo.GetCount(); nIndex++)
	{
		pos = listXMLTreeMarkInfo.FindIndex(nIndex);
		stXmlTreeInfo = listXMLTreeMarkInfo.GetAt(pos);

		SetAddMark(stXmlTreeInfo);
	}
}

void CXmlTreeCtrl::SetExpandLanguage()
{
	 Expand(m_htiLanguage, TVE_EXPAND);
}