#pragma once
#pragma	pack(1)
struct	_ledgerH {
	char	size[6]{};	// 0	전문길이		I	S	X	전문전체(Size-6)
	char	type[1]{};	// 6	전문구분		I	S		B:계정계, I:정보계, F:파일송수신, N:Notice 자동갱신(R:정보계, D:계정계)
	char	encf[2]{};	// 7	압축구분		I/O	S	X	전문압축/암호화구분(00:압축,암호화 안함)
	char	dptr[3]{};	// 9	데이터 옵셋		I/O	S		입/출력 데이타가 시작되는 위치(공통헤더를 포함)
	char	svcd[8]{};	// 12	서비스코드		I	Cf	●	서비스코드               ♥ TxHead(3) of Map
	char	cono[3]{};	// 20	회사번호		I	S		회사번호 "991"
	char	usid[16]{};	// 23	사용자ID		I	C		트랜잭션을 발생시킨 사용자 고유정보 (예:사용자 접속ID)
	char	cogb[2]{};	// 39	접속분류코드		I	S		접속 접속매체 (HT : 자체HTS)
	char	fep1[12]{};	// 41	접속채널번호		I		X	접속 순차번호(3)+일련번호(9)
	char	pbid[16]{};	// 53	트랜잭션추적ID		I	Cf	X	전문 추적ID ('@'로시작)  ♥ 로그인 성공시 받은값 유지
	char	puip[12]{};	// 69	공인IP			I	S		공인IP (168.126.63.1의 경우 "168126063001" 형태)
	char	pcip[12]{};	// 81	사설IP			I	C		사설IP (168.126.63.1의 경우 "168126063001" 형태)
	char	dept[3]{};	// 93	처리지점 번호		I			처리PC가 위치한 지점
	char	brno[3]{};	// 96	지점번호		I			계좌가 속한 지점
	char	term[8]{};	// 99	단말번호		I	Cm	●	매체코드 (직원 : 001 고객 : 002)
	char	lang[1]{};	// 107	언어구분		I	S		K:한글, E:영문, C:중국, J:일본
	char	time[9]{};	// 108	AP처리시각		O			요청 처리시각 (HHMMSSmmm)
	char	ecod[4]{};	// 117	메시지코드		O			오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
	char	eknd[1]{};	// 121	출력메시지구분		O			
	char	fep2[1]{};	// 122	압축요청구분		I		X	
	char	fkey[4]{};	// 123	기능키목록		I/O	Cm	●	입력: 기능키(1바이트) ♥ TxHead(2) of Map (5:확인 7:연속 C:전송)
	char	rcnt[4]{};	// 127	요청 레코드 개수	I	Cm	●	일반적으로 "0000"을 입력
	char	fil1[6]{};	// 131	Filler			-		X	
	char	next[1]{};	// 137	연속여부		I/O	C		연속 데이터 유무 (Y:연속있음, 이외의 값:연속없음)
	char	nkey[18]{};	// 138	연속키			I/O	C	●	연속처리를 위한 키값
	char	usiz[2]{};	// 156	가변시스템정보길이	I		X	IBK 고정	
	char	hsiz[2]{};	// 158	가변헤더정보길이	I/O		X	사용 : epwd와 함께 사용
	char	msiz[2]{};	// 160	가변메시지길이		O		X	성공 및 오류메시지 (요청시 "00"입력)
	char	fep3[10]{};	// 162	Reserved for  FEP		S	X	
	char	mkty[1]{};	// 172	시장구분			Cfm	●	1:현물 2:선옵 3:기타    ♥TxHead(0) of MAP
	char	odrf[1]{};	// 173	주문구분			Cfm	●	1:주문 2:조회 3:이체    ♥TxHead(1) of MAP
	char	evid[4]{};	// 174	Reserved for  I/F			X	
	char	svrm[12]{};	// 178	서버IP				S		서버IP (168.126.63.1의 경우 "168126063001" 형태)
	char	skip[1]{};	// 190	Flag for I/F
	char	fil2[49]{};	// 191	Filler					X	
	char	eact[1]{};	// 240	에러구분					에러구분(0:상태바 1:메세지박스)
	char	emsg[99]{};	// 241	오류메세지					코드(4)+메시지(94)(9999 오류입니다 !)
	char	epwd[44]{};	// 340	sha256암호화 된 계좌비밀번호
};

#define	L_ledgerH	sizeof(struct _ledgerH)		// 384 bytes

#pragma	pack()
