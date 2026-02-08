#ifndef CCONDITION_MNGDEF
#define CCONDITION_MNGDEF

/************************************************************/
/*																													*/
/*		ConditionMng Define => STRUCT / DEFINE / TYPEDEF 정의	*/
/*																													*/
/*					2006. 10.  		Doori														  */
/*						작성자 : 우동우																*/
/************************************************************/

//#include "./EU_p0623.h"

#include <afxtempl.h>   //<= CList
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define KEY_USERDIRECTORY _T("USERDIRECTORY")

#define STR_CONVERT(pBuff, cData, nSize, strData)	\
	pBuff = new char[nSize + 1];							\
	memset(pBuff, 0x00, nSize + 1);						\
	memcpy(pBuff, cData, nSize);							\
	strData = (CString)pBuff;									\
	strData.TrimRight();											\
	delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
	pBuff = new char[nSize + 1];						\
	memset(pBuff, 0x00, nSize + 1);					\
	memcpy(pBuff, cData, nSize);						\
	nData = atoi(pBuff);										\
	delete [] pBuff;

#define CODE_LENGTH						6
#define CODENAME_LENGTH					20

#define		MAX_IDXCNT					25	/* 최대 지표 개수(내부)	*/

#define		REAL_IDXCNT					60	/* 최대 지표 개수(외부)	*/
#define		MAX_PARAM					6//6//3->6		/* param 의 최대 개수 	*/
#define		MAX_CONDI					12//6->12		/* condition의 최대 개수*/
#define		EXPLEN						400/*80*/	/* 조건색의 최대 길이 	*/

#define		RESULT_MAX_OFFSET			1
#define		RESULT_MAX					4000	/* 최대 종목 수         */	//3000 -> 4000
//#define		RESULT_MAX					2000	/* 최대 종목 수         */
#define		RESULT_FIELD_MAX			30		/* 결과필드 최대 개수 	*/

#define		LANGUAGE_MAX				1000

/********************************************************************/
/*  조건검색 INPUT DATA                                             */
/********************************************************************/

typedef struct tagTargetChg
{
	char    sJongFillterChk;				 /* 대상지정 체크 여부  '0' : No, '1' : Yes */       
	char    sJongFillterIndex[RESULT_MAX];   /* 대상 종목코드Bitset          */
} TargetChg;
#define SZ_TargetChg sizeof(TargetChg)

typedef struct tagUser_Define
{
	long    IndexNo; 				/* 지표 index				*/
	long    GroupNo; 				/* 소속 그룹번호			*/
	long    GroupIdxCnt;			/* 그룹내 지표 개수			*/
	long    TargetBase;				/* 0:일간, 1:주간, 2:월간, 3:1분, 4:3분	*/
	/* 5:5분,  6:10분, 7:15분, 8:30분, 9:60분*/
	long    Type; 					/* 지표의 종류					*/
	/* 00XX	: 기술적 지표		*/
	/* 10XX	: 캔들분석			*/
	/* 20XX	: 시세지표			*/
	/* 30XX	: 재무지표			*/
	/* 40XX	: 기본적 분석		*/
	/* 50XX	: 시그널 지표 		*/
	/* 60XX	: 전략				*/
	/* 70XX : 패턴 (new)		*/
	/* 80XX : 뉴스 (new)		*/
	long    Type1; 					/* 조건 형태				*/
	/* 0: 기준값 돌파(상향/하향)*/
	/* 1: 기준선 돌파(상향/하향)*/
	/* 2: 기준값 비교(비교) 	*/
	/* 3: 기준선 비교(비교) 	*/
	/* 4: 값 비교(비교,<,<=,>,>=,==)*/
	/* 5: 범위 비교(c1이상~c2이하)  */
	/* 6: 추세(상승/하락)           */
	/* 7: 반전(상승/하락)           */
	/* 8: 순위검색					*/
	/* 9: 배열(정배열/역배열)       */
	/* 10: n% 종목 순위검색1	    */
	/* 11: n% 종목 순위검색2 	    */
	/* 20: 뉴스 체크 (new)          */
	/* 30: 패턴 체크 (new)          */
	/* 90: 기준값 돌파(상향/하향)	##삭제##*/ 
	/* 91: 기준선 돌파(상향/하향)	##삭제##*/
	/* 92: 기준값 비교(비교)    	##삭제##*/
	/* 93: 기준선 비교(비교)    	##삭제##*/
	/* 94: 값비교(비교,<,<=,>,>=,==)##삭제##*/
	/* 95: 범위 비교(c1이상~c2이하) ##삭제##*/
	/* 96: 추세(상승/하락)          ##삭제##*/
	/* 97: 반전(상승/하락)          ##삭제##*/
	/* 99:기타1                     ##삭제##*/
	/* 98:기타2                     ##삭제##*/
	long    Type2; 					/* 상세 형태					*/
	/* 0: 상향/이상/상승/정배열/매수*/
	/* 1: 하향/이하/하락/역배열/매도*/
	/* 2: 전일기준(가격지표)        ##삭제##*/
	/* 3: 당일기준(가격지표)        ##삭제##*/
	/* 99:기타                      */
	long    STerm;					/* n봉전 기준               	*/
	long    ETerm;					/* n봉이내                 		*/
	long    Rank;					/* 순위검색 종목 수             */
	long    Trend;					/* 추세횟수						*/
	long	SignalCnt1;				/* 발생회수	(new)	X번에서 X번 만족*/
	long	SignalCnt2;				/* 발생회수	(new)					*/
	long    ResultViewNum;			/* 결과값보기 필드 갯수(0:사용안함)	*/    

	char    IndexType;				/* 검색구분 0:일반 1:순위 2:순차	*/
	char    SearchType;				/* 검색형태 0:일반 1:사용자정의	2:뉴스 3:패턴   (new) */

	char    Operator;				/* 연산자'0':OR '1':AND '2':NEXT	*/
	char    Reverse;				/* 결과제외 '0':A, '1':Not A		*/
	char    IdxContiGubun;			/* 지표의 연속발생 구분
									'0':단일발생, 1:연속발생지표	*/
	char    Element;				/* 조건식 사용문자(A) (new)			*/
	char	Code[6];

	// @유진삭제	char    filler1[4];
	double  Param[MAX_PARAM];      	/* 지표관련 변수값 (ex.12,26,9)		*/
	double  Condition[MAX_CONDI];   /* 기타 설정 변수값					*/
} User_Define;
#define SZ_User_Define sizeof(User_Define)

/********************************************************************/
/*  사용자 정의 조건검색 패킷                                       */
/********************************************************************/
typedef struct tagUser_Finder
{
	TargetChg	stTargetChg;

	long	TrIndex;
	long	nTargetType;					/* 0 :종목 , 10 : 업종	동부 [NEW]*/		
	long    TargetIdxCnt;                	/* 대상 지표 갯수       */
	long    TargetResultViewCnt;            /* 대상 값보기필드 개수 */
	char    TargetExp[EXPLEN];              /* 대상조건 조합 표현 식*/

	long    ComplexIdxCnt;                  /* 복합 지표 갯수       */
	long    ComplexResultViewCnt;           /* 복합 값보기필드 개수 */
	char    ComplexExp[EXPLEN];             /* 복합조건 조합 표현 식*/

	long    FieldEditerCnt;                 /* 필드편집 개수   [Fix + Move] */
	long    FieldList[REAL_IDXCNT];			/* 필드편집 번호리스트 [Just Fix + Move] */

	long    ResultIdxCnt;					/* 결과값비교 개수          [NEW]*/
	char    ResultExp[EXPLEN];				/* 결과값 비교조합 표현 식  [NEW]*/

	char    TargetBitChk;                   // TargetBit 사용 여부 체크
	char	TargetBit[RESULT_MAX];			/* 검색 대상 종목		*/

	char    Sort_flag;                      /* 정렬 방향(0:오름, 1:내림)*/
	long    Sort_Num;                       /* 정렬 번호(값보기필드pos) */

	long    BackDay;						/* backtest 일자(미사용:-1) */	
	long    BackTime;						/* backtest 시간(미사용:-1) */

	User_Define    stUser_Define[MAX_IDXCNT];    /* 사용자 지표조건          */

// 	int ScriptLength; 
// 	char ScriptData[LANGUAGE_MAX];
	
} User_Finder;
#define SZ_User_Finder  sizeof(User_Finder)

/******************************************************/
/* 하나대투 input값제한으로 인해 데이터를 나눠서 받음 */
/******************************************************/
typedef struct tagLanguage
{
	char	sState[1];				/* 'F' : first data전송 */
									/* 'M' : middle data전송 */
									/* 'L' : last data 전송 */
									/* 'C' : 전송 취소 */
	char	sCnt[4];				/* 수신 카운트 (4) */
	char	ScriptLength[8];		/* 랭귀지스크립트길이 (8)       */
	char	gsScript[LANGUAGE_MAX];	/* 랭귀지스크립트               */
} Language;
#define SZ_Language  sizeof(Language)

/********************************************************************/
/*  Output Packet Struct                                            */
/********************************************************************/
typedef struct tagOutPacket{
	long	lTrIndex;
	long	lTotalJongCnt;				/* 검색된 전제 종목 수      */
	long	lOutFieldCnt;				/* Out Field Count          */
	long	lOutListPacketSize;			/* 결과 List 사이즈(한종목) */
	long	lFindTime;					/* 검색시간					*/
	//char    ResultBit[RESULT_MAX];	/* 결과종목 bit list		*/
} OutPacket;
#define SZ_OutPacket sizeof(OutPacket)

// KB투자증권용	-hidden-
typedef struct tagD1021Out1
{
	char    giCallGubun[4];
	char    giTotalJongCnt[4];
	char    giOutFieldCnt[2];
	char    giOutListPacketSize[4];
	char    giFindTime[8];
} D1021OUT1;
#define  SIZE_D1021OUT1	sizeof(D1021OUT1)

/************************************************************************/
/*  Output List Packet Struct                                           */
/************************************************************************/
typedef struct tagOutListPacket{
	char		chSign;					/* 전일대비 부호  1:상한 2:상승 3:보합 4:하한 5:하락*/
	char		chCode[CODE_LENGTH];	/* 종목 코드	 			*/
	char		chName[CODENAME_LENGTH];/* 종목명		 			*/ 
	char		chMarket;               /* 거래소:'1', 코스닥:'2'	*/
	long		lOrderRank;				/* 순위검색 순위 			*/	
	double		dFieldData[RESULT_FIELD_MAX];
} OutListPacket;
#define SZ_OutListPacket sizeof(OutListPacket)

// KB투자증권용	-hidden-
typedef struct tagD1021Out2
{
	char    gsDaebi[1];
	char    gsCode[6];
	char    gsHname[20];
	char    gsMarket[1];

	char	sUpcodeL[4];			/* 업종 대분류			*/
	char	sUpcodeM[4];			/* 업종 중분류			*/
	char	sUpcodeS[4];			/* 업종 소분류			*/
    char    sUsjg[1];               /* 우선주   (0:해당없음,1:해당) */
    char    sUnder[1];              /* 관리종목                     */
    char    sStop[1];               /* 거래정지종목                 */
    char    sDishonest[1];          /* 불성실공시종목               */
    char    sConfidencelimit[1];    /* 신용제한종목                 */
    char    sJrmm[1];               /* 정리매매종목                 */
    char    sDrat[3];               /* 증거금비율                   */
    char    sMrktalrm[3];           /* 시장경보
                                       1:투자주의, 2:투자경고
                                       3:투자위험, 4:투자위험예고
                                    */	

	char	sInvtalrm[1];			/* 투자주의환기*/
	char	sFiscal[2];				/* 결산월   */
	char	sCapital[16];			/* 자본금   */
	char	sTotPrice[16];			/* 시가총액 */
	char	sVolume[16];			/* 거래량   */
	char	sParValue[16];			/* 액면가   */
	char	sPrice[16];				/* 현재가   */
	char    giOrderRank[4];
	char    gdResultField[60][16];
//	char    gdResultField[20][16];
} D1021OUT2;
#define SIZE_D1021OUT2 sizeof(D1021OUT2)

typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* 우선주   (0:해당없음,1:해당) */
	char    sUnder[1];              /* 관리종목                     */
	char    sStop[1];               /* 거래정지종목                 */
	char    sDishonest[1];          /* 불성실공시종목               */
	char    sConfidencelimit[1];    /* 신용제한종목?               */
	char    sJrmm[1];          	    /* 정리매매종목                 */
	char    sDrat[3];               /* 증거금비율                   */
	char    sMrktalrm[1];           /* 시장경보
										1:투자주의, 2:투자경고
										3:투자위험, 4:투자위험예고*/
	char	sMonth[2];				/* 결산월						*/
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1

/****************************************************************/
/*                                                              */
/*		  Condition Client =>  STRUCT/DEFINE/TYPEDEF 정의					*/
/*																															*/
/*                   2005. 12.  		Doori												*/
/*                        작성자 : 우동우                       */
/****************************************************************/
const		LPCTSTR		CONDITION_INDEX[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I",
"J", "K", "L", "M", "N", "O", "P", "Q", "R",
"S", "T", "U", "V", "W", "X", "Y", "Z",
"a", "b", "c", "d", "e", "f", "g", "h", "i", 
"j", "k", "l", "m", "n", "o", "p", "q", "r",
"s", "t",  "u", "v", "w", "x", "y", "z" };


const		LPCTSTR		TARGETLIST_TYPE1[] = { " 일간", " 주간", " 월간", "  1분", "  2분", "  3분", "  5분", 
" 10분", " 15분", " 20분", " 30분", " 60분", "120분" };
const		int		TARGETINDEX_TYPE1[] = {0, 1, 2, 3, 20, 4, 5, 6, 7, 21, 8, 9, 22};

const		LPCTSTR		TARGETLIST_TYPE2[] = { "일간" };
const		int		TARGETINDEX_TYPE2[] = { 0 };

//--> 0015707: [7127] "당일내발생" 내 조건에 2분, 20분, 120분 주기 추가요청
const		LPCTSTR		TARGETLIST_TYPE3[] = { "  1분", "  2분", "  3분", "  5분", 
" 10분", " 15분", " 20분", " 30분", " 60분", "120분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 20, 4, 5, 6, 7, 21, 8, 9, 22};
//<--
/*
const		LPCTSTR		TARGETLIST_TYPE3[] = { " 1분", " 3분", " 5분", 
"10분", "15분", "30분", "60분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 
6, 7, 8, 9 };

const		LPCTSTR		TARGETLIST_TYPE1[] = { "일간", "주간", "월간", " 1분", " 3분", " 5분", 
"10분", "15분", "60분" };
const		int		TARGETINDEX_TYPE1[] = { 0, 1, 2, 3, 4, 5, 
6, 7, 9 };

const		LPCTSTR		TARGETLIST_TYPE2[] = { "일간" };
const		int		TARGETINDEX_TYPE2[] = { 0 };

const		LPCTSTR		TARGETLIST_TYPE3[] = { " 1분", " 3분", " 5분", 
"10분", "15분", "60분" };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 
6, 7, 9 };
*/

#define MAX_FIELD_CNT				17		//# 결과 Grid Column 구성 Max Count


#define GRID_MOVEFIELD_POS_NAME			"[값보기 위치]"
#define GRID_MOVEFIELD_POS_INDEX		-100


#define XML_VERSION_USERFILE	"3.0" // KHD : LOGIC_VALUE 속성을 추가했기 때문에 버젼을 올림.

#define FOLDER_NEWS						"\\Chart\\Data\\NewsData"
#define NEWS_KEYWORD_FILE			"\\txnews_keyword.dat"

#define FOLDER_TABLE					"\\Table"
#define MEMBERCOMP_FILE				"seccd.dat"

#define FOLDER_FINDDATA				"\\FindData"
#define FOLDER_DATA						"\\icss\\finddata"
#define FOLDER_MAP						FOLDER_DATA + "\\map" 
#define FOLDER_EXP						FOLDER_DATA + "\\exp" 
#define FOLDER_USERFILE				FOLDER_FINDDATA + "\\usr"  //20090615 SJ_KIM Fix the path
#define FOLDER_SYSFILE				FOLDER_DATA + "\\sys"
#define FOLDER_OTHERFILE			"\\Other"
#define FOLDER_LANGUAGEFILE		"ChartData\\Search" 
#define FOLDER_DATAROOT			"\\Chart\\Data"
#define FOLDER_EXCAVATION		FOLDER_DATA + FOLDER_OTHERFILE + "\\ItemExcavation"

#define USRDIR					"\\user\\"
#define FILE_GROUP				"\\portfolio.ini"


#define FOLDER_TEMPFILE					"임시그룹"					

#define TREE_COMMON_FILE				"TreeCommon.xml"
#define TREE_COMMON_SORT_FILE		"TreeCommon_sort.xml"
#define TREE_COMMON_MARK_FILE		"TreeCommon_mark.xml"
#define TREE_ORDER_FILE					"TreeOrder.xml"
#define TREE_ORDER_SORT_FILE		"TreeOrder_sort.xml"
#define TREE_ORDER_MARK_FILE		"TreeOrder_mark.xml"

#define TREE_CAVATIOON_FILE			"TreeExcavation.xml"

#define TREE_USER_FILE					"TreeUser.xml"
#define TREE_RECOMMEND_FILE			"TreeRecommend.xml"
#define TREE_LANGUAGE_FILE			"TreeLanguage.xml"			//# Not User!
#define TREE_TEMP_FILE				"temp.xml"
#define TREE_EXCAVATION_FILE		"treeexcavation.xml"

#define QUICK_FILE						  "QuickBtnReg.xml"


#define MOVEFIELD_SYS_FILE			"movefield_sys.xml"
#define MOVEFIELD_USER_FILE			"movefield_user.xml"
#define FIXFIELD_SYS_FILE			"fixfield_sys.xml"
#define FIXFIELD_USER_FILE			"fixfield_user.xml"
#define FIXFIELD_USER_EXCAVATION	"fixfield_user_excavation.xml"

#define FIXFIELD_SYS_INDUSTRY_FILE	"fixfield_sys_industry.xml"

#define CANDLE_INDEX_FILE			"candleinfo.xml"
#define PATEN_INDEX_FILE			"patterninfo.xml"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define LOCALVERSION_INIFILE		"LocalVersion.ini"

#define CODESEARCH_FILE				"CodeSearch.ini"			//# Port Temp File!
#define USERCONNECTMENU_FILE		"UserConnectMenu.ini"


#define CONDITIONLIST_FOLDER		"OneClk"
#define CONDITIONLIST_FILE			"OneClkList.xml"
#define CONDITIONLIST_INDUSTRYFILE  "OneClkList_Industry.xml"
#define CONDITIONLIST_SUDDENFILE	"oneclklist_sudden.xml"
#define CONDITIONLIST_REAL_FILE		"CondRealList.xml"		

#define FILTTER_FILE				"\\Filter_ConditionSearch_One.dat"
#define FILTTER_FILE_REAL			"\\Filter_ConditionSearch_One_Real.dat"

/************************************************************/
/*																													*/
/* 서비스 프로그램에서 사용하는 XML File Saveed Insert 정의 */
/*																													*/
/*								2004. 11. 02		Doori(우동우)							*/
/************************************************************/
typedef struct tagElement 
{
	CString   strOperation;
	CString   strNot;
	CString   strElement;

	BOOL      bView;

	CString   strBracket;
	LONG      lBracketCount;
	CString   strTag; //20091020 SJ_KIM 관심 or관련 수
} OneElementInfo;

typedef struct tagGroupElement
{
	int nGroup;
	OneElementInfo stOneElementInfo;
} GroupElementInfo;

// 2009.11.10 -hidden- -----------------------------------------------------------
typedef CArray<GroupElementInfo, GroupElementInfo&>		ArrGroupElementInfo;

typedef struct tagGroupElementInfo
{
	int nGroup;
	ArrGroupElementInfo arrGroupElemInfo;
} GROUP_ELEMINFO;

typedef CArray<GROUP_ELEMINFO*, GROUP_ELEMINFO*>		ArrGroupElements;


/*
#define MAX_PORT_GROUP        99
#define MAX_PORT_ITEM         99
typedef struct tagPortGroupInfo
{
int nNo;
CString strName;

int nItemCnt;
int nMarketType[MAX_PORT_ITEM];		 //# 거래소:'1', 코스닥:'2'
CString strCode[MAX_PORT_ITEM];
} PortGroupInfo;
*/


#define HEAD_OTHER_CNT      2  //# 추가 필드 개수
typedef struct tagHeadInfoType
{
	long        lHeadID;
	CString     strHeadName;
	int         nUnitNum;
	CString     strUnitText;
	CString     strDataType;
	int         nDataLength;
	int         nColWidth;
	CString     strAlignment;
} HeadInfoType;

typedef struct tagHeadMoveInfo{
	int         nColCnt;    //## Add Col Count
	int			nIndex;			
	CString		strElement; 
	BOOL		bCondition; 
	BOOL		bView;
	HeadInfoType  stHeadType[HEAD_OTHER_CNT];
} HeadMoveInfo;


typedef struct tagHeadFixInfo{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;


const LPCTSTR		MATCHVAL_OPT_LIST[] = { ">", ">=", "=", "<=", "<" };
const LPCTSTR		MATCHVAL_OPT_DATA = ">\n>=\n=\n<=\n<\n";
typedef struct tagMatchInfo
{
	int			nRow;
	BOOL		bUser;
	int			nOptIndex;
	CString		strField;
	CString		strElement;
	int 		nFieldIndex;

	int			nGridSourceRow;			//# Temp
} MatchInfo;


typedef struct tagExplainInfo
{
	int			nType;		//# 0 : Text, 1 :URL
	CString		strName;
	CString		strExplain;
	CString		strURL;
} ExplainInfo;


//# 조건 하나의 저장 정보 #
typedef struct tagOneCondition {
	BOOL bFalg;					//# 조건 포함 : TRUE

	int	nNotUserFlag;
	int	nGubunUserFlag;			//# 연속/비연속 

	int	nTargetBaseType;		//# TargetBase Type (일간고정, 그외 ...)

	CString strTag;		  //# Tag  <= Map Create Tag
	CString strMapFile;	//# Map File Name

	int	nNonAlert;
	int	nChartValSet;

	CString strName;	//# 지표명
	CString strText;	//# 지표 완성 구문

	User_Define stCondition;

	BOOL bFieldVal;			//# 값보기
	BOOL bFieldState;		//# 값보기 Check Field Have
	BOOL bFieldEnable;	//# 값보기 Enable 상태 
	//=> TRUE : Field values가 없거나 조건이 Not인 경우
	//=> FALSE : Field values가 있는 경우 

	HeadMoveInfo  stHeadMoveInfo;		//# 값보기 필드 정보
} OneConditionInfo;

typedef struct tagConditionGroupInfo{
	int           nGroup;             //## Group
	CString       strElement;         //## Condition Index
	OneConditionInfo  stOneConditionInfo; //## One Condition Info
} ConditionGroupInfo;

// 2009.11.10 -hidden- -----------------------------------------------------------
typedef CArray<ConditionGroupInfo, ConditionGroupInfo&>		ArrCondGroupInfo;

typedef struct tagGroupInfo
{
	int nGroup;
	ArrCondGroupInfo arrConGrpInfo;
} GROUP_INFO;

typedef CArray<GROUP_INFO*, GROUP_INFO*>					ArrGroups;
//---------------------------------------------------------------------------------

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

typedef struct tagFileInfo
{
	CString   strFolderPath;

	CString   strFilePath;
	CString   strFileName;
	CString   strRegCheck;
} FileInfo;

//# [Xml Tree] ###############################################################################
#define TREE_SELECT_TYPE_CLASS        1

#define TREE_SELECT_TYPE_ITEM_COMM    10
#define TREE_SELECT_TYPE_ITEM_ORDER   11
#define TREE_SELECT_TYPE_PORTPOLIO    12
#define TREE_SELECT_TYPE_ACCOUNT      13
#define TREE_SELECT_TYPE_LANGUAGE     14
#define TREE_SELECT_TYPE_USERPATTEN   15
#define TREE_SELECT_TYPE_NEWS         16

#define TREE_SELECT_TYPE_USERFILE     20
#define TREE_SELECT_TYPE_SYSFILE      21

#define TAG_TYPE_PORTPOLIO    "PORT"
#define TAG_TYPE_ACCOUNT      "ACCOUNT"
#define TAG_TYPE_LANGUAGE     "LANGUAGE"
#define TAG_TYPE_USERPATTEN   "USER_PATTEN"
#define TAG_TYPE_NEWS         "NEWS"

#define CONVERT_TAGTOTYPE(strItemTag, nItemType)	      \
	if (strItemTag == (CString)TAG_TYPE_PORTPOLIO)        \
	nItemType = (int)TREE_SELECT_TYPE_PORTPOLIO;        \
  else if (strItemTag == (CString)TAG_TYPE_ACCOUNT)     \
  nItemType = (int)TREE_SELECT_TYPE_ACCOUNT;          \
  else if (strItemTag == (CString)TAG_TYPE_LANGUAGE)    \
  nItemType = (int)TREE_SELECT_TYPE_LANGUAGE;         \
  else if (strItemTag == (CString)TAG_TYPE_USERPATTEN)  \
  nItemType = (int)TREE_SELECT_TYPE_USERPATTEN;       \
  else if (strItemTag == (CString)TAG_TYPE_NEWS)        \
  nItemType = (int)TREE_SELECT_TYPE_NEWS;             \
  else                                                  \
  nItemType = -1;


#define NEWS_SUBROOTTITLE			  	"뉴스분석"
#define PORTPOLIO_SUBROOTTITLE		"관심그룹"
#define LANGUAGE_SUBROOTTITLE			"랭귀지"
#define PATTEN_ROOTTITLE				  "사용자패턴"
#define ACCOUNT_SUBROOTTITLE			"보유종목그룹"

#define CONVERT_TYPETOTITLE(nItemType, strTitle)	        \
	if (nItemType == (int)TREE_SELECT_TYPE_PORTPOLIO)       \
	strTitle = (CString)PORTPOLIO_SUBROOTTITLE;           \
  else if (nItemType == (int)TREE_SELECT_TYPE_ACCOUNT)    \
  strTitle = (CString)ACCOUNT_SUBROOTTITLE;             \
  else if (nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)   \
  strTitle = (CString)LANGUAGE_SUBROOTTITLE;            \
  else if (nItemType == (int)TREE_SELECT_TYPE_USERPATTEN) \
  strTitle = (CString)PATTEN_ROOTTITLE;                 \
  else if (nItemType == (int)TREE_SELECT_TYPE_NEWS)       \
  strTitle = (CString)NEWS_SUBROOTTITLE;                \
  else                                                    \
  strTitle = ""

#define MAP_OTHER_FILE        "Other"

typedef struct tagNewsInfo
{
	int nIndex;
	int nCode;
	CString strType;
	CString strKeyWord;
} NewsInfo;

typedef struct tagXMLTreeInfo
{
	int nItemType;

	CString strName;
	CString strTag;

	int nNonAlert;
	int nCondiType;
	int nTargetBase_Type;

	int nGubun;
	int nGubunDefVal;

	int nNotUser;
	int nNotDefVal;

	CString strResultViewID;
	int nResultViewDefVal;

	int nChartValSet;
	CString strMapFile;
} XMLTreeInfo;

// 201001 박찬모 
// vs60 -> vs2008
//inline COPY_XMLTreeInfo(XMLTreeInfo* xpSrc, XMLTreeInfo* xpTar)
inline void COPY_XMLTreeInfo(XMLTreeInfo* xpSrc, XMLTreeInfo* xpTar)
{
	xpTar->nItemType = xpSrc->nItemType;
	xpTar->strName = xpSrc->strName;
	xpTar->strTag = xpSrc->strTag;
	xpTar->nNonAlert = xpSrc->nNonAlert;
	xpTar->nCondiType = xpSrc->nCondiType;
	xpTar->nTargetBase_Type = xpSrc->nTargetBase_Type;
	xpTar->nGubun = xpSrc->nGubun;
	xpTar->nGubunDefVal = xpSrc->nGubunDefVal;
	xpTar->nNotUser = xpSrc->nNotUser;
	xpTar->nNotDefVal 	 = xpSrc->nNotDefVal;		
	xpTar->strResultViewID = xpSrc->strResultViewID;
	xpTar->nResultViewDefVal = xpSrc->nResultViewDefVal;
	xpTar->nChartValSet = xpSrc->nChartValSet;
	xpTar->strMapFile = xpSrc->strMapFile;
}


typedef struct tagXMLTreeInfoFile
{
	CString strType;		//# USER, SYS

	CString strFolderPath;
	CString strFilePath;
	CString strFileName;

	BOOL	bQuickReg;		//# Deawoo [NEW]
	BOOL	bUnitCan;		//# Deawoo [NEW]

	CString strAlertReg;

	CString strExplain;
} XMLTreeInfoFile;

typedef struct tagXMLTreeInfoFolder
{
	BOOL	bLock;
	CString strPath;
	CString strName;

	int nFileCnt;

	CList	<XMLTreeInfoFile, XMLTreeInfoFile&> *plistXMLTreeInfoFile;
} XMLTreeInfoFolder;


typedef struct tagInfoFolder
{
	BOOL		bLock;
	CString strPath;
	CString strName;
	int nFileCnt;
	BOOL bTarget;
} InfoFolder;


//# Language!
typedef struct tagLanguageInfo
{
	int	nType;	// 사용된 언어 0 : TS, 1 : YES, 2 : YES
	LPSTR	lpszName;		  // 파일명은 확장자 .sts만 붙이면 됨
	// 스크립트 경로는 루트경로/ScriptFiles
} LanguageInfo;


//# Lib Group Data!
typedef struct tagConditionItemInfo
{
	CString strGroupName;

	CStringArray strItemName;
	CStringArray strItemData;   //# Path, Data(Tag, Val...)
} ConditionItemInfo;


//# Filter Info ########################################################
typedef struct tagSubFilterInfo
{
	int nTime;
	int nData;
} SubFilterInfo;


#define CNT_CHANGEVAL		5
#define CNT_DIRECTION		8

typedef struct tagFilterInfo
{
	int nMarketType;			//# 0 : 전체, 1 : 코스피, 2 : 코스닥

	int nNowPriceChk;
	int nNowPriceUp, nNowPriceDown;

	int nChangRateChk;
	int nChangRateUp, nChangRateDown;

	int nChangValChk[CNT_CHANGEVAL];

	int nDealVolType;		//# 0 : 고정, 1 : Time
	int nDealVol;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimeVol;

	int nDealPriceType;		//# 0 : 고정, 1 : Time
	int nDealPrice;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimePrice;

	int nDirectionChk[CNT_DIRECTION];

	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)


// ------------------------------------------------------------------------------------
//@유진에서 추가
// ------------------------------------------------------------------------------------
// #include "../../inc/UseVector.h"
// #define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())
// class C0621OutBlock1
// {
// public:
// 	int  GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][1];	// 검색된 전제 종목 수
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		// 조회된 종목이 없을 경우 (-900)이 리턴된다고 함.
// 		return (nRet<0)? 0: nRet;
// 	}
// 
// 	void SetData(CHARVECTOR2 &vvRpValueList, OutPacket* pOutData)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];
// 		pOutData->lTrIndex	= atol(szTmp);			//콜구분
// 
// 		szTmp	 = vvRpValueList[1][1];
// 		pOutData->lTotalJongCnt	= atol(szTmp);		//검색된 전제 종목 수
// 
// 		szTmp	 = vvRpValueList[1][2];
// 		pOutData->lOutFieldCnt	= atol(szTmp);		//Out Field Count
// 
// 		szTmp	 = vvRpValueList[1][3];
// 		pOutData->lOutListPacketSize = atol(szTmp);	// 결과 List 사이즈(한종목)
// 
// 		szTmp	 = vvRpValueList[1][4];
// 		pOutData->lFindTime	= atol(szTmp);			// 검색시간
// 	};
// };
// 
// class C0621OutBlock2
// {
// public:
// 	void SetData(CHARVECTOR2 &vvRpValueList, OutListPacket* pListData, int nCnt)
// 	{
// 		if(nCnt<=0) return;
// 
// 		CString schSign,
// 			schCode,
// 			schName,
// 			schMarket,
// 			slOrderRank,
// 			sdFieldData;
// 		CString szTmp;
// 		char aTmp[16+1]={0,};
// 		for(int i=0; i<nCnt; i++)
// 		{
// 			schSign		= vvRpValueList[i+1][ 0];
// 			schCode		= vvRpValueList[i+1][ 1];
// 			schName		= vvRpValueList[i+1][ 2];
// 			schMarket	= vvRpValueList[i+1][ 3];
// 			slOrderRank	= vvRpValueList[i+1][ 4];
// 			sdFieldData = vvRpValueList[i+1][ 5];
// 
// 			pListData[i].chSign = schSign[0];
// 			Safe_CheckCopy(pListData[i].chCode, schCode);
// 			Safe_CheckCopy(pListData[i].chName, schName);
// 			pListData[i].chMarket = schMarket[0];
// 			pListData[i].lOrderRank = atol(slOrderRank);
// 
// 			for(int nFi=0; nFi<RESULT_FIELD_MAX; nFi++)
// 			{
// 
// 				// 201001 박찬모 
// 				// vs60 -> vs2008
// 				//sprintf(aTmp, "%16s", sdFieldData.Mid(nFi*16, 16));
// 				sprintf_s(aTmp, "%16s", sdFieldData.Mid(nFi*16, 16));
// 				aTmp[16] = 0;
// 				pListData[i].dFieldData[nFi] = atof(aTmp);
// 			}
// 		}
// 	}
// };
// 
// class C0626OutBlock1
// {
// public:
// 	int  GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];	// 요청갯수
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		return (nRet<0)? 0: nRet;	// CConditionResultWnd::SetSearch_WorkDate()에서 42개 요청.
// 	}
// };
// 
// class C0626OutBlock2
// {
// public:
// 	void SetData(CHARVECTOR2 &vvRpValueList, char* pOutData, int nDataCnt)
// 	{
// 		CString szTmp;	// 영업일
// 
// 		for(int i=0; i<nDataCnt; i++)
// 		{
// 			szTmp	 = vvRpValueList[i+1][0];
// 			memcpy(&pOutData[i*8], (LPCSTR)szTmp, 8);
// 		}
// 	};
// };
// 
// class C0627OutBlock1
// {
// public:
// 	int  GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];	// 검색된 전제 종목 수
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		return (nRet<0)? 0: nRet;
// 	}
// };
// 
// class C0627OutBlock2
// {
// public:
// 	void SetData(CHARVECTOR2 &vvRpValueList, char* pDataBuf, int nDataCnt)
// 	{
// 		CString sMarket,		//마켓구분
// 			sCode,		//종목코드
// 			sName,		//종목명
// 			sSetPos;	//인덱스
// 		int nPos = 0;
// 
// 		for(int i=0; i<nDataCnt; i++)
// 		{
// 			MSTCode* pOutData = (MSTCode*)&pDataBuf[nPos];
// 			nPos += sizeof(MSTCode);
// 
// 			ZeroMemory(pOutData, sizeof(MSTCode));
// 
// 			sMarket	 = vvRpValueList[i+1][0];
// 			pOutData->sMarket[0] = sMarket[0];
// 
// 			sCode = vvRpValueList[i+1][1];
// 			Safe_CheckCopy(pOutData->sCode ,sCode);
// 			pOutData->sCode[6] = 0;
// 
// 			sName = vvRpValueList[i+1][2];
// 			sName.TrimRight();
// 			//Safe_CheckCopy(pOutData->sName ,sName);
// 			memcpy(pOutData->sName, (LPCSTR)sName, 20);
// 			pOutData->sName[sName.GetLength()]=0;
// 
// 			sSetPos = vvRpValueList[i+1][3];
// 			Safe_CheckCopy(pOutData->sSetPos ,sSetPos);
// 			//memcpy(pOutData->sSetPos, (LPCSTR)sSetPos, 4);
// 		}
// 	};
// };


typedef struct stRealData
{
	BYTE	chCount;
	char	szKeyCode[4];
	int		nSize;
	LPSTR	pDataBuf;
	int		lRealStruct;
	BYTE	chRealType;
} ST_REALDATA;

#define usrMasterCondition	"SearchTemp"
#define gszCacheDir			"DrCache"
#define gszDataKey_MstCode	"MstCode"
#define gszDataKey_WorkDate "WorkDate"

#define FILE_DATAVERSION		"SvrVersionDataFile.cfg"
#define FILE_DATAVERSION_LOCAL	"LocalVersionDataFile.cfg"
#define FILE_FINDDATA			"finddata"
#define FILE_LANGGUAGEDATA		"data"
#define FILE_EXP				"exp"
#define FILE_MAP				"map"
#define FILE_OTHER				"other"
#define FILE_PATTERNDATA		"patterndata"
#define FILE_SYS				"sys"
#define FILE_EXTENSION			".zip"
#define	FILE_CONDTARGET_EXT		".tdf"

#endif // CCONDITION_MNGDEF
