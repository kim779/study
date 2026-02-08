#if !defined(AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_)
#define AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_

// #include "../../_include/IMainExInfoMng.h"
#include "IBaseDefine.h"
//  int nID = 22;
// 	IMainExInfoManager* pMainExMng = (IMainExInfoManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IMainExInfoManager* pMng = (IMainExInfoManager*)AfxGetPctrInterface(nID);
//	#define		UUID_IMainExInfoManager				22
//	#define		UUID_IMainExInfoManager2			22

//#define UUID_IMainExInfoManager		22
#define UUID_IMainExInfoManager2		22
#define UUID_IMainExInfoManager3		22
#define IMainExInfoManagerLast			IMainExInfoManager3

#define MAX_IMainExInfoManager			41

//  0 - 30 : 예약된 범위.
//	0	소켓핸들	long	O	O	메인
//	1	CodeOcx윈도우 포인터	long	O	O	메인
//	2	Signal.dll Lib핸들(HMODULE)	long	O	O	DRDS
//	3	메인프레임 핸들	long	O	O	메인
//	4	메인 AFX_MODULE_STATE*	long	O	O	메인
//	5		long	O	O	메인
//	6		long	O	O	메인
//	7					메인예약
//	8					메인예약
//	9					메인예약
//	10					메인예약
//	11					메인예약
//	12					메인예약
//	13					메인예약
//	14					메인예약
//	15					메인예약
//  16		long		관심예약
//  17		long		임시그룹예약

#define IMainExInfoManager_Socket		1
#define IMainExInfoManager_CodeOcx		2
#define IMainExInfoManager_Gwansim		16
#define IMainExInfoManager_ImsiGroup	17


// 추후 이 변수를 사용하게 될경우 적절히 늘려서 사용하세요^^
// 지금은 미래를 위해 미리 구조만 잡아놓음.
// #define		MAX_MAINEXDISP				30
#define		MAX_MAINEXDISP				1

DECLARE_INTERFACE_(IMainExInfoManager, IBaseDataDB)
{
	// {{ DISPID를 얻어오는 루틴. 속도 개선을 위해서 넣음.
	// 어디에서인가 한 번 이 루틴이 콜되었으면 처리되도록 하기 위한 루틴임.
	// 이 루틴은 현재 구현하지는 않음. 추후 속도관련하여 개선할 경우 구현하기 바람.
	STDMETHOD_(LONG, GetDispid)(int nID) PURE;
	STDMETHOD_(void, SetDispid)(INT nID) PURE;
	// }}
};

// 사용자가 필요한 데이터를 저장하고 삭제하고 관리하는 기능을 위해서 지원하는 인터페이스
// 사용시에 중복되지 않도록 IsGetUserExData(..)를 통해서 FALSE일때 사용하세요.
// 사용후에는 삭제를 해줘야 메모리에서 사라집니다.
DECLARE_INTERFACE_(IMainExInfoManager2, IMainExInfoManager)
{
	STDMETHOD_(LONG, GetUserExData)(LONG nID) PURE;
	STDMETHOD_(BOOL, SetUserExData)(LONG nID, LONG dwValue) PURE;
	STDMETHOD_(BOOL, DeleteUserExData)(LONG nID) PURE;
	STDMETHOD_(BOOL, IsGetUserExData)(LONG nID) PURE;
};

DECLARE_INTERFACE_(IMainExInfoManager3, IMainExInfoManager2)
{
	STDMETHOD_(BOOL, GetUserStringData)(LPCSTR szKey, CString& rValue) PURE;
	STDMETHOD_(BOOL, SetUserStringData)(LPCSTR szKey, LPSTR szData) PURE;

	STDMETHOD_(POSITION, GetUserData)(LPCSTR szKey, LPBYTE *pData, int &nDataLen) PURE;
	STDMETHOD_(BOOL, SetUserData)(LPCSTR szKey, long pData, int nDataLen) PURE;
};

#define		IMRIM_ROOTDIR			"A12000"
#define		IMRIM_DATADIR			"A12002"
#define		IMRIM_ENCUSERDIR		"A12007"

#define		gszCacheDir				"drcache"
#define		gszDataKey_WorkDate		"WorkDate"
#define		gszDataKey_USDollar		"USDollar"
#define		gszDataKey_MstCode		"MstCode"

#endif //AFX_INTERFACE_OF_MAINEXINFO_1464_45B_BY_JUNOK_LEE_INCLUDED_
