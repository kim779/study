#if !defined(AFX__TREEITEM_LIST__HEADER__)
#define  AFX__TREEITEM_LIST__HEADER__

// #include "../CommonTR/TreeItem.h"

//-------------------------------------------------------------------------------##
//                    사용TR번호:88888                                           ##
//--------------------------------[INPUT ]---------------------------------------##

typedef struct _tr88888_i
{
    char  reserved  [ 8];       /* 결과리턴시 되돌려주는 값 */
	char  mainkey	[ 4];       /* 분류키                   */
	char  subkey    [ 3];       /* 분류-서브분류키          */
	char  sort      [ 1];       /* 0 : code순 
	                               1 : codename순           */
	char  datatype  [ 1];       /* 요청하는 데이터 타입     */
	                            /* A : TR_88888_A
								   B : TR_88888_B           */
} TR_88888_I;
#define  TR_88888_I_SIZE sizeof(TR_88888_I)

//--------------------------------[OUTPUT ]---------------------------------------##

typedef struct _tr_88888_A
{
	char code       [ 6];				/* 종목코드			*/
	char hname      [20];				/* 종목명(한글)M	*/
} TR_88888_A;

typedef struct _tr_88888_B
{
	char code       [ 6];				/* 종목코드			*/
	char hname      [20];				/* 종목명(한글)M	*/
    char time       [ 6];				/* 시간(HH:MM:SS)	*/
    char sign       [ 1];				/* 등락부호			*/
    char change     [ 9];				/* 등락폭			*/
    char price      [ 9];				/* 현재가			*/
    char chrate     [ 5];				/* 등락률			*/
	char volume     [12];               /* 누적거래량       */
} TR_88888_B;



typedef struct _tr_88888_o
{
    char  reserved  [ 8];       /* 결과리턴시 되돌려주는 값 */
	char  count     [ 4];       /* 데이터 카운트            */
    // 데이터타입에 따른 데이타를 여기에 붙여서 보낸다.
	//
} TR_88888_O;

#endif //AFX__TREEITEM_LIST__HEADER__
