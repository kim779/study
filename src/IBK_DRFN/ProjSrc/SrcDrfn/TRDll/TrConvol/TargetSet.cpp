// TargetSet.cpp : implementation file
//

#include "stdafx.h"
//#include "../TrConVol/trconvolview.h"
#include "MainFrmWnd.h"
#include "TargetSet.h"

//#include "../../Contents/inc/IPCTRMng.h"
//#include "../../Contents/inc/IGateMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const		LPCTSTR		TARGET_TREE_HEADER[] = { _T("전종목"),  _T("거래소"), _T("코스닥"), 
						_T("거래소"), _T("코스닥"), _T("KOSPI200"), _T("KRX100"), _T("관심종목"),
						_T("보유종목"), _T("최근조회"), _T("테마종목") };

/////////////////////////////////////////////////////////////////////////////
// CTargetSet dialog


//CTargetSet::CTargetSet(CWnd* pParent /*=NULL*/)
//	: CDialog(CTargetSet::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CTargetSet)
//		// NOTE: the ClassWizard will add member initialization here
//	//}}AFX_DATA_INIT
//}

CTargetSet::CTargetSet(CWnd* pParent, CString strScreenNo, ST_SEARCHTARGET* pMainSrc)
	: CDialog(CTargetSet::IDD, pParent)
{
	m_strScreenNo = strScreenNo;
	//m_pDataUtilMng = NULL;
	m_strPassWord = "";

	m_pMainData = pMainSrc;
	STCOPY_P_ST_SEARCHTARGET(pMainSrc, (&m_stSearchTarget));
	
	m_pctrlCodeInput = NULL;
	m_hCodeHwnd		 = NULL;
}

void CTargetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetSet)
//	DDX_Control(pDX, IDC_STATIC_DIV, m_ctrlDiv);
//	DDX_Control(pDX, IDC_STATIC_SUBGROUP, m_ctrlSubGroup);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_ctrlGroup);
	DDX_Control(pDX, IDC_CHK_INVESTRISKNOTICE, m_chkInvestRiskNotice);
	DDX_Control(pDX, IDC_CHK_ELW, m_chkELW);
	DDX_Control(pDX, IDC_CHK_INVESTATTENTION, m_chkInvestAttention);
	DDX_Control(pDX, IDC_CHK_INVESTWARN, m_chkInvestWarn);
	DDX_Control(pDX, IDC_CHK_INVESTRISK, m_chkInvestRisk);
	DDX_Control(pDX, IDC_CHK_DISHONESTNOTICE, m_chkDishonest);
	DDX_Control(pDX, IDC_CHK_LOGICSELLING, m_chkLogicSell);
	DDX_Control(pDX, IDC_CHK_DISPOSITMONEY, m_chkDisposit);
	DDX_Control(pDX, IDC_CHK_PREFERSHARE, m_chkPreferShare);
	DDX_Control(pDX, IDC_CHK_TRADESTOP, m_chkTradeStop);
	DDX_Control(pDX, IDC_CHK_MANAGEITEM, m_chkManage);
	DDX_Control(pDX, IDC_STATIC_REGITEMCNT, m_ctrlRegCnt);
	DDX_Control(pDX, IDC_LIST_EXCEPTRESULT, m_listExResult);
	DDX_Control(pDX, IDC_TREE_SELECTITEM, m_treectrl);
//	DDX_Control(pDX, IDC_CMBCODE,	m_cmbCode);
//	DDX_Control(pDX, IDC_CMBCODENAME,	m_cmbCodeName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetSet, CDialog)
	//{{AFX_MSG_MAP(CTargetSet)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADDTARGET, OnBtnAddExItem)
	ON_BN_CLICKED(IDC_BTN_DELTARGET, OnBtnDelExItem)
	ON_BN_CLICKED(IDC_BTN_DELALLTARGET, OnBtnDelallExItem)
//	ON_CBN_SELCHANGE(IDC_CMBCODE, OnSelchangeCmbCode)
//	ON_CBN_SELCHANGE(IDC_CMBCODENAME, OnSelchangeCmbCodeName)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SELECTITEM, OnChangeTreeItem)
//	ON_MESSAGE(WM_GET_DATA,		OnGetData)		-hidden-
	ON_REGISTERED_MESSAGE(RMSG_DBCLICK, OnRmsgDBClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetSet message handlers

void CTargetSet::OnDestroy() 
{
	if(m_hCodeHwnd && IsWindow(m_hCodeHwnd))
	{
		CWnd::FromHandle(m_hCodeHwnd)->ShowWindow(SW_HIDE);
		CWnd::FromHandle(m_hCodeHwnd)->SetParent(GetParent()->GetParent());
	}

	fnClearTargetSet_Data();

	CDialog::OnDestroy();

	//m_pDataUtilMng = NULL;

	m_saCodeMST.RemoveAll();
	m_saCodeKOSPI.RemoveAll();
	m_saCodeKOSDAQ.RemoveAll();

	m_stSearchTarget.saTargetCode.RemoveAll();
	m_stSearchTarget.saExceptCode.RemoveAll();
	m_stSearchTarget.nSizeTargetcode = m_stSearchTarget.saTargetCode.GetSize();
	m_stSearchTarget.nSizeExceptCode = m_stSearchTarget.saExceptCode.GetSize();
}


BOOL CTargetSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_gridExResult.CreateGrid(WS_CHILD | WS_VISIBLE | WS_BORDER,
    CRect(0, 0, 0, 0), this, IDC_GRID_EXRESULT);

	//memset(&m_stSearchTarget, 0x20, sizeof(ST_SEARCHTARGET));

	CRect rect;
	m_listExResult.GetWindowRect(&rect);
	ScreenToClient(rect);
	m_gridExResult.MoveWindow(&rect);

	SetInit();
	DoCodeRectCalc();
	return TRUE;  
}

void CTargetSet::DisplayStatic()
{
//	m_ctrlSubGroup.SetWindowText("");
//	m_ctrlDiv.SetWindowText("");

	CString szSelGroup;

	if(m_strSubGroup.GetLength()>0)
		szSelGroup.Format("선택그룹: %s/%s", m_strGroup, m_strSubGroup);
	else
		szSelGroup.Format("선택그룹: %s", m_strGroup);

	m_ctrlGroup.SetWindowText(szSelGroup);
//	m_ctrlGroup.SetWindowText(m_strGroup);
//	m_ctrlSubGroup.SetWindowText(m_strSubGroup);

//	CString strData;
//	m_ctrlSubGroup.GetWindowText(strData);
	
//	if( strData == "")
//		m_ctrlDiv.SetWindowText("");
//	else
//		m_ctrlDiv.SetWindowText(":");
	

}

void CTargetSet::SetInit()
{
//	GETPATH_APP
	CString strScreen;
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
	CString strUserPath;
	strUserPath.Format("%s%s%s_%s", pMainFrmWnd->Variant(homeCC), USRDIR, pMainFrmWnd->Variant(nameCC), CONVOL_FILE);

	strScreen = "\\" + m_strScreenNo + "_";
	m_strFullPath = strUserPath + strScreen + CONVOL_FILE;
		

	m_ctrlRegCnt.SetWindowText("0");
	m_strPassWord = "";	// KSLink_GetLoginInfo("USER_PASSWORD"); -hidden-

	SetTrData_ThemeGroup();
//	SetTrData_MSTCode();
	
	CreateTreeCtrl();
		

//	HeaderSet_ExceptGet();
	HeaderSet_ExResult();
	SetInit_Tree();	

	GetInfo_AllItem();
	m_strGroup = m_stSearchTarget.strGroupName;
	m_strSubGroup = m_stSearchTarget.strSubGroupName;
	DisplayStatic();

	DoCheckTreeItem();
}

void CTargetSet::SetLoad_Account(CStringArray *saAccount)
{
/* -hidden-
	saAccount->RemoveAll();

	int nAccCount = KSLink_GetAccountCount();
	
	CString strAccName, strTypeName;
	CString strAccCode, strAccType, strAccPosition;

	for (int nIndex = 0; nIndex < nAccCount; nIndex++)
	{
		strAccCode = KSLink_GetAccount(nIndex);
		if (strAccCode.GetLength() == 11)
		{
			strAccName	= KSLink_GetAccInfo(strAccCode, 3);
			strAccPosition = strAccCode.Left(3);
			strAccType = strAccCode.Mid(3, 2);

			//유진증권 => 지점번호 3자리, 계좌구분 2자리, 계좌번호 6자리 (Total 11자리)
		    //01 - 위탁자, 11 - 위탁계자, 17 - 위탁계자(은행연계), 22 - 선물/옵션, 24 - KOFEX계좌, 36 - 저축계자
			CString strAccDisplay;
			if(strAccPosition == "001" && strAccType != "22" && strAccType != "24") //if(strAccType == "100" || strAccType[0]=='3')
			{
				strAccDisplay = strAccPosition + "-" + strAccType + "-" + strAccCode.Right(6) + " " + strAccName;
				saAccount->Add(strAccCode + ":" + strAccDisplay);			
			}
		}
	}
*/
}


BOOL CTargetSet::GetAccountToHaveCode(CString strAccount, CStringArray *saAccountInCode)
{
	CString strType;
	
	//# Find Account -> Code!
	CStringArray *psaAccountInCode;
	if (m_mapAccountInCode.Lookup(strAccount, (void*&)psaAccountInCode) == TRUE)
	{
		saAccountInCode->Copy(*psaAccountInCode);
		return TRUE;
	}
	if (m_strPassWord.IsEmpty() == TRUE)
		return FALSE;
	
	//#주민 번호 
//	CString strUserRegiNum = KSLink_GetLoginInfo( "JUMIN_NO" ); 
//	CString strAccSelectOpt = "2";   //#1:계좌전체  #2:지정계좌
//	CString strPrevKey, strNextKey;
//	strPrevKey.Format("%40s", " ");
//	strNextKey.Format("%40s", " ");

	strAccount.Replace("-", "");
	m_strAccount = strAccount;	
	CommSendData_Jan(strAccount);
	return FALSE;
}

#include "./DefineFiles/EU_a7001.h"
#include "./DefineFiles/eu_CheJanStruct.h"

long CTargetSet::OnGetData(WPARAM wParam, LPARAM lParam)
{
/* -hidden-
	int nResult = -1;

	AS_SOCK_PACKET * pASPacket = (AS_SOCK_PACKET *)lParam;
	AS_TR_PACKET * pRQRPData = (AS_TR_PACKET *)pASPacket->data;

	char* pData = NULL;
	int nPacketUniqueID = pRQRPData->head.m_windowid;

	if(pRQRPData->head.dhead_type[0] == '1')
		pData = (char*)&pRQRPData->data + sizeof(STAOHDR);	// 데이타 헤더 정보
	else
		pData = (char*)&pRQRPData->data;

	if ( nPacketUniqueID == 10 )		//	전계좌 잔고조회
	{		
		//EU_a7001_OutRec1 outRec1;
		JAN_RQ_OUTPUT1 outRec1;
		memcpy(&outRec1, pData, sizeof(JAN_RQ_OUTPUT1));

		char aCnt[4+1]={0,};
		memcpy(aCnt, outRec1.nCnt, 4);
		int nCnt = atoi(aCnt);

		int nPos = sizeof(JAN_RQ_OUTPUT1);
	//	EU_a7001_OutRec2 OutRec2;
		JAN_RQ_OUTPUT2 OutRec2;

		CString strItemCode, strAccount;
		CStringArray saAccountInCode;
		char *pBuff;		
		for(int i=0; i<nCnt; i++)
		{
			memcpy(&OutRec2, pData+nPos, sizeof(JAN_RQ_OUTPUT2));
			nPos += sizeof(JAN_RQ_OUTPUT2);
//			STR_CONVERT(pBuff, OutRec2.accno, sizeof(OutRec2.accno), strAccount);
			STR_CONVERT(pBuff, OutRec2.jmcode, sizeof(OutRec2.jmcode), strItemCode);
			saAccountInCode.Add(strItemCode);
//			TRACE("%12.12s\n", OutRec2.jmcode);
		}
		SetAccountToCode(m_strAccount, &saAccountInCode);
	}

	delete (BYTE *)lParam;
*/
	return 0L;
}


void CTargetSet::SetAccountToCode(CString strAccount, CStringArray *psaAccountInCode)
{
	CStringArray *psaAccountInCode_Find;
	if (m_mapAccountInCode.Lookup(strAccount, (void*&)psaAccountInCode_Find) == FALSE)
	{
		psaAccountInCode_Find = new CStringArray;
		psaAccountInCode_Find->Copy(*psaAccountInCode);
		m_mapAccountInCode.SetAt(strAccount, psaAccountInCode_Find);

		HTREEITEM hSelectedItem = m_treectrl.GetSelectedItem();
		SetChangeSubTree(hSelectedItem);
	}
	
}

void CTargetSet::SetDelete_Account()
{
	POSITION pos;
	CString strAccount;
	CStringArray *psaAccountInCode;
	for (pos = m_mapAccountInCode.GetStartPosition(); pos != NULL;)
	{
		m_mapAccountInCode.GetNextAssoc(pos, strAccount, (void*&)psaAccountInCode);
		
		psaAccountInCode->RemoveAll();
		delete psaAccountInCode;
		m_mapAccountInCode.RemoveKey(strAccount);
	}
	m_mapAccountInCode.RemoveAll();
}

BOOL CTargetSet::CommSendData_Jan(CString strAcc)
{
/* -hidden-
	CString strUniqueID = "";

	// Transaction Data에 세트할 전문
	REQUEST_COMMINFO	s_RqCommInfo_Jan;

	//	잔고조회
	memset(&s_RqCommInfo_Jan,	0x0, RQ_COMMINFO_SIZE);
	strcpy(s_RqCommInfo_Jan.sServiceName, S_RQ_JAN_CODEINFO);
	s_RqCommInfo_Jan.TrHeader.nTimeOut = 30;
	s_RqCommInfo_Jan.cFIDCommGB	= 0;						// TRAN 통신구분	('0':TR통신,1:FID통신)
	s_RqCommInfo_Jan.TrHeader.nRqID = 10;

	JAN_RQ_INPUT		TrBody_Jan;

	memset( &TrBody_Jan, 0x20,	sizeof( JAN_RQ_INPUT ));
	memcpy( &TrBody_Jan.accno,			strAcc,					strAcc.GetLength());			//	계좌번호

	BYTE ucRet;
	ucRet = CommRequestData(GetSafeHwnd(), &s_RqCommInfo_Jan, (BYTE*)&TrBody_Jan, sizeof(JAN_RQ_INPUT));
	if( ucRet )
	{
//		TRACE("에러 발생 서버로 전송 실패\n");
		return FALSE;
	}
*/
	return TRUE;
}

void CTargetSet::CreateTreeCtrl()
{
/*	CBitmap bmap_on, bmap_off;
	bmap_on.LoadBitmap(IDB_SMENU_ON);
	bmap_off.LoadBitmap(IDB_SMENU_OFF);	
	
	m_treeImageList.Create(IDB_SMENU_OFF, 18, 2, RGB(255, 0, 255));
	m_treeImageList.Add(&bmap_on, RGB(255,0,255));


	m_treectrl.SetImageList(&m_treeImageList, TVSIL_NORMAL);
*/
}

//LPSTR CTargetSet::StringDivideCodeAndName(CString strData, CString &strName)
DWORD CTargetSet::StringDivideCodeAndName(CString strData, CString &strName)
{
	CString strCode;

	strName = strData;
	int nPos = strName.Find(":"); 
	strCode = strName.Left(nPos);
	strName = strName.Mid(nPos+1, strName.GetLength() - nPos);

	//char *sCode;
	//sCode = new char[strCode.GetLength() + 1];
	char sCode[MAX_PATH]={0,};
	memset(sCode, 0x00, strCode.GetLength() + 1);
	memcpy(sCode, (LPSTR)(LPCTSTR)strCode, strCode.GetLength());
	//return sCode;
	ST_TargetSet_Data* pDataItem = new ST_TargetSet_Data;
	pDataItem->szData = sCode;
	POSITION pos = m_ListSTData.AddTail(pDataItem);

	return (DWORD)pos;
}

void CTargetSet::SetInit_Tree()
{
//	if(bTrReceive == FALSE)
//	{
//		return;
//	}
	m_treectrl.DeleteAllItems();
	
	HTREEITEM	hLevel1 = NULL;
	HTREEITEM	hLevel2 = NULL;
	POSITION	hInfoPos = NULL;

	CStringArray saItemCategory;  //업종
//	CStringArray saItemCode;	  //종목
	
	CString strData;
	CString strName;   
	CString strCode;   

	for(int nIndex = 0; nIndex < _countof(TARGET_TREE_HEADER); nIndex++)
	{
		hLevel1 = m_treectrl.InsertItem(TARGET_TREE_HEADER[nIndex]);//,0, 1);

		//char *sCode;
		DWORD lCode;

		if(strcmp(TARGET_TREE_HEADER[nIndex] , "거래소" ) == 0  &&  nIndex >2)			
		{		
			SetLoad_CUMaster(&saItemCategory);
			for(int nSubIndex = 0; nSubIndex < saItemCategory.GetSize(); nSubIndex++)
			{				
				strData = saItemCategory.GetAt(nSubIndex);
				lCode = StringDivideCodeAndName(strData, strName);
				hLevel2 = m_treectrl.InsertItem(strName, hLevel1);
				m_treectrl.SetItemData(hLevel2, lCode);
			}		
		}
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "코스닥") == 0	&&	nIndex >2)
		{			
			SetLoad_KUMaster(&saItemCategory);
			for(int nSubIndex = 0; nSubIndex < saItemCategory.GetSize(); nSubIndex++)
			{
				strData = saItemCategory.GetAt(nSubIndex);
				lCode = StringDivideCodeAndName(strData, strName);
				hLevel2 = m_treectrl.InsertItem(strName, hLevel1);
				m_treectrl.SetItemData(hLevel2, lCode);		
			}				
		}
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "KOSPI200") == 0)
		{			
		}
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "KRX100") == 0)
		{
		}	
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "관심종목") == 0)
		{
			CStringArray saPortGroup;
			//saPortGroup.RemoveAll();
			SetLoad_PortGroup(&saPortGroup);
			for(int nSubIndex = 0; nSubIndex < saPortGroup.GetSize(); nSubIndex++)
			{
				strData = saPortGroup.GetAt(nSubIndex);
				lCode = StringDivideCodeAndName(strData, strName);
				hLevel2 = m_treectrl.InsertItem(strName, hLevel1);
				m_treectrl.SetItemData(hLevel2, (DWORD)lCode);	
			}
		}
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "보유종목") == 0)
		{
			CStringArray saAccount;
			SetLoad_Account(&saAccount);
			for(int nSubIndex = 0; nSubIndex < saAccount.GetSize(); nSubIndex++)
			{					
				strData = saAccount.GetAt(nSubIndex);
				lCode = StringDivideCodeAndName(strData, strName);
				hLevel2 = m_treectrl.InsertItem(strName, hLevel1);
				m_treectrl.SetItemData(hLevel2, (DWORD)lCode);				
			}				
		}
		else if(strcmp(TARGET_TREE_HEADER[nIndex] , "테마종목") == 0)
		{	
			for(int nSubIndex = 0; nSubIndex < m_saGroupTheme.GetSize(); nSubIndex++)
			{				
				strData = m_saGroupTheme.GetAt(nSubIndex);
				strData.TrimRight();
				lCode = StringDivideCodeAndName(strData, strName);
				hLevel2 = m_treectrl.InsertItem(strName, hLevel1);
				m_treectrl.SetItemData(hLevel2, (DWORD)lCode);				
			}			
		}	
	}
}

//void CTargetSet::HeaderSet_ExceptGet()
//{
//	CRect rect;
//	m_listExcept.DeleteAllItems();
//	m_listExcept.GetClientRect(&rect);
//
//	m_listExcept.InsertColumn(0, _T("종목코드"), LVCFMT_LEFT, 60);
//	m_listExcept.InsertColumn(1, _T("종목명"), LVCFMT_LEFT, rect.Width() - 100);
//	
//	m_listExcept.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//}

void CTargetSet::HeaderSet_ExResult()
{
	m_gridExResult.HeaderSetup("CODE");

}

//const LPCTSTR HEADER_EXCEPT[] = {_T("종목코드"), _T("종목명")};
//
//void CTargetSet::SetAdd_ExceptList(CStringArray *saCodeMst)
//{
//	CString strCode, strName;
//	DWORD  lCode;
////	int nIName;
//	for(int nIndex = 0; nIndex < saCodeMst->GetSize(); nIndex++)
//	{	
//		strName = "", strCode = "";
//		lCode = StringDivideCodeAndName(saCodeMst->GetAt(nIndex), strName);
//		strCode = fnGetTargetSet_DataString(lCode);
//		m_listExResult.InsertItem(nIndex, strCode);
//		m_listExResult.SetItemText(nIndex, 1, strName);	
////		m_cmbCode.AddString(strCode);
////		nIName = m_cmbCodeName.AddString(strName);
////		m_cmbCodeName.SetItemData(nIName, nIndex);
//	}		
//}

void CTargetSet::OnChangeTreeItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

 	HTREEITEM item = m_treectrl.GetSelectedItem();

	if(m_treectrl.GetItemData(item) == 0)
		SetChangeGroupTree(item);
	else 
		SetChangeSubTree(item);

	*pResult = 0;
}

void CTargetSet::SetChangeGroupTree(HTREEITEM hItem)
{
	if (m_treectrl.ItemHasChildren(hItem))
		return;

	m_stSearchTarget.saTargetCode.RemoveAll();
	m_stSearchTarget.nSizeTargetcode = m_stSearchTarget.saTargetCode.GetSize();

	CString strGroupName = m_treectrl.GetItemText(hItem);
	CStringArray saData;

//	if(strcmp(strGroupName, "전종목") == 0)      //전종목
//		saData.Add(MARKET_ALL);
//
//	else if(strcmp(strGroupName, "거래소") == 0) //거래소
//		saData.Add(MARKET_KOSPI);
//
//	else if(strcmp(strGroupName, "코스닥") == 0) //코스닥
//		saData.Add(MARKET_KODAQ);
//	else
	if(strcmp(strGroupName, "KOSPI200") == 0)
		SetLoad_K200Code(&saData);

	else if(strcmp(strGroupName, "KRX100") == 0)
		SetLoad_KRX100Code(&saData);

//	if(strcmp(strGroupName, "전종목") != 0 && strcmp(strGroupName, "거래소") != 0 &&
//		strcmp(strGroupName, "코스닥") != 0)
//	{
//		m_stSearchTarget.saTargetCode.Copy(saData);
//		saData.RemoveAll();
//	}

	m_stSearchTarget.saTargetCode.Copy(saData);
	m_stSearchTarget.nSizeTargetcode = m_stSearchTarget.saTargetCode.GetSize();

	m_strGroup = strGroupName;
	m_strSubGroup = "";
	DisplayStatic();

}

void CTargetSet::SetChangeSubTree(HTREEITEM hItem)
{
	if(!hItem) return;
	m_stSearchTarget.saTargetCode.RemoveAll();
	m_stSearchTarget.nSizeTargetcode = m_stSearchTarget.saTargetCode.GetSize();

	HTREEITEM hParent = m_treectrl.GetParentItem(hItem);
	CString strParent = m_treectrl.GetItemText(hParent);
	CString strCategoryCode;
	strCategoryCode = fnGetTargetSet_DataString(m_treectrl.GetItemData(hItem));

	CStringArray saData;

	if(strcmp(strParent, "관심종목") == 0)
	{
		CString strKey;
		strKey= fnGetTargetSet_DataString(m_treectrl.GetItemData(hItem));
		SetLoad_PortGroupToCode(strKey, &saData);
	}
	else if(strcmp(strParent, "테마종목") == 0)
	{
		CString strCode;
		strCode = fnGetTargetSet_DataString(m_treectrl.GetItemData(hItem));
		if(m_saThemeItem.GetSize() == 0)
			SetTrData_ThemeGroupToItem(strCode);
		saData.Copy(m_saThemeItem);
	}
	else if (strcmp(strParent, "보유종목") == 0)
	{
		CStringArray saAccountInCode;
		CString strAccount = fnGetTargetSet_DataString(m_treectrl.GetItemData(hItem));
		GetAccountToHaveCode(strAccount, &saAccountInCode);	
		saData.Copy(saAccountInCode);
	}
	else
	{
		saData.Add(strCategoryCode);
	}
	m_stSearchTarget.saTargetCode.Copy(saData);
	m_stSearchTarget.nSizeTargetcode = m_stSearchTarget.saTargetCode.GetSize();

	saData.RemoveAll();

	CString strhItem = m_treectrl.GetItemText(hItem);
	m_strGroup = strParent;
	m_strSubGroup = strhItem;
	DisplayStatic();
}

//void CTargetSet::SetTrData_MSTCode()
//{
//	int nType = 2;	
//	char* lpData = new char[sizeof(int) + 1];
//	
//	memset(lpData, 0x00, sizeof(lpData) );
//	memcpy(lpData, &nType, sizeof(lpData) );
//
//	int nState = Modual_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE*)lpData);
//
//	if (lpData)
//		delete[] lpData;
//}

//void CTargetSet::SetTrData_MSTCode()
//{
//	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331);
//	if(pDataManager)
//	{
//		// RunTime\CodeMaster\MasterDefine.h
//		// "0192" : 전종목(거래소+코스닥)
//		//	CODE_JONGMOK			"0101"  	// 상장 종목
//		//	CODE_KJONGMOK			"0102"		// 코스닥 종목
//		CStringArray arrCode, arrName;
//		pDataManager->GetMasterData("0101", arrCode, arrName);
//
//		CString strData;
//		int nIndex = 0;
//		for (nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
//		{
//			m_saCodeKOSPI.Add(arrCode.GetAt(nIndex));
//			strData = arrCode.GetAt(nIndex) + ":" + arrName.GetAt(nIndex);
//			m_saCodeMST.Add(strData);
//		}
//
//		arrCode.RemoveAll();
//		arrName.RemoveAll();
//		pDataManager->GetMasterData("0102", arrCode, arrName);
//		for (nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
//		{
//			m_saCodeKOSDAQ.Add(arrCode.GetAt(nIndex));
//			strData = arrCode.GetAt(nIndex) + ":" + arrName.GetAt(nIndex);
//			m_saCodeMST.Add(strData);
//		}
//
//		SetAdd_ExceptList(&m_saCodeMST);
//	}
//}
//////////////
//코스닥 업종
//////////////
void CTargetSet::SetLoad_KUMaster(CStringArray *saCatogoryKosdaq)
{
	saCatogoryKosdaq->RemoveAll();

// 	IMasterDataManager* pDataManager;
// 	pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 1);

	CStringArray *saCode, *saName, *saEngName;;
	saCode = new CStringArray;
	saName = new CStringArray;
	saEngName = new CStringArray;

// 	if(pDataManager)
// 		pDataManager->GetKSQUpMst(saCode, saName, saEngName);

	CString strData;

	for(int nIndex = 0; nIndex < saCode->GetSize(); nIndex++)
	{
		strData = "";
		strData += saCode->GetAt(nIndex) + ":";		
		strData += saName->GetAt(nIndex);		
		saCatogoryKosdaq->Add(strData);
	}
	delete saCode;
	delete saName;
	delete saEngName;
}

//////////////
//코스피 업종
//////////////
void CTargetSet::SetLoad_CUMaster(CStringArray *saCatogoryKospi)
{
	saCatogoryKospi->RemoveAll();

// 	IMasterDataManager* pDataManager;
// 	pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 1);

	CStringArray *saCode, *saName, *saEngName;;
	saCode = new CStringArray;
	saName = new CStringArray;
	saEngName = new CStringArray;

// 	if(pDataManager)
// 		pDataManager->GetKSPUpMst(saCode, saName, saEngName);

	CString strData;

	for(int nIndex = 0; nIndex < saCode->GetSize(); nIndex++)
	{
		strData = "";
		strData += saCode->GetAt(nIndex) + ":";
		strData += saName->GetAt(nIndex);		
		saCatogoryKospi->Add(strData);
	}
	SetLoad_K200Code(saCatogoryKospi);
	delete saCode;
	delete saName;
	delete saEngName;
}

//////////////
//K200Code
//////////////
void CTargetSet::SetLoad_K200Code(CStringArray *saCodeK200)
{
//	saCodeK200->RemoveAll();

// 	IMasterDataManager* pDataManager;
// 	pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 1);
	
	CStringArray *saCode, *saName, *saEngName;;
	saCode = new CStringArray;
	saName = new CStringArray;
	saEngName = new CStringArray;

// 	if(pDataManager)
// 		pDataManager->GetK200UpMst(saCode, saName, saEngName);

	CString strData;

	for(int nIndex = 0; nIndex < saCode->GetSize(); nIndex++)
	{
		strData = "";
		strData += saCode->GetAt(nIndex) + ":";
		strData += saName->GetAt(nIndex);		
		saCodeK200->Add(strData);
	}
	delete saCode;
	delete saName;
	delete saEngName;
}

//////////////
//KRX100Code
//////////////

void CTargetSet::SetLoad_KRX100Code(CStringArray *saCodeKRX100)
{
	saCodeKRX100->RemoveAll();

// 	IMasterDataManager080331* pDataManager;
// 	pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331, 1);

	CStringArray saCode, saName;

	//pDataManager->GetMasterDataInUpDetails("KRXUMASTER", saCode, saName, "501"); //501 : KRX100

	CString strData;
	for(int nIndex = 0; nIndex < saCode.GetSize(); nIndex++)
	{
		strData = "";
		strData += saCode.GetAt(nIndex) + ":";
		strData += saName.GetAt(nIndex);		
		saCodeKRX100->Add(strData);
	}
}

void CTargetSet::SetLoad_PortGroup(CStringArray *saPortGroup)
{
	saPortGroup->RemoveAll();
	
// 	IAUGwanManager* pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager, 1);
// 
// 	CStringArray saGroupKey, saGroupName;
// 	if(pGwanMng)
// 		pGwanMng->GetGwansimGroupName(saGroupKey, saGroupName);
// 
// 	CString strData;
// 	for(int nIndex = 0; nIndex < saGroupKey.GetSize(); nIndex++)
// 	{
// 		strData = "";
// 		strData += saGroupKey.GetAt(nIndex) + ":";
// 		strData += saGroupName.GetAt(nIndex);
// 		saPortGroup->Add(strData);
// 	}
}

void CTargetSet::SetLoad_PortGroupToCode(CString strKey, CStringArray *saPortCode)
{
// 	IAUGwanManager* pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager, 1);
// 
// 	CStringArray saItemCode, saItemName;
// 	if(pGwanMng)
// 		pGwanMng->GetGwansimItem(strKey, saItemCode, saItemName);
// 
// 	CString strData;
// 	for(int nIndex = 0; nIndex < saItemCode.GetSize(); nIndex++)
// 		saPortCode->Add(saItemCode.GetAt(nIndex));
}

void CTargetSet::SetTrData_ThemeGroup()
{
// 	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331, 1);
// 	if(pDataManager)
// 	{
// 		// RunTime\CodeMaster\MasterDefine.h
// 		// "0192" : 전종목(거래소+코스닥)
// 		//	CODE_JONGMOK			"0101"  	// 상장 종목
// 		//	CODE_KJONGMOK			"0102"		// 코스닥 종목
// 		CStringArray arrCode, arrName;
// 		pDataManager->GetMasterData("0602", arrCode, arrName);
// 
// 		CString strData;
// 		for(int nIndex=0; nIndex<arrCode.GetSize(); nIndex++)
// 		{
// 			strData = arrCode.GetAt(nIndex) + ":" + arrName.GetAt(nIndex);
// 			m_saGroupTheme.Add(strData);
// 		}
// 	}
}

void CTargetSet::SetTrData_ThemeGroupToItem(CString strCode)
{
	m_saThemeItem.RemoveAll();

// 	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager080331, 1);
// 	if(pDataManager)
// 	{
// 		CStringArray arrCode, arrName;
// 		pDataManager->GetMasterDataInUpDetails("0602", arrCode, arrName, strCode);
// 
// 		CString strData, strCode, strMarket;
// 		BYTE cType;
// 		for(int nIndex=0; nIndex<arrCode.GetSize(); nIndex++)
// 		{
// 			strCode = arrCode.GetAt(nIndex);
// 			cType = pDataManager->GetMarketTypeFromCode(strCode);
// 			if(cType==1) strMarket = "KOSPI";
// 			else		strMarket = "KOSDAQ";
// 
// 			strData = strCode + ":" + strMarket;
// 			m_saThemeItem.Add(strData);
// 		}
// 		HTREEITEM hSelectedItem = m_treectrl.GetSelectedItem();
// 		SetChangeSubTree(hSelectedItem);
// 	}
}

void CTargetSet::OnBtnAddExItem() 
{
	CString rString;

	CString strCode, strName;
	strCode = m_pctrlCodeInput->GetCode();

	if(strCode.GetLength()>=6)
	{
		strName = GetCodeToName(strCode);
	}
	else return;

	BOOL bAdd = TRUE;
	CString strCheckCode;
	for(int nIndex = 0; nIndex < m_stSearchTarget.saExceptCode.GetSize(); nIndex++)
	{
		strCheckCode = m_stSearchTarget.saExceptCode.GetAt(nIndex);
		strCheckCode = strCheckCode.Left(6);
		if(strCode == strCheckCode)
		{
			bAdd = FALSE;
			CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
			pMainFrmWnd->SetMessageBox(_T("이미 제외종목으로 등록되어 있습니다."), PROGRAM_TITLE, MB_OK);
			break;
		}
	}

	if(bAdd == FALSE)
		return;
	else
	{
		CString strData;
		if(strCode.IsEmpty() == FALSE)
		{
			m_gridExResult.SetAddRow(strCode, strName, TRUE);
			strData = strCode + ":" + strName;
			m_stSearchTarget.saExceptCode.Add(strData);
			m_stSearchTarget.nSizeExceptCode = m_stSearchTarget.saExceptCode.GetSize();

			CString strCnt; 
			strCnt.Format("%d", m_stSearchTarget.saExceptCode.GetSize());
			m_ctrlRegCnt.SetWindowText(strCnt);
		}			
	}	
}

long CTargetSet::OnRmsgDBClick(WPARAM wParam, LPARAM lParam)
{
	OnBtnDelExItem();
	return 0L;
}

void CTargetSet::OnBtnDelExItem() 
{
	CString strCode, strName;
	m_gridExResult.GetRowData(m_gridExResult.m_nLastSelRow, strCode, strName);

	CString strCheckCode;
	for (int nIndex = 0; nIndex < m_stSearchTarget.saExceptCode.GetSize(); nIndex++)
	{
		strCheckCode = m_stSearchTarget.saExceptCode.GetAt(nIndex);
		strCheckCode = strCheckCode.Left(6);
		if (strCode == strCheckCode)
		{
			m_stSearchTarget.saExceptCode.RemoveAt(nIndex);
			m_stSearchTarget.nSizeExceptCode = m_stSearchTarget.saExceptCode.GetSize();
			break;
		}
	}
	m_gridExResult.SetDeleteRow(m_gridExResult.m_nLastSelRow);

	CString strCnt; 
	strCnt.Format("%d", m_stSearchTarget.saExceptCode.GetSize());
	m_ctrlRegCnt.SetWindowText(strCnt);


}

void CTargetSet::OnBtnDelallExItem() 
{
	m_gridExResult.ClearAll();
	m_stSearchTarget.saExceptCode.RemoveAll();	
	m_stSearchTarget.nSizeExceptCode = m_stSearchTarget.saExceptCode.GetSize();
	m_ctrlRegCnt.SetWindowText("0");
}

void CTargetSet::CheckItem_SetInfo()
{
	m_stSearchTarget.nChkManagement =		m_chkManage.GetCheck();
	m_stSearchTarget.nChkTradeStop =		m_chkTradeStop.GetCheck();
	m_stSearchTarget.nChkPrefferShare =		m_chkPreferShare.GetCheck();
	m_stSearchTarget.nChkDepositMoney =		m_chkDisposit.GetCheck();
	m_stSearchTarget.nChkLogicSell =		m_chkLogicSell.GetCheck();
	m_stSearchTarget.nChkInsincerityNotice = m_chkDishonest.GetCheck();
	m_stSearchTarget.nChkInvestRisk =		m_chkInvestRisk.GetCheck();
	m_stSearchTarget.nChkInvestWarn =		m_chkInvestWarn.GetCheck();
	m_stSearchTarget.nChkInvestAttention =	m_chkInvestAttention.GetCheck();
	m_stSearchTarget.nChkELW =				0; //m_chkELW.GetCheck();
	m_stSearchTarget.nChkInvestRiskNotice = m_chkInvestRiskNotice.GetCheck();
}

void CTargetSet::GetInfo_AllItem()
{
//	GetLoad_ConVolTargetInfo(m_strFullPath, &m_stSearchTarget);
//체크 관련 항목
	m_chkManage.SetCheck(m_stSearchTarget.nChkManagement);
	m_chkTradeStop.SetCheck(m_stSearchTarget.nChkTradeStop);
	m_chkPreferShare.SetCheck(m_stSearchTarget.nChkPrefferShare);
	m_chkDisposit.SetCheck(m_stSearchTarget.nChkDepositMoney);
	m_chkLogicSell.SetCheck(m_stSearchTarget.nChkLogicSell);
	m_chkDishonest.SetCheck(m_stSearchTarget.nChkInsincerityNotice);
	m_chkInvestRisk.SetCheck(m_stSearchTarget.nChkInvestRisk);
	m_chkInvestWarn.SetCheck(m_stSearchTarget.nChkInvestWarn);
	m_chkInvestAttention.SetCheck(m_stSearchTarget.nChkInvestAttention);
	m_chkELW.SetCheck(0);  //(m_stSearchTarget.nChkELW);
	m_chkInvestRiskNotice.SetCheck(m_stSearchTarget.nChkInvestRiskNotice);

	HTREEITEM hItem = NULL;//m_stSearchTarget.hLatestItem;
	m_treectrl.SetCheck(hItem);


	char* sCode;
	long lCode;
	CString strData, strName;
	for(int nIndex = 0; nIndex < m_stSearchTarget.saExceptCode.GetSize(); nIndex++)
	{
		strData = m_stSearchTarget.saExceptCode.GetAt(nIndex);
		lCode = StringDivideCodeAndName(strData, strName);
		sCode = (char*)fnGetTargetSet_DataString(lCode);
		m_gridExResult.SetAddRow((CString)sCode, strName, TRUE);
	}

	CString strCnt;   strCnt.Format("%d", m_stSearchTarget.saExceptCode.GetSize());
	m_ctrlRegCnt.SetWindowText(strCnt);
	
}


BOOL CTargetSet::SetSave_ConVolTargetInfo(CString strFullPath, ST_SEARCHTARGET	*pstSEARCHTARGET)
{	
	BOOL bSave = TRUE;
	CString strData;

	TRY
	{
		strData = pstSEARCHTARGET->strGroupName;
		WritePrivateProfileString("TargetSet", "strGroupName", strData, strFullPath);

		strData = pstSEARCHTARGET->strSubGroupName;
		WritePrivateProfileString("TargetSet", "strSubGroupName", strData, strFullPath);
//체크 항목
		strData.Format("%d", pstSEARCHTARGET->nChkManagement);
		WritePrivateProfileString("TargetSet", "nChkManagement", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkTradeStop);
		WritePrivateProfileString("TargetSet", "nChkTradeStop", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkPrefferShare);
		WritePrivateProfileString("TargetSet", "nChkPrefferShare", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkDepositMoney);
		WritePrivateProfileString("TargetSet", "nChkDepositMoney", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkLogicSell);
		WritePrivateProfileString("TargetSet", "nChkLogicSell", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkInsincerityNotice);
		WritePrivateProfileString("TargetSet", "nChkInsincerityNotice", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkInvestRisk);
		WritePrivateProfileString("TargetSet", "nChkInvestRisk", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkInvestWarn);
		WritePrivateProfileString("TargetSet", "nChkInvestWarn", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkInvestAttention);
		WritePrivateProfileString("TargetSet", "nChkInvestAttention", strData, strFullPath);

		strData.Format("%d", 0);//pstSEARCHTARGET->nChkELW);
		WritePrivateProfileString("TargetSet", "nChkELW", strData, strFullPath);

		strData.Format("%d", pstSEARCHTARGET->nChkInvestRiskNotice);
		WritePrivateProfileString("TargetSet", "nChkInvestRiskNotice", strData, strFullPath);

//마지막 선택 트리
//		HTREEITEM hItem = m_treectrl.GetSelectedItem();
//		strData = (LPCTSTR)hItem;
//		WritePrivateProfileString("TargetSet", "hLatestItem", strData, strFullPath);
//선택 그룹 코드

		int nSize;
		nSize = pstSEARCHTARGET->saTargetCode.GetSize();
		pstSEARCHTARGET->nSizeTargetcode = nSize;
	
		strData.Format("%d", nSize);
		WritePrivateProfileString("TargetCode", "nSizeTargetcode", strData, strFullPath);		

		CString strIndex;	
		int nIndex = 0;
		for(nIndex = 0; nIndex < nSize; nIndex++)
		{
			strData = pstSEARCHTARGET->saTargetCode.GetAt(nIndex);
			strIndex.Format("TargetCode_%d", nIndex);			
			WritePrivateProfileString("TargetCode", strIndex, strData, strFullPath);
		}
//제외 종목 코드
		nSize = pstSEARCHTARGET->saExceptCode.GetSize();
		pstSEARCHTARGET->nSizeExceptCode = nSize;

		strData.Format("%d", nSize);
		WritePrivateProfileString("ExceptCode", "nSizeExceptCode", strData, strFullPath);	

		for(nIndex = 0; nIndex < nSize; nIndex++)
		{
			strData = pstSEARCHTARGET->saExceptCode.GetAt(nIndex);
			strIndex.Format("ExceptCode_%d", nIndex);			
			WritePrivateProfileString("ExceptCode", strIndex, strData, strFullPath);
		}
	}
	CATCH(CFileException, pEx)
	{
		bSave = FALSE;
	}
	END_CATCH

	return bSave;
}

BOOL CTargetSet::GetLoad_ConVolTargetInfo(CString strFullPath, ST_SEARCHTARGET	*pstSEARCHTARGET)
{
	BOOL bLoad = TRUE;
	int nData;

	CString strData;
	TCHAR buffer[MAX_PATH];
	char *pBuff;

	TRY
	{
		GetPrivateProfileString("TargetSet", "strGroupName", "전종목", buffer, MAX_PATH, strFullPath);
		pstSEARCHTARGET->strGroupName = buffer;

		GetPrivateProfileString("TargetSet", "strSubGroupName", " ", buffer,MAX_PATH, strFullPath);
		pstSEARCHTARGET->strSubGroupName = buffer;


//체크 항목	
		nData = GetPrivateProfileInt("TargetSet", "nChkManagement", 0, strFullPath);
		pstSEARCHTARGET->nChkManagement = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkTradeStop", 0, strFullPath);
		pstSEARCHTARGET->nChkTradeStop = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkPrefferShare", 0, strFullPath);
		pstSEARCHTARGET->nChkPrefferShare = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkDepositMoney", 0, strFullPath);
		pstSEARCHTARGET->nChkDepositMoney = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkLogicSell", 0, strFullPath);
		pstSEARCHTARGET->nChkLogicSell = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkInsincerityNotice", 0, strFullPath);
		pstSEARCHTARGET->nChkInsincerityNotice = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkInvestRisk", 0, strFullPath);
		pstSEARCHTARGET->nChkInvestRisk = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkInvestWarn", 0, strFullPath);
		pstSEARCHTARGET->nChkInvestWarn = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkInvestAttention", 0, strFullPath);
		pstSEARCHTARGET->nChkInvestAttention = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkELW", 0, strFullPath);
		pstSEARCHTARGET->nChkELW = nData;

		nData = GetPrivateProfileInt("TargetSet", "nChkInvestRiskNotice", 0, strFullPath);
		pstSEARCHTARGET->nChkInvestRiskNotice = nData;

//마지막 선택 트리
//		if(GetPrivateProfileString("TargetSet", "hLatestItem", NULL, buffer, MAX_PATH, strFullPath)>0)
//		{
//			STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
//			HTREEITEM hItem = (HTREEITEM)(LPCTSTR)strData;
//			pstSEARCHTARGET->hLatestItem = hItem;
//		}
//		else
//		{
//			pstSEARCHTARGET->hLatestItem = NULL;
//		}
	
		
//선택 그룹 코드
		pstSEARCHTARGET->saTargetCode.RemoveAll();

		int nSize;
		CString strIndex;
	
		nSize = GetPrivateProfileInt("TargetCode", "nSizeTargetCode", 0, strFullPath);
		pstSEARCHTARGET->nSizeTargetcode = nSize;
		int nIndex = 0;
		for(nIndex = 0; nIndex < nSize; nIndex++)
		{
			strIndex.Format("TargetCode_%d", nIndex);
			if(GetPrivateProfileString("TargetCode", strIndex, "", buffer, 256, strFullPath)>0)
			{
				STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
				strData.TrimRight();
				pstSEARCHTARGET->saTargetCode.Add(strData);
			}
		}
		pstSEARCHTARGET->nSizeTargetcode = pstSEARCHTARGET->saTargetCode.GetSize();
		
			
//제외 종목 코드
		pstSEARCHTARGET->saExceptCode.RemoveAll();
		
		nSize = GetPrivateProfileInt("ExceptCode", "nSizeExceptCode", 0, strFullPath);
		pstSEARCHTARGET->nSizeExceptCode = nSize;
		for(nIndex = 0; nIndex < nSize; nIndex++)
		{
			strIndex.Format("ExceptCode_%d", nIndex);
			if(GetPrivateProfileString("ExceptCode", strIndex, "", buffer, 256, strFullPath)>0)
			{
				STR_CONVERT(pBuff, buffer, sizeof(buffer), strData);
				strData.TrimRight();
				pstSEARCHTARGET->saExceptCode.Add(strData);
			}
		}
		pstSEARCHTARGET->nSizeExceptCode = pstSEARCHTARGET->saExceptCode.GetSize();

	}
	CATCH(CFileException, pEx)
	{
		bLoad = FALSE;
	}
	END_CATCH

	return bLoad;
}


void CTargetSet::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	
}

//BOOL CTargetSet::PreTranslateMessage(MSG* pMsg) 
//{
//	switch (pMsg->message)
//	{
//		case WM_KEYDOWN:
//		{
//			switch (pMsg->wParam)
//			{
//				case VK_RETURN:
//				{
//					if(pMsg->hwnd)
//						{
//							CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
//							if(pWnd)
//							{
//								HWND hFocus = pWnd->GetParent()->GetSafeHwnd();
//								HWND hFocus1 = pWnd->GetSafeHwnd();
//								//콤보의 Edit가 선택되었을 경우대비.
//								if(hFocus==m_cmbCodeName.GetSafeHwnd() ||hFocus1==m_cmbCodeName.GetSafeHwnd())
//								{
//									fnSyncCombo(2);
//									OnBtnAddExItem();
//									return 1L;
//								}
//								else if(hFocus==m_cmbCode.GetSafeHwnd() || hFocus1==m_cmbCode.GetSafeHwnd())
//								{
//									fnSyncCombo(1);
//									OnBtnAddExItem();
//									return 1L;
//								}
//							}
//						}
//					}
//				break;
//			}
//		}
//		break;
//	}
//	
//	return CDialog::PreTranslateMessage(pMsg);
//}

//#include "../../inc/IMasterDataMng.h"
CString CTargetSet::GetCodeToName(CString strCode)
{
	//CString strName = "";
	
/*	int nID = 5;
	IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);
	if (pManager != NULL)
	{
		strName = pManager->GetCodeInfo("JMASTER", 0, strCode);
		
		if (strName.IsEmpty() == TRUE)
			strName = pManager->GetCodeInfo("KMASTER", 0, strCode);
	}
*/
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();

	char szBuf[16];
	sprintf(szBuf, "%s ", strCode);
	CString strName = (char*)pMainFrmWnd->GetModuleMain()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBuf);
	strName.TrimLeft();
	strName.TrimRight();

	return strName;
}

//BOOL CTargetSet::DoCheckCodeFirst()
//{
//	int nSel = m_cmbCodeName.GetCurSel();
//	if(nSel<0)
//	{
//		CString rString;
//		m_cmbCodeName.GetWindowText(rString);
//		if(rString.GetLength()==6)
//		{
//			nSel = m_cmbCode.FindString(0, rString);
//			if(nSel<0)	return FALSE;
//
//			m_cmbCode.SetCurSel(nSel);
//
//			CString szCodeName = GetCodeToName(rString);
//			int nIndex = m_cmbCodeName.FindString(0, szCodeName);
//			if(nIndex>0) m_cmbCodeName.SetCurSel(nIndex);
//			return TRUE;
//		}
//	}
//	return FALSE;
//}


//void CTargetSet::fnSyncCombo(int nType)
//{
//	CString rString;
//	if(nType==1) // 종목CodeCombo
//	{
//		int nSel = m_cmbCode.GetCurSel();
//		if(nSel<0)
//		{
//			m_cmbCode.GetWindowText(rString);
//			nSel = m_cmbCode.FindString(0, rString);
//			if(nSel<0)	return;
//		}
//
//		m_cmbCode.SetCurSel(nSel);
//		m_cmbCode.GetLBText(nSel, rString);
//
//		CString szCodeName = GetCodeToName(rString);
//		int nIndex = m_cmbCodeName.FindString(0, szCodeName);
//		if(nIndex>0) m_cmbCodeName.SetCurSel(nIndex);
//	}
//	else if(nType==2) // 종목명Combo
//	{
//		// 종목코드 입력 콤보를 감추도록 수정이 되어서 수정함.
//		if(DoCheckCodeFirst()==TRUE)
//			return;
//		
//		int nSel = m_cmbCodeName.GetCurSel();
//		if(nSel<0)
//		{
//			m_cmbCodeName.GetWindowText(rString);
//			nSel = m_cmbCodeName.FindString(0, rString);
//			if(nSel<0)	return;
//
//			m_cmbCodeName.SetCurSel(nSel);			
//		}
//		int nIndex = (int)m_cmbCodeName.GetItemData(nSel);
//		m_cmbCode.SetCurSel(nIndex);
//	}
//}


//void CTargetSet::OnSelchangeCmbCode()
//{
//	fnSyncCombo(1);
//}
//
//void CTargetSet::OnSelchangeCmbCodeName()
//{
//	fnSyncCombo(2);
//}

void CTargetSet::fnClearTargetSet_Data()
{
	for(POSITION pos=m_ListSTData.GetHeadPosition(); pos; )
	{
		delete m_ListSTData.GetNext(pos);
	}
	m_ListSTData.RemoveAll();
}

LPCSTR CTargetSet::fnGetTargetSet_DataString(DWORD _pos)
{
	POSITION pos = (POSITION)_pos;
	if(!pos) return "";

	static ST_TargetSet_Data* pItemData = m_ListSTData.GetAt(pos);
	return (LPCSTR)pItemData->szData;
}


void CTargetSet::OnOk() 
{
	CheckItem_SetInfo();

	CString strData;
	m_ctrlGroup.GetWindowText(strData);

	strData.TrimLeft();
	strData.TrimRight();
	if(strData.GetLength()==0)
		strData = "전종목";

	strData.TrimRight();
	int nPos = strData.Find(":");

	if(nPos>0)
		strData = strData.Mid(nPos+1);
	strData.TrimLeft();

	CString strGroupName, strSubGroupName;
	nPos = strData.Find("/");

	strGroupName = strData;

	if(nPos>0)
	{
		strGroupName = strData.Left(nPos);
		strSubGroupName = strData.Mid(nPos+1);
	}

	m_stSearchTarget.strGroupName	= strGroupName;
	m_stSearchTarget.strSubGroupName= strSubGroupName;

	STCOPY_P_ST_SEARCHTARGET((&m_stSearchTarget), m_pMainData);

	CString strCode;

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();

	for(int nIndex = 0; nIndex <m_stSearchTarget.saTargetCode.GetSize(); nIndex++)
	{
		if(m_stSearchTarget.saTargetCode.GetSize()>0)
			strCode = m_stSearchTarget.saTargetCode.GetAt(nIndex);

//		if(strCode.CompareNoCase("0") == 0)
//			g_iMetaTable.ShowMessageBox(m_hWnd, "%s", strCode);
		if(strCode.GetLength() < 3)
			pMainFrmWnd->SetMessageBox(strCode, PROGRAM_TITLE, MB_OK);
		
	}

	CDialog::OnOK();
}

void CTargetSet::DoCodeRectCalc()
{
	if(m_hCodeHwnd && IsWindow(m_hCodeHwnd))
	{
		CRect rectComp, ctlRect;
		GetDlgItem(IDC_CMBCODENAME)->GetWindowRect(rectComp);
		ScreenToClient(rectComp);

		CWnd* pWnd = CWnd::FromHandle(m_hCodeHwnd);
		pWnd->SetWindowPos(&CWnd::wndTop, 
				rectComp.left, rectComp.top, rectComp.Width(), rectComp.Height(), SWP_NOZORDER);
		pWnd->SetParent(this);
		pWnd->ShowWindow(SW_SHOW);
	}
}

void CTargetSet::DoCodeInput(int nType)
{
	if( nType==2 || nType==4 )
		OnBtnAddExItem();
}

//#include "../../inc/HelperTreeCtrl.h"
void CTargetSet::DoCheckTreeItem()
{
	if(m_strGroup.GetLength()==0) return;

	BOOL bSub = m_strSubGroup.GetLength();
// 	HTREEITEM hItem = CHelperTreeCtrl::FindTreeItemEx(&m_treectrl, m_strGroup, NULL, bSub);
// 	if(hItem)
// 	{
// 		if(bSub)
// 		{
// 			hItem = CHelperTreeCtrl::FindTreeItem(&m_treectrl, m_strSubGroup, hItem);			
// 		}
// 		if(hItem)
// 		{
// 			m_treectrl.EnsureVisible(hItem);
// 			m_treectrl.SelectItem(hItem);
// 		}
// 	}
}

