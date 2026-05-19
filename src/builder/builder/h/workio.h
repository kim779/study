#pragma once

#pragma	pack(1)
//////////////////////////////////////////////////////
#define TRGB_FORMSOURCE		2
#define TRGB_FORMCHECK		6
#define TRGB_FORMHISTORY	7
#define TRGB_FORMSEARCH		8

struct i_saversc {
	char	type[1]; 		/* File Type                */
					/* 'B' : FORM Bin            */
					/* 'S' : FORM Src            */
					/* 'G' : IMAGE              */
					/* 'T' : TR Layout          */
					/* 'I' : Infomation(cfg...) */
	char	ver1[4]; 		/* Version 1                */
	char	ver2[4]; 		/* Version 2                */
	char	ver3[4]; 		/* Version 3                */
	char	mdsc[1024];		/* modify description   */
	char	file[1];		/* file context   */
};
#define	L_saversc	sizeof(i_saversc)

struct i_history {
	char rscType;
	char dirt;
	char seqn[8];
};
#define	L_history	sizeof(i_history)

struct s_condition {
	char	cono[2];
	char	cstr[80];	//null terminated string
};
#define	L_condition	sizeof(s_condition)

struct form_info {
	char	resn[16];		//파일명
	char	desc[32];
	char	usid[32];		//작성자
	char	yymd[8];		//작성일
};

struct search_info {
	char	msg[80];
	char	nrec[5];
	struct form_info FormInfo[1];
};

//struct g_rschist {
//	char	seqn[10];		/* idno     */
//	char	ver1[4]; 		/* Version 1                */
//	char	ver2[4]; 		/* Version 2                */
//	char	ver3[4]; 		/* Version 3                */
//	char	usid[32];		/* update Id            */
//	char	udat[14];		/* Update datetime           */
//	char	ruid[32];		/* release request usid   */
//	char	rdat[14];		/* release date time    */
//	char	mdsc[1024];		/* modify description         */
//};

//struct o_loadhistrsc {
//	char	fnam[64];		/* File Name(FORMName)     */
//	char	type[1];
//	char	seqn[10];
//	char	mesg[80];
//	char	nrec[4];
//	struct	g_rschist	rhist[1];
//};

//////////////////////////////////////////////////////
#define GB_TM	'1'
#define	GB_MP	'0'

struct _workIO	{
	char	tgub;		// map & template 구분
	char	mapn[8];	// map name
	char	mgub[4];	// map category 구분자
	char	mapd[20];	// map desc
};
#define	L_workIO	sizeof(_workIO)

//struct _mapTree{
//	char	mgub[4];	// map category 구분자
//	char	fil1;		// filler
//	char	mapd[20];	// map desc
//	char	fil2;		// filler
//	char	mapn[8];	// map name
//	char	fil3;		// filler
//};

struct _mapTree{
	char	mgub[4];	// map category 구분자
	char	fil1;		// filler
	char	mapd[20];	// map desc
	char	fil2;		// filler
	char	mapn[16];	// map name
	char	fil3;		// filler
	char	ckid[32];	// user id
	char	fil4;		// filler
};
#define	L_mapTree	sizeof(_mapTree)

struct  _trxlist{
	char    name[8];        /* trlayout file name                    */
	char    fil1;               /* filler                       */
	char    trxc[8];           /* tran name                    */
	char    fil2;               /* filler                       */
	char    trxd[64];         /* tran desc                    */
};
#define	L_trxlist	sizeof(_trxlist)

#pragma	pack()

