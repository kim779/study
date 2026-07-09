#ifndef	_LEDGERHANA_H_
#define	_LEDGERHANA_H_

struct	_ledgerHanaH {
	char	tran[8];	// 0	서비스 코드 : TR Code
	char	gubn[1];	// 8	작업구분 (Q:query, S:save, ..)
	char	nrec[6];	// 9	처리건수
	char	ecode[6];	// 15	에러코드
	char	etype[1];	// 21	에러구분 (0:상태바, 1:메세지박스, 2:정상)
	char	contf[1];	// 22	연속거래구분 (0:정상, 1:연속거래)
	char	msg[130];	// 23	에러메시지(88->130), 정상일 경우도 메세지 있음.
	char	next[50];	// 153	연속거래키
	char	opid[12];	// 203	사용자 (ID or 사번)
	char	media[2];	// 215	매체구분
	char	term[8];	// 217	단말번호
	char	ips[15];	// 225	IP Address
	char	mac[15];	// 240	MAC Address
	char	isca[1];	// 255	공인인증포함여부 (0:미포함, 1:포함)
	char	time_r[12];	// 256	수신시간 (HH:MM:SS)
	char	time_s[12];	// 268	송신시간 (HH:MM:SS)
	char	screen[4];	// 280	화면번호
	char	branch[3];	// 284	소속점
	char	branch2[3];	// 287	처리점
	char	scall[4];	// 290	Service Call No.
	char	epos[3];	// 294	에러위치
	char	mediaf[1];	// 297	입력매체구분
	char	opgb[3];	// 298	책임자 업무구분
	char	opcd[1];	// 301	책임자카드구분
	char	opcn[3];	// 302	책임자카드일련번호
	char	optm[8];	// 305	책임자승인단말번호
	char	opn[1];		// 313	책임자승인건수
	char	dt_svnm[10];	// 314	tuxedo service명
	char	dt_ccgb[2];	// 324	Channel Server
	char	dt_jmno[13];	// 326	사용자 주민등록번호
	char	dt_group[2];	// 339	사용자 그룹
	char	dt_imgb[1];	// 341	입력매체구분
	char	dt_rcnt[3];	// 342	GRID MAX ROW
	char	dt_bnkno[10];	// 345	통장번호
	char	dt_cadno[8];	// 355	카드일련번호
	char	dt_rptgb[1];	// 363	레포팅툴 사용구분
	char	dt_mgno[5];	// 364	승인책임자 사번
	char	dt_mgokno[5];	// 369	책임자승인번호
	char	dt_mgjogb[1];	// 374	책임자 업무구분
	char	dt_fnkey[1];	// 375	처리기능구분(사용자지정)
	char	dt_dcnt[4];	// 376	치러건수
	char	dt_lkey[6];	// 380	row처리 last key
	char	filler[64];	// 386	filler
};

#define	L_ledgerHanaH	sizeof(struct _ledgerHanaH)

#endif