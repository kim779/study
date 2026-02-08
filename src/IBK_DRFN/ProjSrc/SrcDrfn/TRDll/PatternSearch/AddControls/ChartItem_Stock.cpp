// ChartItem.cpp: implementation of the CChartItem_Stock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItem_Stock.h"
#include "ChartDef_Stock.h"
#include "../MainFrmWnd.h"
#include "../../../DR_Include/PropertyMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPCTSTR szSetAllForSignalnew[] = 
{
	"EN=1",
	"HI=0",
	"ST=",
	"GS=",
	"LS=",
	"XCM=신호등",
	"XID=",
	"CFT=굴림체;12;0;0;0;0;",
	"TKS=1",
	"UDF=1",
	"OCN=NULL"
};

LPCTSTR szSetAll_Stock[] = 
{
// 	"CM=Name,SfnChart0",
// 	"AN=SfnChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",
	"BDT=0",
	"SLT=1",							// 점선(graph 선택시)
	"WV=0",
	"PDC=100",
	"BAC=10",			// 최대 블럭수
	"GDD=0",
	"LM=0",	// left 마진
	"RM=0",	// right 마진
	"TM=0",	// top 마진
	"BM=0",	// bottom 마진
	"AGM=1",												// 컨텍스트 메뉴 
	"EFN=",
	"IFN=",
	"SFN=",
	"HI=0",
	"IR=0",
	//"TO=내부\r\n데이터보기;Graph순으로 보기;+/-를 색상으로 표시안함;\r\n수치조회창;팝업 사용안함;수치Dialog 사용;\r\n십자선;수치조회창 사용안함;;\r\n추세선;수치조회창 사용안함;;\r\n모두지우기;;;\r\n스크롤바;;;\r\nDisplay수 설정바;;;\r\n확대,축소;개수;10개;\r\n"
	//"외부\r\n"
	//"십자선;수치조회창 사용안함;;\r\n"
	//"RButton\r\n",
	"CL=BlockColumn;\r\n"
	"Block;\r\n"
	"자료일자;1;1;/;:;0;0;0;1;\r\n"  // 자료일자;4;1 로 하면 x축스케일이 안나온다.
	"VertScaleGroup;\r\n"
	"지표 Data;1;1;0;0;0;\r\n"
	"Graph;\r\n"
	";가격차트;G:가격차트;시가,고가,저가,종가,;0:0;0;0;0;0;1;\r\n"
	"0;0;0;가격;가격;0;208,45,45;208,45,45;45,45,208;45,45,208;208,45,45;208,45,45;1;1;1;1;1;0;\r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	";가격 이동평균;G:가격 이동평균;종가,;0:0;0;0;0;0;0;\r\n"
	"1;0;0;_PMA1_;5;0;202,7,2;202,7,2;202,7,2;202,7,2;202,7,2;202,7,2;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA2_;10;0;70,70,255;70,70,255;70,70,255;70,70,255;70,70,255;70,70,255;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA3_;20;0;222,149,8;222,149,8;222,149,8;222,149,8;222,149,8;222,149,8;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA4_;60;1;24,130,58;24,130,58;24,130,58;24,130,58;24,130,58;24,130,58;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA5_;120;1;149,151,148;149,151,148;149,151,148;149,151,148;149,151,148;149,151,148;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA6_;240;1;255,70,70;255,70,70;255,70,70;255,70,70;255,70,70;255,70,70;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA7_;250;1;156,48,47;156,48,47;156,48,47;156,48,47;156,48,47;156,48,47;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA8_;260;1;64,224,208;64,224,208;64,224,208;64,224,208;64,224,208;64,224,208;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA9_;280;1;145,91,255;145,91,255;145,91,255;145,91,255;145,91,255;145,91,255;14;14;1;1;1;0;0;1;0;\r\n"
	"1;0;0;_PMA10_;290;1;100,85,255;100,85,255;100,85,255;100,85,255;100,85,255;100,85,255;14;14;1;1;1;0;0;1;0;\r\n"

// 	"1;0;0;가격1;5;0;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;1;1;0,0,255;0,0,255;0,0,255;0,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격2;10;1;77,149,249;77,149,249;77,149,249;77,149,249;77,149,249;77,149,249;1;1;77,149,249;77,149,249;77,149,249;77,149,249;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격3;20;0;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;255,0,255;255,0,255;255,0,255;255,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격4;30;1;255,128,128;255,128,128;255,128,128;255,128,128;255,128,128;255,128,128;1;1;255,128,128;255,128,128;255,128,128;255,128,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격5;40;1;232,197,55;232,197,55;232,197,55;232,197,55;232,197,55;232,197,55;1;1;232,197,55;232,197,55;232,197,55;232,197,55;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격6;60;1;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격7;80;1;115,115,0;115,115,0;115,115,0;115,115,0;115,115,0;115,115,0;1;1;115,115,0;115,115,0;115,115,0;115,115,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격8;90;1;155,106,255;155,106,255;155,106,255;155,106,255;155,106,255;155,106,255;1;1;155,106,255;155,106,255;155,106,255;155,106,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격9;100;1;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;1;1;67,154,8;67,154,8;67,154,8;67,154,8;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격10;120;1;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;1;1;255,0,0;255,0,0;255,0,0;255,0,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격11;150;1;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0,0,128;0,0,128;0,0,128;0,0,128;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격12;180;1;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;255,0,255;255,0,255;255,0,255;255,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격13;200;1;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;1;1;0,128,64;0,128,64;0,128,64;0,128,64;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격14;210;1;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;1;1;255,153,0;255,153,0;255,153,0;255,153,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격15;240;1;255,133,11;255,133,11;255,133,11;255,133,11;255,133,11;255,133,11;1;1;255,133,11;255,133,11;255,133,11;255,133,11;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격16;250;1;0,136,0;0,136,0;0,136,0;0,136,0;0,136,0;0,136,0;1;1;0,136,0;0,136,0;0,136,0;0,136,0;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격17;270;1;24,62,188;24,62,188;24,62,188;24,62,188;24,62,188;24,62,188;1;1;24,62,188;24,62,188;24,62,188;24,62,188;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격18;300;1;138,0,255;138,0,255;138,0,255;138,0,255;138,0,255;138,0,255;1;1;138,0,255;138,0,255;138,0,255;138,0,255;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격19;330;1;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;67,154,8;1;1;67,154,8;67,154,8;67,154,8;67,154,8;1;1;0;1;1;1;0;1;1;0;\r\n"
// 	"1;0;0;가격20;350;1;132,132,132;132,132,132;132,132,132;132,132,132;132,132,132;132,132,132;1;1;132,132,132;132,132,132;132,132,132;132,132,132;1;1;0;1;1;1;0;1;1;0;\r\n"
	"GraphEnd;\r\n"
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
// 	"Block;\r\n"
// 	"자료일자;1;2;/;:;0;0;0;0;\r\n"
// 	"VertScaleGroup;\r\n"
// 	"지표 Data;1;2;0;\r\n"
// 	"Graph;\r\n"
// 	";거래량차트;기본거래량,;0:0;0;0;0;0;0;\r\n"
// 	"2;0;0;거래량;거래량;0;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;0,128,64;1;1;1;1;1;0;\r\n"
// 	"GraphEnd;\r\n"
	
	/*
	"Graph;\r\n"
	";거래량 이동평균;기본거래량,;0:0;0;0;0;0;0;\r\n"
	"1;0;0;거래1;5;0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;255,0,0;0;0;1;1;1;0;\r\n"
	"1;0;0;거래2;20;0;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0,0,255;0;0;1;1;1;0;\r\n"
	"1;0;0;거래3;60;0;0,0,102;0,0,102;0,0,102;0,0,102;0,0,102;0,0,102;0;0;1;1;1;0;\r\n"
	"1;0;0;거래4;80;1;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;255,0,255;1;1;1;1;1;0;\r\n"
	"1;0;0;거래5;120;1;153,0,204;153,0,204;153,0,204;153,0,204;153,0,204;153,0,204;1;1;1;1;1;0;\r\n"
	"1;0;0;거래6;160;1;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;255,153,0;1;1;1;1;1;0;\r\n"
	"GraphEnd;\r\n"
	*/
//	"VertScaleGroupEnd;\r\n"
//	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",
	"PO=1;자료일자;;8;YYYYMMDD;유;\r\n"
	"1;시가;;10;× 1;유;\r\n"
	"1;고가;;10;× 1;유;\r\n"
	"1;저가;;10;× 1;유;\r\n"
	"1;종가;;10;× 1;유;\r\n"
	"1;기본거래량;;10;× 1;유;\r\n"
	"1;락구분;;1;× 1;유;\r\n"
	"1;락유무;;2;× 1;무;\r\n"
	"1;락비율;;10;× 0.01;무;\r\n"
	"1;상하한 Sign;;1;× 1;무;\r\n",
	"RL=",
	"URD=1",
	"IRL=자료일자.S31.time.코드;종가.S31.price.코드;기본거래량.S31.volume.코드;\r\n",
	"MBT=3",
	"USESKIN=1",
	"CBKG=244,244,244",
	"CBKB=122,122,122",
	"CB=255,255,255",
	"CBB=122,122,122",
	"CSL=192,192,192",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=82,103,146",
	"BR=row;0;216;\r\n"
	"column;0;387;",
	"CSL2=192,192,192",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=255,255,255",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=6",//"NIO=4",//"NIO=7",//수치조회 기본 (6), 확대기본(4) ??(7)
//	"GSL=시간-그리드4;시간-차트2;"
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
};


LPCTSTR szSetAllNoToolbar[] = 
{
	"CM=Name,Any2Chart0",
	"AN=Any2Chart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
//	"RC=1",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",

	"BDT=0",
//	"SLT=0",							// 실선(graph 선택시)
	"SLT=2",							// 점선(graph 선택시)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source에서 사용
	"PDC=140",
	"BAC=10",			// 최대 블럭수
	"GDD=0",
	"LM=0",	// left 마진
	"RM=0",	// right 마진
	"TM=0",	// top 마진
	"BM=0",	// bottom 마진
	"AGM=1",												// 컨텍스트 메뉴 
	"EFN=",
	"IFN=",
	"SFN=",
	"HI=0",
	"IR=0",
	"TO=내부\r\n이전;;;\r\n스크롤바;;;\r\n확대,축소;개수;10개;\r\nDisplay수 설정바;;;\r\n이후;;;\r\n"
//	"TO=내부\n스크롤바;;;\nGraph형태;;;\n선굵기;;;\n색설정;;;\n수치조회창;팝업 사용;수치Dialog 사용;\n십자선;수치조회창 사용안함;;\n이전;;;\n이후;;;\n확대,축소;개수;10개;\n돋보기;;;\r\n"
	"외부\r\n"
//	"십자선;수치조회창 사용안함;;\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"
//	"자료일자;4;2;/;:;0;0;0;\r\n"
	"자료일자;4;1;/;:;0;0;0;\r\n"
	"VertScaleGroup;\r\n"
	"지표 Data;1;2;0;\r\n"
//	"지표 Data;1;3;0;\r\n"

 
//=========================================================================================
//From_LTH (2003.7.1 ) 
// 아래에서 AddGraph롤 통해 설정하므로 주석처리함
//	"Graph;\r\n"
//	"선형;;실시간 지수;0;0;0;0;0;0;\r\n"
//	"1;0;0;Graph1;실시간지수;0;17,0,0;17,0,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
//	"GraphEnd;\r\n"
//=========================================================================================

/*	"Graph;\r\n",
	"선형;;매도지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;매도지수;0;0,0,255;0,0,255;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"선형;;매수지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;매수지수;0;204,0,0;204,0,0;0,0,255;0,0,255;255,0,255;255,0,255;0;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"선형;;거래소지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;거래소지수;0;0,102,0;0,119,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"*/
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;자료일자;;6;HHMMSS;유;\r\n",
//	"1;실시간 지수;;10;× 0.01;유;\r\n"
//	"1;매도지수;;10;× 0.01;유;\r\n"
//	"1;매수지수;;10;× 0.01;유;\r\n"
//	"1;거래소지수;;10;× 0.01;유;",
//	"PO=1;자료일자;;6;HHMM  ;유;\r\n",		==> Big Bang Original Source에서 사용
//	"1;실시간 지수;;10;× 0.01;유;\r\n",

//	"LSL=선물코드5;",

//	"RL=KP2.time.코드-실시간지수;"
//	"KP2.time.코드-실시간지수;"
//	"KP2.medojisu.코드-실시간지수;"
//	"KP2.mesujisu.코드-실시간지수;"
//	"KP2.upjisu.코드-실시간지수;",
	"RL=",
//	"RL=SC0.time.선물코드;"
//	"SC0.price.선물코드;",

	"URD=1",
	"MBT=3",
//	"CBKG=208,209,213",
	"CBKG=255,255,255",
	"CBKB=188,198,223",
//	"CBKB=0,0,0",
//	"CBKB=255,255,255",
//	"CB=236,236,236",
	"CB=255,255,255",
//	"CBB=231,233,230",
	"CBB=0,0,0",
//	"CSL=255,255,255",
	"CSL=204,204,204",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=82,103,146",


	"BR=row;0;216;\r\n"
	"column;0;387;\r\n",// 2005. 07. 22 비교차트에서 사용하기 위해 수정한다... 본래 모양 "column;0;387;"

	"CSL2=204,204,204",
	"RDB=-1", //2005. 07. 01 블럭의 사이즈의 동일화 
	"UIDWCL=0",
	"CDB=-1", //2005. 07. 01 블럭의 사이즈의 동일화 
	"CIW=255,255,255",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",	
	"UDTT=0",
	"NRBMT=0",//05.11.17 1->0 R버튼 시 메뉴 단순형으로 뛰우도록 
	"BSWMB=1",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=0",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=4",//"NIO=7",
//	"GSL=시간-그리드4;시간-차트2;"
//	"SLHW=시간_차트,1,0,0x00000000;시간_그리드,2,0,0x00000000;", //05. 08. 29 한화에서 쓰는 공유방식.
//	"PAGECOUNTTR=0" //05.11.28 추가
	"PGCNTTR=0"	// JSR 20060221 ADD : From Doori
/*
	"CBKG=255,255,255",
	"CBKB=0,0,0",
	"CB=255,255,255",
	"CBB=0,0,0",
	"CSL=128,128,128",
	"CST=0,0,0",
	"CGB1=0,136,0",
	"CGB2=255,55,255",
	"CGB3=255,51,51",
	"CGB4=0,109,219",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=0,0,0",

	"BR=row;0;216;\r\n"
	"column;0;387;",

	"CSL2=128,128,128",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=203,228,203",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=1",
	"BCBS=1",
	"BBMD=0",
	"BST=0",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=2048",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
*/
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItem_Stock::CChartItem_Stock()
{
	m_bInitalChart = FALSE;
	m_lKey = 0;
}

CChartItem_Stock::~CChartItem_Stock()
{
}

BOOL CChartItem_Stock::Create(CWnd *pWnd,LPRECT lpRect,int nID)
{
	CString strPath;
	GetDir_DrfnData(strPath);
	return CKISChart::Create(strPath, NULL,WS_VISIBLE|WS_CHILD,*lpRect,pWnd,nID);
}

void CChartItem_Stock::SetpBaseDesk(long lKey,HWND hWndDll)
{
	m_lKey = lKey;
	CKISChart::SetBaseDesk(lKey);
	CKISChart::SetContainerDllInfo((long)hWndDll,GetDlgCtrlID());
//	SetAllProperties();

}

BOOL CChartItem_Stock::SetAllProperties(BOOL bNoToolbar /*=FALSE*/)
{
	CPropertyMap mapProp;
	CString strData, strValue;
	int nPos;

	if(bNoToolbar)
	{
		for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
		{
			strData = szSetAllNoToolbar[i];
			nPos = strData.Find(_T('='));
			if(nPos >= 0)
			{
				strValue = strData.Mid(nPos + 1);
				strData = strData.Left(nPos);
				mapProp.SetAt(strData, strValue);
			}
		}
	}
	else
	{
		for(int i=0;i<sizeof(szSetAll_Stock)/sizeof(LPCTSTR);i++)
		{
			strData = szSetAll_Stock[i];
			nPos = strData.Find(_T('='));
			if(nPos >= 0)
			{
				strValue = strData.Mid(nPos + 1);
				strData = strData.Left(nPos);
				mapProp.SetAt(strData, strValue);
			}
		}
		//for(int i=0;i<sizeof(szSetAll_Stock)/sizeof(LPCTSTR);i++)
		//	strlistSetAll.AddTail(szSetAll_Stock[i]);	
	}

	CKISChart::SetAllProperties3((long)(IPropertyMap*)&mapProp);

	return TRUE;
}

CString CChartItem_Stock::MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal)
{
	char szTemp[64];
	CString strPacket;
	strPacket = "1;";
	strPacket += lpPacketName; // 사용할 Packet명
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);
	strPacket += szTemp;
	strPacket += ";";
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += "유;\r\n";
	}
	else
	{
		strPacket += "무;\r\n";
	}
	return strPacket;
}


CString CChartItem_Stock::MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle /*=0*/,
									COLORREF clrGrp1 /*= RGB(218,46,46)*/, COLORREF clrGrp2 /*= RGB(218,46,46)*/,
									COLORREF clrGrp3 /*= RGB(46,46,218)*/, COLORREF clrGrp4 /*= RGB(46,46,218)*/,
									COLORREF clrGrp5 /*= RGB(218,46,46)*/, COLORREF clrGrp6 /*= RGB(218,46,46)*/,
									BOOL bUpFilled /*= 0*/, BOOL bDownFilled /*= 1*/,
									long lUpLineWidth /*= 1*/, long lDownLineWidth /*= 1*/, BOOL bHideGrp /*= 0*/)
{
	CString strChartType = lpChartType;
	CString tempStr;
	CString strGraphOption("Graph;\r\n");
	strGraphOption += lpChartType;
	strGraphOption += ";";
	strGraphOption +=lpChartName;
	strGraphOption += ";";
	LONG lSize = rArrayPacketList.GetSize();
	for(long lIndex=0;lIndex<lSize;lIndex++)
	{
		if(lIndex)	strGraphOption += ",";
		strGraphOption += rArrayPacketList.GetAt(lIndex);		
	}
	if(strChartType==g_strChartType_Stock[0])
	{
		strGraphOption += ";0;0;0;0;1;0";
	}
	strGraphOption += ";\r\n";

	// "그래프타입;그래프스타일;그리는스타일;이름;title;숨김;색1;색2;색3;색4;색5;색6;
	//  상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n"
	
	if(strChartType==g_strChartType_Stock[0])
	{
		strGraphOption += "0;";
	}
	else if(strChartType==g_strChartType_Stock[1])
	{
		strGraphOption += "1;";
	}
	else if(strChartType==g_strChartType_Stock[2])
	{
		strGraphOption += "2;";
	}
	else
	{
		ASSERT(0); // Error !! 
	}
	tempStr.Format("%d;",lChartStyle);
	strGraphOption += tempStr;
	strGraphOption += "0;"; 
	strGraphOption += lpChartName;
	strGraphOption += ";";
	strGraphOption += lpChartTitle;
	tempStr.Format(";0;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d,%d,%d;%d;%d;%d;%d;%d;%d;\r\n",
		GetRValue(clrGrp1),GetGValue(clrGrp1), GetBValue(clrGrp1),
		GetRValue(clrGrp2),GetGValue(clrGrp2), GetBValue(clrGrp2),
		GetRValue(clrGrp3),GetGValue(clrGrp3), GetBValue(clrGrp3),
		GetRValue(clrGrp4),GetGValue(clrGrp4), GetBValue(clrGrp4),
		GetRValue(clrGrp5),GetGValue(clrGrp5), GetBValue(clrGrp5),
		GetRValue(clrGrp6),GetGValue(clrGrp6), GetBValue(clrGrp6),
		bUpFilled,bDownFilled,
		lUpLineWidth,lDownLineWidth,bHideGrp);
	strGraphOption += tempStr;
	
	strGraphOption += "GraphEnd;\r\n";	
	return strGraphOption;
}

CString CChartItem_Stock::GetPacketName()
{
	CString strPacketName;
	long lSize = m_strArrayPacketName.GetSize();
	for(long lPos=0;lPos<lSize;lPos++)
	{
		strPacketName += m_strArrayPacketName.GetAt(lPos);
		strPacketName += ";";
	}
	return strPacketName;
}



long CChartItem_Stock::GetDataSize() 
{ 
	return 0;
}

void CChartItem_Stock::AddRealItem(CString strCode)
{
	CString strRealItem;	
	// 자료일자
	strRealItem = g_strPacketName_Stock[0];
	strRealItem += '.';
	strRealItem += "S31.time.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);
	m_strArrayRealItemRegistered.Add(strRealItem);
	// 종가
	strRealItem = g_strPacketName_Stock[4];
	strRealItem += '.';
	strRealItem += "S31.price.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);
	m_strArrayRealItemRegistered.Add(strRealItem);
	// 거래량
	strRealItem = g_strPacketName_Stock[5];
	strRealItem += '.';
	strRealItem += "S31.volume.";
	strRealItem += strCode;
	strRealItem += ';';
	CKISChart::AddRealItem(strRealItem);		
	m_strArrayRealItemRegistered.Add(strRealItem);

//	// 시가
//	strRealItem = g_strPacketName[1];
//	strRealItem += '.';
//	strRealItem += "S31.open.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);
//	// 고가 
//	strRealItem = g_strPacketName[2];
//	strRealItem += '.';
//	strRealItem += "S31.high.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);
//	// 저가
//	strRealItem = g_strPacketName[3];
//	strRealItem += '.';
//	strRealItem += "S31.low.";
//	strRealItem += strCode;
//	strRealItem += ';';
//	CKISChart::AddRealItem(strRealItem);
//	m_strArrayRealItemRegistered.Add(strRealItem);

}

void CChartItem_Stock::RemoveRealItem()
{
	CString strRealItemRegistered;
	int nSize = m_strArrayRealItemRegistered.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		strRealItemRegistered = m_strArrayRealItemRegistered.GetAt(nPos);
		CKISChart::RemoveRealItem(strRealItemRegistered);
	}
	m_strArrayRealItemRegistered.RemoveAll();
}


// BOOL CChartItem_Stock::AddPacketAndGraph()
// {	
// 	if(!m_bInitalChart)
// 	{
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[0]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[1]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[2]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[3]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[4]);
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[5]);
// 	//	m_strArrayPacketName.Add(g_strPacketName_Stock[6]);
// 	//	m_strArrayPacketName.Add(g_strPacketName_Stock[7]);	
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);				
// 		m_bInitalChart = TRUE;
// 	}
// 	return TRUE;
// }

BOOL CChartItem_Stock::AddPacketAndGraph()
{	
	if(!m_bInitalChart)
	{
		AddPacket(MakePacketOption(g_strPacketName_Stock[0],8,g_strPacketTypeDate_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[0]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[1],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[1]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[2],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[2]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[3],10,g_strPacketTypeNumber_Stock[0],FALSE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[3]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[4],10,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[4]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[5],10,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[5]);
		
		AddPacket(MakePacketOption(g_strPacketName_Stock[6],2,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[6]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[7],10,g_strPacketTypeNumber_Stock[4],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[7]);
		AddPacket(MakePacketOption(g_strPacketName_Stock[8],1,g_strPacketTypeNumber_Stock[0],TRUE));
		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);
// 		AddPacket(MakePacketOption(g_strPacketName_Stock[7],1,g_strPacketTypeEtc_Stock[1],FALSE));
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[7]);	
// 		AddPacket(MakePacketOption(g_strPacketName_Stock[8],6,g_strPacketTypeEtc_Stock[1],FALSE));
// 		m_strArrayPacketName.Add(g_strPacketName_Stock[8]);

		CStringArray arrayPacket;
		arrayPacket.Add(g_strPacketName_Stock[0]);
		arrayPacket.Add(g_strPacketName_Stock[1]);	// 시가
		arrayPacket.Add(g_strPacketName_Stock[2]);	// 고가
		arrayPacket.Add(g_strPacketName_Stock[3]);	// 저가
		arrayPacket.Add(g_strPacketName_Stock[4]);	// 종가
		arrayPacket.Add(g_strPacketName_Stock[5]);
		arrayPacket.Add(g_strPacketName_Stock[6]);
		arrayPacket.Add(g_strPacketName_Stock[7]);
		arrayPacket.Add(g_strPacketName_Stock[8]);

		//		AddGraph(0,0,0,"가격차트",FALSE,FALSE,FALSE);
		ChangeGraphDrawingData("가격차트", FALSE, FALSE, FALSE, FALSE, 1);
		//		AddGraph(0,0,0,"가격 이동평균",FALSE,FALSE,FALSE);
		//		AddGraph(1,0,0,"거래량차트",TRUE,TRUE,TRUE);

		m_bInitalChart = TRUE;
		//		ChangeGraphTitle(0, 0, 0,""); // LYH : 동양차트를 Nts 차트로 바꾸면서, 에러 나서 막는다. 이영희과장님왈, 빠진거는 필요없어서 빠진것임
		SetNUseInquiryWithCrossline(0);
		ResetChartCfgWith(0); //
	}
	else
	{
		/*ResetChart();*/
	}

	return TRUE;
}

void CChartItem_Stock::SetInit(HWND p_lDllWnd, long lKey, LPCTSTR strHtsPath, BOOL bNoToolBar /*=FALSE*/)
{	
	CKISChart::SetHtsPath(strHtsPath);
	CKISChart::SetContainerDllInfo((long)p_lDllWnd, GetDlgCtrlID());
	SetAllProperties(bNoToolBar);	
	
//	LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");
	
}


BOOL CChartItem_Stock::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
			{
				return FALSE;
				break;					
			}
		case VK_TAB:
			if(::GetKeyState(VK_CONTROL) < 0) // 삼성: ctrl-tab 이 안먹는 현상 때문에
			{
				GetParent()->GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				return FALSE;
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);

	return CChartItem_Stock::PreTranslateMessage(pMsg);
}
