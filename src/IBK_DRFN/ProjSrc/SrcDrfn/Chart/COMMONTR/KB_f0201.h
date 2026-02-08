/*************************************************************************************/
/*  1. 프로그램ID : KB_f0201                                                         */
/*  2. 프로그램명 : 선물옵션(호가)                                                   */
/*  3. 서비스명   : KB_f0201                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_f0201_H__
#define __KB_TRAN_KB_f0201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_f0201"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		futcode[8];		/*  종목코드     */
//	char		mode[1];		/*  조회모드     */
//	char		count[4];		/*  조회건수     */
}KB_f0201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

typedef struct _KB_f0201_OutRec1 {
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
} KB_f0201_OutRec1;

/*************************************************************************************/
class CKB_f0201
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
	CKB_f0201()
	{
	}

	virtual void  Convert(KB_f0201_OutRec1* pInput)
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

