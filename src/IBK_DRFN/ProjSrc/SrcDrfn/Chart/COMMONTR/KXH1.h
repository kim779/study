#if !defined(AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX 호가잔량(KXH1) ( KXH1,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXH1     "KXH1"

// 입력
typedef struct _KXH1_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
} KXH1_InBlock, *LPKXH1_InBlock;
#define NAME_KXH1_InBlock     "InBlock"

// 출력
typedef struct _KXH1_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] 호가시간
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] 매도호가1
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] 매수호가1
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] 매도호가잔량1
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] 매수호가잔량1
    char    preoffercha1        [  12];    char    _preoffercha1        ;    // [long  ,   12] 직전매도대비수량1
    char    prebidcha1          [  12];    char    _prebidcha1          ;    // [long  ,   12] 직전매수대비수량1
    char    offerho2            [   8];    char    _offerho2            ;    // [long  ,    8] 매도호가2
    char    bidho2              [   8];    char    _bidho2              ;    // [long  ,    8] 매수호가2
    char    offerrem2           [  12];    char    _offerrem2           ;    // [long  ,   12] 매도호가잔량2
    char    bidrem2             [  12];    char    _bidrem2             ;    // [long  ,   12] 매수호가잔량2
    char    preoffercha2        [  12];    char    _preoffercha2        ;    // [long  ,   12] 직전매도대비수량2
    char    prebidcha2          [  12];    char    _prebidcha2          ;    // [long  ,   12] 직전매수대비수량2
    char    offerho3            [   8];    char    _offerho3            ;    // [long  ,    8] 매도호가3
    char    bidho3              [   8];    char    _bidho3              ;    // [long  ,    8] 매수호가3
    char    offerrem3           [  12];    char    _offerrem3           ;    // [long  ,   12] 매도호가잔량3
    char    bidrem3             [  12];    char    _bidrem3             ;    // [long  ,   12] 매수호가잔량3
    char    preoffercha3        [  12];    char    _preoffercha3        ;    // [long  ,   12] 직전매도대비수량3
    char    prebidcha3          [  12];    char    _prebidcha3          ;    // [long  ,   12] 직전매수대비수량3
    char    offerho4            [   8];    char    _offerho4            ;    // [long  ,    8] 매도호가4
    char    bidho4              [   8];    char    _bidho4              ;    // [long  ,    8] 매수호가4
    char    offerrem4           [  12];    char    _offerrem4           ;    // [long  ,   12] 매도호가잔량4
    char    bidrem4             [  12];    char    _bidrem4             ;    // [long  ,   12] 매수호가잔량4
    char    preoffercha4        [  12];    char    _preoffercha4        ;    // [long  ,   12] 직전매도대비수량5
    char    prebidcha4          [  12];    char    _prebidcha4          ;    // [long  ,   12] 직전매수대비수량5
    char    offerho5            [   8];    char    _offerho5            ;    // [long  ,    8] 매도호가5
    char    bidho5              [   8];    char    _bidho5              ;    // [long  ,    8] 매수호가5
    char    offerrem5           [  12];    char    _offerrem5           ;    // [long  ,   12] 매도호가잔량5
    char    bidrem5             [  12];    char    _bidrem5             ;    // [long  ,   12] 매수호가잔량5
    char    preoffercha5        [  12];    char    _preoffercha5        ;    // [long  ,   12] 직전매도대비수량5
    char    prebidcha5          [  12];    char    _prebidcha5          ;    // [long  ,   12] 직전매수대비수량5
    char    offerho6            [   8];    char    _offerho6            ;    // [long  ,    8] 매도호가6
    char    bidho6              [   8];    char    _bidho6              ;    // [long  ,    8] 매수호가6
    char    offerrem6           [  12];    char    _offerrem6           ;    // [long  ,   12] 매도호가잔량6
    char    bidrem6             [  12];    char    _bidrem6             ;    // [long  ,   12] 매수호가잔량6
    char    preoffercha6        [  12];    char    _preoffercha6        ;    // [long  ,   12] 직전매도대비수량6
    char    prebidcha6          [  12];    char    _prebidcha6          ;    // [long  ,   12] 직전매수대비수량6
    char    offerho7            [   8];    char    _offerho7            ;    // [long  ,    8] 매도호가7
    char    bidho7              [   8];    char    _bidho7              ;    // [long  ,    8] 매수호가7
    char    offerrem7           [  12];    char    _offerrem7           ;    // [long  ,   12] 매도호가잔량7
    char    bidrem7             [  12];    char    _bidrem7             ;    // [long  ,   12] 매수호가잔량7
    char    preoffercha7        [  12];    char    _preoffercha7        ;    // [long  ,   12] 직전매도대비수량7
    char    prebidcha7          [  12];    char    _prebidcha7          ;    // [long  ,   12] 직전매수대비수량7
    char    offerho8            [   8];    char    _offerho8            ;    // [long  ,    8] 매도호가8
    char    bidho8              [   8];    char    _bidho8              ;    // [long  ,    8] 매수호가8
    char    offerrem8           [  12];    char    _offerrem8           ;    // [long  ,   12] 매도호가잔량8
    char    bidrem8             [  12];    char    _bidrem8             ;    // [long  ,   12] 매수호가잔량8
    char    preoffercha8        [  12];    char    _preoffercha8        ;    // [long  ,   12] 직전매도대비수량8
    char    prebidcha8          [  12];    char    _prebidcha8          ;    // [long  ,   12] 직전매수대비수량8
    char    offerho9            [   8];    char    _offerho9            ;    // [long  ,    8] 매도호가9
    char    bidho9              [   8];    char    _bidho9              ;    // [long  ,    8] 매수호가9
    char    offerrem9           [  12];    char    _offerrem9           ;    // [long  ,   12] 매도호가잔량9
    char    bidrem9             [  12];    char    _bidrem9             ;    // [long  ,   12] 매수호가잔량9
    char    preoffercha9        [  12];    char    _preoffercha9        ;    // [long  ,   12] 직전매도대비수량9
    char    prebidcha9          [  12];    char    _prebidcha9          ;    // [long  ,   12] 직전매수대비수량9
    char    offerho10           [   8];    char    _offerho10           ;    // [long  ,    8] 매도호가10
    char    bidho10             [   8];    char    _bidho10             ;    // [long  ,    8] 매수호가10
    char    offerrem10          [  12];    char    _offerrem10          ;    // [long  ,   12] 매도호가잔량10
    char    bidrem10            [  12];    char    _bidrem10            ;    // [long  ,   12] 매수호가잔량10
    char    preoffercha10       [  12];    char    _preoffercha10       ;    // [long  ,   12] 직전매도대비수량10
    char    prebidcha10         [  12];    char    _prebidcha10         ;    // [long  ,   12] 직전매수대비수량10
    char    totofferrem         [  12];    char    _totofferrem         ;    // [long  ,   12] 매도호가총수량
    char    totbidrem           [  12];    char    _totbidrem           ;    // [long  ,   12] 매수호가총수량
    char    preoffercha         [  12];    char    _preoffercha         ;    // [long  ,   12] 직전매도호가총대비수량
    char    prebidcha           [  12];    char    _prebidcha           ;    // [long  ,   12] 직전매수호가총대비수량
    char    donsigubun          [   1];    char    _donsigubun          ;    // [long  ,    1] 동시호가구분
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} KXH1_OutBlock, *LPKXH1_OutBlock;
#define NAME_KXH1_OutBlock     "OutBlock"

class CKBREAL_KXH1
{
public: //Variable
	CString		m_szkshcode;		/*  [단축코드]				*/
	CString		m_szofferho1;		/*  [매도호가 1]			*/
	CString		m_szbidho1;			/*  [매수호가 1]			*/
	CString		m_szofferrem1;		/*  [매도호가 잔량 1]		*/
	CString		m_szbidrem1;		/*  [매수호가 잔량 1]		*/
	CString		m_szofferho2;		/*  [매도호가 2]			*/
	CString		m_szbidho2;			/*  [매수호가 2]			*/
	CString		m_szofferrem2;		/*  [매도호가 잔량 2]		*/
	CString		m_szbidrem2;		/*  [매수호가 잔량 2]		*/
	CString		m_szofferho3;		/*  [매도호가 3]			*/
	CString		m_szbidho3;			/*  [매수호가 3]			*/
	CString		m_szofferrem3;		/*  [매도호가 잔량 3]		*/
	CString		m_szbidrem3;		/*  [매수호가 잔량 3]		*/
	CString		m_szofferho4;		/*  [매도호가 4]			*/
	CString		m_szbidho4;			/*  [매수호가 4]			*/
	CString		m_szofferrem4;		/*  [매도호가 잔량 4]		*/
	CString		m_szbidrem4;		/*  [매수호가 잔량 4]		*/
	CString		m_szofferho5;		/*  [매도호가 5]			*/
	CString		m_szbidho5;			/*  [매수호가 5]			*/
	CString		m_szofferrem5;		/*  [매도호가 잔량 5]		*/
	CString		m_szbidrem5;		/*  [매수호가 잔량 5]		*/
	CString		m_sztotofferrem;	/*  [매도호가총수량]		*/
	CString		m_sztotbidrem;		/*  [매수호가총수량]		*/

public:
	CKBREAL_KXH1()
	{
	}

	virtual void  Convert(LPKXH1_OutBlock pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [단축코드]				*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [매도호가 1]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [매수호가 1]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/*  [매도호가 잔량 1]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/*  [매수호가 잔량 1]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  [매도호가 2]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/*  [매수호가 2]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/*  [매도호가 잔량 2]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/*  [매수호가 잔량 2]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  [매도호가 3]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/*  [매수호가 3]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/*  [매도호가 잔량 3]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/*  [매수호가 잔량 3]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/*  [매도호가 4]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/*  [매수호가 4]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/*  [매도호가 잔량 4]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/*  [매수호가 잔량 4]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/*  [매도호가 5]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/*  [매수호가 5]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/*  [매도호가 잔량 5]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/*  [매수호가 잔량 5]		*/
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/*  [매도호가총수량]		*/
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/*  [매수호가총수량]		*/
	}
};

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_
