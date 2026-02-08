#if !defined(AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_)
#define AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     업종
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/*  [Data 구분]				*/
	char		kupcode[3];		/*  [업종코드]				*/
	char		time[6];		/*  [시간]					*/
	char		jisu[8];		/*  [지수]					*/
	char		sign[1];		/*  [2:상승 3:보합 5:하락]  */
	char		change[8];		/*  [전일비]				*/
	char		drate[6];		/*  [등락율]				*/
	char		cvolume[8];		/*  [체결량]				*/
	char		volume[8];		/*  [거래량]				*/
	char		volchange[8];	/*  [거래량전일대비]		*/
	char		voldrate[8];	/*  [거래량등락율]			*/
	char		value[8];		/*  [거래대금]				*/
	char		upjo[4];		/*  [상한 종목수]			*/
	char		highjo[4];		/*  [상승 종목수]			*/
	char		unchgjo[4];		/*  [보합 종목수]			*/
	char		lowjo[4];		/*  [하락 종목수]			*/
	char		downjo[4];		/*  [하한 종목수]			*/
	char		upjrate[6];		/*  [상승 종목비율]			*/
	char		openjisu[8];	/*  [시가지수]				*/
	char		opentime[6];	/*  [시가시간]				*/
	char		highjisu[8];	/*  [고가지수]				*/
	char		hightime[6];	/*  [고가시간]				*/
	char		lowjisu[8];		/*  [저가지수]				*/
	char		lowtime[6];		/*  [저가시간]				*/
	char		frgsvolume[8];	/*  [외인순매수수량]		*/
	char		orgsvolume[8];	/*  [기관순매수수량]		*/
	char		frgsvalue[10];	/*  [외인순매수금액]		*/
	char		orgsvalue[10];	/*  [기관순매수금액]		*/
	char		upcode[3];		/*  [업종코드]				*/
} stEUREAL_IXIJ;	//101

class CEUREAL_IXIJ
{
public: //Variable
	CString m_szdgubun;			/*  [Data 구분]				*/
	CString m_szkupcode;		/*  [업종코드]				*/
	CString m_sztime;			/*  [시간]					*/
	CString m_szjisu;			/*  [지수]					*/
	CString m_szsign;			/*  [2:상승 3:보합 5:하락]  */
	CString m_szchange;			/*  [전일비]				*/
	CString m_szdrate;			/*  [등락율]				*/
	CString m_szcvolume;		/*  [체결량]				*/
	CString m_szvolume;			/*  [거래량]				*/
	CString m_szvolchange;		/*  [거래량전일대비]		*/
	CString m_szvoldrate;		/*  [거래량등락율]			*/
	CString m_szvalue;			/*  [거래대금]				*/
	CString m_szupjo;			/*  [상한 종목수]			*/
	CString m_szhighjo;			/*  [상승 종목수]			*/
	CString m_szunchgjo;		/*  [보합 종목수]			*/
	CString m_szlowjo;			/*  [하락 종목수]			*/
	CString m_szdownjo;			/*  [하한 종목수]			*/
	CString m_szupjrate;		/*  [상승 종목비율]			*/
	CString m_szopenjisu;		/*  [시가지수]				*/
	CString m_szopentime;		/*  [시가시간]				*/
	CString m_szhighjisu;		/*  [고가지수]				*/
	CString m_szhightime;		/*  [고가시간]				*/
	CString m_szlowjisu;		/*  [저가지수]				*/
	CString m_szlowtime;		/*  [저가시간]				*/
	CString m_szfrgsvolume;		/*  [외인순매수수량]		*/
	CString m_szorgsvolume;		/*  [기관순매수수량]		*/
	CString m_szfrgsvalue;		/*  [외인순매수금액]		*/
	CString m_szorgsvalue;		/*  [기관순매수금액]		*/
	CString m_szupcode;			/*  [업종코드]				*/

public:
	CEUREAL_IXIJ()
	{
	}

	virtual void  Convert(stEUREAL_IXIJ* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data 구분]				*/
		DATA_MEMCOPY(sTmp, pInput, kupcode);		/*  [업종코드]				*/
		DATA_MEMCOPY(sTmp, pInput, time);			/*  [시간]					*/
		DATA_MEMCOPY(sTmp, pInput, jisu);			/*  [지수]					*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [2:상승 3:보합 5:하락]  */
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [전일비]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [등락율]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [체결량]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [거래량]				*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [거래량전일대비]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [거래량등락율]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [거래대금]				*/
		DATA_MEMCOPY(sTmp, pInput, upjo);			/*  [상한 종목수]			*/
		DATA_MEMCOPY(sTmp, pInput, highjo);			/*  [상승 종목수]			*/
		DATA_MEMCOPY(sTmp, pInput, unchgjo);		/*  [보합 종목수]			*/
		DATA_MEMCOPY(sTmp, pInput, lowjo);			/*  [하락 종목수]			*/
		DATA_MEMCOPY(sTmp, pInput, downjo);			/*  [하한 종목수]			*/
		DATA_MEMCOPY(sTmp, pInput, upjrate);		/*  [상승 종목비율]			*/
		DATA_MEMCOPY(sTmp, pInput, openjisu);		/*  [시가지수]				*/
		DATA_MEMCOPY(sTmp, pInput, opentime);		/*  [시가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, highjisu);		/*  [고가지수]				*/
		DATA_MEMCOPY(sTmp, pInput, hightime);		/*  [고가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, lowjisu);		/*  [저가지수]				*/
		DATA_MEMCOPY(sTmp, pInput, lowtime);		/*  [저가시간]				*/
		DATA_MEMCOPY(sTmp, pInput, frgsvolume);		/*  [외인순매수수량]		*/
		DATA_MEMCOPY(sTmp, pInput, orgsvolume);		/*  [기관순매수수량]		*/
		DATA_MEMCOPY(sTmp, pInput, frgsvalue);		/*  [외인순매수금액]		*/
		DATA_MEMCOPY(sTmp, pInput, orgsvalue);		/*  [기관순매수금액]		*/
		DATA_MEMCOPY(sTmp, pInput, upcode);			/*  [업종코드]				*/
	}
};

#endif // AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_
