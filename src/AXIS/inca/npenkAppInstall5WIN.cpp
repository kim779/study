// npenkAppInstall5WIN.cpp : 구현 파일
//

#include "stdafx.h"
#include "npenkAppInstall5WIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable : 4996)

namespace 
{
#ifdef _UNICODE 
	const char START_FN_NAME[]				= "startW";
	const char START_XGUARD_FN_NAME[]		= "startXGuardW";
	const char SHUTDOWN_XGUARD_FN_NAME[]	= "shutdownXGuardW";
#else
	const char START_FN_NAME[]				= "startA";
	const char START_XGUARD_FN_NAME[]		= "startXGuardA";
	const char SHUTDOWN_XGUARD_FN_NAME[]	= "shutdownXGuardA";
#endif

	const char KILL_MAIN_FN_NAME[]			= "killMain";
	const char IS_MAIN_RUNNING_FN_NAME[]	= "isMainRunning";
	const char IS_XGUARD_RUNNING_FN_NAME[]	= "isXGuardRunning";
	const char UNINIT_APP_INSTALLER_FN_NAME[]		= "uninit";

	const TCHAR NPENK_APP_INSTALL5[]		= _T("npenkAppInstall5.dll");
}

// 에러코드
const DWORD npenkAppInstall5WIN::eErr_True				= 0x00000000;	// 정상, 성공, TRUE
const DWORD npenkAppInstall5WIN::eErr_False				= 0x00000001;	// 오류, 실패, FALSE
const DWORD npenkAppInstall5WIN::eErr_NoFunc				= 0x21000003;	// DLL 내부에 함수가 없음
const DWORD npenkAppInstall5WIN::eErr_FailLoadLibrary	= 0x21000004;	// DLL 로드 실패
const DWORD npenkAppInstall5WIN::eErr_TimeOver			= 0x2100000F;	// 시간 초과
const DWORD npenkAppInstall5WIN::eErr_NoDoc				= 0x21000101;	// XML 파일 읽기 실패.
const DWORD npenkAppInstall5WIN::eErr_NoNode				= 0x21000103;	// XML 파일 내용 중 NODE 읽기 실패.
const DWORD npenkAppInstall5WIN::eErr_FailAuthFileName	= 0x2100030D;	// 파일 인증 실패. ( 파일이 손상 되었거나 필요한 파일이 존재 하지 않습니다. )

npenkAppInstall5WIN::npenkAppInstall5WIN()
: hDll_( NULL )
, fnStart_( NULL )
, fnKillMain_( NULL )
, fnIsMainRunning_( NULL )
, fnStartXGuard_( NULL )
, fnShutdownXGuard_( NULL )
{
	memset( szDllPath_, NULL, sizeof( szDllPath_ ) );
}

npenkAppInstall5WIN::~npenkAppInstall5WIN()
{
	uninitDll();
}

DWORD npenkAppInstall5WIN::initDll()
{
	if( NULL == hDll_ )
	{
		if( 0 == _tcslen( szDllPath_ ) )
		{
			return eErr_FailLoadLibrary;
		}

		const TCHAR PathSeparator( '\\' );
		TCHAR szDllPath[MAX_PATH];
		memset( szDllPath, NULL, MAX_PATH );
		_tcscat( szDllPath, szDllPath_ );
		size_t Len = _tcslen( szDllPath );
		if( PathSeparator != szDllPath[Len] )
		{
			szDllPath[Len] = PathSeparator;
			++Len;
			szDllPath[Len] = NULL;
		}
		_tcscat( szDllPath, NPENK_APP_INSTALL5 );

		hDll_ = ::LoadLibrary( szDllPath );

		if( NULL == hDll_ )
		{
			return eErr_FailLoadLibrary;
		}

		fnStart_			= ( fnStart )::GetProcAddress( hDll_, START_FN_NAME );
		fnKillMain_			= ( fnKillMain )::GetProcAddress( hDll_, KILL_MAIN_FN_NAME );
		fnIsMainRunning_	= ( fnIsMainRunning )::GetProcAddress( hDll_, IS_MAIN_RUNNING_FN_NAME );
		fnStartXGuard_		= ( fnStartXGuard )::GetProcAddress( hDll_, START_XGUARD_FN_NAME );
		fnShutdownXGuard_	= ( fnShutdownXGuard )::GetProcAddress( hDll_, SHUTDOWN_XGUARD_FN_NAME );
		fnIsXGuardRunning_	= ( fnIsXGuardRunning )::GetProcAddress( hDll_, IS_XGUARD_RUNNING_FN_NAME );
		fnUninit_			= ( fnUninit )::GetProcAddress( hDll_, UNINIT_APP_INSTALLER_FN_NAME );

		if( NULL == fnStart_ ||
			NULL == fnKillMain_ ||
			NULL == fnIsMainRunning_ ||
			NULL == fnStartXGuard_ ||
			NULL == fnShutdownXGuard_ ||
			NULL == fnIsXGuardRunning_ || 
			NULL == fnUninit_ )
		{
			return eErr_NoFunc;
		}
	}

	return eErr_True;
}

DWORD npenkAppInstall5WIN::uninitDll()
{
	if( NULL != hDll_ )
	{
		FreeLibrary( hDll_ );
		hDll_ = NULL;
	}

	return eErr_True;
}

DWORD npenkAppInstall5WIN::setDllPath( const TCHAR *szDllPath )
{
	if( 0 == _tcscpy( szDllPath_, szDllPath ) )
	{
		return eErr_True;
	}
	
	return eErr_False;
}

DWORD npenkAppInstall5WIN::start( TCHAR *szCustomID, DWORD dwWaitTime )
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnStart_( szCustomID, dwWaitTime );
}

DWORD npenkAppInstall5WIN::killMain()
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnKillMain_();
}

DWORD npenkAppInstall5WIN::isMainRunning()
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnIsMainRunning_();
}

DWORD npenkAppInstall5WIN::startXGuard( TCHAR *szCustomID, DWORD dwWaitTime )
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnStartXGuard_( szCustomID, dwWaitTime );
}

DWORD npenkAppInstall5WIN::shutdownXGuard( TCHAR *szCustomID )
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnShutdownXGuard_( szCustomID );
}

DWORD npenkAppInstall5WIN::isXGuardRunning()
{
	const DWORD dwRet = initDll();
	if( eErr_True != dwRet )
		return dwRet;

	return fnIsXGuardRunning_();
}

DWORD npenkAppInstall5WIN::uninit()
{
	if ( NULL != hDll_ )
	{
		fnUninit_();

	}
	return uninitDll();
}