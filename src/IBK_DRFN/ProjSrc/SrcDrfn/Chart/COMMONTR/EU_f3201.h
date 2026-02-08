/*************************************************************************************/
/*  1. 프로그램ID : EU_f3201                                                         */
/*  2. 프로그램명 : 주식옵션(호가)                                                   */
/*  3. 서비스명   : EU_f3201                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_f3201_H__
#define __EU_TRAN_EU_f3201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_f3201"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		optcode[8];		/*  종목코드     */
	char		mode[1];		/*  조회모드     */
	char		count[4];		/*  조회건수     */
}EU_f3201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char EU_f3201_OutRec1_count[4];
//typedef struct
//{
//	char		optcode[8];		/*  단축코드     */
//	char		expcode[12];		/*  종목코드     */
//	char		ipwhedate[8];		/*  입회일자     */
//	char		seqno[4];		/*  종목SEQ번호     */
//	char		opkind[1];		/* 옵션종류     */
//	char		eitem[2];		/*  거래대상종목     */
//	char		lastmonth[6];		/*  만기년월     */
//	char		actprice[15];		/*  행사가격     */
//	char		actprice_r[8];		/*  행사가격(조회용)     */
//	char		tradeunit[16];		/*  거래단위     */
//	char		queryno[4];		/*  조회순번     */
//	char		hname[50];		/*  한글종목명     */
//	char		ename[50];		/*  영문종목명     */
//	char		name[25];		/*  종목축약명     */
//	char		actgubun[1];		/*  권리행사유형구분     */
//	char		listdatecnt[4];		/*  기준일수(상장일수)     */
//	char		jandatecnt[4];		/*  잔존일수     */
//	char		optgubun[1];		/*  최근월물구분     */
//	char		atmgubun[1];		/*  ATM구분     */
//	char		listdate[8];		/*  상장일     */
//	char		startdate[8];		/*  최초거래일     */
//	char		lastdate[8];		/*  최종거래일     */
//	char		lastdategubun[1];		/*  최종거래일구분     */
//	char		newgubun[1];		/*  종목상장구분     */
//	char		newdate[8];		/*  종목상장작업일     */
//	char		divideratio[13];		/*  배당액지수현재가치     */
//	char		theorypricek[13];		/*  이론가     */
//	char		jnilopen[7];		/*  전일시가     */
//	char		jnilhigh[7];		/*  전일고가     */
//	char		jnillow[7];		/*  전일저가     */
//	char		jnilclose[7];		/*  전일종가     */
//	char		jnilclosegubun[1];		/*  전일종가구분     */
//	char		bmarginprice[7];		/*  매매증거금기준가     */
//	char		bmargingubun[1];		/*  매매증거금기준가구분     */
//	char		hprice[7];		/*  정상호가범위     */
//	char		lprice[7];		/*  정상호가범위     */
//	char		cdratio[6];		/*  CD금리     */
//	char		jnilopenyak[8];		/* 전일미결제약정수량     */
//	char		jnilvolume[8];		/*  전일거래량     */
//	char		jnilvalue[15];		/* 전일거래대금     */
//	char		yearhdate[8];		/*  연중최고가일     */
//	char		yearhprice[7];		/*  연중최고가     */
//	char		yearldate[8];		/*  연중최저가일     */
//	char		yearlprice[7];		/*  연중최저가     */
//	char		listhdate[8];		/*  상장중최고가일     */
//	char		listhprice[7];		/*  상장중최고가     */
//	char		listldate[8];		/*  상장중최저가일     */
//	char		listlprice[7];		/*  상장중최저가     */
//	char		mrkgubun[1];		/*  시장가허용구분     */
//	char		jogengubun[1];		/*  조건부지정가허용구분     */
//	char		choiygubun[1];		/*  최유리지정가허용구분     */
//	char		recprice[7];		/* 기준가격     */
//	char		recpricegubun[1];		/*  기준가격구분     */
//	char		bimpv[6];		/* 내재변동성     */
//	char		adjustgubun[1];		/*  조정구분     */
//	char		stopgubun[1];		/*  거래정지여부     */
//	char		mmchetype[1];		/*  거래정지여부     */
//	char		itemcode[12];		/*  기초자산종목코드     */
//	char		settletype[1];		/*  결제방법     */
//	char		mktmakegubun[1];		/*  시장조성종목여부     */
//	char		expname[20];		/*  표준종목명     */
//	char		bdate[8];		/*  최종수신일자     */
//	char		xday[4];		/* BATCH없이경과일수     */
//	char		bjandatecnt[4];		/* 영업일잔존일수     */
//	char		chetime[10];		/*  체결시간     */
//	char		price[7];		/*  현재가     */
//	char		open[7];		/*  시가     */
//	char		high[7];		/*  고가     */
//	char		low[7];		/*  저가     */
//	char		sign[1];		/*  전일대비구분     */
//	char		change[7];		/*  전일대비　     */
//	char		volume[7];		/*  누적거래량     */
//	char		value[11];		/*  누적거래대금     */
//	char		cvolume[7];		/*  건별체결수량     */
//	char		cgubun[1];		/*  체결구분     */
//	char		mdvolume[8];		/* 매도누적거래량     */
//	char		msvolume[8];		/*  매수누적거래량     */
//	char		mdchecnt[5];		/* 매도누적체결건수     */
//	char		mschecnt[5];		/*  매수누적체결건수     */
//	char		mdvolume_h[7];		/*  매도체결수량     */
//	char		msvolume_h[7];		/*  매수체결수량     */
//	char		hotime[10];		/*  수신시간     */
//	char		offerho1[7];		/* 매도호가1     */
//	char		bidho1[7];		/*  매수호가1     */
//	char		offerrem1[7];		/*  매도호가수량1     */
//	char		bidrem1[7];		/*  매수호가수량1     */
//	char		offercnt1[5];		/*  매도호가건수1     */
//	char		bidcnt1[5];		/*  매수호가건수1     */
//	char		offerremcha1[7];		/*  직전매도대비수량1     */
//	char		bidremcha1[7];		/*  직전매수대비수량1     */
//	char		offercntcha1[5];		/*  직전매도대비건수1     */
//	char		bidcntcha1[5];		/*  직전매수대비건수1     */
//	char		offerho2[7];		/*  매도호가2     */
//	char		bidho2[7];		/*  매수호가2     */
//	char		offerrem2[7];		/*  매도호가수량2     */
//	char		bidrem2[7];		/*  매수호가수량2     */
//	char		offercnt2[5];		/*  매도호가건수2     */
//	char		bidcnt2[5];		/*  매수호가건수2     */
//	char		offerremcha2[7];		/*  직전매도대비수량2     */
//	char		bidremcha2[7];		/*  직전매수대비수량2     */
//	char		offercntcha2[5];		/*  직전매도대비건수2     */
//	char		bidcntcha2[5];		/*  직전매수대비건수2     */
//	char		offerho3[7];		/*  매도호가3     */
//	char		bidho3[7];		/*  매수호가3     */
//	char		offerrem3[7];		/*  매도호가수량3     */
//	char		bidrem3[7];		/*  매수호가수량3     */
//	char		offercnt3[5];		/*  매도호가건수3     */
//	char		bidcnt3[5];		/*  매수호가건수3     */
//	char		offerremcha3[7];		/*  직전매도대비수량3     */
//	char		bidremcha3[7];		/*  직전매수대비수량3     */
//	char		offercntcha3[5];		/*  직전매도대비건수3     */
//	char		bidcntcha3[5];		/*  직전매수대비건수3     */
//	char		offerho4[7];		/*  매도호가4     */
//	char		bidho4[7];		/*  매수호가4     */
//	char		offerrem4[7];		/*  매도호가수량4     */
//	char		bidrem4[7];		/*  매수호가수량4     */
//	char		offercnt4[5];		/*  매도호가건수4     */
//	char		bidcnt4[5];		/*  매수호가건수4     */
//	char		offerremcha4[7];		/*  직전매도대비수량4     */
//	char		bidremcha4[7];		/*  직전매수대비수량4     */
//	char		offercntcha4[5];		/*  직전매도대비건수4     */
//	char		bidcntcha4[5];		/*  직전매수대비건수4     */
//	char		offerho5[7];		/*  매도호가5     */
//	char		bidho5[7];		/*  매수호가5     */
//	char		offerrem5[7];		/*  매도호가수량5     */
//	char		bidrem5[7];		/*  매수호가수량5     */
//	char		offercnt5[5];		/*  매도호가건수5     */
//	char		bidcnt5[5];		/*  매수호가건수5     */
//	char		offerremcha5[7];		/*  직전매도대비수량5     */
//	char		bidremcha5[7];		/*  직전매수대비수량5     */
//	char		offercntcha5[5];		/*  직전매도대비건수5     */
//	char		bidcntcha5[5];		/*  직전매수대비건수5     */
//	char		totofferrem[7];		/*  매도호가총수량     */
//	char		totbidrem[7];		/*  매수호가총수량     */
//	char		totoffercnt[6];		/*  매도호가총건수     */
//	char		totbidcnt[6];		/* 매수호가총건수     */
//	char		totofferremcha[7];		/*  직전매도대비총수량     */
//	char		totbidremcha[7];		/*  직전매수대비총수량     */
//	char		totoffercntcha[6];		/* 직전매도대비총건수     */
//	char		totbidcntcha[6];		/* 직전매수대비총건수     */
//	char		jgubun[2];		/*  장운용구분     */
//	char		yakdate[8];		/*  체결일자     */
//	char		mggubun[1];		/*  미결제약정구분     */
//	char		openyak[7];		/*  미결제약정수량     */
//	char		openyakcha[7];		/* 미결제약정수량직전대비     */
//	char		openopenyak[7];		/*  미결제약정시가     */
//	char		highopenyak[7];		/*  미결제약정고가     */
//	char		lowopenyak[7];		/*  미결제약정저가     */
//	char		marginprice[7];		/*  매매증거금기준가     */
//	char		margingubun[1];		/*  매매증거금기준가구분     */
//	char		lastprice[16];		/*  권리행사결제가격     */
//	char		lastpricegubun[1];		/*  권리행사결제가격구분     */
//	char		baseprice[7];		/*  대상종목현제가     */
//	char		eqva[6];		/*  KOSPI등가     */
//	char		xtime[8];		/*  지표산출시간     */
//	char		theoryprice[13];		/*  장중이론가     */
//	char		delt[6];		/*  델타     */
//	char		gama[6];		/*  감마     */
//	char		ceta[6];		/*  세타     */
//	char		vega[6];		/*  베가     */
//	char		rhox[6];		/*  로우     */
//	char		impv[6];		/*  내재변동성     */
//	char		timevalue[6];		/*  시간가치     */
//	char		offerimpv[6];		/*  매도호가내재변동성     */
//	char		bidimpv[6];		/*  매도호가내재변동성     */
//	char		histimpv[6];		/*  역사적변동성     */
//	char		b_theoryprice[6];		/*  장중이론가이항모델     */
//	char		b_delt[6];		/*  델타이항모델     */
//	char		b_gama[6];		/*  감마이항모델     */
//	char		b_ceta[6];		/*  세타이항모델     */
//	char		b_vega[6];		/*  베가이항모델     */
//	char		b_rhox[6];		/*  로우이항모델     */
//	char		drate[6];		/*  등락율     */
//	char		b_volrate[6];		/*  전일거래량비     */
//}EU_f3201_OutRec1;

typedef struct _EU_f3201_OutRec1 {
	char		optcode[8];		/*  단축코드     */
	char		recprice[7];		/*  기준가격     */
	char		hprice[7];		/*  상한가     */
	char		lprice[7];		/*  하한가     */
	char		price[7];		/*  현재가     */
	char		offerho1[8];		/*  매도호가1     */
	char		offerho2[8];		/*  매도호가2     */
	char		offerho3[8];		/*  매도호가3     */
	char		bidho1[8];		/*  매수호가1     */
	char		bidho2[8];		/*  매수호가2     */
	char		bidho3[8];		/*  매수호가3     */
} EU_f3201_OutRec1;

/*************************************************************************************/
class CEU_f3201
{
public: //Variable
	CString		m_szoptcode;		/*  단축코드     */
	CString		m_szrecprice;		/*  기준가격     */
	CString		m_szhprice;		/*  상한가     */
	CString		m_szlprice;		/*  하한가     */
	CString		m_szprice;		/*  현재가     */
	CString		m_szofferho1;		/*  매도호가1     */
	CString		m_szofferho2;		/*  매도호가2     */
	CString		m_szofferho3;		/*  매도호가3     */
	CString		m_szbidho1;		/*  매수호가1     */
	CString		m_szbidho2;		/*  매수호가2     */
	CString		m_szbidho3;		/*  매수호가3     */

public:
	CEU_f3201()
	{
	}

	virtual void  Convert(EU_f3201_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, optcode);		/*  단축코드     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/*  기준가격     */
		DATA_MEMCOPY(sTmp, pInput, hprice);		/*  상한가     */
		DATA_MEMCOPY(sTmp, pInput, lprice);		/*  하한가     */
		DATA_MEMCOPY(sTmp, pInput, price);		/*  현재가     */
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  매도호가1     */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  매도호가2     */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  매도호가3     */
		DATA_MEMCOPY(sTmp, pInput, bidho1);		/*  매수호가1     */
		DATA_MEMCOPY(sTmp, pInput, bidho2);		/*  매수호가2     */
		DATA_MEMCOPY(sTmp, pInput, bidho3);		/*  매수호가3     */
	}
};

#endif

