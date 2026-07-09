#define WM_DLL_SCREEN	WM_USER+101		// 받기
#define WM_DLL_CONTROL	WM_USER+102		// 보내기

#define evChangeCode	1			// 받기 전용
#define scSetCode	2			// 보내기 
#define scGetCode	3			// 보내기
#define enChageAcno	4			// 계좌 받기
#define enSetAcno	5			// 계좌 설정
#define enGetAcno	6			// 계좌 return
#define scClearCode	7
#define scHistoryClear	8
#define scSetAllCode	9	 
#define scSetBgColor	10	 //배경색 설정