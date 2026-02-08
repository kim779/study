// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
#define AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_

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

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <list>
#include <map>		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
#include <string>	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO

#include <AfxTempl.h>
#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExPlatform.h"	// 플랫폼 관련 부분 모음.
#include <afxdisp.h>

#include "../../inc/IMasterDataMng.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IAUPartnerMng.h"

#include "../../inc/MasterDataSub.h"

#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

extern IPartnerUtilManager* g_pPtUtilMng;

// 기본 / 사용자선택
enum { MODE_DEFAULT, MODE_USER };
// 종목변경 / 차트추가 / 종목추가 / 종목중첩 / 복기
enum { CHANGE_ITEM, NEW_CHART, NEW_ITEM, OVERLAPP_ITEM, REPLAY_ITEM };
//@유진추가

// Period Type (틱/초/분/시/일/주/월)
//enum { TICK_DATA_CHART, SEC_DATA_CHART, MIN_DATA_CHART, HOUR_DATA_CHART, DAY_DATA_CHART, WEEK_DATA_CHART, MONTH_DATA_CHART };

#define		__MULTI_LANG__
#define		_MT_MANUAL_LOAD
#define		_GNF_PROJECT_
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
