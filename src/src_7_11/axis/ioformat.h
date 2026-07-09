#if !defined(AFX_IOFORMAT_H__INCLUDED_)
#define AFX_IOFORMAT_H__INCLUDED_

//
//	TRCODE : pihoaccn

struct  _inacct {
	char	usid[8];		// 사용자ID (kangmc)
	char	tygb[1];		// '1' : 대행유저ID리스트 포함
	char	sort[1];		// '1' : 계좌명정렬, '0' : 계좌번호정렬
	char	key[1];			// 가상화면별 KEY 값
};

//	_acctinfo.acct == fid_PINCH
//		_outacct.acct : pinch_id, _outacct.anam : pinch_name

struct	_acctinfo {
	char	fid[1];			// field(folder) gubun id
	char	symboName[20];		// symbom Name
	char	fnam[20];		// field(folder) name

	char	gid[3];			// group(fund) id
	char	gnam[20];		// group(fund) name
	
	char	acnt[4];		// account count
};
#define		sz_ACCTINFO	sizeof(struct _acctinfo)

struct	_outacct {			// for kiwoom_fb
	char	acct[11];		// account
	char	pswd[8];		// account password
	char	anam[30];		// account name
	char	basu[4];		// account basu, only fund, default "0001"
	char	myst[1];		/* 관심종목 유무 */
};

#define		sz_OUTACCT	sizeof(struct _outacct)
#define		sz_USERID	8

struct  mid1 {
        char    hotkey[10];             /* 조회구분                     */
        char    usid[8];                /* 선물/옵션 구분               */
		char	jygb[1];				/* 0:주간 1:야간	20131113 SBC */
};

struct	mod1 {
	char	mgfg[1];		/* '1' : guid msg */
	char	mesg[128];
};

struct  _zcom_head {
	 char ywin[4];
	 char tcnt[4];
	 char seqn[4];
	 char fill[4];
};
#define		sz_zcom_head	sizeof(struct _zcom_head)



#endif //AFX_IOFORMAT_H__INCLUDED_