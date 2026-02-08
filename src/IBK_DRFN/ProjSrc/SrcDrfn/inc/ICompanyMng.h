#if !defined(AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

/* ------------------------------------------------------------------------
사이트마다 다른 부분을 처리하기 위한 루틴
사이트에 맞게 꼭 변경하기 바랍니다.
필요한 경우 PcTrMng에서 43번으로 정의해서 사용합니다.
------------------------------------------------------------------------ */

// #include "../../inc/ICompanyMng.h"
#include "IBaseDefine.h"

#define		UUID_ICompanyManager		43
//  int nID = UUID_ICompanyManager;
// 	ICompanyManager* pGateMng = (ICompanyManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ICompanyManager* pMng = (ICompanyManager*)AfxGetPctrInterface(nID);

enum 
{
	nPCTRComID_DAISHIN=1,	//대신
	nPCTRComID_DAEWOO	,	//대우
	nPCTRComID_HYUNDAI	,	//현대
	nPCTRComID_WOORI	,	//우리
	nPCTRComID_HANWHA	,	//한화
	nPCTRComID_DONGBU	,	//동부
	nPCTRComID_SK		,	//SK	
	nPCTRComID_KYOBO	,	//교보
	nPCTRComID_DONGWON	,	//동원
	nPCTRComID_KIWOOM	,	//키움
	nPCTRComID_SEOUL	,	//서울
	nPCTRComID_SAMSUNG	,	//삼성
	nPCTRComID_BOOKOOK	,	//부국
	nPCTRComID_HANTU	,	//한투
	nPCTRComID_KOSCOM	,	//증전
	nPCTRComID_EUGENE	,	//유진
	nPCTRComID_KB			//KB
};

// 사이트 회사별 처리를 위한 루틴
// 현재 이루틴은 삼성용.
// 다른 사이트는 별도로 사용바람.
DECLARE_INTERFACE(ICompanyManager)
{
	//위에 정의된 순서.
	STDMETHOD_(int, GetID)() PURE;

	// 위에 정의된 변수중 'nPCTRComID_'를 뺀 부분.
	// ex nPCTRComID_SAMSUNG -> SAMSUNG
	STDMETHOD_(LPCSTR, GetName)(int nID) PURE;
};

typedef void (CALLBACK* SysEduCallbackProc)(long pThisPoint, LPCSTR dwMainKey, long dwSubKey, long dwNotifyData);


typedef struct 
{
	long pThisPoint;
	CString szMainKey;
	long dwSubKey;
	long dwNotifyData;
} STSysCallProcItem;

#define		ACC_SERIAL			0		// 약정상품일련번호(3)
#define		ACC_PWD				1		// 계좌비밀번호  Passwd(8)
#define		ACC_CODE_NAME		2		// 계좌명(20)
#define		ACC_PRODUCTCODE		3		// 약정상품코드(3)
#define		ACC_PRODUCNAME		4		// 약정상품유형명(30)
#define		ACC_TYPECODE		5		// 계좌개설구분코드(3)
#define		ACC_LAPTYPECODE		6		// 랩구분코드(1)(주문을 직원에게 위임, 고객은 조회만 가능)
#define		ACC_OLDACCTYPECODE	7		// 구계좌구분상세코드(2)
#define     ACC_STIPYN          8		// 종합약관확인여부(1)
#define		ACC_ISALL			9		// 종합여부(1)
#define		ACC_ISCLOSE			10		// 폐쇄여부(1) 
#define		ACC_HTS				11		// HTS사용등급구분코드(1)
#define		ACC_ISPWD			12		// 비밀

typedef struct
{
	CString szAcct;					// 계좌번호
	CString	InData[ACC_ISPWD+1];	// 계좌관련정보.
} STACC_SSDATA;	// 삼성계좌정보.

typedef CList<STACC_SSDATA*, STACC_SSDATA*> LIST_STACC_SSDATA;

#define LIST_STACC_SSDATA_DELETEALL(xList) { for(POSITION pos=xList.GetHeadPosition(); pos;) delete xList.GetNext(pos); }
#define LIST_STACC_SSDATA_DELETEALLP(xpList) { for(POSITION pos=xpList->GetHeadPosition(); pos;) delete xpList->GetNext(pos); }

DECLARE_INTERFACE_(ICompanyManager_SS, ICompanyManager)
{
	STDMETHOD_(int, GetAcctCode)(CStringList& listAcct, CStringList& listAcctName) PURE;
	STDMETHOD_(CString, GetPwd)() PURE;

//	STDMETHOD_(long, OnGetData)(WPARAM wParam, LPARAM lParam) PURE;
//
//	// 선물주문.
//	// 1:HTS  0:FRONT(지점)
//	STDMETHOD_(int, RqNewOrder)(HWND hWnd, long/*COrderSave**/ pSave, int p_nUserType) PURE;	//COrderSave* pSave
//	
//	// 주식매수주문.
//	// 1:HTS  0:FRONT(지점)
//	STDMETHOD_(int, RqNewOrderStockBuy)(HWND hWnd, long _pSave, int p_nUserType) PURE;
//
//	// 주식매도주문.
//	// 1:HTS  0:FRONT(지점)
//	STDMETHOD_(int, RqNewOrderStockSell)(HWND hWnd, long _pSave, int p_nUserType) PURE;
//
//	// 주문이 성공했을 때만 콜
//	STDMETHOD_(BOOL, GetLastMessage)(CString& szMsgCode, CString& szMsg) PURE;


	// 리턴값 : 0(미교육), 1(교육완료), 0이하값(조회전상태)
	STDMETHOD_(int, GetSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc) PURE;

	STDMETHOD_(void, RemoveSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc) PURE;

	// listAcct : 순수한 계좌만.
	// listAcctName : 계좌명만.
	// listAcctType : 계좌타입 구분자.
	// listAcctTypeName : 계좌타입에 따른 이름.
	STDMETHOD_(int, GetAcctCodeEx)(LIST_STACC_SSDATA &ListAcctInfo, int nReqType) PURE;

	// 계좌정보 구하기
	// nAcctSize 계좌자리수, 실제는 11자리. 보여주는 8자리가지고도 정보를 구할 수 있다.
	//			8 or 11 만가능하게 되어있음.
	// nInfoType
	//				0		// 약정상품일련번호(3)
	//				1		// 계좌비밀번호  Passwd(8)
	//				2		// 계좌명(20)
	//				3		// 약정상품코드(3)
	//				4		// 약정상품유형명(30)
	//				5		// 계좌개설구분코드(3)
	//				6		// 랩구분코드(1)(주문을 직원에게 위임, 고객은 조회만 가능)
	//				7		// 구계좌구분상세코드(2)
	//				8		// 종합약관확인여부(1)
	//				9		// 종합여부(1)
	//				10		// 폐쇄여부(1) 
	//				11		// HTS사용등급구분코드(1)
	//				12		// 비밀
	STDMETHOD_(LPCSTR, GetAcctInfo)(LPCSTR szAcct, int nAcctSize, int nInfoType) PURE;
};

//#define	gCURRENT_COMPANY_ID	nPCTRComID_SAMSUNG	// 삼성
#define	gCURRENT_COMPANY_ID	nPCTRComID_EUGENE	// 유진

#define SZFNPROMAX_SYSEDU		"FNPROMAX_SYSEDU"
#define SZEUGENE_SYSEDU			"EUGENE_SYSEDU"

#define gGetSamsungSysEdu(xSysEdu) { xSysEdu=-1; char aBuf[10+1]={0,};		\
	if(::GetEnvironmentVariable(SZFNPROMAX_SYSEDU, aBuf, 10)>0)	{		\
	if(strcmp(aBuf, "1")==0 || strcmp(aBuf, "0")==0) xSysEdu = atoi(aBuf);	}}

#define gGetEugeneSysEdu(xSysEdu) { xSysEdu=-1; char aBuf[10+1]={0,};		\
	if(::GetEnvironmentVariable(SZEUGENE_SYSEDU, aBuf, 10)>0)	{		\
	if(strcmp(aBuf, "1")==0 || strcmp(aBuf, "0")==0) xSysEdu = atoi(aBuf);	}}

//#define	SAMSUNG_VERSION		200703	// 2007.03오픈한 운영중인 Max
#define	SAMSUNG_VERSION		200709	// 2007.09오픈용 신시스템버전.

#endif //AFX_HANARO_ICOMPANYMAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
