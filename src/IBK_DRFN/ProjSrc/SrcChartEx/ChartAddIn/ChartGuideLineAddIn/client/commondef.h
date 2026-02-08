#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H
#endif

/////////////////////////////////////////////////////////////////////////////
//목적 : 기준선 작도 AddIn Module에서 쓰는 공통 내역 정의
//날짜 : 2005. 01. 28
//작성자 : Nam 
/////////////////////////////////////////////////////////////////////////////

#define		INI_FILENAME			"ChartGuideLine.ini"
#define		ACCT_NUM				11			// 계좌
#define		ACCT_NAME				40			// 계좌명
#define		ACCT_PASS				13			// 비밀번호
#define     SIZEFULLACCT			64			//계좌(11) + 계좌명(40) + 비밀번호(13)

//InvokeAddIn 호출될 때 넘어오는 번호값 

#define INVOKEMSG_SETACCOUNTNQUOTECODE		1
#define INVOKEMSG_GETAVGINFO				2
#define INVOKEMSG_INIT						100 // 데이터를 조회할수 있는 HWND과 현 차트조각(?)의 포인터를 받는다. 
#define INVOKEMSG_RECEIVEDATA				101 // 체결 데이터를 받음.
#define INVOKEMSG_CHANGECODE				102	// 종목코드 바뀌었음 의미
#define INVOKEMSG_SHOWORDEREDINFO			103 // 차트에 그려라...



#define		UMSG_SEND2SERVERFROMINVOKE			WM_USER+500	 //TR조회 요청
#define		UMSG_GETCODEFROMINVOKE				WM_USER+501	 //종목코드 가져오기 LPARAM에 CString *를 준다.
#define		UMSG_GETMAINHANDLEFROMINVOKE		WM_USER+502	 //메인핸들 요청한다.

const UINT RMSG_GETACCTPASSENCKEY		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASSENCKEY"));	// 계좌비밀번호 요청
const UINT RMSG_VERIFYACCTPASS			= ::RegisterWindowMessage(_T("RMSG_VERIFYACCTPASS"));		// 계좌번호 검증(내부, 외부 구분없음)

// 계좌,비밀번호 관련
typedef struct _ST_CHECKACCTPASS
{
	char	szAccount[11];
	char	szAcctPass[13];
	int		nAcctCheckType;
} ST_CHECKACCTPASS;


//
/*
typedef struct _D10418_O 
{ 
	char sShcode[7];	// Short Code  
	char sName[20];		// Code Name  
	char sAvg1[16];		// Moving Average 1 Value(Type: F9.6)  
	char sAvg2[16];		// Moving Average 2 Value(Type: F9.6)  
	char sAvg3[16];		// Moving Average 3 Value(Type: F9.6)  
	char sAvg4[16];		// Moving Average 4 Value(Type: F9.6)  
	char sBuffer[12];	// Use Only Client  
} D10418_O;*/
 
typedef struct {
	char szBuffer	[12]; /* CWnd Type Pointer */
	char shcode		[7]; /* 종목코드*/ 
	char nAvg1		[3]; /* 이평1*/ 
	char nAvg2		[3]; /* 이평2*/ 
	char nAvg3		[3]; /* 이평3*/ 
	char nAvg4		[3]; /* 이평4*/ 
	/// don't send to server..
	char	chNull;
} D10418_I; 

typedef struct _D10418_O 
{ 	
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // 예를 들어 5일 이라 그러면 오늘을 제외한 이전 4일의 합 
	char sSum2[16]; // 예를 들어 20일 이라 그러면 오늘을 제외한 이전 19일의 합 
	char sSum3[16]; // 예를 들어 60일 이라 그러면 오늘을 제외한 이전 59일의 합 
	char sSum4[16]; // 예를 들어 120일 이라 그러면 오늘을 제외한 이전 119일의 합 
	
} D10418_O; 

typedef struct _D10418_INVOKE_O 
{ 	
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // 예를 들어 5일 이라 그러면 오늘을 제외한 이전 4일의 합 
	char sSum2[16]; // 예를 들어 20일 이라 그러면 오늘을 제외한 이전 19일의 합 
	char sSum3[16]; // 예를 들어 60일 이라 그러면 오늘을 제외한 이전 59일의 합 
	char sSum4[16]; // 예를 들어 120일 이라 그러면 오늘을 제외한 이전 119일의 합 
	
} D10418_INVOKE_O; 

// Server Input Structure
typedef struct __STSEND2SERVER 
{
	CString		m_strTRCode;
	void*		m_pBuffer;
	long		m_lLength;
	CWnd*		m_pWnd;
	int			m_nType;	// 0: Normal 1:Invoke
}stSend2Server;


// Server Output Structure
typedef struct __STRECEIVEFROMSERVER // AP 조회용 
{
	CString		m_strTRCode;
	void *		m_pBuffer;
	DWORD		m_dwLength;
}stReceiveFromServer;

typedef struct  {
	HWND		m_hChartMng;
	CWnd*		m_pDummy;		// Do not use it. m_pChartItem;
	void *		m_pBuffer;		
	DWORD		m_dwLength;

}stMSGToInvoke;

//===================================================================================
// 체결 리스트 조회 =========================
//===================================================================================
#define GRDMX 100

typedef struct _D48112_ISUB
{
    char    nxtGb        [ 1];       /* 0:처음조회, 1:연속조회   */
    char    ordseq       [ 1];       /* 순서구분     *//* start of gwInPacket */
	char    querygb     [ 1];       /* 조회 구분   0:전체, 1:매도, 2:매수, 3:미체결, 4:체결 */
    char    gejanum     [11];       /* 계좌번호     */
    char    passwd      [13];       /* 비밀번호     */
    char    jmcode       [12];       /* 'A'&종목코드 */
    char    frstordno    [ 5];       /* 최초원주문번호*/
    char    oriordno     [ 5];       /* 원주문번호   */
    char    ordno         [ 5];       /* 주문번호     */
    char    chedanga   [ 9];       /* 체결단가     *//* end of gwInPacket */
} D48112_ISUB;

typedef struct _D48112_OG
{
//	char	garvage		[ 4];		/*동양에서 넣은 값*/
    char    frstordno   [ 5];       /* 최초원주문번호*/
    char    orgordno    [ 5];       /* 원주문번호   */
    char    ordno       [ 5];       /* 주문번호     */
    char    cheno       [ 5];       /* 체결번호     */
    char    jmcode      [12];       /* 종목코드     */
    char    jmnm        [60];       /* 종목명       */
    char    ordgb       [ 2];       /* 주문구분     */
    char    mkgb        [ 2];       /* 시장거래구분 */
    char    ordcnt      [ 9];       /* 주문수량     */
    char    orddanga    [ 9];       /* 주문단가     */
    char    checnttot   [ 9];       /* 체결수량합   */
    char    chedanga    [ 9];       /* 체결단가     */
    char    checnt      [ 9];       /* 체결수량     */
    char    jjcancnt    [ 9];       /* 정정취소수량 */
    char    michecnt    [ 9];       /* 미체결수량   */
    char    mesumedogb  [ 1];       /* 매도매수구분 */
    char    singb       [ 2];       /* 신용구분     */
    char    jjcangb     [ 1];       /* 정정취소구분 */
    char    ordtime     [ 8];       /* 주문시간     */
    char    chetime     [ 8];       /* 체결시간     */
    char    mechegb     [ 2];       /* 매체구분     */
    char    ordjr       [ 2];       /* 주문종류     */
    char    ordjg       [ 2];       /* 주문조건     */
    char    curigb      [ 4];       /* 처리구분     */
} D48112_OG;



typedef struct _D48112_OSUB
{
    char        nxtGb       [ 1];
    char        medoyakval  [12];
    char        medoyakvol  [ 9];
    char        mesuyakval  [12];
    char        mesuyakvol  [ 9];
	
    char        grdCnt      [ 7];
	
    D48112_OG   grid        [GRDMX];
} D48112_OSUB;

typedef struct _D48112_I
{
    char        sBuffer     [12];
    D48112_ISUB iData;
} D48112_I;


typedef struct _D48112_O
{
    char        sBuffer     [12];
    D48112_OSUB oData;
} D48112_O;