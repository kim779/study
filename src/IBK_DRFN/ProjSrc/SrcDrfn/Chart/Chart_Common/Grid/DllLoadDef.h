#if !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/DllLoadDef.h"

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
} StDllLoadData, *LPStDllLoadData;


typedef CDialog*	(WINAPI * FpDllLoad) (CView*);
typedef void		(WINAPI * FpDllUnload) (CDialog*);
typedef void		(WINAPI * FpSetData) (CDialog*, void*);
typedef void		(WINAPI * FpFormLoad) (CDialog*);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)

/*#if !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/DllLoadDef.h"

// * DLL 화면 로드시 필요한 데이타 구조체 
typedef struct {
	HWND		m_hSocket;				// 소켓 핸들
	HWND		m_hParent;				// 부모 핸들
	CString		m_strRootPath;			// 루트 경로	
	CString		m_strUserID;				// 유저 id	
	CString		m_strMapID;				// 화면 번호
	CString		m_strEnvFilePath;		// 환경파일 경로
	long		m_dwInterface;			// 공유 관리자 포인터
	long		m_dwAcctManager;		// 계좌 관리자 포인터 
	CString		m_strExtra1;				// 임시
	CString		m_strExtra2;				// 임시
	long		m_lExtra1;				// 임시
	long		m_lExtra2;				// 임시 
} StDllLoadData, *LPStDllLoadData;


typedef CDialog*	(WINAPI * FpDllLoad) (CWnd*, LPCTSTR);
typedef void		(WINAPI * FpDllUnload) (CDialog*);
typedef void		(WINAPI * FpSetData) (CDialog*, void*);
typedef void		(WINAPI * FpFormLoad) (CDialog*);

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
*/