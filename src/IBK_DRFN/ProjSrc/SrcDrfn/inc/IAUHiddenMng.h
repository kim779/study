#if !defined(AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/IAUHiddenMng.h"
#include "IBaseDefine.h"

//  int nID = 7;
// 	IAUHiddenManager* pTrCommMng = (IAUHiddenManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUHiddenManager* pMng = (IAUHiddenManager*)AfxGetPctrInterface(nID);

// 개념 : 
//
// 만든이  : 이준옥 2005.11.02(수)
// History : 2005.11.02 인터페이스 정의
//

DECLARE_INTERFACE(IHiddenSite)
{
	//
	//
	// I-PARAM : 
	// O-PARAM : 
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef  IHiddenSite FAR * LPIHiddenSite;


//DECLARE_INTERFACE(IHiddenItem)
//{
//};

// IAUHiddenManager
DECLARE_INTERFACE(IAUHiddenManager)
{
	//
	// I-PARAM : szKey
	// O-PARAM : BOOL
	STDMETHOD_(BOOL, IsBackGround)(LPCSTR szKey) PURE;
	
	// 주어진 화면을 BackGround로 실행한다. 
	// dllName으로 DLL을 로드한다. 
	//
	STDMETHOD_(void, SetBackGround)(LPCSTR szKey, LPCSTR szDllName) PURE;

	// szKey : 화면KEY
	// szDllName  : DLL이름(여기서는 세팅하지 않아도 된다.)
	// hLoadDll : 로드된 DLL의 인스턴스핸들
	// pDllKey  : 로드된 DLL의 DllKey(윈도위 포인트)
	STDMETHOD_(void, AddBackGround)(LPCSTR szKey, LPCSTR szDllName, HINSTANCE hLoadDll, void* pDllKey)=0;

	// szKey : 화면szKey
	// szDllName  : DLL이름(여기서는 세팅하지 않아도 된다.)
	// hLoadDll : 로드된 DLL의 인스턴스핸들
	// pDllKey  : 로드된 DLL의 DllKey(윈도위 포인트)
	STDMETHOD_(void, RemoveBackGround)(LPCSTR szKey, LPCSTR szDllName, HINSTANCE hLoadDll, void* pDllKey)=0;
};

// 배열의 갯수
#define		ARRAYCOUNT( array )		(sizeof(array) /sizeof(array[0]))


#endif // AFX_INTERFACE_OF_BACKGOUND_EXCUTE_HIDDEND_BY_JUNOK_LEE_8B1A__INCLUDED_
