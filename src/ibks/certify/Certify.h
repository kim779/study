#pragma once

// Certify.h: Certify.DLL의 주 헤더 파일

#if !defined( __AFXCTL_H__ )
#error "이 파일을 포함하기 전에 'afxctl.h'를 포함하십시오."
#endif

#include "resource.h"       // 주 기호입니다.

struct	_pwdR {
	char	ret[1];		// return code
				// '0' : success
				// 'X' : stop key in
	char	msg[80];	// message
	char	pwdn[1];	// invalid passwd count
};

#define	checkPass	0
#define	rebootAxis	1
#define	encryptPass	2

// CCertifyApp: 구현은 Certify.cpp를 참조하세요.

class CCertifyApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

