#ifndef	_LEDGERDAETO_H_
#define	_LEDGERDAETO_H_

struct	_ledgerDaetoH {
	char	tran[4];	//   0	tr_code			TR CODE(화면번호)
	char	svcn[10];	//   4	svc_name		TUXEDO Service Name
	char	svr[2];		//  14	src_svr			Channel Server
				//				T1:업무계, T2:Call Center, H1:HTS(영업점), H2:HTS(고객),  W1:Wrap
				//				M1:MTS,    H3:WTS,         I1:인터넷뱅킹,  R1:ARS,        P1:011
				//				P6:016,    P7:017,         P8:018,         P9:019,        N1:AirPost
				//				N2:Micess, K1:방카,        X1:CRM,         E1:ERP,        Z1:RM
				//				N3:PDA,    B1:은행(CD),    B2:은행(기타),  C1:현금지급기, D1:시스템
				//				F1:FIX,    I2:홈페이지
	char	pgm[8];		//  16	pgm_id			Program ID

	char	idno[12];	//  24	id_no			사번
	char	regno[13];	//  36	reg_no			사용자 주민등록번호
	char	group[2];	//  49	emp_grp			사용자 그룹
	char	open[3];	//  51	open_dept		소속점
	char	dept[3];	//  54	dept_cd			부서 (처리점)
	char	term[8];	//  57	term_id			단말기번호
	char	ips[15];	//  65	ip_no			IP Address

	char	media[1];	//  80	mdr_cd			입력매체구분 (0:수기, 1:카드, 2:통장, 3:책임자카드)
	char	gubn[1];	//  81	job_cd			작업구분 (1:Query, 2:Insert, 3:Update, 4:Delete)
	char	rows[3];	//  82	max_row			GRID MAX ROW
	char	book[10];	//  85	book_seq		통장번호
	char	card[8];	//  95	card_seq		카드일련번호
	char	report[1];	// 103	rpt_tool_use_cd		레포팅툴 사용구분 (0:TR, 1:Use Tool)

	char	optp[1];	// 104	mgr_appr_tp		책임자 승인구분 (0:발생전, 1:대상 및 요청, 2:승인, 3:취소)
	char	opid[5];	// 105	mgr_appr_empno		승인 책임자 사번
	char	optm[8];	// 110	mgr_appr_term_id	책임자승인단말번호
	char	opno[5];	// 118	mgr_appr_seqno		책임자 승인번호
	char	opn[1];		// 123	mgr_appr_cnt		책임자승인건수
	char	opgb[1];	// 124	mgr_job_cd		책임자 업무구분
	char	opcd[1];	// 125	mgr_card_cd		책임자카드구분 (1:지점장, 3:책임자)

	char	func[1];	// 126	tr_fnkey		처리기능구분(사용자 지정)
	char	ecode[6];	// 127	tr_err_code		에러코드
	char	etype[1];	// 133	tr_err_cd		에러구분 (0:상태바, 1:메세지박스, 3:메세지처리없음)
	char	msg[130];	// 134	tr_err_msg		에러메시지
	char	contf[1];	// 264	tr_cont_yn		연속거래구분 (0:정상, 1:연속거래)

	char	nrec[4];	// 265	tr_cnt			처리건수
	char	keys[6];	// 269	lst_key			row 처리 last key
	char	next[50];	// 275	next_key		next key
	char	svcno[4];	// 325	svc_no			service no
	char	mts[5];		// 329	mts_key			mts key
	char	rsv[66];	// 334	filler
};

#define	L_ledgerDaetoH	sizeof(struct _ledgerDaetoH)		// 400 bytes

#endif