// Define.h : header file
//

// Control Resource ID
#define IDC_ACCTITLE			1001
#define IDC_GROUPTITLE			1002
#define IDC_SELTITLE			1003
#define IDC_ACCGRID				1004
#define IDC_LTOR				1005
#define IDC_GROUPGRID			1006
#define IDC_SELGRID				1007
#define IDC_IDSTATIC			1008
#define IDC_ID					1009
#define IDC_NAMESTATIC			1010
#define IDC_NAME				1011
#define IDC_ADDMODIFY			1012
#define IDC_GROUPDELETE			1013
#define IDC_GROUPUP				1014
#define IDC_GROUPDOWN			1015
#define IDC_ACCSTATIC			1016
#define IDC_ACCOUNT				1017
#define IDC_RATESTATIC			1018
#define IDC_RATE				1019
#define IDC_RATESPIN			1020
#define IDC_SELMODIFY			1021
#define IDC_SELDELETE			1022
#define IDC_SELUP				1023
#define IDC_SELDOWN				1024

// Size Define
#define SZ_MAP_WIDTH			642
#define	SZ_MAP_HEIGHT			450
#define	SZ_FORM_MARGIN			6
#define SZ_CTRL_ACCWIDTH		250
#define SZ_CTRL_RIGHTWIDTH		310
#define SZ_CTRL_GROUPHEIGHT		90
#define SZ_CTRL_SELHEIGHT		210
#define SZ_CTRL_VGAP			2
#define SZ_CTRL_HGAP			5
#define SZ_CTRL_HEIGHT			20
#define SZ_CTRL_1BUTTON			20
#define SZ_CTRL_4BUTTON			60
#define SZ_CTRL_1STATIC			24
#define SZ_CTRL_2STATIC			36
#define SZ_CTRL_3STATIC			48
#define SZ_CTRL_2EDIT			28
#define SZ_CTRL_4EDIT			52
#define SZ_CTRL_5EDIT			64
#define SZ_CTRL_7EDIT			88
#define SZ_CTRL_2BUTTON			36
#define SZ_CTRL_3BUTTON			48
#define SZ_CTRL_5BUTTON			72
#define SZ_CTRL_7BUTTON			96
#define SZ_CTRL_8STATIC			92
#define SZ_COL_ACCOUNT			116
#define SZ_COL_ACCNAME			116
#define SZ_COL_CODE				64
#define	SZ_ROWHEIGHT			18
#define SZ_COL_GROUPID			48
#define SZ_COL_GROUPNAME		245
#define SZ_COL_TOTRATE			64

// Color Index Define
#define CLR_FORM_BACK			64
#define CLR_GUIDE_BACK			66
#define CLR_GUIDE_FORE			69
#define CLR_GUIDE_LINE			75
#define	CLR_STATIC_FORE			69
#define CLR_GRID_TEXT			69
#define CLR_GRID_BACK			68
#define CLR_GRID_LINE			75
#define CLR_GRID_HBACK			74
#define CLR_GRID_HTEXT			76
#define CLR_GRID_SBACK			78
#define CLR_GRID_STEXT			69
#define CLR_PANEL_BORD			165

// TR Key
#define KEY_GROUPLIST			100
#define KEY_GROUPAMD			101
#define KEY_GROUPACCOUNT		102


/*******************************************************************************
 * 선물옵션 펀드등록
 ******************************************************************************/

struct fofamd_rec
{
	char zAcntNo[20];			// 계좌번호
	char zFnoIsuNo[32];			// 선물옵션종목번호
};

struct fofamd_mid
{
	char In[5];					// 레코드갯수
	char zTrxPtnTp[1];			// 처리유형구분
	char zUserTp[1];			// 사용자구분
	char zFundOrdUserId[16];	// 펀드주문사용자ID
	char zEmpPwd[8];			// 사원비밀번호
	char zFundId[12];			// 펀드ID
	char zFundPtnTp[1];			// 펀드유형구분
	char zAcntNo[20];			// 계좌번호
	char zPwd[8];				// 비밀번호
	char zFnoIsuNo[32];			// 선물옵션종목번호
	char lOrdAbleQty[16];		// 주문가능수량
	char dRatVal[19];			// 비율값
	char dWghtval[19];			// 가중치
	char zPrcPtnTp[1];			// 가격유형구분
	char zOrdMthdTp[1];			// 주문방식구분
	char lOrdQty[16];			// 주문수량
	char dOrdPrc[13];			// 주문가
	char zBnsTp[1];				// 매매구분
	char zFundDescr40[40];		// 펀드설명40
	char In2[5];				// 레코드갯수
	struct fofamd_rec rec[30];	//
};

struct fofamd_mod
{
	char Out[5];				// 레코드갯수
	char zRegDt[8];				// 등록일
	char zRegTime[9];			// 등록시각
};


/*******************************************************************************
 * 선물옵션 펀드조회
 ******************************************************************************/


struct fofqry_mid
{
	char In[5];					// 레코드갯수
	char zUserTp[1];			// 사용자구분
	char zFundOrdUserId[16];	// 펀드주문사용자ID
	char zEmpPwd[8];			// 사원비밀번호
};

struct fofqry_rec
{
	char zFundId[12];			// 펀드ID
	char zFundPtnTp[1];			// 펀드유형구분
	char zFundDescr40[40];		// 펀드설명40
};

struct fofqry_mod
{
	char Out[5];					//	레코드갯수
	struct fofqry_rec rec[999];
};

#define L_fofqry_mid	sizeof(fofqry_mid)
#define L_fofqry_rec	sizeof(fofqry_rec)
#define L_fofqry_mod	sizeof(fofqry_mod)


/*******************************************************************************
 * 선물옵션 펀드별종목조회
 ******************************************************************************/

struct fofaqry_mid
{
	char In[5];					// 레코드갯수
	char zUserTp[1];			// 사용자구분
	char zFundOrdUserId[16];	// 펀드주문사용자ID
	char zEmpPwd[8];			// 사원비밀번호
	char zFundId[12];			// 펀드ID
	char zFundPtnTp[1];			// 펀드유형구분
};

struct fofaqry_rec
{
	char zAcntNo[20];			// 계좌번호
	char zFnoIsuNo[32];			// 선물옵션종목번호
	char lLastFundOrdNo[10];	// 최종펀드주문번호
	char zLastOrdDt[8];		// 최종주문일
	char zRegDt[8];				// 등록일
	char zRegTime[9];			// 등록시각
	char lOrdAbleQty[16];		// 주문가능수량
	char dRatVal[19];			// 비율값
	char dWghtval[19];			// 가중치
	char zPrcPtnTp[1];			// 가격유형구분
	char zOrdMthdTp[1];			// 주문방식구분
	char lOrdQty[16];			// 주문수량
	char dOrdPrc[13];			// 주문가
	char zBnsTp[1];				// 매매구분
	char zIsuNm[40];			// 종목명
	char zAcntNm[40];			// 계좌명
	char zPrcPtnNm[40];			// 가격유형명
	char zOrdMthdCodeNm[40];	// 주문방법코드명
	char zBnsTpNm[10];			// 매매구분
	char zFundDescr40[40];		// 펀드설명40
	char lFundRegSeq[10];		// 펀드등록순서
};

struct fofaqry_mod
{
	char Out[5];				// 레코드갯수
	struct fofaqry_rec rec[999];
};

#define L_fofaqry_mid	sizeof(fofaqry_mid)
#define L_fofaqry_rec	sizeof(fofaqry_rec)
#define L_fofaqry_mod	sizeof(fofaqry_mod)


























/*******************************************************************************
 * 펀드등록/수정/삭제 IO FORMAT	(그룹 등록/수정/삭제)
 ******************************************************************************/

struct gamd_mid
{
	char In[5];					// 레코드갯수   - F12 : 전송
	char zTrxTp[1];				// 처리구분     - 1 : 펀드조회, 2 : 펀드입력, 3 : 펀드정정, 4 : 펀드삭제, 6 : 펀드전체삭제
	char zUserTp[1];			// 사용자구분   - 1 : 직원, 2 : 고객
	char zUserId[16];			// 사용자ID
	char zEmpPwd[8];			// 사원비밀번호
	char zFundNm[40];			// 펀드명
	char zFundPtnCode[2];		// 펀드유형코드 - 1 : 계좌:종목(1:N), 2 : 계좌:종목(N:1), 3 : 계좌:종목(N:N)
	char zAcntNo[20];			// 계좌번호
	char zPwd[8];				// 비밀번호
	char zIsuNo[12];			// 종목번호
	char lOrdAbleQty[16];		// 주문가능수량
	char dRatVal[19];			// 비율값
	char dWghtval[19];			// 가중치
	char zPrcPtnTp[1];			// 가격유형구분 - 1 : 1단호가, 2 : 2단호가 , 3 : 3단호가 , 4 : 4단호가 , 5 : 5단호가 , C : 현재가  , H : 상한가  , L : 하한가  , Z : 지정가
	char zOrdMthdTp[1];			// 주문방식구분 - 1 : 수량, 2 : 비율, 3 : 가중치
	char lOrdQty[16];			// 주문수량
	char dOrdPrc[13];			// 주문가
	char zBnsTp[1];				// 매매구분     - 1 : 매도, 2 : 매수
	char zFundDescr40[40];		// 펀드설명40
	char zMktTp[1];				// 시장구분
};

struct gamd_mod
{
	struct gamd_mid mid;		// 입력데이터
	char Out[5];				// 레코드갯수
	char zRegDt[8];				// 등록일
	char zRegTime[9];			// 등록시각
	char lOrdAbleQty[16];		// 주문가능수량
	char dRatVal[19];			// 비율값
	char dWghtval[19];			// 가중치
	char zPrcPtnTp[1];			// 가격유형구분
	char zOrdMthdTp[1];			// 주문방식구분
	char lOrdQty[16];			// 주문수량
	char dOrdPrc[13];			// 주문가
	char zBnsTp[1];				// 매매구분
	char zPrcPtnNm[40];			// 가격유형명
	char zFundPtnNm[40];		// 펀드유형명
	char zBnsTpNm[10];			// 매매구분
	char zFundDescr40[40];		// 펀드설명40
	char zMktTp[1];				// 시장구분
};

#define L_gamd_mid	sizeof(gamd_mid)
#define L_gamd_mod	sizeof(gamd_mod)


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