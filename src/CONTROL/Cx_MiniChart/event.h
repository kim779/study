#if !defined(CMP_EVENT__H)
#define CMP_EVENT__H

#define	MIF_CODE	"31303"	// Future input code	--> index(1)
#define	MIO_CODE	"41303"	// Option input code
#define	MIQ_CODE	"51303" // kofex input code	--> index(1)
#define	MII_ALLCODE	"23302"	// All Code - char[5*4]
#define	MIF_ALLCODE	"33302"	// All Code
#define	MIO_ALLCODE	"43302"	// All Code
#define	MIQ_ALLCODE	"53302"	// All Code

//////////////////////////////////업종

#define	MII_SYM		"23500"	// 심볼

#define	MDI_JGGA	"23023"	// 종가
#define	MDI_CVOL	"23032"	// 체결량
#define	MDI_TIME	"23034"	// 체결시간

//////////////////////////////////선물

#define	MIF_SYM		"33500"	// 심볼

#define	MDF_JGGA	"33023"	// 종가
#define	MDF_CVOL	"33032"	// 체결량
#define	MDF_TIME	"33034"	// 체결시간
#define	MDF_MYGL	"33201"	// 미결제약정수량


//////////////////////////////////옵션

#define	MIO_SYM		"43500"	// 심볼

#define	MDO_JGGA	"43023"	// 종가
#define	MDO_CVOL	"43032"	// 체결량
#define	MDO_TIME	"43034"	// 체결시간
#define	MDO_MYGL	"43201"	// 미결제약정수량

//////////////////////////////////kofex

#define	MIQ_SYM		"53500"	// 심볼

#define	MDQ_JGGA	"53023"	// 종가
#define	MDQ_CVOL	"53032"	// 체결량
#define	MDQ_TIME	"53034"	// 체결시간
#define	MDQ_MYGL	"53201"	// 미결제약정수량

#endif

/*
1 - 선물 최근월물
2 - 옵션 최근월
3 - 선물 스프레드 최근월물
4 - 옵션 스프레드 최근월물
*/