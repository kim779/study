#ifndef CCONDITION_MNGDEF
#define CCONDITION_MNGDEF

#include <afxtempl.h>   //<= CList
#define _countof(array) (sizeof(array)/sizeof(array[0]))

/************************************************************/
/*																													*/
/*			Condition Server =>  STRUCT/DEFINE/TYPEDEF 정의			*/
/*																													*/
/*					2004. 11. 2 		Doori														*/
/*						작성자 : 우동우																*/
/************************************************************/
#define ERR_EXPCODE					"E00010"

#define TRNO_CONDITION			"80001"
#define TRNO_WORKDATE				"80003"

#define CODE_LENGTH					7
#define CODENAME_LENGTH			20


#define		MAX_IDXCNT				18	/* 최대 지표 개수(내부)	*/

#define		REAL_IDXCNT				18	/* 최대 지표 개수(외부)	*/
#define		MAX_PARAM					3		/* param 의 최대 개수 	*/
#define		MAX_CONDI					6		/* condition의 최대 개수*/
#define		EXPLEN						80	/* 조건색의 최대 길이 	*/

#define		RESULT_MAX_OFFSET	1
#define		RESULT_MAX				2000	/* 최대 종목 수         */
#define		RESULT_FIELD_MAX	20		/* 결과필드 최대 개수 	*/

#define		LANGUAGE_MAX			180000


/********************************************************************/
/*  조건검색 INPUT DATA                                             */
/********************************************************************/
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
	long		SignalCnt1;				/* 발생회수	(new)	X번에서 X번 만족*/
	long		SignalCnt2;				/* 발생회수	(new)					*/
	long    ResultViewNum;			/* 결과값보기 필드 갯수(0:사용안함)	*/    

	char    IndexType;				/* 검색구분 0:일반 1:순위 2:순차	*/
	char    SearchType;				/* 검색형태 0:일반 1:사용자정의	2:뉴스 3:패턴   (new) */

	char    Operator;				/* 연산자'0':OR '1':AND '2':NEXT	*/
	char    Reverse;				/* 결과제외 '0':A, '1':Not A		*/
	char    IdxContiGubun;			/* 지표의 연속발생 구분
										'0':단일발생, 1:연속발생지표	*/
	char    Element;				/* 조건식 사용문자(A) (new)			*/
	char	Code[6];

	double  Param[MAX_PARAM];      	/* 지표관련 변수값 (ex.12,26,9)		*/
	double  Condition[MAX_CONDI];   /* 기타 설정 변수값					*/
} User_Define;
#define SZ_User_Define sizeof(User_Define)

/********************************************************************/
/*  사용자 정의 조건검색 패킷                                       */
/********************************************************************/
typedef struct tagUser_Finder
{
	long		TrIndex;
	long		TargetType;									  /* 0 :종목 , 10 : 업종	동부 [NEW]*/		
	long    TargetIdxCnt;                	/* 대상 지표 갯수       */
	long    TargetResultViewCnt;          /* 대상 값보기필드 개수 */
	char    TargetExp[EXPLEN];            /* 대상조건 조합 표현 식*/
	
	long    ComplexIdxCnt;                /* 복합 지표 갯수       */
	long    ComplexResultViewCnt;         /* 복합 값보기필드 개수 */
	char    ComplexExp[EXPLEN];           /* 복합조건 조합 표현 식*/
	
	long    FieldEditerCnt;               /* 필드편집 개수   [Fix + Move] */
	long    FieldList[REAL_IDXCNT];				/* 필드편집 번호리스트 [Just Fix + Move] */

	long    ResultIdxCnt;									/* 결과값비교 개수         [NEW]*/
	char    ResultExp[EXPLEN];						/* 결과값 비교조합 표현 식 [NEW]*/
		
	char    TargetBitChk;                 // TargetBit 사용 여부 체크
	//char		Jongmoklist[RESULT_MAX][CODE_LENGTH];			// 관심그룹 종목코드
	char		TargetBit[RESULT_MAX];  /* 검색 대상 종목		*/
	
	char    Sort_flag;                    /* 정렬 방향(0:오름, 1:내림)*/
	long    Sort_Num;                     /* 정렬 번호(값보기필드pos) */
	    
	long    BackDay;											/* backtest 일자(미사용:-1) */	
	long    BackTime;											/* backtest 시간(미사용:-1) */

	User_Define    stUser_Define[MAX_IDXCNT];    /* 사용자 지표조건   */
	
	/*
	int ScriptLength;
	char ScriptData[LANGUAGE_MAX];
	*/
} User_Finder;
#define SZ_User_Finder  sizeof(User_Finder)

/********************************************************************/
/*  Output Packet Struct                                            */
/********************************************************************/
typedef struct tagOutPacket{
	long	lTrIndex;
	long  lTotalJongCnt;				/* 검색된 전제 종목 수      */
	long  lOutFieldCnt;					/* Out Field Count          */
	long  lOutListPacketSize;		/* 결과 List 사이즈(한종목) */
	long	lFindTime;						/* 검색시간					*/
	//char    ResultBit[RESULT_MAX];	    /* 결과종목 bit list*/
} OutPacket;
#define SZ_OutPacket sizeof(OutPacket)

/************************************************************************/
/*  Output List Packet Struct                                           */
/************************************************************************/
typedef struct tagOutListPacket{
	char		chSign;									/* 전일대비 부호  1:상한 2:상승 3:보합 4:하한 5:하락*/
	char		chCode[CODE_LENGTH];		/* 종목 코드	 			*/
	char		chName[CODENAME_LENGTH];/* 종목명		 			*/ 
	char		chMarket;               /* 거래소:'1', 코스닥:'2'	*/
	char		Filler[3];
	long		lOrderRank;							/* 순위검색 순위 			*/	
	double	dFieldData[RESULT_FIELD_MAX];
} OutListPacket;
#define SZ_OutListPacket sizeof(OutListPacket)


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
												"s", "t",  "u", "w", "x", "y", "z" };


const		int		TARGETINDEX_TYPE1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const		int		TARGETINDEX_TYPE2[] = { 0 };
const		int		TARGETINDEX_TYPE3[] = { 3, 4, 5, 6, 7, 8, 9 };

#define MAX_FIELD_CNT				17		//# 결과 Grid Column 구성 Max Count


#define GRID_MOVEFIELD_POS_NAME			"[값보기 위치]"
#define GRID_MOVEFIELD_POS_INDEX		-100

#define MAX_PORTPOLIO_CNT				200
#define XML_VERSION_USERFILE		"2.0"

#define FOLDER_NEWS						"\\Data\\NewsData"
#define NEWS_KEYWORD_FILE			"\\txnews_keyword.dat"

#define FOLDER_DATA						"\\Data\\FindData"
#define FOLDER_MAP						FOLDER_DATA + "\\map" 
#define FOLDER_EXP						FOLDER_DATA + "\\exp" 
#define FOLDER_USERFILE				FOLDER_DATA + "\\user" 
#define FOLDER_SYSFILE				FOLDER_DATA + "\\sys"
#define FOLDER_OTHERFILE			FOLDER_DATA + "\\Other"
#define FOLDER_LANGUAGEFILE		"ChartData\\Search" 

#define FOLDER_TEMPFILE					"임시그룹"					

#define TREE_COMMON_FILE				"TreeCommon.xml"
#define TREE_COMMON_SORT_FILE		"TreeCommon_sort.xml"
#define TREE_COMMON_MARK_FILE		"TreeCommon_mark.xml"
#define TREE_ORDER_FILE					"TreeOrder.xml"
#define TREE_ORDER_SORT_FILE		"TreeOrder_sort.xml"
#define TREE_ORDER_MARK_FILE		"TreeOrder_mark.xml"
#define TREE_USER_FILE					"TreeUser.xml"
#define TREE_RECOMMEND_FILE			"TreeRecommend.xml"
#define TREE_LANGUAGE_FILE			"TreeLanguage.xml"			//# Not User!

#define QUICK_FILE						  "QuickBtnReg.xml"


#define MOVEFIELD_SYS_FILE			"movefield_sys.xml"
#define MOVEFIELD_USER_FILE			"movefield_user.xml"
#define FIXFIELD_SYS_FILE				"fixfield_sys.xml"
#define FIXFIELD_USER_FILE			"fixfield_user.xml"

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define LOCALVERSION_INIFILE		"LocalVersion.ini"

#define CODESEARCH_FILE					"CodeSearch.ini"			//# Port Temp File!
#define USERCONNECTMENU_FILE		"UserConnectMenu.ini"


/************************************************************/
/*																													*/
/* 서비스 프로그램에서 사용하는 XML File Saveed Insert 정의 */
/*																													*/
/*								2004. 11. 02		Doori(우동우)							*/
/************************************************************/


typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

//### RealFinder Pro 2002. 11 : Danny ###
typedef struct tagElement 
{
	CString   strOperation;
	CString   strNot;
	CString   strElement;
	
	BOOL      bView;
	
	CString   strBracket;
	LONG      lBracketCount;
} OneElementInfo;

typedef struct tagGroupElement
{
	int nGroup;
	OneElementInfo stOneElementInfo;
} GroupElementInfo;


typedef struct tagPortPolio
{
	CString		strOperation;
	int			nType;				//# 0 : PortPolio, 1 : Account
	CString		strPortNumber;
	CString		strPortName;
} PortPolio;

//# Portfolio Save Packet
typedef struct tagPortfolioGroup
{
	CString strCode;			//ex)[관심 05]
	CString strGroupName;	//ex)memo=
	int nElementCnt;			//관심종목그룹 내의 지표 갯수
} PortfolioGroup;
#define SZ_PortfolioGroup sizeof(PortfolioGroup)



//# [Port]
#define	PORT_GROUP_MAX								99
#define	PORT_GROUP_ITEM_MAX						40

typedef struct tagPortInfo
{
	int nNo;
	CString strName;

	int nItemCnt;
	int nMarketType[PORT_GROUP_ITEM_MAX];		 //# 거래소:'1', 코스닥:'2'
	CString strCode[PORT_GROUP_ITEM_MAX];
} PortInfo;


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
	BOOL        bFalg;					//# 조건 포함 : TRUE
	
	int				nNotUserFlag;
	int				nGubunUserFlag;			//# 연속/비연속 

	int			nTargetBaseType;		//# TargetBase Type (일간고정, 그외 ...)

	CString     strConditionTag;		//# 조건 Tag  <= Map Create Tag
	CString     strConditionFileName;	//# 조건 File Name

	int			nNonAlert;				//# Deawoo [NEW]
	int			nChartValSet;			//# Deawoo [NEW]

	CString     strConditionTitle;		//# 조건명
	CString     strConditionText;		//# 조건 완성 구문
	
	User_Define   stCondition;			//# condition Info  

//#	User_Script		stUserScript;
//#	CString				strScriptText;

	BOOL          bFieldVal;			//# 값보기
	BOOL          bFieldState;			//# 값보기 Check Field Have
	BOOL          bFieldEnable;			//# 값보기 Enable 상태 
										//=> TRUE : Field values가 없거나 조건이 Not인 경우
										//=> FALSE : Field values가 있는 경우 
	
	HeadMoveInfo  stHeadMoveInfo;		//# 값보기 필드 정보
} OneConditionInfo;

typedef struct tagConditionGroupInfo{
	int           nGroup;             //## Group
	CString       strElement;         //## Condition Index
	OneConditionInfo  stOneConditionInfo; //## One Condition Info
} ConditionGroupInfo;

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


typedef struct tagQuickList
{
	BOOL		bCheck;
	CString	strType;
	BOOL		bAlertReg;

	CString strFileName;
	CString	strQuickName;
	CString	strFilePath;
	CString	strFolderPath;
} QuickList;
#define SZ_QuickList sizeof(QuickList)


//# [Xml Tree] ###############################################################################
#define TREE_SELECT_TYPE_ITEM			1
#define TREE_SELECT_TYPE_PARENTITEM		2

#define TREE_SELECT_TYPE_USERFILE		3
#define TREE_SELECT_TYPE_SYSFILE		4

#define TREE_SELECT_TYPE_PORTPOLIO		5
#define TREE_SELECT_TYPE_ACCOUNT		6
#define TREE_SELECT_TYPE_LANGUAGE		7
#define TREE_SELECT_TYPE_USERPATTEN		8


#define TAG_USERPATTEN				"P1_1"

typedef struct tagNewsInfo
{
	int nIndex;
	int nCode;
	CString strType;
	CString strstrKeyWord;
} XMLTreeNewsInfo;

typedef struct tagXMLTreeInfo
{
	int nTreeType;			//# Item : Common[0] / Order[1] / Language[4] / User Patten[5]

	CString strName;
	CString strTag;

	int nNonAlert;			//# Deawoo [NEW]
	int nCondiType;
	int nTargetBase_Type;

	int nGubun;
	int nGubunDefVal;

	int nNotUser;
	int nNotDefVal;
		
	CString strResultViewID;
	int nResultViewDefVal;

	int nChartValSet;		//# Deawoo [NEW]

	CString strFileName;
} XMLTreeInfo;


typedef struct tagXMLTreeInfoFile
{
	CString strType;		//# USER, SYS

	CString strFolderPath;
	CString strFilePath;
	CString strFileName;

	BOOL	bQuickReg;		//# Deawoo [NEW]
	BOOL	bUnitCan;		//# Deawoo [NEW]

	BOOL	bAlertReg;

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

//# Danny => 2005. 12 [동부]
typedef struct tagCodeMaster
{
	int		nType;		// 0:주식종목, 1:코스닥종목
	CString strUnder;		// 1  관리/감리구분
	CString strWarn;		// 1  투자유의 구분
	CString strName;		// 18 종목명
	CString strCode;		// 6  종목코드
} CodeMaster;


//# Language!
typedef struct tagLanguageInfo
{
	int	nLanguageType;	// 사용된 언어 0 : TS, 1 : YES, 2 : YES
	LPSTR	lpszName;		  // 파일명은 확장자 .sts만 붙이면 됨
					            // 스크립트 경로는 루트경로/ScriptFiles
} LanguageInfo;

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




#endif // CCONDITION_MNGDEF
