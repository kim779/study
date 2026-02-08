/*************************************************************************************/
/*  1. 프로그램ID : KB_s3301                                                         */
/*  2. 프로그램명 : 주식선물(호가)                                                   */
/*  3. 서비스명   : KB_s3301                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_s3301_H__
#define __KB_TRAN_KB_s3301_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_s3301"
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
}KB_s3301_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char KB_s3301_OutRec1_count[4];
//typedef struct
//{
//	char		futcode[8];		/*  단축코드     */
//	char		expcode[12];		/*종목코드     */
//	char		seqno[3];		/*  종목SEQ번호     */
//	char		hname[50];		/*한글종목명     */
//	char		ename[50];		/*영문종목명     */
//	char		sname[25];		/*축약종목명     */
//	char		name[4];		/*  종목명     */
//	char		listdate[8];		/*  상장일     */
//	char		futgubun[1];		/*  최근월물구분     */
//	char		listdatecnt[3];		/*  상장일수     */
//	char		jandatecnt[3];		/*  잔존일수     */
//	char		lastdate[8];		/*  최종거래일     */
//	char		lastdategubun[1];		/*  최종거래일여부     */
//	char		divideratio[13];		/*  배당액지수미래가치     */
//	char		recpricegubun[1];		/*  기준가격구분     */
//	char		recprice[7];		/*  기준가격     */
//	char		theorypricek[13];		/*  이론가(정산가)     */
//	char		theorypriceg[13];		/*  이론가(기준가)     */
//	char		cdratio[6];		/*  CD금리     */
//	char		uplmtprice[7];		/*  상한가     */
//	char		dnlmtprice[7];		/*  하한가     */
//	char		jnilsetgubun[1];		/*  전일정산가구분     */
//	char		jnilsetprice[16];		/*  전일정산가     */
//	char		jnilclosegubun[1];		/*  전일종가구분     */
//	char		jnilclose[7];		/*  전일종가     */
//	char		jnilvolume[8];		/*  전일체결수량     */
//	char		jnilgmvolume[8];		/*  전일근월물체결수량     */
//	char		jnilwmvolume[8];		/*  전일원월물체결수량     */
//	char		jnilvalue[15];		/*  전일체결대금     */
//	char		jnilgmvalue[15];		/*  전일근월물거래대금     */
//	char		jnilwmvalue[15];		/*  전일원월물거래대금     */
//	char		jnilopenyak[8];		/*  전일미결제약정수량     */
//	char		listhprice[7];		/*  상장최고가     */
//	char		listhdate[8];		/*  상장최고가일     */
//	char		listlprice[7];		/* 상장최저가     */
//	char		listldate[8];		/*  상장최저가일     */
//	char		ipwhedate[8];		/*  입회일자     */
//	char		mrkgubun[1];		/*  시장가허용구분     */
//	char		jogengubun[1];		/*  조건부지정가허용구분     */
//	char		choiygubun[1];		/*  최유리지정가허용구분     */
//	char		lastprice[16];		/*  최종결제가격     */
//	char		lastpricegubun[1];		/*  최종결제가격구분     */
//	char		cbhprice[7];		/*  CB적용상한가     */
//	char		cblprice[7];		/*  CB적용하한가     */
//	char		jnilmmchetime[8];		/*  전일최종매매체결시각     */
//	char		jnilspgmclose[7];		/*  전일스프레드근월물종가     */
//	char		jnilspwmclose[7];		/*  전일스프레드원월물종가     */
//	char		gexpcode[12];		/*  스프레드근월물표준코드     */
//	char		wexpcode[12];		/*  스프레드원월물표준코드     */
//	char		tradeunit[16];		/*  거래단위     */
//	char		mktmakegubun[1];		/*  시장조성종목여부     */
//	char		adjustgubun[1];		/*  조정구분     */
//	char		stopgubun[1];		/*  거래정지여부     */
//	char		settletype[1];		/*  결제방법     */
//	char		mmchetype[1];		/*  매매체결방법     */
//	char		itemcode[12];		/*  대상주식코드     */
//	char		expname[20];		/*  표준종목명     */
//	char		bdate[8];		/*  최종수신일자     */
//	char		bjandatecnt[3];		/*  영업일잔존일수     */
//	char		chetime[10];		/*  체결시간     */
//	char		price[7];		/*  현재가(SIGN포함)     */
//	char		open[7];		/*  시가(SIGN포함)     */
//	char		high[7];		/*  고가(SIGN포함)     */
//	char		low[7];		/*  저가(SIGN포함)     */
//	char		sign[1];		/*  전일대비구분     */
//	char		change[7];		/*전일대비　     */
//	char		volume[12];		/*누적거래량     */
//	char		value[15];		/*누적거래대금     */
//	char		cvolume[6];		/*건별체결수량     */
//	char		cgubun[1];		/*체결구분     */
//	char		mdvolume[7];		/*  매도누적거래량     */
//	char		msvolume[7];		/*  매수누적거래량     */
//	char		mdchecnt[6];		/*  매도누적체결건수     */
//	char		mschecnt[6];		/*  매수누적체결건수     */
//	char		gmprice[7];		/*  근월물의제약정가격     */
//	char		wmprice[7];		/*  원월물의제약정가격     */
//	char		gmvolume[10];		/*  근월물체결수량     */
//	char		wmvolume[10];		/*  원월물체결수량     */
//	char		gmvalue[15];		/*  근월물거래대금     */
//	char		wmvalue[15];		/*  원월물거래대금     */
//	char		mdvolume_h[10];		/*  매도체결수량(호가기준)     */
//	char		msvolume_h[10];		/*  매수체결수량(호가기준)     */
//	char		hotime[10];		/*  호가수신시간     */
//	char		offerho1[8];		/*매도호가1     */
//	char		bidho1[8];		/*  매수호가1     */
//	char		offerrem1[6];		/*매도호가수량1     */
//	char		bidrem1[6];		/*  매수호가수량1     */
//	char		offercnt1[4];		/*매도호가건수1     */
//	char		bidcnt1[4];		/*  매수호가건수1     */
//	char		offerremcha1[6];		/*직전매도대비수량1     */
//	char		bidremcha1[6];		/*  직전매수대비수량1     */
//	char		offercntcha1[4];		/*직전매도대비건수1     */
//	char		bidcntcha1[4];		/*  직전매수대비건수1     */
//	char		offerho2[8];		/*매도호가2     */
//	char		bidho2[8];		/*  매수호가2     */
//	char		offerrem2[6];		/*매도호가수량2     */
//	char		bidrem2[6];		/*  매수호가수량2     */
//	char		offercnt2[4];		/*매도호가건수2     */
//	char		bidcnt2[4];		/*  매수호가건수2     */
//	char		offerremcha2[6];		/*직전매도대비수량2     */
//	char		bidremcha2[6];		/*  직전매수대비수량2     */
//	char		offercntcha2[4];		/*직전매도대비건수2     */
//	char		bidcntcha2[4];		/*  직전매수대비건수2     */
//	char		offerho3[8];		/*매도호가3     */
//	char		bidho3[8];		/*  매수호가3     */
//	char		offerrem3[6];		/*매도호가수량3     */
//	char		bidrem3[6];		/*  매수호가수량3     */
//	char		offercnt3[4];		/*매도호가건수3     */
//	char		bidcnt3[4];		/*  매수호가건수3     */
//	char		offerremcha3[6];		/*직전매도대비수량3     */
//	char		bidremcha3[6];		/*  직전매수대비수량3     */
//	char		offercntcha3[4];		/*직전매도대비건수3     */
//	char		bidcntcha3[4];		/*  직전매수대비건수3     */
//	char		offerho4[8];		/*매도호가4     */
//	char		bidho4[8];		/*  매수호가4     */
//	char		offerrem4[6];		/*매도호가수량4     */
//	char		bidrem4[6];		/*  매수호가수량4     */
//	char		offercnt4[4];		/*매도호가건수4     */
//	char		bidcnt4[4];		/*  매수호가건수4     */
//	char		offerremcha4[6];		/*직전매도대비수량4     */
//	char		bidremcha4[6];		/*  직전매수대비수량4     */
//	char		offercntcha4[4];		/*직전매도대비건수4     */
//	char		bidcntcha4[4];		/*  직전매수대비건수4     */
//	char		offerho5[8];		/*매도호가5     */
//	char		bidho5[8];		/*  매수호가5     */
//	char		offerrem5[6];		/*매도호가수량5     */
//	char		bidrem5[6];		/*  매수호가수량5     */
//	char		offercnt5[4];		/*매도호가건수5     */
//	char		bidcnt5[4];		/*  매수호가건수5     */
//	char		offerremcha5[6];		/*직전매도대비수량5     */
//	char		bidremcha5[6];		/*  직전매수대비수량5     */
//	char		offercntcha5[4];		/*직전매도대비건수5     */
//	char		bidcntcha5[4];		/*  직전매수대비건수5     */
//	char		offerho6[8];		/*매도호가6     */
//	char		bidho6[8];		/*  매수호가6     */
//	char		offerrem6[6];		/*매도호가수량6     */
//	char		bidrem6[6];		/*  매수호가수량6     */
//	char		offercnt6[4];		/*매도호가건수6     */
//	char		bidcnt6[4];		/*  매수호가건수6     */
//	char		offerremcha6[6];		/*직전매도대비수량6     */
//	char		bidremcha6[6];		/*  직전매수대비수량6     */
//	char		offercntcha6[4];		/*직전매도대비건수6     */
//	char		bidcntcha6[4];		/*  직전매수대비건수6     */
//	char		offerho7[8];		/*매도호가7     */
//	char		bidho7[8];		/*  매수호가7     */
//	char		offerrem7[6];		/*매도호가수량7     */
//	char		bidrem7[6];		/*  매수호가수량7     */
//	char		offercnt7[4];		/*매도호가건수7     */
//	char		bidcnt7[4];		/*  매수호가건수7     */
//	char		offerremcha7[6];		/*직전매도대비수량7     */
//	char		bidremcha7[6];		/*  직전매수대비수량7     */
//	char		offercntcha7[4];		/*직전매도대비건수7     */
//	char		bidcntcha7[4];		/*  직전매수대비건수7     */
//	char		offerho8[8];		/*매도호가8     */
//	char		bidho8[8];		/*  매수호가8     */
//	char		offerrem8[6];		/*매도호가수량8     */
//	char		bidrem8[6];		/*  매수호가수량8     */
//	char		offercnt8[4];		/*매도호가건수8     */
//	char		bidcnt8[4];		/*  매수호가건수8     */
//	char		offerremcha8[6];		/*직전매도대비수량8     */
//	char		bidremcha8[6];		/*  직전매수대비수량8     */
//	char		offercntcha8[4];		/*직전매도대비건수8     */
//	char		bidcntcha8[4];		/*  직전매수대비건수8     */
//	char		offerho9[8];		/*매도호가9     */
//	char		bidho9[8];		/*  매수호가9     */
//	char		offerrem9[6];		/*매도호가수량9     */
//	char		bidrem9[6];		/*  매수호가수량9     */
//	char		offercnt9[4];		/*매도호가건수9     */
//	char		bidcnt9[4];		/*  매수호가건수9     */
//	char		offerremcha9[6];		/*직전매도대비수량9     */
//	char		bidremcha9[6];		/*  직전매수대비수량9     */
//	char		offercntcha9[4];		/*직전매도대비건수9     */
//	char		bidcntcha9[4];		/*  직전매수대비건수9     */
//	char		offerho10[8];		/*매도호가10     */
//	char		bidho10[8];		/*  매수호가10     */
//	char		offerrem10[6];		/*매도호가수량10     */
//	char		bidrem10[6];		/*  매수호가수량10     */
//	char		offercnt10[4];		/*매도호가건수10     */
//	char		bidcnt10[4];		/*  매수호가건수10     */
//	char		offerremcha10[6];		/*직전매도대비수량10     */
//	char		bidremcha10[6];		/*  직전매수대비수량10     */
//	char		offercntcha10[4];		/*직전매도대비건수10     */
//	char		bidcntcha10[4];		/*  직전매수대비건수10     */
//	char		totofferrem[7];		/*매도호가총수량     */
//	char		totbidrem[7];		/*  매수호가총수량     */
//	char		totoffercnt[5];		/*매도호가총건수     */
//	char		totbidcnt[5];		/*  매수호가총건수     */
//	char		totofferremcha[7];		/*직전매도대비총수량     */
//	char		totbidremcha[7];		/*  직전매수대비총수량     */
//	char		totoffercntcha[5];		/*직전매도대비총건수     */
//	char		totbidcntcha[5];		/*  직전매수대비총건수     */
//	char		jgubun[2];		/*  장운용구분     */
//	char		yakdate[8];		/*  체결일자     */
//	char		mggubun[1];		/*  미결제약정구분     */
//	char		openyak[7];		/*  미결제약정수량     */
//	char		openyakcha[7];		/*  미결제약정수량직전대비     */
//	char		openopenyak[7];		/*  미결제약정시가     */
//	char		highopenyak[7];		/*  미결제약정고가     */
//	char		lowopenyak[7];		/*  미결제약정저가     */
//	char		jsprice[16];		/*정산가격     */
//	char		jsgubun[1];		/*  정산가구분     */
//	char		baseprice[6];		/*  대상종목현재가     */
//	char		theoryprice[6];		/*  이론가(현재가)     */
//	char		kasis[6];		/*  괴리율     */
//	char		sbasis[6];		/*  시장BASIS     */
//	char		ibasis[6];		/*  이론BASIS     */
//	char		drate[6];		/*  등락율     */
//	char		b_volrate[8];		/*  전일거래량비     */
//	char		svolume[8];		/*  호가순매수     */
//	char		pivot[8];		/*  피봇     */
//	char		resist1[8];		/*  1차저항     */
//	char		support1[8];		/*1차지지     */
//	char		resist2[8];		/*  2차저항     */
//	char		support2[8];		/*  2차지지     */
//	char		ho_drate1[6];		/*  매도호가등락율1     */
//	char		ho_drate2[6];		/*  매도호가등락율2     */
//	char		ho_drate3[6];		/*  매도호가등락율3     */
//	char		ho_drate4[6];		/*  매도호가등락율4     */
//	char		ho_drate5[6];		/*  매도호가등락율5     */
//	char		ho_drate6[6];		/*  매도호가등락율6     */
//	char		ho_drate7[6];		/*  매도호가등락율7     */
//	char		ho_drate8[6];		/*  매도호가등락율8     */
//	char		ho_drate9[6];		/*  매도호가등락율9     */
//	char		ho_drate10[6];		/*매도호가등락율10     */
//	char		hb_drate1[6];		/*  매수호가등락율1     */
//	char		hb_drate2[6];		/*  매수호가등락율2     */
//	char		hb_drate3[6];		/*  매수호가등락율3     */
//	char		hb_drate4[6];		/*  매수호가등락율4     */
//	char		hb_drate5[6];		/*  매수호가등락율5     */
//	char		hb_drate6[6];		/*  매수호가등락율6     */
//	char		hb_drate7[6];		/*  매수호가등락율7     */
//	char		hb_drate8[6];		/*  매수호가등락율8     */
//	char		hb_drate9[6];		/*  매수호가등락율9     */
//	char		hb_drate10[6];		/*  매수호가등락율10     */
//}KB_s3301_OutRec1;

typedef struct _KB_s3301_OutRec1 {
	char		futcode[8];		/*  단축코드     */
	char		recprice[7];		/*  기준가격     */
	char		uplmtprice[7];		/*  상한가     */
	char		dnlmtprice[7];		/*  하한가     */
	char		price[7];		/*  현재가     */
	char		offerho1[8];		/*  매도호가1     */
	char		offerho2[8];		/*  매도호가2     */
	char		offerho3[8];		/*  매도호가3     */
	char		bidho1[8];		/*  매수호가1     */
	char		bidho2[8];		/*  매수호가2     */
	char		bidho3[8];		/*  매수호가3     */
} KB_s3301_OutRec1;

/*************************************************************************************/
class CKB_s3301
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
	CKB_s3301()
	{
	}

	virtual void  Convert(KB_s3301_OutRec1* pInput)
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

