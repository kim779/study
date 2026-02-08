#if !defined(AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
#define AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_

// #include "AgentTreeList.h"
// AgentTreeList.h : header file
//
#include <AFXTEMPL.H>
#include "../../Inc/IGateMng.h"
#include "../../Inc/IMainInfoMng.h"
#include "../../Inc/ITreeListMng.h"
#include "../../Inc/IAUTrCommMng.h"
//#include "../../Dlls/CommonTR/TreeItem.h"
#include "../../Inc/IWinixPlatform.h"

#include "LoadConditionMng.h"		// 원클릭추가를 위해서..
#include "TreeList_Thema.h"

#include "Account.h"
const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));

class CPacketInfo
{
public:
	ITLMCallback* m_pCallback;
	char		m_aDataType;
	long		m_dwReserved;
};
typedef CList<CPacketInfo, CPacketInfo> LIST_CPacketInfo;

//class CTreeResItemEx
//{
//public:
//	ITrComm*		m_pTrComm;
//	LIST_CItemLongData*		m_pItemList;
//	HTREEITEM		m_hGwansim;
//	HTREEITEM		m_hImsi;
//
//public:
//	CTreeResItemEx()
//	{
//		m_pTrComm = NULL;
//		m_pItemList = NULL;
//		m_hGwansim = NULL;
//		m_hImsi = NULL;
//	}
//};
//typedef CList<CTreeResItemEx*, CTreeResItemEx*> LIST_CTreeResItemEx;
// LoadConditionMng.h로 이동.

class CTreeListManager : public ITreeListManager2
{
public:
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, LPCSTR szData) ;
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, long   dwData) ;

	STDMETHOD_(LONG, LoadTreeList)(CInputCodeListEx *pInData, LPCSTR szSection="TREELIST");
	STDMETHOD_(void, UnLoadTreeList)(LONG dwKey);
//	STDMETHOD_(BOOL, LoadFile)(CInputCodeListEx *pInData, LPCSTR szSection="LIST");
	STDMETHOD_(void, RequestData)(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0);
	STDMETHOD_(void, ChangeConfigFile)(LPCSTR szFileName);

	STDMETHOD_(void*, GetITrComm)(LONG dwKey);

	// pITrComm : GetITrComm에서 넘겨받은값.
	STDMETHOD_(void, RequestCodeData)(void* pITrComm, LIST_TR_CODEINFO* pCodeList, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0) ;

	// 관심그룹 트리를 다시 로딩하도록 요청.
	STDMETHOD_(void, ReloadGwansimGroup)(LONG dwKey, CTreeCtrl* pTreeCtrl);

	// 관심그룹이 설정된 TREE의 HITEM
	STDMETHOD_(HTREEITEM, GetGwansimItem)(CTreeCtrl* pTreeCtrl);

	// 트리에서 hTreeRoot밑에서 폴더의 이름을 얻어온다. 상위폴더임.
	// 따라서 관심그룹, 임시그룹등..
	// hTreeRoot이면 상위에서 검색, 
	// 하위폴더로의 검색은 하지 않은다. 같은 레벨에서만 찾는다.
	// hRootItem이 NULL아닌것은 현재 지원하지 않음. 현재 필요가 없어서...
	STDMETHOD_(HTREEITEM, GetTreeItem)(CTreeCtrl* pTreeCtrl, LPCSTR szFolder, HTREEITEM hTreeRoot=NULL);

	// 트리에 관심관련 세팅을 한다.
	// 기존에 관심에서 IMainExInfoManager인터페이스를 통해서. IMainExInfoManager_Gwansim으로 Key로 등록하던 부분을 
	// (같은효과를) 트리인터페이스에도 추가한 것임.
	// 주의 1.관심그룹을 변경하기 전에 이 데이터를 세팅하는 관심에서는 기존데이터를 지워줘야함.
	//      2.관심 내려갈 때 이 포인터도 지워줘야함.
	STDMETHOD_(void, GwansimRegist)(long dwData);

	// 트리의 임시그룹에 등록하는 기능. 기존것은 지우고 새로 만든다.
	//  szSubKey는 사용하지 않으므로 NULL, 추후 임시그룹 및에 그룹명을 더 넣을 경우 대비.
	STDMETHOD_(int, ImsiRegist)(LPCSTR szSubKey, long dwData);

	// 트리의 임시그룹의 내용을 지우는 함수
	//  szSubKey는 사용하지 않으므로 NULL, 추후 임시그룹 및에 그룹명을 더 넣을 경우 대비.
	STDMETHOD_(int, ImsiDelete)(LPCSTR szSubKey);

	// 트리의 임시그룹에 추가하는 함수
	//  szSubKey는 사용하지 않으므로 NULL, 추후 임시그룹 및에 그룹명을 더 넣을 경우 대비.
	// 기존 임시그룹의 데이터를 지우지않고 추가하는 기능
	// STDMETHOD_(int, ImsiAppend)(LPCSTR szSubKey, long dwData);

	STDMETHOD_(void, SetBaseData) (LPCSTR szKey, LPCSTR  szValue) ;
	STDMETHOD_(void, GetBaseData) (LPCSTR szKey, CString &rValue) ;

//->@Solomon에만 해당.
	STDMETHOD_(void, SetSendKey) (HWND hWinix, HWND hDrfnWrap);
	STDMETHOD_(void, SetGwanInterface) (long pInterface);
//<--@Solomon에만 해당.

	STDMETHOD_(int, LoadAccountAfter)( CTreeCtrl* pTreeCtrl,HTREEITEM hParentTree );

public:
	CTreeListManager();
	~CTreeListManager();
	void ClearList(LIST_CBaseLongData* pList);
	void ClearAll();
	
	BOOL LoadPCMASTER(LIST_CBaseLongData *pList, HTREEITEM hTree, LPCSTR szKey);
	BOOL LoadTreeConfig(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szSection, LPCSTR szFile, CListCtrl* pListCtrl=NULL);

	BOOL GetFileName(LPCSTR szKey, CString& szFile);
	int  LoadTreeConfig_Master(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, CString szMasterKey ,LPCSTR szTr ,LPCSTR szInputData);
	int	 LoadGwansimGroup     (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	int	 LoadGwansimGroup	  (CTreeResItemEx* pList, CListCtrl* pListCtrl, LPCSTR szTr ,LPCSTR szInputData);
	BOOL LoadOnlnyGwansimGroup(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	
	int	 LoadAccountGroup	  (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR szInputData);
	void GetAccountGroupCode  (char* buf);
	void GetFOAccountGroupCode(char* buf);
	void SendTr_AccountInfo   (CString strGroupKey);
	void RevData_AccountInfo  (char* buf, long len);
	
//	int  LoadJMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree);
//	int  LoadKMASTER(CTreeCtrl* pTree, HTREEITEM hParentTree);
	int  LoadTree_CUMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CKUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CKRXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int  LoadTree_CKNXUMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int  LoadTree_CTHMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CGMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CELWIndexMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CELWIssueMaster (CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_CUserMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr ,LPCSTR _szInputData, long dwKey1);
	int	 LoadTree_JFMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int	 LoadTree_JPMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_PMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree	 ,LPCSTR szTr ,LPCSTR szInputData ,long dwKey1);
	int  LoadTree_WPMaster(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1);
	int  LoadTree_FolderMaster(int nKind, CString strKind, CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1);
	int  LoadTree_KofexListMaster(int nKind, CString strKind, CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree, LPCSTR szTr, LPCSTR szInputData, long dwKey1);
	
	void RequestDataOfOneClick(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved=0);
	void RequestData_A_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_MASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_A_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_Tr(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);

	// 업종코드 처럼 그 하위의 종목을 리턴하는 경우
	void RequestData_A_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);
	void RequestData_B_SUBMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved);

	void RequestData_TR       (CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
	void RequestData_TR_Group (CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
	void RequestData_HistoryTR(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, char cSort, long dwReserved);
//	void RequestData_Thema_Group(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);
//	void RequestData_Thema_Item(CItemLongData* pILData, ITLMCallback* pCallback, char cDataType, char cSort, long dwReserved);

	// Type='A'이면서 마스터파일에서 읽어야 하는 경우를    위한 루틴...
//	void Callback_Master_J_K(LIST_CCodeTypeA* pListTypeA);
	void Callback_KSMaster(LPCSTR szMasterKey, LIST_CCodeTypeA* pListTypeA, CString &szInputData);	
	void Callback_CJMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CKMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CUMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CTMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CKUMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CETFMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_CELWMaster(LIST_CCodeTypeA* pListTypeA);
	void Callback_NONE(LIST_CCodeTypeA* pListTypeA);
	
	void CallHistory(LIST_CCodeTypeA* pListTypeA);
	void GetGwanGroupCode(CString szGroupName, LIST_TR_CODEINFO* pCodeList);
	LIST_TR_CODEINFO* GetImsiGroupCode(CString szGroupName);

	long GetBitFromCodeInfo(CString& szWonjoCodeChoice);
	int  FP_strlistHistory(CString strHistory, CStringList &strList);

	int RequestData_GroupMASTER(CItemLongData* pILData, ITLMCallback* pCallback, char aDataType, long dwReserved, char cSort);

public:
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);
		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);

	public:
		CTreeListManager* m_pTLMParent;
	} m_xTrCommSite;
	friend class CTreeListManager;


protected:
	BOOL CheckAllListt(POSITION comPos);
	CItemLongData m_clItem;//account 환경파일 저장 tour2k
public:
	CString m_szConfigFile;
	IGateManager*		m_pGeteMng;
	IMainInfoManager*	m_pMainInfoMng;
	//IMasterDataManager* m_pMDMng;
	IMasterDataManagerLast* m_pMDMng;
	IAUTrCommManager*	m_pTrCommMng;
//	ITrComm*			m_pTrComm;

	CTreeCtrl*			m_pTreeCtrl;
	CListCtrl*			m_pListCtrl;
	// LIST_LIST_CItemLongData	m_AllList;
	LIST_CTreeResItemEx	m_AllList;
//	LIST_CItemLongData	m_List;
	LIST_CPacketInfo	m_PacketList;


	void SetIGateManager(long);

	// 관심그룹이 변경되었을 때 불리는 함수.
	HTREEITEM hGwanTree;		// 관심그룹의 HTREEITEM값을 저장한다.
	// void ReLoadGwanGroup(long dwKey, long dwSubKey, long dwNotifyData);
	void DeleteTreeChildInfo(CTreeCtrl* pTree, HTREEITEM hParentTree);

	CString m_strReserved;
	//테마 조회
	//void	Send076220Rq();
	//void	Send076210Rq(CString strKey);
//	int		GetThemaGroup();
//	HRESULT ReceiveDataThemaGroup(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
//	HRESULT ReceiveDataThemaItem(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
//	CMapStringToPtr m_ThemaTrIndex;

	//{{원클릭추가..
	CCondHelper m_LibCondNng;
	void LoadConditonMng(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);
	void LoadConditonMng2(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);	
	void LoadConditonMngUser(CTreeResItemEx* pList, CTreeCtrl* pTree, HTREEITEM hParentTree = NULL);	

	HRESULT ReceiveDataOneClick_Change(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
	HRESULT ReceiveDataOneClick(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);

	HRESULT SetReceiveData_NULL();
	HRESULT SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen);
	BOOL m_bLoadCondMng;
	//}}

//2007.10.05추가 Junok Lee
public:
	CMapStringToString	m_DataTable;
	HWND m_hWinixWnd, m_hWrapper;	//For Solomon
	long m_pWinixGwanInterface;

	//파일에서 읽어오도록 하는 루틴.
	void _DoAlzioSerialize(CArchive& ar,
		CStringArray* parCode, CStringArray* parName, CStringArray* parEngName);

	CPacketInfo m_CurPackInfo;

	ACCOUNT			m_accn;			// 내 계좌목록 hold

	static void CALLBACK fnWinixCommCallbackProc(int nKey, long pThisPoint,long dwUserKey, int nLen, LPBYTE pData);
};

/*******************************************************************************
* 주식 잔고평가 조회 IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* 종목코드         */
    char    hnam[40];   /* 종목명           */
    char    jggb[2];    /* 잔고코드         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */
	
    char    curr[10];   /* 현재가     (*)   */
	char    diff[10];   /* 전일대비         */
    char    rate[10];   /* 등락률           */
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[100];
}; 

#define L_smid	sizeof(struct s_mid)				// Length of stock input

/*******************************************************************************
* 선물/옵션 잔고평가 조회 IO FORMAT
******************************************************************************/
struct	f_mid	{		// 선물 Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	dsgb[6];	/* 구분			*/
	
	char	bqty[10];	/* 수량			*/
	char	cqty[10];	/* 매도가능수량		*/
	char	pprc[10];	/* 평균가/정산가	*/
	char	curr[10];	/* 현재가		*/
	char	diff[10];	/* 평균가 대비		*/
	char	pamt[15];	/* 평가금액		*/
	char	psam[15];	/* 평가손익		*/
	char	prat[10];	/* 수익률		*/
	char	tamt[15];	/* 매입금액		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[100];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_AGENTTREELISTMANAGER_IMPL_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
