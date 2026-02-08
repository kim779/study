#if !defined(AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/OcxDLLDef.h"

#include "WndObj.h"

// * DLL 화면 로드시 필요한 데이타 구조체 
typedef struct {
	HWND		m_hSocket;				// 소켓 핸들
	HWND		m_hParent;				// 부모 핸들
	CString		m_szRootPath;			// 루트 경로	
	CString		m_szUserID;				// 유저 id	
	CString		m_szMapID;				// 화면 번호
	CString		m_szEnvFilePath;		// 환경파일 경로
	long		m_dwInterface;			// 공유 관리자 포인터
	long		m_dwAcctManager;		// 계좌 관리자 포인터 
	CString		m_szExtra1;				// 임시
	CString		m_szExtra2;				// 임시
	long		m_lExtra1;				// 임시
	long		m_lExtra2;				// 임시 
	long		m_lExtra3;				// 임시 
} StDllLoadData, *LPStDllLoadData;

#define		Init_StDllLoadData(pSt)		pSt->m_hSocket=NULL;	\
				pSt->m_hParent=NULL;		\
				pSt->m_dwInterface=NULL;	\
				pSt->m_dwAcctManager=NULL;	\
				pSt->m_lExtra1=NULL;		\
				pSt->m_lExtra2=NULL;

typedef void*		(WINAPI * FpDllLoad) (void*);
typedef void		(WINAPI * FpDllUnload) (void*);
typedef void		(WINAPI * FpSetData) (void*, void*);
typedef void		(WINAPI * FpFormLoad) (void*);
typedef void		(WINAPI * FpSetKeyData) (void*, LPCSTR szKey, void*, void*, void*);
typedef void		(WINAPI * FpFuncCall) (void*, LPCSTR szFuncName, LPCSTR szParam, VARIANT& result);

// --------------------------------------------------------------------------
// OCX_DLL은 스크립트에서 직접 제어가 가능할 수 있도록 IDispatch에서 상속받아 처리를 한다.
// IDispatch 값은 DLL_OCX를 통해 그 값을 넘겨줘서 맵에서 DLL을 직접 제어한다.
// --------------------------------------------------------------------------
//typedef struct {
//	HRESULT InvokeHelper(LPCSTR szFuncName, LPCSTR szParam, VARIANT& result) { return S_FALSE; }
//	HRESULT GetProperty(LPCSTR szProperty, VARIANT& data) { return S_FALSE; }
//	HRESULT SetProperty(LPCSTR szProperty, VARIANT  data)  { return S_FALSE; }
//} 

// #define		MAXDLLPARAM			3
typedef struct {
//	char	m_aParam[MAXDLLPARAM][1024+1];	// 파라미터
	long	m_dwReserved1;
} STODINIT_IN;
#define		SZSTODINIT_IN		sizeof(STODINIT_IN)
#define		INIT_STODINIT_IN(pst)		pst->m_dwReserved1=NULL;


typedef struct {
	void		*m_pDllKey;
	IPlatformObj *m_pPlatformObj;
} STODINIT_OUT;
#define		SZSTODINIT_OUT		sizeof(STODINIT_OUT)
#define		INIT_STODINIT_OUT(pst)		pst->m_pDllKey=NULL;	pst->m_pPlatformObj=NULL;
#define		INIT_STODINIT_OUT2(st)		st.m_pDllKey=NULL;	st.m_pPlatformObj=NULL;


typedef struct {
	STODINIT_OUT *m_pInitOut;
	HWND	m_hParent;
	CRect	m_Rect;		// 영역(Rect)
	UINT	m_nID;		// 생성할 컨트롤의 ID
	//char	szMapFileName[30+1];
	char	m_aOpenInfo[300+1];
} STODCREATEINFO_IN;
#define		SZSTODCREATEINFO_IN		sizeof(STODCREATEINFO_IN)

#define		INIT_STODCREATEINFO_IN(pstIn)		pstIn->m_hParent=NULL;	\
				pstIn->m_nID=0;		ZeroMemory(pstIn->m_aOpenInfo, sizeof(pstIn->m_aOpenInfo));


typedef struct {
//	char		szDllKey[30+1];		// DLL키값은 최대 30으로 한다. NULL스트링.
//	                                // DLL의 키값은 핵심이름은 "maindll"이라는 이름을 사용한다.
//									// TR I/O관련 GetBase는 되도록 첫번째에서 한다.
	IDispatch*	pDispatch;			// DLL키값에 해당하는 Dispatch
} STDLLDISPATCH;

// #define		MAXDLLDISPATCH			3
typedef struct {
	STODINIT_OUT m_InitOut;
	
	IAWndObj	*m_pWndObj;		// IAWndObj* 포인트
	// STDLLDISPATCH m_stDllDisp;	// 
	IDispatch*	m_pDispatch;	// 
} STODCREATEINFO_OUT;
#define		SZSTODCREATEINFO_OUT		sizeof(STODCREATEINFO_OUT)

#define		INIT_STODCREATEINFO_OUT(pstOut)		pstOut->m_pWndObj = NULL;	\
			pstOut->m_pDispatch = NULL;


	// STODCREATEINFO_IN  : 
	// STODCREATEINFO_OUT : 
typedef HRESULT		(WINAPI * FPInit) (STODINIT_IN*, STODINIT_OUT*);
typedef HRESULT		(WINAPI * FPLoad) (STODCREATEINFO_IN*, STODCREATEINFO_OUT*);
typedef HRESULT		(WINAPI * FPUnLoad) (void* dwDllKey);
//typedef HRESULT		(WINAPI * FPGetState) (void* dwDllKey, LPCSTR szKey, long szState);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_OCXDLLSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
