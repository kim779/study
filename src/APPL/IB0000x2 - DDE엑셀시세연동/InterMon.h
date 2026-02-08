#pragma once

#include "axisfire.h"

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

struct _foldex{
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
    char    ngrs[ 2];           /* New Group Seqn       */
    char    ogrs[ 2];           /* Old Group Seqn       */
	char	gnam[30];			
};

struct  _gfoldex {
    char    nrec[ 4];           /* Group Count          */
    struct  _glistex glist[MAX_GR];
};

struct  _updn2 {
	_uinfo	uinfo;      /* 사용자 정보 HEAD     */
	_gfoldex gfoldex;
 };

/////////////////////////////11.04 신규/////////////////////////
struct jlist {          /* 관심종목             */
    char    gubn[1];            /* 종목구분             */
    char    code[12];           /* 종목코드             */
	/* 현물(6), 선물/옵션(8)*/
	/* 하이일드펀드(9)      */
	/* 업종코드(5)          */
    char    bqty[10];           /* 보유수량             */
    char    bprc[10];           /* 매수가격             */
};

//struct fold {          /* 관심종목             */
//    char    kjid[2];            /* 관심그룹코드(00~99)  */
//    char    name[20];           /* 관심그룹명           */
//    char    nrec[4];            /* 종목갯수(space 포함) */
//    struct  jlist   jlist[MAX_GR];
//};


const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);
const int sz_updn2    = sizeof(struct _updn2);
const int sz_foldex    = sizeof(struct _foldex);
const int sz_gfoldex    = sizeof(struct _gfoldex);
const int sz_glistex    = sizeof(struct _glistex);

//
//	하나증권 관심종목 파일포맷
//
struct	_hanasec {
	char	head[17];	// "INTERESTGROUPFILE"
	char	gnox[2];	// 그룹개수 (개수만큼 반복)

	char	gnam[40];	// 그룹이름
	char	jrec[2];	// 종목갯수 (개수만큼 반복)

	char	code[15];	// 종목코드
};

const int sz_hanas    = sizeof(struct _hanasec);

#define TRKEY_GROUP		0x13	// 관심그룹...
#define TRKEY_INTER		0x14	// 관심리스트...


#pragma pack(1)
constexpr int codelen = 12;
constexpr int namelen = 32;
constexpr int pricelen = 10;
constexpr int xnumlen = 10;

struct _inters
{
	char	gubn[1]{};	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수	
	char	code[12]{};	// 종목코드
	char	name[32]{};	// 종목명
	char	xprc[10]{};	// 보유단가
	char	xnum[10]{};	// 보유수량
	char	xupnum[16]{};	// 상장 주식수
	char	bookmark[1]{};	// 북마크
	char	futureGubn[1]{};	// 신용매매 구분
	double  creditPrc{};		// 신용금액
	double	maeipPrc{};		// 매입금액
	char	filler[6]{};		// Reserved
};


struct _intersx
{
	char	gubn = '0';		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수	
	CString code;			// 종목코드
	CString name;			// 종목명
	CString	xprc;			// 보유단가
	CString	xnum;			// 보유수량
	CString	xupnum;			// 상장 주식수
	char	bookmark = '0';		// 북마크
	char	futureGubn{};		// 신용매매 구분
	double  creditPrc{};		// 신용금액
	double	maeipPrc{};		// 매입금액

	void copy(void* item);
	void operator=(struct _intersx& ref)
	{
		code = ref.code;
		name = ref.name;
		gubn = ref.gubn;
		xprc = ref.xprc;
		xnum = ref.xnum;
		xupnum = ref.xupnum;
		bookmark = ref.bookmark;
		futureGubn = ref.futureGubn;
		creditPrc = ref.creditPrc;
		maeipPrc = ref.maeipPrc;
	}

	void empty()
	{
		code.Empty();
		name.Empty();
		gubn = '0';
		xprc.Empty();
		xnum.Empty();
		xupnum.Empty();
		bookmark = '0';
		futureGubn = '0';
		creditPrc = 0;
		maeipPrc = 0;
	}
};

struct	_bookmarkinfo {
	char	gubn[1]{};		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12]{};		// 종목코드
	char	name[32]{};		// 종목명
	char	bookmark[1]{};		// 북마크 여부
};
#define	sz_inters	sizeof(struct _inters)
#define	sz_bookmark	sizeof(struct _bookmarkinfo)

struct _codeinfo
{
	char	code[12]{};	// 종목코드
	char	name[32]{};	// 종목명
	char	capture[1]{};	// 실행여부 	'0':포착전, '1':포착완료

	struct _alarm
	{
		char	kind[1]{};// 감시항목	0:none, 1:현재가, 2:전일대비, 3:매도호가, 4:매수호가, 5:등락률, 6:거래량
		char	aval[10]{};//감시비교값
		char	cond[1]{};// 감시조건	'0':이상, '1':이하
		char	oper[1]{};// 감시연산	'0':AND,  '1':OR
	}alarm[6]{};

	char	o_run[1]{};	// 주문창	'0':미실행,'1':실행
	char	o_kind[1]{};	// 주문구분	'0':매도,  '1':매수
	char	o_mmgb[1]{};	// 매매구분	'0':지정가,'1':시장가
	char	o_num[10]{};	// 주문수량
	char	o_prc[10]{};	// 주문단가

	char	a_kind[1]{};	// 알람설정	0x01: 확인창, 0x02: 관심종목신호적용 0x04:소리적용
	char	a_wave[128]{};	// 소리파일
	char	filler[20]{};	// Reserved
};

#define	sz_codeinfo	sizeof(struct _codeinfo)

struct	_hisinfo
{
	char	date[14]{};	// 포착일자,시간
	struct	_codeinfo	codeinfo {};
	char	gval[6][10]{};	// 포착시 가격
	char	chk[6]{};		// check
};
#define	sz_hisinfo	sizeof(struct _hisinfo)


class InterMon
{
public :

	InterMon(CWnd* pParent, CWnd* pOwner) {
		m_parent = pParent;
		m_owner = pOwner;
	}
	

	CWnd* m_parent{};
	CWnd* m_owner{};

	int m_group_count = 0;

	std::vector<std::pair<CString, CString>> m_groupname;
	CArray <std::shared_ptr<_inters>, std::shared_ptr<_inters>> m_inters;

	void Request_GroupList();
	void Request_GroupCode(int iseq, int vidx);
	void receiveOub(int key, CString data, int vidx);
	void sendTR(CString trCode, char* datB, int datL, int key);
};
