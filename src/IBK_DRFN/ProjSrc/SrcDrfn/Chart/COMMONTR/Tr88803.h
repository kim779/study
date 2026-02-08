#if !defined (_HEADERFILE_TR_88803_H)
#define _HEADERFILE_TR_88803_H

// #include "../CommonTR/Tr88803.h"
//#include "../../_Include/DefineCode.h"

//-------------------------------------------------------------------------------
//                    사용TR번호:88803                                           ##
//--------------------------------[INPUT ]---------------------------------------
//typedef struct
//{
//    char gb         [ 1];        /* '1':주식, '2':코스닥    */
//	char code       [ 6];        /* 종목코드                */
//} TR_CODEINFO;
//#define  TR_CODEINFO_SIZE sizeof(TR_CODEINFO)
// DefineCode.h에서 TR_CODEINFO 정의

#define	 SZKEY_CODEREQUEST	"A9999B0000"
#define  SZKEY_TRNO			"88803"

typedef struct _tr88803_i
{
    char  reserved  [ 8];       /* 결과리턴시 되돌려주는 값 */
	char  mainkey	[ 5];       /* 분류키   A로시작         */
	                            // A9999 B0000 일때는 종목리스트로 A,B타입의 정보를 구할 수 있다.
	char  subkey    [ 5];       /* 분류-서브분류키, B로시작 */
	char  sort      [ 1];       /* 0 : code순 
	                               1 : codename순           */
	char  datatype  [ 1];       /* 요청하는 데이터 타입     */
	                            /* A : TR_88803_A
								   B : TR_88803_B           */
	// 아래는 A9999 B0000 일때만 적용. 그외에는 스페이스.
	char  count     [ 4];       /* 데이터 카운트            */
	char  data      [ 1];       /* 카운트만크 TR_CODEINFO형태의 반복]   */
} TR_88803_I;
#define  TR_88803_I_SIZE sizeof(TR_88803_I)
#define  MakeSpaceTR_88803_I(xIn)	memset(&xIn, 0x20, TR_88803_I_SIZE)

//--------------------------------[OUTPUT ]---------------------------------------

typedef struct _tr_88803_A
{
    char gb         [ 1];               /* '1':주식, '2':코스닥    */
	char code       [ 6];				/* 종목코드			*/
	char hname      [20];				/* 종목명(한글)M	*/
} TR_88803_A;
#define  TR_88803_A_SIZE sizeof(TR_88803_A)


//typedef struct _tr_88803_B
//{
//    char gb         [ 1];               /* '1':주식, '2':코스닥    */
//	char code       [ 6];				/* 종목코드			*/
//	char hname      [20];				/* 종목명(한글)M	*/
//    char time       [ 6];				/* 시간(HH:MM:SS)	*/
//    char sign       [ 1];				/* 등락부호			*/
//    char change     [ 9];				/* 등락폭			*/
//    char price      [ 9];				/* 현재가			*/
//    char chrate     [ 5];				/* 등락률			*/
//	char volume     [12];               /* 누적거래량       */
//	char value      [12];               /* 거래대금         */
//} TR_88803_B;
//#define  TR_88803_B_SIZE sizeof(TR_88803_B)

typedef struct _tr_88803_B
{
	char gb         [ 1];               /* '1':주식, '2':코스닥    */
	char code       [ 6];				/* 종목코드			*/
	char hname      [20];				/* 종목명(한글)M	*/
	char price      [10];				/* 현재가			*/
	char sign       [ 1];				/* 등락부호			*/
	char change     [10];				/* 등락폭			*/
	char chrate     [ 6];				/* 등락률			*/
	//char volume     [12];               /* 누적거래량       */
	//char value      [12];               /* 거래대금         */
} TR_88803_B;
#define  TR_88803_B_SIZE sizeof(TR_88803_B)

// TR_88803_C은 관심전용 타입 ojtaso
typedef struct _tr_88803_C
{
    char under           [ 1];   /* 관감리구분 :  0:정상  1:관리  2:감리 */
    char jg_tp           [ 1];   /* 구분                                 */
    char jg_code         [15];   /* 종목코드                             */
    char jg_name         [30];   /* 종목명                               */
    char price           [ 9];   /* 현재가                               */
    char change          [ 6];   /* 전일대비                             */
    char change_rate     [ 5];   /* 대비율                               */
    char sign            [ 1];   /* 등락부호                             */
    char volume          [12];   /* 누적거래량                           */
    char offerho         [ 9];   /* 매도호가                             */
    char bidho           [ 9];   /* 매수호가                             */
    char ofremain        [12];   /* 매도잔량                             */
    char bidremain       [12];   /* 매수잔량                             */
    char offertot        [12];   /* 총매도잔량                           */
    char bidtot          [12];   /* 총매수잔량                           */
    char openam          [ 9];   /* 시가                                 */
    char high            [ 9];   /* 고가                                 */
    char low             [ 9];   /* 저가                                 */
    char value           [14];   /* 거래금액                             */
    char uplmtprice      [ 9];   /* 상한가                               */
    char dnlmtprice      [ 9];   /* 하한가                               */
    char openyak         [ 9];   /* 미결제약정수량                       */
    char parvalue        [ 9];   /* 액면가                               */
    char presign         [ 1];   /* 전일등락부호                         */
    char updnlmtcont_tp  [ 1];   /* 연속구분                             */
    char updnlmtcont     [ 2];   /* 연속구분                             */
    char recprice        [ 9];   /* 기준가                               */
    char cvolume         [ 9];   /* 체결량                               */
    char open_chrate     [ 5];   /* 시가전일대비율                       */
    char high_chrate     [ 5];   /* 고가전일대비율                       */
    char low_chrate      [ 5];   /* 저가전일대비율                       */
    char hightime        [ 6];   /* 고가시간                             */
    char lowtime         [ 6];   /* 저가시간                             */
    char expsign         [ 1];   /* 예상체결 부호                        */
    char expprice        [ 9];   /* 예상체결 가                          */
    char expchang        [ 9];   /* 예상체결 대비                        */
    char expchgrt        [ 9];   /* 예상체결 대비율                      */
    char expvol          [12];   /* 예상체결 거래량                      */
    char color           [ 1];   /* 1:Blue 2:Yellow 3:Green 4:Red        */
} TR_88803_C;
#define  TR_88803_C_SIZE sizeof(TR_88803_C)

typedef struct _tr_88803_o
{
    char  reserved  [ 8];       /* 결과리턴시 되돌려주는 값 */
	char  count     [ 4];       /* 데이터 카운트            */
    // 데이터타입에 따른 데이타를 여기에 붙여서 보낸다.
	char  data      [ 1];		/*  데이타부분              */
} TR_88803_O;

#endif // _HEADERFILE_TR_88803_H
