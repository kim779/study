#if !defined(AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_)
#define AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식선물 체결 (  )
#pragma pack( push, 1 )

#define NAME_ohhts_stft_cntg     "ohhts_stft_cntg"

// 
typedef struct _ohhts_stft_cntg_InBlock1
{
    char    FUTS_NEW_SHRN_ISCD  [   9];    // [string,    9] 선물 신 단축 종목코드
} ohhts_stft_cntg_InBlock1, *LPohhts_stft_cntg_InBlock1;
#define NAME_ohhts_stft_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_stft_cntg_OutBlock1
{
    char    FUTS_NEW_SHRN_ISCD  [   9];    // [string,    9] 선물 신 단축 종목코드
    char    CNTG_CLS_CODE       [   1];    // [string,    1] 체결 구분 코드
    char    FUTS_SRNO           [  11];    // [numstring,   11] 선물 일련번호
    char    CNTG_HOUR           [   6];    // [string,    6] 체결 시간
    char    PRPR                [  19];    // [numstring,   19] 현재가
    char    PRDY_VRSS           [  11];    // [numstring,   11] 전일 대비
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] 전일 대비 부호
    char    PRDY_CTRT           [   9];    // [numstring,  9.2] 전일 대비율
    char    ASKP                [  11];    // [numstring,   11] 매도호가
    char    BIDP                [  11];    // [numstring,   11] 매수호가
    char    STCK_HGPR           [  11];    // [numstring,   11] 주식 최고가
    char    STCK_LWPR           [  11];    // [numstring,   11] 주식 최저가
    char    STCK_OPRC           [  11];    // [numstring,   11] 주식 시가2
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
} ohhts_stft_cntg_OutBlock1, *LPohhts_stft_cntg_OutBlock1;
#define NAME_ohhts_stft_cntg_OutBlock1     "OutBlock1"

class CREAL_STOCKFUTURE
{
public: //Variable
    CString m_szFUTS_NEW_SHRN_ISCD  ;    // [string,    9] 선물 신 단축 종목코드
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] 체결 구분 코드
    CString m_szFUTS_SRNO           ;    // [numstring,   11] 선물 일련번호
    CString m_szCNTG_HOUR           ;    // [string,    6] 체결 시간
    CString m_szPRPR                ;    // [numstring,   19] 현재가
    CString m_szPRDY_VRSS           ;    // [numstring,   11] 전일 대비
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] 전일 대비 부호
    CString m_szPRDY_CTRT           ;    // [numstring,  9.2] 전일 대비율
    CString m_szASKP                ;    // [numstring,   11] 매도호가
    CString m_szBIDP                ;    // [numstring,   11] 매수호가
    CString m_szSTCK_HGPR           ;    // [numstring,   11] 주식 최고가
    CString m_szSTCK_LWPR           ;    // [numstring,   11] 주식 최저가
    CString m_szSTCK_OPRC           ;    // [numstring,   11] 주식 시가2
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
	CREAL_STOCKFUTURE()
	{
	}

	virtual void  Convert(ohhts_stft_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, FUTS_NEW_SHRN_ISCD  );    // [string,    9] 선물 신 단축 종목코드
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] 체결 구분 코드
		DATA_MEMCOPY(sTmp, pInput, FUTS_SRNO           );    // [numstring,   11] 선물 일련번호
		DATA_MEMCOPY(sTmp, pInput, CNTG_HOUR           );    // [string,    6] 체결 시간
		DATA_MEMCOPY(sTmp, pInput, PRPR                );    // [numstring,   19] 현재가
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS           );    // [numstring,   11] 전일 대비
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] 전일 대비 부호
		DATA_MEMCOPY(sTmp, pInput, PRDY_CTRT           );    // [numstring,  9.2] 전일 대비율
		DATA_MEMCOPY(sTmp, pInput, ASKP                );    // [numstring,   11] 매도호가
		DATA_MEMCOPY(sTmp, pInput, BIDP                );    // [numstring,   11] 매수호가
		DATA_MEMCOPY(sTmp, pInput, STCK_HGPR           );    // [numstring,   11] 주식 최고가
		DATA_MEMCOPY(sTmp, pInput, STCK_LWPR           );    // [numstring,   11] 주식 최저가
		DATA_MEMCOPY(sTmp, pInput, STCK_OPRC           );    // [numstring,   11] 주식 시가2
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

#endif // AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
