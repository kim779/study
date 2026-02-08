#if !defined(AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUBGMng.h"
#include "IBaseDefine.h"

//  int nID = 7;
// 	IAUBGManager* pMng = (IAUBGManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUBGManager* pMng = (IAUBGManager*)AfxGetPctrInterface(nID);

// 개념 : 백그라운드로 실행이 필요한 모듈을 지원하는 매니저
//
// 만든이  : 이준옥 2005.12.03(토)
//           성공의 3단계법칙
//           1단계 : 확실히 믿고
//           2단계 : 이미 이루어진 것을 바라보고
//           3단계 : 말로 선포하라
// History : 2005.12.03 인터페이스 정의
//

/* ----------------------------------------------------------------------
	// {{ 기본설명
	// HINSTANCE : LoadLib를 했을 때 리턴받는값.

	// 백그라운드로  만드는 DLL의 기본룰
	#1 같은 모듈에 대해서는 하나의 백그라운드만 가능.
	#2	Export함수.
		1. long GetInterface(int nKey);
		2. 
	// }} 기본설명...
---------------------------------------------------------------------- */
DECLARE_INTERFACE(IAUBGManager)
{
	// 화면이 지금 백그라운드로 실행중인지 여부를 리턴한다. 
	// I-PARAM : szModuleName 고유키, DLL의 이름을 넣어주면 고유키가 될듯...
	// 백그라운드로 실행되고 있으면 LoadLib했을 때의 값을 리턴해준다.
	STDMETHOD_(HINSTANCE, IsBackGround)(LPCSTR szModuleName) PURE;

	STDMETHOD_(HINSTANCE, AddBackGround)(LPCSTR szModuleName) PURE;

	// hLib 리턴값도 동일.
	STDMETHOD_(HINSTANCE, AddBackGround)(HINSTANCE hLib) PURE;

	// Reference Count가 0일때는 NULL 그렇지 않으면 hLib리턴.
	STDMETHOD_(HINSTANCE, DelBackGround)(LPCSTR szModuleName) PURE;

	// Reference Count가 0일때는 NULL 그렇지 않으면 hLib리턴.
	STDMETHOD_(HINSTANCE, DelBackGround)(HINSTANCE hLib) PURE;
	
	// DLL과 약속한 인터페이스를 넘겨준다.
	// DLL이 로드가 안되어 있으면 DLL을 로드한다.
	// nKey에 해당하는 인터페이스를 pInterface에 리턴한다.
	STDMETHOD_(HINSTANCE, GetInterface)(LPCSTR szModuleName, int nKey, long &pInterface) PURE;

	// hLib를 알고 있을 때..
	// nKey에 해단하는 정보를 받는다.
	STDMETHOD_(long, GetInterface)(HINSTANCE hLib, int nKey) PURE;

	// hLib를 알고 있을 때..
	// nKey와 pInterface 조합으로 데이터를 넘겨준다.
	STDMETHOD_(BOOL, SetInterface)(HINSTANCE hLib, int nKey, long pInterface) PURE;

};

typedef  long  (WINAPI* BKMNG_GetInterface)(int);
typedef  BOOL  (WINAPI* BKMNG_SetInterface)(int, long  pInterface);
typedef  BOOL  (WINAPI* BKMNG_Terminate)();

#endif // AFX_INTERFACE_OF_BACKGROUNDMANAGER_BY_JUNOK_LEE_8B1A__INCLUDED_


/*
	// 화면이 지금 백그라운드로 실행중인지 여부를 리턴한다. TRUE이면 실행중.
	// I-PARAM : szKey 고유키, DLL의 이름을 넣어주면 고유키가 될듯...
	// O-PARAM : TRUE/FALSE
	STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;

	// BackGround로 실행한다.
	// I-PARAM : szKey 고유키
	//           szModuleName DLL명
	// O-PARAM : 
	STDMETHOD_(void, SetBackGround)(LPCSTR szKey, LPCSTR szDoduleName) PURE;

	// BackGround 실행으로 등록한다.
	// I-PARAM : szKey 고유키
	//           szModuleName DLL명
	//           hModule DLL핸들
	//           pDllKey  : 로드된 DLL의 DllKey(윈도위 포인트)
	// O-PARAM : 
	STDMETHOD_(void, AddBackGround)(LPCSTR szKey, LPCSTR szDoduleName, HINSTANCE hModule, void* pDllKey) PURE;

	// BackGround 실행리스트에서 삭제한다.
	// I-PARAM : szKey 고유키
	//           szModuleName DLL명
	//           hModule DLL핸들
	//           pDllKey  : 로드된 DLL의 DllKey(윈도위 포인트)
	// O-PARAM :
	STDMETHOD_(void, RemoveBackGround)(LPCSTR szKey, LPCSTR szDoduleName, HINSTANCE hModule, void* pDllKey) PURE;
*/


