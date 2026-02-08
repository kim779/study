// npenkAppInstall5WIN.h : 헤더 파일
//

#pragma once


class npenkAppInstall5WIN
{
public:
	// 에러코드
	static const DWORD eErr_True;	// 정상, 성공, TRUE
	static const DWORD eErr_False;	// 오류, 실패, FALSE
	static const DWORD eErr_NoFunc;	// DLL 내부에 함수가 없음
	static const DWORD eErr_FailLoadLibrary;	// DLL 로드 실패
	static const DWORD eErr_TimeOver;	// 시간 초과
	static const DWORD eErr_NoDoc;	// XML 파일 읽기 실패.
	static const DWORD eErr_NoNode;	// XML 파일 내용 중 NODE 읽기 실패.
	static const DWORD eErr_FailAuthFileName;	// 파일 인증 실패. ( 파일이 손상 되었거나 필요한 파일이 존재 하지 않습니다. )


private:
#ifdef _UNICODE 
	typedef DWORD (WINAPI *fnStart)( WCHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnStartXGuard)( WCHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnShutdownXGuard)( WCHAR *szCustomID );
#else
	typedef DWORD (WINAPI *fnStart)( CHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnStartXGuard)( CHAR *szCustomID, DWORD dwWaitTime );
	typedef DWORD (WINAPI *fnShutdownXGuard)( CHAR *szCustomID );
#endif
	typedef DWORD (WINAPI *fnKillMain)();
	typedef DWORD (WINAPI *fnIsMainRunning)();
	typedef DWORD (WINAPI *fnIsXGuardRunning)();
	typedef DWORD (WINAPI *fnUninit)();

public:
	npenkAppInstall5WIN();
	~npenkAppInstall5WIN();

	/**
	* \brief		DLL 경로 설정
	* \author		leesangwoo
	* \date			2011.02.23
	* \param		strDllPath	(AppInstall5.dll의 위치)
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			setDllPath( const TCHAR *szDllPath );

	/**
	* \brief		메인 실행하기
	* \author		kimdaehee
	* \date			2010.06.18
	* \param		strCustomID	업체아이디(정책파일명)
	* \param		dwWaitTime	(최대) 응답 대기 시간
	*							-1		: 무한 대기(sync)
	*							0		: 대기하지 않음(async)
	*							1 이상	: 최대 dwWaitTime millisecond 만큼 기다림
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			start( TCHAR *szCustomID, DWORD dwWaitTime );

	/**
	* \brief		메인 종료하기(종료 조건에서 caller 만 제외시킴)
	* \author		kimdaehee
	* \date			2010.06.18
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			killMain();

	/**
	* \brief		메인이 실행중인가?
	* \author		kimdaehee
	* \date			2010.06.18
	* \return		eErr_True(실행중), 그 외(실행중이지 않음)
	*/
	DWORD			isMainRunning();

	/**
	* \brief		XGuard 실행
	* \author		leesangwoo
	* \date			2010.10.21
	* \param		strCustomID	업체아이디(정책파일명)
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			startXGuard( TCHAR *szCustomID, DWORD dwWaitTime );

	/**
	* \brief		XGuard 종료
	* \author		leesangwoo
	* \date			2010.10.21
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			shutdownXGuard( TCHAR *szCustomID );

	/**
	* \brief		XGuard 실행중인가?
	* \author		leesangwoo
	* \date			2011.02.21
	* \return		eErr_True(성공), 그 외(실패)
	*/
	DWORD			isXGuardRunning();

	/**
	* \brief		AppInstaller 기능 중단
	* \author		kiminsu
	* \date			2011.04.22
	* \return		eErr_True(정상)
	*/
	DWORD			uninit();


	DWORD			uninitDll();
private:
	DWORD			initDll();
	

	HINSTANCE			hDll_;
	TCHAR				szDllPath_[255];
	fnStart				fnStart_;
	fnKillMain			fnKillMain_;
	fnIsMainRunning		fnIsMainRunning_;
	fnStartXGuard		fnStartXGuard_;
	fnShutdownXGuard	fnShutdownXGuard_;
	fnIsXGuardRunning	fnIsXGuardRunning_;
	fnUninit			fnUninit_;
};
