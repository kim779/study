#ifndef __OUTPUT_USERDEF__HEADER
#define __OUTPUT_USERDEF__HEADER
// UserDef.h
#define		DFT_NUMERIC		0
#define		DFT_STRING		1
#define		DFT_DATE		2
#define		DFT_TIME		3
#define		DFT_UPDOWN		4
#define		DFT_NUMERIC2	5	//0이면 표시안하는 Numeric
#define		DFT_FUTURE		6	//선물

// Read Data Type
#define		RDT_USER		0




// 스타일 정의
// DFT_NUMERIC 관련 스타일 정의
// 시간 형태 스타일 지정
#define		DFS_TIME01		0	// HH:MM:SS
#define		DFS_TIME02		1	// HH:MM
#define		DFS_TIME03		2	// MM:SS
#define		DFS_TIME04		3	// HHMMSS

// 일자 형태
#define		DFS_DATE01		0	// YY/MM/DD
#define		DFS_DATE02		1	// YYYY/MM/DD
#define		DFS_DATE03		2	// YYYYMMDD
#define		DFS_DATE04		3	// YYYY/MM
#define		DFS_DATE05		4	// YY/MM
#define		DFS_DATE06		5	// MM/DD
#define 	DFS_DATE07		6	// YYMMDD




#endif //__OUTPUT_USERDEF__HEADER