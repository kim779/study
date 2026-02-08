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
#include "../../inc/commonDefines.h" // 0024696: 파생상품 코드체계 변경 제도개선
#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

#include "LogMan.h"

extern IDebugManager*	g_pDebugMng;
extern IPartnerUtilManager* g_pPtUtilMng;
extern CLogMan g_logMan;

//@유진추가
// Market Type (주식/업종/선옵/주식선옵/해외)

//-->@inc\MasterDataSub.h 에 동일한 순서로 정의됨.
//enum { STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, STOCKFUTOPT_CHART, FOREIGN_CHART };
//enum { MARKET_UNKNOWN = -1, MARKET_KOSPI_STOCK, MARKET_KOSDAQ_STOCK, MARKET_ELW, MARKET_KOSPI_UPJONG, MARKET_KOSDAQ_UPJONG, MARKET_KFUTURE, MARKET_KOPTION, MARKET_FUTURE_SPREAD, MARKET_STOCKFUT, MARKET_STOCKOPT, MARKET_US_ITEM};
//<--@inc\MasterDataSub.h 에 동일한 순서로 정의됨.

// 기본 / 사용자선택
enum { MODE_DEFAULT, MODE_USER };
// 종목변경 / 차트추가 / 종목추가 / 종목중첩 / 복기
enum { CHANGE_ITEM, NEW_CHART, NEW_ITEM, OVERLAPP_ITEM, REPLAY_ITEM };
//@유진추가

// Period Type (틱/초/분/시/일/주/월)
//enum { TICK_DATA_CHART, SEC_DATA_CHART, MIN_DATA_CHART, HOUR_DATA_CHART, DAY_DATA_CHART, WEEK_DATA_CHART, MONTH_DATA_CHART };

#define nScreenID_6204		7119		//0369 :미니틱차트
#define nScreenID_6205		7120		//0370 :미니업종차트
#define gSTOCKFUTOPT_0503	7114		//선옵차트
#define SCREENID_FOREIGN	7116		//해외증시
#define nScreenID_6301		7125		//일주월 
#define nScreenID_6401		508		//0508
#define gFUTOPTJIPYO_0515	515		//0515
#define gSTOCKFUTOPT_0516	516		//0516

#define		theStdParent		((CStdDialog*)GetParent())

extern CString gszDebug;

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
//20110502 이문수 >>
int		GetCommonDataInfo(HWND hWnd, int nDataIndex, int nSubInfo);
int		SetCommonDataInfo(HWND hWnd, int nDataIndex, int nData);
CString GetTrCodeFromCode(HWND hWnd, CString strCode);
//20110502 이문수 <<

const UINT RMSG_GET_STDDLG_PTR = ::RegisterWindowMessage("RMSG_GET_STDDLG_PTR");//20220117 이문수 >>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__69714C6C_A7C1_4698_B590_3B9B0ACDEBA3__INCLUDED_)
