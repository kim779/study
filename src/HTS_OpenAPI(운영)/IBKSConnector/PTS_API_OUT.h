#include "Open_API_OUT.h"

#define TRANSFER_URL	(1)	//이체
#define CONTRACT_URL	(2)	//약정신청
#define CME_URL			(3)	//CME 야간선물 거래신청/해지
#define QNA_URL			(4)	//QnA

#pragma once

#pragma	pack(1)

//////////////////////////////////////////////////////////////////////////////
//PTS 추가
//////////////////////////////////////////////////////////////////////////////

/******************************************************************************
* NAME : TR4224
* DESC : 선물옵션 - 선물옵션 계좌익일예상증거금조회2
 ******************************************************************************/
struct TR4224_mid
{
	char In[5];							/* 레코드갯수             */
	char zAcntNo[20];					/* 계좌번호               */
	char zInptPwd[8];					/* 입력비밀번호           */
	char zKpi200Idx[13];				/* 코스피200지수          */
};
#define L_TR4224_mid	sizeof(struct TR4224_mid)

struct TR4224_grid
{
	char zFnoIsuNo[32];					/* 선물옵션종목번호			*/
	char zBnsTp[1];						/* 매매구분					*/
	char zBnsTpNm[10];					/* 매매구분					*/
	char zUnsttQty[16];					/* 미결제수량				*/
	char zCurPrc[13];					/* 현재가					*/
	char zIvlatRat[7];					/* 내재변동성비율			*/
	char zFcastThrprc[19];				/* 예상이론가				*/
	char zRemnDays[6];					/* 잔존일수					*/
};
#define L_TR4224_grid		sizeof(struct TR4224_grid)

struct TR4224_mod
{
	struct TR4224_mid mid;
	char Out[5];						/* 레코드갯수				*/
	char zAcntNm[40];					/* 계좌명					*/
	char zFnoAcntAfmgnNm[16];			/* 선물옵션계좌사후증거금명	*/
	char zDpsTotamt[16];				/* 예수금총액				*/
	char zDps[16];						/* 예수금					*/
	char zSubstAmt[16];					/* 대용금액					*/
	char zCsgnMgn[16];					/* 위탁증거금액				*/
	char zMnyCsgnMgn[16];				/* 현금위탁증거금액			*/
	char zAddMgn[16];					/* 추가증거금액				*/
	char zMnyAddMgn[16];				/* 현금추가증거금액			*/
	char zPsnOutAbleAmt[16];			/* 인출가능금액				*/
	char zPsnOutAbleCurAmt[16];			/* 인출가능현금액			*/
	char zOrdAbleTotAmt[16];			/* 주문가능총금액			*/
	char zOrdAbleAmt[16];				/* 주문가능금액				*/
	char zOut2[5];						/* 레코드갯수				*/
	char zDpsTotamt2[16];				/* 예수금총액				*/
	char zDps2[16];						/* 예수금					*/
	char zSubstAmt2[16];					/* 대용금액					*/
	char zCsgnMgn2[16];					/* 위탁증거금액				*/
	char zMnyCsgnMgn2[16];				/* 현금위탁증거금액			*/
	char zAddMgn2[16];					/* 추가증거금액				*/
	char zMnyAddMgn2[16];				/* 현금추가증거금액			*/
	char zPsnOutAbleAmt2[16];			/* 인출가능금액				*/
	char zPsnOutAbleCurAmt2[16];			/* 인출가능현금액			*/
	char zOrdAbleTotAmt2[16];			/* 주문가능총금액			*/
	char zOrdAbleAmt2[16];				/* 주문가능금액				*/
	char zOut3[5];						/* 레코드갯수				*/
	char zDpstgTotamtLackAmt[16];		/* 예탁총액부족금액(위탁증거금기준)				*/
	char zDpstgMnyLackAmt[16];			/* 예탁현금부족금액(위탁증거금기준)				*/
	char zDpstgTotamtLackAmt1[16];		/* 예탁총액부족금액(유지증거금기준)				*/
	char zDpstgMnyLackAmt1[16];			/* 예탁현금부족금액(유지증거금기준)				*/
	char zFutsThdayDfamt[16];			/* 선물당일차금				*/
	char zFutsUdamt[16];				/* 선물갱신차금				*/
	char zFutsLastSettDfamt[16];		/* 선물최종결제차금			*/
	char zAdjstAmt[16];					/* 정산금액					*/
	char zOptBuyAmt[16];				/* 옵션매수금액				*/
	char zOptSellAmt[16];				/* 옵션매도금액				*/
	char zOptRgtexeDfamt[16];			/* 옵션권리행사차금			*/
	char zOptRgtAsgnDfamt[16];			/* 옵션권리배정차금			*/
	char zFutsCmsnAmt[16];				/* 선물수수료				*/
	char zOptCmsnAmt[16];				/* 옵션수수료				*/
	char zFnoPrdaySubstSellAmt[16];		/* 선물옵션전일대용매도금액				*/
	char zFnoCrdaySubstSellAmt[16];		/* 선물옵션금일대용매도금액				*/
	char zOptEvalPnlAmt[16];			/* 옵션평가손익금액			*/
	char zOut4[16];						/* 레코드갯수				*/
	struct TR4224_grid grid[1];
};
#define L_TR4224_mod		sizeof(struct TR4224_mod)

/******************************************************************************
* NAME : TR4223
* DESC : 선물옵션 - 선물옵션 추가증거금발생현황
 ******************************************************************************/
struct TR4223_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zSrtDt[8];						/* 시작일				*/
	char zEndDt[8];						/* 종료일				*/
};
#define L_TR4223_mid	sizeof(struct TR4223_mid)

struct TR4223_grid
{
	char zOcrDt[8];						/* 발생일					*/
	char zEvalDpsamtTotamt[15];			/* 평가예탁금총액			*/
	char zOpnmkAddMgn[16];				/* 개장추가증거금액			*/
	char zOpnmkMnyAddMgn[16];			/* 개장현금추가증거금액		*/
	char zOpnmkFcurrAddMgn[16];			/* 개장외화추가증거금		*/
	char zOpnmkAddMgnTpNm[12];			/* 추가증거금상태			*/
	char zAddMgnTpNm[12];				/* 추가증거금발생			*/
	char zIsuNo[12];					/* 종목번호					*/
	char zBnsTp[1];						/* 매매구분					*/
	char zBnsTpNm[10];					/* 매매구분					*/
	char zCvrgQty[16];					/* 반대매매수량				*/
	char zOrdPrc[13];					/* 주문가					*/
	char zCvrgAmt[16];					/* 반대매매금액				*/
	char zLnkAcntNo[20];				/* 연계계좌번호				*/
	char zAddMgnTrxTp[1];				/* 추가증거금처리구분		*/
	char zTrxStatNm[20];				/* 처리상태명				*/
};
#define L_TR4223_grid	sizeof(struct TR4223_grid)

struct TR4223_mod
{
	struct TR4223_mid mid;
	char Out[5];						/* 레코드갯수				*/
	struct TR4223_grid grid[1];
};
#define L_TR4223_mod		sizeof(struct TR4223_mod)

/******************************************************************************
* NAME : TR4303
* DESC : 선물옵션 - 선물옵션 일별 계좌손익내역
 ******************************************************************************/
struct TR4303_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zQrySrtDt[8];					/* 조회시작일			*/
	char zQryEndDt[8];					/* 조회종료일			*/
	char zQryTp[1];						/* 조회구분			1.일자별, 2.월별, 3.주간별	*/
	char zStnlnSeqTp[1];				/* 정렬순서구분		1.순, 2.역순*/
	char zBalEvalTp[1];					/* 잔고평가구분		0.기본, 1.이동평균법, 2.선입선출법	*/
};
#define L_TR4303_mid	sizeof(struct TR4303_mid)

struct TR4303_grid
{
	char zQryDt[8];			/*조회일*/
	char zDpstgTotamt[16];	/*예탁총액*/
	char zDpstgMny[16];		/*예탁현금*/
	char zFnoMgn[16];		/*선물옵션증거금액*/
	char zFutsPnlAmt[16];	/*선물손익금액*/
	char zOptBsnPnlAmt[16];	/*옵션매매손익금액*/
	char zOptEvalPnlAmt[16];/*옵션평가손익금액*/
	char zCmsnAmt[16];		/*수수료*/
	char zSumAmt1[16];		/*합계금액1*/
	char zSumAmt2[16];		/*합계금액*/
	char zPnlSumAmt[16];	/*손익합계금액*/
	char zFutsBuyAmt[16];	/*선물매수금액*/
	char zFutsSellAmt[16];	/*선물매도금액*/
	char zOptBuyAmt[16];	/*옵션매수금액*/
	char zOptSellAmt[16];	/*옵션매도금액*/
	char zInAmt[16];		/*입금액*/
	char zOutAmt[16];		/*출금액*/
	char zEvalAmt[16];		/*평가금액*/
	char zAddupEvalAmt[16];	/*합산평가금액*/
	char zAmt2[16];			/*금액2*/
};
#define L_TR4303_grid		sizeof(struct TR4303_grid)

struct TR4303_mod
{
	struct TR4303_mid mid;
	char Out[5];						/* 레코드갯수				*/
	char zFutsAdjstDfamt[16];			/*선물정산차금*/
	char zOptBnsplAmt[16];				/*옵션매매손익금액*/
	char zFnoCmsnAmt[16];				/*선물옵션수수료*/
	char zPnlSumAmt[16];				/*손익합계금액*/
	char zMnyinAsmAmt[16];				/*입금누계금액*/
	char zMnyoutAsmAmt[16];				/*출금누계금액*/
	char zAcntNm[40];					/*계좌명 */
	char Out2[5];                       /* 레코드갯수             */
    struct TR4303_grid grid[1];
};
#define L_TR4303_mod		sizeof(struct TR4303_mod)

/******************************************************************************
* NAME : TR4201
* DESC : 선물옵션 - 선물옵션 지수잔고조회
 ******************************************************************************/
struct TR4201_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
	char zFnoClssCode[2];				/* 선물옵션분류코드		00.전체, 11.선물, 12.옵션*/
	char zPdGrpCode[2];					/* 상품군코드			default 00 */
};
#define L_TR4201_mid	sizeof(struct TR4201_mid)

struct TR4201_grid
{
	char zFnoIsuNo[32];		/* 선물옵션종목번호 */
	char zBnsTp[1];			/* 매매구분 */
	char zBnsTpNm[10];		/* 매매구분 */
	char zBgnDt[8];			/* 당초일 */
	char zUnsttQty[16];		/* 미결제수량 */
	char zFnoAvrPrc[19];	/* 평균가 */
	char zCurprc[13];		/* 현재가 */
	char zUnsttAmt[16];		/* 미결제금액 */
	char zEvalPnlAmt[16];	/* 평가손익금액 */
};
#define L_TR4201_grid		sizeof(struct TR4201_grid)

struct TR4201_mod
{
	struct TR4201_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAcntNm[40];			/* 계좌명 */
	char zDpstgTotamt[16];		/* 예탁총액 */
	char zDps[16];				/* 예수금 */
	char zMgn[16];				/* 증거금액 */
	char zMnyMgn[16];			/* 현금증거금액 */
	char zOrdAbleAmt[16];		/* 주문가능금액 */
	char zMnyOrdAbleAmt[16];	/* 현금주문가능금액 */
	char zPsnOutAbleAmt[16];	/* 인출가능금액 */
	char zPsnOutAbleCurAmt[16];	/* 인출가능현금액 */
	char zFutsPnlAmt[16];		/* 선물손익금액 */
	char zOptBnsplAmt[16];		/* 옵션매매손익금액 */
	char zPnlSumAmt[16];		/* 손익합계금액 */
	char Out2[5];               /* 레코드갯수 */
    struct TR4201_grid grid[1];
};
#define L_TR4201_mod		sizeof(struct TR4201_mod)

/******************************************************************************
* NAME : TR4101
* DESC : 계좌 비밀번호 검증
 ******************************************************************************/
struct TR4101_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
};
#define L_TR4101_mid	sizeof(struct TR4101_mid)

struct TR4101_mod
{
	struct TR4101_mid mid;
	char Out[5];				/* 레코드갯수 */
	char zAvalYn[1];			/* 유효여부 */
};
#define L_TR4101_mod		sizeof(struct TR4101_mod)

/******************************************************************************
* NAME : TR4221
* DESC : 선물옵션 계좌예탁금증거금조회
 ******************************************************************************/
struct TR4221_mid
{
	char In[5];							/* 레코드갯수           */
	char zAcntNo[20];					/* 계좌번호				*/
	char zPwd[8];						/* 비밀번호				*/
};
#define L_TR4221_mid	sizeof(struct TR4221_mid)

struct TR4221_grid
{
	char zPdGrpCodeNm[20];				/* 상품군코드명*/
	char zNetRiskMgn[16];				/* 순위험증거금액*/
	char zPrcFlctMgn[16];				/* 가격변동증거금액*/
	char zSprdMgn[16];					/* 스프레드증거금액*/
	char zMgnRdctAmt[16];				/* 증거금감면액총액*/
	char zMinMgn[16];					/* 최소증거금액*/
	char zPrcMgn[16];					/* 가격증거금액*/
	char zOrdMgn[16];					/* 주문증거금액*/
	char zOptNetBuyAmt[16];				/* 옵션순매수금액*/
	char zCsgnMgn[16];					/* 위탁증거금액*/
	char zUndMgn[16];					/* 인수도증거금액*/
	char zMaintMgn[16];					/* 유지증거금액*/
	char zFutsBuyExecAmt[16];			/* 선물매수체결금액*/
	char zFutsSellExecAmt[16];			/* 선물매도체결금액*/
	char zOptBuyExecAmt[16];			/* 옵션매수체결금액*/
	char zOptSellExecAmt[16];			/* 옵션매도체결금액*/
	char zFutsPnlAmt[16];				/* 선물손익금액*/
	char zTotRiskCsgnMgn[16];			/* 총위험위탁증거금*/
};
#define L_TR4221_grid		sizeof(struct TR4221_grid)

struct TR4221_mod
{
	struct TR4221_mid mid;
	char Out[5];						/* 레코드갯수 */
	char zAcntNm[40];					/* 계좌명	*/
	char zDpsTotamt[16];				/* 예수금총액	*/
	char zDps[16];						/* 예수금		*/
	char zSubstAmt[16];					/* 대용금액		*/
	char zFilupDpsamtTotamt[16];		/* 충당예탁금총액*/
	char zFilupDps[16];					/* 충당예수금*/
	char zFutsPnlAmt[16];				/* 선물손익금액*/
	char zPsnOutAbleAmt[16];			/* 인출가능금액*/
	char zPsnOutAbleCurAmt[16];			/* 인출가능현금액*/
	char zPsnOutAbleSubstAmt[16];		/* 인출가능대용금액*/
	char zMgn[16];						/* 증거금액*/
	char zMnyMgn[16];					/* 현금증거금액*/
	char zOrdAbleAmt[16];				/* 주문가능금액*/
	char zMnyOrdAbleAmt[16];			/* 현금주문가능금액*/
	char zAddMgn[16];					/* 추가증거금액*/
	char zMnyAddMgn[16];				/* 현금추가증거금액*/
	char zAmtPrdayChckInAmt[16];		/* 금전일수표입금액*/
	char zFnoPrdaySubstSellAmt[16];		/* 선물옵션전일대용매도금액*/
	char zFnoCrdaySubstSellAmt[16];		/* 선물옵션금일대용매도금액*/
	char zFnoPrdayFdamt[16];			/* 선물옵션전일가입금액*/
	char zFnoCrdayFdamt[16];			/* 선물옵션금일가입금액*/
	char zFcurrSubstAmt[16];			/* 외화대용금액*/
	char zFnoAcntAfmgnNm[20];			/* 선물옵션계좌사후증거금명*/
	char Out2[5];						/* 레코드갯수 */
    struct TR4221_grid grid[1];
};
#define L_TR4221_mod		sizeof(struct TR4221_mod)

/******************************************************************************
* NAME : TR4401
* DESC : 선물옵션 계좌 거래내역
 ******************************************************************************/
struct TR4401_mid
{
	char In[5];							/* 레코드갯수           */
	char zQryTp[1];						/* 조회구분 0.전체, 1.입출금, 2.입출고, 3.매매 */
	char zAcntNo[20];					/* 계좌번호*/
	char zPwd[8];						/* 비밀번호*/
	char zQrySrtDt[8];					/* 조회시작일*/
	char zQryEndDt[8];					/* 조회종료일*/
	char zSrtNo[10];					/* 시작번호 Default +000000000 */
	char zPdptnCode[2];					/* 상품유형코드 Default 00 */
	char zIsuNo[12];					/* 종목번호*/
};
#define L_TR4401_mid	sizeof(struct TR4401_mid)

struct TR4401_grid
{
	char zTrdDate[8];					/* 거래일자*/
	char zTrdNo[10];					/* 거래번호*/
	char zTrdTpNm[20];					/* 거래구분*/
	char zSmryNo[4];					/* 적요번호*/
	char zSmryNm[40];					/* 적요명*/
	char zCancTpNm[20];					/* 취소구분*/
	char zTrdQty[16];					/* 거래수량*/
	char zTrtax[16];					/* 거래세*/
	char zAdjstAmt[16];					/* 정산금액*/
	char zOvdSum[16];					/* 연체합*/
	char zDpsBfbalAmt[16];				/* 예수금전잔금액*/
	char zSellPldgRfundAmt[16];			/* 매도담보상환금*/
	char zDpspdgLoanBfbalAmt[16];		/* 예탁담보대출전잔금액*/
	char zTrdmdaNm[40];					/* 거래매체명*/
	char zOrgTrdNo[10];					/* 원거래번호*/
	char zIsuNm[40];					/* 종목명*/
	char zTrdUprc[13];					/* 거래단가*/
	char zCmsnAmt[16];					/* 수수료*/
	char zRfundDiffAmt[16];				/* 상환차이금액*/
	char zRepayAmtSum[16];				/* 변제금합계*/
	char zSecCrbalQty[16];				/* 유가증권금잔수량*/
	char zCslLoanRfundIntrstAmt[16];	/* 매도대금담보대출상환이자금액*/
	char zDpspdgLoanCrbalAmt[16];		/* 예탁담보대출금잔금액*/
	char zTrxTime[9];					/* 처리시각*/
	char zInouno[10];					/* 출납번호*/
	char zIsuNo[12];					/* 종목번호*/
	char zTrdAmt[16];					/* 거래금액*/
	char zTaxSumAmt[16];				/* 세금합계금액*/
	char zIntrstUtlfee[16];				/* 이자이용료*/
	char zDvdAmt[16];					/* 배당금액*/
	char zRcvblOcrAmt[16];				/* 미수발생금액*/
	char zTrxBrnNo[3];					/* 처리지점번호*/
	char zTrxBrnNm[40];					/* 처리지점명*/
	char zDpspdgLoanAmt[16];			/* 예탁담보대출금액*/
	char zDpspdgLoanRfundAmt[16];		/* 예탁담보대출상환금액*/
	char zBasePrc[13];					/* 기준가*/
	char zDpsCrbalAmt[16];				/* 예수금금잔금액*/
	char zBoa[16];						/* 과표*/
	char zMnyoutAbleAmt[16];			/* 출금가능금액*/
	char zBcrLoanOcrAmt[16];			/* 수익증권담보대출발생금*/
	char zBcrLoanBfbalAmt[16];			/* 수익증권담보대출전잔금*/
	char zBnsBasePrc[20];				/* 매매기준가*/
	char zTaxchrBasePrc[20];			/* 과세기준가    */
	char zTrdUnit[16];					/* 거래좌수      */
	char zBalUnit[16];					/* 잔고좌수      */
	char zEvrTax[16];					/* 제세금      */  
	char zEvalAmt[16];					/* 평가금액      */
	char zBcrLoanRfundAmt[16];			/* 수익증권담보대출상환금*/
	char zBcrLoanCrbalAmt[16];			/* 수익증권담보대출금잔금*/
	char zAddMgnOcrTotamt[16];			/* 추가증거금발생총액*/
	char zAddMnyMgnOcrAmt[16];			/* 추가현금증거금발생금액*/
	char zAddMgnDfryTotamt[16];			/* 추가증거금납부총액*/
	char zAddMnyMgnDfryAmt[16];			/* 추가현금증거금납부금액*/
	char zBnsplAmt[16];					/* 매매손익금액  */
	char zIctax[16];					/* 소득세        */
	char zIhtax[16];					/* 주민세      */  
	char zLoanDt[8];					/* 대출일    */    
};
#define L_TR4401_grid		sizeof(struct TR4401_grid)

struct TR4401_mod
{
	struct TR4401_mid mid;
	char Out[5];						/* 레코드갯수 */
	char zAcntNm[40];					/* 계좌명	*/
	char Out2[5];						/* 레코드갯수 */
    struct TR4401_grid grid[1];
	char Out3[5];						/* 레코드갯수 */
	char zPnlSumAmt[16];				/* 손익합계금액  */
	char zCtrctAsm[16];					/* 약정누계      */
	char zCmsnAmtSumAmt[16];			/* 수수료합계금액*/
};
#define L_TR4401_mod		sizeof(struct TR4401_mod)

/******************************************************************************
* NAME : TR4202
* DESC : 선물옵션 계좌잔고 및 평가현황2
 ******************************************************************************/
struct TR4202_mid
{
	char In[5];                        /* 레코드갯수             */
	char zAcntNo[20];                  /* 계좌번호               */
	char zInptPwd[8];                  /* 입력비밀번호           */
	char zOrdDt[8];                    /* 주문일                 */
	char zBalEvalTp[1];                /* 잔고평가구분           1.이동평균법, 2.선입선출법*/
	char zFutsPrcEvalTp[1];            /* 선물가격평가구분       1.당초가, 2.전일종가*/
	char zLqdtQtyQryTp[1];             /* 청산수량조회구분       */
};
#define L_TR4202_mid		sizeof(struct TR4202_mid)

struct TR4202_grid
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
#define L_TR4202_grid		sizeof(struct TR4202_grid)

struct TR4202_mod
{
	struct TR4202_mid mid;
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
	struct TR4202_grid grid[1];
};
#define L_TR4202_mod		sizeof(struct TR4202_mod)

/******************************************************************************
* NAME : TR4003
* DESC : 선물옵션 복수청산주문체결예상증거금
 ******************************************************************************/
struct TR4003_grid_mid
{
	char zFnoIsuNo[32];			/* 선물옵션종목번호*/
	char zBnsTp[1];				/* 매매구분        1.매도, 2.매수*/
	char zBnsTpNm[10];			/* 매매구분        1.매도, 2.매수*/
	char zUnsttQty[16];			/* 미결제수량      */
	char zLqdtQty[16];			/* 청산수량        */
	char zExecPrc[13];			/* 체결가          */
};
#define L_TR4003_grid_mid		sizeof(struct TR4003_grid_mid)

struct TR4003_mid
{
	char In[5];            /* 레코드 갯수      */
	char zAcntNo[20];      /* 계좌번호         */
	char zInptPwd[8];      /* 입력비밀번호     */
	char zMprcOrdTp[1];    /* 시장가주문구분   0.현재가, 1.시장가 */
};
#define L_TR4003_mid		sizeof(struct TR4003_mid)

struct TR4003_mid2
{
	char In2[5];           /* 레코드 갯수      */
	struct TR4003_grid_mid grid[100];
};
#define L_TR4003_mid2		sizeof(struct TR4003_mid2)

struct TR4003_mod
{
	struct TR4003_mid mid;
	char In[5];
	struct TR4003_grid_mid grid_mid[1];
	char Out[5];                /* 레코드갯수             */
	char zAcntNm[40];			/* 계좌명                  */
	char zCsgnMgn[16];			/* 위탁증거금액			   */
	char zMnyCsgnMgn[16];		/* 현금위탁증거금액        */
	char zExecAfCsgnMgn[16];	/* 체결후위탁증거금액      */
	char zExecAfMnyCsgnMgn[16];	/* 체결후현금위탁증거금액  */
};
#define L_TR4003_mod		sizeof(struct TR4003_mod)


/******************************************************************************
* NAME : TR4004
* DESC : 선물옵션 복수종목주문
 ******************************************************************************/
struct TR4004_grid_mid
{
	char zFnoIsuNo[32];			/* 선물옵션종목번호*/
	char zBnsTp[1];				/* 매매구분        */
	char zFnoOrdprcPtnCode[2];	/* 선물옵션호가유형코드	03.시장가, 00.현재가*/
	char zOrdQty[16];			/* 주문수량*/
	char zOrdPrc[13];			/* 주문가*/
	char zCurPrc[13];			/* 현재가*/
	char zOrdNo[10];			/* 주문번호 Default +000000000 */
	char zMsgCode[4];			/* 메시지코드 Default 빈값 */
};
#define L_TR4004_grid_mid		sizeof(struct TR4004_grid_mid)

struct TR4004_mid
{
	char In[5];				/* 레코드 갯수      */
	char zAcntNo[20];		/* 계좌번호         */
	char zInptPwd[8];		/* 입력비밀번호     */
	char zTrxTp[1];			/* 처리구분     Default 2*/
	char zFnoTrdPtnCode[2];	/* 선물옵션거래유형코드   Default 03  */
	char zCommdaCode[2];	/* 통신매체코드     Default 01*/
	char zIp[12];			/* IP주소			*/
};
#define L_TR4004_mid		sizeof(struct TR4004_mid)

struct TR4004_mid2
{
	char In2[5];           /* 레코드 갯수      */
	struct TR4004_grid_mid grid[100];
};
#define L_TR4004_mid2		sizeof(struct TR4004_mid2)

struct TR4004_mod
{
	struct TR4004_mid mid;
	char In[5];
	struct TR4004_grid_mid grid_mid[1];
	char Out[5];                /* 레코드갯수              */
	char zAcntNm[40];			/* 계좌명                  */
	char zMsgCode[4];			/* 메시지코드              */
	char zCsgnMgn[16];			/* 위탁증거금액			   */
	char zMnyCsgnMgn[16];		/* 현금위탁증거금액        */
	char zExecAfCsgnMgn[16];	/* 체결후위탁증거금액      */
	char zExecAfMnyCsgnMgn[16];	/* 체결후현금위탁증거금액  */
};
#define L_TR4004_mod		sizeof(struct TR4004_mod)

/******************************************************************************
* NAME : TR4403
* DESC : 선물옵션 계좌별 매체약정및수수료 내역
 ******************************************************************************/

struct TR4403_mid
{
	char In[5];					/* 레코드 갯수      */
	char zChoicInptTp[1];		/* 선택입력구분 Default 3 대표계좌번호*/
	char zAcntNo[20];			/* 대표계좌번호 */
	char zPwd[8];				/* 비밀번호 */
	char zFnoClssCode[2];		/* 선물옵션분류코드 00.전체, 11.선물, 12.옵션*/
	char zPdGrpCode[2];			/* 상품군코드 Default 00 전체*/
	char zCmsnAmtAddupCode[2];	/* 수수료합산코드 
									99.전체, 00.영업점, 01.HTS, 02.WTS, 06.콜센터, 07.ARS, 
									81.MTS(iphone 신어플), 82.MTS(안드로이드 신어플),
									85.MTS(아이패드), 86.MTS(갤럭시탭), 87.MTS(iphone 구어플),
									88.MTS(안드로이드 구어플), 83.MTS(윈도우모바일)*/
	char zSrtDt[8];				/* 시작일 */
	char zEndDt[8];				/* 종료일 */
};
#define L_TR4403_mid		sizeof(struct TR4403_mid)

struct TR4403_grid
{
	char zAcntNo[20];			/* 계좌번호 */
	char zAcntNm[40];			/* 계좌명 */
	char zRmnno[13];			/* 실명확인번호 */
	char zFnoIsuNo[32];			/* 선물옵션종목번호 */
	char zIsuNm[40];			/* 종목명 */
	char zCommdaCode[2];		/* 통신매체코드 */
	char zCommdaNm[40];			/* 통신매체명 */
	char zFnoCmsnAmtGrdCode[20];/* 선물옵션수수료등급코드 */
	char zCmsnAmtAppCode[2];	/* 수수료적용코드 */
	char zCmsnAmtAppUpCodeNm[8];/* 수수료적용코드명 */
	char zBuyQty[16];			/* 매수수량 */
	char zSellQty[16];			/* 매도수량 */
	char zQty1[16];				/* 수량1 */
	char zBuyctrAmt[16];		/* 매수약정금액 */
	char zSlctrAmt[16];			/* 매도약정금액 */
	char zBuyCmsnAmt[16];		/* 매수수수료 */
	char zSellCmsnAmt[16];		/* 매도수수료 */
	char zBnsCmsnAmt[16];		/* 매매수수료 */
	char zCtrctAmt[16];			/* 약정금액 */
};
#define L_TR4403_grid		sizeof(struct TR4403_grid)

struct TR4403_mod
{
	struct TR4403_mid mid;
	char Out[5];                /* 레코드갯수              */
	struct TR4403_grid grid[1];
};
#define L_TR4403_mod		sizeof(struct TR4403_mod)

/******************************************************************************
* NAME : TR8001
* DESC : 고객등급조회
 ******************************************************************************/
struct TR8001_mid {
    char usid[16];
	char accn[16];
};
#define L_TR8001_mid		sizeof(struct TR8001_mid)

struct TR8001_mod {
    char gubn[1];
};
#define L_TR8001_mod		sizeof(struct TR8001_mod)


#pragma	pack()