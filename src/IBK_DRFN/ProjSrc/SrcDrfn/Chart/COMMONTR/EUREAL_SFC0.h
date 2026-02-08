#if !defined(AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_)
#define AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     스타
//--------------------------------------------------------------------

typedef struct {
    char    dgubun            [4]; /* Data 구분 : 'SFC0'                     */
    char    kfutcode          [8]; /* 선물단축코드                           */
    char    chetime           [6]; /* 체결시간 HHMMSS                        */
    char    sign              [1]; /* 2:상승 3:보합 5:하락                   */
    char    change            [6]; /* 전일대비                     9(4)v9(2) */
    char    drate             [6]; /* 등락율                       9(4)v9(2) */
    char    price             [6]; /* 현재가                       9(4)v9(2) */
    char    open              [6]; /* 시가                         9(4)v9(2) */
    char    high              [6]; /* 고가                         9(4)v9(2) */
    char    low               [6]; /* 저가                         9(4)v9(2) */
    char    cgubun            [1]; /* 체결구분           ('+':매수 '-':매수) */
    char    cvolume           [6]; /* 체결량                       단위:계약 */
    char    volume           [12]; /* 누적거래량                   단위:계약 */
    char    volchange        [12]; /* 거래량전일대비               단위:계약 */
    char    voldrate          [8]; /* 거래량등락율                 9(6)v9(2) */
    char    value            [12]; /* 누적거래대금                 단위:천원 */
    char    mdvolume         [12]; /* 매도누적 체결량                        */
    char    mdchecnt          [8]; /* 매도누적 체결건수                      */
    char    msvolume         [12]; /* 매수누적 체결량                        */
    char    mschecnt          [8]; /* 매수누적 체결건수                      */
    char    cpower            [9]; /* 체결강도 999999.99                     */
    char    offerho1          [6]; /* 매도호가1                    9(4)v9(2) */
    char    bidho1            [6]; /* 매수호가1                    9(4)v9(2) */
    char    openyak           [8]; /* 미결제약정수량               단위:계약 */
    char    openyakchange     [8]; /* 미결제약정전일대비           단위:계약 */
    char    starjisu          [6]; /* 스타지수                               */
    char    theoryprice       [6]; /* 이론가                       9(4)v9(2) */
    char    grate             [6]; /* 괴리도                       9(4)v9(2) */
    char    kasis             [6]; /* 괴리율                       9(4)v9(2) */
    char    sbasis            [6]; /* 시장 BASIS                   9(4)v9(2) */
    char    ibasis            [6]; /* 이론 BASIS                   9(4)v9(2) */
    char    futcode           [8]; /* 선물단축코드                           */
} stEUREAL_SFC0;	//67

class CEUREAL_SFC0
{
public: //Variable
    CString m_szdgubun;			/* Data 구분 : 'SFC0'                     */
    CString m_szkfutcode;		/* 선물단축코드                           */
    CString m_szchetime;		/* 체결시간 HHMMSS                        */
    CString m_szsign;			/* 2:상승 3:보합 5:하락                   */
    CString m_szchange;			/* 전일대비                     9(4)v9(2) */
    CString m_szdrate;			/* 등락율                       9(4)v9(2) */
    CString m_szprice;			/* 현재가                       9(4)v9(2) */
    CString m_szopen;			/* 시가                         9(4)v9(2) */
    CString m_szhigh;			/* 고가                         9(4)v9(2) */
    CString m_szlow;			/* 저가                         9(4)v9(2) */
    CString m_szcgubun;			/* 체결구분           ('+':매수 '-':매수) */
    CString m_szcvolume;		/* 체결량                       단위:계약 */
    CString m_szvolume;			/* 누적거래량                   단위:계약 */
    CString m_szvolchange;		/* 거래량전일대비               단위:계약 */
    CString m_szvoldrate;		/* 거래량등락율                 9(6)v9(2) */
    CString m_szvalue;			/* 누적거래대금                 단위:천원 */
    CString m_szmdvolume;		/* 매도누적 체결량                        */
    CString m_szmdchecnt;		/* 매도누적 체결건수                      */
    CString m_szmsvolume;		/* 매수누적 체결량                        */
    CString m_szmschecnt;		/* 매수누적 체결건수                      */
    CString m_szcpower;			/* 체결강도 999999.99                     */
    CString m_szofferho1;		/* 매도호가1                    9(4)v9(2) */
    CString m_szbidho1;			/* 매수호가1                    9(4)v9(2) */
    CString m_szopenyak;		/* 미결제약정수량               단위:계약 */
    CString m_szopenyakchange;	/* 미결제약정전일대비           단위:계약 */
    CString m_szstarjisu;		/* 스타지수                               */
    CString m_sztheoryprice;	/* 이론가                       9(4)v9(2) */
    CString m_szgrate;			/* 괴리도                       9(4)v9(2) */
    CString m_szkasis;			/* 괴리율                       9(4)v9(2) */
    CString m_szsbasis;			/* 시장 BASIS                   9(4)v9(2) */
    CString m_szibasis;			/* 이론 BASIS                   9(4)v9(2) */
    CString m_szfutcode;		/* 선물단축코드                           */

public:
	CEUREAL_SFC0()
	{
	}

	virtual void  Convert(stEUREAL_SFC0* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun            );/* Data 구분 : 'SFC0'                     */
		DATA_MEMCOPY(sTmp, pInput, kfutcode          );/* 선물단축코드                           */
		DATA_MEMCOPY(sTmp, pInput, chetime           );/* 체결시간 HHMMSS                        */
		DATA_MEMCOPY(sTmp, pInput, sign              );/* 2:상승 3:보합 5:하락                   */
		DATA_MEMCOPY(sTmp, pInput, change            );/* 전일대비                     9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, drate             );/* 등락율                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, price             );/* 현재가                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, open              );/* 시가                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, high              );/* 고가                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, low               );/* 저가                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, cgubun            );/* 체결구분           ('+':매수 '-':매수) */
		DATA_MEMCOPY(sTmp, pInput, cvolume           );/* 체결량                       단위:계약 */
		DATA_MEMCOPY(sTmp, pInput, volume            );/* 누적거래량                   단위:계약 */
		DATA_MEMCOPY(sTmp, pInput, volchange         );/* 거래량전일대비               단위:계약 */
		DATA_MEMCOPY(sTmp, pInput, voldrate          );/* 거래량등락율                 9(6)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, value             );/* 누적거래대금                 단위:천원 */
		DATA_MEMCOPY(sTmp, pInput, mdvolume          );/* 매도누적 체결량                        */
		DATA_MEMCOPY(sTmp, pInput, mdchecnt          );/* 매도누적 체결건수                      */
		DATA_MEMCOPY(sTmp, pInput, msvolume          );/* 매수누적 체결량                        */
		DATA_MEMCOPY(sTmp, pInput, mschecnt          );/* 매수누적 체결건수                      */
		DATA_MEMCOPY(sTmp, pInput, cpower            );/* 체결강도 999999.99                     */
		DATA_MEMCOPY(sTmp, pInput, offerho1          );/* 매도호가1                    9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, bidho1            );/* 매수호가1                    9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, openyak           );/* 미결제약정수량               단위:계약 */
		DATA_MEMCOPY(sTmp, pInput, openyakchange     );/* 미결제약정전일대비           단위:계약 */
		DATA_MEMCOPY(sTmp, pInput, starjisu          );/* 스타지수                               */
		DATA_MEMCOPY(sTmp, pInput, theoryprice       );/* 이론가                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, grate             );/* 괴리도                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, kasis             );/* 괴리율                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, sbasis            );/* 시장 BASIS                   9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, ibasis            );/* 이론 BASIS                   9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, futcode           );/* 선물단축코드                           */
	}
};

#endif // AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_
