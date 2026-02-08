// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_)
#define AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
///////추가 헤더 파일 : 프리 헤더 파일 ////
//Skin And Control
#include "Control/XTabCtrl.h"
//Base
//#include "../../COMMON_BASE/BaseInfo/BaseInfo.h"
// {{ 메인쪽 라이브러리 링크 ( 확장 dll )  }}
//#include "../../../DataMgr/DBMgr.h"

#include "../../inc/EnvDir.h"

#include "../Common_ST/STConfigDef.h"//헤더파일 추가
#include "../Common_ST/StrategyLoader.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/ISDataItem.h"

#include "../../inc/ExCommonHeader.h"
//////////
#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExPlatform.h"	// 플랫폼 관련 부분 모음.

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"
#include "../../inc/ICompanyMng.h"	// 사이트의 정보를 정의한 파일.
#include "../Common_ST/EscDialog.h"	//Esc눌렸을 때 화면닫기 : KHD:2006.11.17

#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__51E1CBAA_8DBD_4588_99AA_6169A2A5B9D5__INCLUDED_)
