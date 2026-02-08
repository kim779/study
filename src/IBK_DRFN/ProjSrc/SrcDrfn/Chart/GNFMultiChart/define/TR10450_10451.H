/***************************************************************************
* 파 일 명 : tr_10450.h
* 작 성 자 : Suh Song lee
* 작 성 일 : 2005.05.12
* 기    능 : 주식차트 - 시장지표 항목 데이터 조회  structure define
***************************************************************************/

#ifndef __MARKET_TR_H
#define __MARKET_TR_H
#endif

//@유진삭제
//typedef struct
//{
//	char	wndID			[ 8];	// 요청 윈도우 ID
//    char    type            [ 1];   // 0:종목, 1:업종		
//    char    shcode          [ 8];   // 종목코드				
//    char    edate           [ 8];   // 검색일자 (최근일)    
//    char    num             [ 4];   // 검색할 데이터 갯수   
//    char    imgr_Gb         [ 1];   // 허(0)/실봉(1) 구분   
//	char	pkName			[100];	// 패킷명 
//
//    char    nxtGb       [ 1];   /* 다음(1)/조회(0) 구분 */
//    char    nxtKey      [14];   /* 다음/이전 Fetch Key  */
//}TR_MARKET_I;
//#define SZ_TR_MARKET_I sizeof(TR_MARKET_I)
//@유진삭제

//@유진추가
typedef struct
{
	char	wndID			[ 8];	// 요청 윈도우 ID
    char    type            [ 1];   // 0:종목, 1:업종		
    char    shcode          [ 8];   // 종목코드				
    char    edate           [ 8];   // 검색일자 (최근일)    
    char    num             [ 4];   // 검색할 데이터 갯수   
    char    imgr_Gb         [ 1];   // 허(0)/실봉(1) 구분   
	char	pkName			[100];	// 패킷명 

    char    nxtGb			[ 1];   /* 다음(1)/조회(0) 구분 */
    char    nxtKey			[14];   /* 다음/이전 Fetch Key  */
}	p0607InBlock;
#define SZ_TR_MARKET_I sizeof(p0607InBlock)
//@유진추가

// 3개 짜리 =============================================================
// 투자자 순매수 전용
		typedef struct
		{
			char    date        [ 8];   // 날짜
			char    pk_name1    [ 15];   //            
			char    pk_name2    [ 15];   //            
			char    pk_name3    [ 15];   // 
		}TR_MARKET_3_OS;
		#define SZ_TR_MARKET_3_OS sizeof(TR_MARKET_3_OS)

		typedef struct
		{
			char	wndID		[ 8];		// 요청 윈도우 ID
			char	pkName		[100];		// 패킷명 
			char    inNumGb     [ 1];      /* 1 or 3           */
		    char    shcode      [ 8];      // 종목코드				
			char    nxtKey      [14];      /* 이전키            */
			char    chtCnt      [ 5];      /* 카운트            */
//			char    Cbojolen	[ 4];	   /* Chart Bojo len   */
			char    Cbojo		[300];     /* Chart Bojo Data  */

			TR_MARKET_3_OS	os_buf	[3000];
		}TR_MARKET_3_O;

		typedef struct
		{	
			char    chtCnt      [ 5];      /* 카운트            */
//			char    Cbojolen	[ 4];	   /* Chart Bojo len   */
			char    Cbojo		[300];     /* Chart Bojo Data  */

			TR_MARKET_3_OS	os_buf	[3000];
			char	chNULL		[1];
		}TR_MARKET_3_CHART;
		

// 1개 짜리 =============================================================
// 기타 시장 지표 아웃풋 구조.
		typedef struct
		{
			char    date        [ 8];   // 날짜
			char    pk_name1    [15];   //            	
		}TR_MARKET_1_OS;
		#define SZ_TR_MARKET_1_OS sizeof(TR_MARKET_1_OS)

		typedef struct
		{
			char	wndID		[ 8];	   // 요청 윈도우 ID
			char	pkName		[100];		// 패킷명
			char    inNumGb     [ 1];      /* 1 or 3           */
			char    shcode      [ 8];      // 종목코드				
			char    nxtKey      [14];      /* 이전키            */
			char    chtCnt      [ 5];      /* 카운트            */
			char    Cbojolen	[ 4];	   /* Chart Bojo len   */
			char    Cbojo		[252];     /* Chart Bojo Data  */

			TR_MARKET_1_OS	os_buf	[3000];
		}TR_MARKET_1_O;

		typedef struct
		{	
			char    chtCnt      [ 5];      /* 카운트            */
			char    Cbojolen	[ 4];	   /* Chart Bojo len   */
			char    Cbojo		[252];     /* Chart Bojo Data  */

			TR_MARKET_1_OS	os_buf	[3000];
			char	chNULL		[1];
		}TR_MARKET_1_CHART;





//------------------------------------------------------------------------
/***************************************************************************
* 파 일 명 : tr_10451.h
* 작 성 자 : Suh Song lee
* 작 성 일 : 2005.05.12
* 기    능 : 주식차트 - 시장지표 항목 데이터 조회  structure define
***************************************************************************/

//typedef struct
//{
//	char	wndID			[ 8];	// 요청 윈도우 ID
//    char    type            [ 1];   /* 0:종목, 1:업종   */
//    char    shcode          [ 7];   /* 종목코드     */
//    char    edate           [ 8];   /* 검색일자 (최근일)    */
//    char    num             [ 4];   /* 검색할 데이터 갯수   */
///**************추가******************************************/
//    char    imgr_Gb         [ 1];   /* 허(0)/실봉(1) 구분   */
///************************************************************/
//	char    pkName	        [50];
////    char    new_gang_Gb     [ 1];   /* 뉴스강도           구분 */
////    char    new_cont_Gb     [ 1];   /* 뉴스건수           구분 */
//
//    char    nxtGb       [ 1];   /* 다음(1)/조회(0) 구분 */
//    char    nxtKey      [14];   /* 다음/이전 Fetch Key  */
//}TR_10451_I;
//
//typedef struct
//{
//    char    date        [ 8];   /* 날짜                    */
// //   char    close       [ 9];   /* 종가                    */
//
//    char    pk_name1    [9];   //
//}TR_10451_OS;
//#define SZ_TR_10451_OS sizeof(TR_10451_OS)
//
//typedef struct
//{
//	char	wndID		[ 8];	   // 요청 윈도우 ID
//	char    pkName	    [50];
//	char    shcode      [ 7];      // 종목코드				
//    char    nxtKey      [14];      /* 이전키            */
//    char    chtCnt      [ 5];      /* 카운트            */
//    char    Cbojolen    [ 4];      /* Chart Bojo len   */
//    char    Cbojo       [252];     /* Chart Bojo Data  */
//
//    TR_10451_OS	os_buf	[3000];
//}TR_10451_O;
//#define SZ_TR_10451_O sizeof(TR_10451_O)
//
//typedef struct
//{
//    char    chtCnt      [ 5];      /* 카운트            */
//    char    Cbojolen    [ 4];      /* Chart Bojo len   */
//    char    Cbojo       [252];     /* Chart Bojo Data  */
//
//    TR_10451_OS	os_buf	[3000];
//}TR_10451_CHART;


