#pragma once

// SDKDDKVer.h를 포함하면 최고 수준의 가용성을 가진 Windows 플랫폼이 정의됩니다.

// 이전 Windows 플랫폼에 대해 응용 프로그램을 빌드하려는 경우에는 SDKDDKVer.h를 포함하기 전에
// WinSDKVer.h를 포함하고 _WIN32_WINNT 매크로를 지원하려는 플랫폼으로 설정하십시오.

#include <SDKDDKVer.h>

#include <afxtempl.h>   // MFC Template support
#include <afxmt.h>      // MFC Multi-threading support
#include <afxole.h>     // MFC OLE support
#include <shlobj.h>     // IShellFolder support
#include <lm.h>         // WNet support

#include "htmlhelp.h"


#include "axStd.h"
#define AFX_ZERO_INIT_OBJECT(base_class) memset(((base_class*)this)+1, 0, sizeof(*this) - sizeof(class base_class));

