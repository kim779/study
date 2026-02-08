#pragma once


typedef struct _jcode
{
	char	indx;		/* 번호			*/
	char	code[12];	/* 종목코드		*/
	char	cod2[20];	/* 종목명		*/
	char	giho;		/* 기호			*/
	char	udyl[7];	/* 전일대비		*/
	char	bdyl[7];	/* 변동비중		*/
	char	sdyl[7];	/* 시가비중		*/
}_jcode2;

typedef struct _outbound
{
	char	djis[7];	/* 하락지수		*/
	char	dnyl[7];	/* 하락비중		*/
	char	dnum[7];	/* 하락종목수		*/
	char	ujis[7];	/* 상승지수		*/
	char	upyl[7];	/* 상승비중		*/
	char	unum[7];	/* 상승종목수		*/
	struct _jcode	ucod[10];/* 상승 종목		*/
	struct _jcode	dcod[10];/* 하락 종목		*/
	char	rank[4];	/* rank			*/
	char	time[6];	/* 시간			*/
}_outbound2;

