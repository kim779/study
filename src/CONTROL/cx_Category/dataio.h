#ifndef _DATAIO_H_
#define _DATAIO_H_

//
//	관심종목 UPLOAD / DOWNLOAD
//

#define	trUPDOWN	"pidomyst"

struct	_uinfo {
	char	gubn[2];	// 업무구분
				// "MY" : 관심종목
				// "UL" : 사용자 데이터 목록 
				// "UD" : 사용자 데이터
	char	dirt[1];	// 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char	cont[1];	// 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
				// 'S':그룹편집, 'E':그룹조합처리, 'b':DB백업, 'R':DB원복, 'V':최종상태저장, 'r':최종상태복원
				// 'D':그룹삭제, 'j':해당그룹종목전송, 'g':그룹리스트조회, 'G':그룹종목전송, 'C':전송취소
	char	name[80];	// file name
	char	nblc[5];	// 블럭개수, 파일크기(max:64k)
	char	retc[1];	// return value 'O':정상 'E':에러 'U'신규 정상
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	xprc[10];	// 보유단가
	char	xnum[10];	// 보유수량
};

struct _foldex {
	char	gubn[1];	//입력구분 D, G
	char	kjid[2];	//관심그룹 시퀀스
	char	name[20];	//관심그룹명
	char	nrec[4];	//종목개수
};

struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

#define MAX_GR 100

struct  _glistex {
	char	ngrs[2];	// New Group Seqn
	char	ogrs[2];	// Old Group Seqn
	char	gnam[30];
};

struct  _gfoldex {
	char	nrec[4];	// Group Count
	struct	_glistex glist[MAX_GR];
};

struct  _updn2 {
	_uinfo	uinfo;		// 사용자 정보 HEAD
	_gfoldex gfoldex;
};

/////////////////////////////11.04 신규/////////////////////////
struct	jlist {			// 관심종목
	char	gubn[1];	// 종목구분
	char	code[12];	// 종목코드
				// 현물(6), 선물/옵션(8)
				// 하이일드펀드(9)
				// 업종코드(5)
	char	bqty[10];	// 보유수량
	char	bprc[10];	// 매수가격
};

struct	fold {			// 관심종목
	char	kjid[2];	// 관심그룹코드(00~99)
	char	name[20];	// 관심그룹명
	char	nrec[4];	// 종목갯수(space 포함)
	struct	jlist   jlist[MAX_GR];
};


const int sz_uinfo =	sizeof(struct _uinfo);
const int sz_ginfo =	sizeof(struct _ginfo);
const int sz_jinfo =	sizeof(struct _jinfo);
const int sz_updn =	sizeof(struct _updn);
const int sz_updn2 =	sizeof(struct _updn2);
const int sz_foldex =	sizeof(struct _foldex);
const int sz_gfoldex =	sizeof(struct _gfoldex);
const int sz_glistex =	sizeof(struct _glistex);

#endif