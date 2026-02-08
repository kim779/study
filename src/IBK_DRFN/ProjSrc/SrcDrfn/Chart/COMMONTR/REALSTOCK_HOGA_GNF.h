#include "Data_Common.h"

//--------------------------------------------------------------------
//     주식
//--------------------------------------------------------------------
typedef struct 
{
	char		dgubun[4];		/*  [Data 구분: CAH0]     */
	char		jmcode[10];		/*  [종목코드]     */
	char		chetime[6];		/*  [호가시간 HHMMSS]     */
	char		offerho1[10];		/*  [우선매도호가]     */
	char		offerho1_d[15];		/*  [진법전환전 우선매도호가]     */
	char		bidho1[10];		/*  [우선매수호가]     */
	char		bidho1_d[15];		/*  [진법전환전 우선매수호가]     */
	char		offerrem[8];		/*  [우선매도잔량]     */
	char		bidrem[8];		/*  [우선매수잔량]     */
	char		kjmcode[10];		/*  [종목코드]     */

} STREAL_GNF_STOCK_HOGA;



class CREAL_GNF_STOCK_HOGA
{
public: //Variable
	CString m_szdgubun;		/*  [Data 구분: CAH0]     */
	CString m_szjmcode;		/*  [종목코드]     */
	CString m_szchetime;		/*  [호가시간 HHMMSS]     */
	CString m_szofferho1;		/*  [우선매도호가]     */
	CString m_szofferho1_d;		/*  [진법전환전 우선매도호가]     */
	CString m_szbidho1;		/*  [우선매수호가]     */
	CString m_szbidho1_d;		/*  [진법전환전 우선매수호가]     */
	CString m_szofferrem;		/*  [우선매도잔량]     */
	CString m_szbidrem;		/*  [우선매수잔량]     */
	CString m_szkjmcode;		/*  [종목코드]     */
public:
	CREAL_GNF_STOCK_HOGA()
	{
	}

	virtual void  Convert(STREAL_GNF_STOCK_HOGA* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);		
		DATA_MEMCOPY(sTmp, pInput, jmcode);		
		DATA_MEMCOPY(sTmp, pInput, chetime);		
		DATA_MEMCOPY(sTmp, pInput, offerho1);	
		DATA_MEMCOPY(sTmp, pInput, offerho1_d);	
		DATA_MEMCOPY(sTmp, pInput, bidho1);		
		DATA_MEMCOPY(sTmp, pInput, bidho1_d);	
		DATA_MEMCOPY(sTmp, pInput, offerrem);	
		DATA_MEMCOPY(sTmp, pInput, bidrem);		
		DATA_MEMCOPY(sTmp, pInput, kjmcode);		
	}
};


