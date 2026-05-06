#ifndef _EAPI_INTERFACE_DEFINE_H_
#define _EAPI_INTERFACE_DEFINE_H_


#ifdef EAPI_DLL_EXPORT
	#define DLL_EAPI extern "C" __declspec(dllexport)
#else 
	#define DLL_EAPI extern "C" __declspec(dllimport)	
#endif

typedef DWORD (*RESP_CALLBACK)(
	int   nMessageID, 
	char* pszUniqueNO,
//	int   nDeviceType,
	char* pszDeviceType,
	char* pszResult,
	char* pszCause,
	char* pszCauseStr, 
	HWND hWnd, 
	HANDLE hHnd);

typedef DWORD (*EVENT_CALLBACK)(
	int   nMessageID,
	char* pszCallID1,			//	PrimaryCallID(Main)
	char* pszCallID2,			//	SecondaryCallID
	char* pszANI,				//	CallerID
	char* pszDNIS,				//	DNIS
	char* pszCallType,			//	CallType
	char* pszCallDirection,		//	����(IN/OUT)
	char* pszPrevStatus,		//	����Call����
	char* pszCallStatus,		//	����Call����
	char* pszDevicdID,			//  �߻� Line(����)
	HWND hWnd,
	HANDLE hHnd);

typedef DWORD (*UUI_CALLBACK)(
	int   nMessageID, 
	char* pszCallID,
	char* pszUUIData,
	char* pszResult,
	char* pszCause,
	char* pszCauseStr, 
	HWND hWnd, 
	HANDLE hHnd);
#endif