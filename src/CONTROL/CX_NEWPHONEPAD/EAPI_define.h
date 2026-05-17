#pragma once
#pragma once
#include "StdAfx.h"
// ======================================================
// EAPI Message ID Define
// ======================================================

// Request
#define REQ_LOGIN                   1001
#define REQ_LOGOUT                  1002
#define REQ_ADDLOGIN                1003
#define REQ_MAKE_CALL               1011
#define REQ_ANSWER_CALL             1012
#define REQ_CALL_RELEASE            1013
#define REQ_HOLD_CALL               1014
#define REQ_RETRIEVE_CALL           1015
#define REQ_TRANSFER_CALL           1017
#define REQ_CONFERENCE_CALL         1018
#define REQ_CONSULTATION_CALL       1019
#define REQ_PHONEPAD                1020
#define REQ_GET_UUI                 1075
#define REQ_SET_UUI                 1076
#define REQ_REMOVEPARTY             1079

// Response
#define RES_LOGIN                   1101
#define RES_LOGOUT                  1102
#define RES_MAKE_CALL               1111
#define RES_ANSWER_CALL             1112
#define RES_CALL_RELEASE            1113
#define RES_HOLD_CALL               1114
#define RES_RETRIEVE_CALL           1115
#define RES_TRANSFER_CALL           1117
#define RES_CONFERENCE_CALL         1118
#define RES_CONSULTATION_CALL       1119
#define RES_PHONEPAD                1120
#define RES_GET_UCDATA              1175
#define RES_SET_UCDATA              1176
#define RES_REMOVEPARTY             1179

// Event
#define EVT_ESTABLISHED             1202
#define EVT_CALL_RELEASE            1203
#define EVT_HOLD_CALL               1204
#define EVT_RETRIEVE_CALL           1205
#define EVT_TRANSFERRED_CALL        1206
#define EVT_CONFERENCE_CALL         1207
#define EVT_DELIVERED               1208
#define EVT_OUTGOING                1209
#define EVT_OUT_OF_SERVICE          1304
#define EVT_IN_SERVICE              1305
#define EVT_LOGOUT                  1306
#define EVT_OFFERED                 1308
#define EVT_UUI_DATA                1313
#define EVT_UC_DATA                 1314
#define EVT_REMOVE_PARTY            1315
#define EVT_CONSULT_CALL            1316
#define EVT_CONSULT_INITIATED       1317


inline LPCTSTR GetEAPIMessageName(int nMsgID)
{
    switch (nMsgID)
    {
    case REQ_LOGIN:                 return _T("REQ_LOGIN");
    case REQ_LOGOUT:                return _T("REQ_LOGOUT");
    case REQ_ADDLOGIN:              return _T("REQ_ADDLOGIN");
    case REQ_MAKE_CALL:             return _T("REQ_MAKE_CALL");
    case REQ_ANSWER_CALL:           return _T("REQ_ANSWER_CALL");
    case REQ_CALL_RELEASE:          return _T("REQ_CALL_RELEASE");
    case REQ_HOLD_CALL:             return _T("REQ_HOLD_CALL");
    case REQ_RETRIEVE_CALL:         return _T("REQ_RETRIEVE_CALL");
    case REQ_TRANSFER_CALL:         return _T("REQ_TRANSFER_CALL");
    case REQ_CONFERENCE_CALL:       return _T("REQ_CONFERENCE_CALL");
    case REQ_CONSULTATION_CALL:     return _T("REQ_CONSULTATION_CALL");
    case REQ_PHONEPAD:              return _T("REQ_PHONEPAD");
    case REQ_GET_UUI:               return _T("REQ_GET_UUI");
    case REQ_SET_UUI:               return _T("REQ_SET_UUI");
    case REQ_REMOVEPARTY:           return _T("REQ_REMOVEPARTY");

    case RES_LOGIN:                 return _T("RES_LOGIN");
    case RES_LOGOUT:                return _T("RES_LOGOUT");
    case RES_MAKE_CALL:             return _T("RES_MAKE_CALL");
    case RES_ANSWER_CALL:           return _T("RES_ANSWER_CALL");
    case RES_CALL_RELEASE:          return _T("RES_CALL_RELEASE");
    case RES_HOLD_CALL:             return _T("RES_HOLD_CALL");
    case RES_RETRIEVE_CALL:         return _T("RES_RETRIEVE_CALL");
    case RES_TRANSFER_CALL:         return _T("RES_TRANSFER_CALL");
    case RES_CONFERENCE_CALL:       return _T("RES_CONFERENCE_CALL");
    case RES_CONSULTATION_CALL:     return _T("RES_CONSULTATION_CALL");
    case RES_PHONEPAD:              return _T("RES_PHONEPAD");
    case RES_GET_UCDATA:            return _T("RES_GET_UCDATA");
    case RES_SET_UCDATA:            return _T("RES_SET_UCDATA");
    case RES_REMOVEPARTY:           return _T("RES_REMOVEPARTY");

    case EVT_ESTABLISHED:           return _T("EVT_ESTABLISHED");
    case EVT_CALL_RELEASE:          return _T("EVT_CALL_RELEASE");
    case EVT_HOLD_CALL:             return _T("EVT_HOLD_CALL");
    case EVT_RETRIEVE_CALL:         return _T("EVT_RETRIEVE_CALL");
    case EVT_TRANSFERRED_CALL:      return _T("EVT_TRANSFERRED_CALL");
    case EVT_CONFERENCE_CALL:       return _T("EVT_CONFERENCE_CALL");
    case EVT_DELIVERED:             return _T("EVT_DELIVERED");
    case EVT_OUTGOING:              return _T("EVT_OUTGOING");
    case EVT_OUT_OF_SERVICE:        return _T("EVT_OUT_OF_SERVICE");
    case EVT_IN_SERVICE:            return _T("EVT_IN_SERVICE");
    case EVT_LOGOUT:                return _T("EVT_LOGOUT");
    case EVT_OFFERED:               return _T("EVT_OFFERED");
    case EVT_UUI_DATA:              return _T("EVT_UUI_DATA");
    case EVT_UC_DATA:               return _T("EVT_UC_DATA");
    case EVT_REMOVE_PARTY:          return _T("EVT_REMOVE_PARTY");
    case EVT_CONSULT_CALL:          return _T("EVT_CONSULT_CALL");
    case EVT_CONSULT_INITIATED:     return _T("EVT_CONSULT_INITIATED");
    }

    return _T("UNKNOWN");
}


inline LPCTSTR GetEAPIMessageDesc(int nMsgID)
{
    switch (nMsgID)
    {
    case REQ_LOGIN:                 return _T("로그인 요청");
    case REQ_LOGOUT:                return _T("로그아웃 요청");
    case REQ_ADDLOGIN:              return _T("추가 로그인 요청");
    case REQ_MAKE_CALL:             return _T("발신 요청");
    case REQ_ANSWER_CALL:           return _T("착신 응답 요청");
    case REQ_CALL_RELEASE:          return _T("통화 종료 요청");
    case REQ_HOLD_CALL:             return _T("보류 요청");
    case REQ_RETRIEVE_CALL:         return _T("보류 복원 요청");
    case REQ_TRANSFER_CALL:         return _T("통화 전환 요청");
    case REQ_CONFERENCE_CALL:       return _T("회의 통화 요청");
    case REQ_CONSULTATION_CALL:     return _T("상담 통화 요청");
    case REQ_PHONEPAD:              return _T("폰패드 요청");
    case REQ_GET_UUI:               return _T("UUI 데이터 조회 요청");
    case REQ_SET_UUI:               return _T("UUI 데이터 설정 요청");
    case REQ_REMOVEPARTY:           return _T("참가자 제거 요청");

    case RES_LOGIN:                 return _T("로그인 응답");
    case RES_LOGOUT:                return _T("로그아웃 응답");
    case RES_MAKE_CALL:             return _T("발신 응답");
    case RES_ANSWER_CALL:           return _T("수신 응답");
    case RES_CALL_RELEASE:          return _T("통화 종료 응답");
    case RES_HOLD_CALL:             return _T("보류 응답");
    case RES_RETRIEVE_CALL:         return _T("보류 복원 응답");
    case RES_TRANSFER_CALL:         return _T("통화 전환 응답");
    case RES_CONFERENCE_CALL:       return _T("회의 통화 응답");
    case RES_CONSULTATION_CALL:     return _T("상담 통화 응답");
    case RES_PHONEPAD:              return _T("폰패드 응답");
    case RES_GET_UCDATA:            return _T("UC 데이터 조회 응답");
    case RES_SET_UCDATA:            return _T("UC 데이터 설정 응답");
    case RES_REMOVEPARTY:           return _T("참가자 제거 응답");

    case EVT_ESTABLISHED:           return _T("통화 연결");
    case EVT_CALL_RELEASE:          return _T("통화 종료");
    case EVT_HOLD_CALL:             return _T("통화 보류");
    case EVT_RETRIEVE_CALL:         return _T("보류 복원");
    case EVT_TRANSFERRED_CALL:      return _T("통화 전환 완료");
    case EVT_CONFERENCE_CALL:       return _T("회의 통화 연결");
    case EVT_DELIVERED:             return _T("벨 울림");
    case EVT_OUTGOING:              return _T("발신 중");
    case EVT_OUT_OF_SERVICE:        return _T("디바이스 서비스 중단");
    case EVT_IN_SERVICE:            return _T("디바이스 서비스 시작");
    case EVT_LOGOUT:                return _T("인증 해제 이벤트");
    case EVT_OFFERED:               return _T("수화기 들림");
    case EVT_UUI_DATA:              return _T("UUI 데이터 이벤트");
    case EVT_UC_DATA:               return _T("UC 데이터 이벤트 / 폰패드 결과");
    case EVT_REMOVE_PARTY:          return _T("회의 참가자 제거");
    case EVT_CONSULT_CALL:          return _T("상담 통화 요청");
    case EVT_CONSULT_INITIATED:     return _T("상담 통화 시작");
    }

    return _T("알 수 없는 메시지");
}


inline LPCTSTR GetEAPIMessageType(int nMsgID)
{
    if (nMsgID >= 1000 && nMsgID < 1100)
        return _T("REQ");

    if (nMsgID >= 1100 && nMsgID < 1200)
        return _T("RES");

    if (nMsgID >= 1200 && nMsgID < 1400)
        return _T("EVT");

    return _T("UNKNOWN");
}


inline CString FormatEAPIMessage(int nMsgID)
{
    CString s;
    s.Format(_T("[%s] %d %s - %s"),
        GetEAPIMessageType(nMsgID),
        nMsgID,
        GetEAPIMessageName(nMsgID),
        GetEAPIMessageDesc(nMsgID));

    return s;
}