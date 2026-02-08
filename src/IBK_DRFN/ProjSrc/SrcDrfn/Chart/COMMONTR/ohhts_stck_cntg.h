#if !defined(AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_)
#define AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식체결 자동갱신 (  )
#pragma pack( push, 1 )

#define NAME_ohhts_stck_cntg     "ohhts_stck_cntg"

// 
typedef struct _ohhts_stck_cntg_InBlock1
{
    char    MKSC_SHRN_ISCD      [   6];    // [string,    6] 유가증권 단축 종목코드
} ohhts_stck_cntg_InBlock1, *LPohhts_stck_cntg_InBlock1;
#define NAME_ohhts_stck_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_stck_cntg_OutBlock1
{
    char    MKSC_SHRN_ISCD      [   6];    // [string,    6] 유가증권 단축 종목코드
    char    TR_SUSP_CLS_CODE    [   2];    // [string,    2] 거래 정지 구분 코드
    char    CNTG_CLS_CODE       [   1];    // [string,    1] 체결 구분 코드
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] 전일 대비 부호
    char    STCK_DATA_SRNO      [  21];    // [numstring,   21] 주식 데이터 일련번호
    char    STCK_CNTG_HOUR      [   6];    // [string,    6] 주식 체결 시간
    char    STCK_PRPR           [  11];    // [numstring,   11] 주식 현재가
    char    PRDY_VRSS           [  11];    // [numstring,   11] 전일 대비
    char    PRDY_CTRT           [   9];    // [numstring,  9.2] 전일 대비율
    char    ASKP                [  11];    // [numstring,   11] 매도호가
    char    BIDP                [  11];    // [numstring,   11] 매수호가
    char    ASKP_RSQN1          [  13];    // [numstring,   13] 매도호가 잔량1
    char    BIDP_RSQN1          [  13];    // [numstring,   13] 매수호가 잔량1
    char    STCK_HGPR           [  11];    // [numstring,   11] 주식 최고가
    char    STCK_LWPR           [  11];    // [numstring,   11] 주식 최저가
    char    CNTG_VOL            [  19];    // [numstring,   19] 체결 거래량
    char    ACML_VOL            [  19];    // [numstring,   19] 누적 거래량
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] 누적 거래 대금
    char    TOTAL_ASKP_RSQN     [  13];    // [numstring,   13] 총 매도호가 잔량
    char    TOTAL_BIDP_RSQN     [  13];    // [numstring,   13] 총 매수호가 잔량
    char    STCK_OPRC           [  11];    // [numstring,   11] 주식 시가2
    char    TOTAL_SELN_QTY      [  19];    // [numstring,   19] 총 매도 수량
    char    TOTAL_SHNU_QTY      [  19];    // [numstring,   19] 총 매수2 수량
    char    WGHN_AVRG_STCK_PRC  [  20];    // [numstring, 20.2] 가중 평균 주식 가격
    char    HTS_AVLS            [  19];    // [numstring,   19] HTS 시가총액
    char    HOUR_CLS_CODE       [   1];    // [string,    1] 시간 구분 코드
    char    CTTR                [  12];    // [numstring, 12.2] 체결강도
    char    STCK_PRDY_CLPR      [  11];    // [numstring,   11] 주식 전일 종가
    char    NEW_MKOP_CLS_CODE   [   2];    // [string,    2] 신 장운영 구분 코드
    char    SELN_CNTG_QTY       [  19];    // [numstring,   19] 매도 체결 수량
    char    SHNU_CNTG_QTY       [  19];    // [numstring,   19] 매수2 체결 수량
    char    NTBY_CNQN           [  19];    // [numstring,   19] 순매수 체결량
} ohhts_stck_cntg_OutBlock1, *LPohhts_stck_cntg_OutBlock1;
#define NAME_ohhts_stck_cntg_OutBlock1     "OutBlock1"

class CReal_STOCK
{
public: //Variable
    CString m_szMKSC_SHRN_ISCD      ;    // [string,    6] 유가증권 단축 종목코드
    CString m_szTR_SUSP_CLS_CODE    ;    // [string,    2] 거래 정지 구분 코드
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] 체결 구분 코드
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] 전일 대비 부호
    CString m_szSTCK_DATA_SRNO      ;    // [numstring,   21] 주식 데이터 일련번호
    CString m_szSTCK_CNTG_HOUR      ;    // [string,    6] 주식 체결 시간
    CString m_szSTCK_PRPR           ;    // [numstring,   11] 주식 현재가
    CString m_szPRDY_VRSS           ;    // [numstring,   11] 전일 대비
    CString m_szPRDY_CTRT           ;    // [numstring,  9.2] 전일 대비율
    CString m_szASKP                ;    // [numstring,   11] 매도호가
    CString m_szBIDP                ;    // [numstring,   11] 매수호가
    CString m_szASKP_RSQN1          ;    // [numstring,   13] 매도호가 잔량1
    CString m_szBIDP_RSQN1          ;    // [numstring,   13] 매수호가 잔량1
    CString m_szSTCK_HGPR           ;    // [numstring,   11] 주식 최고가
    CString m_szSTCK_LWPR           ;    // [numstring,   11] 주식 최저가
    CString m_szCNTG_VOL            ;    // [numstring,   19] 체결 거래량
    CString m_szACML_VOL            ;    // [numstring,   19] 누적 거래량
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] 누적 거래 대금
    CString m_szTOTAL_ASKP_RSQN     ;    // [numstring,   13] 총 매도호가 잔량
    CString m_szTOTAL_BIDP_RSQN     ;    // [numstring,   13] 총 매수호가 잔량
    CString m_szSTCK_OPRC           ;    // [numstring,   11] 주식 시가2
    CString m_szTOTAL_SELN_QTY      ;    // [numstring,   19] 총 매도 수량
    CString m_szTOTAL_SHNU_QTY      ;    // [numstring,   19] 총 매수2 수량
    CString m_szWGHN_AVRG_STCK_PRC  ;    // [numstring, 20.2] 가중 평균 주식 가격
    CString m_szHTS_AVLS            ;    // [numstring,   19] HTS 시가총액
    CString m_szHOUR_CLS_CODE       ;    // [string,    1] 시간 구분 코드
    CString m_szCTTR                ;    // [numstring, 12.2] 체결강도
    CString m_szSTCK_PRDY_CLPR      ;    // [numstring,   11] 주식 전일 종가
    CString m_szNEW_MKOP_CLS_CODE   ;    // [string,    2] 신 장운영 구분 코드
    CString m_szSELN_CNTG_QTY       ;    // [numstring,   19] 매도 체결 수량
    CString m_szSHNU_CNTG_QTY       ;    // [numstring,   19] 매수2 체결 수량
    CString m_szNTBY_CNQN           ;    // [numstring,   19] 순매수 체결량

public:
	CReal_STOCK()
	{
	}

	virtual void  Convert(ohhts_stck_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, MKSC_SHRN_ISCD      );    // [string,    6] 유가증권 단축 종목코드
		DATA_MEMCOPY(sTmp, pInput, TR_SUSP_CLS_CODE    );    // [string,    2] 거래 정지 구분 코드
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] 체결 구분 코드
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] 전일 대비 부호
		DATA_MEMCOPY(sTmp, pInput, STCK_DATA_SRNO      );    // [numstring,   21] 주식 데이터 일련번호
		DATA_MEMCOPY(sTmp, pInput, STCK_CNTG_HOUR      );    // [string,    6] 주식 체결 시간
		DATA_MEMCOPY(sTmp, pInput, STCK_PRPR           );    // [numstring,   11] 주식 현재가
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS           );    // [numstring,   11] 전일 대비
		DATA_MEMCOPY(sTmp, pInput, PRDY_CTRT           );    // [numstring,  9.2] 전일 대비율
		DATA_MEMCOPY(sTmp, pInput, ASKP                );    // [numstring,   11] 매도호가
		DATA_MEMCOPY(sTmp, pInput, BIDP                );    // [numstring,   11] 매수호가
		DATA_MEMCOPY(sTmp, pInput, ASKP_RSQN1          );    // [numstring,   13] 매도호가 잔량1
		DATA_MEMCOPY(sTmp, pInput, BIDP_RSQN1          );    // [numstring,   13] 매수호가 잔량1
		DATA_MEMCOPY(sTmp, pInput, STCK_HGPR           );    // [numstring,   11] 주식 최고가
		DATA_MEMCOPY(sTmp, pInput, STCK_LWPR           );    // [numstring,   11] 주식 최저가
		DATA_MEMCOPY(sTmp, pInput, CNTG_VOL            );    // [numstring,   19] 체결 거래량
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] 누적 거래량
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] 누적 거래 대금
		DATA_MEMCOPY(sTmp, pInput, TOTAL_ASKP_RSQN     );    // [numstring,   13] 총 매도호가 잔량
		DATA_MEMCOPY(sTmp, pInput, TOTAL_BIDP_RSQN     );    // [numstring,   13] 총 매수호가 잔량
		DATA_MEMCOPY(sTmp, pInput, STCK_OPRC           );    // [numstring,   11] 주식 시가2
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SELN_QTY      );    // [numstring,   19] 총 매도 수량
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SHNU_QTY      );    // [numstring,   19] 총 매수2 수량
		DATA_MEMCOPY(sTmp, pInput, WGHN_AVRG_STCK_PRC  );    // [numstring, 20.2] 가중 평균 주식 가격
		DATA_MEMCOPY(sTmp, pInput, HTS_AVLS            );    // [numstring,   19] HTS 시가총액
		DATA_MEMCOPY(sTmp, pInput, HOUR_CLS_CODE       );    // [string,    1] 시간 구분 코드
		DATA_MEMCOPY(sTmp, pInput, CTTR                );    // [numstring, 12.2] 체결강도
		DATA_MEMCOPY(sTmp, pInput, STCK_PRDY_CLPR      );    // [numstring,   11] 주식 전일 종가
		DATA_MEMCOPY(sTmp, pInput, NEW_MKOP_CLS_CODE   );    // [string,    2] 신 장운영 구분 코드
		DATA_MEMCOPY(sTmp, pInput, SELN_CNTG_QTY       );    // [numstring,   19] 매도 체결 수량
		DATA_MEMCOPY(sTmp, pInput, SHNU_CNTG_QTY       );    // [numstring,   19] 매수2 체결 수량
		DATA_MEMCOPY(sTmp, pInput, NTBY_CNQN           );    // [numstring,   19] 순매수 체결량
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
