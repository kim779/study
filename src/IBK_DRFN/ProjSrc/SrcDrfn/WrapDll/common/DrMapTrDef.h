#if !defined(AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_)
#define AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//DrMapTrDef.h

#pragma	pack(1)

#define  KEY_SIZE 23
struct mid
{
	char    trxc[8];            /* 서비스명                 */
	char    usr1[12];           /* 공통 입력1               */
	char    usr2[12];           /* 공통 입력2               */
	char    usr3[12];           /* 공통 입력3               */

	char	cod2[100];
};
#define	SZ_HEADSIZE_MID		(sizeof(mid)-100)
//
// 	struct grid
// 	{
// 		char    date[ 8];           /* 날짜                     */
// 		char    siga[10];           /* 시가                     */
// 		char    koga[10];           /* 고가                     */
// 		char    jega[10];           /* 저가                     */
// 		char    jgga[10];           /* 종가                     */
// 		char    gvol[10];           /* 거래량                   */
// 		char    gamt[18];           /* 거래대금                 */
// 		char    jchk[ 1];           /* 권리락                   */
// 	};

struct mod 
{
	char    trxc[8];            /* 서비스명                 */
	char    usr1[12];           /* 공통 입력1               */
	char    usr2[12];           /* 공통 입력2               */
	char    usr3[12];           /* 공통 입력3               */
	
// 	char    cod2[32];           /* 종목 코드                */    
// 	char    siga[12];           /* 시가                     */
// 	char    koga[12];           /* 고가                     */
// 	char    jega[12];           /* 저가                     */
// 	char    jgga[12];           /* 종가                     */
// 	char    gvol[ 9];           /* 거래량                   */
// 	char    msga[12];           /* 매수호가                 */
// 	char    mdga[12];           /* 매도호가                 */
// 	char    shga[12];           /* 상한가                   */
// 	char    hhga[12];           /* 하한가                   */
// 	char    jsga[12];           /* 전일 시가                */
// 	char    jhga[12];           /* 전일 고가                */
// 	char    jjga[12];           /* 전일 저가                */
// 	char    jcur[12];           /* 전일 종가                */
// 	char    jvol[ 9];           /* 전일 거래량              */
// 	char    diff[10];           /* 전일 대비                */
// 	char    rate[ 8];           /* 등락률                   */
// 	char    gjga[12];           /* 기준가                   */
// 	char    rtim1[6];           /* 장시작시간               */
// 	char    rtim2[6];           /* 장마감시간               */
// 	char    bday[ 8];           /* 영업일                   */
// 	char    giho[ 1];           /* 전일대비기호             */
// 	char    nkey[KEY_SIZE];     /* Next Key                 */
// 	char    gapk[ 9];           /* 갭보정                   */
// 	char    fill[ 3];           /* 널문자체크               */
// 	char    nrec[ 4];           /* 반복건수                 */
// 	struct  grid    grid[1];
};

// 	typedef struct
// 	{
// 		char cBozoSize[6];	//보조메세지 사이즈 
// 		char Bozo;			//보노메세지 Name=value; 
// 		char nCount[4];		//차트봉갯수
// 		struct  grid    grid[1];
// 	}TEST_HD;

#pragma	pack()

#endif // !defined(AFX_FF7DE5F5_27EC_4edf_9D00_090C502CE25B__INCLUDED_)