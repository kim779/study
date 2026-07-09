#ifndef _WORKIO_H
#define _WORKIO_H

#define GB_TM	'1'
#define	GB_MP	'0'

struct _workIO	{
	char	tgub;		/* map & template 구분			*/
	char	mapn[8];	/* map name				*/
	char	mgub[4];	/* map category 구분자			*/
	char	mapd[20];	/* map desc				*/
};

struct _mapTree{
	char	mgub[4];	/* map category 구분자			*/
	char	fil1;		/* filler				*/
	char	mapd[20];	/* map desc				*/
	char	fil2;		/* filler				*/
	char	mapn[8];	/* map name				*/
	char	fil3;		/* filler				*/
};
#define	L_workIO	sizeof(_workIO)
#define	L_mapTree	sizeof(_mapTree)
#endif