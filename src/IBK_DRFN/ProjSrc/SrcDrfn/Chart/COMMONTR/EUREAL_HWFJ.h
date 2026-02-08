#if !defined(AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_)
#define AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     해외
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/*  [데이터구분]			*/
	char		symbol[10];		/*  [심볼코드]				*/
	char		date[8];		/*  [일자(현지일자)]		*/
	char		time[10];		/*  [시간(현지시간)]		*/
	char		kodate[8];		/*  [한국일자]				*/
	char		kotime[10];		/*  [한국시간]				*/
	char		open[8];		/*  [시가]					*/
	char		high[8];		/*  [고가]					*/
	char		low[8];			/*  [저가]					*/
	char		price[8];		/*  [현재가]				*/
	char		sign[1];		/*  [전일대비구분]			*/
	char		change[8];		/*  [전일대비]				*/
	char		drate[6];		/*  [등락율]				*/
	char		bid[8];			/*  [매수호가]				*/
	char		bidrem[9];		/*  [매수잔량]				*/
	char		offer[8];		/*  [매도호가]				*/
	char		offerrem[9];	/*  [매도잔량]				*/
	char		cvolume[9];		/*  [체결수량]				*/
	char		volume[9];		/*  [거래량]				*/
} stEUREAL_HWFJ;

class CEUREAL_HWFJ
{
public: //Variable
	CString		m_szdgubun;		/*  [데이터구분]			*/
	CString		m_szsymbol;		/*  [심볼코드]				*/
	CString		m_szdate;		/*  [일자(현지일자)]		*/
	CString		m_sztime;		/*  [시간(현지시간)]		*/
	CString		m_szkodate;		/*  [한국일자]				*/
	CString		m_szkotime;		/*  [한국시간]				*/
	CString		m_szopen;		/*  [시가]					*/
	CString		m_szhigh;		/*  [고가]					*/
	CString		m_szlow;		/*  [저가]					*/
	CString		m_szprice;		/*  [현재가]				*/
	CString		m_szsign;		/*  [전일대비구분]			*/
	CString		m_szchange;		/*  [전일대비]				*/
	CString		m_szdrate;		/*  [등락율]				*/
	CString		m_szbid;		/*  [매수호가]				*/
	CString		m_szbidrem;		/*  [매수잔량]				*/
	CString		m_szoffer;		/*  [매도호가]				*/
	CString		m_szofferrem;	/*  [매도잔량]				*/
	CString		m_szcvolume;	/*  [체결수량]				*/
	CString		m_szvolume;		/*  [거래량]				*/

public:
	CEUREAL_HWFJ()
	{
	}

	virtual void  Convert(stEUREAL_HWFJ* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);		/*  [데이터구분]			*/
		DATA_MEMCOPY(sTmp, pInput, symbol);		/*  [심볼코드]				*/
		DATA_MEMCOPY(sTmp, pInput, date);		/*  [일자(현지일자)]		*/
		DATA_MEMCOPY(sTmp, pInput, time);		/*  [시간(현지시간)]		*/
		DATA_MEMCOPY(sTmp, pInput, kodate);		/*  [한국일자]				*/
		DATA_MEMCOPY(sTmp, pInput, kotime);		/*  [한국시간]				*/
		DATA_MEMCOPY(sTmp, pInput, open);		/*  [시가]					*/
		DATA_MEMCOPY(sTmp, pInput, high);		/*  [고가]					*/
		DATA_MEMCOPY(sTmp, pInput, low);		/*  [저가]					*/
		DATA_MEMCOPY(sTmp, pInput, price);		/*  [현재가]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);		/*  [전일대비구분]			*/
		DATA_MEMCOPY(sTmp, pInput, change);		/*  [전일대비]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);		/*  [등락율]				*/
		DATA_MEMCOPY(sTmp, pInput, bid);		/*  [매수호가]				*/
		DATA_MEMCOPY(sTmp, pInput, bidrem);		/*  [매수잔량]				*/
		DATA_MEMCOPY(sTmp, pInput, offer);		/*  [매도호가]				*/
		DATA_MEMCOPY(sTmp, pInput, offerrem);	/*  [매도잔량]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);	/*  [체결수량]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);		/*  [거래량]				*/
	}
};

#endif // AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_
