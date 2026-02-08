#if !defined(AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_)
#define AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     주식
//--------------------------------------------------------------------

typedef struct {
	char dgubun[4];			/*  [Data 구분:]			*/
	char kshcode[6];		/*  [단축코드]				*/
	char chetime[6];		/*  [체결시간]				*/
	char sign[1];			/*  [1:상한]				*/
	char change[8];			/*  [전일대비,   원]		*/
	char drate[6];			/*  [등락율]				*/
	char price[8];			/*  [현재가,     원]		*/
	char opentime[6];		/*  [시가시간]				*/
	char open[8];			/*  [시가,       원]		*/
	char hightime[6];		/*  [고가시간]				*/
	char high[8];			/*  [고가,       원]		*/
	char lowtime[6];		/*  [저가시간]				*/
	char low[8];			/*  [저가,       원]		*/
	char openchange[8];		/*  [시가전일대비]			*/
	char opendrate[6];		/*  [시가전일대비등락율]    */
	char highchange[8];		/*  [고가전일대비]			*/
	char highdrate[6];		/*  [고가전일대비등락율]    */
	char lowchange[8];		/*  [저가전일대비]			*/
	char lowdrate[6];		/*  [저가전일대비등락율]    */
	char cgubun[1];			/*  [체결구분]				*/
	char cvolume[8];		/*  [체결량, 주]			*/
	char volume[12];		/*  [누적거래량, 주]		*/
	char volchange[12];		/*  [거래량전일대비]		*/
	char voldrate[8];		/*  [거래량등락율]			*/
	char voltrate[6];		/*  [거래량회전율]			*/
	char value[12];			/*  [누적거래대금]			*/
	char valchange[12];		/*  [거래대금전일대비]      */
	char valdrate[8];		/*  [거래대금등락율]		*/
	char mdvolume[12];		/*  [매도누적 체결량]		*/
	char mdchecnt[8];		/*  [매도누적 체결건수]     */
	char msvolume[12];		/*  [매수누적 체결량]		*/
	char mschecnt[8];		/*  [매수누적 체결건수]     */
	char cpower[9];			/*  [체결강도]				*/
	char w_avrg[8];			/*  [가중 평균가]			*/
	char offerho1[8];		/*  [매도호가, 원]			*/
	char bidho1[8];			/*  [매수호가, 원]			*/
	char status[2];			/*  [장구분 정보]			*/
	char jnilvolume[12];	/*  [전일동시간대거래량]    */
	char shcode[6];			/*  [단축코드]				*/
} stEUREAL_KXH3;	//21

class CEUREAL_KXH3
{
public: //Variable
	CString m_szdgubun;			/*  [Data 구분:]			*/
	CString m_szkshcode;		/*  [단축코드]				*/
	CString m_szchetime;		/*  [체결시간]				*/
	CString m_szsign;			/*  [1:상한]				*/
	CString m_szchange;			/*  [전일대비,   원]		*/
	CString m_szdrate;			/*  [등락율]				*/
	CString m_szprice;			/*  [현재가,     원]		*/
	CString m_szopentime;		/*  [시가시간]				*/
	CString m_szopen;			/*  [시가,       원]		*/
	CString m_szhightime;		/*  [고가시간]				*/
	CString m_szhigh;			/*  [고가,       원]		*/
	CString m_szlowtime;		/*  [저가시간]				*/
	CString m_szlow;			/*  [저가,       원]		*/
	CString m_szopenchange;		/*  [시가전일대비]			*/
	CString m_szopendrate;		/*  [시가전일대비등락율]    */
	CString m_szhighchange;		/*  [고가전일대비]			*/
	CString m_szhighdrate;		/*  [고가전일대비등락율]    */
	CString m_szlowchange;		/*  [저가전일대비]			*/
	CString m_szlowdrate;		/*  [저가전일대비등락율]    */
	CString m_szcgubun;			/*  [체결구분]				*/
	CString m_szcvolume;		/*  [체결량, 주]			*/
	CString m_szvolume;			/*  [누적거래량, 주]		*/
	CString m_szvolchange;		/*  [거래량전일대비]		*/
	CString m_szvoldrate;		/*  [거래량등락율]			*/
	CString m_szvoltrate;		/*  [거래량회전율]			*/
	CString m_szvalue;			/*  [누적거래대금]			*/
	CString m_szvalchange;		/*  [거래대금전일대비]      */
	CString m_szvaldrate;		/*  [거래대금등락율]		*/
	CString m_szmdvolume;		/*  [매도누적 체결량]		*/
	CString m_szmdchecnt;		/*  [매도누적 체결건수]     */
	CString m_szmsvolume;		/*  [매수누적 체결량]		*/
	CString m_szmschecnt;		/*  [매수누적 체결건수]     */
	CString m_szcpower;			/*  [체결강도]				*/
	CString m_szw_avrg;			/*  [가중 평균가]			*/
	CString m_szofferho1;		/*  [매도호가, 원]			*/
	CString m_szbidho1;			/*  [매수호가, 원]			*/
	CString m_szstatus;			/*  [장구분 정보]			*/
	CString m_szjnilvolume;		/*  [전일동시간대거래량]    */
	CString m_szshcode;			/*  [단축코드]				*/

public:
	CEUREAL_KXH3()
	{
	}

	virtual void  Convert(stEUREAL_KXH3* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data 구분:]			*/
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [단축코드]				*/
		DATA_MEMCOPY(sTmp, pInput, chetime);		/*  [체결시간]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [1:상한]				*/
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [전일대비,   원]		*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [등락율]				*/
		DATA_MEMCOPY(sTmp, pInput, price);			/*  [현재가,     원]		*/
		DATA_MEMCOPY(sTmp, pInput, opentime);		/*  [시가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, open);			/*  [시가,       원]		*/
		DATA_MEMCOPY(sTmp, pInput, hightime);		/*  [고가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, high);			/*  [고가,       원]		*/
		DATA_MEMCOPY(sTmp, pInput, lowtime);		/*  [저가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, low);			/*  [저가,       원]		*/
		DATA_MEMCOPY(sTmp, pInput, openchange);		/*  [시가전일대비]			*/
		DATA_MEMCOPY(sTmp, pInput, opendrate);		/*  [시가전일대비등락율]    */
		DATA_MEMCOPY(sTmp, pInput, highchange);		/*  [고가전일대비]			*/
		DATA_MEMCOPY(sTmp, pInput, highdrate);		/*  [고가전일대비등락율]    */
		DATA_MEMCOPY(sTmp, pInput, lowchange);		/*  [저가전일대비]			*/
		DATA_MEMCOPY(sTmp, pInput, lowdrate);		/*  [저가전일대비등락율]    */
		DATA_MEMCOPY(sTmp, pInput, cgubun);			/*  [체결구분]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [체결량, 주]			*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [누적거래량, 주]		*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [거래량전일대비]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [거래량등락율]			*/
		DATA_MEMCOPY(sTmp, pInput, voltrate);		/*  [거래량회전율]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [누적거래대금]			*/
		DATA_MEMCOPY(sTmp, pInput, valchange);		/*  [거래대금전일대비]      */
		DATA_MEMCOPY(sTmp, pInput, valdrate);		/*  [거래대금등락율]		*/
		DATA_MEMCOPY(sTmp, pInput, mdvolume);		/*  [매도누적 체결량]		*/
		DATA_MEMCOPY(sTmp, pInput, mdchecnt);		/*  [매도누적 체결건수]     */
		DATA_MEMCOPY(sTmp, pInput, msvolume);		/*  [매수누적 체결량]		*/
		DATA_MEMCOPY(sTmp, pInput, mschecnt);		/*  [매수누적 체결건수]     */
		DATA_MEMCOPY(sTmp, pInput, cpower);			/*  [체결강도]				*/
		DATA_MEMCOPY(sTmp, pInput, w_avrg);			/*  [가중 평균가]			*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [매도호가, 원]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [매수호가, 원]			*/
		DATA_MEMCOPY(sTmp, pInput, status);			/*  [장구분 정보]			*/
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/*  [전일동시간대거래량]    */
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [단축코드]				*/
	}
};

#endif // AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_
