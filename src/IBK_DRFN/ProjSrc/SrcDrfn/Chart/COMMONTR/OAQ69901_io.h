#ifndef __OAQ69901_H__
#define __OAQ69901_H__

#pragma pack(push)
#pragma pack(1)

#define	MIGYULJE_PID			0x61			// 미결제 실시간

#define	TROAQ69901		"OAQ69901"

typedef struct tagOAQ69901_InRec
{
	char		userid[8];		/* USERID */
}OAQ69901_INREC;

typedef struct tagOutRec1
{
	char		acntno[10];		/* 계좌번호 */
	char		series[30];		/* 종목 */
	char		buysellsect[1];	/* 구분 */
	char		posqty[8];		/* 미결제수량 */
	char		posprice[12];	/* 평균약정가 */
}OAQ69901_OUTREC1;

typedef struct tagOAQ69901_OutRec2
{
	char		acntno[10];		/* 계좌번호 */
	char		series[30];		/* 종목 */
	char		ordno[5];		/* 주문번호 */
	char		buysellsect[1];	/* 구분 */
	char		ordqty[8];		/* 주문량 */
	char		ordprice[12];	/* 주문가 */
}OAQ69901_OUTREC2;

//리얼은 0x61임.

typedef struct _Order_Info
{
	CString		m_stracntno;		/* 계좌번호 */
	CString		m_strseries;		/* 종목 */
	CString		m_strbuysellsect;	/* 구분 */
	CString		m_strposqty;		/* 미결제수량 */
	CString		m_strposprice;	/* 평균약정가 */
	CString		m_strstate;		/* 처리상태 */
	double		m_dblPrice;		/* 평균약정가 */
} ORDER_INFO;

#pragma pack(pop)

#endif // __OAQ69901_H__
