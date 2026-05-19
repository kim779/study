#ifndef _WORKIO_H
#define _WORKIO_H

#pragma	pack(1)

#define GB_TM	'1'
#define	GB_MP	'0'

#define TRGB_NONE		0
#define TRGB_LOGIN		1
#define TRGB_FORMALL		2
#define TRGB_TRLAYOUT		3
#define TRGB_FORMSOURCE		4
#define TRGB_FORMSAVE		5
#define TRGB_FORMCHECK		6
#define TRGB_FORMHISTORY	7
#define TRGB_FORMTRLAYOUT	8

//struct _loadform {						
//	char	gubn[1];		/* F: form  L:layout		*/		
//};
//#define	L_loadform		sizeof(_loadform)

struct _formlist {		
	char	nrec[8];	
};
#define	L_formlist		sizeof(_formlist)

struct _mapTree{
	char	mgub[4];		/* map category 구분자		*/
	char	fil1;			/* filler			*/
	char	mapd[20];		/* map desc			*/
	char	fil2;			/* filler			*/
	char	mapn[16];		/* map name			*/
	char	fil3;			/* filler			*/
	char	ckgb[1]; 		/* check in gubn		*/		
	char	fil4;    		/* filler			*/		
	char	ckid[16];		/* check in user Id		*/		
	char	fil5;    		/* filler			*/		

};
#define	L_mapTree		sizeof(_mapTree)


struct _workIO	{
	char	idno[8];		/* idno				*/		
	char	fnam[64];		/* File Name(FORMName)		*/		
	char	gubn[1];		/* 0: read only (default)	*/	
					/* 1 : write mode		*/
					/* 2 : tmp mode			*/
};
#define	L_workIO		sizeof(_workIO)

struct _workIOUT {						
	char	idno[8];		/* idno				*/		
	char	fnam[64];		/* File Name			*/		
	char	type[1]; 		/* File Type			*/		
					/* 'B' : FORM Bin		*/		
					/* 'S' : FORM Src		*/		
					/* 'G' : IMAGE			*/		
					/* 'T' : TR Layout		*/		
					/* 'I' : Infomation(cfg...)	*/		
	char	ver1[4]; 		/* Version 1			*/		
	char	ver2[4]; 		/* Version 2			*/		
	char	ver3[4]; 		/* Version 3			*/		
	char	gubn[1];		/* 0: read only (default)	*/		
					/* 1 : write mode		*/		
					/* 2 : tmp mode			*/		
	char	ckid[16];		/* check in user Id		*/		
	char	desc[20];		/* rsc description		*/		
	char	datl[8]; 		/* File length			*/		
};
#define	L_workIOUT		sizeof(_workIOUT)


struct _saveform {						
	char	idno[8]; 		/* idno				*/		
					/* 0    : New Resource		*/		
					/* idno : Update Resource	*/		
	char	fnam[64];		/* File Name			*/		
	char	type[1]; 		/* File Type			*/		
					/* 'B' : FORM Bin		*/		
					/* 'S' : FORM Src		*/		
					/* 'G' : IMAGE			*/		
					/* 'T' : TR Layout		*/		
					/* 'I' : Infomation(cfg...)	*/		
	char	ver1[4]; 		/* Version 1			*/		
	char	ver2[4]; 		/* Version 2			*/		
	char	ver3[4]; 		/* Version 3			*/		
	char	gubn[1]; 		/* 0 : tmp저장			*/		
					/* 1: update			*/		
	char	desc[20]; 		/* Description			*/		
	char	mdsc[1024];		/* modify description		*/		
	char	datl[8]; 		/* File length			*/		
};
#define	L_saveform		sizeof(_saveform)


struct _downform {						
	char	idno[8]; 		/* idno				*/		
					/* 0    : New Resource		*/		
					/* idno : Update Resource	*/		
	char	fnam[64];		/* File Name			*/		
	char	type[1]; 		/* File Type			*/		
					/* 'B' : FORM Bin		*/		
					/* 'S' : FORM Src		*/		
					/* 'G' : IMAGE			*/		
					/* 'T' : TR Layout		*/		
					/* 'I' : Infomation(cfg...)	*/		
	char	ver1[4]; 		/* Version 1			*/		
	char	ver2[4]; 		/* Version 2			*/		
	char	ver3[4]; 		/* Version 3			*/		
	char	gubn[1]; 		/* 0 : tmp저장			*/		
					/* 1: update			*/		
	char	desc[16]; 		/* Description			*/		
	char	mdsc[20];		/* modify description		*/		
	char	datl[8]; 		/* File length			*/		
};
#define	L_downform		sizeof(_downform)




struct _formcheck {						
	char	idno[8];		/* idno				*/		
	char	fnam[64];		/* File Name			*/		
};
#define	L_formcheck		sizeof(_formcheck)


struct _formhistoryIN {
	char	fnam[64];		/* File Name(FORMName)		*/		
};
#define	L_formhistoryIN		sizeof(_formhistoryIN)

struct _formhistorylist {
	char	fnam[64];		/* File Name(FORMName)		*/		
	char	nrec[8];				
};
#define	L_formhistorylist	sizeof(_formhistorylist)

struct _formhistoryOUT {					
	char	idno[8];		/* idno				*/		
	char	fnam[64];		/* File Name			*/		
	char	type[1]; 		/* File Type			*/		
					/* 'B' : FORM Bin		*/		
					/* 'S' : FORM Src		*/		
					/* 'G' : IMAGE			*/		
					/* 'T' : TR Layout		*/		
					/* 'I' : Infomation(cfg...)	*/		
	char	ver1[4]; 		/* Version 1			*/		
	char	ver2[4]; 		/* Version 2			*/		
	char	ver3[4]; 		/* Version 3			*/		
	char	usid[16];		/* update Id			*/		
	char	udat[14];		/* Update datetime		*/		
	char	rchk[1];		/* check release		*/		
					/* 'Y' : Release wait ok	*/		
					/* 'N' : Relase wait No		*/		
	char	rlid[16];		/* release request usid		*/		
	char	rdat[14];		/* release date time		*/		
	char	mdsc[1024];		/* modify description		*/		
};
#define	L_formhistoryOUT	sizeof(_formhistoryOUT)

#pragma	pack()

#endif