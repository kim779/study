// DrTreeCtrl.cpp : implementation file
//
/************************************************/
/*																		          */
/*							Dr Code Tree Control						*/
/*                                              */
/*																		          */
/*							작성일 : 2006. 12 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

#include "stdafx.h"
#include "DrTreeCtrl.h"

#include "../resource.h"
#include "Winsock2.h"

#include "../../Dr_Include/EU_p0621.h"
#include "../../Dr_Include/Condition_MngDef.h"
#include "../AnalystMainFrmWnd.h"
//#include "../../../inc/IMasterDataMng.h"

//#include "../../../inc/IAUGwanMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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


#define TRNO_THEMA          "HFIBOA076220"
#define TRNO_THEMATOCODE    "HFIBOA076210"

extern void Dll_GetRegKwansimMasterGroupName(CStringArray &arrName);
extern void Dll_GetRegKwansimMasterGroupItem(CString strName, CStringArray &arrItem);

typedef struct {
	EU_p0621_InRec1 inRec1;
	EU_p0621_InRec2 inRec2[MAX_IDXCNT];
} EU_p0621_InAll;

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)

/////////////////////////////////////////////////////////////////////////////
// CDrTreeCtrl

CDrTreeCtrl::CDrTreeCtrl()
{
	m_hConditionMng = NULL;

	m_nIndex_Port = -1;
	m_htiPort = NULL;

	m_nIndex_Thema = -1;

	m_nTrIndex = 0;

	m_htiConditionSearch = NULL;

	m_nPreReceiveLen = -1;
}

CDrTreeCtrl::~CDrTreeCtrl()
{
	POSITION pos;
	DrTreeItem stDrTreeItem;
	int nKey;
	
	for (pos = m_mapItem.GetStartPosition(); pos != NULL;)
	{
		m_mapItem.GetNextAssoc(pos, nKey, stDrTreeItem);
		m_mapItem.RemoveKey(nKey);
	}
	m_mapItem.RemoveAll();

	if (m_htiPort != NULL)
		SetInit_LibPortPolio(FALSE);

	m_imglistTree.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CDrTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDrTreeCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP

  //# 삼성증권
//	ON_MESSAGE(UM_CHANGE_REGKW, OnNotify_PortPolio)		-hidden-

//  ON_MESSAGE(WM_RP_RECEIVED, OnGetData)				-hidden-
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrTreeCtrl message handlers

int CDrTreeCtrl::Create(CWnd* pParentWnd, UINT nID) 
{
	int nResult = -1;

	nResult = CTreeCtrl::Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | TVS_HASBUTTONS | 
												TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP,
												CRect(0, 0, 0, 0),
												pParentWnd,	
												nID);
	CTreeCtrl::ModifyStyleEx(0, WS_EX_CLIENTEDGE);

  m_pwndParent = pParentWnd;
  
	SetIconLoad();
  //SetItemAllView(FALSE);

	return nResult;
}

int CDrTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDrTreeCtrl::OnDestroy() 
{
	if (m_hConditionMng != NULL)
	{
		Lib_Close();
		FreeLibrary(m_hConditionMng); 
		m_hConditionMng = NULL;
	}
	
	CTreeCtrl::OnDestroy();
}

#define _countof(array) (sizeof(array)/sizeof(array[0]))

const		LPCTSTR		ROOTTITLE_MARKET[] = {"검색종목", "전체", "거래소", "코스닥", 
											  "KOSPI200", "KRX", "거래소업종", 
											  "코스닥업종", "종목검색(사용자조건)", "종목검색(추천조건)", 
											  "빠른종목검색", "관심그룹" };

const		LPCTSTR		TYPE_MARKETCODE[]  = {"", "", "", "", 
										      "2001", "4001",  "0001",
											  "1001", "1", "0",
											  "2", "" };

const		LPCTSTR		TYPE_CODE[]        = {"1111", "0190", "0101", "0102", 
                                              "101", "0190", "0101",
										      "0102", "", "", 
											  "",  "" };

const		LPCTSTR		TYPE_DATA[]        = {"C", "I", "I", "I", 
											  "I", "M", "M", 
											  "M", "F", "F", 
											  "F", "P" };

const		UINT		OFFSET_DATA[]      = {6000, 1, 2, 3, 
                                              100, 400, 500,
											  600, 2000, 1000, 
											  3000, 5000 };

void CDrTreeCtrl::SetInit()
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();
	if (SetLoad_ConditionMngLib() == TRUE)
	{
		SetSearch_MSTCode();

		//# Load Tree Parent Item!
		int nIndex_Market = 0;
		for (int nIndex = 0; nIndex < _countof(ROOTTITLE_MARKET); nIndex++)
		{
			if (TYPE_DATA[nIndex] == "I")
			{
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
							        m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, OFFSET_DATA[nIndex]);

				DrTreeItem stDrTreeItem;

				stDrTreeItem.nIndex = OFFSET_DATA[nIndex];
				stDrTreeItem.strDataType = TYPE_DATA[nIndex];    //# "I" "M" "T" "F" "P"
				stDrTreeItem.strMarketCode = TYPE_MARKETCODE[nIndex];
				stDrTreeItem.strData = TYPE_CODE[nIndex];
				stDrTreeItem.strTitle = ROOTTITLE_MARKET[nIndex];

				m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);
			}
			else if (TYPE_DATA[nIndex] == "M")
			{
				CStringArray saCode, saName;
				if (GetMarketInfo(TYPE_MARKETCODE[nIndex], &saCode, &saName) == TRUE)
				{
					m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
																m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, -1);

					SetInitTree_Item(m_htiMarket[nIndex_Market], nIndex, &saName, &saCode);
					nIndex_Market++;
				}
			}
			else if (TYPE_DATA[nIndex] == "T")
			{
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
																m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);

				if (TYPE_CODE[nIndex] == "Thema")
				{
				  m_nIndex_Thema = nIndex_Market;
				  SetSearch_Thema();
				}
				else if (TYPE_CODE[nIndex] == "Group")
				{

				}

				nIndex_Market++;
			}
			else if (TYPE_DATA[nIndex] == "F")
			{
				CStringArray saGroupName, saCondiData;
				if (Lib_GetConditionData(atoi(TYPE_MARKETCODE[nIndex]), FALSE, 0, saGroupName, saCondiData) == TRUE)
				{
					m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
																	m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, -1);

					SetInitTree_File(m_htiMarket[nIndex_Market], nIndex, &saGroupName, &saCondiData);

					nIndex_Market++;
				}
			}
			else if (TYPE_DATA[nIndex] == "P")
			{
				SetInit_LibPortPolio(TRUE);

				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
																m_nTreeIconIndex0St, m_nTreeIconIndex0St + 1, -1);
				SetInitTree_Port(m_htiMarket[nIndex_Market], nIndex);

				m_nIndex_Port = nIndex;
				m_htiPort = m_htiMarket[nIndex_Market];

				nIndex_Market++;
			}
			else if (CString(TYPE_DATA[nIndex]) == "C")
			{
				m_htiMarket[nIndex_Market] = SetTreeItem(htiRoot, ROOTTITLE_MARKET[nIndex], 
					m_nTreeIconIndex0St_File, m_nTreeIconIndex0St_File + 1, OFFSET_DATA[nIndex]);

				m_htiConditionSearch = m_htiMarket[nIndex_Market];
				
				DrTreeItem stDrTreeItem;
				stDrTreeItem.nIndex = OFFSET_DATA[nIndex];
				stDrTreeItem.strDataType = TYPE_DATA[nIndex];    //# "I" "M" "T" "F" "P"
				stDrTreeItem.strMarketCode = TYPE_MARKETCODE[nIndex];
				stDrTreeItem.strData = TYPE_CODE[nIndex];
				stDrTreeItem.strTitle = ROOTTITLE_MARKET[nIndex];			
				m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);	
			}
		}
	}
}

//extern int Dll_SetRegKwCombo(CWnd* pWnd, int nType);  //# nType 1-추가, 0-삭제
void CDrTreeCtrl::SetInit_LibPortPolio(BOOL bInit)
{
 // Dll_SetRegKwCombo(this, (bInit == TRUE)?1:0);	-hidden-
}

void CDrTreeCtrl::OnNotify_PortPolio(WPARAM wParam, LPARAM lParam)
{
	if (m_htiPort != NULL)
		SetInitTree_Port(m_htiPort, m_nIndex_Port);
}

//# 삼성증권
//extern BOOL    dll_GetMasterData(CString szType, CStringArray &arrCode, CStringArray &arrName);
//extern BOOL    dll_GetMasterDataInUp(CString szType, CStringArray &arrCode, CStringArray &arrName, CString strUp);
BOOL CDrTreeCtrl::GetMarketInfo(CString strType, CStringArray *psaCode, CStringArray *psaName)
{
	BOOL bResult = TRUE;

	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();

	if(!pMainWnd)
		return FALSE;

	psaCode->RemoveAll();
	psaName->RemoveAll();

	if("4001" == strType)		// KRX
	{
		bResult = pMainWnd->GetUpMasterData((CStringArray *&)psaCode, (CStringArray *&)psaName, GETMARKET_KRX);
	}
	else if("0001" == strType)	// 거래소업종
	{
		bResult = pMainWnd->GetUpMasterData((CStringArray *&)psaCode, (CStringArray *&)psaName, GETMARKET_KOSPI);
	}
	else if("1001" == strType)	// 코스닥업종
	{
		bResult = pMainWnd->GetUpMasterData((CStringArray *&)psaCode, (CStringArray *&)psaName, GETMARKET_KOSDAQ);
	}
	else if("2001" == strType)	// KOSPI200
	{
		bResult = pMainWnd->GetUpMasterData((CStringArray *&)psaCode, (CStringArray *&)psaName, GETMARKET_KOSPI200);
	}

  return bResult;
}

BOOL CDrTreeCtrl::SetLoad_ConditionMngLib()
{
	if (m_hConditionMng != NULL)
		return TRUE;
  
	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();
	CString strRoot, strPath;
	strRoot = pMainWnd->Variant(homeCC);
	strPath = strRoot + "\\dev\\Lib_ConditionMng.dll";

	m_hConditionMng = ::LoadLibrary(strPath);
	if (m_hConditionMng)
	{
		Lib_Open = (_DLL_Open)::GetProcAddress(m_hConditionMng, "_DLL_Open");
		Lib_Close = (_DLL_Close)::GetProcAddress(m_hConditionMng, "_DLL_Close");

		Lib_GetConditionData = (_DLL_GetConditionData)::GetProcAddress(m_hConditionMng, "_DLL_GetConditionData");
		Lib_GetFind_Packet = (_DLL_GetFind_Packet)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Packet");
		Lib_GetFind_Result = (_DLL_GetFind_Result)::GetProcAddress(m_hConditionMng, "_DLL_GetFind_Result");

		Lib_SetUserFolderPath = (_DLL_SetUserFolderPath)::GetProcAddress(m_hConditionMng, "_DLL_SetUserFolderPath");
		CString strUserFolderPath;
		strUserFolderPath.Format("%s%s%s", pMainWnd->Variant(homeCC), USRDIR, pMainWnd->Variant(nameCC));
		Lib_SetUserFolderPath(strUserFolderPath);
	}
	else
	{
		TRACE("\n# Lib_ConditionMng.dll이 없습니다.");
		return FALSE;
	}

	return TRUE;
}

void CDrTreeCtrl::SetIconLoad()
{
	m_imglistTree.DeleteImageList();
	m_imglistTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 3, 3);

	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00));	//# 0 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE00_FILE));	//# 0 st - File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE01_FILE));
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 1 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE10));	//# 2 st
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE11));	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# 일반
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NONALERT));	//# Non Alert
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_NORMAL));		//# PortPolio
	
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE));					//# 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_UNIT));			//# 조합 일반 File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERT));		//# Alert File
	m_imglistTree.Add(AfxGetApp()->LoadIcon(IDI_ICON_TREE_ALERTUNIT));	//# Alert 조합 File
		
	SetImageList(&m_imglistTree, TVSIL_NORMAL);
}

int CDrTreeCtrl::GetTreeIconIndex_File(BOOL bAlertReg, BOOL bUnitCan)
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

void CDrTreeCtrl::SetControlState(BOOL bState)
{
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
}

HTREEITEM CDrTreeCtrl::SetTreeItem(HTREEITEM htiParent, CString strItem, 
																		int nImgIndex, int nImgSelIndex, int lParam) 
{
	TV_INSERTSTRUCT tvstruct;
	
	tvstruct.hParent = htiParent;
	tvstruct.hInsertAfter = TVI_LAST;
		
	tvstruct.item.pszText = (LPTSTR)(LPCSTR)strItem;	
		
	tvstruct.item.lParam = (long)lParam;
	tvstruct.item.iImage = nImgIndex;
	tvstruct.item.iSelectedImage = nImgSelIndex;
	tvstruct.item.mask = TVIF_TEXT | TVIF_PARAM |TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			
	//HTREEITEM hItem = GetChildItem(htiParent);
	HTREEITEM hItem = InsertItem(&tvstruct);
	Expand(hItem, TVE_COLLAPSE);
	UpdateWindow();
		
	return hItem; 
}

void CDrTreeCtrl::SetItemAllView(BOOL bAllItemView)
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();
	
	if (bAllItemView == m_bAllItemView)
		return;

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

void CDrTreeCtrl::SetItemChildView(HTREEITEM hItem, BOOL bAllItemView)
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


void CDrTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

BOOL CDrTreeCtrl::SetDeleteItem(HTREEITEM hSelItem)
{
	BOOL bResult = FALSE;

	if (hSelItem != NULL)
	{
		DeleteItem(hSelItem);
		hSelItem = GetSelectedItem();
		if (hSelItem != NULL)
			GetSelectItemData(hSelItem);

		bResult = TRUE;
	}

  return bResult;
}

void CDrTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	UINT uFlags;
	HTREEITEM hSelItem = HitTest(point, &uFlags);
	if (hSelItem != NULL)
	{
		if (TVHT_ONITEM & uFlags)
		{
			SelectItem(hSelItem);

			if (GetChildItem(hSelItem) != NULL)
			{
				if (GetSelectItemData(hSelItem) == FALSE)
				{

				}
				return;
			}
		}
		else
			return;
	}
	else
		return;

	GetSelectItemData(hSelItem);

	CTreeCtrl::OnLButtonUp(nFlags, point);
}

BOOL CDrTreeCtrl::GetSelectItemData(HTREEITEM hSelItem)
{
	BOOL bResult = TRUE;

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
			if (item.lParam == -1)
				bResult = FALSE;
			else
			{
				CStringArray saCode;
				bResult = GetCode(item.lParam, saCode);

				CString strTemp;
				strTemp.Format("\n# Select Item Data Cnt : %d", saCode.GetSize());
				TRACE(strTemp);

				for (int nIndex = 0; nIndex < saCode.GetSize(); nIndex++)
				{
					strTemp.Format("\n# %s", saCode.GetAt(nIndex));
					TRACE(strTemp);
				}
			}
		}
	}

  return bResult;
}

BOOL CDrTreeCtrl::SetSelectItme(CString strFolderPath, CString strFileName)
{
	HTREEITEM	htiRoot;
	htiRoot = GetRootItem();

	if (strFolderPath.IsEmpty() == TRUE)
		return FALSE;
	else
	{
		HTREEITEM	htiSelect;
		GetFindTreeItem(htiRoot, strFolderPath, htiSelect);

		Expand(htiRoot, TVE_EXPAND);
		Expand(htiSelect, TVE_EXPAND);

		if (strFileName.IsEmpty() == FALSE)
		{
			if (GetFindTreeItem(htiSelect, strFileName, htiSelect) == FALSE)
				return FALSE;
		}
		
		SelectItem(htiSelect);
		Invalidate();
	}

	return TRUE;
}


/*
const		LPCTSTR		ROOTTITLE_MARKET[] = {"전체", "거래소", "코스닥", 
                                        "KOSPI200", "KOSDAQ50", "KRX", "스타지수", "거래소업종", "코스닥업종", 
                                        "테마종목", "그룹사별", 
                                        "사용자조건", "추천조건", "원클릭조건", 
                                        "관심그룹" };

const		LPCTSTR		TYPE_MARKETCODE[]      = {"전체", "거래소", "코스닥", 
                                        "0202", "0204", "0206", "0207", "0201", "0203", 
                                        "테마종목", "그룹사별",
                                        "1", "0", "2",
                                        "" };

const		LPCTSTR		TYPE_DATA[]        = {"D", "D", "D", 
                                       "M", "M", "M", "M", "M", "M", 
                                       "T", "T",
                                       "F", "F", "F", 
                                       "P" };

const		int		OFFSET_DATA[]         = {"1", "2", "3", 
                                       "100", "200", "300", "400", "500", "600", 
                                       "700", "800",
                                       "2000", "1000", "3000", 
                                       ""5000"" };
                                       */

BOOL CDrTreeCtrl::GetCode(int nKey, CStringArray &saCode)
{
	BOOL bResult = FALSE;

	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();

	saCode.RemoveAll();

	CStringArray  saName;
	DrTreeItem stDrTreeItem;
	if (m_mapItem.Lookup(nKey, stDrTreeItem) == TRUE)
	{
		if (stDrTreeItem.strDataType == "I")    //# "I" "M" "T" "F" "P"
		{
			if (stDrTreeItem.strMarketCode == "")
			{
				if("전체" == stDrTreeItem.strTitle)
					bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_ALL);
				else if("거래소" == stDrTreeItem.strTitle)
					bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KOSPI);
				else
					bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KOSDAQ);
			}
			else
			{
				bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KOSPI200);	// KOSPI200
			}
 		}
		else if (stDrTreeItem.strDataType == "M")
		{
			if(stDrTreeItem.strMarketCode == "0101")
			{
				bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KOSPI, atoi(stDrTreeItem.strData));	// 거래소 업종
			}
			else if(stDrTreeItem.strMarketCode == "0102")
			{
				bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KOSDAQ, atoi(stDrTreeItem.strData));	// 코스닥 업종
			}
			else if("0190" == stDrTreeItem.strMarketCode)
			{
				bResult = pMainWnd->GetStockMasterData(saCode, saName, GETMARKET_KRX);	// KRX
			}
		}
		else if (stDrTreeItem.strDataType == "T")
		{

		}
		else if (stDrTreeItem.strDataType == "F")
		{
			//# TR Send!
			int nState = -1;

			char *pcPacket;
			int nOrderCnt = 0;
			int nLen;
			nLen = Lib_GetFind_Packet(stDrTreeItem.strData, m_nTrIndex, FALSE, TRUE, TRUE, pcPacket, nOrderCnt);
			if (nLen > 0)
			{
				EU_p0621_InAll inAll;
				int nSendTrDataSize = InMakeTr_P0621((BYTE*)pcPacket, (BYTE*)&inAll);
				nState = Modual_SendTr((CString)TRNO_CONDITION, nSendTrDataSize, (BYTE*)&inAll);
				delete pcPacket;
			}
		}
		else if (stDrTreeItem.strDataType[0] == 'C')
		{
			CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();

			CStringArray strArrayCodeName;
			pMainWnd->GetUserMaster(usrMasterCondition, saCode, strArrayCodeName);
			bResult = (saCode.GetSize() > 0) ? TRUE : FALSE;
			
		}
		else if (stDrTreeItem.strDataType == "P")
		{
			CString strIndex;
			CStringArray saPortItem;

			strIndex = stDrTreeItem.strData;
			/*strIndex = strIndex.Mid(0, strIndex.Find(' '));*/
			
			if(pMainWnd->m_bLinkData && strIndex == "조건검색결과" )
			{
				for(int i = 0; i < pMainWnd->m_saConditionSearchResultItemCode.GetSize(); i++)
				{
					saPortItem.Add(pMainWnd->m_saConditionSearchResultItemCode.GetAt(i));
				}
			}
			else
			{
				GetItemsWithGroupName(strIndex, saPortItem);			
			}

			if (saPortItem.GetSize() > 0)
			{
				CString strCode;
				for (int nIndex = 0; nIndex < saPortItem.GetSize(); nIndex++)
				{
					strCode = saPortItem.GetAt(nIndex);
					saCode.Add(strCode);
				}
			}

			bResult = (saCode.GetSize() > 0) ? TRUE : FALSE;
		}

		m_pwndParent->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_ITEMTITLE, (LPARAM)&stDrTreeItem.strTitle);
	}

	if (bResult == TRUE)
		m_pwndParent->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_CLK, (LPARAM)&saCode);

	return bResult;
}

BOOL CDrTreeCtrl::GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, HTREEITEM &hItem)
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

void CDrTreeCtrl::SetInitTree_File(HTREEITEM htiParent, int nItemIndex, CStringArray *psaGroupName, CStringArray *psaCondiData)
{
	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	HTREEITEM	htiChild;
	int nIndex, nIndex_Group, nIndex_Item;

	DrTreeItem stDrTreeItem;

	int nFindPos;
	CString strGroupName, strCondiData, strFileName;
	CString strInfo[5];
	for (nIndex_Group = 0; nIndex_Group < psaGroupName->GetSize(); nIndex_Group++)
	{
		strGroupName = psaGroupName->GetAt(nIndex_Group);

		htiChild = SetTreeItem(htiParent, strGroupName, 
									    m_nTreeIconIndex1St, m_nTreeIconIndex1St + 1, -1);
		
		for (nIndex_Item = 0; nIndex_Item < psaCondiData->GetSize(); nIndex_Item++)
		{
			strCondiData = psaCondiData->GetAt(nIndex_Item);

			nIndex = 0;
      //#	[형식] : Group View Name | File View Name | Condition File Path | Alert State | 설명 |
			while (strCondiData.IsEmpty() == FALSE)
			{
				nFindPos = strCondiData.Find("|", 0);
				if (nFindPos > -1)
				{
					strInfo[nIndex] = strCondiData.Left(nFindPos);
					strCondiData = strCondiData.Mid(nFindPos + 1, strCondiData.GetLength() - nFindPos);

					nIndex++;
				}
				else
					strCondiData = "";
			}

			if (strGroupName == strInfo[0])
			{
				if ((strInfo[3] == "FALSE") || (strInfo[3].IsEmpty() == TRUE))
					SetTreeItem(htiChild, strInfo[1], m_nTreeIconIndex_NormalFile, m_nTreeIconIndex_NormalFile, nOffset);
				else
					SetTreeItem(htiChild, strInfo[1], m_nTreeIconIndex_AlertFile, m_nTreeIconIndex_AlertFile, nOffset);

				stDrTreeItem.nIndex = nOffset;
				stDrTreeItem.strDataType = "F";    //# "I" "M" "T" "F" "P"
				stDrTreeItem.strMarketCode = ""; 
				stDrTreeItem.strData = strInfo[2];
				stDrTreeItem.strTitle = strInfo[1];
				m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);

				nOffset++;
			}
		}
	}
}

void CDrTreeCtrl::SetInitTree_Item(HTREEITEM htiParent, int nItemIndex, CStringArray *psaName, CStringArray *psaCode)
{
	if (htiParent == NULL)
		return;

	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	DrTreeItem stDrTreeItem;
	CString strName, strCode, strMarketCode;
	for (int nIndex = 0; nIndex < psaName->GetSize(); nIndex++)
	{
		strName = psaName->GetAt(nIndex);

		SetTreeItem(htiParent, strName, m_nTreeIconIndexNormal, m_nTreeIconIndexNormal, nOffset);

		stDrTreeItem.nIndex = nOffset;
		stDrTreeItem.strDataType = "M";    //# "I" "M" "T" "F" "P"

		if ((CString)(TYPE_CODE[nItemIndex]) == "")
			strMarketCode = (CString)TYPE_MARKETCODE[nItemIndex];
		else
			strMarketCode = (CString)TYPE_CODE[nItemIndex];

		stDrTreeItem.strMarketCode = strMarketCode;
		stDrTreeItem.strData = psaCode->GetAt(nIndex);
		stDrTreeItem.strTitle = strName;
		m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);

		nOffset++;
	}
}

void CDrTreeCtrl::SetInitTree_Port(HTREEITEM htiParent, int nItemIndex)
{
	if (htiParent == NULL)
		return;

	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	DrTreeItem stDrTreeItem;
	CStringArray saPortName;
//  Dll_GetRegKwansimMasterGroupName(saPortName);
	GetKwansimGroupName(saPortName);
	for (int nIndex = 0; nIndex < saPortName.GetSize(); nIndex++)
	{
		SetTreeItem(htiParent, saPortName.GetAt(nIndex), m_nTreeIconIndexPort, m_nTreeIconIndexPort, nOffset);

		stDrTreeItem.nIndex = nOffset;
		stDrTreeItem.strDataType = "P";    //# "I" "M" "T" "F" "P"
		stDrTreeItem.strMarketCode = ""; 
		stDrTreeItem.strData = saPortName.GetAt(nIndex);
		stDrTreeItem.strTitle = "[관심그룹]" + saPortName.GetAt(nIndex);
		m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);

		nOffset++;
	}
}
//인자로 넘어온 array를 tree에 셋팅한다
void CDrTreeCtrl::SetInitTree_PortData(HTREEITEM htiParent, int nItemIndex , CStringArray &saPortName)
{
	if (htiParent == NULL)
		htiParent = m_htiMarket[5];

	int nOffset;
	nOffset = OFFSET_DATA[nItemIndex];

	DrTreeItem stDrTreeItem;

	for (int nIndex = 0; nIndex < saPortName.GetSize(); nIndex++)
	{
		SetTreeItem(htiParent, saPortName.GetAt(nIndex), m_nTreeIconIndexPort, m_nTreeIconIndexPort, nOffset);

		stDrTreeItem.nIndex = nOffset;
		stDrTreeItem.strDataType = "P";    //# "I" "M" "T" "F" "P"
		stDrTreeItem.strMarketCode = "";
		stDrTreeItem.strData = saPortName.GetAt(nIndex);
		stDrTreeItem.strTitle = "[관심그룹]" + saPortName.GetAt(nIndex);
		m_mapItem.SetAt(stDrTreeItem.nIndex, stDrTreeItem);

		nOffset++;
	}
}

void CDrTreeCtrl::GetKwansimGroupName(CStringArray& arrGroupName)
{
	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();
	pMainWnd->GetKwansimGroupName(arrGroupName);
}

void CDrTreeCtrl::GetItemsWithGroupName(CString strIndex, CStringArray& arrItems)
{
	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();
	pMainWnd->GetItemsWithGroupName(strIndex, arrItems);
}

BOOL CDrTreeCtrl::GetItemNameAndMarket(CString& strCode, CString& strName, int& nMarket)
{
	MSTITEMINFO stMstItemInfo;

	if(m_mapMstItemInfo.Lookup(strCode, stMstItemInfo))
	{
		nMarket = stMstItemInfo.nMarket;
		strName = stMstItemInfo.szItemName;

		return TRUE;
	}
	
	return FALSE;
}

//extern CString		Glb_SubTrCode; //ViewWrap.cpp 파일에 global로 정의되어있음.
int CDrTreeCtrl::Modual_SendTr(CString strTrNo, int nTrLen, BYTE *TrData)
{
 	CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();
 	int nState = pMainWnd->Modual_SendTr(strTrNo, nTrLen, TrData);
	return 1;
}

void CDrTreeCtrl::SetSearch_MSTCode()
{
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	nType = ntohl(nType);
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);
	  
	int nState = Modual_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);

	if (nState < 0)
	  TRACE("조회실패...");
	else
	TRACE("조회중...");
		
	if (lpData)
		delete[] lpData;
}

//# 테마조회
void CDrTreeCtrl::SetSearch_Thema()
{
  /*
	CCommOptionInfo	option;
	CString strTemp;
	
	option.m_strTrCode   = (CString)TRNO_THEMA;
	option.m_Destination = 'X';
	
	BYTE*	lpInputData;
	lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076220_InRec1), sizeof(BYTE));
	memset(lpInputData, ' ', sizeof(fn_HFIBOA076220_InRec1));

	fn_HFIBOA076220_InRec1	lpInput;
	memset(&lpInput, ' ', sizeof(fn_HFIBOA076220_InRec1));
		
	strTemp = "1";
	memcpy(lpInput.Sect2,				(LPCSTR)strTemp,					strTemp.GetLength() );
	strTemp = "W";
	memcpy( lpInput.WMonthSect,			(LPCSTR)strTemp,					strTemp.GetLength() );
	
	memcpy(lpInputData, &lpInput, sizeof(fn_HFIBOA076220_InRec1));

  int nState;
	nState = CommRequestDataOption( GetSafeHwnd(), (BYTE *) lpInputData, sizeof( fn_HFIBOA076220_InRec1 ), (BYTE *)&option );

	free(lpInputData);
  */
}

//# 테마종목조회
void CDrTreeCtrl::SetSearch_ThemaToCode(CString strThemaCode)
{
  /*
	CCommOptionInfo	option;
	CString strTemp;
	
	option.m_strTrCode   = (CString)TRNO_THEMATOCODE;
	option.m_Destination = 'X';
	
	BYTE*	lpInputData;
	lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076210_InRec1), sizeof(BYTE));
	memset(lpInputData, ' ', sizeof(fn_HFIBOA076210_InRec1));

	fn_HFIBOA076210_InRec1	lpInput;
	memset(&lpInput, ' ', sizeof(fn_HFIBOA076210_InRec1));
		
	strTemp = "30";
	memcpy(lpInput.InItem1,				(LPCSTR)strTemp,					strTemp.GetLength() );
//	strTemp = "";
//	memcpy( lpInput.InItem2,			(LPCSTR)strTemp,					strTemp.GetLength() );
//	strTemp = "1";
//	memcpy(lpInput.ContinueKey4,		(LPCSTR)strTemp,					strTemp.GetLength() );
	strTemp = strThemaCode;
	memcpy( lpInput.ThemaCd,			(LPCSTR)strTemp,					strTemp.GetLength() );
	
	memcpy(lpInputData, &lpInput, sizeof(fn_HFIBOA076210_InRec1));
	int nState;
	nState = CommRequestDataOption(GetSafeHwnd(), (BYTE *)lpInputData, sizeof( fn_HFIBOA076210_InRec1 ), (BYTE *)&option);

	free(lpInputData);
  */
}

long CDrTreeCtrl::OnGetData(WPARAM wParam, LPARAM lParam)
{
  int nResult = -1;
/* -hidden-
	CRpDataParser rpParser;
	rpParser.OnGetData (wParam, lParam);

	if (!rpParser.IsError())
	{
		TRACE("OnGetData 총PacketSize   = [%d]\n", rpParser.GetPacketSize() );
		TRACE("OnGetData PacketID(RpID) = [%d]\n", rpParser.GetPacketID()   );
		TRACE("OnGetData TranCode       = [%s]\n", rpParser.GetTranCode()   );
		TRACE("OnGetData ScreenNum      = [%s]\n", rpParser.GetScreenNum()  );
		TRACE("OnGetData Tran결과크기   = [%d]\n\n", rpParser.GetDataSize()   );
    
    CString strTrCode;
    strTrCode = rpParser.GetTranCode();
    strTrCode.TrimRight(" ");

    nResult = ReceiveData(strTrCode, rpParser.GetErrCode(), rpParser.GetErrMsg(), 
						            rpParser.GetData(), rpParser.GetDataSize());
  }
	else
	{
    char *pErrMsg = rpParser.GetErrMsg();

    CString strMsg;
		if (pErrMsg)
      strMsg.Format("ErrorCode[%s]", rpParser.GetErrCode());
		else
			strMsg.Format("ErrorCode[%s]=[%s]", rpParser.GetErrCode(), pErrMsg);

    TRACE(strMsg);
	}
*/
	return nResult;
}

HRESULT CDrTreeCtrl::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	if (dwTRDateLen == 0)		//# 0 : err
	{
    
	}
	else if (dwTRDateLen > 0)
	{
		if ((CString)szTR == TRNO_CONDITION)
		{
			CStringArray saCode;

			CStringArray saResultData;
			SetReceiveData((char*)aTRData, dwTRDateLen, saResultData);
			
			m_pwndParent->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_CLK, (LPARAM)&saResultData);
		}
		else if (szTR == (CString)TRNO_THEMA)
		{
		/*
			fn_HFIBOA076220_OutRec1 *pOutRec1;
			pOutRec1 = (fn_HFIBOA076220_OutRec1 *) rpParser.GetData();

			fn_HFIBOA076220_OutRec2 *pOutRec2;
			pOutRec2 = (fn_HFIBOA076220_OutRec2 *)(rpParser.GetData() + sizeof(fn_HFIBOA076220_OutRec1));

			int nDataCnt;
			INT_CONVERT(pData, pOutRec1->PrntCnt3, sizeof(pOutRec1->PrntCnt3), nDataCnt);

			CString	strCode, strName;
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				fn_HFIBOA076220_OutRec2 *pTmpRec = (fn_HFIBOA076220_OutRec2 *)(pOutRec2 + nIndex);

				STR_CONVERT(pData, pTmpRec->ThemaGrpCd, sizeof(pTmpRec->ThemaGrpCd), strCode);

				STR_CONVERT(pData, pTmpRec->ThemaGrpNm, sizeof(pTmpRec->ThemaGrpNm), strName);
				strName.TrimRight();
			}

			SetInitTree_Item(m_htiThema, nIndex, &saName, &saCode);
		*/
		}
		else if (szTR == (CString)TRNO_THEMATOCODE)
		{
			/*
			fn_HFIBOA076210_OutRec1 *pOutRec1;
			pOutRec1 = (fn_HFIBOA076210_OutRec1 *) rpParser.GetData();
			
			fn_HFIBOA076210_OutRec2 *pOutRec2;
			pOutRec2 = (fn_HFIBOA076210_OutRec2 *)(rpParser.GetData() + sizeof( fn_HFIBOA076210_OutRec1 ));

			int nDataCnt;
			INT_CONVERT(pData, pOutRec1->PrntCnt3, sizeof(pOutRec1->PrntCnt3), nDataCnt);

			CString	strCode, strName;
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				fn_HFIBOA076210_OutRec2 *pTmpRec = (fn_HFIBOA076210_OutRec2 *)(pOutRec2 + nIndex);

				STR_CONVERT(pData, pTmpRec->ItemCd13, sizeof(pTmpRec->ItemCd13), strCode);
				saCode.Add(strCode);

				STR_CONVERT(pData, pTmpRec->ItemNm1, sizeof(pTmpRec->ItemNm1), strName);
				strName.TrimRight();
			}

			m_pwndParent->SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_CLK, (LPARAM)&saCode);
			*/
		}
		else if (szTR == (CString)TRNO_MSTCODE)
		{
			CStringArray saMSTCode;
			MSTCode stMSTCode;
			int nDataCnt = dwTRDateLen / sizeof(MSTCode);

			// 받은 마스터 데이터의 크기가 작을 경우는 오류로 판단하고, 재조회.
			if (dwTRDateLen < MINLEN_MSTCODE_TR)
			{
				CAnalystMainFrmWnd* pMainWnd = (CAnalystMainFrmWnd*)GetParent();

				CString strFile, strDataDir;
				strDataDir.Format("%s%s\\%s", pMainWnd->Variant(homeCC), FOLDER_DATA, gszCacheDir);
				strFile.Format("%s\\%s", strDataDir, "RequestToday.ini");
				::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strFile);
				strFile.Format("%s%s", strDataDir, "\\MstCode.dat");
				::DeleteFile(strFile);

				if (m_nPreReceiveLen == dwTRDateLen)
					AfxMessageBox(_T("\"MstCode.dat\" 파일을 정상적으로 생성할 수 없습니다.\n 화면을 다시 실행하여 주십시요."));
				else
				{
					m_nPreReceiveLen = dwTRDateLen;

					SetSearch_MSTCode();
				}
				return S_OK;
			}

			char *psData;
			CString strCode, strMarket, strName;

			char szCount[5] = {0,};
			memcpy(szCount, aTRData, 4);
			nDataCnt = atoi(szCount);

			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				memset(&stMSTCode, 0x00, sizeof(MSTCode));
				memcpy(&stMSTCode, (char*)aTRData + sizeof(MSTCode) * nIndex + 4, sizeof(MSTCode));

				STR_CONVERT(psData, stMSTCode.sCode, sizeof(stMSTCode.sCode), strCode);
				saMSTCode.Add(strCode);

				MSTITEMINFO stMstItemInfo;
				memset(&stMstItemInfo, NULL, SZ_MSTITEMINFO);

				strMarket.Format("%1.1s", stMSTCode.sMarket);
				stMstItemInfo.nMarket = atoi(strMarket);

				STR_CONVERT(psData, stMSTCode.sName, sizeof(stMSTCode.sName), strName);
				memcpy(stMstItemInfo.szItemName, strName, strName.GetLength());
		//		memcpy(stMstItemInfo.szItemName, stMSTCode.sName, 20);

				m_mapMstItemInfo[strCode] = stMstItemInfo;
			}
			Lib_Open(&saMSTCode);
			saMSTCode.RemoveAll();
		}

		//RestoreWaitCursor();
	}

	return S_OK;
}

int CDrTreeCtrl::GetFindTreeItem(HTREEITEM htItemNode, CString strFindText, CArray<HTREEITEM, HTREEITEM> &saFindItemArray)
{
	if (strFindText.IsEmpty())
		return 0;

	strFindText.MakeUpper();
	HTREEITEM htChildNode;

	if (htItemNode == NULL)
		htChildNode = GetRootItem();
	else
		htChildNode = GetChildItem(htItemNode);

	CString	strItemText;
	while (htChildNode != NULL)
	{
		strItemText =	GetItemText(htChildNode);
		strItemText.MakeUpper();
		if (strItemText.Find((LPCTSTR)strFindText) != -1)
		saFindItemArray.Add(htChildNode);

		if (ItemHasChildren(htChildNode))
			int nResultItem = GetFindTreeItem(htChildNode, strFindText, saFindItemArray);

		htChildNode = GetNextItem(htChildNode, TVGN_NEXT);
	}

	return saFindItemArray.GetSize();
}

void CDrTreeCtrl::SetSelectItem(HTREEITEM hSelItem)
{
	Select(hSelItem, TVGN_CARET);
	GetSelectItemData(hSelItem);
}

int CDrTreeCtrl::InMakeTr_P0621(BYTE *pTrData, BYTE* pTrOutData)
{
	int nInRec2Cnt = MAX_IDXCNT;
	User_Finder* pUserFinder = (User_Finder*)pTrData;
	EU_p0621_InAll* pInAll = (EU_p0621_InAll*)pTrOutData;
	EU_p0621_InRec1 *pInRec1 = (EU_p0621_InRec1*)&pInAll->inRec1;
	EU_p0621_InRec2 *pInRec2 = (EU_p0621_InRec2*)&pInAll->inRec2;

	char tmpVal[100]={0,};
	INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);
	memcpy(pInRec1->giCallGubun				, tmpVal, 4);

	INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);
	memcpy(pInRec1->giTargetGubun			, tmpVal, 2);

	
	INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);
	memcpy(pInRec1->giTargetIdxCnt			, tmpVal, 2);
	tmpVal[2]=0;
	nInRec2Cnt = atoi(tmpVal);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);
	memcpy(pInRec1->giComplexIdxCnt			, tmpVal, 2);
	tmpVal[2]=0;
	nInRec2Cnt += atoi(tmpVal);


	INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);
	memcpy(pInRec1->giTargetResultViewCnt	, tmpVal, 2);
		
	memcpy(pInRec1->gsTargetExp				, pUserFinder->TargetExp, 80);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);
	memcpy(pInRec1->giComplexIdxCnt			, tmpVal, 2);

	INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);
	memcpy(pInRec1->giComplexResultViewCnt	, tmpVal, 2);

	if(pUserFinder->ComplexExp)
		memcpy(pInRec1->gsComplexExp			, pUserFinder->ComplexExp, min(80, strlen(pUserFinder->ComplexExp)));

	INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);
	memcpy(pInRec1->giFieldEditerCnt		, tmpVal, 2);

	// FieldList
	int nFi = 0;
	for(nFi=0; nFi<18; nFi++)
	{
		sprintf(&tmpVal[nFi*4], "%#04.4d", pUserFinder->FieldList[nFi]);
	}
	sprintf(&tmpVal[nFi*4], "%s", "00000000");
	memcpy(pInRec1->giFieldList				, tmpVal, 80);

	INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);
	memcpy(pInRec1->giResultIdxCnt			, tmpVal, 4);

	memcpy(pInRec1->gsResultExp				, pUserFinder->ResultExp, 80);
	memcpy(pInRec1->gsTargetBitChk			, (void*)&pUserFinder->TargetBitChk,  1);
	memcpy(pInRec1->gsJongmokIndex			, pUserFinder->TargetBit,	RESULT_MAX);	//3000 -> 4000
//	memset(pInRec1->gsJongmokIndex			, '1',	2500);
//	memcpy(pInRec1->gsTargetBitChk			, tmpVal, 4);
//	memcpy(pInRec1->gsJongmokIndex			, pUserFinder->ResultExp, 80);
	memcpy(pInRec1->gsSort_flag				, "0", 1);


	User_Define* pUserDefine = pUserFinder->stUser_Define;
	for(int i=0; i<nInRec2Cnt && i<MAX_IDXCNT; i++)
	{
		INT2STR_BUF(tmpVal, 2, pUserDefine[i].IndexNo);
		memcpy(pInRec2[i].giIndexNo		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].GroupNo);
		memcpy(pInRec2[i].giGroupNo		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].GroupIdxCnt);
		memcpy(pInRec2[i].giGroupIdxCnt,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].TargetBase);
		memcpy(pInRec2[i].giTargetBase	,tmpVal, 2);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Type);
		memcpy(pInRec2[i].giType		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].Type1);
		memcpy(pInRec2[i].giType1		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].Type2);
		memcpy(pInRec2[i].giType2		,tmpVal, 2);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].STerm);
		memcpy(pInRec2[i].giSTerm		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].ETerm);
		memcpy(pInRec2[i].giETerm		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Rank);
		memcpy(pInRec2[i].giRank		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].Trend);
		memcpy(pInRec2[i].giTrend		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].SignalCnt1);
		memcpy(pInRec2[i].giSignalCnt1		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserDefine[i].SignalCnt2);
		memcpy(pInRec2[i].giSignalCnt2		,tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserDefine[i].ResultViewNum);
		memcpy(pInRec2[i].giResultViewNum	,tmpVal, 2);
		
		pInRec2[i].gsIndexType[0]		= pUserDefine[i].IndexType;
		pInRec2[i].gsSearchType[0]		= pUserDefine[i].SearchType;
		pInRec2[i].gsOperator[0]		= pUserDefine[i].Operator;
		pInRec2[i].gsReverse[0]			= pUserDefine[i].Reverse;
		pInRec2[i].gsIdxContiGubun[0]	= pUserDefine[i].IdxContiGubun;
		pInRec2[i].gsElement[0]			= pUserDefine[i].Element;

		memcpy(pInRec2[i].gsScode	,pUserDefine[i].Code, 6);

		sprintf(tmpVal, "%#016f%#016f%#016f",	pUserDefine[i].Param[0],
										pUserDefine[i].Param[1],
										pUserDefine[i].Param[2]);
		memcpy(pInRec2[i].gdParam, tmpVal, 48);

		for(nFi=0; nFi<6; nFi++)
		{
			sprintf(&tmpVal[nFi*16], "%#016f", pUserDefine[i].Condition[nFi]);
		}
		memcpy(pInRec2[i].gdCondition	,tmpVal, 96);
	}

	int nRetSize = sizeof(EU_p0621_InRec1) + ( sizeof(EU_p0621_InRec2) * nInRec2Cnt);
	return nRetSize;
}

BOOL CDrTreeCtrl::SetReceiveData(LPTSTR pData, DWORD dwDataLen, CStringArray& arrCode)
{
	LONG lTotCnt = 0L;
	LONG lTime = 0L;
	LONG lFieldCnt = 0L;
	LONG lSize = 0L;
	CString strData;
	
	CString strTemp;
	strTemp.Format("\n# [ConditionSearch] SetReceiveData Size : %d", dwDataLen);
	TRACE(strTemp);

	if (dwDataLen == 0)
		return FALSE;
	
	D1021OUT1* pstD1021Out1 = (D1021OUT1*)pData;

	strTemp.Format("%4.4s", pstD1021Out1->giCallGubun);
	
	strTemp.Format("%4.4s", pstD1021Out1->giTotalJongCnt);
	lTotCnt = atol(strTemp);

	strTemp.Format("%2.2s", pstD1021Out1->giOutFieldCnt);
	lFieldCnt = atol(strTemp);
	
	strTemp.Format("%8.8s", pstD1021Out1->giFindTime);
	lTime = atol(strTemp);
	
	pData = &pData[SIZE_D1021OUT1];
	dwDataLen = dwDataLen - SIZE_D1021OUT1;
	
	D1021OUT2* pstOutListPacket = (D1021OUT2*)pData;
	for (int nIndex = 0; nIndex < lTotCnt; nIndex++)
	{
		BaseData_Code stBaseDataItem;
		memset(&stBaseDataItem, 0x00, SZ_BASEDATA_CODE);

		memcpy(stBaseDataItem.sMarket,	pstOutListPacket[nIndex].gsMarket,	sizeof(pstOutListPacket[nIndex].gsMarket));
		memcpy(stBaseDataItem.sCode,	pstOutListPacket[nIndex].gsCode,	sizeof(pstOutListPacket[nIndex].gsCode));
		memcpy(stBaseDataItem.sName,	pstOutListPacket[nIndex].gsHname,	sizeof(pstOutListPacket[nIndex].gsHname));

		strTemp.Format("%-16.16s", 	pstOutListPacket[nIndex].gdResultField[0]);
		strTemp.TrimLeft(); strTemp.TrimRight();
 		sprintf(stBaseDataItem.sPrice ,	"%-9.9s",	strTemp);
 
 		memcpy(stBaseDataItem.sSign,	pstOutListPacket[nIndex].gsDaebi,	sizeof(pstOutListPacket[nIndex].gsDaebi));	
 
		strTemp.Format("%-16.16s",	pstOutListPacket[nIndex].gdResultField[1]);
		strTemp.TrimLeft(); strTemp.TrimRight();
 		sprintf(stBaseDataItem.sChange, "%-9.9s",	strTemp);
 
 		strTemp.Format("%-16.16s",	pstOutListPacket[nIndex].gdResultField[2]);
 		strTemp.TrimLeft(); strTemp.TrimRight();
 
 		CString strChangeRate;
 		strChangeRate.Format("%-4.2f",	atof(strTemp));
 		sprintf(stBaseDataItem.sChgRate, "%-7.7s",	strChangeRate);
 
 		strTemp.Format("%-16.16s",	pstOutListPacket[nIndex].gdResultField[3]);
 		strTemp.TrimLeft(); strTemp.TrimRight();
 		sprintf(stBaseDataItem.sPre, "%-9.9s",	strTemp);

		arrCode.Add(stBaseDataItem.sCode);
	}
	
	return TRUE;
}


BOOL CDrTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}
