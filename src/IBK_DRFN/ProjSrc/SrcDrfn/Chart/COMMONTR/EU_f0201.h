/*************************************************************************************/
/*  1. 프로그램ID : EU_f0201                                                         */
/*  2. 프로그램명 : 선물옵션(호가)                                                   */
/*  3. 서비스명   : EU_f0201                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_f0201_H__
#define __EU_TRAN_EU_f0201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_f0201"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		futcode[8];		/*  종목코드     */
	char		mode[1];		/*  조회모드     */
	char		count[4];		/*  조회건수     */
}EU_f0201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char EU_f0201_OutRec1_count[4];
//typedef struct
//{
//	char		futcode[8];		/*  단축코드     */
//	char		expcode[12];		/*  종목코드     */
//	char		expname[20];		/*  표준종목명     */
//	char		hname[30];		/*  한글종목명     */
//	char		ename[30];		/*  영문종목명     */
//	char		sname[15];		/*  축약종목명     */
//	char		name[4];		/*  종목명     */
//	char		listdate[8];		/*  상장일     */
//	char		listdatecnt[3];		/*  상장일수     */
//	char		jandatecnt[3];		/*  잔존일수     */
//	char		lastdate[8];		/*  최종거래일     */
//	char		lastdategubun[1];		/*  최종거래일구분     */
//	char		divideratio[9];		/*  배당액지수미래가치     */
//	char		recpricegubun[1];		/*  기준가격구분     */
//	char		recprice[6];		/*  기준가격     */
//	char		theorypricek[6];		/*  이론가     */
//	char		theorypriceg[6];		/*  이론가     */
//	char		cdratio[6];		/*  CD금리     */
//	char		uplmtprice[6];		/*  상한가     */
//	char		dnlmtprice[6];		/*  하한가     */
//	char		jnilsetgubun[1];		/*  전일정산가구분     */
//	char		jnilsetprice[6];		/*  전일정산가     */
//	char		jnilclosegubun[1];		/*  전일종가구분     */
//	char		jnilclose[6];		/*  전일종가     */
//	char		jnilvolume[8];		/*  전일체결수량     */
//	char		jnilgmvolume[8];		/*  전일근월물체결수량     */
//	char		jnilwmvolume[8];		/*  전일원월물체결수량     */
//	char		jnilvalue[15];		/*  전일체결대금     */
//	char		jnilgmvalue[15];		/*  전일근월물거래대금     */
//	char		jnilwmvalue[15];		/*  전일원월물거래대금     */
//	char		jnilopenyak[8];		/*  전일미결제약정수량　     */
//	char		listhprice[6];		/*  상장중최고가(SIGN포함)     */
//	char		listhdate[8];		/*  상장중최고가일     */
//	char		listhrate[6];		/*  상장중최고가비율     */
//	char		listlprice[6];		/*  상장중최저가(SIGN포함)     */
//	char		listldate[8];		/*  상장중최저가일     */
//	char		listlrate[6];		/*  상장중최저가비율     */
//	char		ipwhedate[8];		/*  입회일자     */
//	char		mrkgubun[1];		/*  시장가허용구분     */
//	char		jogengubun[1];		/*  조건부지정가허용구분     */
//	char		choiygubun[1];		/*  최유리지정가허용구분     */
//	char		lastprice[6];		/*  최종결제가     */
//	char		lastpricegubun[1];		/*  최종결제가구분     */
//	char		cbhprice[6];		/*  CB적용상한가     */
//	char		cblprice[6];		/*  CB적용하한가     */
//	char		jnilmmchetime[8];		/*  전일최종매매체결시각     */
//	char		jnilspgmclose[6];		/*  전일스프레드근월물종가     */
//	char		jnilspwmclose[6];		/*  전일스프레드원월물종가     */
//	char		gexpcode[12];		/*  스프레드근월물표준코드     */
//	char		wexpcode[12];		/*  스프레드원월물표준코드     */
//	char		tradeunit[16];		/*  거래단위     */
//	char		chetime[6];		/*  체결시간     */
//	char		price[6];		/*  현재가     */
//	char		open[6];		/*  시가     */
//	char		high[6];		/*  고가     */
//	char		low[6];		/*  저가     */
//	char		sign[1];		/*  전일대비구분     */
//	char		change[6];		/*  전일대비　     */
//	char		volume[7];		/*  누적거래량     */
//	char		value[15];		/*  누적거래대금     */
//	char		vol_change[6];		/*  전일거래량대비     */
//	char		diff_rate[6];		/*  등락률     */
//	char		cvolume[6];		/*  건별체결수량     */
//	char		cgubun[1];		/*  체결구분     */
//	char		mdvolume[7];		/*  매도누적거래량     */
//	char		msvolume[7];		/*  매수누적거래량     */
//	char		mdchecnt[6];		/*  매도누적체결건수     */
//	char		mschecnt[6];		/* 매수누적체결건수     */
//	char		gmprice[6];		/*  근월물의제약정가격     */
//	char		wmprice[6];		/*  원월물의제약정가격     */
//	char		gmvolume[7];		/*  근월물체결수량     */
//	char		wmvolume[7];		/*  원월물체결수량     */
//	char		gmvalue[15];		/*  근월물거래대금     */
//	char		wmvalue[15];		/*  원월물거래대금     */
//	char		mdvolume_h[7];		/*  매도체결수량(호가기준)     */
//	char		msvolume_h[7];		/*  매수체결수량(호가기준)     */
//	char		hotime[6];		/*  호가수신시간     */
//	char		offerho1[6];		/*  매도호가1     */
//	char		bidho1[6];		/*  매수호가1     */
//	char		offerrem1[6];		/*  매도호가수량1     */
//	char		bidrem1[6];		/*  매수호가수량1     */
//	char		offercnt1[4];		/*  매도호가건수1     */
//	char		bidcnt1[4];		/*  매수호가건수1     */
//	char		offerho2[6];		/*  매도호가2     */
//	char		bidho2[6];		/*  매수호가2     */
//	char		offerrem2[6];		/*  매도호가수량2     */
//	char		bidrem2[6];		/*  매수호가수량2     */
//	char		offercnt2[4];		/*  매도호가건수2     */
//	char		bidcnt2[4];		/*  매수호가건수2     */
//	char		offerho3[6];		/*  매도호가3     */
//	char		bidho3[6];		/*  매수호가3     */
//	char		offerrem3[6];		/*  매도호가수량3     */
//	char		bidrem3[6];		/*  매수호가수량3     */
//	char		offercnt3[4];		/*  매도호가건수3     */
//	char		bidcnt3[4];		/*  매수호가건수3     */
//	char		offerho4[6];		/*  매도호가4     */
//	char		bidho4[6];		/*  매수호가4     */
//	char		offerrem4[6];		/*  매도호가수량4     */
//	char		bidrem4[6];		/*  매수호가수량4     */
//	char		offercnt4[4];		/*  매도호가건수4     */
//	char		bidcnt4[4];		/*  매수호가건수4     */
//	char		offerho5[6];		/*  매도호가5     */
//	char		bidho5[6];		/*  매수호가5     */
//	char		offerrem5[6];		/*  매도호가수량5     */
//	char		bidrem5[6];		/*  매수호가수량5     */
//	char		offercnt5[4];		/*  매도호가건수5     */
//	char		bidcnt5[4];		/*  매수호가건수5     */
//	char		totofferrem[6];		/*  매도호가총수량     */
//	char		totbidrem[6];		/*  매수호가총수량     */
//	char		totoffercnt[4];		/*  매도호가총건수     */
//	char		totbidcnt[4];		/*  매수호가총건수     */
//	char		jgubun[2];		/*  장운용구분     */
//	char		yakdate[8];		/*  체결일자     */
//	char		mggubun[1];		/*  미결제약정구분     */
//	char		jsprice[6];		/*  정산가격     */
//	char		jsgubun[1];		/*  정산가구분     */
//	char		k200jisu[8];		/*  KOSPI200지수     */
//	char		theoryprice[6];		/*  이론가     */
//	char		kasis[6];		/*  괴리율     */
//	char		sbasis[6];		/*  시장BASIS     */
//	char		ibasis[6];		/* 이론BASIS     */
//	char		openyak[8];		/*  미결제약정수량     */
//	char		openopenyak[8];		/*  미결제약정시가     */
//	char		highopenyak[8];		/* 미결제약정고가     */
//	char		lowopenyak[8];		/*  미결제약정저가     */
//	char		openupdn[8];		/*  미결제증감수량     */
//	char		offer2[6];		/*  2차저항     */
//	char		offer1[6];		/*  1차저항     */
//	char		pivot[6];		/*  피봇값     */
//	char		supp1[6];		/*  1차지지     */
//	char		supp2[6];		/*  2차지지     */
//	char		seqno[4];		/*  종목SEQ번호     */
//	char		opkind[1];		/*  옵션종류     */
//	char		eitem[2];		/*  거래대상종목     */
//	char		lastmonth[6];		/*  만기년월     */
//	char		actprice[6];		/*  행사가격     */
//	char		queryno[4];		/*  조회순번     */
//	char		actgubun[3];		/*  권리행사유형구분     */
//	char		lastgubun[1];		/* 최근월물구분     */
//	char		atmgubun[1];		/*  ATM구분     */
//	char		startdate[8];		/* 최초거래일     */
//	char		newgubun[1];		/*  신규구분     */
//	char		newdate[8];		/*  신규일자     */
//	char		jnilopen[6];		/*  전일시가     */
//	char		jnilhigh[6];		/*  전일고가     */
//	char		jnillow[6];		/*  전일저가     */
//	char		bmarginprice[6];		/*  매매증거금기준가     */
//	char		bmargingubun[1];		/*  매매증거금기준가구분     */
//	char		hprice[6];		/* 정상호가범위(상한치)     */
//	char		lprice[6];		/*  정상호가범위(하한치)     */
//	char		basevalue[9];		/*  가격단위기준값(3.00)     */
//	char		upunit[3];		/*  호가단위(0.01)     */
//	char		dwunit[3];		/*  호가단위(0.05)     */
//	char		yearhdate[8];		/*  연중최고가일     */
//	char		yearhprice[6];		/*  연중최고가     */
//	char		yearldate[8];		/*  연중최저가일     */
//	char		yearlprice[6];		/* 연중최저가     */
//	char		bimpv[6];		/*  전일내재변동성     */
//	char		marginprice[8];		/*  매매증거금기준가     */
//	char		margingubun[1];		/*  매매증거금기준가구분     */
//	char		eqva[8];		/*  KOSPI등가     */
//	char		xtime[8];		/*  지표산출시간     */
//	char		delt[7];		/*  델타     */
//	char		gama[7];		/*  감마     */
//	char		ceta[12];		/*  세타     */
//	char		vega[12];		/*  베가     */
//	char		rhox[12];		/*  로우     */
//	char		impv[9];		/*  내재변동성     */
//	char		imvalue[9];		/*  내재가치     */
//	char		timevalue[1];		/*  시간가치     */
//	char		offerimpv[9];		/*  매도호가내재변동성     */
//	char		bidimpv[9];		/*  매도호가내재변동성     */
//	char		histimpv[9];		/*  역사적변동성     */
//	char		b_theoryprice[14];		/*  장중이론가이항모델     */
//	char		b_delt[7];		/*  델타이항모델     */
//	char		b_gama[7];		/*  감마이항모델     */
//	char		b_ceta[7];		/*  세타이항모델     */
//	char		b_vega[7];		/*  베가이항모델     */
//	char		b_rhox[7];		/*  로우이항모델     */
//	char		mimpvc1[9];		/*  거래량Cal기준내재변동성     */
//	char		mimpvc2[9];		/*  거래대금Cal기준내재변동성     */
//	char		mimpvc3[9];		/*  거래량Put기준내재변동성     */
//	char		mimpvc4[9];		/*  거래대금Put기준내재변동성     */
//	char		impvchk[9];		/*  변동성시간대장마감을Update했는지     */
//	char		mktmakegubun[1];		/*  시장조성종목여부     */
//	char		xday[3];		/*  BATCH없이경과된일수     */
//	char		b_bidrem1[6];		/*  순매수잔량1     */
//	char		b_bidrem2[6];		/*  순매수잔량2     */
//	char		b_bidrem3[6];		/*  순매수잔량3     */
//	char		b_bidrem4[6];		/*  순매수잔량4     */
//	char		b_bidrem5[6];		/*  순매수잔량5     */
//	char		tot_bidrem[8];		/*  총순매수잔량     */
//	char		ho_drate1[6];		/*  매도호가등락율1     */
//	char		ho_drate2[6];		/*  매도호가등락율2     */
//	char		ho_drate3[6];		/*  매도호가등락율3     */
//	char		ho_drate4[6];		/*  매도호가등락율4     */
//	char		ho_drate5[6];		/*  매도호가등락율5     */
//	char		hb_drate1[6];		/*  매수호가등락율1     */
//	char		hb_drate2[6];		/*  매수호가등락율2     */
//	char		hb_drate3[6];		/*  매수호가등락율3     */
//	char		hb_drate4[6];		/*  매수호가등락율4     */
//	char		hb_drate5[6];		/*  매수호가등락율5     */
//	char		kasis_d[6];		/*  괴리도     */
//	char		bjandatecnt[3];		/*  영업일잔존일수     */
//	char		totofferremcha[8];		/*  직전매도대비총수량     */
//	char		totbidremcha[8];		/*  직전매수대비총수량     */
//	char		totoffercntcha[8];		/*  직전매도대비총건수     */
//	char		totbidcntcha[8];		/*  직전매수대비총건수     */
//	char		yeuplmt[6];		/*  익일예상상한가     */
//	char		yednlmt[6];		/*  익일예상하한가     */
//	char		yedate[8];		/* 예약실제주문일자     */
//}EU_f0201_OutRec1;

typedef struct _EU_f0201_OutRec1 {
	char		futcode[8];		/*  단축코드     */
	char		recprice[6];		/*  기준가격     */
	char		uplmtprice[6];		/*  상한가     */
	char		dnlmtprice[6];		/*  하한가     */
	char		price[6];		/*  현재가     */
	char		offerho1[6];		/*  매도호가1     */
	char		offerho2[6];		/*  매도호가2     */
	char		offerho3[6];		/*  매도호가3     */
	char		bidho1[6];		/*  매수호가1     */
	char		bidho2[6];		/*  매수호가2     */
	char		bidho3[6];		/*  매수호가3     */
} EU_f0201_OutRec1;

/*************************************************************************************/
class CEU_f0201
{
public: //Variable
	CString		m_szfutcode;		/*  단축코드     */
	CString		m_szrecprice;		/*  기준가격     */
	CString		m_szuplmtprice;		/*  상한가     */
	CString		m_szdnlmtprice;		/*  하한가     */
	CString		m_szprice;		/*  현재가     */
	CString		m_szofferho1;		/*  매도호가1     */
	CString		m_szofferho2;		/*  매도호가2     */
	CString		m_szofferho3;		/*  매도호가3     */
	CString		m_szbidho1;		/*  매수호가1     */
	CString		m_szbidho2;		/*  매수호가2     */
	CString		m_szbidho3;		/*  매수호가3     */

public:
	CEU_f0201()
	{
	}

	virtual void  Convert(EU_f0201_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, futcode);		/*  단축코드     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/*  기준가격     */
		DATA_MEMCOPY(sTmp, pInput, uplmtprice);		/*  상한가     */
		DATA_MEMCOPY(sTmp, pInput, dnlmtprice);		/*  하한가     */
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

