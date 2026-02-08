#ifndef	_LIB_DEFINE_
#define	_LIB_DEFINE_

#define	TM_EVENT	1000		// 실시간 잔고체결 ID
#define	TM_GAP		1		// 실시간 잔고체결 timer

#define	KEY_FREMAIN	1		// 잔고조회
#define	KEY_CONT	2		// 미체결조회
#define	KEY_MINGAM	3		// 미감도조회
#define	KEY_FCURR	4		// 현재가조회


#define RT_JCODE	"907"		// 종목코드
#define RT_JNAME	"908"		// 종목명
#define RT_GUBN		"912"		// 주문구분
#define RT_JPRICE	"910"		// 주문가
#define RT_JAMT		"909"		// 주문수량
#define RT_CPRICE	"916"		// 체결가
#define RT_CAMT		"931"		// 체결수량
#define RT_STATE	"988"		// 상태
#define	RT_MCHG		"921"		// 미체결수량

#define	RT_CCURR	"023"		// 체결현재가
#define	RT_CDIFF	"024"		// 체결대비
#define	RT_CYYDT	"370"		// 체결영업일수
#define	RT_CDDDT	"314"		// 체결달력일수


enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR};

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
enum	{ SM_NOT = -1, SM_ALL = 0, SM_UPDATE, SM_DELETE, SM_INSERT, SM_ERROR  }; //  sendToMap
enum	{ FPOS_NOT = 0, FPOS_BULL, FPOS_BEAR };	// 전체, 강세, 약세

CString _GetString(char *pChar, int nMinLen);
CString _GetFormatData(CString src);
CString	_Parser(CString &srcstr, CString substr);
/*******************************************************************************
 * 선물/옵션 잔고평가 조회 IO FORMAT
 ******************************************************************************/
struct	f_mid	{		// 선물 Input
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	dsgb[15];	/* 구분			*/
	
	char	bqty[10];	/* 수량			*/
	char	cqty[10];	/* 매도가능수량		*/
	char	pprc[10];	/* 평균가/정산가	*/
	char	curr[10];	/* 현재가		*/
	char	diff[10];	/* 평균가 대비		*/
	char	pamt[15];	/* 평가금액		*/
	char	psam[15];	/* 평가손익		*/
	char	prat[15];	/* 수익률		*/
	char	tamt[15];	/* 매입금액		*/
};

struct	f_mod	{
	char	accn[10];
	char	nrec[4];
	struct	f_rec	rec[1];
};

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid


/*******************************************************************************
 * 선물/옵션 미체결 조회 IO FORMAT
 ******************************************************************************/
struct f_cmid {
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	gubn[1];	// 0.All 1.체결 2.미체결
	char	juno[6];
	char	sort[1];	// 1.역순 2.정순
	char	date[8];
	char	cod2[8];
};

struct f_crec {
	char	mono[6];	/* 원주문번호		*/
	char	juno[6];	/* 주문  번호		*/
	char	ojno[6];	/* 원주문 번호		*/
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	odgb[8];	/* 주문구분		*/
	char	hogb[1];	/* 주문유형		*/
	char	jprc[11];	/* 주문가격		*/
	char	jqty[7];	/* 주문수량		*/
	char	dlgb[4];	/* 체결구분		*/
	char	dprc[11];	/* 체결가격		*/
	char	dqty[7];	/* 체결수량		*/
	char	wqty[7];	/* 미체결수량		*/
	char	stat[8];	/* 접수상태		*/
};

struct	f_cmod {
	char	nrec[4];
	struct	f_crec rec[1];
};

#define L_fcmid	sizeof(struct f_cmid)				// Length of Future input
#define L_fcmod	sizeof(struct f_cmod) - sizeof(struct f_crec)	// Length of Future output
#define L_fcrec	sizeof(struct f_crec)				// Length of Future Grid

/*******************************************************************************
 * PIBO3101 : 민감도
 ******************************************************************************/
struct  _mmid
{
	char	crec[3];	// 코드개수
	char	codes[50][8];	// 종목코드(최대 50개) 선물 or 옵션
};
#define	L_MMID	sizeof (struct _mmid)

struct  _mgmod
{
	char	ccod[8];	// 종목코드
	char	ccur[8];	// 현재가
	char	cdif[8];	// 대비
        char    shga[8];	// 상한가
        char    hhga[8];	// 하한가
        char    mdga[5][8];	// 매도호가1~5
        char    msga[5][8];	// 매수호가1~5
	char	cgvol[12];	// 거래량
	char	jjil[2][3];	// 잔존일수 (거래일수/달력일수)
	char	bdjs[5];	// 배당지수
};
#define L_MGMOD	sizeof (struct _mgmod)

struct  _mmod
{
	char	frts[8];	// 최근월물 선물
	char	fnam[20];
	char	fcur[8];
	char	krts[8];	// KOSPI
	char	kcur[8];
	char	k200rts[8];	// KOSPI 200
	char	k200cur[8];
	char	k200tim[8];	// KOSPI 200 Time
	char	hv[4][8];	// hv (90/60/30/21)
	char	cdgl[5];	// CD 금리
	//char	bdjs[5];	// 배당지수(
	char	grec[4];	// 코드개수( count of gmod )
//	struct  _gmod    gmod[50];  // MAX_R = 50
};
#define	L_MMOD	sizeof (struct _mmod)

#endif	// _LIB_DEFINE_