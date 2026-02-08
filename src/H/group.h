#pragma once

#pragma pack(1)
#define	TR_GROUP	"PIDOSETA"	//  server TR

// MAX Count
#define	MAX_GP		100			// 최대 그룹갯수
#define	MAX_EDIT	40			// 최대 계좌 등록갯수

//======================================================================
// group I/O : ihead는 size큰 경우에만 사용(14K이상)
// I : _gmid + { _group + _accn * 등록계좌수 } * 그룹수
// 14K보다 큰 데이타 저장시 _ihead + 저장값(14K)씩 서버로 보냄
// O : _gmod
//
// piioseta.c
//======================================================================
struct  _ihead  {
	char	xflg[1]{};		// 'F':first, 'N':next, 'E':end(이것은 14K이상일때만 적용)
	char	csiz[6]{};		// 보낼 size	
	char	fsiz[6]{};		// file size
	char	fill[3]{};
};
#define	L_ihead		sizeof(struct _ihead)

struct  _gmid {
	char    xflg{};			// 1: 조회
					// 2: 저장
	char	usrid[12]{};		// 사원번호
	char	sflag{};			// 1: 주식
					// 2: 선물/옵션
	char    grec[4]{};		// 그룹갯수
//	char	fill;			// 널바이트(struct  group    i_group[1];)
};
#define	L_gmid		sizeof(struct _gmid)

struct  _gmod {
	char    okgb{};			// 'Y':정상 , 'N':오류
	char    grec[4]{};		// 그룹갯수
//	char	fill;			// 널바이트(struct  group    o_group[1];)
};
#define	L_gmod		sizeof(struct _gmod)

struct  _group {
	char	xflg{};			// 'U': Update, 'D': Delete, 'I': Insert
	char	gpid[3]{};		// 계좌그룹아이디
	char    seqn[3]{};		// 계좌그룹일련번호
	char    gnam[20]{};		// 그룹명
	char    nrec[4]{};		// 계좌 COUNT
//	char	fill;			// 널바이트(struct  accn    accn;)
};	
#define	L_group		sizeof(struct _group)

struct  _accn {
	char    seqn[3]{};		// 계좌일련번호
	char    accn[11]{};		// 계좌번호
	char	pass[10]{};	// 계좌 비밀번호
	char    acnm[30]{};	// 계좌명
	char    rate[5]{};	// 배정비율
	char    multi[7]{};	// 승수
	char    alis[30]{};	// 계좌별칭
	char    prea{};		// 1:대표계좌
//	char	fill;		// 널바이트
};
#define	L_accn		sizeof(struct _accn)

struct _acif {
	char	acno[11]{};
	char	gubn[2]{};
};
#define	L_acif		sizeof(struct _acif)

struct _acif_out {
	char	grcd[2]{};
	char	grnm[40]{};
	char	regy[1]{};
	char	lgnm[100]{};
	char	prnm[100]{};
};
#define	L_acif_out		sizeof(struct _acif_out)

#pragma pack()