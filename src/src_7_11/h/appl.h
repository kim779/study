 //*******************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2003
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : appl.h	application common header
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-05	Initial version
//		 02.00  2008-02 Upgrade version (for EU2S)
//		 02.10	2009-06 Customize version (for KATS)
//*****************************************************************************
#ifndef	_APPLCOMMON
#define	_APPLCOMMON

//
// confirm dialog : type option
//
#define	CFD_EDIT	0x00000001		// edit enable(1개인 경우)
#define	CFD_FUND	0x00000002		// fund 주문

#define	CFD_ONE		0x00000010		// 1개 주문
#define	CFD_TWO		0x00000020		// 2개 주문
#define	CFD_THREE	0x00000040		// 3개 주문
#define	CFD_OTHER	0x00000080		// 4개 이상
#define	CFD_FOUR	0x00000100		// 4개 주문
#define	CFD_HTWO	0x00000200		// 4개 주문

#define	CFD_LTPOS	0x000001000		// left top
#define	CFD_MTPOS	0x000002000		// middle top
#define	CFD_RTPOS	0x000004000		// right top
#define	CFD_LBPOS	0x000008000		// left bottom
#define	CFD_MBPOS	0x000010000		// middle bottom
#define	CFD_RBPOS	0x000020000		// right bottom
#define	CFD_WCPOS	0x000040000		// window center
#define	CFD_SCPOS	0x000080000		// screen center
#define	CFD_USPOS	0x000100000		// user define pos : CRect(x, y, 0, 0)

//
// childframe message (to m_parent->GetParent())
//
#define	WM_AXCHILD	(WM_USER + 500)

//
// message : WM_AXCHILD, LOWORD(wparam)
//
#define	apROOTDIR	0x51		// char* return : root 
#define	apLOGINID	0x52		// char* return : loginID
#define	apLOGINIDx	0x53		// char* return : loginID|desc
#define	apGUIDE1	0x54		// lparam : message
#define	apGUIDE2	0x55		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
					//                                    black, red, blue
#define	apGUIDE3	0x56		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
#define	apFIXEDA	0x57		// lparam : LOWORD(lparam) = width, HIWORD(lparam) = height
#define	apFIXEDW	0x58
#define	apFIXEDH	0x59
#define	apHOOKED	0x60		// HIWORD(wparam) 1: hooked

#define apPINCH		0x61		// 
#define	apSETCONFIG	0x62		//
#define apSETTG		0x63

//////////////////////////////////////////////////////////////////////////
/*	
apCODECANCEL	종목별 전체 취소	
apBECANCEL	직전주문 취소
apBEEDIT	직전주문 정정
apSELORDER	매도주문
apBUYORDER	매수주문
apALLORDER	청산주문
apAUTOSORT	원클릭 자동 정렬
apAUTOSTOP	자동주문 STOP
apA3MANUAL	자동주문3 수동초기화
*/
//////////////////////////////////////////////////////////////////////////
#define apCODECANCEL	0x71		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apBECANCEL	0x72		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apBEEDIT	0x73		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apSELORDER	0x74		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apBUYORDER	0x75		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apALLORDER	0x76		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apAUTOSORT	0x77		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apAUTOSTOP	0x78		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apA3MANUAL	0x79		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0

// 환경설정 INI
#define	CONFIGINI	"config.ini"	// config file
#define	ORDER		"ORDER"		// SECTION:주문
#define	CONFIRM		"CONFIRM"	// key 주문확인창 1:TRUE, 0:FALSE
#define	PRICE		"PRICE"		// key 0:상대호가, 1:현재가, 2:사용안함
#define	DISPLAY		"DISPLAY"	// SECTION
#define	FLASH		"FLASH"		// key Blink사용여부 1:TREU, 0:FALSE
//
// 주문 TRCODE
//
#define	FJUMUNTR	"pibofjum"		// 선물/옵션 주문 TR
#define	FFNDJUMUNTR	"piboffnd"		// 선물/옵션 펀드 주문 TR
#define	HJUMUNTR	"pibohjum"		// 현물주문TR
#define	PIBOJQTY	"pibojqty"		// 주문가능수량  (prev : pibo1675)
#define	HO711001	"piboacct"		// 계좌설정	 (prev : ho711001)

#define	MAX_JROW	130
#define MAX_HROW	300

#define ACC_LEN		11			// 계좌 자리수(선물,현물)


#define F_JOLEN		7			// 선물 주문 자리수
#define F_FOLEN		7			// 선물 펀드 주문 자리수
//
// 선물주문
//
struct	fojum	{
	char	jmid[6];		// 주문 ID		right align, zerofill
	char	jmgb[1];		// 주문구분
					// '1' : 매도
					// '2' : 매수
					// '3' : 매도정정
					// '4' : 매수정정
					// '5' : 매도취소
					// '6' : 매수취소
	char	accn[ACC_LEN];		// 계좌번호
	char	pswd[8];		// 비밀번호		left align, space fill
	char	codx[15];		// 종목코드		left align, space fill
	char	giho[1];		// 가격기호		가격 >= 0 : '+', 가격 < 0 : '-'
					// '+', '-'
	char	jprc[9];		// 주문단가 9(7)V99	right align, zerofill(000002345)
					// 소수점표시 안함
	char	jqty[8];		// 주문수량		right align, zerofill
	char	ojno[F_JOLEN];		// 원주문번호
	char	jgub[1];		// 주문유형
					// '1' : 지정가
					// '2' : 조건부지정가
					// '3' : 시장가					
					// '4' : 최유리지정가
					// '5' : IOC					
					// '6' : FOK
	char	jrgb[1];		// 전략구분					
					// '1' : 차익
					// '2' : 헤지
					// '3' : 기타, 일반
	char	usid[8];		// user id
	char	fdid[2];		// fund id
	char	bjgb[1];		// 주문구분
					// '1' : 바스켓
					// '2' : 차익자동주문
					// '3' : 일반주문
					// '4' : 펀드주문
					// '5' : 시스템주문
					// '7' :스팟주문  
					// '9' : 이재윤자동1~2
					// 'A' : 이재윤자동3
					// 'B' : 이종남자동1
	char	bseq[4];		// 바스켓 seqn
	char	fjno[F_FOLEN];		// 펀드 주문 번호
	char    fdix[1];                // 펀드 index
        char    curr[9];                // 현재가
	char	ptsq[3]; 		/* CD 주문횟차		*/
        char    fill[17];               // filler
};
#define	sz_FOJUM	sizeof(struct fojum)


struct	fojno	{
	char	jmid[6];		// 주문ID
	char	accn[ACC_LEN];		// 계좌번호
	char	codx[15];		// 종목코드
	char	juno[F_JOLEN];		// 주문번호
	char	ecod[4];		// error code
	char	epos[2];		// error position
	char	emsg[80];		// error message
	char	fjno[F_FOLEN];		/* 펀드 주문 번호	*/
	char	fill[13];		// filler
};
#define	sz_FOJNO	sizeof(struct fojno)

struct	fmid	{
	char	nrec[4];			// 주문갯수
	struct	fojum	fojum[MAX_JROW];	// 주문
};

struct	fmod	{
	char	nrec[4];			// 주문갯수
	struct	fojno	fojno[MAX_JROW];	// 주문응답
};


#define H_JOLEN		7			// 현물 주문 자리수
#define H_FOLEN		7			// 현물 펀드 주문 자리수
//
// 현물주문
//
struct	hjum	{
	char	jmid[4];		// 주문 ID
	char	jmgb[1];		// 주문구분
					// '1' : 매도
					// '2' : 매수
					// '3' : 정정
					// '4' : 취소
	char	accn[ACC_LEN];		// 계좌번호
	char	pswd[8];		// 비밀번호
	char	codx[12];		// 종목코드
	char	jprc[9];		// 주문단가	zerofill
	char	jqty[9];		// 주문수량	zerofill
	char	ojno[H_JOLEN];		// 원주문번호
	char	jgub[2];		// 주문유형
					// "00" : 지정가  
					// "03" : 시장가  
					// "05" : 조건부지정가  
					// "06" : 최유리지정가  
					// "07" : 최우선지정가  
					// "91" : 장전 시간외  
					// "95" : 시간외단일가  
					// "99" : 장마감시간외종가 
	char	ogub[1];		// 주문 조건   
					// '0' 없음 '1'.IOC , '2' FOK  
	char	bsno[2];		// 바스켓번호
	char	mdgb[1];		// 매도구분
					// '0' : 일반매도
					// '1' : 공매도
	char	gong[1];		// 공매도호가구분
					// '0' : 일반매도
					// '1' : 가격제한 있는 공매도=>차입주식매도 2005.07.06 변경
					// '2' : 가격제한 없는 공매도=>기타공매도 2005.07.06 변경
	char	phgb[2];		// 프로그램 호가구분
					// '00' : 일반주문
					// '01' : 차익거래
					// '02' : 비차익거래
	char	usid[8];		// user id
	char	fdid[2];		// fund id
	char	bjgb[1];		// 주문구분
					// '1' : 바스켓
					// '2' : CD주문
					// '3' : 일반주문
					// '4' : 펀드주문
					// '5' : 시스템주문
					// '7' :스팟주문  
					// '9' : 이재윤자동1~2
					// 'A' : 이재윤자동3
					// 'B' : 이종남자동1
	char	bseq[4];		// 바스켓 seqn
	char	tcod[12];		// 운영코드 ('0')
	char	fjno[H_FOLEN];		/* 펀드 주문번호		*/
	char	fdix[2];		 /* 펀드 index   */
	char	curr[9];		 /* 현재가   */
	char	ptsq[3]; 		/* 비중순서 /CD 주문횟차	*/
	char	fill[7];		// filler
};
#define	sz_HJUM		sizeof(struct hjum)

struct	hjno	{
	char	jmid[4];		// 주문ID
	char	accn[ACC_LEN];		// 계좌번호
	char	codx[12];		// 종목코드
	char	juno[H_JOLEN];		// 주문번호
	char	ecod[4];		// error code
	char	epos[2];		// error position
	char	emsg[80];		// error message
	char	fjno[H_FOLEN];		/* 펀드 주문번호		*/
	char	fill[13];		// filler
};
#define	sz_HJNO		sizeof(struct hjno)

struct	hmid	{
	char	nrec[4];		// 주문갯수
	struct	hjum	hjum[MAX_HROW];	// 주문
};

struct	hmod	{
	char	nrec[4];		// 주문갯수
	struct	hjno	hjno[MAX_HROW];	// 주문응답
};

//
// 주문가능수량
//
// 최대 20건 조회가능합니다.
// 20건 이상일시에는 재조회 하셔야 합니다.
struct  jqtyQry	{			// prev : pibo1657itr
        char    usid[8];                // 사용자 아이디
        char    code[8];                // 종목코드     
        char    jmgb[1];                // 주문유형구분 
                                        // L:지정가     
                                        // M.시장가     
                                        // C.조건부지정가
                                        // B.최유리지정가
        char    jprc[9];                // 주문가격      
        char    nrec[3];
};
#define	sz_JQTYQRY	sizeof(struct jqtyQry)
struct  jqtyQrymid {
        char    accn[ACC_LEN];               // 계좌번호             
        char    pswd[8];                // 패스워드             
};
#define sz_JQTYQRYMID	sizeof(struct jqtyQrymid)

struct  jqtyRsp {			
        char    err[1];                 // prev : pibo1657otr
        char    nrec[3];		// ERR CHK FIELD 0:정상  1:에러
};
#define	sz_JQTYRSP	sizeof(struct jqtyRsp)
struct  jqtyRspmod {
        char    accn[ACC_LEN];               // 계좌번호             
        char    dqty[10];               // 매도가능수량         
        char    sqty[10];               // 매수가능수량         
};
#define	sz_JQTYRSPMOD	sizeof(struct jqtyRspmod)

//
// 계좌설정
//
struct	acctQry	{			// prev : ho711001itr
	char	gubn[1];		// dir
					// 1: 조회
					// 2: 변경
	char	accn[ACC_LEN];		// 계좌번호
	char	limt[4];		// 옵션매도매수한계	99V99
	char	jlmt[4];		// 선물매도매수한계	99V99
	char	jmjc[9];		// 주문기준 수량	right align, zerofill
};
#define	sz_ACCTQRY	sizeof(struct acctQry)

struct	acctRsp	{			// prev : ho711001otr
	char	err[1];			// ERR CHK FIELD
					// 0:정상 1:에러 
	char	limt[4];		// 옵션매도매수한계	99V99
	char	jlmt[4];		// 선물매도매수한계	99V99
	char	jmjc[9];		// 주문기준 수량	right align, zerofill
};
#define	sz_ACCTRSP	sizeof(struct acctRsp)

#endif