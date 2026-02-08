#if !defined(AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_)
#define AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     선물
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [Data 구분]				*/
	char		kfutcode[8];		/*  [선물단축코드]			*/
	char		chetime[6];			/*  [체결시간]				*/
	char		sign[1];			/*  [2:상승 3:보합 5:하락]  */
	char		change[6];			/*  [전일대비]				*/
	char		drate[6];			/*  [등락율]				*/
	char		price[6];			/*  [현재가]				*/
	char		open[6];			/*  [시가]					*/
	char		high[6];			/*  [고가]					*/
	char		low[6];				/*  [저가]					*/
	char		cgubun[1];			/*  [체결구분]				*/
	char		cvolume[6];			/*  [체결량]				*/
	char		volume[12];			/*  [누적거래량]			*/
	char		volchange[12];		/*  [거래량전일대비]		*/
	char		voldrate[8];		/*  [거래량등락율]			*/
	char		value[12];			/*  [누적거래대금]			*/
	char		mdvolume[12];		/*  [매도누적 체결량]		*/
	char		mdchecnt[8];		/*  [매도누적 체결건수]     */
	char		msvolume[12];		/*  [매수누적 체결량]		*/
	char		mschecnt[8];		/*  [매수누적 체결건수]     */
	char		cpower[9];			/*  [체결강도]				*/
	char		offerho1[6];		/*  [매도호가1]				*/
	char		bidho1[6];			/*  [매수호가1]				*/
	char		openyak[8];			/*  [미결제약정수량]		*/
	char		openyakchange[8];	/*  [미결제약정전일대비]	*/
	char		k200jisu[6];		/*  [KOSPI200 지수]			*/
	char		theoryprice[6];		/*  [이론가]				*/
	char		grate[6];			/*  [괴리도]				*/
	char		kasis[6];			/*  [괴리율]				*/
	char		sbasis[6];			/*  [시장 BASIS]			*/
	char		ibasis[6];			/*  [이론 BASIS]			*/
	char		jgubun[2];			/*  [장운영 정보]			*/
	char		jnilvolume[12];		/*  [전일 동시간대 거래량]  */
	char		futcode[8];			/*  [단축코드]				*/
} stEUREAL_FFC0;	//65

class CEUREAL_FFC0
{
public: //Variable
	CString m_szdgubun;			/*  [Data 구분]				*/
	CString m_szkfutcode;		/*  [선물단축코드]			*/
	CString m_szchetime;		/*  [체결시간]				*/
	CString m_szsign;			/*  [2:상승 3:보합 5:하락]  */
	CString m_szchange;			/*  [전일대비]				*/
	CString m_szdrate;			/*  [등락율]				*/
	CString m_szprice;			/*  [현재가]				*/
	CString m_szopen;			/*  [시가]					*/
	CString m_szhigh;			/*  [고가]					*/
	CString m_szlow;			/*  [저가]					*/
	CString m_szcgubun;			/*  [체결구분]				*/
	CString m_szcvolume;		/*  [체결량]				*/
	CString m_szvolume;			/*  [누적거래량]			*/
	CString m_szvolchange;		/*  [거래량전일대비]		*/
	CString m_szvoldrate;		/*  [거래량등락율]			*/
	CString m_szvalue;			/*  [누적거래대금]			*/
	CString m_szmdvolume;		/*  [매도누적 체결량]		*/
	CString m_szmdchecnt;		/*  [매도누적 체결건수]     */
	CString m_szmsvolume;		/*  [매수누적 체결량]		*/
	CString m_szmschecnt;		/*  [매수누적 체결건수]     */
	CString m_szcpower;			/*  [체결강도]				*/
	CString m_szofferho1;		/*  [매도호가1]				*/
	CString m_szbidho1;			/*  [매수호가1]				*/
	CString m_szopenyak;		/*  [미결제약정수량]		*/
	CString m_szopenyakchange;	/*  [미결제약정전일대비]	*/
	CString m_szk200jisu;		/*  [KOSPI200 지수]			*/
	CString m_sztheoryprice;	/*  [이론가]				*/
	CString m_szgrate;			/*  [괴리도]				*/
	CString m_szkasis;			/*  [괴리율]				*/
	CString m_szsbasis;			/*  [시장 BASIS]			*/
	CString m_szibasis;			/*  [이론 BASIS]			*/
	CString m_szjgubun;			/*  [장운영 정보]			*/
	CString m_szjnilvolume;		/*  [전일 동시간대 거래량]  */
	CString m_szfutcode;		/*  [단축코드]				*/

public:
	CEUREAL_FFC0()
	{
	}

	virtual void  Convert(stEUREAL_FFC0* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data 구분]				*/
		DATA_MEMCOPY(sTmp, pInput, kfutcode);		/*  [선물단축코드]			*/
		DATA_MEMCOPY(sTmp, pInput, chetime);		/*  [체결시간]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [2:상승 3:보합 5:하락]  */
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [전일대비]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [등락율]				*/
		DATA_MEMCOPY(sTmp, pInput, price);			/*  [현재가]				*/
		DATA_MEMCOPY(sTmp, pInput, open);			/*  [시가]					*/
		DATA_MEMCOPY(sTmp, pInput, high);			/*  [고가]					*/
		DATA_MEMCOPY(sTmp, pInput, low);			/*  [저가]					*/
		DATA_MEMCOPY(sTmp, pInput, cgubun);			/*  [체결구분]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [체결량]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [누적거래량]			*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [거래량전일대비]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [거래량등락율]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [누적거래대금]			*/
		DATA_MEMCOPY(sTmp, pInput, mdvolume);		/*  [매도누적 체결량]		*/
		DATA_MEMCOPY(sTmp, pInput, mdchecnt);		/*  [매도누적 체결건수]     */
		DATA_MEMCOPY(sTmp, pInput, msvolume);		/*  [매수누적 체결량]		*/
		DATA_MEMCOPY(sTmp, pInput, mschecnt);		/*  [매수누적 체결건수]     */
		DATA_MEMCOPY(sTmp, pInput, cpower);			/*  [체결강도]				*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [매도호가1]				*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [매수호가1]				*/
		DATA_MEMCOPY(sTmp, pInput, openyak);		/*  [미결제약정수량]		*/
		DATA_MEMCOPY(sTmp, pInput, openyakchange);	/*  [미결제약정전일대비]	*/
		DATA_MEMCOPY(sTmp, pInput, k200jisu);		/*  [KOSPI200 지수]			*/
		DATA_MEMCOPY(sTmp, pInput, theoryprice);	/*  [이론가]				*/
		DATA_MEMCOPY(sTmp, pInput, grate);			/*  [괴리도]				*/
		DATA_MEMCOPY(sTmp, pInput, kasis);			/*  [괴리율]				*/
		DATA_MEMCOPY(sTmp, pInput, sbasis);			/*  [시장 BASIS]			*/
		DATA_MEMCOPY(sTmp, pInput, ibasis);			/*  [이론 BASIS]			*/
		DATA_MEMCOPY(sTmp, pInput, jgubun);			/*  [장운영 정보]			*/
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/*  [전일 동시간대 거래량]  */
		DATA_MEMCOPY(sTmp, pInput, futcode);		/*  [단축코드]				*/
	}
};

#endif // AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_
