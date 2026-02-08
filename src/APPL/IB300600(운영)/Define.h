#pragma once
// Define.h : header file
//

// Control Resource ID
#define IDC_TAB					1001
#define IDC_GUIDE				1002
#define IDC_ORDER				1003
#define IDC_CLEAR				1004
#define IDC_MODCAN				1005
#define IDC_EXPAND				1006
#define	IDC_ACCOUNT				1007
#define	IDC_ACCOUNTNAME			1008
#define IDC_PASSSTATIC			1009
#define IDC_PASSWORD			1010
#define IDC_CLEAR2				1011
#define IDC_REMAIN				1012
#define IDC_ORDERSEND			1013
#define IDC_EXPANDBTN			1014
#define IDC_SELL				1015
#define	IDC_BUY					1016
#define IDC_ORDERTYPESTATIC		1017
#define IDC_ORDERTYPE			1018
#define IDC_ORDERPRICESTATIC	1019
#define IDC_ORDERPRICETYPE		1020
#define IDC_ORDERPRICE			1021
#define IDC_ORDERSPIN			1022
#define IDC_QUANTITYSTATIC		1023
#define IDC_QUANTITY			1024
#define IDC_QUANTITYSPIN		1025
#define IDC_ORDERGRID			1026
#define IDC_CODEEDIT			1027
#define IDC_CODECTRL			1028
#define IDC_CODEBUTTON			1029
#define IDC_MODCANPRICESTATIC	1030
#define IDC_MODCANPRICETYPE		1031
#define	IDC_NONCONT				1032
#define	IDC_MODIFY				1033
#define	IDC_CANCELMODE			1034
#define IDC_MODCANGRID			1035
#define IDC_CONFIRMGRID			1036
#define IDC_MSG					1037
#define IDC_CONTRACT			1038


// Size Define
#define SZ_MAP_WIDTH		960
#define	SZ_MAP_HEIGHT		378
#define	SZ_FORM_MARGIN		3
#define SZ_TAB_HMARGIN		2
#define SZ_CTRL_VGAP		2
#define SZ_CTRL_HGAP		5
#define SZ_CTRL_HEIGHT		20
#define	SZ_CTRL_ACCN		140
#define SZ_CTRL_ACCNM		64
#define SZ_CTRL_2STATIC		26
#define SZ_CTRL_3BUTTON		44
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_4STATIC		50
#define SZ_CTRL_3EDIT		44
#define SZ_CTRL_4EDIT		56
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_6STATIC		74
#define SZ_CTRL_1BUTTON		20
#define SZ_EXPAND_WIDTH		320
#define SZ_RADIO_2WIDTH		44
#define SZ_COMBO_3WIDTH		60
#define SZ_COMBO_5WIDTH		84
#define SZ_ROWHEIGHT		18

// Color Index Define
#define CLR_FORM_BACK		64
#define CLR_PANNEL_BACK		66
#define CLR_PANNEL_LINE		67
#define	CLR_STATIC_BACK		78
#define	CLR_STATIC_FORE		69
#define	CLR_STATIC_LINE		65
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define CLR_GRID_TEXT		69
#define CLR_GRID_BACK		68
#define CLR_GRID_LINE		75
#define CLR_GRID_HBACK		74
#define CLR_GRID_HTEXT		76
#define CLR_GRID_SBACK		78
#define CLR_GRID_STEXT		69
#define CLR_DATA_UP			94
#define CLR_DATA_DOWN		95
#define CLR_SYMBOL_BACK		160
#define CLR_SYMBOL_FORE		69
#define CLR_SYMBOL_LINE		89
#define CLR_GUIDE_BACK		66
#define CLR_GUIDE_FORE		69
#define CLR_GUIDE_LINE		75

// etc
#define MAX_ORDERGRIDCOL	11
#define DEF_ORDERGRIDROW	13
#define MAX_MODCANGRIDCOL	11
#define DEF_MODCANGRIDROW	13
#define MAX_CONFIRMGRIDCOL	7
#define DEF_CONFIRMROW		7

//cell 'param' values, used as a style field
#define DROPLIST_ORDERSTATE		100
#define DROPLIST_ORDERTYPE		101
#define	DROPLIST_MODCANSTATE	100
#define DROPLIST_MODCANTYPE		101
#define CELLTYPE_IS_EDITABLE	120
#define SPIN_QUANTITY			200
#define SPIN_ORDERPRICE			201

#define MAPNO_FHOGA			"IB300610"
#define MAPNO_CLEAR			"IB320700"

#define SYMBOL_HOGA			"3006_fpric"

#define TT_GUIDE			100

// TR Key
#define KEY_ACCOUNT			255
#define KEY_ACCNAME			254
#define KEY_FREMAIN			100
#define KEY_FOHOGA			101
#define KEY_FOCURR			102
#define KEY_FORDER			103
#define	KEY_NONCONT			104
#define KEY_MODCANFOHOGA	105
#define KEY_MODCANFOCURR	106
#define KEY_MODCANFORDER	107

/*******************************************************************************
 * 선물/옵션 잔고평가 조회 IO FORMAT
 ******************************************************************************/
struct	f_mid	{		// 선물 Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	dsgb[6];	/* 구분			*/
	
	char	bqty[10];	/* 수량			*/
	char	cqty[10];	/* 매도가능수량		*/
	char	pprc[10];	/* 평균가/정산가	*/
	char	curr[10];	/* 현재가		*/
	char	diff[10];	/* 평균가 대비		*/
	char	pamt[15];	/* 평가금액		*/
	char	psam[15];	/* 평가손익		*/
	char	prat[10];	/* 수익률		*/
	char	tamt[15];	/* 매입금액		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[1];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD};

/*******************************************************************************
 * 선물/옵션 현재가 or 상대/우선 호가 조회
 ******************************************************************************/
struct  i_hoga {
    char    dsgb[1];            /* '1' 매도  '2' 매수   */
    char    cod2[8];            /* 종목코드             */
};

struct  fhmid {
    char    gubn[2];            /* 호가구분             */
                                /* '00' : 현재가        */
                                /* '10' : 우선1호가     */
                                /* '11' : 우선2호가     */
                                /* '12' : 우선3호가     */
                                /* '20' : 상대1호가     */
                                /* '21' : 상대2호가     */
                                /* '22' : 상대3호가     */
    char    nrec[4];            /* 레코드 갯수          */
    struct  i_hoga  rec[999];
};

struct  o_hoga {
    char    cod2[8];            /* 종목코드             */
    char    curr[9];            /* 현재가               */
    char    hoga[9];            /* 호가                 */
};

struct  fhmod {
    char    nrec[4];
    struct  o_hoga rec[999];
}; 


#define L_fhmid	sizeof(struct fhmid)
#define L_i_hoga	sizeof(struct i_hoga)
#define L_fhmod	sizeof(struct fhmod)
#define L_o_hoga	sizeof(struct o_hoga)

/*******************************************************************************
 * 주문 전송
 ******************************************************************************/
#define MAX_REC     150

struct irec
{
    char    odgb[1];    /* 주문유형 : 1:현물 2:선물옵션 3:예약주문          */
    char    mkgb[1];    /* filler           */
    char    mmgb[1];    /* 매매구분 : 1:매도 2:매수 3:정정 4: 취소          */
    char    accn[11];   /* 계좌번호         */
    char    pswd[8];    /* 비밀번호         */
    char    ojno[12];   /* 원주문번호       */
    char    cod2[12];   /* 단축코드         */
    char    jqty[8];    /* 주문수량         */
    char    jprc[10];   /* 주문단가         */
    char    hogb[2];    /* 호가구분         */
    char    cdgb[1];    /* 주문조건 : 0:없슴 1:IOC 2:FOK                    */
    char    mdgb[1];    /* 정정취소 : 1:일부 2:전부                         */
    char    prgb[1];    /* 처리구분 : 'A,B,C,D' : 이하 가변 'X' : 사용안함  */
    char    usrf[40];
};

struct i_com
{
    char    nrec[4];    /* 주문 갯수        */
    struct  irec    rec[MAX_REC];
};

struct orec
{
    char    jmno[6];    /* 주문 번호        */
    char    ojno[6];    /* 원주문번호       */
    char    emsg[80];   /* 오류 메세지      */
};

struct o_com   
{
    char    nrec[4];
    struct  orec    rec[MAX_REC];
};

#define L_i_com		sizeof(struct i_com)
#define L_o_com		sizeof(struct o_com)
#define L_irec		sizeof(struct irec)
#define L_orec		sizeof(struct orec)

/*******************************************************************************
 * 선물/옵션 미체결 조회 IO FORMAT
 ******************************************************************************/
struct f_cmid {
	char	accn[11];
	char	pswd[8];
	char	gubn[1];	/* 0.All 1.체결 2.미체결    */
	char	juno[6];
	char	sort[1];	/* 0.X   1.역순 2.정순      */
	char	date[8];	
	char	cod2[8];	/* 종목코드                 */
    char    mkgb[1];    /* 0.All F.선물 C.콜옵션 P.풋옵션 S.스프레드 X.특정종목 */
    char    dsgb[1];    /* 0.All 1.매도 2.매수      */
};

struct f_crec {
	char	mono[6];	/* 원주문번호		*/
	char	juno[6];	/* 주문  번호		*/
	char	ojno[6];	/* 원주문 번호		*/
	char	cod2[8];	/* 종목코드			*/
	char	hnam[30];	/* 종목명			*/
	char	odgb[8];	/* 주문구분			*/
	char	hogb[20];	/* 주문유형			*/
	char	jprc[11];	/* 주문가격			*/
	char	jqty[7];	/* 주문수량			*/
	char	dlgb[4];	/* 체결구분			*/
	char	dprc[11];	/* 체결가격			*/
	char	dqty[7];	/* 체결수량			*/
	char    dtim[6];    /* 체결시간         */
	char	wqty[7];	/* 미체결수량		*/
    char    hqty[7];    /* 정정/취소수량    */
	char	stat[8];	/* 접수상태			*/
	char	time[6];	/* 접수시간			*/
    char    jseq[6];    /* 접수번호         */
    char    yseq[7];    /* 약정번호         */
    char    ecod[4];    /* 거부코드         */
};

struct	f_cmod {
	char	accn[11];
	char	nrec[4];
	struct	f_crec rec[1];
};

#define L_fcmid	sizeof(struct f_cmid)				// Length of Future input
#define L_fcmod	sizeof(struct f_cmod) - sizeof(struct f_crec)	// Length of Future output
#define L_fcrec	sizeof(struct f_crec)				// Length of Future Grid