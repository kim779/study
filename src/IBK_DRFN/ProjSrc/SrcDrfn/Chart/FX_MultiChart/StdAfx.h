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

//@100121-alzoyes dsp 링크옵션에서 StaAfx.h로 루틴가져옴.
#define __MULTI_LANG__
#define _MT_MANUAL_LOAD

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExPlatform.h"	// 플랫폼 관련 부분 모음.
#include <afxdisp.h>

#include "../../inc/IMasterDataMng.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IDebugMng.h"
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IAUGwanMng.h"

#include "../../inc/MasterDataSub.h"

#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

extern IDebugManager*	g_pDebugMng;
extern IPartnerUtilManager* g_pPtUtilMng;

//@유진추가
// Market Type (주식/업종/선옵/주식선옵/해외)
//<<20100310_JS.Kim  inc/MasterDataSub.h 에 있는 값 가져옴...
enum { STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, STOCKFUTOPT_CHART, FOREIGN_CHART, FUOPT_JIPYO_CHART, COMMODITY_CHART, MARKET_CHART, FO_FOREIGN_CHART, FX_CHART };
//>>
// 기본 / 사용자선택
enum { MODE_DEFAULT, MODE_USER };
// 종목변경 / 차트추가 / 종목추가 / 종목중첩 / 복기
enum { CHANGE_ITEM, NEW_CHART, NEW_ITEM, OVERLAPP_ITEM, REPLAY_ITEM };
//@유진추가

#define	CLR_FONT_DEFAULT			RGB(0,0,1)

#define nScreenID_6204		369		//0369
#define nScreenID_6205		370		//0370
#define gSTOCKFUTOPT_0503	503		//0503
#define SCREENID_FOREIGN	504		//0503
#define nScreenID_6301		506		//0506
#define nScreenID_6401		508		//0508
#define gFUTOPTJIPYO_0515	515		//0515
#define gSTOCKFUTOPT_0516	516		//0516
#define nScreenID_9401		9401	//9401		// FX

#define		theStdParent		((CStdDialog*)GetParent())

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
