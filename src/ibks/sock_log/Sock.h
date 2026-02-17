#pragma once

// Sock.h: Sock.DLL의 주 헤더 파일

#if !defined( __AFXCTL_H__ )
#error "이 파일을 포함하기 전에 'afxctl.h'를 포함하십시오."
#endif

#include "resource.h"       // 주 기호입니다.


// CSockApp: 구현은 Sock.cpp를 참조하세요.

class CSockApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

