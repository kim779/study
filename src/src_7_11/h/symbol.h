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
//  Components   : symbol.h	define symbol
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-05	Initial version
//		 02.00  2008-02 Upgrade version (for EU2S)
//		 02.10	2009-06 Customize version (for KATS)
//*****************************************************************************
#ifndef	_SYMBOL
#define	_SYMBOL

const char gnCHAR	= 0x7F;
const char tabCHAR	= 0x09;
const char lfCHAR	= 0x0A;
const char crCHAR	= 0x0D;

#define	SNOT	"000"

//****************************************************************
// major SYMBOL	        
//****************************************************************
#define	MSST	"001"		// 현물
#define	MSFU	"020"		// 선물
#define	MSOP	"030"		// 옵션
#define	MSHO	"040"		// 주식옵션
#define	MSSF	"060"		// 주식선물
#define	MSUP	"010"		// 업종
#define MSPT	"070"		// 상품선물

#define	MGST	"002"		// 현물	체결 GRID
#define	MGFU	"021"		// 선물	체결 GRID
#define	MGOP	"031"		// 옵션	체결 GRID
#define	MGHO	"041"		// 주식옵션 체결 GRID
#define	MGSF	"061"		// 주식선물 체결 GRID

#define MSPS	"071"		// 상품선물 시세
#define MSPH	"072"		// 상품선물 호가
#define	MGPT	"074"		// 상품선물 체결 GRID




//****************************************************************
// 입력코드	
//****************************************************************
#define	ICOD	"301"		// 현물 : MSST + ICOD
				// 선물 : MSFU + ICOD
				// 옵션 : NSOP + ICOD

#define	UDCD	"301"		// 업종대분류
				//	00:KOSPI
				//	10:KOSPI200
				//	11:REAL 200
				//	20:KOSPI100
				//	30:KOSPI IT
				//	90:KOSDAQ
				//	91:KOSDAQ50
				//	92:REAL50
				//	40:한국대표지수(Korea Blue Chip Index)
#define	USCD	"302"		// 업종소분류
#define	UCCD	"303"		// 업종코드(4자리)
				//	00XX:KOSPI
				//	10XX:KOSPI200
				//	11XX:REAL 200
				//	20XX:KOSPI100
				//	30XX:KOSPI IT
				//	90XX:KOSDAQ
				//	91XX:KOSDAQ50
				//	92XX:REAL50
				//	40XX:한국대표지수(Korea Blue Chip Index)
//****************************************************************
// 시세 관련 SYMBOL	        
//****************************************************************
#define	CODX	"021"		// 종목코드(단축코드5)
#define	HNAM	"022"		// 한글종목명
#define	CURR	"023"		// 현재가
#define	DIFF	"024"		// 전일대비
#define	MDGA	"025"		// 매도호가
#define	MSGA	"026"		// 매수호가
#define	GVOL	"027"		// 누적거래량(단위:계약)
#define	GAMT	"028"		// 누적거래대금(단위:억원)
#define	SIGA	"029"		// 시가
#define	KOGA	"030"		// 고가
#define	JEGA	"031"		// 저가
#define	CVOL	"032"		// 체결거래량(계약)
#define	UDYL	"033"		// 등락율
#define	CTIM	"034"		// 체결시간HHMMSS

//****************************************************************
// 우선호가 관련 SYMBOL		 
//****************************************************************
#define	HTIM	"040"		// 호가 시간

#define	DHJ1	"041"		// 1차우선호가잔량(매도)
#define	DHJ2	"042"		// 2차우선호가잔량(매도)
#define	DHJ3	"043"		// 3차우선호가잔량(매도)
#define	DHJ4	"044"		// 4차우선호가잔량(매도)
#define	DHJ5	"045"		// 5차우선호가잔량(매도)
#define	DHJ6	"046"		// 6차우선호가잔량(매도)
#define	DHJ7	"047"		// 7차우선호가잔량(매도)
#define	DHJ8	"048"		// 8차우선호가잔량(매도)
#define	DHJ9	"049"		// 9차우선호가잔량(매도)
#define	DHJA	"050"		// 10차우선호가잔량(매도)

#define	DHG1	"051"		// 1차우선호가(매도)
#define	DHG2	"052"		// 2차우선호가(매도)
#define	DHG3	"053"		// 3차우선호가(매도)
#define	DHG4	"054"		// 4차우선호가(매도)
#define	DHG5	"055"		// 5차우선호가(매도)
#define	DHG6	"056"		// 6차우선호가(매도)
#define	DHG7	"057"		// 7차우선호가(매도)
#define	DHG8	"058"		// 8차우선호가(매도)
#define	DHG9	"059"		// 9차우선호가(매도)
#define	DHGA	"060"		// 10차우선호가(매도)

#define	SHJ1	"061"		// 1차우선호가잔량(매수)
#define	SHJ2	"062"		// 2차우선호가잔량(매수)
#define	SHJ3	"063"		// 3차우선호가잔량(매수)
#define	SHJ4	"064"		// 4차우선호가잔량(매수)
#define	SHJ5	"065"		// 5차우선호가잔량(매수)
#define	SHJ6	"066"		// 6차우선호가잔량(매수)
#define	SHJ7	"067"		// 7차우선호가잔량(매수)
#define	SHJ8	"068"		// 8차우선호가잔량(매수)
#define	SHJ9	"069"		// 9차우선호가잔량(매수)
#define	SHJA	"070"		// 10차우선호가잔량(매수)

#define	SHG1	"071"		// 1차우선호가(매수)
#define	SHG2	"072"		// 2차우선호가(매수)
#define	SHG3	"073"		// 3차우선호가(매수)
#define	SHG4	"074"		// 4차우선호가(매수)
#define	SHG5	"075"		// 5차우선호가(매수)
#define	SHG6	"076"		// 6차우선호가(매수)
#define	SHG7	"077"		// 7차우선호가(매수)
#define	SHG8	"078"		// 8차우선호가(매수)
#define	SHG9	"079"		// 9차우선호가(매수)
#define	SHGA	"080"		// 10차우선호가(매수)

//****************************************************************
// 현물 호가잔량		
//****************************************************************
#define	DHX1	"081"		// 1차우선호가잔량대비(매도)
#define	DHX2	"082"		// 2차우선호가잔량대비(매도)
#define	DHX3	"083"		// 3차우선호가잔량대비(매도)
#define	DHX4	"084"		// 4차우선호가잔량대비(매도)
#define	DHX5	"085"		// 5차우선호가잔량대비(매도)
#define	DHX6	"086"		// 6차우선호가잔량대비(매도)
#define	DHX7	"087"		// 7차우선호가잔량대비(매도)
#define	DHX8	"088"		// 8차우선호가잔량대비(매도)
#define	DHX9	"089"		// 9차우선호가잔량대비(매도)
#define	DHXA	"090"		// 10차우선호가잔량대비(매도)

#define	SHX1	"091"		// 1차우선호가잔량대비(매수)
#define	SHX2	"092"		// 2차우선호가잔량대비(매수)
#define	SHX3	"093"		// 3차우선호가잔량대비(매수)
#define	SHX4	"094"		// 4차우선호가잔량대비(매수)
#define	SHX5	"095"		// 5차우선호가잔량대비(매수)
#define	SHX6	"096"		// 6차우선호가잔량대비(매수)
#define	SHX7	"097"		// 7차우선호가잔량대비(매수)
#define	SHX8	"098"		// 8차우선호가잔량대비(매수)
#define	SHX9	"099"		// 9차우선호가잔량대비(매수)
#define	SHXA	"100"		// 10차우선호가잔량대비(매수)

#define	DHCJ	"101"		// 우선호가총잔량(매도)
#define	DHTJ	"102"		// 시간외매도수량
#define	DHCC	"103"		// 우선호가총잔량건수(매도)
#define	DHXX	"104"		// 총잔량직전대비(매도)
#define	DHTX	"105"		// 매도시간외대비

#define	SHCJ	"106"		// 우선호가총잔량(매수)
#define	SHTJ	"107"		// 시간외매수수량
#define	SHCC	"108"		// 우선호가총잔량건수(매수)
#define	SHXX	"109"		// 총잔량직전대비(매수)
#define	SHTX	"110"		// 매수시간외대비

#define	SYCP	"111"		// 예상체결가
#define	SYCS	"112"		// 예상체결수량

#define	YSCA	"111"		/* 예상체결가			*/
#define	YSCV	"112"		/* 예상주문체결수량		*/
#define	YSDB	"113"		/* 예상대비   (현재가기준)	*/
#define	YSRT	"114"		/* 예상등락률 (현재가기준)	*/
#define	YSD2	"115"		/* 예상대비   (전일종가기준) 	*/
#define	YSR2	"116"		/* 예상등락률 (전일종가기준)	*/
#define	MDVL	"118"		/* 누적매도체결량		*/
#define	SDVL	"119"		/* 누적매수체결량		*/
#define	DVOL	"20"		/* 매도체결수량			*/
#define	SVOL	"19"		/* 매수체결수량			*/


//****************************************************************
// elw 민감도 관련 SYMBOL		 
//****************************************************************
#define	SNJBS	"271"		// 내재변동성(I.V)
#define	SDLTA	"272"		// 델타 (모델이론값)
#define	SBEGA	"273"		// 베가	(모델이론값)
#define	SGRYC	"274"		// 이론가
#define	SLPPO	"275"		// LP보유수량
#define	STHTA	"276"		// 세타 (모델이론값)
#define	SGAMA	"277"		// 감마 (모델이론값)
#define	SRHOO	"278"		// 로	(모델이론값)
#define SDLT2	"279"		// 델타2(영업일기준 모델이론값)

//****************************************************************
// piar 매매손익SYMBOL		 
//****************************************************************
#define	SACCN	"271"		// 계좌번호
#define	SARBT	"272"		// ARBT_IDX
#define	SJQTY	"273"		// 잔고
#define	SPAMT	"274"		// 매입금액
#define	STAMT	"275"		// 매매손익


#define	DTN1	"121"		// 매도거래원명1
#define	DTN2	"122"		// 매도거래원명2
#define	DTN3	"123"		// 매도거래원명3
#define	DTN4	"124"		// 매도거래원명4
#define	DTN5	"125"		// 매도거래원명5


//****************************************************************
// 거래원 관련 SYMBOL		 
//****************************************************************
#define	DTN1	"121"		// 매도거래원명1
#define	DTN2	"122"		// 매도거래원명2
#define	DTN3	"123"		// 매도거래원명3
#define	DTN4	"124"		// 매도거래원명4
#define	DTN5	"125"		// 매도거래원명5

#define	DTV1	"131"		// 매도거래원수량1
#define	DTV2	"132"		// 매도거래원수량2
#define	DTV3	"133"		// 매도거래원수량3
#define	DTV4	"134"		// 매도거래원수량4
#define	DTV5	"135"		// 매도거래원수량5

#define	DTR1	"141"		// 매도거래원비중1 (백분율 999V99)
#define	DTR2	"142"		// 매도거래원비중2 (백분율 999V99)
#define	DTR3	"143"		// 매도거래원비중3 (백분율 999V99)
#define	DTR4	"144"		// 매도거래원비중4 (백분율 999V99)
#define	DTR5	"145"		// 매도거래원비중5 (백분율 999V99)

#define	STN1	"151"		// 매수거래원명1
#define	STN2	"152"		// 매수거래원명2
#define	STN3	"153"		// 매수거래원명3
#define	STN4	"154"		// 매수거래원명4
#define	STN5	"155"		// 매수거래원명5

#define	STV1	"161"		// 매수거래원수량1
#define	STV2	"162"		// 매수거래원수량2
#define	STV3	"163"		// 매수거래원수량3
#define	STV4	"164"		// 매수거래원수량4
#define	STV5	"165"		// 매수거래원수량5

#define	STR1	"171"		// 매수거래원비중1 (백분율 999V99)
#define	STR2	"172"		// 매수거래원비중2 (백분율 999V99)
#define	STR3	"173"		// 매수거래원비중3 (백분율 999V99)
#define	STR4	"174"		// 매수거래원비중4 (백분율 999V99)
#define	STR5	"175"		// 매수거래원비중5 (백분율 999V99)

#define	STVS	"181"		// 매수거래원수량합
#define	STRS	"183"		// 매수거래원비중합 (백분율 999V99)
#define	DTVS	"186"		// 매도거래원수량합
#define	DTRS	"188"		// 매도거래원비중합 (백분율 999V99)

//****************************************************************
// 선물시세관련			 
//****************************************************************
#define	MGJY	"201"		// 미결제 약정
#define	GIRG	"202"		// 이론 가격
#define	IBAS	"203"		// 이론 BASIS
#define	SBAS	"204"		// 시장 BASIS
#define	MDIF	"206"		// 미결제 약정 대비

#define	DHC1	"211"		// 1차우선호가건수(매도)
#define	DHC2	"212"		// 2차우선호가건수(매도)
#define	DHC3	"213"		// 3차우선호가건수(매도)
#define	DHC4	"214"		// 4차우선호가건수(매도)
#define	DHC5	"215"		// 5차우선호가건수(매도)
#define	DHC6	"216"		// 6차우선호가건수(매도)
#define	DHC7	"217"		// 7차우선호가건수(매도)
#define	DHC8	"218"		// 8차우선호가건수(매도)
#define	DHC9	"219"		// 9차우선호가건수(매도)
#define	DHCA	"220"		// 10차우선호가건수(매도)

#define	SHC1	"221"		// 1차우선호가건수(매수)
#define	SHC2	"222"		// 2차우선호가건수(매수)
#define	SHC3	"223"		// 3차우선호가건수(매수)
#define	SHC4	"224"		// 4차우선호가건수(매수)
#define	SHC5	"225"		// 5차우선호가건수(매수)
#define	SHC6	"226"		// 6차우선호가건수(매수)
#define	SHC7	"227"		// 7차우선호가건수(매수)
#define	SHC8	"228"		// 8차우선호가건수(매수)
#define	SHC9	"229"		// 9차우선호가건수(매수)
#define	SHCA	"230"		// 10차우선호가건수(매수)

#define SHSC	"112"		// 체결여부 "" or 현재가
//******************************* *********************************
// 옵션시세관련			 
//****************************************************************
#define	OGRYC	"205"		// 괴리율(계산)	
#define	ODLTA	"243"		// 델타 (모델이론값)
#define	OGAMA	"244"		// 감마 (모델이론값)
#define	OTHTA	"245"		// 세타 (모델이론값)
#define	OBEGA	"246"		// 베가	(모델이론값)
#define	ORHOO	"247"		// 로	(모델이론값)
#define ODLT2	"248"		// 델타2(영업일기준 모델이론값)
#define	ONJBS	"250"		// 내재변동성(I.V)
//****************************************************************
// 현물 SYMBOL:300번 이후					  
//****************************************************************
#define HSJJC	"117"		// 시장조치
				//	0:정상
				//	1:거래정지
				//	2:거래중단

#define	HCODX	"301"		// 단축코드(6):A#####
#define	HCODF	"302"		// 종목코드, Full Code
#define	HCODR	"303"		// 종목코드, Real Code
#define	HENAM	"304"		// 영문명

#define	HSYMB	"305"		// 주권심볼
#define	HSGUB	"306"		// 소속구분
#define	HLGUB	"307"		// 락구분
#define	HTJYU	"308"		// 투자유의
#define	HGGGB	"309"		// 관리/감리구분
#define	HJBGA	"310"		// 자본금규모

#define	HUPMG	"320"		// 업종중분류
#define	HUPSG	"321"		// 업종소분류
#define	HSULG	"322"		// 산업업종대분류
#define	HSUMG	"323"		// 산업업종중분류
#define	HSUSG	"324"		// 산업업종소분류

#define	HJJGN	"325"		// 제조업구분
#define	HK2UP	"326"		// K200업종코드	
#define	HK2K5	"327"		// K200,KQ50구분
#define	HK1K5	"328"		// K100/K50구분
#define	HJTGN	"329"		// 정보통신구분
#define	HKYCD	"330"		// 계열사코드,000:비계열사
#define	HJMGR	"331"		// 종목그룹번호(1-10)
#define	HSJGN	"332"		// 상장구분(종목구분)
#define	HJBGY	"333"		// 자본금(단위:억)
#define	HSJNM	"334"		// 상장주식수(단위:주)
#define	HGMON	"335"		// 결산월
#define	HGJJG	"336"		// 기준주가(단위:원)
#define	HYMGA	"337"		// 액면가(단위:원)
#define	HDYGA	"338"		// 대용가(단위:원)
#define	HSYGB	"339"		// 신용가능구분
#define	HMMGB	"340"		// 매매구분
#define	HSTSY	"341"		// 거래정지사유(거래정지시)
#define	HDLJS	"342"		// 등록주선자
#define	HJSTD	"343"		// 정리매매개시일
#define	HJEND	"344"		// 정리매매종료일
#define	HSJYD	"345"		// 상장(예정)일		YYYYMMDD
#define	HSJCD	"346"		// 상장폐지일		YYYYMMDD
#define	HBDGD	"347"		// 배당기산일		YYYYMMDD
#define	HMMSD	"348"		// 매매개시일		YYYYMMDD
#define	HCLHG	"349"		// 최저호가
#define	HCHHG	"350"		// 최고호가
#define	HPGGA	"351"		// 평가가격
#define	HHSGA	"352"		// 행사가격
#define	HBSGY	"353"		// 불성실공시여부
#define	HKQDG	"354"		// KQ등록일구분(관리용)
#define	HKQGG	"355"		// KQ결산구분(관리용)
#define	HKQJS	"356"		// KQ관리종목 지정 사유
#define	HKQGN	"357"		// KQ종목별 그룹번호
#define	HKQSG	"358"		// KQ시초가 산출구분
#define	HSSDD	"359"		// 수신일(관리용)
#define	HBTND	"360"		// BATCH수신없이경과된일수

#define	HJMTS	"370"		// 종목특이사항
#define	HJJGB	"371"		// 장구분
#define	HUPGB	"372"		// 업종구분
#define	HSHSL	"373"		// 신고가,신저가
#define	HLJGN	"374"		// 락구분,자본금규모
#define	HKRUG	"375"		// 거래정지,업종분류
#define	HTJYJ	"376"		// 투자유의,장구분
#define	HJMSD	"377"		// 주문수량단위

#define	HSHGA	"400"		// 상한가
#define	HLHGA	"401"		// 하한가
#define	HJGSJ	"402"		// 장구분,시장조치
#define	HSGCY	"403"		// 시가총액
#define	HHGDY	"404"		// 호가단위
#define	HMDCG	"405"		// 매도체결ㄴ
#define	HMDCS	"406"		// 매도체결수량
#define	HMDCB	"407"		// 매도체결비중
#define	HMSCG	"408"		// 매수체결건수
#define	HMSCS	"409"		// 매수체결수량
#define	HMSCB	"410"		// 매수체결비중
#define	HJJGA	"411"		// 전일종가
#define	HJJVO	"412"		// 전일거래량
#define	HJJDA	"413"		// 전일거래대금(백만)
#define	HGADB	"414"		// 거래량증가율(전일동시간대대비)
#define	HYJGS	"415"		// 외국인주문가능수량
#define	HYHSR	"416"		// 외국인한도수량
#define	HYJHB	"417"		// 외국인주문한도비율
#define	HYBYB	"418"		// 외국인보유비중
#define	HYBCB	"419"		// 외국인취득비중
#define	HYDSS	"420"		// 외국인당일순매수
#define	HYDMS	"421"		// 외국인당일매수수량
#define	HYDMD	"422"		// 외국인당일매도수량
#define	HDRPK	"425"		// 등락폭
#define	HJHPK	"426"		// 제한폭

#define	HWCHG	"431"		// 52주최고가
#define	HWCLG	"432"		// 52주최저가
#define	HYCHG	"433"		// 년중최고가
#define	HYCLG	"434"		// 년중최저가
#define	HWCHV	"435"		// 52주 최고거래량
#define	HWCLV	"436"		// 52주 최저거래량
#define	HYCHV	"437"		// 년중최고거래량
#define	HYCLV	"438"		// 년중최저거래량
#define	HWCHGD	"441"		// 52주최고가일
#define	HWCLGD	"442"		// 52주최저가일
#define	HYCHGD	"443"		// 년중최고가일
#define	HYCLGD	"444"		// 년중최저가일
#define	HWCHVD	"445"		// 52주최고거래량일
#define	HWCLVD	"446"		// 52주최저거래량일
#define	HYCHVD	"447"		// 년중최고거래량일
#define	HYCLVD	"448"		// 년중최저거래량일

#define	HYSSH	"450"		// 연속상한일수
#define	HYSSS	"451"		// 연속상승일수
#define	HYSHH	"452"		// 연속하한일수
#define	HYSHL	"453"		// 연속하락일수

#define	HBETA	"455"		// 배타계수
#define	HPCR	"456"		// PCR
#define	HPBR	"457"		// PBR
#define	HEPSS	"458"		// EPS_전기
#define	HEPSM	"459"		// EPS_반기
#define	HPERS	"460"		// PER_전기
#define	HPERM	"461"		// PER_반기
#define	HBSYR	"462"		// 배당수익율
#define	HJSYY	"463"		// 주당순이익
#define	HJSJS	"464"		// 주당순자산
#define	HJSCF	"465"		// 주당CashFlow
#define	HJSMC	"466"		// 주당매출액
#define	HEVTA	"467"		// EV/EVITA
#define	HSYBY	"468"		// 신용잔고비율
#define	HSYJG	"469"		// 신용잔고금액

//****************************************************************
// 선물 SYMBOL:300번 이후					  
//****************************************************************
#define FFFCG	"256"		// 최고가갱신횟수
#define FFFCJ	"257"		// 최저가갱신횟수

#define	FCODX	"301"		// 종목코드(단축)
#define	FCODF	"302"		// 종목코드FULL
#define	FCODS	"303"		// 종목 SEQ번호	
#define	FCOD2	"304"		// 종목코드(단축)
#define	FENAM	"305"		// 영문종목명
#define	FENMS	"306"		// 영문종목명(Short)
#define	FHANM	"307"		// 종목명

#define	FSJYJ	"310"		// 상장일
#define	FSJYS	"311"		// 상장일수
#define	FGJYS	"312"		// 기준일수(MT DATA)
#define	FJJYS	"313"		// 잔존일수
#define	FFGRD	"314"		// 최초거래일(MT DATA)
#define	FLGRD	"315"		// 최종거래일
#define	FLGRY	"316"		// 최종거래일여부
#define	FBDYM	"317"		// 배당액지수미래가치	999V999999
#define	FGJGB	"318"		// 기준가격구분
#define	FGJGA	"319"		// 기준가격		99999V99
#define	FIRGJ	"320"		// 이론가(결제가)	99999V99
#define	FIRGB	"321"		// 이론가(기준가)	99999V99
#define	FCDGR	"322"		// CD금리  		999V99
#define	FJSGB	"323"		// 정산가격구분
#define	FJJSG	"324"		// 전일정산가격		99999V99
#define	FJJGB	"325"		// 전일종가구분
#define	FJCSR	"326"		// 전일체결수량(단위:계약)
#define	FJCAM	"327"		// 전일체결대금(단위:억원)
#define	FJMGJ	"328"		// 전일미결제약정수량
#define	FSJHG	"329"		// 상장중최고가		9999V99
#define	FHGYJ	"330"		// 최고가일자
#define	FSJLG	"331"		// 상장중최저가		9999V99
#define	FLGYJ	"332"		// 최저가일자
#define	FYHYJ	"333"		// 입회일자
#define	FSJYN	"334"		// 시장가허용구분

#define	FSJHY	"335"		// 시장가허용일
#define	FSJHT	"336"		// 시장가허용시간
#define	FCJGG	"337"		// 최종결제가격		9999V99
#define	FCJGB	"338"		// 최종결제가격구분
#define	FGRYG	"339"		// 괴리율구분(MT DATA)
#define	FGRYD	"340"		// 괴리율(MT DATA)
#define	FCBHG	"341"		// CB적용상한가		9999V99
#define	FCBLG	"342"		// CB적용하한가		9999V99
#define	FSSDD	"343"		// 최종수신일
#define	FBTND	"344"		// BATCH없이경과된일수

#define	FK200	"352"		// K200지수
#define	FGRYC	"205"		// 괴리율(계산)

#define	FSHGA	"354"		// 상한가		99999V99
#define	FLHGA	"355"		// 하한가		99999V99
#define	FJJGA	"356"		// 전일종가		99999V99
#define	FDRPK	"357"		// 등락폭(상한가-하한가)

#define	FCGGB	"400"		// 체결구분
				// 매수 : +
				// 매도 : -

#define	FJYGB	"401"		// 장운용구분
#define	FMGCY	"402"		// 미결제체결일자
#define	FMGGB	"403"		// 미결제약정구분
#define	FGSGA	"404"		// 정산가격
#define	FGSGB	"405"		// 정산구분
#define	FCJKG	"406"		// 최종결제가
#define	FCJKB	"407"		// 최종결제가구분
#define	FHGDY	"408"		// 호가단위
#define	FGMUL	"411"		// 거래승수
#define JSTAT	"510"		// 장상태구분
#define	YCPRC	"511"		// 예상체결
#define	YDIFF	"512"		// 예상체결대비

//****************************************************************
// 옵션 SYMBOL:300번 이후					  
//****************************************************************
#define	OCODX	"301"		// 종목코드
#define	OCODF	"302"		// 종목코드(FULL)
#define	OIBYJ	"303"		// 입회일자
#define	OENAM	"305"		// 영문종목명
#define	OHNAM	"306"		// 한글종목명(Short)
#define	OOSGB	"307"		// 옵션구분
#define	ODJGB	"308"		// 거래대상종목구분
#define	OMGYM	"309"		// 만기년월
#define	OATMG	"310"		// 행사가격 9999V99
#define	OCODS	"311"		// 종목 SEQ번호	
#define	OYHGB	"312"		// 권리행사유형구분
#define	OGJYS	"313"		// 기준일수
#define	OJJYS	"314"		// 잔존일수
#define	OYMGB	"315"		// 최근월물구분
#define	OATGB	"316"		// ATM구분
#define	OSJYJ	"317"		// 상장일
#define	OFGRD	"318"		// 최초거래일
#define	OLGRD	"319"		// 최종거래일
#define	OLGGB	"320"		// 최종거래구분
#define	ONARG	"321"		// 신규/추가/기존 구분
#define	ONAYJ	"322"		// 신규/추가 일자
#define	OBDHG	"323"		// 배당액지수현재가치	999V999999
#define	OIRGJ	"324"		// 이론가(결재가)	9999V999999
#define	OMMJG	"325"		// 매매증거금기준가	9999V99
#define	OMMGB	"326"		// 매매증거금기준가구분
#define	OSHGA	"327"		// 상한가		9999V99
#define	OLHGA	"328"		// 하한가		9999V99
#define	OGDGJ	"329"		// 가격단위기준값(3.00)
#define	OHGD1	"330"		// 호가단위(0.01)
#define	OHGD5	"331"		// 호가단위(0.05)
#define	OCDGR	"332"		// CD금리		999V999
#define	OMGCS	"333"		// 미결제체결수량
#define	OJYSG	"334"		// 전일시가
#define	OJYHG	"335"		// 전일고가
#define	OJYJG	"336"		// 전일저가
#define	OJJGB	"337"		// 전일종가구분
#define	OJJVO	"338"		// 전일거래량
#define	OJJDA	"339"		// 전일거래대금(억원)
#define	OYHYJ	"340"		// 연중최고일자
#define	OYHGA	"341"		// 연중최고가격
#define	OYLYJ	"342"		// 연중최저일자
#define	OYLGA	"343"		// 연중최저가격
#define	OSJYN	"344"		// 시장가허용구분
#define	OSJHY	"345"		// 시장가허용적용일자
#define	OSJHT	"346"		// 시장가허용적용시간
#define	OGJGA	"347"		// 기준가격		9999V99
#define	OGJGB	"348"		// 기준가격구분
#define	OSJHG	"349"		// 상장최고가격		9999V99
#define	OSJHJ	"350"		// 상장최고일자
#define	OSJLG	"351"		// 상장최저가격		9999V99
#define	OSJLJ	"352"		// 상장최저일자
#define	OUPDT	"353"		// Update Date
#define	ONJBD	"354"		// 내재가치변동성
#define	OAGHD	"355"		// 자동권리행사대상구분
#define	OJJYB	"356"		// 주요종목여부
#define	OGGYS	"357"		// 경과일수(if-1, deleted)
#define	OJSEQ	"358"		// 조회SEQ번호
#define	OJJGA	"359"		// 전일종가
#define	ODRPK	"370"		// 등락폭

#define	OCGGB	"401"		// 체결구분
				// 매수 : +
				// 매도 : -
#define	OJYGB	"402"		// 장운용구분
#define	OMGCY	"403"		// 미결제체결일자
#define	OMGGB	"404"		// 미결제약정구분
#define	OGSGA	"405"		// 정산가격
#define	OGSGB	"406"		// 정산가구분
#define	OCJGK	"407"		// 최종결제가
#define	OCJGB	"408"		// 최종결제가구분
#define	OGMUL	"411"		// 거래승수
#define	OK200	"426"		// K200지수
#define	OFUMN	"427"		// 선물최근월
#define	OHGDY	"433"		// 호가단위


//****************************************************************
// 업종 SYMBOL : MSUP				  
//****************************************************************
#define	UDHG1	"041"		// 매도 1호가 지수
#define	UDHG2	"042"		// 매도 2호가 지수
#define	USHG1	"046"		// 매수 1호가 지수
#define	USHG2	"047"		// 매수 2호가 지수

#define	UFHBS	"050"		// 선물 현재가 베이시스
#define	UHDB1	"051"		// 매도 1호가 선물 현재가 베이시스
#define	UHDB2	"052"		// 매도 2호가 선물 현재가 베이시스
#define	UHSB1	"056"		// 매수 1호가 선물 현재가 베이시스
#define	UHSB2	"057"		// 매수 2호가 선물 현재가 베이시스
#define	UFJBS	"060"		// 선물 절대호가 베이시스
#define	UJDB1	"061"		// 매도 1호가 선물 절대호가 베이시스
#define	UJDB2	"062"		// 매도 2호가 선물 절대호가 베이시스
#define	UJSB1	"066"		// 매수 1호가 선물 절대호가 베이시스
#define	UJSB2	"067"		// 매수 2호가 선물 절대호가 베이시스
#define	UFSBS	"070"		// 선물 상대호가 베이시스
#define	USDB1	"071"		// 매도 1호가 선물 상대호가 베이시스
#define	USDB2	"072"		// 매수 2호가 선물 상대호가 베이시스
#define	USSB1	"076"		// 매도 1호가 선물 상대호가 베이시스
#define	USSB2	"077"		// 매수 2호가 선물 상대호가 베이시스

#define	UCODX	"301"		// 업종코드(2자리)
#define	UUPGB	"302"		// 시장구분
#define	UUPCD	"303"		// 시장구분+업종코드
#define	UHANM	"304"		// 한글명
#define	UENAM	"305"		// 영문명
#define	UJYJS	"306"		// 전일지수

//****************************************************************
// 주식옵션 SYMBOL:300번 이후					  
//****************************************************************
#define	HOMUL	"411"		// 거래승수
#define	HOCDX	"499"		// 기초자산주식코드
#define	HOKDY	"500"		// 거래승수
#define OSJBS	"210"		// 주식옵션 내재변동성

//****************************************************************
// 주식선물 SYMBOL:300번 이후					  
//****************************************************************
#define SFGJM   "409"		// 결재방법
#define SFSJJ	"410"		// 시장조성
#define	SFMUL	"411"		// 거래승수
#define	SFCDX	"499"		// 기초자산주식코드
#define	SFKDY	"500"		// 거래승수


//****************************************************************
// 현물 체결 내역 GRID						  
// 중분류 : MGST사용						  
//****************************************************************
// GRID SYMBOL
#define	HGRCH	"310"		// 일반체결내역
#define	HGRGN	"410"		// 가격변동체결내역

// GRID COLUMN SYMBOL
#define	HGRCG	"023"		// 체결가
#define	HGRDB	"024"		// 전일대비
#define	HGRMD	"025"		// 매도호가
#define	HGRMS	"026"		// 매수호가
#define	HGRCV	"032"		// 체결량
#define	HGRVL	"027"		// 누적거래량
#define	HGRDY	"033"		// 등락율
#define	HGRCT	"034"		// 체결시간

// 일반 OUTPUT SYMBOL
#define	HGRBA	"305"		// 이전/다음

//****************************************************************
// 선물 체결 내역 GRID						  
// 중분류 : MGFU사용						  
//****************************************************************
// GRID SYMBOL
#define	FGRCH	"310"		// 종목체결내역

// GRID COLUMN SYMBOL
#define	FGRCG	"023"		// 체결가
#define	FGRDB	"024"		// 전일대비
#define	FGRMD	"025"		// 매도호가
#define	FGRMS	"026"		// 매수호가
#define	FGRVL	"027"		// 누적거래량(단위:계약)
#define	FGRAM	"028"		// 누적거래대금(단위:백만원)
#define	FGRCV	"032"		// 체결량
#define	FGRDY	"033"		// 등락율
#define	FGRTM	"034"		// 시간
#define	FGRGB	"035"		// 체결구분

// 일반 OUTPUT SYMBOL
#define	FGRBA	"305"		// 이전/다음

//****************************************************************
// 옵션 체결 내역 GRID						  
// 중분류 : MGOP사용						  
//****************************************************************
// GRID SYMBOL
#define	OGRCH	"310"		// 종목체결내역

// GRID COLUMN SYMBOL
#define	OGRCG	"023"		// 체결가
#define	OGRDB	"024"		// 전일대비
#define	OGRMD	"025"		// 매도호가
#define	OGRMS	"026"		// 매수호가
#define	OGRVL	"027"		// 누적거래량(단위:계약)
#define	OGRAM	"028"		// 누적거래대금(단위:백만원)
#define	OGRCV	"032"		// 체결량
#define	OGRDY	"033"		// 등락율
#define	OGRTM	"034"		// 시간
#define	OGRGB	"035"		// 체결구분
#define	OGRMG	"201"		// 미결제약정수량(단위:계약)

// 일반 OUTPUT SYMBOL
#define	OGRBA	"305"		// 이전/다음

//****************************************************************
// 체결+바스켓
//****************************************************************
#define	CCODX	"021"		// 종목코드
#define	CHNAM	"022"		// 종목명
#define	CCURR	"023"		// 현재가
#define	CMDGA	"051"		// 매도호가
#define	CMDHJ	"041"		// 매도잔량
#define	CMSGA	"071"		// 매수호가
#define	CMSHJ	"061"		// 매수잔량

#define	CGUBN	"900"		// RTS 구분
				//	A:바스켓 정보
				//	B:바스켓 주문오류응답
				//	C:바스켓 주문체결
				//	D:현물 주문 체결
				//	K:계좌별 종목구분별 FEE송신


#define	CUSER	"901"		// 의뢰자 및 관리자ID
#define	CBNUM	"902"		// 바스켓번호

#define	CJMBN	"903"		// 주문번호
				//	주식	:5
				//	선물옵션:6

#define	CFDID	"904"		// 펀드ID(usid+fdid)
#define	CJUID	"905"		// 주문자ID
#define	CJMGB	"906"		// 주문 구분
				//	1:원주문
				//	2:정정주문
				//	3:취소주문

#define	CACCN	"907"		// 계좌번호(일반)
#define	CANAM	"908"		// 계좌명
#define	CMDMS	"909"		// 매도/매수 구분
				//	매도:1
				//	매수:2

#define	CJGSU	"910"		// 잔고수량
#define	CMYDG	"911"		// 매입평균단가
#define	CCHJS	"912"		// 청산주문가능수량
#define	CFJNO	"913"		// 펀드주문번호
#define	CJMGA	"914"		// 주문가격
#define	CJMSU	"915"		// 주문수량
#define	CCHGA	"916"		// 체결단가
#define	CCHSU	"917"		// 체결수량
#define	CMCHS	"918"		// 미체결수량
#define	CFJCS	"919"		// 선물주문체결상태
				//	01:주문접수
				//	02:체결중
				//	03:체결완료
				//	04:정정접수
				//	05:정정완료
				//	06:취소접수
				//	07:취소완료
				//	08:거부

#define	COJMN	"920"		// 원주문번호
#define	CJMYH	"921"		// 주문유형
				//	01:지정가
				//	02:시장가
				//	03:조건부지정가
				//	04:최유리지정가

#define	CJJSR	"922"		// 정정수량
#define	CCSSR	"923"		// 취소수량
#define	CGBSR	"924"		// 거부수량
#define	CMDMC	"925"		// 매도미체결수량
#define	CMSMC	"926"		// 매수미체결수량
#define	CRAT1	"927"		// 체결률(체결수량/주문수량)
#define	CRAT2	"928"		// 체결률(100 - (미체결수량/주문수량*100) => [999V9]
				// #미체결금액 : 시장가 주문인경우 매수(상한가), 매도(하한가)
#define	CKRDY	"930"		// 거래단위(잔고 선물옵션)
#define	CCTIM	"931"		// 체결시간(HHMMSS)
#define	CENAM	"932"		// 종목명 영문(Short)
#define	CDMSI	"933"		// 당일 매매(청산)손익
#define	CCNUM	"934"		// 체결일련번호
#define	CMYKY	"935"		// 매입금액
#define	CJGBN	"936"		// 주문구문
				//	1:바스켓
				//	3:일반주문, 
				//	4:펀드주문
				//	5:시스템
				//	6:착오매매정리

#define CTCOD	"937"		// 운용코드

////////////////////////////////////////////////////////////////
#define CMDCQ	"938"		// 매도체결갯수(tfdchqt)
#define CMSCQ	"938"		// 매수체결갯수(tfdchqt)
#define CMDSN	"939"		// i_tfdchqt->msqt - i_tfdchqt->msqt
//****************************************************************
// V2 호환성 정의( EU2S 사용안함)
//#define	CMDCQ	CMDCQ		// 매도체결건수
//#define	CMSCQ	CMDSN		// 매수체결건수
//****************************************************************


#define	CQSBP	"940"		// 건별 체결단가 (선물옵션의 주문내역에 체결시에만 건별)
#define CQSBN	"941"		// 건별 체결수량 (선물옵션의 주문내역에 체결시에만 건별)
#define CJMCT	"942"		// 주문조건
				//	0:일반
				//	1:IOC
				//	2:FOK
//****************************************************************
// V2 호환성 정의 (EU2S 사용안함)
#define	CNETQ	CQSBP		// NET	
#define CMDCC	CQSBN		// 차익주문매도체결건수
#define CMSCC	CJMCT		// 차익주문매수체결건수
//****************************************************************




#define CJOGB	"943"		// 종목구분
				//	1:KOSPI
				//	2:KOSDAQ
				//	3:ETFinKOSPI
				//	4:ETFinKOSDAQ
				//	5:ELW

#define CFDON	"946"		// 펀드 주문번호
#define	CKIND	"947"		// RTS 구분자
				// D:현물 주문체결
				// E:현물잔고
				// F:선물옵션 주문체결
				// G:선물옵션잔고
				// J:펀드별 SUM정보
				// H:CD주문별 SUM정보
				// I:CD주문별 사용자변경
				// K:계좌별 종목구분별 FEE송신
				// L:CD주문 시분할 처리 진행정보
				// M:OCIMS 모니터링

#define	CBJSS	"950"		// 비중순서
#define	CGBHN	"951"		// (한글2자리-4Byte)
				// 매도
				// 매수
				// 정정
				// 취소

#define	CGSBJ	"952"		// 구성비중(999V999)
#define	CBSKN	"953"		// 바스켓명
#define	CBSGB	"954"		// 바스켓주문구분(신규/청산 + 매도/매수)
#define	CBJGA	"955"		// 바스켓 주식 주문금액
#define	CBCGA	"956"		// 바스켓 주식 체결금액
#define	CBJCH	"957"		// 바스켓 주식 체결률
#define	CBFOJ	"958"		// 바스켓 선물 주문수량
#define	CBFOC	"959"		// 바스켓 선물 체결수량
#define	CBFOR	"960"		// 바스켓 선물 체결금액
				
#define	CBJMS	"961"		// 바스켓주문상태
				//	1:대기
				//	2:주문
				//	3:완료

#define	CBJSR	"962"		// 바스켓 코드 주문수량
#define	CBSCR	"963"		// 바스켓 코드 체결수량
#define	CBSSR	"964"		// 바스켓 코드 취소수량
#define	CBKSR	"965"		// 바스켓 코드 거부수량
#define	CBCJS	"966"		// 바스켓 콜옵션 주문수량
#define	CBCCS	"967"		// 바스켓 콜옵션 체결수량
#define	CBCCK	"968"		// 바스켓 콜옵션 체결금액
#define	CBPJS	"969"		// 바스켓 풋옵션 주문수량
#define	CBPCS	"970"		// 바스켓 풋옵션 체결수량
#define	CBPCK	"971"		// 바스켓 풋옵션 체결금액
#define	CBFCD	"972"		// 바스켓 선물코드
#define	CBCCD	"973"		// 바스켓 콜옵션 코드
#define	CBPCD	"974"		// 바스켓 풋옵션 코드
#define	CBMCK	"975"		// 바스켓 미체결금액
#define	CBCKY	"976"		// 바스켓코드 체결금액
#define	CBCMS	"977"		// 바스켓코드 미체결수량
#define	CBRAT	"978"		// 바스켓 선물옵션 체결률
#define	CBMCS	"979"		// 바스켓 미체결수량
#define	CBCSR	"980"		// 바스켓 체결수량
#define	CBJJS	"981"		// 바스켓 주문수량
#define	CJMER	"982"		// 바스켓 코드 주문에러번호
#define	COJMG	"983"		// 바스켓 최초주문금액


//****************************************************************
// CD 주문 관련
//****************************************************************
#define CDOID	"985"		// CDID
#define CDSEQ	"986"		// CD SEQ
//****************************************************************
// V2 호환성 정의 (EU2S 사용 안함)
#define	CHTCD	"984"		// 사용안함
#define	CFTCD	CDOID		// V2 호환성 위해서 존재
#define	COTCD	CDSEQ		// V2 호환성 위해서 존재

//****************************************************************
// OCIMS 모니터링
//****************************************************************
#define	CRSEQ	"031"		// 리얼순번
#define CTEAM	"032"		// 팀코드
#define CJUGB	"033"		// 시장거래 구분 
				// 0:KOSDAQ 
				// 1:장내거래 
				// 2:장외단주
				// 3:프리보드 
				// 4:수익증권
				// 8:신주인수권
				// 9:장외매매
#define	CPMGB	"034"		// 프로그램호가 구분
				// 1: 프로그램
				// 0: 일반
#define MIREA	"035"		// 미래에셋계좌구분
//****************************************************************

#define CDORN	"987"		// CD주문 회차
#define CDCGB	"988"		// 종목구분
				//	1:선물
				//	2:옵션
				//	3:개별주식옵션
				//	4:선물스프래드
				//	5:주식선물

#define CDFEE	"989"		// FEE 세금
#define CDOAL	"990"		// 시디주문 총 주문나간 회차
#define CDORR	"991"		// CD주문 남은 회차
#define CDSOC	"992"		// CD주문 스팟주문 수량


#endif


