// Define.h : header file
//
#pragma once
// Control Resource ID
#define IDC_TAB				1001
#define IDC_GUIDE			1002
#define IDC_BUY				1003
#define IDC_SELL			1004
#define IDC_MODIFY			1005
#define IDC_CANCEL			1006
#define IDC_EXPAND			1007
#define IDC_EXPANDBTN		1008
#define IDC_GROUPSTATIC		1009
#define IDC_GROUP			1010
#define IDC_ADDACCOUNT		1011
#define IDC_CLEAR			1012
#define IDC_REMAIN			1013
#define IDC_ORDER			1014
#define IDC_CONTRACT		1015
#define IDC_CODECTRL		1016
#define IDC_CODESEARCH		1017
#define IDC_TYPESTATIC		1018
#define IDC_TYPE			1019
#define IDC_CONDITION		1020
#define IDC_QUANTITYSTATIC	1021
#define IDC_QUANTITY		1022
#define IDC_COSTSTATIC		1023
#define IDC_COSTTYPE		1024
#define IDC_COST			1025
#define IDC_SELLGRID		1026
#define IDC_MAX				1027
#define IDC_BUYGRID			1028
#define IDC_CANCELGRID		1029
#define IDC_MODIFYGRID		1030
#define IDC_REMAINQTY		1031
#define IDC_PARTQTY			1032
#define IDC_NONCONTRACT		1033
#define IDC_CONTRACTGRID	1034
#define IDC_ORDERTYPE		1035
#define IDC_ORDERTYPESTATIC	1036
#define IDC_WARNING			1037
#define IDC_PASSSET			1038
#define IDC_QTYTYPESTATIC	1036
#define IDC_GROUPID			1037
#define IDC_ABLEQTY			1039

// Size Define
#define SZ_MAP_WIDTH		994
#define	SZ_MAP_HEIGHT		523
#define	SZ_FORM_MARGIN		3
#define SZ_TAB_HMARGIN		2
#define SZ_CTRL_VGAP		2
#define SZ_CTRL_HGAP		5
#define SZ_CTRL_HEIGHT		20

#define SZ_TAB_HEIGHT		28
#define	SZ_CTRL_ACCN		140
#define SZ_CTRL_ACCNM		64
#define SZ_CTRL_1STATIC		14
#define SZ_CTRL_2STATIC		26
#define SZ_CTRL_1BUTTON		20
#define SZ_CTRL_2BUTTON		32
#define SZ_CTRL_3BUTTON		44
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_8BUTTON		104
#define SZ_CTRL_4STATIC		50
#define SZ_CTRL_2EDIT		32
#define SZ_CTRL_3EDIT		44
#define SZ_CTRL_4EDIT		56
#define SZ_CTRL_6EDIT		80
#define SZ_CTRL_4BUTTON		56
#define SZ_CTRL_6STATIC		74
#define SZ_CHECK_5WIDTH		80
#define SZ_EXPAND_WIDTH		320
#define SZ_RADIO_1WIDTH		32
#define SZ_RADIO_2WIDTH		44
#define SZ_RADIO_3WIDTH		56
#define SZ_COMBO_2WIDTH		48
#define SZ_COMBO_3WIDTH		60
#define SZ_COMBO_4WIDTH		72
#define SZ_COMBO_5WIDTH		84
#define SZ_COMBO_6WIDTH		96
#define SZ_COMBO_7WIDTH		108
#define SZ_ROWHEIGHT		18
#define SZ_SYMBOL			110
#define SZ_CONGRID_WIDTH	135


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
#define CLR_GRID_BACK2		77
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
#define CLR_GUIDE_BACK		34
#define CLR_GUIDE_FORE		130
#define CLR_GUIDE_LINE		75


// etc
#define MAPNO_SHOGA			"IB100610"
#define MAX_SELLGRIDCOL		11
#define DEF_SELLGRIDROW		20
#define MAX_BUYGRIDCOL		11
#define DEF_BUYGRIDROW		20
#define MAX_CANCELGRIDCOL		10
#define DEF_CANCELGRIDROW		20
#define MAX_MODIFYGRIDCOL		11
#define DEF_MODIFYGRIDROW		20
#define MAX_CONTRACTGRIDCOL		4
#define DEF_CONTRACTGRIDROW		20
#define TT_GUIDE			100

// TR KEY
#define KEY_SYMBOL				101
#define KEY_REMAIN				102
#define KEY_GROUPLIST			103
#define KEY_SISE				104
#define KEY_MODIFYNCDETAIL		105
#define KEY_CANCELNCREMAIN		106
#define KEY_SELLORDER			107
#define KEY_GROUPACCOUNT		108
#define KEY_BUYORDER			109
#define KEY_MODIFYNONCONTRACT	110
#define KEY_MODIFYORDER			111
#define KEY_CANCELNONCONTRACT	112
#define KEY_CANCELNCDETAIL		113
#define KEY_CANCELORDER			114
#define KEY_BUYABLEQTY			115

#define UG_CONT_CELLCHANGE	WM_USER + 100

struct i_frem
{
	char In[5];				// 레코드갯수  
	char zUserTp[1];		// 사용자구분  
	char zUserId[16];		// 사용자ID    
	char zFundNm[40];		// 펀드명      
	char zFundKindCode[2];	// 펀드종류코드
	char zIsuNo[12];		// 종목번호    
};

struct o_frec
{
	char zAcntNo[20];		// 계좌번호    
	char zAcntNm[40];		// 계좌명      
	char zIsuNo[12];		// 종목번호    
	char zIsuNm[40];		// 종목명      
	char lOrdAbleQty[16];	// 주문가능수량
	char lBalQty[16];		// 잔고수량    
	char lUnercQty[16];		// 미체결수량  
};

struct o_frem
{
	char Out1[5];			// 레코드갯수  
	struct o_frec rec[100];
};

#define L_i_frem sizeof(i_frem)
#define L_o_frec sizeof(o_frec)
#define L_o_frem sizeof(o_frem)


/*******************************************************************************
 * 펀드조회 IO FORMAT	(그룹리스트 조회)
 ******************************************************************************/

struct gqry_mid
{
	char In[5];					// 레코드갯수   - F12 : 전송, F7 : 연속
	char zUserTp[1];			// 사용자구분	- 1 : 직원
	char zCreUserId[16];		// 생성사용자ID
	char zAcntPwd[8];			// 비밀번호
};

struct gqry_rec
{
	char zFundNm[40];			// 펀드명
	char zFundKindTp[1];		// 펀드종류구분
	char zCampDescr[100];		// 캠페인설명
};

struct gqry_mod
{
	struct gqry_mid mid;		// 입력데이터
	char Out[5];				// 레코드갯수
	struct gqry_rec rec[50];
};

#define L_gqry_mid	sizeof(gqry_mid)
#define L_gqry_mod	sizeof(gqry_mod)
#define L_gqry_rec	sizeof(gqry_rec)


/*******************************************************************************
 * 펀드종목조회 IO FORMAT	(복수계좌 조회)
 ******************************************************************************/

struct aqry_mid
{
	char In[5];					// 레코드갯수  	- F12 : 전송, F7 : 연속 
	char zUserTp[1];			// 사용자구분   - 1 : 직원, 2 : 고객  
	char zCreUserId[16];		// 생성사용자ID
	char zPwd[8];				// 비밀번호    
	char zFundNm[40];			// 펀드명      
	char zFundKindTp[1];		// 펀드종류구분	- 1 : 1 vs N, 2 : N vs 1, 3 : N vs N
};

struct aqry_rec
{
	char zAcntNo[20];			// 계좌번호        
	char zIsuNo[12];			// 종목번호        
	char lLastFundOrdNo[10];	// 최종펀드주문번호
	char zLastFundOrdDt[8];		// 최종펀드주문일  
	char zRegDate[8];			// 등록일자        
	char zRegTime[9];			// 등록시각        
	char lOrdAbleQty[16];		// 주문가능수량    
	char dRatVal[19];			// 비율값          
	char dWghtval[19];			// 가중치          
	char zPrcPtnTp[1];			// 가격유형구분    
	char zOrdprcPtnCode[2];		// 호가유형코드    
	char zOrdMthdTp[1];			// 주문방식구분    
	char lOrdQty[16];			// 주문수량        
	char dOrdPrc[13];			// 주문가          
	char zBnsTp[1];				// 매매구분        
	char zIsuNm[40];			// 종목명          
	char zAcntNm[40];			// 계좌명          
	char zPrcPtnNm[40];			// 가격유형명      
	char zOrdMthdNm[20];		// 주문방식명      
	char zBnsTpNm[10];			// 매매구분        
	char zFundDescr40[40];		// 펀드설명40      
	char zMkTp[1];				// 시장구분        
	char lMtiordSeqno[10];		// 복수주문일련번호
};

struct aqry_mod
{	
	struct aqry_mid mid;		// 입력데이터
	char ST_SONAQ039[5];		// 레코드갯수
	struct aqry_rec rec[30];
};

#define L_aqry_mid	sizeof(aqry_mid)
#define L_aqry_rec	sizeof(aqry_rec)
#define L_aqry_mod	sizeof(aqry_mod)


/*******************************************************************************
* 펀드주문상세조회
*******************************************************************************/
struct goqry_mid
{
	char In[5];				// 레코드갯수
	char zUserTp[1];		// 사용자구분
	char zCreUserId[16];	// 생성사용자ID
	char zFundNm[40];		// 펀드명
	char zFundKindTp[1];	// 펀드종류구분
	char lFundOrdNo[10];	// 펀드주문번호
	char zOrdDt[8];			// 주문일
};

struct goqry_rec
{
	char zMgmtBrnNo[3];		// 관리지점번호
	char zAcntNo[20];		// 계좌번호
	char zAcntNm[40];		// 계좌명
	char zIsuNo[12];		// 종목번호
	char zIsuNm[40];		// 종목명
	char lOrdNo[10];		// 주문번호
	char zBnsTp[1];			// 매매구분
	char zBnsTpNm[10];		// 매매구분
	char zMrcTp[1];			// 정정취소구분
	char zMrcTpNm[10];		// 정정취소구분명
	char lMrcQty[16];		// 정정취소수량
	char lOrdQty[16];		// 주문수량
	char lExecQty[16];		// 체결수량
	char dOrdPrc[13];		// 주문가
	char dAvrExecPrc[13];	// 평균체결가
	char lExecAmt[16];		// 체결금액
	char zOrdTime[9];		// 주문시각
	char lOrdTrxPtnCode[9];	// 주문처리유형코드
};

struct goqry_mod
{
	char Out[5];			// 레코드갯수
	struct goqry_rec rec[30];
};

#define L_goqry_mid	sizeof(goqry_mid)
#define L_goqry_mod	sizeof(goqry_mod)
#define L_goqry_rec	sizeof(goqry_rec)

/*******************************************************************************
* 현물펀드주문
*******************************************************************************/
struct godr_mid
{
	char In[5];				// 레코드갯수
	char zUserTp[1];		// 사용자구분
	char zUserId[16];		// 사용자ID
	char zFundNm[40];		// 펀드명
	char zIvstfndPtnTp[1];	// 투신펀드유형구분
	char lSpotOrdCnt[6];	// 현물주문횟수
	char zAcntNo[20];		// 계좌번호
	char zIsuNo[12];		// 종목번호
	char lOrdQty[16];		// 주문수량
	char dOrdPrc[13];		// 주문가
	char zBnsTp[1];			// 매매구분
	char zOrdprcPtnCode[2];	// 호가유형코드
	char zOrdCndiTp[1];		// 주문조건구분
	char zCommdaCode[2];	// 통신매체코드
	char zOpDrtnNo[12];		// 운용지시번호
	char zGrpPtnTp[1];		// 그룹유형구분
};

struct godr_mod
{
	char Out[5];			// 레코드갯수
	char lFundOrdNo[10];	// 펀드주문번호
	char lOrdNo[10];		// 주문번호
};

#define L_godr_mid	sizeof(godr_mid)
#define L_godr_mod	sizeof(godr_mod)


/*******************************************************************************
* 현물펀드정정주문
*******************************************************************************/
struct gmod_mid
{
	char In[5];				// 레코드갯수
	char zUserTp[1];		// 사용자구분
	char zUserId[16];		// 사용자ID
	char zFundNm[40];		// 펀드명
	char zIvstfndPtnTp[1];	// 투신펀드유형구분
	char lFundOrgOrdNo[10];	// 펀드원주문번호
	char lSpotOrdCnt[6];	// 현물주문횟수
	char lOrgOrdNo[10];		// 원주문번호
	char zAcntNo[20];		// 계좌번호
	char zIsuNo[12];		// 종목번호
	char lOrdQty[16];		// 주문수량
	char zOrdprcPtnCode[2];	// 호가유형코드
	char zOrdCndiTp[1];		// 주문조건구분
	char dOrdPrc[13];		// 주문가
	char zCommdaCode[2];	// 통신매체코드
	char zGrpPtnTp[1];		// 그룹유형구분
};

struct gmod_mod
{
	char Out[5];			// 레코드갯수
	char lFundOrdNo[10];	// 펀드주문번호
	char lOrdNo[10];		// 주문번호
};

#define L_gmod_mid	sizeof(gmod_mid)
#define L_gmod_mod	sizeof(gmod_mod)


/*******************************************************************************
* 현물펀드취소주문
*******************************************************************************/
struct gcan_mid
{
	char In[5];				// 레코드갯수
	char zUserTp[1];		// 사용자구분
	char zUserId[16];		// 사용자ID
	char zFundNm[40];		// 펀드명
	char zIvstfndPtnTp[1];	// 투신펀드유형구분
	char lFundOrgOrdNo[10];	// 펀드원주문번호
	char lSpotOrdCnt[6];	// 현물주문횟수
	char lOrgOrdNo[10];		// 원주문번호
	char zAcntNo[20];		// 계좌번호
	char zIsuNo[12];		// 종목번호
	char lOrdQty[16];		// 주문수량
	char zCommdaCode[2];	// 통신매체코드
	char zGrpPtnTp[1];		// 그룹유형구분
};

struct gcan_mod
{
	char Out[5];			// 레코드갯수
	char lFundOrdNo[10];	// 펀드주문번호
	char lOrdNo[10];		// 주문번호
};

#define L_gcan_mid	sizeof(gcan_mid)
#define L_gcan_mod	sizeof(gcan_mod)


/*******************************************************************************
* 펀드주문번호별주문체결조회
*******************************************************************************/
struct gntn_mid
{
	char In[5];					// 레코드갯수
	char zUserTp[1];			// 사용자구분
	char zCreUserId[16];		// 생성사용자ID
	char zFundNm[40];			// 펀드명
	char zFundKindTp[2];		// 펀드종류구분
	char lFundOrdNo[10];		// 펀드주문번호
	char zOrdDt[8];				// 주문일
};

struct gntn_rec
{
	char lFundOrdNo[10];		// 펀드주문번호
	char lFundOrgOrdNo[10];		// 펀드원주문번호	
	char zMrcTp[1];				// 정정취소구분
	char lExecQty[16];			// 전체체결수량
	char lOrdQty[16];			// 전체주문수량		
	char zTrxTime[9];			// 처리시각
	char lMrcAbleQty[16];		// 정정취소가능수량	
};

struct gntn_mod
{
	char Out[5];				// 레코드갯수
	char zFundDescr100[100];	// 펀드설명100
	char recOut[5];				// 레코드갯수
	struct gntn_rec rec[70];		
};

#define L_gntn_mid	sizeof(gntn_mid)
#define L_gntn_rec	sizeof(gntn_rec)
#define L_gntn_mod	sizeof(gntn_mod)


/*******************************************************************************
* 주문가능수량 조회 SONAQ200
*******************************************************************************/
struct sonaq200_mid
{
	char In[5];
	char zBnsTp[1];
	char zAcntNo[20];
	char zPwd[8];
	char zIsuNo[12];
	char dOrdPrc[13];
	char zCommdaCode[2];
	char zLoanPtnCode[2];
};

struct sonaq200_mod
{
	struct sonaq200_mid mid;
	char Out[5];
	char zAcntNm[40];                          /* 계좌명                          */
	char zIsuNm[40];                           /* 종목명                          */
	char zBnsTpNm[10];                         /* 매매구분                        */
	char lDps[16];                             /* 예수금                          */
	char lSubstAmt[16];                        /* 대용금액                        */
	char lMnyMgn[16];                          /* 현금증거금액                    */
	char lSubstMgn[16];                        /* 대용증거금액                    */
	char lRcvblSumAmt[16];                     /* 미수합계금액                    */
	char lRuseOrdAbleAmt[16];                  /* 재사용주문가능금액              */
	char lMnyRuseObjAmt[16];                   /* 현금재사용대상금액              */
	char lSubstRuseObjAmt[16];                 /* 대용재사용대상금액              */
	char lPrdayRuseOrdAbleAmt[16];             /* 전일재사용주문가능금액          */
	char lCrdayRuseOrdAbleAmt[16];             /* 금일재사용주문가능금액          */
	char lMnyOrdAbleAmt[16];                   /* 현금주문가능금액                */
	char lSubstOrdAbleAmt[16];                 /* 대용주문가능금액                */
	char lCrdtPldgRuseAmt[16];                 /* 신용담보재사용금액              */
	char lSeOrdAbleAmt[16];                    /* 거래소금액                      */
	char lKdqOrdAbleAmt[16];                   /* 코스닥금액                      */
	char lAbleAmt[16];                         /* 가능금액                        */
	char lMgnRat100pctOrdAbleAmt[16];          /* 증거금률100퍼센트주문가능금액   */
	char lMgnRat100OrdAbleQty[16];             /* 증거금률100퍼센트주문가능수량   */
	char dLoanPldgRat[9];                      /* 대출담보율                      */
	char dPldgMaintRat[7];                     /* 담보유지비율                    */
	char lOrdAbleQty[16];                      /* 주문가능수량                    */
	char lUnercBuyOrdAmt[16];                  /* 미체결매수주문금액              */
	char dCmsnAmtRat[11];                      /* 수수료율                        */
	char lAddCmsnAmtLevyAmt[16];               /* 추가수수료징수금액              */
	char lBalQty[16];                          /* 잔고수량                        */
	char lHtsOrdAbleAmt[16];                   /* HTS주문가능금액                 */
	char lD1Dps[16];                           /* D+1예수금                       */
	char lD2Dps[16];                           /* D2예수금                        */
	char lPrdayRuseOrdAbleQty[16];             /* 전일재사용주문가능수량          */
	char lCrdayRuseOrdAbleQty[16];             /* 금일재사용주문가능수량          */
	char lCrdtOrdAbleAmt[16];                  /* 신용주문가능금액                */
};
