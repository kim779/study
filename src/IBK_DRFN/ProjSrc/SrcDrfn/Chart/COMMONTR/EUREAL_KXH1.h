#if !defined(AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_

// #include "../CommonTR/ChartTrAll.h"
// 날자 : 2008.04.25(금)

#include "Data_Common.h"

//--------------------------------------------------------------------
//     주식호가
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [KRX 호가잔량]     */
	char		kshcode[6];			/*  [단축코드]     */
	char		hotime[6];			/*  [호가시간 HHMMSS]     */
	char		offerho1[8];		/*  [매도호가 1]     */
	char		bidho1[8];			/*  [매수호가 1]     */
	char		offerrem1[12];		/*  [매도호가 잔량 1]     */
	char		bidrem1[12];		/*  [매수호가 잔량 1]     */
	char		preoffercha1[12];	/*  [직전매도대비수량 1]     */
	char		prebidcha1[12];		/*  [직전매수대비수량 1]     */
	char		offerho2[8];		/*  [매도호가 2]     */
	char		bidho2[8];			/*  [매수호가 2]     */
	char		offerrem2[12];		/*  [매도호가 잔량 2]     */
	char		bidrem2[12];		/*  [매수호가 잔량 2]     */
	char		preoffercha2[12];	/*  [직전매도대비수량 2]     */
	char		prebidcha2[12];		/*  [직전매수대비수량 2]     */
	char		offerho3[8];		/*  [매도호가 3]     */
	char		bidho3[8];			/*  [매수호가 3]     */
	char		offerrem3[12];		/*  [매도호가 잔량 3]     */
	char		bidrem3[12];		/*  [매수호가 잔량 3]     */
	char		preoffercha3[12];	/*  [직전매도대비수량 3]     */
	char		prebidcha3[12];		/*  [직전매수대비수량 3]     */
	char		offerho4[8];		/*  [매도호가 4]     */
	char		bidho4[8];			/*  [매수호가 4]     */
	char		offerrem4[12];		/*  [매도호가 잔량 4]     */
	char		bidrem4[12];		/*  [매수호가 잔량 4]     */
	char		preoffercha4[12];	/*  [직전매도대비수량 5]     */
	char		prebidcha4[12];		/*  [직전매수대비수량 5]     */
	char		offerho5[8];		/*  [매도호가 5]     */
	char		bidho5[8];			/*  [매수호가 5]     */
	char		offerrem5[12];		/*  [매도호가 잔량 5]     */
	char		bidrem5[12];		/*  [매수호가 잔량 5]     */
	char		preoffercha5[12];	/*  [직전매도대비수량 5]     */
	char		prebidcha5[12];		/*  [직전매수대비수량 5]     */
	char		offerho6[8];		/*  [매도호가 6]     */
	char		bidho6[8];			/*  [매수호가 6]     */
	char		offerrem6[12];		/*  [매도호가 잔량 6]     */
	char		bidrem6[12];		/*  [매수호가 잔량 6]     */
	char		preoffercha6[12];	/*  [직전매도대비수량 6]     */
	char		prebidcha6[12];		/*  [직전매수대비수량 6]     */
	char		offerho7[8];		/*  [매도호가 7]     */
	char		bidho7[8];			/*  [매수호가 7]     */
	char		offerrem7[12];		/*  [매도호가 잔량 7]     */
	char		bidrem7[12];		/*  [매수호가 잔량 7]     */
	char		preoffercha7[12];	/*  [직전매도대비수량 7]     */
	char		prebidcha7[12];		/*  [직전매수대비수량 7]     */
	char		offerho8[8];		/*  [매도호가 8]     */
	char		bidho8[8];			/*  [매수호가 8]     */
	char		offerrem8[12];		/*  [매도호가 잔량 8]     */
	char		bidrem8[12];		/*  [매수호가 잔량 8]     */
	char		preoffercha8[12];	/*  [직전매도대비수량 8]     */
	char		prebidcha8[12];		/*  [직전매수대비수량 8]     */
	char		offerho9[8];		/*  [매도호가 9]     */
	char		bidho9[8];			/*  [매수호가 9]     */
	char		offerrem9[12];		/*  [매도호가 잔량 9]     */
	char		bidrem9[12];		/*  [매수호가 잔량 9]     */
	char		preoffercha9[12];	/*  [직전매도대비수량 9]     */
	char		prebidcha9[12];		/*  [직전매수대비수량 9]     */
	char		offerho10[8];		/*  [매도호가 10]     */
	char		bidho10[8];			/*  [매수호가 10]     */
	char		offerrem10[12];		/*  [매도호가 잔량 10]     */
	char		bidrem10[12];		/*  [매수호가 잔량 10]     */
	char		preoffercha10[12];	/*  [직전매도대비수량 10]     */
	char		prebidcha10[12];	/*  [직전매수대비수량 10]     */
	char		totofferrem[12];	/*  [매도호가총수량]     */
	char		totbidrem[12];		/*  [매수호가총수량]     */
	char		preoffercha[12];	/*  [직전매도호가총대비수량]     */
	char		prebidcha[12];		/*  [직전매수호가총대비수량]     */
	char		donsigubun[1];		/*  [동시호가 구분]     */
	char		shcode[6];			/*  [단축코드]     */
} stEUREAL_KXH1;	//1

class CEUREAL_KXH1
{
public: //Variable
	CString		m_szdgubun;			/*  [KRX 호가잔량]			*/
	CString		m_szkshcode;		/*  [단축코드]				*/
	CString		m_szhotime;			/*  [호가시간 HHMMSS]		*/
	CString		m_szofferho1;		/*  [매도호가 1]			*/
	CString		m_szbidho1;			/*  [매수호가 1]			*/
	CString		m_szofferrem1;		/*  [매도호가 잔량 1]		*/
	CString		m_szbidrem1;		/*  [매수호가 잔량 1]		*/
	CString		m_szpreoffercha1;	/*  [직전매도대비수량 1]    */
	CString		m_szprebidcha1;		/*  [직전매수대비수량 1]    */
	CString		m_szofferho2;		/*  [매도호가 2]			*/
	CString		m_szbidho2;			/*  [매수호가 2]			*/
	CString		m_szofferrem2;		/*  [매도호가 잔량 2]		*/
	CString		m_szbidrem2;		/*  [매수호가 잔량 2]		*/
	CString		m_szpreoffercha2;	/*  [직전매도대비수량 2]    */
	CString		m_szprebidcha2;		/*  [직전매수대비수량 2]    */
	CString		m_szofferho3;		/*  [매도호가 3]			*/
	CString		m_szbidho3;			/*  [매수호가 3]			*/
	CString		m_szofferrem3;		/*  [매도호가 잔량 3]		*/
	CString		m_szbidrem3;		/*  [매수호가 잔량 3]		*/
	CString		m_szpreoffercha3;	/*  [직전매도대비수량 3]    */
	CString		m_szprebidcha3;		/*  [직전매수대비수량 3]    */
	CString		m_szofferho4;		/*  [매도호가 4]			*/
	CString		m_szbidho4;			/*  [매수호가 4]			*/
	CString		m_szofferrem4;		/*  [매도호가 잔량 4]		*/
	CString		m_szbidrem4;		/*  [매수호가 잔량 4]		*/
	CString		m_szpreoffercha4;	/*  [직전매도대비수량 5]    */
	CString		m_szprebidcha4;		/*  [직전매수대비수량 5]    */
	CString		m_szofferho5;		/*  [매도호가 5]			*/
	CString		m_szbidho5;			/*  [매수호가 5]			*/
	CString		m_szofferrem5;		/*  [매도호가 잔량 5]		*/
	CString		m_szbidrem5;		/*  [매수호가 잔량 5]		*/
	CString		m_szpreoffercha5;	/*  [직전매도대비수량 5]    */
	CString		m_szprebidcha5;		/*  [직전매수대비수량 5]    */
	CString		m_szofferho6;		/*  [매도호가 6]			*/
	CString		m_szbidho6;			/*  [매수호가 6]			*/
	CString		m_szofferrem6;		/*  [매도호가 잔량 6]		*/
	CString		m_szbidrem6;		/*  [매수호가 잔량 6]		*/
	CString		m_szpreoffercha6;	/*  [직전매도대비수량 6]    */
	CString		m_szprebidcha6;		/*  [직전매수대비수량 6]    */
	CString		m_szofferho7;		/*  [매도호가 7]			*/
	CString		m_szbidho7;			/*  [매수호가 7]			*/
	CString		m_szofferrem7;		/*  [매도호가 잔량 7]		*/
	CString		m_szbidrem7;		/*  [매수호가 잔량 7]		*/
	CString		m_szpreoffercha7;	/*  [직전매도대비수량 7]    */
	CString		m_szprebidcha7;		/*  [직전매수대비수량 7]    */
	CString		m_szofferho8;		/*  [매도호가 8]			*/
	CString		m_szbidho8;			/*  [매수호가 8]			*/
	CString		m_szofferrem8;		/*  [매도호가 잔량 8]		*/
	CString		m_szbidrem8;		/*  [매수호가 잔량 8]		*/
	CString		m_szpreoffercha8;	/*  [직전매도대비수량 8]    */
	CString		m_szprebidcha8;		/*  [직전매수대비수량 8]    */
	CString		m_szofferho9;		/*  [매도호가 9]			*/
	CString		m_szbidho9;			/*  [매수호가 9]			*/
	CString		m_szofferrem9;		/*  [매도호가 잔량 9]		*/
	CString		m_szbidrem9;		/*  [매수호가 잔량 9]		*/
	CString		m_szpreoffercha9;	/*  [직전매도대비수량 9]    */
	CString		m_szprebidcha9;		/*  [직전매수대비수량 9]    */
	CString		m_szofferho10;		/*  [매도호가 10]			*/
	CString		m_szbidho10;		/*  [매수호가 10]			*/
	CString		m_szofferrem10;		/*  [매도호가 잔량 10]		*/
	CString		m_szbidrem10;		/*  [매수호가 잔량 10]		*/
	CString		m_szpreoffercha10;	/*  [직전매도대비수량 10]   */
	CString		m_szprebidcha10;	/*  [직전매수대비수량 10]   */
	CString		m_sztotofferrem;	/*  [매도호가총수량]		*/
	CString		m_sztotbidrem;		/*  [매수호가총수량]		*/
	CString		m_szpreoffercha;	/*  [직전매도호가총대비수량]*/
	CString		m_szprebidcha;		/*  [직전매수호가총대비수량]*/
	CString		m_szdonsigubun;		/*  [동시호가 구분]			*/
	CString		m_szshcode;			/*  [단축코드]				*/

public:
	CEUREAL_KXH1()
	{
	}

	virtual void  Convert(stEUREAL_KXH1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [KRX 호가잔량]			*/
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [단축코드]				*/
		DATA_MEMCOPY(sTmp, pInput, hotime);			/*  [호가시간 HHMMSS]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [매도호가 1]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [매수호가 1]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/*  [매도호가 잔량 1]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/*  [매수호가 잔량 1]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha1);	/*  [직전매도대비수량 1]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha1);		/*  [직전매수대비수량 1]    */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  [매도호가 2]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/*  [매수호가 2]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/*  [매도호가 잔량 2]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/*  [매수호가 잔량 2]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha2);	/*  [직전매도대비수량 2]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha2);		/*  [직전매수대비수량 2]    */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  [매도호가 3]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/*  [매수호가 3]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/*  [매도호가 잔량 3]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/*  [매수호가 잔량 3]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha3);	/*  [직전매도대비수량 3]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha3);		/*  [직전매수대비수량 3]    */
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/*  [매도호가 4]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/*  [매수호가 4]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/*  [매도호가 잔량 4]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/*  [매수호가 잔량 4]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha4);	/*  [직전매도대비수량 5]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha4);		/*  [직전매수대비수량 5]    */
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/*  [매도호가 5]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/*  [매수호가 5]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/*  [매도호가 잔량 5]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/*  [매수호가 잔량 5]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha5);	/*  [직전매도대비수량 5]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha5);		/*  [직전매수대비수량 5]    */
		DATA_MEMCOPY(sTmp, pInput, offerho6);		/*  [매도호가 6]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho6);			/*  [매수호가 6]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem6);		/*  [매도호가 잔량 6]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem6);		/*  [매수호가 잔량 6]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha6);	/*  [직전매도대비수량 6]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha6);		/*  [직전매수대비수량 6]    */
		DATA_MEMCOPY(sTmp, pInput, offerho7);		/*  [매도호가 7]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho7);			/*  [매수호가 7]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem7);		/*  [매도호가 잔량 7]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem7);		/*  [매수호가 잔량 7]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha7);	/*  [직전매도대비수량 7]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha7);		/*  [직전매수대비수량 7]    */
		DATA_MEMCOPY(sTmp, pInput, offerho8);		/*  [매도호가 8]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho8);			/*  [매수호가 8]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem8);		/*  [매도호가 잔량 8]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem8);		/*  [매수호가 잔량 8]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha8);	/*  [직전매도대비수량 8]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha8);		/*  [직전매수대비수량 8]    */
		DATA_MEMCOPY(sTmp, pInput, offerho9);		/*  [매도호가 9]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho9);			/*  [매수호가 9]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem9);		/*  [매도호가 잔량 9]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem9);		/*  [매수호가 잔량 9]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha9);	/*  [직전매도대비수량 9]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha9);		/*  [직전매수대비수량 9]    */
		DATA_MEMCOPY(sTmp, pInput, offerho10);		/*  [매도호가 10]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho10);		/*  [매수호가 10]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem10);		/*  [매도호가 잔량 10]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem10);		/*  [매수호가 잔량 10]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha10);	/*  [직전매도대비수량 10]   */
		DATA_MEMCOPY(sTmp, pInput, prebidcha10);	/*  [직전매수대비수량 10]   */
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/*  [매도호가총수량]		*/
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/*  [매수호가총수량]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha);	/*  [직전매도호가총대비수량]*/
		DATA_MEMCOPY(sTmp, pInput, prebidcha);		/*  [직전매수호가총대비수량]*/
		DATA_MEMCOPY(sTmp, pInput, donsigubun);		/*  [동시호가 구분]			*/
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [단축코드]				*/
	}
};


typedef struct 
{
	char		dgubun[4];			/*  [장전시간외호가잔량]     */
	char		kshcode[6];			/*  [단축코드]     */
	char		hotime[6];			/*  [호가시간]     */
	char		tmofferrem[12];		/*  [시간외매도잔량]     */
	char		tmbidrem[12];		/*  [시간외매수잔량]     */
	char		pretmoffercha[12];	/*  [시간외매도수량 직전대비]     */
	char		pretmbidcha[12];	/*  [시간외매수수량 직전대비]     */
	char		shcode[6];			/*  [단축코드]     */
} EUREAL_KXH2;	//3

#endif // AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_
