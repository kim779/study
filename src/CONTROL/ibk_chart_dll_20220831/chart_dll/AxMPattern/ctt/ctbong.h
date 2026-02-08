
#include <stdio.h>
#include <stdlib.h>

struct	yymd	{
	short	yy;
	char	mm;
	char	dd;
};
struct	hms	{
	char	hh;
	char	mm;
	char	ss;
};
typedef	struct	yymd	YMD;
typedef	struct	hms	HMS;

struct	bunbong	{
	YMD	date;		/* 일자				*/
	HMS	time;		/* 시분				*/
	int	siga;		/* 시가				*/
	int	koga;		/* 고가				*/
	int	jega;		/* 저가				*/
	int	jgga;		/* 종가				*/
	char	giho;		/* 기호				*/
	int	diff;		/* 대비				*/
	int	gvol;		/* 거래량			*/
	int	xvol;		/* 누적 거래량			*/
};

struct  bonginf {
	int	nrec;
	int	type;		/* 1:date,2:date+time,3:time */
	char	cod2[16];
	struct	bunbong bong[400];
};