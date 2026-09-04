#pragma once

#include "TR_IN.h"
#include "PTS_API_OUT.h"

struct SONBQ504_mid
{
	char In[5];                        /* 레코드갯수             */
	char zAcntNo[20];                  /* 계좌번호               */
	char zInptPwd[8];                  /* 입력비밀번호           */
	char zOrdDt[8];                    /* 주문일                 */
	char zBalEvalTp[1];                /* 잔고평가구분           */
	char zFutsPrcEvalTp[1];            /* 선물가격평가구분       */
	char zLqdtQtyQryTp[1];             /* 청산수량조회구분       */
};
#define L_SONBQ504_mid		sizeof(struct SONBQ504_mid)

struct SONBQ504_grid
{
	char zFnoIsuNo[32];                /* 선물옵션종목번호       */
	char zIsuNm[40];                   /* 종목명                 */
	char zBnsTp[1];                    /* 매매구분               */
	char zBnsTpNm[10];                 /* 매매구분               */
	char lUnsttQty[16];                /* 미결제수량             */
	char dFnoAvrPrc[19];               /* 평균가                 */
	char dNowPrc[13];                  /* 현재가                 */
	char dCmpPrc[13];                  /* 대비가                 */
	char lEvalPnl[16];                 /* 평가손익               */
	char dPnlRat[12];                  /* 손익률                 */
	char lEvalAmt[16];                 /* 평가금액               */
	char dEvalRat[7];                  /* 평가비율               */
	char lLqdtAbleQty[16];             /* 청산가능수량           */
};
#define L_SONBQ504_grid		sizeof(struct SONBQ504_grid)

struct SONBQ504_mod
{
	char Out[5];                       /* 레코드갯수             */
	char zAcntNm[40];                  /* 계좌명                 */
	char lEvalDpsamtTotamt[15];        /* 평가예탁금총액         */
	char lMnyEvalDpstgAmt[15];         /* 현금평가예탁금액       */
	char lDpsamtTotamt[16];            /* 예탁금총액             */
	char lDpstgMny[16];                /* 예탁현금               */
	char lDpstgSubst[16];              /* 예탁대용               */
	char lFcurrSubstAmt[16];           /* 외화대용금액           */
	char lPsnOutAbleTotAmt[15];        /* 인출가능총금액         */
	char lPsnOutAbleCurAmt[16];        /* 인출가능현금액         */
	char lPsnOutAbleSubstAmt[16];      /* 인출가능대용금액       */
	char lOrdAbleTotAmt[15];           /* 주문가능총금액         */
	char lMnyOrdAbleAmt[16];           /* 현금주문가능금액       */
	char lCsgnMgnTotamt[16];           /* 위탁증거금총액         */
	char lMnyCsgnMgn[16];              /* 현금위탁증거금액       */
	char lMtmgnTotamt[15];             /* 유지증거금총액         */
	char lMnyMaintMgn[16];             /* 현금유지증거금액       */
	char lAddMgnTotamt[15];            /* 추가증거금총액         */
	char lMnyAddMgn[16];               /* 현금추가증거금액       */
	char lCmsnAmt[16];                 /* 수수료                 */
	char lRcvblAmt[16];                /* 미수금액               */
	char lRcvblOdpnt[16];              /* 미수연체료             */
	char lFutsEvalPnlAmt[16];          /* 선물평가손익금액       */
	char lOptEvalPnlAmt[16];           /* 옵션평가손익금액       */
	char lOptEvalAmt[16];              /* 옵션평가금액           */
	char lOptBnsplAmt[16];             /* 옵션매매손익금액       */
	char lFutsAdjstDfamt[16];          /* 선물정산차금           */
	char lTotPnlAmt[16];               /* 총손익금액             */
	char lNetPnlAmt[16];               /* 순손익금액             */
	char Out2[5];                      /* 레코드갯수             */
	struct SONBQ504_grid grid[1];
};
#define L_SONBQ504_mod		sizeof(struct SONBQ504_mod)

//////////////////////////////////////////////////////////////////////////
//은행이체 관련
/******************************************************************************
* NAME : TR4501
* DESC : 은행이체출금(증권->은행)
 ******************************************************************************/
struct TR4501_mid
{
	char In[5];					/* 레코드 갯수      */
	char zCardFirmNo[3];		/* 카드회사번호*/
	char zCardAcntNo[20];		/* 카드계좌번호*/
	char zCardPtnCode[2];		/* 카드유형코드*/
	char zCardIssCnt[10];		/* 카드발급건수 Default +000000000 */
	char zMgrCardFirmNo[3];		/* 책임자카드회사번호*/
	char zMgrCardAcntNo[20];	/* 책임자카드계좌번호*/
	char zMgrCardPtnCode[2];	/* 책임자카드유형코드*/
	char zMgrCardIssCnt[10];	/* 책임자카드발급횟수 Default +000000000 */
	char zAgrmReqstDt[8];		/* 승인요청일*/
	char zAgrmTrxId[8];			/* 승인처리ID*/
	char zLastAgrmUserId[16];	/* 최종승인자ID*/
	char zAgrmUserNm[40];		/* 승인사용자명*/
	char zAgrmAddMsg[100];		/* 승인자추가메시지*/
	char zRlyBnkCode[3];		/* 중계은행코드*/
	char zMnyoutAcntNo[20];		/* 출금계좌번호*/
	char zPwd[8];				/* 비밀번호*/
	char zMnyoutAmt[16];		/* 출금금액*/
	char zMnyinBnkCode[3];		/* 입금은행코드*/
	char zBnkAcntNo[20];		/* 은행계좌번호*/
	char zRcvrNm[40];			/* 수취인명*/
	char zBnkTrsfPwd[8];		/* 은행이체비밀번호*/
	char zMnyoutPtnTp[1];		/* 출금유형	Default 1 */
	char zIvstfndCode[12];		/* 투신펀드코드*/
	char zAskpsnNm[40];			/* 의뢰인명*/
	char zTelExno[4];			/* 전화국번호*/
	char zTelRgno[4];			/* 전화지역번호*/
	char zTelSeqno[4];			/* 전화일련번호*/
};
#define L_TR4501_mid		sizeof(struct TR4501_mid)

struct TR4501_mod
{
	struct TR4501_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명		*/
	char zMtrsfTrdNo[10];		/* 이체거래번호*/
	char zRlyBnkNm[40];			/* 중계은행명*/
	char zMnyinBnkNm[40];		/* 입금은행명*/
	char zRcvrNm[40];			/* 수취인명*/
	char zOrdNo[10];			/* 주문번호*/
	char zBncertSellUnit[16];	/* 수익증권매도좌수*/
	char zInouno[10];			/* 출납번호*/
	char zMnyoutAmt[16];		/* 출금금액*/
	char zOutTrsfCmsnAmt[16];	/* 이체출금수수료*/
	char zMnyoutTotAmt[16];		/* 출금총금액*/
	char zDpsBfbalAmt[16];		/* 예수금전잔금액*/
	char zDpsCrbalAmt[16];		/* 예수금금잔금액*/
	char zFundNm[40];			/* 펀드명*/
	char zBnkRspnsCode[4];		/* 은행응답코드*/
	char zBnkMsgCnts[100];		/* 은행메시지내용*/
	char zAgrmTrxStep[10];		/* 승인처리단계*/
	char zAgrmMthdCode[2];		/* 승인방식코드*/
};
#define L_TR4501_mod		sizeof(struct TR4501_mod)


/******************************************************************************
* NAME : TR4502
* DESC : 계좌간자금대체(증권 -> 증권)
 ******************************************************************************/
struct TR4502_mid
{
	char In[5];					/* 레코드 갯수      */
	char zCardFirmNo[3];		/* 카드회사번호 */
	char zCardAcntNo[20];		/* 카드계좌번호 */
	char zCardPtnCode[2];		/* 카드유형코드 */
	char lCardIssCnt[10];		/* 카드발급건수 Default +000000000 */
	char zMgrCardFirmNo[3];		/* 책임자카드회사번호 */
	char zMgrCardAcntNo[20];	/* 책임자카드계좌번호 */
	char zMgrCardPtnCode[2];	/* 책임자카드유형코드 */
	char lMgrCardIssCnt[10];	/* 책임자카드발급횟수 Default +000000000 */
	char zAcntNo[20];			/* 계좌번호 */
	char zPwd[8];				/* 비밀번호 */
	char lBkeepAmt[16];			/* 대체금액 */
	char zBncertSellIsuNo[6];	/* 매도종목번호 */
	char zBncertSellIsuSeqno[6];/* 매도종목일련번호 */
	char zMnyoutPtnTp[1];		/* 출금유형 Default 4 */
	char zMnyinAcntNo[20];		/* 입금계좌번호 */
	char zBncertBuyIsuNo[6];	/* 매수종목번호 */
	char zBncertBuyIsuSeqno[6];	/* 매수종목일련번호 */
	char zAgrmBranNo[3];		/* 승인지점번호 */
	char zAgrmTermNo[3];		/* 승인단말번호 */
	char zLastAgrmUserId[16];	/* 최종승인자ID */
	char zAgrmReqstYn[1];		/* 승인요청여부 */
	char zEmpPwd[8];			/* 사원비밀번호 */
	char zInptPwd[8];			/* 입력비밀번호 */
	char zSmryNo[4];			/* 적요번호 */
};
#define L_TR4502_mid		sizeof(struct TR4502_mid)

struct TR4502_mod
{
	struct TR4502_mid mid;
	char Out[5];						/* 레코드갯수 */
	char lMnyoutInouno[8];				/* 출금출납번호 */
	char lBkeepAmt[16];					/* 대체금액 */
	char lMnyoutDpsBfbalAmt[16];		/* 출금예수금전잔 */
	char lMnyoutDpsCrbalAmt[16];		/* 출금예수금금잔 */
	char lMnyoutNSavBfbalAmt[16];		/* 출금비저축전잔 */
	char lMnyoutNSavCrbalAmt[16];		/* 출금비저축금잔 */
	char lMnyinInouno[8];				/* 입금출납번호 */
	char lMnyinDpsBfbalAmt[16];			/* 입금예수금전잔 */
	char lMnyinDpsCrbalAmt[16];			/* 입금예수금금잔 */
	char lMnyinNSavBfbalAmt[16];		/* 입금비저축전잔 */
	char lMnyinNSavCrbalAmt[16];		/* 입금비저축금잔 */
	char lMnyRcvblRepayAmt[16];			/* 현금미수변제금액 */
	char lMnyOdpntRepayAmt[16];			/* 현금미수연체료변제금액 */
	char lIntdltRepayAmt[16];			/* 이자미납변제금액 */
	char lIntdltOdpntRepayAmt[16];		/* 이자미납연체료변제금액 */
	char lEtclndRepayAmt[16];			/* 기타대여변제금액 */
	char lEtclndDlinqOdpntRepayAmt[16];	/* 기타대여연체료변제금액 */
	char lDpsCrdaySubRepayAmt[16];		/* 예수금일부변제금액 */
	char lRealInAmt[16];				/* 실입금액 */
	char lRealOutAmt[16];				/* 실출금액 */
	char zEmpPwd[8];					/* 사원비밀번호 */
	char zMnyoutAcntNm[40];				/* 출금계좌명 */
	char zSellIsuNm[40];				/* 매도종목명 */
	char zMnyinAcntNm[40];				/* 입금계좌명 */
	char zBuyIsuNm[40];					/* 매수종목명 */
	char zSmryNo[4];					/* 적요번호 */
	char zSmryNm[40];					/* 적요명 */
	char lBkeepInouno[10];				/* 대체출납번호 */
	char lBrnInouno[10];				/* 지점출납번호 */
	char lReqstInouno[10];				/* 요청출납번호 */
	char Out2[5];						/* 레코드갯수 */
	char lOrdNo[10];					/* 주문번호 */
	char lBuyQty[16];					/* 매수수량 */
	char dFundBfbalQty[20];				/* 펀드전잔수량 */
	char dBasePrc[13];					/* 기준가 */
	char zSettDt[8];					/* 결제일 */
	char lBuyAmt[16];					/* 매수금액 */
	char dFundCrbalQty[20];				/* 펀드금잔수량 */
	char dBoaBasePrc[20];				/* 과표기준가 */
	char lMchndCmsnAmt[16];				/* 판매수수료 */
	char zHanglIvstfndNm[40];			/* 한글투신펀드명 */
	char lIvstfndSeqno[10];				/* 투신펀드일련번호 */
	char zBuyPtnCode[2];				/* 매수유형코드 */
	char zBuyPtnNm[40];					/* 매수유형 */
};
#define L_TR4502_mod		sizeof(struct TR4502_mod)


/******************************************************************************
* NAME : TR4503
* DESC : 은행이체출금(은행->증권)
 ******************************************************************************/
struct TR4503_mid
{
	char In[5];					/* 레코드 갯수      */
	char zMnyinAcntNo[20];		/* 입금계좌번호 */
	char zBnkCode[3];			/* 은행코드 */
	char zBnkAcntNo[20];		/* 은행계좌번호 */
	char zPwd[8];				/* 비밀번호 */
	char zMnyinAmt[16];			/* 입금금액 */
};
#define L_TR4503_mid		sizeof(struct TR4503_mid)

struct TR4503_mod
{
	struct TR4503_mid mid;
	char Out[5];					/* 레코드갯수 */
	char zSecAcntNm[40];			/* 증권계좌명 */
	char zBnkAcntNm[40];			/* 은행계좌명 */
	char zMtrsfTrdNo[10];			/* 이체거래번호 */
	char zMnyInouno[10];			/* 현금출납번호 */
	char zDpsBfbalAmt[16];			/* 예수금전잔금액 */
	char zDpsCrbalAmt[16];			/* 예수금금잔금액 */
	char zMnyrclRepayAmt[16];		/* 현금미수변제금액 */
	char zMnyrclOdpntRepayAmt[16];	/* 현금미수연체료변제금액 */
	char zIntdltRepayAmt[16];		/* 이자미납변제금액 */
	char zIntdltOdpntRepayAmt[16];	/* 이자미납연체료변제금액 */
	char zEtclndRepayAmt[16];		/* 기타대여변제금액 */
	char zEtclndOdpntRepayAmt[16];	/* 기타대여금연체료변제금액 */
	char zDpsCrdaySubRepayAmt[16];	/* 예수금일부변제금액 */
	char zBnkDps[15];				/* 은행예수금 */
	char zBnkNm[40];				/* 은행명 */
	char zBnkRspnsCode[4];			/* 은행응답코드 */
	char zBnkMsgCnts[100];			/* 은행메시지내용 */
};
#define L_TR4503_mod		sizeof(struct TR4503_mod)


/******************************************************************************
* NAME : TR4511
* DESC : 계좌별은행이체거래내역
 ******************************************************************************/
struct TR4511_mid
{
	char In[5];				/* 레코드 갯수      */
	char zAcntNo[20];		/* 계좌번호*/
	char zPwd[8];			/* 비밀번호*/
	char zQrySrtDt[8];		/* 조회시작일*/
	char zQryEndDt[8];		/* 조회종료일*/
};
#define L_TR4511_mid		sizeof(struct TR4511_mid)

struct TR4511_grid
{
	char zTrdDt[8];			/* 거래일 */
	char zTrxTime[9];		/* 처리시각 */
	char zMtrsfTrdNo[10];	/* 이체거래번호 */
	char zTrdTpNm[20];		/* 거래구분 */
	char zTrxBrnNm[40];		/* 처리지점명 */
	char zCancYnNm[10];		/* 취소여부명 */
	char zTrdBnkNm[40];		/* 거래은행명 */
	char zRlyBnkNm[40];		/* 중계은행명 */
	char zBnkAcntNo[20];	/* 은행계좌번호 */
	char zBnkAcntNm[40];	/* 은행계좌명 */
	char zTrdAmt[16];		/* 거래금액 */
	char zCmsnAmt[16];		/* 수수료 */
	char zMnyAmt[16];		/* 현금금액 */
	char zChckAmt[16];		/* 수표금액 */
	char zColorTp[1];		/* 색구분 */
	char zTrxStatNm[20];	/* 처리상태명 */
	char zUnTrxRsn[20];		/* 미처리사유 */
	char zCommdaCodeNm[40];	/* 통신매체코드명 */
	char zIntaccNo[20];		/* 종합계좌번호 */
	char zTelRgno[4];		/* 전화지역번호 */
	char zTelExno[4];		/* 전화국번호 */
	char zTelSeqno[4];		/* 전화일련번호 */
	char zInouno[10];		/* 출납번호 */
	char zOrgInouno[10];	/* 원출납번호 */
	char zUablCode[4];		/* 불능코드 */
	char zOcrPtnTp[1];		/* 발생유형구분 */
	char zTrxtrNm[40];		/* 처리자명 */
	char zChckAbndSettTpNm[8];	/* 처리구분 */
};
#define L_TR4511_grid		sizeof(struct TR4511_grid)

struct TR4511_mod
{
	struct TR4511_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명		*/
	char Out2[5];				/* 레코드갯수 */
	struct TR4511_grid grid[1];
};
#define L_TR4511_mod		sizeof(struct TR4511_mod)


/******************************************************************************
* NAME : TR4512
* DESC : 계좌대체거래내역
 ******************************************************************************/
struct TR4512_mid
{
	char In[5];				/* 레코드 갯수      */
	char zBrnNo[3];			/* 지점번호 Default 000 */
	char zQrySrtDt[8];		/* 조회시작일 */
	char zQryEndDt[8];		/* 조회종료일 */
	char zSrtTrdNo[6];		/* 시작거래번호 Default +00000 */
	char zBkeepTp[1];		/* 대체구분 Default 1 */
	char zAcntNo[20];		/* 계좌번호 */
	char zPwd[8];			/* 비밀번호 */
	char zQryCnt[10];		/* 조회건수 Default +000000020 */
};
#define L_TR4512_mid		sizeof(struct TR4512_mid)

struct TR4512_grid
{
	char zBkeepNo[10];			/* 대체번호 */
	char zInouno[10];			/* 출납번호 */
	char zBkeepTpNm[20];		/* 대체구분 */
	char zCancTpNm[20];			/* 취소구분 */
	char zMnyoutAcntNo[20];		/* 출금계좌번호 */
	char zMnyoutAcntNm[40];		/* 출금계좌명 */
	char zSecoutIsuNo1[12];		/* 출고종목번호1 */
	char zSeqno1[10];			/* 일련번호 */
	char zSecoutIsuNm1[40];		/* 출고종목명1 */
	char zMnyinAcntNo[20];		/* 입금계좌번호 */
	char zMnyinAcntNm[40];		/* 입금계좌명 */
	char zSecinIsuNo1[12];		/* 입고종목번호1 */
	char zSeqno2[10];			/* 일련번호 */
	char zSecinIsuNm1[40];		/* 입고종목명1 */
	char zTrdAmt[16];			/* 거래금액 */
	char zMnyoutSeqno[10];		/* 출금일련번호 */
	char zMnyinSeqno[10];		/* 입금일련번호 */
	char zIsuNo[12];			/* 종목번호 */
	char zIsuNm[40];			/* 종목명 */
	char zTrxTermNo[3];			/* 처리단말번호 */
	char zTrxTime[9];			/* 처리시각 */
	char zBuyDate[8];			/* 매수일자 */
	char zSecPtnCode[4];		/* 유가증권유형코드 */
	char zTrdDate[8];			/* 거래일자 */
};
#define L_TR4512_grid		sizeof(struct TR4512_grid)

struct TR4512_mod
{
	struct TR4512_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zBrnNo[3];				/* 지점번호 */
	char zBrnNm[40];			/* 지점명 */
	char zQrySrtDt[8];			/* 조회시작일 */
	char zQryEndDt[8];			/* 조회종료일 */
	char zSrtTrdNo[6];			/* 시작거래번호 */
	char zBkeepTp[1];			/* 대체구분 */
	char Out2[5];				/* 레코드갯수 */
	struct TR4512_grid grid[1];
};
#define L_TR4512_mod		sizeof(struct TR4512_mod)


/******************************************************************************
* NAME : TR4521
* DESC : 은행리스트조회
 ******************************************************************************/
struct TR4521_grid
{
	char zBnkNo[3];				/* 은행번호 */
	char zBnkNm[40];			/* 은행명 */
	char zFltYn[1];				/* 장애여부 */
	char zColortp[1];			/* 색상구분 */
};
#define L_TR4521_grid		sizeof(struct TR4521_grid)

struct TR4521_mod
{
	char Out[5];				/* 레코드갯수 */
	struct TR4521_grid grid[1];
};
#define L_TR4521_mod		sizeof(struct TR4521_mod)


/******************************************************************************
* NAME : TR4522
* DESC : 은행계좌번호조회
 ******************************************************************************/
struct TR4522_mid
{
	char In[5];				/* 레코드 갯수      */
	char zBankCode[3];		/* 은행코드			*/
	char zAcntNo[20];		/* 계좌번호         */
	char zInptPwd[8];		/* 입력비밀번호     */
};
#define L_TR4522_mid		sizeof(struct TR4522_mid)

struct TR4522_grid
{
	char zBankCode[3];			/* 은행코드 */
	char zBnkNm[40];			/* 은행명 */
	char zBnkAcntNo[20];		/* 은행계좌번호 */
};
#define L_TR4522_grid		sizeof(struct TR4522_grid)

struct TR4522_mod
{
	struct TR4522_mid mid;
	char Out[5];				/* 레코드갯수 */
	struct TR4522_grid grid[1];
};
#define L_TR4522_mod		sizeof(struct TR4522_mod)


/******************************************************************************
* NAME : TR4523
* DESC : HTS로그인계좌목록
 ******************************************************************************/
struct TR4523_mid
{
	char In[5];					/* 레코드 갯수      */
	char zElecfnUserId[16];		/* 전자금융사용자ID*/
};
#define L_TR4523_mid		sizeof(struct TR4523_mid)

struct TR4523_grid
{
	char zPdptnCode[2];		/* 상품유형코드 */
	char zPdptnNm[40];		/* 상품유형명 */
	char zAcntClssTp[1];	/* 계좌계층구분 */
	char zAcntTpNm[20];		/* 계좌구분 */
	char zAcntStatCode[2];	/* 계좌상태코드 */
	char zAcntStatNm[40];	/* 계좌상태명 */
	char zBrnNo[3];			/* 지점번호 */
	char zBrnNm[40];		/* 지점명 */
	char zAcntNo[20];		/* 계좌번호 */
	char zAcntNm[40];		/* 계좌명 */
	char zSmpsnYn[1];		/* 동일인여부 */
	char zSmpsnYnNm[40];	/* 동일인여부 */
	char zDueDt[8];			/* 만기일 */
};
#define L_TR4523_grid		sizeof(struct TR4523_grid)

struct TR4523_mod
{
	struct TR4523_mid mid;
	char Out[5];			/* 레코드갯수 */
	struct TR4523_grid grid[1];
};
#define L_TR4523_mod		sizeof(struct TR4523_mod)


/******************************************************************************
* NAME : TR4524
* DESC : 인증기본정보조회
 ******************************************************************************/
struct TR4524_mid
{
	char In[5];				/* 레코드 갯수      */
	char zTrxTp[1];			/* 처리구분 Default I */
	char zUserId[16];		/* 사용자 ID */
	char zRmnno[13];		/* 실명확인번호 */
};
#define L_TR4524_mid		sizeof(struct TR4524_mid)

struct TR4524_mod
{
	struct TR4524_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zUserId[16];			/* 사용자 ID */
	char zRmnno[13];			/* 실명확인번호 */
	char zClntTp[1];			/* 고객구분 */
	char zClntNm[40];			/* 고객명 */
	char zlDigisigErrCnt[2];	/* 전자서명오류횟수 */
	char zCertStatTp[1];		/* 인증서상태구분 */
	char zCertDnm[256];			/* 인증서식별명 */
	char zOnesfCnfTp[1];		/* 본인확인구분 */
};
#define L_TR4524_mod		sizeof(struct TR4524_mod)


/******************************************************************************
* NAME : TR4525
* DESC : 출금가능금액및실명번호조회
 ******************************************************************************/
struct TR4525_mid
{
	char In[5];				/* 레코드 갯수      */
	char zMnyoutAcntNo[20];	/* 출금계좌번호 */
	char zMnyoutPtnTp[1];	/* 출금유형 Default 1 */
	char zIvstfndCode[12];	/* 투신펀드코드 */
};
#define L_TR4525_mid		sizeof(struct TR4525_mid)

struct TR4525_mod
{
	struct TR4525_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zRmnno[13];			/* 실명확인번호 */
	char MnyoutAbleAmt[16];		/* 출금가능금액 */
	char zD2Dps[16];			/* D2 예수금 */
	char zAcntOnceLmtAmt[16];	/* 계좌일회한도금액 */
	char zAcntDailyLmtAmt[16];	/* 계좌일일한도금액 */
	char zLastMtrsfDt[8];		/* 최종이체일 */
	char zThdayMtrsfTotamt[16];	/* 당일이체총액 */
	char zCoOnceLmtAmt[16];		/* 회사일회한도금액 */
	char zCoDailyLmtAmt[16];	/* 회사일일한도금액 */
};
#define L_TR4525_mod		sizeof(struct TR4525_mod)


/******************************************************************************
* NAME : TR4526
* DESC : 계좌 서비스현황
 ******************************************************************************/
struct TR4526_mid
{
	char In[5];				/* 레코드 갯수      */
	char zAcntTp[1];		/* 계좌구분 Default 2 */
	char zAcntNo[20];		/* 계좌번호 */
	char zInptPwd[8];		/* 입력비밀번호 */
	char zSvcKindCode[4];	/* 서비스종류코드 Default 1101 */
};
#define L_TR4526_mid		sizeof(struct TR4526_mid)

struct TR4526_grid
{
	char zSvcKindCode[4];		/* 서비스종류코드 */
	char zSvcKindNm[40];		/* 서비스종류명 */
	char zOppIttCode[3];		/* 상대기관코드 */
	char zTrdIttNm[40];			/* 거래기관명 */
	char zOppIttAcntNo[20];		/* 상대기관계좌번호 */
	char zOppTrdAcntNm[40];		/* 상대거래계좌명 */
	char zCtrctRegDt[8];		/* 약정등록일 */
	char zCtrctXpirDt[8];		/* 약정만료일 */
	char zIntaccNo[20];			/* 종합계좌번호 */
	char zBnkDlgConnAcntNo[20];	/* 은행대표연결계좌번호 */
	char zBbkpgPtnNm[40];		/* 일괄대체유형 */
	char zAbkpDt[8];			/* 자동대체일 */
	char zAmtrBkeepAmt[16];		/* 자동이체대체금액 */
	char zUndDsgnNm[40];		/* 인수도지정 */
	char zBuyFundNo[12];		/* 매수펀드번호 */
	char zSellFundNo[12];		/* 매도펀드번호 */
	char zOtcoNm[40];			/* 타사명 */
	char zBkeepTpNm[20];		/* 대체구분 */
	char zOppAcntRrno[13];		/* 상대계좌주민등록번호 */
	char zFundNm[40];			/* 펀드명 */
	char zOppFundNm[40];		/* 상대펀드명 */
	char zCtrctSrtDt[8];		/* 약정시작일 */
	char zCtrctAbndDt[8];		/* 약정해지일 */
};
#define L_TR4526_grid		sizeof(struct TR4526_grid)

struct TR4526_mod
{
	struct TR4526_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명		*/
	char Out2[5];				/* 레코드갯수 */
	struct TR4526_grid grid[1];
};
#define L_TR4526_mod		sizeof(struct TR4526_mod)


/******************************************************************************
* NAME : TR4527
* DESC : 계좌명조회
 ******************************************************************************/
struct TR4527_mid
{
	char In[5];				/* 레코드 갯수      */
	char zAcntNo[20];		/* 계좌번호 */
};
#define L_TR4527_mid		sizeof(struct TR4527_mid)

struct TR4527_mod
{
	struct TR4527_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명		*/
};
#define L_TR4527_mod		sizeof(struct TR4527_mod)

/******************************************************************************
* NAME : TR8011
* DESC : OPT요청 SBPGT315
 ******************************************************************************/
struct TR8011_mid {
	char In[5];					/* 레코드 갯수      */
	char zOnesfCnfTrxSeqno[5];	/* 본인확인처리일련번호*/
	char zMdaSeqno[16];			/* 매체일련번호*/
	char zOtpVrfVal[12];		/* OTP검증값*/
};
#define L_TR8011_mid		sizeof(struct TR8011_mid)

struct TR8011_mod {
	struct TR8011_mid mid;
	char Out[5];			/* 레코드갯수 */
	char zRetryAbleCnt[2];	/* 재시도가능횟수*/
	char zErrMsgCode[4];	/* 오류메시지코드*/
	char zErrMsgCnts[100];	/* 오류메시지내용*/
};
#define L_TR8011_mod		sizeof(struct TR8011_mod)

/******************************************************************************
* NAME : TR8012
* DESC : OPT요청수신 SBPGT314
 ******************************************************************************/
struct TR8012_mid {
	char In[5];					/* 레코드 갯수      */
	char zUserId[16];			/* 사용자ID*/
	char zReqstSvcId[8];		/* 요청서비스ID*/
};
#define L_TR8012_mid		sizeof(struct TR8012_mid)

struct TR8012_mod {
	struct TR8012_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zOnesfCnfTrxSeqno[5];	/* 본인확인처리일*/
	char zOnesfCnfPtnTp[1];		/* 본인확인유형구분*/
	char zRetryAbleCnt[2];		/* 재시도가능횟수*/
	char zMdaSeqnoReqstYn[1];	/* 매체일련번호 요청여부*/
};
#define L_TR8012_mod		sizeof(struct TR8012_mod)

/******************************************************************************
* NAME : TR8013
* DESC : 보안카드요청 SBPGT310
 ******************************************************************************/
struct TR8013_mid {
	char In[5];						/* 레코드 갯수      */
	char zOnesfCnfTrxSeqno[5];		/* 본인확인처리일련번호 */
	char zSecurCardSeqnoInpt[3];	/* 보안카드일련번호입력 */
	char zSecurCardInpt1[2];		/* 보안카드입력1 */
	char zSecurCardInpt2[2];		/* 보안카드입력2 */
};
#define L_TR8013_mid		sizeof(struct TR8013_mid)

struct TR8013_mod {
	struct TR8013_mid mid;
	char Out[5];			/* 레코드갯수 */
	char zRetryAbleCnt[2];	/* 재시도가능횟수 */
	char zErrMsgCode[4];	/* 오류메시지코드 */
	char zErrMsgCnts[100];	/* 오류메시지내용 */
};
#define L_TR8013_mod		sizeof(struct TR8013_mod)

/******************************************************************************
* NAME : TR8014
* DESC : 보안카드요청수신
 ******************************************************************************/
struct TR8014_mid {
	char In[5];					/* 레코드 갯수      */
	char zUserId[16];			/* 사용자ID */
	char zReqstSvcId[8];		/* 요청서비스ID */
	char zSecurCardAppTp[1];	/* 보안카드적용구분 */
};
#define L_TR8014_mid		sizeof(struct TR8014_mid)

struct TR8014_mod {
	struct TR8014_mid mid;
	char Out[5];					/* 레코드갯수 */
	char zOnesfCnfTrxSeqno[5];		/* 본인확인처리일련번호 */
	char zOnesfCnfPtnTp[1];			/* 본인확인유형구분 */
	char zRetryAbleCnt[2];			/* 재시도가능횟수 */
	char zSecurCardSeqnoLen[2];		/* 보안카드일련번호길이 */
	char zSecurCardSeqnoInptPos[3];	/* 보안카드일련번호입력위치 */
	char zSecurCardInptNo1[2];		/* 보안카드입력번호1 */
	char zSecurCardInptNo2[2];		/* 보안카드입력번호2 */
};
#define L_TR8014_mod		sizeof(struct TR8014_mod)

/******************************************************************************
* NAME : TR8015
* DESC : 보안비밀번호요청
 ******************************************************************************/
struct TR8015_mid {
	char In[5];				/* 레코드 갯수      */
	char zAcntNo[20];		/* 계좌번호 */
	char zInptPwd[8];		/* 비밀번호 */
};
#define L_TR8015_mid		sizeof(struct TR8015_mid)

struct TR8015_mod {
	struct TR8015_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명 */
	char zAvalYn[1];			/* 유효여부 */
	char zPdptnCode[2];			/* 상품유형코드 */
	char zPrdtDtlCode[2];		/* 상품상세코드 */
	char zErrCnt[10];			/* 오류건수 */
};
#define L_TR8015_mod		sizeof(struct TR8015_mod)
//////////////////////////////////////////////////////////////////////////