#ifndef __COLOR_TABLE_H__
#define __COLOR_TABLE_H__


// Basis Color
#define		COLOR_BLACK				RGB(  0,  0,  0)		// 검정색
#define		COLOR_WHITE				RGB(255,255,255)		// 하양색
#define		COLOR_RED				RGB(255,  0,  0)		// 빨강색
#define		COLOR_BLUE				RGB(  0,  0,255)		// 파랑색


// Color for Grid
#define		COLOR_BACK				RGB(217,217,217)		// 배경색
#define		COLOR_GRIDLINE			RGB(153,172,191)		// 그리드 라인
#define		COLOR_GRIDHEADER		RGB(211,223,241)		// 그리드 헤더
#define		COLOR_GRIDROW			RGB(224,237,255)		// 그리드 열

#define		COLOR_SELECTED_CELL		RGB(224,237,224)		// 선택된 셀
#define		COLOR_CELL_OUTLINE		RGB(249,127,156)		// 선택된 셀 테두리
#define		COLOR_STCELL_OUTLINE	RGB(0,128,128)			// 스태틱 셀 테두리


// Color for Dialog
#define		COLOR_BG_ANLYS_DLG		RGB(221,224,233)



//***********************************************************************
//	Color for Pattern
//***********************************************************************
// Line Color 
#define		COLOR_HIGHLOW_PTLINE			RGB(128,  0,  0)		// 주요고점 저점 연결라인 색상
#define		COLOR_MALINE					RGB(255,  0,  0)		// 이동평균 색상
#define		COLOR_HIGH_HORZLINE				RGB(0,    0,255)		// 주요고점 수평라인 색상
#define		COLOR_LOW_HORZLINE				RGB(255,  0,  0)		// 주요저점 수평라인 색상

#define		COLOR_PATTERNWAVE				RGB(255,  0,  0)		// Pattern Wave
#define		COLOR_UPWARD_REVERSE			RGB(255,104, 32)		// Pattern Upward Reverse
#define		COLOR_DNWARD_REVERSE			RGB( 50,205, 50)		// Pattern Downward Reverse
#define		COLOR_UPWARD_CONTINUE			RGB(255,  0,  0) 		// Pattern Upward Continue
#define		COLOR_DNWARD_CONTINUE			RGB(  0,  0,255) 		// Pattern Downward Continue


// Pattern Color
#define		COLOR_BG_UPWARD_REVERSE			RGB(250,170,203)
#define		COLOR_BG_DNWARD_REVERSE			RGB(202,227,253)
#define		COLOR_BG_UPWARD_CONTINUE		RGB(250,170,203)
#define		COLOR_BG_DNWARD_CONTINUE		RGB(202,227,253)



#endif	//__COLOR_TABLE_H__