#if !defined(AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IAUDrdsMng.h"
#include "IBaseDefine.h"
// 기존에 사용하던 CDrdsLoadLib을 그대로 Matching 시킨다.
//
// 만든이  : 이준옥 2005.11.14(월)
// History : 2005.11.14 인터페이스
//
//	int nID = 19;
// 	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUDrdsManager* pMng = (IAUDrdsManager*)AfxGetPctrInterface(nID);
//
//  _Include/ExDialog.h 파일에 
//	AFX_INLINE IAUDrdsManager*	GetDrdsManager();
//	// 실시간 처리 DRDS는 m_pIDrdsLib
//	AFX_INLINE IDrdsLib* GetDrdsLib();
//  위의 3줄이 정의되어 있으므로 따로 인터페이스를 구하지 말고
//  GetDrdsLib()를 호출해서 사용하기 바랍니다.
//  즉
//  GetDrdsLib()->DrdsInit(..);
//  종료시엔  GetDrdsLib()->DrdsTerm(..);
//
//
// 전체적으로 나열해서 설명하면 아래와 같다.
//  Step1 : 시작할 때
//	IDrdsLib* pDrds = pDrdsMng->AdviseDrdsLib();
//	pDrds->DrdsInit(..);
//  
//  Step6 : 종료할 때
//	pDrds->DrdsTerm(..);
//	pDrdsMng->UnadviseDrdsLib(pDrds);
//
//  Step2 : 실시간등록할 때
//	pDrds->DrdsConnect(..);

//  Step3 : Advise
//	pDrds->DrdsAdvise(..);
//
//  Step4 : Unadvise
//	pDrds->DrdsUnadvise(..);
//
//	Step5 : 실시간해제할 때
//	pDrds->DrdsClose(..);
//
//  [사용강력권고사항]
//  Step3와 Step4은 Step2를 부른후 Step5를 부리기전까지 반복한다.
//  더이상 사용하지 않을 경우에는 Step5를 부르고 빠져나간다.
//
//  전에 CDrdsInfo라는 클래스에서는 Connect후 Advise
//  Unadivse후 DisClose를 부르는 형태로 사용했는데 비효율적인 방법이므로
//  [사용강력권고사항]을 따라주기 바랍니다.
//
//	기타
//	pDrds->DrdsSetGetBrodHwnd(..);
//	pDrds->DrdsSetSocketHandle(..);
//	pDrds->DrdsSendCode(..);

#ifndef __DRDSLOADLIB__DEFINE__
	#define __DRDSLOADLIB__DEFINE__
	#define WM_DRDS_DATA				WM_USER + 200
	#define WM_DRDS_DATA_PACKET_END		WM_USER + 202
	#define MAX_DRDS_PACKET_LEN			4096

	typedef struct  
	{
	 char Name[32];
	 char Field[32];
	 char Key[32];
	 UINT Id;
	 int  Len;
	 char Data[MAX_DRDS_PACKET_LEN];
	} DrdsData;
	#define		SIZE_DrdsData		sizeof(DrdsData)
#endif // __DRDSLOADLIB__DEFINE__



// IDrdsLib
// 기존에 사용하던 DRDS루틴을 그대로 한다. (기존DRDS사용과의 호환을 위해서..)
// 사용시에 IAUDrdsManager인터페이스를 통해 AdviseDrdsLib/UnadviseDrdsLib를 해줘야 한다.
// 다 사용후에는 IAUDrdsManager인터페이스의 UnadviseDrdsLib를 호출해줘야 한다.
// 기존에 Library를 Load/UnLoad하는 것이라고 보면된다.


DECLARE_INTERFACE(IDrdsLib)
{
	// 
	// I-PARAM :
	// O-PARAM : 
	// Def 1-5
	STDMETHOD_(void*, DrdsInit)(HWND hWnd) PURE;
	STDMETHOD_(BOOL, DrdsTerm)() PURE;
	STDMETHOD_(BOOL, DrdsConnect)(LPSTR szDataName) PURE;
	STDMETHOD_(BOOL, DrdsClose)(LPSTR szDataName) PURE;
	STDMETHOD_(BOOL, DrdsAdvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) PURE;

	STDMETHOD_(BOOL, DrdsUnadvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) PURE;
	STDMETHOD_(BOOL, DrdsSetSocketHandle)(HWND hWnd) PURE;
	STDMETHOD_(void, DrdsSendCode)() PURE;
	
	STDMETHOD_(BOOL, AdviseRealCodeData)(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList) PURE;
	STDMETHOD_(int,  SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) PURE;
	STDMETHOD_(BOOL, DrdsAdviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) PURE;
	STDMETHOD_(BOOL, DrdsUnadviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) PURE;
	STDMETHOD_(BOOL, DrdsRequest)(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName) PURE;
	STDMETHOD_(void, DrdsPropertySet)() PURE;
	STDMETHOD_(void, DrdsTraceOn)() PURE;
	STDMETHOD_(void, DrdsTraceOff)() PURE;
	STDMETHOD_(void, DrdsSetFlag)(bool bTrace) PURE;
	STDMETHOD_(void, UDPCreate)() PURE;
	STDMETHOD_(void, UDPCreate2)(LPSTR strIP, LPSTR strPort) PURE;
	STDMETHOD_(void, UDPDestroy)() PURE;
	STDMETHOD_(void, UdpCreate3)(LPSTR sCfgFullPathName) PURE;
	STDMETHOD_(int , SetUserDefData)(LPSTR sKeyName, LPSTR sData) PURE;


	STDMETHOD_(BOOL, Drds_OpenSignalMng)(LPSTR sCfgFullPathName,long lMainHwnd) PURE;
	STDMETHOD_(int,  Drds_SignalMng_SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) PURE;
	STDMETHOD_(long, Drds_GetInterface)(UINT nID) PURE;
	STDMETHOD_(BOOL, Drds_SetInterface)(UINT nID, long dwData) PURE;
	STDMETHOD_(int, ConvertToRealStruct)(WPARAM wParam, LPARAM lParam, char* pStruct, int nRealType=-1) PURE;

	STDMETHOD_(BOOL, GetRealStructData)(WPARAM wParam, LPARAM lParam, char*& prValue, int &nRealTpe) PURE;
};


// 메인에서 관리하는 메인리소스매니저.
// 프레임마다 달리 관리해야 하는 경우가 있기때문에 넣음.
DECLARE_INTERFACE(IAUDrdsManager)
{
	// AdviseDrdsManager, UnAdviseDrdsManager 를 꼭 Call.
	STDMETHOD_(IDrdsLib*, AdviseDrdsLib)() PURE;
	STDMETHOD_(void, UnadviseDrdsLib)(IDrdsLib*) PURE;
};


#endif //AFX_HANARO_IAUDRDSMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
