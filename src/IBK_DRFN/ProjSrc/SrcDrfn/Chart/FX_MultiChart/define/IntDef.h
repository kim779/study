#ifndef _INT_DEF_HEADER
#define _INT_DEF_HEADER

#define  SH_KSIZE  7

#define REQUEST_ERROR      "0"   // ERROR
#define REQUEST_INTALL     "1"   // 그룹 리스트 가져오기
#define REQUEST_REG_IF     "2"   // 조건 등록
#define REQUEST_REG_ITEMS  "3"   // 찜   종목 등록
#define REQUEST_REG_SIGNAL "4"   // 시그널에서 종목 등록
#define REQUEST_OPENDLG	   "5"   // 단지 다이얼로그만 오픈시킬 경우

typedef struct
{
	char Ino[2];		// 그룹 번호 
	char Ititle[20];	// 그룹 명
	char Icnt[3];		// 그룹 안에 종목 수
	void* Idata;		//종목 데이터들(CPtrArray)
}INT_GROUP_ST;

typedef struct
{
	char Igubun[2]; // 위의 REQUEST 내용
	char Icnt[3]; // 그룹 수
	char IopenDlg[1]; // 1 : Open, 0 : No Open
	void* Idata;  // 그룹 데이터들(CPtrArray) 
	char Itime[6];
	char Idate[8];
}INT_TOTAL;

typedef struct
{
	char Icode[SH_KSIZE];  // 코드  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   시,분,초
	char Idate[8];		   // YYYYMMDD 년,월,일
	char Ivol[10];		   // 수량 	
	char Ival[10];		   // 편입가격
	void* Idata;
} INT_ITEM_ST;

const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

#endif //_INT_DEF_HEADER