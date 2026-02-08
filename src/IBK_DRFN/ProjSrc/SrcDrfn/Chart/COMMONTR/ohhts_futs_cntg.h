#if !defined(AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_)
#define AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 선물종목체결 (  )
#pragma pack( push, 1 )

#define NAME_ohhts_futs_cntg     "ohhts_futs_cntg"

// 
typedef struct _ohhts_futs_cntg_InBlock1
{
    char    FUTS_SHRN_ISCD      [   8];    // [string,    8] 선물 단축 종목코드
} ohhts_futs_cntg_InBlock1, *LPohhts_futs_cntg_InBlock1;
#define NAME_ohhts_futs_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_futs_cntg_OutBlock1
{
    char    FUTS_SHRN_ISCD      [   8];    // [string,    8] 선물 단축 종목코드
    char    CNTG_CLS_CODE       [   1];    // [string,    1] 체결 구분 코드
    char    FUTS_SRNO           [  11];    // [numstring,   11] 선물 일련번호
    char    CNTG_HOUR           [   6];    // [string,    6] 체결 시간
    char    FUTS_PRPR           [  12];    // [numstring, 12.2] 선물 현재가
    char    FUTS_PRDY_VRSS      [  12];    // [numstring, 12.2] 선물 전일 대비
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] 전일 대비 부호
    char    FUTS_PRDY_CTRT      [   9];    // [numstring,  9.2] 선물 전일 대비율
    char    FUTS_ASKP1          [  12];    // [numstring, 12.2] 선물 매도호가1
    char    FUTS_BIDP1          [  12];    // [numstring, 12.2] 선물 매수호가1
    char    FUTS_HGPR           [  12];    // [numstring, 12.2] 선물 최고가
    char    FUTS_LWPR           [  12];    // [numstring, 12.2] 선물 최저가
    char    FUTS_OPRC           [  12];    // [numstring, 12.2] 선물 시가2
    char    CNQN                [  19];    // [numstring,   19] 체결량
    char    ACML_VOL            [  19];    // [numstring,   19] 누적 거래량
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] 누적 거래 대금
    char    TOTAL_SELN_QTY      [  19];    // [numstring,   19] 총 매도 수량
    char    TOTAL_SHNU_QTY      [  19];    // [numstring,   19] 총 매수2 수량
    char    OTST_STPL_QTY       [  13];    // [numstring,   13] 미결제 약정 수량
    char    OTST_STPL_QTY_ICDC  [  11];    // [numstring,   11] 미결제 약정 수량 증감
    char    OTST_STPL_RGBF_QTY_ICDC[  11];    // [numstring,   11] 미결제 약정 직전 수량 증감
    char    BASIS               [   9];    // [numstring,  9.4] 베이시스
    char    FUTS_THPR           [  12];    // [numstring, 12.2] 선물 이론가
    char    DPRT                [   9];    // [numstring,  9.2] 괴리율
    char    SELN_CNTG_CSNU      [  11];    // [numstring,   11] 매도 체결 건수
    char    SHNU_CNTG_CSNU      [  11];    // [numstring,   11] 매수2 체결 건수
    char    NTBY_CNTG_CSNU      [  11];    // [numstring,   11] 순매수 체결 건수
    char    CTTR                [  12];    // [numstring, 12.2] 체결강도
} ohhts_futs_cntg_OutBlock1, *LPohhts_futs_cntg_OutBlock1;
#define NAME_ohhts_futs_cntg_OutBlock1     "OutBlock1"

class CREAL_FUTURE
{
public: //Variable
    CString m_szFUTS_SHRN_ISCD      ;    // [string,    8] 선물 단축 종목코드
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] 체결 구분 코드
    CString m_szFUTS_SRNO           ;    // [numstring,   11] 선물 일련번호
    CString m_szCNTG_HOUR           ;    // [string,    6] 체결 시간
    CString m_szFUTS_PRPR           ;    // [numstring, 12.2] 선물 현재가
    CString m_szFUTS_PRDY_VRSS      ;    // [numstring, 12.2] 선물 전일 대비
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] 전일 대비 부호
    CString m_szFUTS_PRDY_CTRT      ;    // [numstring,  9.2] 선물 전일 대비율
    CString m_szFUTS_ASKP1          ;    // [numstring, 12.2] 선물 매도호가1
    CString m_szFUTS_BIDP1          ;    // [numstring, 12.2] 선물 매수호가1
    CString m_szFUTS_HGPR           ;    // [numstring, 12.2] 선물 최고가
    CString m_szFUTS_LWPR           ;    // [numstring, 12.2] 선물 최저가
    CString m_szFUTS_OPRC           ;    // [numstring, 12.2] 선물 시가2
    CString m_szCNQN                ;    // [numstring,   19] 체결량
    CString m_szACML_VOL            ;    // [numstring,   19] 누적 거래량
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] 누적 거래 대금
    CString m_szTOTAL_SELN_QTY      ;    // [numstring,   19] 총 매도 수량
    CString m_szTOTAL_SHNU_QTY      ;    // [numstring,   19] 총 매수2 수량
    CString m_szOTST_STPL_QTY       ;    // [numstring,   13] 미결제 약정 수량
    CString m_szOTST_STPL_QTY_ICDC  ;    // [numstring,   11] 미결제 약정 수량 증감
    CString m_szOTST_STPL_RGBF_QTY_ICDC;    // [numstring,   11] 미결제 약정 직전 수량 증감
    CString m_szBASIS               ;    // [numstring,  9.4] 베이시스
    CString m_szFUTS_THPR           ;    // [numstring, 12.2] 선물 이론가
    CString m_szDPRT                ;    // [numstring,  9.2] 괴리율
    CString m_szSELN_CNTG_CSNU      ;    // [numstring,   11] 매도 체결 건수
    CString m_szSHNU_CNTG_CSNU      ;    // [numstring,   11] 매수2 체결 건수
    CString m_szNTBY_CNTG_CSNU      ;    // [numstring,   11] 순매수 체결 건수
    CString m_szCTTR                ;    // [numstring, 12.2] 체결강도

public:
	CREAL_FUTURE()
	{
	}

	virtual void  Convert(ohhts_futs_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, FUTS_SHRN_ISCD      );    // [string,    8] 선물 단축 종목코드
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] 체결 구분 코드
		DATA_MEMCOPY(sTmp, pInput, FUTS_SRNO           );    // [numstring,   11] 선물 일련번호
		DATA_MEMCOPY(sTmp, pInput, CNTG_HOUR           );    // [string,    6] 체결 시간
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRPR           );    // [numstring, 12.2] 선물 현재가
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRDY_VRSS      );    // [numstring, 12.2] 선물 전일 대비
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] 전일 대비 부호
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRDY_CTRT      );    // [numstring,  9.2] 선물 전일 대비율
		DATA_MEMCOPY(sTmp, pInput, FUTS_ASKP1          );    // [numstring, 12.2] 선물 매도호가1
		DATA_MEMCOPY(sTmp, pInput, FUTS_BIDP1          );    // [numstring, 12.2] 선물 매수호가1
		DATA_MEMCOPY(sTmp, pInput, FUTS_HGPR           );    // [numstring, 12.2] 선물 최고가
		DATA_MEMCOPY(sTmp, pInput, FUTS_LWPR           );    // [numstring, 12.2] 선물 최저가
		DATA_MEMCOPY(sTmp, pInput, FUTS_OPRC           );    // [numstring, 12.2] 선물 시가2
		DATA_MEMCOPY(sTmp, pInput, CNQN                );    // [numstring,   19] 체결량
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] 누적 거래량
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] 누적 거래 대금
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SELN_QTY      );    // [numstring,   19] 총 매도 수량
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SHNU_QTY      );    // [numstring,   19] 총 매수2 수량
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY       );    // [numstring,   13] 미결제 약정 수량
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY_ICDC  );    // [numstring,   11] 미결제 약정 수량 증감
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_RGBF_QTY_ICDC);    // [numstring,   11] 미결제 약정 직전 수량 증감
		DATA_MEMCOPY(sTmp, pInput, BASIS               );    // [numstring,  9.4] 베이시스
		DATA_MEMCOPY(sTmp, pInput, FUTS_THPR           );    // [numstring, 12.2] 선물 이론가
		DATA_MEMCOPY(sTmp, pInput, DPRT                );    // [numstring,  9.2] 괴리율
		DATA_MEMCOPY(sTmp, pInput, SELN_CNTG_CSNU      );    // [numstring,   11] 매도 체결 건수
		DATA_MEMCOPY(sTmp, pInput, SHNU_CNTG_CSNU      );    // [numstring,   11] 매수2 체결 건수
		DATA_MEMCOPY(sTmp, pInput, NTBY_CNTG_CSNU      );    // [numstring,   11] 순매수 체결 건수
		DATA_MEMCOPY(sTmp, pInput, CTTR                );    // [numstring, 12.2] 체결강도
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
