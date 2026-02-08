#pragma once
typedef struct st_mid_SDEmemo
{
	char chgubn[1]{}; //구분: S-조회 . I-등록, U-수정, D-삭제
	char chusid[16]{}; //user ID
	char chcode[16]{}; //종목코드 
}mid_SDEmemo;

//삭제 존재여부는 부분요청, 부분응답
typedef struct st_mod_SDEmemo
{
	char gubn[1]; //요청한 구분값
	char chcode[16]{};
	char chretc[1]{}; //성공여부: 1성공(조회, 등록,수정,삭제) . 0실패 (조회/등록/수정/삭제 데이터 없음/실패 등)
	char chemsg[64]{}; //결과메시지
}mod_SDEmemo;





typedef struct st_mid_memo
{
	char chgubn[1]{}; //구분: S-조회 . I-등록, U-수정, D-삭제
	char chusid[16]{}; //user ID
	char chcode[16]{}; //종목코드 
	char chmemo[15 * 1024]{}; //종목메모
}mid_memo;

typedef struct st_mod_memo
{
	char gubn[1]; //요청한 구분값
	char chcode[16]{};
	char chretc[1]{}; //성공여부: 1성공(조회, 등록,수정,삭제) . 0실패 (조회/등록/수정/삭제 데이터 없음/실패 등)
	char chemsg[64]{}; //결과메시지
	char chmemo[15 * 1024]{}; //종목메모
	char chMemoDate[12]; //메모업로드 날자
}mod_memo;