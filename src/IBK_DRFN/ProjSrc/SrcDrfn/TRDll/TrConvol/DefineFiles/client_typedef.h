#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define  GRID_ROW_HEIGHT 18
#define  DEF_FRAMEHEIGHT 523

#define _countof(array) (sizeof(array)/sizeof(array[0]))

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



const		LPCTSTR		DGN_HEADER_NAME[] = { _T("시각"), _T("코드"), _T("종목명"), _T("현재가"), _T("전일대비"),
									 _T("등락률"), _T("체결량"), _T("누적거래량"), _T("거래량비"), _T("체결량비"), _T("체결강도"), _T("카운트") };

const		UINT		DGN_HEADER_SIZE[] = { 60, 55, 80, 60, 65, 55, 60, 80, 61, 59, 61, 40 };


const LPCTSTR		SORT_RESULT_NAME[] = { "기본", "종목코드", "현재가", "전일대비", 
                                        "등락률", "연속", "현재거래량" };

const LPCTSTR		MATCHVAL_OPT_LIST[] = { ">", ">=", "=", "<=", "<" };
const LPCTSTR		MATCHVAL_OPT_DATA = ">\n>=\n=\n<=\n<\n";
const		LPCSTR		FIELDEDIT_TAB_STR[] = {_T("시세"), _T("호가"), _T("시고저가"), _T("최고/최저"), _T("투자자"), _T("예상"), _T("기타")};
/////Client////////////////
#define		MARKET_ALL	 "0"
#define		MARKET_KOSPI "1"
#define		MARKET_KODAQ "2"
///////////////////////////


#define CONVOL_FILE						"ConVolSetInfo.ini"
#define PROGRAM_TITLE					"순간체결량"
#define MAX_FIELD_CNT					17		//# 결과 Grid Column 구성 Max Count

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"
#define GRID_MOVEFIELD_POS_NAME			"[값보기 위치]"

#define XML_VERSION_FIXFIELDFILE			"1.0"
#define FOLDER_DATA							"\\icss"
#define FIXFIELD_SYS_FILE					"fixfield_Pattern_sys.xml"
#define FIXFIELD_USER_PATTERN_FILE			"fixfield_User_Pattern.xml"/*"fixfield_User_section.xml"*/
#define FIXFIELD_USER_TRCONVOL				"fixfield_User_TrConVol.xml"
#define FOLDER_IMAGE						"\\icss\\Image" 

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define FOLDER_FINDDATA				"\\FindData"

#define gszCacheDir "DrCache"
#define gszDataKey_MstCode "MstCode"
#define gszDataKey_WorkDate "WorkDate"
#define usrMasterCondition "SearchTemp"
#define USRDIR "\\user\\"
#define FILE_GROUP "\\portfolio.ini"

#define GETPATH_APP(strAppPath)       \
	char buf[255];														\
	GetModuleFileName(NULL, buf, 255);									\
	int lastlocationofbackslash = 0;									\
	int nBufCnt = strlen(buf);											\
	for (int i = 0; i < nBufCnt; i++)									\
	{																	\
	if (buf[i] == '\\')													\
	lastlocationofbackslash = i;										\
	}                                                                   \
	CString strAppPath = _T("");                                        \
	for (int j = 0; j < lastlocationofbackslash; j++)                   \
	strAppPath += buf[j];  

#define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())

#define FIELD_COUNT	11

typedef struct tagFieldSet	//필드 설정 Dlg
{
	char	 chkTime[1];
	char	 chkCodeItem[1];
	char	 chkNameItem[1];
	char	 chkPriceNow[1];
	char	 chkChange[1];
	char	 chkChgRate[1];
	char	 chkConVol[1];
	char	 chkAccumDealVol[1];
	char	 chkDealVolRate[1];
	char	 chkConVolRate[1];
	char	 chkConVolPower[1];
	char	 chkCount[1];
	char	 chColNum[3];
	char	 chNull[1];
} StFieldSet;

#define STCOPY_P_StFieldSet(pSrc, pTar) memset(pTar, 0x00, sizeof(StFieldSet)); memcpy(pTar, pSrc, sizeof(StFieldSet)-1);
#define STCOPY_StFieldSet(xSrc, xTar) {		\
		StFieldSet *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StFieldSet(pxSrc, pxTar);	\
	}

typedef struct tagConVolSet	//순간 체결량 설정 Dlg
{
	CString		strVer;
	CString		strScreenNo;
	
	int		nChkConVol;
	long	lConVol;

	int		nChkDealPrice;
	long	lDealPrice;

	int		nConditionCheck;	// 0:하나이상 만족, 1:둘다 만족(체결량, 거래대금)중, 3:거래대금만 만족, 4:체결량

	int		nChkAccumDealVol;
	long	lAccumDealVol;

	int		nChkSetPrice;
	long	lSetPriceUp;
	long	lSetPriceDown;

	int		nChkChgRate;
	double	dChgRateUp;
	double	dChgRateDown;

	int		nChkSound;
	CString	strPathSound;

	int		nItemPosition;   //그리드 Display시 0: 위로 추가, 1:아래로 추가

	int		nChkTargetUpjong;		// 대상종목
	CString	strMarketCode;			// 업종코드
	
	int		nChkManagement;			// 관리종목
	int		nChkTidyDeal;			// 정리매매
	int		nChkInvestAttention;	// 투자주의
	int		nChkPrefferShare;		// 우선주
	int		nChkInvestRisk;			// 투자위험
	
	int		nChkDealStop;			// 거래정지
	int		nChkInsincerityNotice;	// 불성실공시
	int		nChkPreInvestRisk;		// 투자위험예고
	int		nChkDeposit;			// 증거금 100% 종목
	int		nChkInvestWarning;		// 투자경고
} StConVolSet;

#define STCOPY_P_StConVolSet(pSrc, pTar) {								\
			pTar->strVer = pSrc->strVer;								\
			pTar->strScreenNo		= pSrc->strScreenNo;				\
			pTar->nChkConVol		= pSrc->nChkConVol;					\
			pTar->lConVol			= pSrc->lConVol;					\
			pTar->nChkDealPrice 	= pSrc->nChkDealPrice;				\
			pTar->lDealPrice		= pSrc->lDealPrice;					\
			pTar->nConditionCheck	= pSrc->nConditionCheck;			\
			pTar->nChkAccumDealVol	= pSrc->nChkAccumDealVol;			\
			pTar->lAccumDealVol 	= pSrc->lAccumDealVol;				\
			pTar->nChkSetPrice		= pSrc->nChkSetPrice;				\
			pTar->lSetPriceUp		= pSrc->lSetPriceUp;				\
			pTar->lSetPriceDown 	= pSrc->lSetPriceDown;				\
			pTar->nChkChgRate		= pSrc->nChkChgRate;				\
			pTar->dChgRateUp		= pSrc->dChgRateUp;					\
			pTar->dChgRateDown		= pSrc->dChgRateDown;				\
			pTar->nChkSound			= pSrc->nChkSound;					\
			pTar->strPathSound		= pSrc->strPathSound;				\
			pTar->nItemPosition 	= pSrc->nItemPosition;				\
																		\
			pTar->nChkTargetUpjong	= pSrc->nChkTargetUpjong;			\
			pTar->strMarketCode		= pSrc->strMarketCode;				\
																		\
			pTar->nChkManagement		= pSrc->nChkManagement;			\
			pTar->nChkTidyDeal			= pSrc->nChkTidyDeal;			\
			pTar->nChkInvestAttention	= pSrc->nChkInvestAttention;	\
			pTar->nChkPrefferShare		= pSrc->nChkPrefferShare;		\
			pTar->nChkInvestRisk		= pSrc->nChkInvestRisk;			\
			pTar->nChkDealStop			= pSrc->nChkDealStop;			\
			pTar->nChkInsincerityNotice	= pSrc->nChkInsincerityNotice;	\
			pTar->nChkPreInvestRisk		= pSrc->nChkPreInvestRisk;		\
			pTar->nChkDeposit			= pSrc->nChkDeposit;			\
			pTar->nChkInvestWarning		= pSrc->nChkInvestWarning;		\
		}
#define STCOPY_StConVolSet(xSrc, xTar) {StConVolSet *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StConVolSet(pxSrc, pxTar); }


typedef struct tagSearchTarget  //검색범위 Dlg
{
	CString strGroupName;		
	CString strSubGroupName;

	int nChkManagement;			//관리종목
	int nChkTradeStop;			//거래정지
	int nChkPrefferShare;		//우선주
	int nChkDepositMoney;		//증거금100%
	int nChkLogicSell;			//정리매매
	int nChkInsincerityNotice;	//불성실공시
	int nChkInvestRisk;			//투자위험
	int nChkInvestWarn;			//투자경고
	int nChkInvestAttention;	//투자주의
	int nChkELW;				
	int nChkInvestRiskNotice;	//투자위험예고

	int nSizeTargetcode;
	CStringArray saTargetCode; //검색할 선택 그룹의 코드

	int nSizeExceptCode;
	CStringArray saExceptCode; //제외할 종목의 코드
}ST_SEARCHTARGET;

#define STCOPY_P_ST_SEARCHTARGET(pSrc, pTar) {						\
			pTar->strGroupName		= pSrc->strGroupName;			\
			pTar->strSubGroupName	= pSrc->strSubGroupName;		\
			pTar->nChkManagement	= pSrc->nChkManagement;			\
			pTar->nChkTradeStop		= pSrc->nChkTradeStop;			\
			pTar->nChkPrefferShare	= pSrc->nChkPrefferShare;		\
			pTar->nChkDepositMoney	= pSrc->nChkDepositMoney;		\
			pTar->nChkLogicSell		= pSrc->nChkLogicSell;			\
			pTar->nChkInsincerityNotice = pSrc->nChkInsincerityNotice;	\
			pTar->nChkInvestRisk	= pSrc->nChkInvestRisk;				\
			pTar->nChkInvestWarn	= pSrc->nChkInvestWarn;				\
			pTar->nChkInvestAttention = pSrc->nChkInvestAttention;		\
			pTar->nChkELW			= pSrc->nChkELW;					\
			pTar->nChkInvestRiskNotice = pSrc->nChkInvestRiskNotice;	\
			pTar->nSizeTargetcode	= pSrc->saTargetCode.GetSize();		\
			pTar->nSizeExceptCode	= pSrc->saExceptCode.GetSize();		\
			int nCnt = 0;												\
			pTar->saTargetCode.RemoveAll();								\
			int i = 0;												\
			for(i=0; i<pSrc->saTargetCode.GetSize(); i++ )			\
				pTar->saTargetCode.Add(pSrc->saTargetCode.GetAt(i));	\
			pTar->saExceptCode.RemoveAll();								\
			for(i=0; i<pSrc->saExceptCode.GetSize(); i++ )				\
				pTar->saExceptCode.Add(pSrc->saExceptCode.GetAt(i));	\
		}

#define STCOPY_ST_SEARCHTARGET(xSrc, xTar) {			\
			ST_SEARCHTARGET *pxSrc=&xSrc, *pxTar=&xTar;	\
			STCOPY_P_ST_SEARCHTARGET(pxSrc, pxTar);		\
		}

//#define INIT_ST_SEARCHTARGET(xST) xST.nChkManagement=0;

typedef struct tagInstConVolume
{
	char	cSign[1];
	char	chGridOpt[1];     
	char	sTime[8];
	char	sCode[6];
	char	sItemName[20];
	char	sPriceNow[8];
	char	sComparison[8];
	char	sChgRate[6];
	char	sConVol[9];
	char	sAccumDealVol[9];
	char	sDealVolRate[6];
	char	sConVolRate[6];
	char	chGubun[1];
	char    sVolumePower[9];                // 체결강도 (20090408 추가)
	char	sEntryCnt[4];
}StConVolClient; //순간 체결량 화면 필드(Display용)

#define STCOPY_P_StConVolClient(pSrc, pTar) memset(pTar, 0x00, sizeof(StConVolClient)); memcpy(pTar, pSrc, sizeof(StConVolClient));
#define STCOPY_StConVolClient(xSrc, xTar) {StConVolClient *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StConVolClient(pxSrc, pxTar) }

typedef struct  
{
	COLORREF clrF;
	COLORREF clrB;	
	COLORREF clrSF;
	COLORREF clrSB;
	COLORREF clrBACK;
} ST_TABCOLOR;

typedef struct  
{
	COLORREF clrGridHeader;
	COLORREF clrGridHText;  
	COLORREF clrGridHilight;
	COLORREF clrGridSBk;
	COLORREF clrGridSText;  
	COLORREF clrGridUpText;  
	COLORREF clrGridDownText;
	COLORREF clrGridVolText; 
} ST_GRIDCOLOR;

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

typedef struct tagHeadFixInfo
{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;

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

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

typedef struct 
{
	CString szCtrlName;
	CWnd* pWnd;
} ST_MOVEITEM;
#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd;

struct	Hana_UpMaster	{
	char	jgub;			// 장구분
	// 0:장내 1:장외
	// 1: KOSPI200
	// 3: KOSDAQ50(1), KQ-IT 50(2)
	// 4: K100P(1), K50(2), IT(3
	// 6: SECTOR, 7:프리보드 
	char	ucod;			// 업종코드
	char	hnam[20];	// 한글명
};

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

#define FOLDER_FINDDATA				"\\FindData"

#define GETMARKET_ALL			1000
#define GETMARKET_KOSPI			0
#define GETMARKET_KOSPI200		100
#define GETMARKET_KOSPI100_50	400
#define	GETMARKET_KOSDAQ		200
#define	GETMARKET_KOSTAR		300
#define GETMARKET_KRX_ALL		900
#define GETMARKET_KRX			600

// KRX 업종별 구분값
#define SEC1_AT       0x01  /* 자동차                   */
#define SEC1_SM       0x02  /* 반도체                   */
#define SEC1_HC       0x04  /* 건강                     */
#define SEC1_BK       0x08  /* 은행                     */
#define SEC2_IT       0x01  /* 정보통신                 */
#define SEC2_EC       0x02  /* 화학에너지               */
#define SEC2_ST       0x04  /* 철강                     */
#define SEC2_CS       0x08  /* 필수소비재               */
#define SEC3_MT       0x01  /* 미디어통신               */
#define SEC3_CO       0x02  /* 건설                     */
#define SEC3_FB       0x04  /* 금융서비스               */
#define SEC3_SI       0x08  /* 증권                     */
#define SEC4_SC       0x01  /* 조선                     */
#define SEC4_SR       0x02  /* 사회투자책임지수         */
#define SEC4_IS       0x04  /* 보험                     */
#define SEC4_TP       0x08  /* 운송                     */
#define SEC5_RT       0x01  /* 소비자유통               */
#define SEC5_LS       0x02  /* 레저                     */
#define SEC5_KX       0x04  /* KRX100                   */

typedef struct  
{
	int xPoint;
	int yPoint;
	CString strCode;
} RButtonInfo;

#endif // CLIENT_TYPEDEF
