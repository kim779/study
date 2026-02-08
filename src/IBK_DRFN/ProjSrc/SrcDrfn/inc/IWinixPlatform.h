#if !defined(AFX_INTERFACE_OF_WINIXPLATFORM_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_WINIXPLATFORM_BY_JUNOK_LEE_8B1A__INCLUDED_

#include <AfxTempl.h>
// IWinixPlatform.h

const UINT RMSG_WINIXPLATFORM = ::RegisterWindowMessage(_T("RMSG_WINIXPLATFORM"));
typedef void (CALLBACK* WinixCommCallbackProc)(int nKey, long pThisPoint, long dwUserKey, int nLen, LPBYTE pData);

#define		rwp_adivse		1
#define		rwp_unadivse	0

#define		rwpComm		0x01
	//MAKEWPARAM(rwpComm, rwp_adivse)
	//MAKEWPARAM(rwpComm, rwp_unadivse)

typedef struct
{
	int nKey;
	long pThisPoint;
	int dwUserKey;
	WinixCommCallbackProc pfnCall;
} stWINIXComm;
typedef CList<stWINIXComm*, stWINIXComm*> List_stWINIXComm;

#define Copy_stWINIXComm(pFromSt, pToSt) pToSt->nKey=pFromSt->nKey; \
		pToSt->pThisPoint=pFromSt->pThisPoint;	\
		pToSt->dwUserKey=pFromSt->dwUserKey;	\
		pToSt->pfnCall=pFromSt->pfnCall;

// 	//헤더파일정의
// 	static void CALLBACK fnWinixCommCallbackProc(int nKey, long pThisPoint,long dwUserKey, int nLen, LPBYTE pData);
// 
// 	//CPP파일구현.
// 	void YourClass::fnWinixCommCallbackProc(int nKey, long pThisPoint, long dwUserKey, int nLen, LPBYTE pData)
// 	{
// 		YourClass* pThis = (YourClass*)pThisPoint;
// 	}


#endif // AFX_INTERFACE_OF_WINIXPLATFORM_BY_JUNOK_LEE_8B1A__INCLUDED_
