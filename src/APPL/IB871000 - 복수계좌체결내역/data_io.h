#pragma once

#define TK_PIDOSETA		0x01
#define TK_PIBOCHEG		0x02

struct  pidoseta_accn {
    char    seqn[3];        /* 계좌일련번호                 */
    char    accn[11];       /* 계좌번호                     */
    char    pass[10];       /* 계좌비밀번호                 */
    char    acnm[30];       /* 계좌명                       */
    char    rate[5];        /* 배정비율                     */
    char    maxq[7];        /* 최대주문수량 for 법인주문    */
    char    alis[30];       /* 계좌별칭                     */
    char    pres[1];        /* 1:대표계좌                   */
};

struct  pidoseta_grop {
    char    xflg[1];        /* 'D' Delete    'I' Insert     */
							/* 'U' Update or Still          */
							/* OUTPUT 사용: group fg        */
    char    gpid[3];        /* 계좌그룹아이디               */
    char    seqn[3];        /* 계좌그룹일련번호             */
    char    gnam[20];       /* 그룹명                       */
    char    nrec[4];        /* 계좌 COUNT                   */
    struct  pidoseta_accn  accn[1];
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

struct	piboscheg_mid	
{
	char    gubn[1];	//2015.05.28 KSJ 구분자 '>' 넣어줌.
	char    accn[11];
	char    pswd[8];
	char    date[8];
	char    cod2[12];
	char    juno[5];	// 시작주분번호               //2015.05.28 KSJ 번호 5 -> 10 
	char    dsgb[1];	// 0.All 1.매도 2.매수      
	char    otim[1];	// 0.All 1.장중 2.시간외    
	char	sygb[1];	// 0.All 1.신용 2.담보
	char	dllf[1];	// 0.MAP 1.DLL
	char    dlgb[1];	// 0.All 1.체결 2.미체결    
	char    sort[1];	// 1.역순 2.정순            
	char    juno2[10];	// 시작주분번호2               //2015.05.28 KSJ 번호 5 -> 10 
};
#define sz_MICHEMID		sizeof(struct piboscheg_mid)

struct	piboscheg_grid	{
	char    juno[10];    // 주문   번호      //2015.05.28 KSJ 번호 5 -> 10 
	char    ojno[10];    // 원주문 번호      //2015.05.28 KSJ 번호 5 -> 10 
	char    mkgb[1];     //시장구분
	char    cod2[12];   // 종목코드         
	char    hnam[40];   // 종목명           
	char    odgb[20];   // 주문구분         
	char    jcgb[20];   // 정정취소구분 
	char    hogb[20];   // 호가구분         
	char    jprc[12];   // 주문가격         
	char    jqty[12];   // 주문수량         
	char    dprc[12];   // 체결가격         
	char    dqty[12];   // 체결수량         
	char    tqty[12];   // 체결수량합       
	char    wqty[12];   // 미체결수량       
	char    stopPrc[12];   // 스톱가격
	char    stopStat[20];   //스톱유형
	char    stat[20];   // 접수상태         
	char    time[8];    // 주문시간         
};

struct	piboscheg_mod	
{
	char	accn[11];
	char	nrec[4];
	struct	piboscheg_grid	rec[250];
};

