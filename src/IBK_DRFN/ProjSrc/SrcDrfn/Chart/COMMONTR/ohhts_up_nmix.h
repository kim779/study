#if !defined(AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_)
#define AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 업종지수 (  )
#pragma pack( push, 1 )

#define NAME_ohhts_up_nmix     "ohhts_up_nmix"

// 
typedef struct _ohhts_up_nmix_InBlock1
{
    char    BSTP_CLS_CODE       [   4];    // [string,    4] 업종 구분 코드
} ohhts_up_nmix_InBlock1, *LPohhts_up_nmix_InBlock1;
#define NAME_ohhts_up_nmix_InBlock1     "InBlock1"

// 
typedef struct _ohhts_up_nmix_OutBlock1
{
    char    BSTP_CLS_CODE       [   4];    // [string,    4] 업종 구분 코드
    char    PRPR_HOUR           [   6];    // [string,    6] 현재가 시간
    char    BSTP_NMIX_PRPR      [  12];    // [numstring, 12.2] 업종 지수 현재가
    char    DATA_RANK           [  11];    // [numstring,   11] 데이터 순위
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] 전일 대비 부호
    char    BSTP_NMIX_PRDY_VRSS [  12];    // [numstring, 12.2] 업종 지수 전일 대비
    char    ACML_VOL            [  19];    // [numstring,   19] 누적 거래량
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] 누적 거래 대금
    char    BSTP_NMIX_HGPR      [  12];    // [numstring, 12.2] 업종 지수 최고가
    char    BSTP_NMIX_LWPR      [  12];    // [numstring, 12.2] 업종 지수 최저가
    char    CNTG_VOL            [  19];    // [numstring,   19] 체결 거래량
    char    BSTP_NMIX_OPRC      [  12];    // [numstring, 12.2] 업종 지수 시가2
    char    BSTP_NMIX_PRDY_CTRT [   9];    // [numstring,  9.2] 업종 지수 전일 대비율
    char    ASCN_ISSU_CNT       [   8];    // [numstring,    8] 상승 종목 수
    char    UPLM_ISSU_CNT       [   8];    // [numstring,    8] 상한 종목 수
    char    DOWN_ISSU_CNT       [   8];    // [numstring,    8] 하락 종목 수
    char    LSLM_ISSU_CNT       [   8];    // [numstring,    8] 하한 종목 수
    char    STNR_ISSU_CNT       [   8];    // [numstring,    8] 보합 종목 수
    char    ASCN_ISSU_RATE      [   9];    // [numstring,  9.4] 상승 종목 비율
    char    FRGN_NTBY_QTY       [  13];    // [numstring,   13] 외국인 순매수 수량
    char    ORGN_NTBY_QTY       [  19];    // [numstring,   19] 기관계 순매수 수량
} ohhts_up_nmix_OutBlock1, *LPohhts_up_nmix_OutBlock1;
#define NAME_ohhts_up_nmix_OutBlock1     "OutBlock1"

class CREAL_UPJONG
{
public: //Variable
    CString m_szBSTP_CLS_CODE       ;    // [string,    4] 업종 구분 코드
    CString m_szPRPR_HOUR           ;    // [string,    6] 현재가 시간
    CString m_szBSTP_NMIX_PRPR      ;    // [numstring, 12.2] 업종 지수 현재가
    CString m_szDATA_RANK           ;    // [numstring,   11] 데이터 순위
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] 전일 대비 부호
    CString m_szBSTP_NMIX_PRDY_VRSS ;    // [numstring, 12.2] 업종 지수 전일 대비
    CString m_szACML_VOL            ;    // [numstring,   19] 누적 거래량
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] 누적 거래 대금
    CString m_szBSTP_NMIX_HGPR      ;    // [numstring, 12.2] 업종 지수 최고가
    CString m_szBSTP_NMIX_LWPR      ;    // [numstring, 12.2] 업종 지수 최저가
    CString m_szCNTG_VOL            ;    // [numstring,   19] 체결 거래량
    CString m_szBSTP_NMIX_OPRC      ;    // [numstring, 12.2] 업종 지수 시가2
    CString m_szBSTP_NMIX_PRDY_CTRT ;    // [numstring,  9.2] 업종 지수 전일 대비율
    CString m_szASCN_ISSU_CNT       ;    // [numstring,    8] 상승 종목 수
    CString m_szUPLM_ISSU_CNT       ;    // [numstring,    8] 상한 종목 수
    CString m_szDOWN_ISSU_CNT       ;    // [numstring,    8] 하락 종목 수
    CString m_szLSLM_ISSU_CNT       ;    // [numstring,    8] 하한 종목 수
    CString m_szSTNR_ISSU_CNT       ;    // [numstring,    8] 보합 종목 수
    CString m_szASCN_ISSU_RATE      ;    // [numstring,  9.4] 상승 종목 비율
    CString m_szFRGN_NTBY_QTY       ;    // [numstring,   13] 외국인 순매수 수량
    CString m_szORGN_NTBY_QTY       ;    // [numstring,   19] 기관계 순매수 수량

public:
	CREAL_UPJONG()
	{
	}

	virtual void  Convert(ohhts_up_nmix_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, BSTP_CLS_CODE       );    // [string,    4] 업종 구분 코드
		DATA_MEMCOPY(sTmp, pInput, PRPR_HOUR           );    // [string,    6] 현재가 시간
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRPR      );    // [numstring, 12.2] 업종 지수 현재가
		DATA_MEMCOPY(sTmp, pInput, DATA_RANK           );    // [numstring,   11] 데이터 순위
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] 전일 대비 부호
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRDY_VRSS );    // [numstring, 12.2] 업종 지수 전일 대비
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] 누적 거래량
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] 누적 거래 대금
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_HGPR      );    // [numstring, 12.2] 업종 지수 최고가
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_LWPR      );    // [numstring, 12.2] 업종 지수 최저가
		DATA_MEMCOPY(sTmp, pInput, CNTG_VOL            );    // [numstring,   19] 체결 거래량
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_OPRC      );    // [numstring, 12.2] 업종 지수 시가2
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRDY_CTRT );    // [numstring,  9.2] 업종 지수 전일 대비율
		DATA_MEMCOPY(sTmp, pInput, ASCN_ISSU_CNT       );    // [numstring,    8] 상승 종목 수
		DATA_MEMCOPY(sTmp, pInput, UPLM_ISSU_CNT       );    // [numstring,    8] 상한 종목 수
		DATA_MEMCOPY(sTmp, pInput, DOWN_ISSU_CNT       );    // [numstring,    8] 하락 종목 수
		DATA_MEMCOPY(sTmp, pInput, LSLM_ISSU_CNT       );    // [numstring,    8] 하한 종목 수
		DATA_MEMCOPY(sTmp, pInput, STNR_ISSU_CNT       );    // [numstring,    8] 보합 종목 수
		DATA_MEMCOPY(sTmp, pInput, ASCN_ISSU_RATE      );    // [numstring,  9.4] 상승 종목 비율
		DATA_MEMCOPY(sTmp, pInput, FRGN_NTBY_QTY       );    // [numstring,   13] 외국인 순매수 수량
		DATA_MEMCOPY(sTmp, pInput, ORGN_NTBY_QTY       );    // [numstring,   19] 기관계 순매수 수량
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
