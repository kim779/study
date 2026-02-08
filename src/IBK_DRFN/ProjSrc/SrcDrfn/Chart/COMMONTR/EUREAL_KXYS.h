#if !defined(AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_)
#define AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     예상체결
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [KRX 예상체결]     */
	char		kshcode[6];			/*  [단축코드]     */
	char		hotime[6];			/*  [호가시간 HHMMSS]     */
	char		yeprice[8];			/*  [예상체결가격]     */
	char		preysign[1];		/*  [예상체결가 대비구분]     */
	char		preychange[8];		/*  [예상체결가 대비]     */
	char		preydrate[6];		/*  [예상체결가 등락율]     */
	char		jnilysign[1];		/*  [예상체결가 전일종가 대비구분]     */
	char		jnilychange[8];		/*  [예상체결가 전일종가 대비]     */
	char		jnilydrate[6];		/*  [예상체결가 전일종가 등락율]     */
	char		yecvolume[8];		/*  [예상체결변동량]     */
	char		yevolume[12];		/*  [예상체결수량]     */
	char		yvolchange[12];		/*  [예상체결수량 전일거래량 대비]     */
	char		yofferho0[8];		/*  [예상매도호가]     */
	char		ybidho0[8];			/*  [예상매수호가]     */
	char		yofferrem0[12];		/*  [예상매도호가수량]     */
	char		ybidrem0[12];		/*  [예상매수호가수량]     */
	char		ypreoffercha0[12];	/*  [예상직전매도대비수량]     */
	char		yprebidcha0[12];	/*  [예상직전매수대비수량]    */
	char		shcode[6];			/*  [단축코드]     */
} stEUREAL_KXYS;	//23

class CEUREAL_KXYS
{
public: //Variable
	CString		m_szdgubun;			/*  [KRX 예상체결]     */
	CString		m_szkshcode;		/*  [단축코드]     */
	CString		m_szhotime;			/*  [호가시간 HHMMSS]     */
	CString		m_szyeprice;		/*  [예상체결가격]     */
	CString		m_szpreysign;		/*  [예상체결가 대비구분]     */
	CString		m_szpreychange;		/*  [예상체결가 대비]     */
	CString		m_szpreydrate;		/*  [예상체결가 등락율]     */
	CString		m_szjnilysign;		/*  [예상체결가 전일종가 대비구분]     */
	CString		m_szjnilychange;	/*  [예상체결가 전일종가 대비]     */
	CString		m_szjnilydrate;		/*  [예상체결가 전일종가 등락율]     */
	CString		m_szyecvolume;		/*  [예상체결변동량]     */
	CString		m_szyevolume;		/*  [예상체결수량]     */
	CString		m_szyvolchange;		/*  [예상체결수량 전일거래량 대비]     */
	CString		m_szyofferho0;		/*  [예상매도호가]     */
	CString		m_szybidho0;		/*  [예상매수호가]     */
	CString		m_szyofferrem0;		/*  [예상매도호가수량]     */
	CString		m_szybidrem0;		/*  [예상매수호가수량]     */
	CString		m_szypreoffercha0;	/*  [예상직전매도대비수량]     */
	CString		m_szyprebidcha0;	/*  [예상직전매수대비수량]    */
	CString		m_szshcode;			/*  [단축코드]     */

public:
	CEUREAL_KXYS()
	{
	}

	virtual void  Convert(stEUREAL_KXYS* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [KRX 예상체결]     */
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [단축코드]     */
		DATA_MEMCOPY(sTmp, pInput, hotime);			/*  [호가시간 HHMMSS]     */
		DATA_MEMCOPY(sTmp, pInput, yeprice);		/*  [예상체결가격]     */
		DATA_MEMCOPY(sTmp, pInput, preysign);		/*  [예상체결가 대비구분]     */
		DATA_MEMCOPY(sTmp, pInput, preychange);		/*  [예상체결가 대비]     */
		DATA_MEMCOPY(sTmp, pInput, preydrate);		/*  [예상체결가 등락율]     */
		DATA_MEMCOPY(sTmp, pInput, jnilysign);		/*  [예상체결가 전일종가 대비구분]     */
		DATA_MEMCOPY(sTmp, pInput, jnilychange);	/*  [예상체결가 전일종가 대비]     */
		DATA_MEMCOPY(sTmp, pInput, jnilydrate);		/*  [예상체결가 전일종가 등락율]     */
		DATA_MEMCOPY(sTmp, pInput, yecvolume);		/*  [예상체결변동량]     */
		DATA_MEMCOPY(sTmp, pInput, yevolume);		/*  [예상체결수량]     */
		DATA_MEMCOPY(sTmp, pInput, yvolchange);		/*  [예상체결수량 전일거래량 대비]     */
		DATA_MEMCOPY(sTmp, pInput, yofferho0);		/*  [예상매도호가]     */
		DATA_MEMCOPY(sTmp, pInput, ybidho0);		/*  [예상매수호가]     */
		DATA_MEMCOPY(sTmp, pInput, yofferrem0);		/*  [예상매도호가수량]     */
		DATA_MEMCOPY(sTmp, pInput, ybidrem0);		/*  [예상매수호가수량]     */
		DATA_MEMCOPY(sTmp, pInput, ypreoffercha0);	/*  [예상직전매도대비수량]     */
		DATA_MEMCOPY(sTmp, pInput, yprebidcha0);	/*  [예상직전매수대비수량]    */
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [단축코드]     */
	}
};

#endif // AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_
