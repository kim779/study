#pragma once


#define GROUPNUM 4
#define ACCNTNUM 5
#define STRLENGTH 20

using namespace std;

class ACCNTINFO
{
public:
	CString sAccntSortNum;	//계좌일련번호
	CString sAccntNum;	//계좌번호
	CString sAccntName;	//계좌이름
	CString	sPassword;	//계좌 비번
	CString sAllocRate;	//배정비율
	CString sAccntNickName;	//계좌별칭
	CString sPriAccnt;	//대표계좌구분
	
	ACCNTINFO(){
		sAccntSortNum.Empty();
		sAccntNum.Empty();
		sAccntName.Empty();
		sPassword.Empty();
		sAllocRate.Empty();
		sAccntNickName.Empty();
		sPriAccnt.Empty();
	};
};

typedef vector<ACCNTINFO> ACCINFOVEC;

class GRINFO
{
public:
	CString sGrSortNum;		// 그룹일련번호
	CString sGrName;		// 그룹명
	CString sAccntCnt;		// 계좌갯수
	CString	sGroupID;		// 그룹아이디
	ACCINFOVEC AccInfo;
	
	GRINFO()
	{
		sGrSortNum.Empty();
		sGrName.Empty();
		sAccntCnt.Empty();
		sGroupID.Empty();
	};
};

struct  _ihead  {
	char	xflg[1];		// 'F':first, 'N':next, 'E':end(이것은 14K이상일때만 적용)
	char	csiz[6];		// 보낼 size	
	char	fsiz[6];		// file size
	char	fill[3];
};
#define	L_ihead		sizeof(struct _ihead)

typedef struct  _group {
	char	xflg;		/* 'U': Update, 'D': Delete, 'I': Insert */
	char	gpid[3];	/* 계좌그룹아이디	*/
	char    seqn[3];	/* 계좌그룹일련번호	*/
	char    gnam[20];	/* 그룹명		*/
	char    nrec[4];	/* 계좌 COUNT		*/
	char	fill;		// 널바이트(struct  accn    accn;)
}groupINFO;

typedef struct  _accn {
	char    seqn[3];	/* 계좌일련번호		*/
	char    accn[10];	/* 계좌번호		*/
	char	pass[10];	/* 계좌 비밀번호	*/
	char    acnm[30];	/* 계좌명		*/
	char    rate[5];	/* 배정비율		*/
	char	multi[7];	/* 승수	2006.10.19추가	*/
	char    alis[30];	/* 계좌별칭		*/
	char    prea;		/* 1:대표계좌		*/
	char	fill;		// 널바이트
}accINFO;

typedef struct  _mid {
	char    xflg;		/* 1: 조회		*/
				/* 2: 저장		*/
	char	usrid[12];	/* 사원번호		*/
	char	sflag;		/* 1: 주식		*/
				/* 2: 선물/옵션		*/
	char    grec[4];	/* 그룹갯수		*/
	char	fill;		// 널바이트(struct  group    i_group[1];)
}groupMid;

typedef struct  _mod {
	char    okgb;		/* 'Y':정상 , 'N':오류	*/
	char    grec[4];        /* 그룹갯수		*/
	char	fill;		// 널바이트(struct  group    o_group[1];)
}groupMod; 


struct  _acInfo {
    char    accn[10];       /* 계좌번호                     */
    char    acnm[20];       /* 계좌명                       */
    char    aseq[2];        /* 계좌순서                     */
                            /* 00: 기본계좌                 */
                            /* 01: 화면상에 보이지 않는계좌 */
    char    pwch[1];        /* 은행연계계좌여부             */
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

#define	L_acinfo	sizeof(struct _acInfo)

