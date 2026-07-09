#ifndef	_LEDGER_H_
#define	_LEDGER_H_

struct	_ledgerH {
	//char	len[8];
	char	anam[40];		/* application name	*/
	char	snam[40];		/* service name		*/
	char	fnam[32];		/* function name	*/
	char	fncd[2];		/* function code	*/
	char	guid[32];		/* global id		*/
	char	ctyp[2];		/* channel type		*/
	char	eflg[1];		/* environment type	*/
	char	tflg[1];		/* tran flag		*/
	char	tdat[8];		/* tran send date	*/
	char	ttim[20];		/* tran send time	*/
	char	clip[31];		/* client ip		*/
	char	rtyp[1];		/* response type	*/
	char	rcod[8];		/* response code	*/
	char	rlcd[20];		/* response log code	*/
	char	rtit[50];		/* response title	*/
	char	rbmg[100];		/* response message	*/
	char	rdmg[500];		/* response message	*/
	char	usid[12];		/* user id		*/
	char	crst[2];		/* character set	*/
	char	fill[78];		/* filler		*/
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	sfill[38];		/* filler		*/
	char	susid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};


/*
struct	_ledgerH {
	char	time[4];
	char	flag[10];	//   0  gateway flag
				// 업무공통	 Start				
	char	svcn[20];	//  10  service name
	char	svid[2];	//  30  service id	("01")
	char	pgid[30];	//  32  program id	($$####$$)
	char	usid[8];	//  62  user id		("HTS002")
	char	dept[3];	//  70  dept id		("F07")
	char	term[15];	//  73  terminal address
	char	usnm[30];	//  88  user name	("HTS002")
	char	trcd[4];	// 118  tr code		(#### of pgid)
				// 업무공통	 END				
	char    scrn[4];	// #screen number
	char    zend[1];	// windows closed				
	char    zfcp[1];	// fix cursor position				
	char	ikey[1];	// key action(up/down)	("0")			

	char	eofd[1];	// end of Data					
	char	reqn[4];	// Request Row Count				
	char	ecod[4];	// Error Code					
	char	emsg[80];	// Error Message				
	char	accn[10];	// accn for rms					
	char	uflg[1];
	char	user[8];
	char	xxxx[15];	// dummy					
};
*/
/*
struct _ledgerLoginH {
	struct _ledgerH ledgerH;
	char	svno[20];
	char	usid[20];
	char	term[15];
	char	inputGB[10];
	char	comGB[10];
};
*/

struct	_ledgerLoginH	{
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	fill[38];		/* filler		*/
	char	usid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};


#define	L_ledgerH	sizeof(struct _ledgerH)		// 176 bytes
#define L_ledgerLoginH	sizeof(struct _ledgerLoginH)

#endif