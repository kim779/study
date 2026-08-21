// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#define _AFXDLL
#pragma once

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
#include <afxmt.h>
#include <afxcontrolbars.h>
#include <AxStd.hpp>
#define ROW_COMMENT    '8'
#define ROW_BOOKMARK '9'
#define DF_BOOKFILE_ORI    "bookmark.i"
//#define DF_USEBOOKFILE

#define DF_SERVERMEMO
#define DF_RTS_CHECK

// 2026-08-06: RTS 실시간 처리 단순화 - AxStd::async 제거(직접호출) +
// CIntgrid 드로잉 홀드/배치 로직 제거(즉시 InvalidateRect). 서버가 데이터 흐름을
// 제어하므로 클라이언트는 데이터 누락 방지를 최우선으로 함. 원복: 이 줄만 주석 처리.
#define DF_NEW_REALPROCESS

// CheckRTSTimer()의 사용자 지정 주기(m_iTime)로 드로잉을 스로틀링.
// 틱마다 raw data[] 슬롯(0~998)을 있는 그대로 깊은복사해서 종목코드 키로 캐시해두고,
// 타이머(TM_RTSTIME)가 울릴 때 한번에 flush. 어떤 슬롯이 어떤 필드인지(w/FwdMap 매핑)는
// 캐시 쪽에서 전혀 판단하지 않고, flush 시 기존 parsingAlertx()를 그대로 재사용해서
// 그 해석을 100% 맡긴다(파싱로직 복제 없음). m_iTime==0(실시간 설정)이면 캐시를 거치지
// 않고 기존 즉시처리 경로를 그대로 탄다. 원복: 이 줄만 주석 처리.
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
// 기존 beginTime/endTime 클라이언트 시각 비교 방식 대신 이 세션ID로 동시호가/단일가 여부를 판단한다.
// 2026-08-19: 기능 잠정 중단(의도적 비활성화, IB202700과 동일). 재개: 주석 해제.
//#define DF_SESSION_EXPECT

inline void Output_DebugString(CString slog)
{
	OutputDebugString(slog);
}

////삭제 존재여부는 부분요청, 부분응답
//typedef struct st_mid_SDEmemo
//{
//	char chgubn[1]{}; //구분: S-조회 . I-등록, U-수정, D-삭제
//	char chusid[16]{}; //user ID
//	char chcode[16]{}; //종목코드 
//}mid_SDEmemo;
//
////삭제 존재여부는 부분요청, 부분응답
//typedef struct st_mod_SDEmemo
//{
//	char gubn[1]; //요청한 구분값
//	char chcode[16]{};
//	char chretc[1]{}; //성공여부: 1성공(조회, 등록,수정,삭제) . 0실패 (조회/등록/수정/삭제 데이터 없음/실패 등)
//	char chemsg[64]{}; //결과메시지
//}mod_SDEmemo;
//
//
//
//constexpr int memo_max = 15 * 1024;
//
//typedef struct st_mid_memo
//{
//public:
//	st_mid_memo() = default;
//	st_mid_memo(const char type, const CString& userID, const CString& code)
//	{
//		ZeroMemory(this, sizeof(st_mid_memo));
//		chgubn[0] = type;
//		FillMemory(chusid, sizeof(chusid), ' ');
//		AxStd::xxCopy(chusid, userID);
//		FillMemory(chcode, sizeof(chcode), ' ');
//		AxStd::xxCopy(chcode, code);
//	}
//
//	st_mid_memo(const char type, const CString& userID, const CString& code, const CString& memo)
//	{
//		ZeroMemory(this, sizeof(st_mid_memo));
//		chgubn[0] = type;
//		FillMemory(chusid, sizeof(chusid), ' ');
//		AxStd::xxCopy(chusid, userID);
//		FillMemory(chcode, sizeof(chcode), ' ');
//		AxStd::xxCopy(chcode, code);
//		if (!memo.IsEmpty() && memo.GetLength() > 0)
//			AxStd::xxCopy(chmemo, memo);
//	}
//
//	char chgubn[1]{}; //구분: S-조회 . I-등록, U-수정, D-삭제
//	char chusid[16]{}; //user ID
//	char chcode[16]{}; //종목코드 
//	char chmemo[15 * 1024]{}; //종목메모
//}mid_memo;
//
//typedef struct st_mod_memo
//{
//	char gubn[1]{}; //요청한 구분값
//	char chcode[16]{};
//	char chretc[1]{}; //성공여부: 1성공(조회, 등록,수정,삭제) . 0실패 (조회/등록/수정/삭제 데이터 없음/실패 등)
//	char chemsg[64]{}; //결과메시지
//	char chmemo[15 * 1024]{}; //종목메모
//	char chMemoDate[12]{};    //메모업로드 날자
//}mod_memo;