#ifndef	_LEDGER_H_
#define	_LEDGER_H_


struct	_ledgerH {
	char	time[4];	//   0  대기시간
	char	flag[10];	//   4  gateway flag
	char	svcn[20];	//  14  service name
	char	svid[2];	//  34  service id
	char	pgid[30];	//  36  program id
	char	usid[8];	//  66  user id
	char	dept[3];	//  74  dept id
	char	term[15];	//  77  terminal address
	char	usnm[30];	//  92  user name
	char	trcd[4];	// 122  tr code

	char    scrn[4];	// 126  screen number
	char    kend[1];	// 130  windows closed
	char    fxcp[1];	// 131  fix cursor position
	char	ikey[1];	// 132  key action(up/down)

	char	eofd[1];	// 133  end of Data
	char	reqn[4];	// 134  Request Row Count
	char	ecod[4];	// 138  Error Code
	char	emsg[80];	// 142  Error Message
	char	accn[10];	// 222  accn for rms
	char	xxxx[24];	// 232  dummy
};

#define	L_ledgerH	sizeof(struct _ledgerH)		// 255 bytes

#endif