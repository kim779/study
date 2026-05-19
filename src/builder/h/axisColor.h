// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information Inc.
//
//  Components   : AXISCOLOR.H	AXIS FORM object
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2009-12	Initial version
// *****************************************************************************

#ifndef	__AXISCOLOR_H__
#define	__AXISCOLOR_H__


// 내부브라우저
#define CLR_FORMHDRTEXT                   		1		// 캡션바 텍스트
#define CLR_FORMLINE                      		2		// 테두리색
#define CLR_FORMBACK                      		3		// 배경

// 판넬
#define CLR_PNLBACKTOP                    		4		// 상
#define CLR_PNLBACKBOTTOM                 		5		// 하
#define CLR_PNLLINE                       		6		// 테두리색(LineColor)

// 그룹
#define CLR_GRPLINE                       		6		// 테두리색

// 박스
#define CLR_BOXBACK                       		7		// 배경
#define CLR_BOXLINE                       		6		// 테두리색(LineColor)

// Grid
#define CLR_GRIDHDRBACKTOP                		8		// 상
#define CLR_GRIDHDRBACKBOTTOM             		9		// 하
#define CLR_GRIDHDRTEXT                   		2		// 헤더텍스트
#define CLR_GRIDHDRLINE                   		10		// 헤더라인색
#define CLR_GRIDLINE                      		6		// 테두리색(LineColor)
#define CLR_GRIDDATALINE                  		11		// 데이터라인색
#define CLR_GRIDROW1ST                    		1		// 1st배경
#define CLR_GRIDROW2ND                    		12		// 2nd배경
#define CLR_GRIDDATA                      		2		// 데이터
#define CLR_GRIDSEL                       		13		// 선택 FocusColor

// Table
#define CLR_TBLHDRBACK                    		14		// 헤더배경
#define CLR_TBLHDRTEXT                    		2		// 헤더텍스트
#define CLR_TBLHDRLINE                    		15		// 헤더라인색
#define CLR_TBLLINE                       		6		// 테두리색(LineColor)
#define CLR_TBLDATA                       		2		// 데이터
#define CLR_TBLBACK                       		16		// 강조배경

// Tab
#define CLR_TABTEXT                       		88		// Tab 텍스트
#define CLR_TABSELTEXT                    		89		// 선택 Tab 텍스트

// Button
#define CLR_BTNTEXT                       		90		// 텍스트

// 데이터
#define CLR_DATABACK                      		1		// 배경
#define CLR_DATATEXT                      		2		// 표준 텍스트
#define CLR_DATACONUP                     		17		// 상승(▲)텍스트
#define CLR_DATACONDOWN                   		18		// 하락(▲)텍스트
#define CLR_DATACONSTAY                   		19		// 보합텍스트
#define CLR_DATACONMAX                    		17		// 상한(▲)텍스트
#define CLR_DATACONMIN                    		18		// 하한(▲)텍스트
#define CLR_DATATRDBUYBACK                		20		// 매수거래원 배경
#define CLR_DATATRDSELLBACK               		21		// 매도거래원 배경
#define CLR_DATATRDBUYDATABACK            		22		// 매수거래원 데이터 배경
#define CLR_DATATRDSELLDATABACK           		23		// 매도거래원 데이터 배경
#define CLR_DATAEXTBACK                   		24		// 배경
#define CLR_DATAEXTTEXT                   		2		// 텍스트

// 주문
#define CLR_ORDBUYBACK                    		25		// 매수배경
#define CLR_ORDSELLBACK                   		26		// 매도배경
#define CLR_ORDMODIFYBACK                 		27		// 정정배경
#define CLR_ORDCANCELBACK                 		28		// 취소배경
#define CLR_ORDBUYTEXT                    		29		// 매수버튼 텍스트
#define CLR_ORDSELLTEXT                   		30		// 매도버튼 텍스트
#define CLR_ORDMODIFYTEXT                 		31		// 정정버튼 텍스트
#define CLR_ORDCANCELTEXT                 		32		// 취소버튼 텍스트
#define CLR_ORDTEXT                       		91		// 텍스트
#define CLR_ORDSELTEXT                    		92		// 선택텍스트

// 호가컨트롤
#define CLR_BIDSELLBACK                   		23		// 매도배경
#define CLR_BIDSELLTEXT                   		18		// 매도텍스트
#define CLR_BIDSELLGRP                    		33		// 매도그래프
#define CLR_BIDBUYBACK                    		22		// 매수배경
#define CLR_BIDBUYTEXT                    		17		// 매수텍스트
#define CLR_BIDBUYGRP                     		34		// 매수그래프
#define CLR_BIDHDRBACK                    		63		// 헤더배경
#define CLR_BIDCURLINE                    		35		// 현재가테두리
#define CLR_BIDOVERLT                     		36		// 좌상
#define CLR_BIDOVERRB                     		37		// 우하
#define CLR_BIDLINE                       		6		// 테두리색(LineColor
#define CLR_BIDINLINE                     		15		// 내부 Line
#define CLR_BIDVOLUME                     		64		// 거래량
#define CLR_BIDOVERTIME                   		65		// 시간외
#define CLR_BIDDOT                        		39		// 점선
#define CLR_BIDHIGH                       		17		// 고가
#define CLR_BIDAVR                        		40		// 가중평균가
#define CLR_BID5MA                        		41		// 5MA

// 일반Edit
#define CLR_EDITBACK                      		1		// 기본 바탕색
#define CLR_EDITLINE                      		6		// 기본테두리 1pixel
#define CLR_EDITHOVERBACK                 		1		// 호버 바탕색
#define CLR_EDITHOVERLINE                 		42		// 호버 테두리 2pixel
#define CLR_EDITPUSHBACK                  		43		// Push 바탕색
#define CLR_EDITPUSHLINE                  		42		// Push 테두리 2pixel
#define CLR_EDITDISBACK                   		44		// DIS 바탕색
#define CLR_EDITDISLINE                   		45		// DIS 테두리 1pixel
#define CLR_EDITDISTEXT                   		46		// DIS 텍스트

// Spin/Code/Combo/Calendar
#define CLR_CTRLBACK                      		1		// 기본 바탕색
#define CLR_CTRLLINE                      		6		// 기본 테두리 1pixel
#define CLR_CTRLDISBACK                   		44		// DIS 바탕색
#define CLR_CTRLDISLINE                   		45		// DIS 테두리 1pixel
#define CLR_CTRLDISTEXT                   		46		// DIS 텍스트

// Scroll bar
#define CLR_SCRLINELT                     		47		// 좌상
#define CLR_SCRLINERB                     		45		// 우하
#define CLR_SCR1LINE                      		48		// 테두리 1라인 en, dn
#define CLR_SCR2LINE                      		1		// 테두리 2라인
#define CLR_SCRBARL                       		49		// 좌
#define CLR_SCRBARR                       		50		// 우
#define CLR_SCRBACKL                      		51		// 좌
#define CLR_SCRBACKR                      		52		// 우

// 메세지
#define CLR_MSGTOP                        		53		// 상
#define CLR_MSGBOTTOM                     		54		// 하
#define CLR_MSGTOPLINE                    		6		// 상단라인
#define CLR_MSGTEXT                       		2		// 텍스트

// Symbol 컨트롤
#define CLR_SYMNORLINE                    		6		// 테두리Line
#define CLR_SYMNORBACK                    		1		// 바탕색
#define CLR_SYMNORTEXT                    		2		// 텍스트
#define CLR_SYMHOVERLINE                  		55		// 테두리Line
#define CLR_SYMHOVERBACK                  		56		// 바탕색
#define CLR_CHR                           		0		// 차트

// 투자자별
#define CLR_INVMAN                        		57		// 개인
#define CLR_INVFRN                        		58		// 외국인
#define CLR_INVORGAN                      		59		// 기관계

// 다이얼로그화면
#define CLR_DLGBACK                       		65		// 그리드Active
#define CLR_DLGPNLBACK                    		66		// 그리드DeActive
#define CLR_DLGPNLLINE                    		6		// 판넬테두리
#define CLR_DLGPNLINLINE                  		62		// 판넬내부라인

// 관심종목
#define CLR_INTGRIDACTIVE                 		65		// 그리드Active
#define CLR_INTGRIDDEACTIVE               		66		// 그리드DeActive

// ICON
#define CLR_ICNKOSPI                      		67		// 거래소
#define CLR_ICNKOSDAQ                     		69		// 코스닥
#define CLR_ICNFREEBOARD                  		71		// 프리보드
#define CLR_ICNELW                        		73		// ELW
#define CLR_ICNWARNING                    		75		// 관리종목
#define CLR_ICNISSUE                      		77		// 투자유의
#define CLR_ICNIFUTURE                    		79		// 지수선물
#define CLR_ICNIOPTION                    		81		// 지수옵션
#define CLR_ICNSFUTURE                    		83		// 주식선물
#define CLR_ICNOFUTURE                    		85		// 상품선물

// 메세지
#define CLR_MSG_GUIDE                     		17		// 가이드메세지
#endif // __AXISCOLOR_H__