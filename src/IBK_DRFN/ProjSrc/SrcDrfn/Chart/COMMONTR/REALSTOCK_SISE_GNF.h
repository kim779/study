#include "Data_Common.h"

//--------------------------------------------------------------------
//     주식
//--------------------------------------------------------------------
typedef struct 
{
	char		dgubun[4];		/*  [Data 구분: CAS1]     */
	char		jmcode[10];		/*  [종목코드]     */
	char		chetime[6];		/*  [체결시간 ]     */
	char		open[10];		/*  [시가]     */
	char		high[10];		/*  [고가]     */
	char		low[10];		/*  [저가]     */
	char		price[10];		/*  [체결가]     */
	char		sign[1];		/*  [전일대비구분]     */
	char		change[10];		/*  [전일대비]     */
	char		drate[6];		/*  [전일대비 등락율]     */
	char		presign[1];		/*  [직전대비구분]     */
	char		cvolume[6];		/*  [체결량]     */
	char		cgubun[1];		/*  [체결구분 ]     */
	char		volume[10];		/*  [누적거래량]     */
	char		value[12];		/*  [누적거래대금]     */
	char		ssvolume[10];	/*  [상승거래량]     */
	char		hrvolume[10];	/*  [하락거래량]     */
	char		sscnt[6];		/*  [상승건수]     */
	char		hrcnt[6];		/*  [하락건수]     */
	char		offerho1[10];	/*  [매도호가]     */
	char		bidho1[10];		/*  [매수호가]     */
	char		offerrem1[8];	/*  [매도잔량]     */
	char		bidrem1[8];		/*  [매수잔량]     */
	char		seqno[8];		/*  [거래번호]     */
	char		price_d[15];	/*  [진법 변환전 체결가]     */
	char		bday[8];		/*  [영업일 ]     */
	char		kjmcode[10];	/*  [종목코드]     */
	char		chetime_g[6];	/*  [기준 체결시간 ]     */
} STREAL_GNF_STOCK_SISE;



class CREAL_GNF_STOCK_SISE
{
public: //Variable
	CString m_szdgubun;		/*  [Data 구분: CAS1]     */
	CString m_szjmcode;		/*  [종목코드]     */
	CString m_szchetime;		/*  [체결시간 ]     */
	CString m_szopen;		/*  [시가]     */
	CString m_szhigh;		/*  [고가]     */
	CString m_szlow;		/*  [저가]     */
	CString m_szprice;		/*  [체결가]     */
	CString m_szsign;		/*  [전일대비구분]     */
	CString m_szchange;		/*  [전일대비]     */
	CString m_szdrate;		/*  [전일대비 등락율]     */
	CString m_szpresign;		/*  [직전대비구분]     */
	CString m_szcvolume;		/*  [체결량]     */
	CString m_szcgubun;		/*  [체결구분 ]     */
	CString m_szvolume;		/*  [누적거래량]     */
	CString m_szvalue;		/*  [누적거래대금]     */
	CString m_szssvolume;	/*  [상승거래량]     */
	CString m_szhrvolume;	/*  [하락거래량]     */
	CString m_szsscnt;		/*  [상승건수]     */
	CString m_szhrcnt;		/*  [하락건수]     */
	CString m_szofferho1;	/*  [매도호가]     */
	CString m_szbidho1;		/*  [매수호가]     */
	CString m_szofferrem1;	/*  [매도잔량]     */
	CString m_szbidrem1;		/*  [매수잔량]     */
	CString m_szseqno;		/*  [거래번호]     */
	CString m_szprice_d;	/*  [진법 변환전 체결가]     */
	CString m_szbday;		/*  [영업일 ]     */
	CString m_szkjmcode;	/*  [종목코드]     */
	CString m_szchetime_g;	/*  [기준 체결시간 ]     */

public:
	CREAL_GNF_STOCK_SISE()
	{
	}

	virtual void  Convert(STREAL_GNF_STOCK_SISE* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);	
		DATA_MEMCOPY(sTmp, pInput, jmcode);	
		DATA_MEMCOPY(sTmp, pInput, chetime);	
		DATA_MEMCOPY(sTmp, pInput, open);	
		DATA_MEMCOPY(sTmp, pInput, high);	
		DATA_MEMCOPY(sTmp, pInput, low);	
		DATA_MEMCOPY(sTmp, pInput, price);	
		DATA_MEMCOPY(sTmp, pInput, sign);	
		DATA_MEMCOPY(sTmp, pInput, change);	
		DATA_MEMCOPY(sTmp, pInput, drate);	
		DATA_MEMCOPY(sTmp, pInput, presign);	
		DATA_MEMCOPY(sTmp, pInput, cvolume);	
		DATA_MEMCOPY(sTmp, pInput, cgubun);	
		DATA_MEMCOPY(sTmp, pInput, volume);	
		DATA_MEMCOPY(sTmp, pInput, value);	
		DATA_MEMCOPY(sTmp, pInput, ssvolume);
		DATA_MEMCOPY(sTmp, pInput, hrvolume);
		DATA_MEMCOPY(sTmp, pInput, sscnt);	
		DATA_MEMCOPY(sTmp, pInput, hrcnt);	
		DATA_MEMCOPY(sTmp, pInput, offerho1);
		DATA_MEMCOPY(sTmp, pInput, bidho1);	
		DATA_MEMCOPY(sTmp, pInput, offerrem1);
		DATA_MEMCOPY(sTmp, pInput, bidrem1);	
		DATA_MEMCOPY(sTmp, pInput, seqno);	
		DATA_MEMCOPY(sTmp, pInput, price_d);
		DATA_MEMCOPY(sTmp, pInput, bday);	
		DATA_MEMCOPY(sTmp, pInput, kjmcode);
		DATA_MEMCOPY(sTmp, pInput, chetime_g);
	}
};


