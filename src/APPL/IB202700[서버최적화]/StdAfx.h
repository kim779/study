// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define _AFXDLL
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
#include <afxmt.h>
#include <afxcontrolbars.h>
#include <AxStd.hpp>

#define ROW_COMMENT '8'   //test mod
#define ROW_BOOKMARK '9'

#define DF_NEWTREE
#define DF_SERVERMEMO

// 2026-08-06: RTS 실시간 처리 단순화 - AxStd::async 제거(직접호출) +
// CIntgrid 드로잉 홀드/배치 로직 제거(즉시 InvalidateRect). 서버가 데이터 흐름을
// 제어하므로 클라이언트는 데이터 누락 방지를 최우선으로 함. 원복: 이 줄만 주석 처리.
#define DF_NEW_REALPROCESS

// CheckRTSTimer()의 사용자 지정 주기(m_iTime)로 드로잉을 스로틀링.
// 틱마다 raw data[] 슬롯(0~998)을 있는 그대로 깊은복사해서 종목코드 키로 캐시해두고,
// 타이머(TM_RTSTIME)가 울릴 때 한번에 flush. 어떤 슬롯이 어떤 필드인지(w/FwdMap 매핑)는
// 캐시 쪽에서 전혀 판단하지 않고, flush 시 기존 parsingAlertx()를 그대로 재사용해서
// 그 해석을 100% 맡긴다(파싱로직 복제 없음).
// m_iTime==0(실시간 설정)이면 캐시를 거치지 않고 기존 즉시처리 경로를 그대로 탄다.
// 원복: 이 줄만 주석 처리.
#define DF_RTS_TIMER

#ifdef DF_RTS_TIMER
struct _Ralert {
	std::string code;
	int stat{};
	int size{};
	std::unique_ptr<char[]> ptr[999]{};
};
#endif

// 서버가 실시간 데이터에 field 298(KRX 장운영세션)/299(NXT 장운영세션)을 추가로 실어줌(2026-08-10 시안).
// 기존 beginTime/endTime 클라이언트 시각 비교 방식(서버 응답 실패시 항상 0으로 남는 문제 있었음) 대신
// 이 세션ID로 동시호가/단일가 여부를 판단한다. 원복: 이 줄만 주석 처리.
#define DF_SESSION_EXPECT1

inline void Output_DebugString(CString slog)
{
	OutputDebugString(slog);
}

#define DF_SORT