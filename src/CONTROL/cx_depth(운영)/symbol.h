/******************************************************************************/
/*  Components  : symbol.h						      */
/*  Description	: 조회 프로그램용 SYMBOL 번호 정의			      */
/*  Rev. History: Ver	Date	Description				      */
/*		  ----	-------	----------------------------------------------*/
/*		  1.0	99-08	Initial version				      */
/******************************************************************************/
#ifndef	_SYMBOL
#define	_SYMBOL

#define	GRID	"AXXYYY"	/* A: A-J, XX: #of rows YYY: 1 ROW의 SIZE */

/********************************/
/* 현물시세 관련 SYMBOL	        */
/********************************/
#define	CODX	21		/* 종목코드(단축코드5)		*/
#define	HNAM	22		/* 종목명			*/
#define	CURR	23		/* 현재가			*/
#define	DIFF	24		/* 전일대비			*/
#define	MDGA	25		/* 매도호가			*/
#define	MSGA	26		/* 매수호가			*/
#define	GVOL	27		/* 거래량			*/
#define	GAMT	28		/* 거래대금(억)			*/
#define	SIGA	29		/* 시가				*/
#define	KOGA	30		/* 고가				*/
#define	JEGA	31		/* 저가				*/
#define	CVOL	32		/* 체결거래량			*/
#define	UDYL	33		/* 등락율			*/
#define	CTIM	34		/* 체결시간			*/
#define	CCOL	35		/* 체결구분			*/
#define CPOW	36		/* 체결강도			*/
#define	EGVL	37		/* ECN 거래량			*/
#define ECVL	38		/* ECN 체결량			*/

/*********************************/
/* 정적VI SYMBOL		 */
/*********************************/
#define SVIU	241		/* 정적vi 상승			*/
#define SVID	258		/* 정적vi 하락			*/


/*********************************/
/* 우선호가 관련 SYMBOL		 */
/*********************************/
#define	HTIM	40		/* 호가 시간			*/

#define	DHJ1	41		/* 1차우선호가잔량(매도)	*/
#define	DHJ2	42		/* 2차우선호가잔량(매도)	*/
#define	DHJ3	43		/* 3차우선호가잔량(매도)	*/
#define	DHJ4	44		/* 4차우선호가잔량(매도)	*/
#define	DHJ5	45		/* 5차우선호가잔량(매도)	*/
#define	DHJ6	46		/* 6차우선호가잔량(매도)	*/
#define	DHJ7	47		/* 7차우선호가잔량(매도)	*/
#define	DHJ8	48		/* 8차우선호가잔량(매도)	*/
#define	DHJ9	49		/* 9차우선호가잔량(매도)	*/
#define	DHJA	50		/* 10차우선호가잔량(매도)	*/

#define	DHG1	51		/* 1차우선호가(매도)		*/
#define	DHG2	52		/* 2차우선호가(매도)		*/
#define	DHG3	53		/* 3차우선호가(매도)		*/
#define	DHG4	54		/* 4차우선호가(매도)		*/
#define	DHG5	55		/* 5차우선호가(매도)		*/
#define	DHG6	56		/* 6차우선호가(매도)		*/
#define	DHG7	57		/* 7차우선호가(매도)		*/
#define	DHG8	58		/* 8차우선호가(매도)		*/
#define	DHG9	59		/* 9차우선호가(매도)		*/
#define	DHGA	60		/* 10차우선호가(매도)		*/

#define	SHJ1	61		/* 1차우선호가잔량(매수)	*/
#define	SHJ2	62		/* 2차우선호가잔량(매수)	*/
#define	SHJ3	63		/* 3차우선호가잔량(매수)	*/
#define	SHJ4	64		/* 4차우선호가잔량(매수)	*/
#define	SHJ5	65		/* 5차우선호가잔량(매수)	*/
#define	SHJ6	66		/* 6차우선호가잔량(매수)	*/
#define	SHJ7	67		/* 7차우선호가잔량(매수)	*/
#define	SHJ8	68		/* 8차우선호가잔량(매수)	*/
#define	SHJ9	69		/* 9차우선호가잔량(매수)	*/
#define	SHJA	70		/* 10차우선호가잔량(매수)	*/

#define	SHG1	71		/* 1차우선호가(매수)		*/
#define	SHG2	72		/* 2차우선호가(매수)		*/
#define	SHG3	73		/* 3차우선호가(매수)		*/
#define	SHG4	74		/* 4차우선호가(매수)		*/
#define	SHG5	75		/* 5차우선호가(매수)		*/
#define	SHG6	76		/* 6차우선호가(매수)		*/
#define	SHG7	77		/* 7차우선호가(매수)		*/
#define	SHG8	78		/* 8차우선호가(매수)		*/
#define	SHG9	79		/* 9차우선호가(매수)		*/
#define	SHGA	80		/* 10차우선호가(매수)		*/

#define   MDPR   146   /* 중간가		*/
#define   MDPRMDJ   147   /* 중간가 매도잔량		*/
#define   MDPRMSJ    148   /* 중간가 매수잔량		*/
#define   MDPRMDB  149   /* 중간가 매도직전대비		*/
#define   MDPRMSB    150   /* 중간가 매수직전대비		*/
#define   MDPRRATE    181  /* 중간가 등락율		*/
/********************************/
/* 호가잔량			*/
/********************************/
#define	DHX1	81		/* 1차우선호가잔량대비(매도)	*/
#define	DHX2	82		/* 2차우선호가잔량대비(매도)	*/
#define	DHX3	83		/* 3차우선호가잔량대비(매도)	*/
#define	DHX4	84		/* 4차우선호가잔량대비(매도)	*/
#define	DHX5	85		/* 5차우선호가잔량대비(매도)	*/
#define	DHX6	86		/* 6차우선호가잔량대비(매도)	*/
#define	DHX7	87		/* 7차우선호가잔량대비(매도)	*/
#define	DHX8	88		/* 8차우선호가잔량대비(매도)	*/
#define	DHX9	89		/* 9차우선호가잔량대비(매도)	*/
#define	DHXA	90		/* 10차우선호가잔량대비(매도)	*/

#define	SHX1	91		/* 1차우선호가잔량대비(매수)	*/
#define	SHX2	92		/* 2차우선호가잔량대비(매수)	*/
#define	SHX3	93		/* 3차우선호가잔량대비(매수)	*/
#define	SHX4	94		/* 4차우선호가잔량대비(매수)	*/
#define	SHX5	95		/* 5차우선호가잔량대비(매수)	*/
#define	SHX6	96		/* 6차우선호가잔량대비(매수)	*/
#define	SHX7	97		/* 7차우선호가잔량대비(매수)	*/
#define	SHX8	98		/* 8차우선호가잔량대비(매수)	*/
#define	SHX9	99		/* 9차우선호가잔량대비(매수)	*/
#define	SHXA	100		/* 10차우선호가잔량대비(매수)	*/

#define	DHCJ	101		/* 우선호가총잔량(매도)		*/
#define	DHTJ	102		/* 시간외잔량 (매도)		*/
#define	DHCC	103		/* 우선호가총잔량건수(매도)	*/
#define	DHXX	104		/* 매도총잔량대비		*/
#define	DHTX	105		/* 시간외매도잔량대비		*/

#define	SHCJ	106		/* 우선호가총잔량(매수)		*/
#define	SHTJ	107		/* 시간외잔량 (매수)		*/
#define	SHCC	108		/* 우선호가총잔량건수(매수)	*/
#define	SHXX	109		/* 매수총잔량대비		*/
#define	SHTX	110		/* 시간외매수잔량대비		*/
#define	DSCHA	120		/* 매수잔량 - 매도잔량		*/

//통합호가 수량(거래소(NXT, KRX) 수량 심볼)
#define	TKD1VOL	511		/* KRX 	매도1호가수량*/
#define	TKD2VOL	512		/* KRX 	매도2호가수량*/
#define	TKD3VOL	513		/* KRX 	매도3호가수량*/
#define	TKD4VOL	514		/* KRX 	매도4호가수량*/
#define	TKD5VOL	515		/* KRX 	매도5호가수량*/
#define	TKD6VOL	516		/* KRX 	매도6호가수량*/
#define	TKD7VOL	517		/* KRX 	매도7호가수량*/
#define	TKD8VOL	518		/* KRX 	매도8호가수량*/
#define	TKD9VOL	519		/* KRX 	매도9호가수량*/
#define	TKD10VOL	520		/* KRX 	매도10호가수량*/

#define	TKS1VOL	521		/* KRX 	매수1호가수량*/
#define	TKS2VOL	522		/* KRX 	매수2호가수량*/
#define	TKS3VOL	523		/* KRX 	매수3호가수량*/
#define	TKS4VOL	524		/* KRX 	매수4호가수량*/
#define	TKS5VOL	525		/* KRX 	매수5호가수량*/
#define	TKS6VOL	526		/* KRX 	매수6호가수량*/
#define	TKS7VOL	527		/* KRX 	매수7호가수량*/
#define	TKS8VOL	528		/* KRX 	매수8호가수량*/
#define	TKS9VOL	529		/* KRX 	매수9호가수량*/
#define	TKS10VOL 530		/* KRX 	매수10호가수량*/

#define	TND1VOL	531		/* NXT 	매도1호가수량*/
#define	TND2VOL	532		/* NXT 	매도2호가수량*/
#define	TND3VOL	533		/* NXT 	매도3호가수량*/
#define	TND4VOL	534		/* NXT 	매도4호가수량*/
#define	TND5VOL	535		/* NXT 	매도5호가수량*/
#define	TND6VOL	536		/* NXT 	매도6호가수량*/
#define	TND7VOL	537		/* NXT 	매도7호가수량*/
#define	TND8VOL	538		/* NXT 	매도8호가수량*/
#define	TND9VOL	539		/* NXT 	매도9호가수량*/
#define	TND10VOL	540		/* NXT 	매도10호가수량*/

#define	TNS1VOL	541		/* NXT 	매수1호가수량*/
#define	TNS2VOL	542		/* NXT 	매수2호가수량*/
#define	TNS3VOL	543		/* NXT 	매수3호가수량*/
#define	TNS4VOL	544		/* NXT 	매수4호가수량*/
#define	TNS5VOL	545		/* NXT 	매수5호가수량*/
#define	TNS6VOL	546		/* NXT 	매수6호가수량*/
#define	TNS7VOL	547		/* NXT 	매수7호가수량*/
#define	TNS8VOL	548		/* NXT 	매수8호가수량*/
#define	TNS9VOL	549		/* NXT 	매수9호가수량*/
#define	TNS10VOL	550		/* NXT 	매수10호가수량*/

#define TKDTVOL  551  /* 통합 KRX 	매도총잔량*/
#define TKSTVOL	   552  /* 통합 KRX 	매수총잔량*/
#define TNDTVOL	553  /* 통합 NXT 매도총잔량*/
#define TNSTVOL	 554  /* 통합 NXT 	매수총잔량*/

#define TKMID	 555  /* 통합 KRX 중간가*/
#define TKMIDDV	 556  /* 통합 KRX 매도중간잔량*/
#define TKMIDSV	 557  /* 통합 KRX 매수중간잔량*/
#define TKMIDDR	 558  /* 통합 KRX 중간가매도비*/
#define TKMIDSR	 559  /* 통합 KRX 중간가매수비*/
#define TKMIDRATE	 560  /* 통합 KRX 중간가등락률*/

#define TNMID	 561  /* 통합 NXT 중간가*/
#define TNMIDDV	 562  /* 통합 NXT 매도중간잔량*/
#define TNMIDSV	 563  /* 통합 NXT 매수중간잔량*/
#define TNMIDDR	 564  /* 통합 NXT 중간가매도비*/
#define TNMIDSR	 565  /* 통합 NXT 중간가매수비*/
#define TNMIDRATE	 566  /* 통합 NXT 중간가등락률*/

#define	YSCA	111		/* 예상체결가			*/
#define	YSCV	112		/* 예상주문체결수량		*/
#define	YSDB	113		/* 예상대비   (현재가기준)	*/
#define	YSRT	114		/* 예상등락률 (현재가기준)	*/
#define	YSD2	115		/* 예상대비   (전일종가기준) 	*/
#define	YSR2	116		/* 예상등락률 (전일종가기준)	*/
#define	MDVL	118		/* 누적매도체결량		*/
#define	SDVL	119		/* 누적매수체결량		*/
#define	DVOL	20		/* 매도체결수량			*/
#define	SVOL	19		/* 매수체결수량			*/

/********************************/
/* ELW LP 보유물량		*/
/********************************/
#define	DLP1	861		/* 1차 LP 보유물량(매도)	*/
#define	DLP2	862		/* 2차 LP 보유물량(매도)	*/
#define	DLP3	863		/* 3차 LP 보유물량(매도)	*/
#define	DLP4	864		/* 4차 LP 보유물량(매도)	*/
#define	DLP5	865		/* 5차 LP 보유물량(매도)	*/
#define	DLP6	866		/* 6차 LP 보유물량(매도)	*/
#define	DLP7	867		/* 7차 LP 보유물량(매도)	*/
#define	DLP8	868		/* 8차 LP 보유물량(매도)	*/
#define	DLP9	869		/* 9차 LP 보유물량(매도)	*/
#define	DLPA	870		/* 10차 LP 보유물량(매도)	*/

#define	SLP1	871		/* 1차 LP 보유물량(매수)	*/
#define	SLP2	872		/* 2차 LP 보유물량(매수)	*/
#define	SLP3	873		/* 3차 LP 보유물량(매수)	*/
#define	SLP4	874		/* 4차 LP 보유물량(매수)	*/
#define	SLP5	875		/* 5차 LP 보유물량(매수)	*/
#define	SLP6	876		/* 6차 LP 보유물량(매수)	*/
#define	SLP7	877		/* 7차 LP 보유물량(매수)	*/
#define	SLP8	878		/* 8차 LP 보유물량(매수)	*/
#define	SLP9	879		/* 9차 LP 보유물량(매수)	*/
#define	SLPA	880		/* 10차 LP 보유물량(매수)	*/

#define	DLPX	850		/* 매도 LP 보유총물량		*/
#define SLPX	851		/* 매수 LP 보유총물량		*/

/*********************************/
/* 거래원 관련 SYMBOL		 */
/*********************************/
#define	DTN1	121		/* 매도거래원명1		*/
#define	DTN2	122		/* 매도거래원명2		*/
#define	DTN3	123		/* 매도거래원명3		*/
#define	DTN4	124		/* 매도거래원명4		*/
#define	DTN5	125		/* 매도거래원명5		*/

#define	DTV1	131		/* 매도거래원수량1		*/
#define	DTV2	132		/* 매도거래원수량2		*/
#define	DTV3	133		/* 매도거래원수량3		*/
#define	DTV4	134		/* 매도거래원수량4		*/
#define	DTV5	135		/* 매도거래원수량5		*/

#define	DTR1	141		/* 매도거래원비중1 (백분율 999V99)	*/
#define	DTR2	142		/* 매도거래원비중2 (백분율 999V99)	*/
#define	DTR3	143		/* 매도거래원비중3 (백분율 999V99)	*/
#define	DTR4	144		/* 매도거래원비중4 (백분율 999V99)	*/
#define	DTR5	145		/* 매도거래원비중5 (백분율 999V99)	*/

#define	STN1	151		/* 매수거래원명1		*/
#define	STN2	152		/* 매수거래원명2		*/
#define	STN3	153		/* 매수거래원명3		*/
#define	STN4	154		/* 매수거래원명4		*/
#define	STN5	155		/* 매수거래원명5		*/

#define	STV1	161		/* 매수거래원수량1		*/
#define	STV2	162		/* 매수거래원수량2		*/
#define	STV3	163		/* 매수거래원수량3		*/
#define	STV4	164		/* 매수거래원수량4		*/
#define	STV5	165		/* 매수거래원수  량5		*/

#define	STR1	171		/* 매수거래원비중1 (백분율 999V99)	*/
#define	STR2	172		/* 매수거래원비중2 (백분율 999V99)	*/
#define	STR3	173		/* 매수거래원비중3 (백분율 999V99)	*/
#define	STR4	174		/* 매수거래원비중4 (백분율 999V99)	*/
#define	STR5	175		/* 매수거래원비중5 (백분율 999V99)	*/
#define	WDVL	176		/* 외국계증권사 매도 합계		*/
#define	WSVL	177		/* 외국계증권사 매수 합계		*/

#define	STVS	181		/* 매수거래원수량합			*/
#define	STVSF	182		/* 매수외국거래원수량합			*/
#define	STRS	183		/* 매수거래원비중합 (백분율 999V99)	*/
#define	STRRF	184		/* 매수외국거래원비중합(백분율 999V99)	*/
#define	DTVS	186		/* 매도거래원수량합			*/
#define	DTVSF	187		/* 매도외국거래원수량합			*/
#define	DTRS	188		/* 매도거래원비중합 (백분율 999V99)	*/
#define	DTRRF	189		/* 매도외국거래원비중합(백분율 999V99)	*/

/*********************************/
/* 선물시세관련			 */
/*********************************/
#define	MGJY	201		/* 미결제 약정			*/
#define	GIRG	202		/* 이론 가격			*/
#define	IBAS	203		/* 이론 BASIS			*/
#define	SBAS	204		/* 시장 BASIS			*/
#define	GRAT	205		/* 괴리율			*/
#define	MDIF	206		/* 미결제 약정 대비		*/
#define	GRCH	207		/* 괴리치			*/
#define	IRGF	208		/* 이론가 대비			*/

#define	DHC1	211		/* 1차우선호가건수(매도)	*/
#define	DHC2	212		/* 2차우선호가건수(매도)	*/
#define	DHC3	213		/* 3차우선호가건수(매도)	*/
#define	DHC4	214		/* 4차우선호가건수(매도)	*/
#define	DHC5	215		/* 5차우선호가건수(매도)	*/
#define	DHC6	216		/* 6차우선호가건수(매도)	*/
#define	DHC7	217		/* 7차우선호가건수(매도)	*/
#define	DHC8	218		/* 8차우선호가건수(매도)	*/
#define	DHC9	219		/* 9차우선호가건수(매도)	*/
#define	DHCA	220		/* 10차우선호가건수(매도)	*/

#define	SHC1	221		/* 1차우선호가건수(매수)	*/
#define	SHC2	222		/* 2차우선호가건수(매수)	*/
#define	SHC3	223		/* 3차우선호가건수(매수)	*/
#define	SHC4	224		/* 4차우선호가건수(매수)	*/
#define	SHC5	225		/* 5차우선호가건수(매수)	*/
#define	SHC6	226		/* 6차우선호가건수(매수)	*/
#define	SHC7	227		/* 7차우선호가건수(매수)	*/
#define	SHC8	228		/* 8차우선호가건수(매수)	*/
#define	SHC9	229		/* 9차우선호가건수(매수)	*/
#define	SHCA	230		/* 10차우선호가건수(매수)	*/

#define	SDFF	235		/* 시가 전일대비		*/
#define	KDFF	236		/* 고가 전일대비		*/
#define	JDFF	237		/* 저가 전일대비		*/

/*********************************/
/* 옵션시세관련			 */
/*********************************/
#define TMVL    240             /* 시간가치                     */
#define	TCVO	241		/* CALL OPTION 거래량합		*/
#define	TPVO	242		/* PUT  OPTION 거래량합		*/
#define	DLTA	243		/* 델타 (모델이론값)		*/
#define	GAMA	244		/* 감마 (모델이론값)		*/
#define	THTA	245		/* 세타 (모델이론값)		*/
#define	BEGA	246		/* 베가	(모델이론값)		*/
#define	RHOO	247		/* 로 (모델이론값)		*/
#define	NJBD	248		/* 내재변동성			*/
#define	XTIM	249		/* 지표산출시간			*/
#define	EQVA	250		/* KOSPI등가			*/

/*********************************/
/* 업종관련 SYMBOL		 */
/*********************************/
#define	SHJS	251		/* 상한종목수	*/
#define	SSJS	252		/* 상승종목수	*/ 
#define	BHJS	253		/* 보합종목수	*/
#define	HHJS	254		/* 하한종목수	*/
#define	HRJS	255		/* 하락종목수	*/

/********************************/
/* ECN 시세 관련 SYMBOL	        */
/********************************/
#define	ECURR	723		/* 현재가			*/
#define	EDIFF	724		/* 전일대비			*/
#define	EMDGA	725		/* 매도호가			*/
#define	EMSGA	726		/* 매수호가			*/
#define	EGVOL	727		/* 거래량			*/
#define	EGAMT	728		/* 거래대금(억)			*/
#define	ESIGA	729		/* 시가				*/
#define	EKOGA	730		/* 고가				*/
#define	EJEGA	731		/* 저가				*/
#define	ECVOL	732		/* 체결거래량			*/
#define	EUDYL	733		/* 등락율			*/
#define	ECTIM	734		/* 체결시간			*/
#define	ECCOL	735		/* 체결구분			*/
#define	EMDVL	737		/* 매도잔량			*/
#define EMSVL	738		/* 매수잔량			*/

/*********************************/
/* ECN 우선호가 관련 SYMBOL	 */
/*********************************/
#define	EHTIM	740		/* 호가 시간			*/

#define	EDHJ1	741		/* 1차우선호가잔량(매도)	*/
#define	EDHJ2	742		/* 2차우선호가잔량(매도)	*/
#define	EDHJ3	743		/* 3차우선호가잔량(매도)	*/
#define	EDHJ4	744		/* 4차우선호가잔량(매도)	*/
#define	EDHJ5	745		/* 5차우선호가잔량(매도)	*/
#define	EDHJ6	746		/* 6차우선호가잔량(매도)	*/
#define	EDHJ7	747		/* 7차우선호가잔량(매도)	*/
#define	EDHJ8	748		/* 8차우선호가잔량(매도)	*/
#define	EDHJ9	749		/* 9차우선호가잔량(매도)	*/
#define	EDHJA	750		/* 10차우선호가잔량(매도)	*/

#define	EDHG1	751		/* 1차우선호가(매도)		*/
#define	EDHG2	752		/* 2차우선호가(매도)		*/
#define	EDHG3	753		/* 3차우선호가(매도)		*/
#define	EDHG4	754		/* 4차우선호가(매도)		*/
#define	EDHG5	755		/* 5차우선호가(매도)		*/
#define	EDHG6	756		/* 6차우선호가(매도)		*/
#define	EDHG7	757		/* 7차우선호가(매도)		*/
#define	EDHG8	758		/* 8차우선호가(매도)		*/
#define	EDHG9	759		/* 9차우선호가(매도)		*/
#define	EDHGA	760		/* 10차우선호가(매도)		*/

#define	ESHJ1	761		/* 1차우선호가잔량(매수)	*/
#define	ESHJ2	762		/* 2차우선호가잔량(매수)	*/
#define	ESHJ3	763		/* 3차우선호가잔량(매수)	*/
#define	ESHJ4	764		/* 4차우선호가잔량(매수)	*/
#define	ESHJ5	765		/* 5차우선호가잔량(매수)	*/
#define	ESHJ6	766		/* 6차우선호가잔량(매수)	*/
#define	ESHJ7	767		/* 7차우선호가잔량(매수)	*/
#define	ESHJ8	768		/* 8차우선호가잔량(매수)	*/
#define	ESHJ9	769		/* 9차우선호가잔량(매수)	*/
#define	ESHJA	770		/* 10차우선호가잔량(매수)	*/

#define	ESHG1	771		/* 1차우선호가(매수)		*/
#define	ESHG2	772		/* 2차우선호가(매수)		*/
#define	ESHG3	773		/* 3차우선호가(매수)		*/
#define	ESHG4	774		/* 4차우선호가(매수)		*/
#define	ESHG5	775		/* 5차우선호가(매수)		*/
#define	ESHG6	776		/* 6차우선호가(매수)		*/
#define	ESHG7	777		/* 7차우선호가(매수)		*/
#define	ESHG8	778		/* 8차우선호가(매수)		*/
#define	ESHG9	779		/* 9차우선호가(매수)		*/
#define	ESHGA	780		/* 10차우선호가(매수)		*/

/********************************/
/* ECN 호가잔량			*/
/********************************/
#define	EDHX1	781		/* 1차우선호가잔량대비(매도)	*/
#define	EDHX2	782		/* 2차우선호가잔량대비(매도)	*/
#define	EDHX3	783		/* 3차우선호가잔량대비(매도)	*/
#define	EDHX4	784		/* 4차우선호가잔량대비(매도)	*/
#define	EDHX5	785		/* 5차우선호가잔량대비(매도)	*/
#define	EDHX6	786		/* 6차우선호가잔량대비(매도)	*/
#define	EDHX7	787		/* 7차우선호가잔량대비(매도)	*/
#define	EDHX8	788		/* 8차우선호가잔량대비(매도)	*/
#define	EDHX9	789		/* 9차우선호가잔량대비(매도)	*/
#define	EDHXA	790		/* 10차우선호가잔량대비(매도)	*/

#define	ESHX1	791		/* 1차우선호가잔량대비(매수)	*/
#define	ESHX2	792		/* 2차우선호가잔량대비(매수)	*/
#define	ESHX3	793		/* 3차우선호가잔량대비(매수)	*/
#define	ESHX4	794		/* 4차우선호가잔량대비(매수)	*/
#define	ESHX5	795		/* 5차우선호가잔량대비(매수)	*/
#define	ESHX6	796		/* 6차우선호가잔량대비(매수)	*/
#define	ESHX7	797		/* 7차우선호가잔량대비(매수)	*/
#define	ESHX8	798		/* 8차우선호가잔량대비(매수)	*/
#define	ESHX9	799		/* 9차우선호가잔량대비(매수)	*/
#define	ESHXA	800		/* 10차우선호가잔량대비(매수)	*/

#define	EDHCJ	801		/* 우선호가총잔량(매도)		*/
#define	EDHTJ	802		/* 시간외잔량 (매도)		*/
#define	EDHCC	803		/* 우선호가총잔량건수(매도)	*/
#define	EDHXX	804		/* 매도총잔량대비		*/
#define	EDHTX	805		/* 시간외매도잔량대비		*/

#define	ESHCJ	806		/* 우선호가총잔량(매수)		*/
#define	ESHTJ	807		/* 시간외잔량 (매수)		*/
#define	ESHCC	808		/* 우선호가총잔량건수(매수)	*/
#define	ESHXX	809		/* 매수총잔량대비		*/
#define	ESHTX	810		/* 시간외매수잔량대비		*/

/*********************************/
/* ECN 거래원 관련 SYMBOL	 */
/*********************************/
#define	EDTN1	821		/* 매도거래원명1		*/
#define	EDTN2	822		/* 매도거래원명2		*/
#define	EDTN3	823		/* 매도거래원명3		*/
#define	EDTN4	824		/* 매도거래원명4		*/
#define	EDTN5	825		/* 매도거래원명5		*/

#define	EDTV1	831		/* 매도거래원수량1		*/
#define	EDTV2	832		/* 매도거래원수량2		*/
#define	EDTV3	833		/* 매도거래원수량3		*/
#define	EDTV4	834		/* 매도거래원수량4		*/
#define	EDTV5	835		/* 매도거래원수량5		*/

#define	EDTR1	841		/* 매도거래원비중1 (백분율 999V99)	*/
#define	EDTR2	842		/* 매도거래원비중2 (백분율 999V99)	*/
#define	EDTR3	843		/* 매도거래원비중3 (백분율 999V99)	*/
#define	EDTR4	844		/* 매도거래원비중4 (백분율 999V99)	*/
#define	EDTR5	845		/* 매도거래원비중5 (백분율 999V99)	*/

#define	ESTN1	851		/* 매수거래원명1		*/
#define	ESTN2	852		/* 매수거래원명2		*/
#define	ESTN3	853		/* 매수거래원명3		*/
#define	ESTN4	854		/* 매수거래원명4		*/
#define	ESTN5	855		/* 매수거래원명5		*/

#define	ESTV1	861		/* 매수거래원수량1		*/
#define	ESTV2	862		/* 매수거래원수량2		*/
#define	ESTV3	863		/* 매수거래원수량3		*/
#define	ESTV4	864		/* 매수거래원수량4		*/
#define	ESTV5	865		/* 매수거래원수량5		*/

#define	ESTR1	871		/* 매수거래원비중1 (백분율 999V99)	*/
#define	ESTR2	872		/* 매수거래원비중2 (백분율 999V99)	*/
#define	ESTR3	873		/* 매수거래원비중3 (백분율 999V99)	*/
#define	ESTR4	874		/* 매수거래원비중4 (백분율 999V99)	*/
#define	ESTR5	875		/* 매수거래원비중5 (백분율 999V99)	*/

#define	ESTVS	881		/* 매수거래원수량합			*/
#define	ESTVSF	882		/* 매수외국거래원수량합			*/
#define	ESTRS	883		/* 매수거래원비중합 (백분율 999V99)	*/
#define	ESTRRF	884		/* 매수외국거래원비중합(백분율 999V99)	*/
#define	EDTVS	886		/* 매도거래원수량합			*/
#define	EDTVSF	887		/* 매도외국거래원수량합			*/
#define	EDTRS	888		/* 매도거래원비중합 (백분율 999V99)	*/
#define	EDTRRF	889		/* 매도외국거래원비중합(백분율 999V99)	*/


/*********************************/
/* ECN	예상체결		 */
/*********************************/
#define EYSCA   811             /* ECN 예상체결가               */
#define EYSCV   812             /* ECN 예상주문체결수량         */
#define EYSDB   813             /* ECN 예상대비   (당일종가기준)*/
#define EYSRT   814             /* ECN 예상등락률 (당일종가기준)*/

/*********************************/
/* ETF 관련 SYMBOL		 */
/*********************************/

/*********************************/
/* Control Symbol		 */
/*********************************/
#define	HOGA5	401		/* 5호가 Control			*/
#define	HOGA10	402		/* 10호가 Control			*/
#define	JINFO	403		/* 종목 장정보 				*/
#define	BONG30	410		/* Bong Chart Control			*/
#endif
