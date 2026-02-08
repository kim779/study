//////////////////////////////////////////////////////////////
//	account.h
//////////////////////////////////////////////////////////////
#pragma once

#define TRCODE	"pidoaccn"
#define TRPSWD	"PIBOPBXQ"

#define LEN_ACCT	11

struct  _acInfo {
    char    accn[LEN_ACCT];     /* 계좌번호			                   */
    char    acnm[45];			/* 계좌명 + '|' + 계좌별칭	           */
    char    ownf[1];			/* 본인계좌플래그			           */
	char	fill[2];			/* filler							   */
};

struct  _acSet {
    char    func[1];        /* FUNCTION CODE                */
                            /* 'Q' Query    'I' Insert      */
                            /* 'U' Update                   */
    char    usid[12];       /* 사용자아이디                 */
    char    errc[1];        /* Error Code                   */
    char    emsg[80];       /* Error message                */
    char    nrec[4];        /* Account Count                */
    struct  _acInfo acinfo[1];
};

struct _chkpwd {
	char	accn[LEN_ACCT];
	char	psws[8];
};

#define L_chkpwd	sizeof(struct _chkpwd)
#define	L_acinfo	sizeof(struct _acInfo)


struct _chkPwdMid {
	char in [5];
	char acctNo[20];
	char password[8];
	//char zTrxTp[1];
};

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char zAvalYn;
	//char lPwdContErrCnt[9];
	//char lAtmPwdContErrCnt[9];
	//char lMtrsfPwdContErrCnt[9];
	//char zAcntNm[40];
};

/*
struct _chkPwdMid {
	char in [5];
	char acctNo[20];
	char password[8];
	char passwordTrxCode[5];
};

struct _chkPwdMod {
	struct _chkPwdMid in;
	char out[5];
	char qty[9];
	char passwordContErrCnt[9];
	char avalYn[1];
	char passwordTrxCode[5];
};
*/

// 2011.02.25 직원의 그룹계좌 저장을 위해 pidoseta declare by LKM
struct  t_accn {
    char    seqn[3];        /* 계좌일련번호                 */
    char    accn[11];       /* 계좌번호                     */
    char    pass[10];       /* 계좌비밀번호                 */
    char    acnm[30];       /* 계좌명                       */
    char    rate[5];        /* 배정비율                     */
    char    maxq[7];        /* 최대주문수량 for 법인주문    */
    char    alis[30];       /* 계좌별칭                     */
    char    pres[1];        /* 1:대표계좌                   */
};

struct  t_grup {
    char    xflg[1];        /* 'D' Delete    'I' Insert     */
	/* 'U' Update or Still          */
	/* OUTPUT 사용: group fg        */
    char    gpid[3];        /* 계좌그룹아이디               */
    char    seqn[3];        /* 계좌그룹일련번호             */
    char    gnam[20];       /* 그룹명                       */
    char    nrec[4];        /* 계좌 COUNT                   */
    struct  t_accn  rec[1];
};

struct  pidoseta_mid {
    char    xflg[1];        /* 1: 조회      2: 저장         */
    char    usid[12];       /* 사용자아이디                 */
    char    sflg[1];        /* 1: 주식                      */
	/* 2: 선물/옵션                 */
	/* 3: 전체                      */
    char    grec[4];        /* 그룹갯수                     */
    char    data[1];
};

struct  pidoseta_mod {
    char    okgb[1];        /* 'Y':정상 , 'N':오류          */
    char    grec[4];        /* 그룹갯수                     */
    char    data[1];
};