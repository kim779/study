#if !defined(AFX_S31_DRDSPACKET_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_S31_DRDSPACKET_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../CommonTR/DrdsS31.h"
// 작성 : 이준옥
// 날자 : 2005.11.23(수)

#include "Data_Common.h"

//---- 현재가 (S31) --------------------------------------------------
//     SVR : 우선호가, 체결, 기세, 장운용, RECOVERY, 전후장종가
//--------------------------------------------------------------------

typedef struct 
{
	DEFINE_DRDS_LINE0( "종목코드",                       code,         char,   6)	
} DRDS_S31_KEY;

//char sign[1]; /* 등락부호                       M  */
              /* '1':상한, '2':상승, '3':보합      */
              /* '4':하한, '5':하락, '6':기세상한  */
              /* '7':기세상승, '8':기세하한        */
              /* '9':기세하락, '0':기세보합        */

typedef struct {
	DEFINE_DRDS_LINE0( "종목코드",                       code,         char,   6)
    DEFINE_DRDS_LINE0( "시간(HH:MM:SS)",                 time,         char,   6)
    DEFINE_DRDS_LINE0( "등락부호",                       sign,         char,   1)  
    DEFINE_DRDS_LINE0( "등락폭",                         change,       char,   9)
    DEFINE_DRDS_LINE0( "현재가",                         price,        char,   9)
    DEFINE_DRDS_LINE0( "등락률",                         chrate,       char,   6)
    DEFINE_DRDS_LINE0( "전장시가",                       openam,       char,   9)
    DEFINE_DRDS_LINE0( "고가",                           high,         char,   9)
    DEFINE_DRDS_LINE0( "저가",                           low,          char,   9)
    DEFINE_DRDS_LINE0( "매도호가",                       offer,        char,   9)
    DEFINE_DRDS_LINE0( "매수호가",                       bid,          char,   9)
    DEFINE_DRDS_LINE0( "누적거래량",                     volume,       char,  12)
    DEFINE_DRDS_LINE0( "변동거래량",                     movolume,     char,   8)
    DEFINE_DRDS_LINE0( "거래대금",                       value,        char,  14)
    DEFINE_DRDS_LINE0( "매매구분",                       maegubun,     char,   1)
    DEFINE_DRDS_LINE0( "장구분",                         janggubun,    char,   1)
    DEFINE_DRDS_LINE0( "CB구분",                         cbgubun,      char,   1)
    DEFINE_DRDS_LINE0( "STOP",                           stop,         char,   1)
    DEFINE_DRDS_LINE0( "시간외거래량",                   over_volume,  char,   9)
    DEFINE_DRDS_LINE0( "시간외대량매매",                 rep_volume,   char,   9)
    DEFINE_DRDS_LINE0( "매도호가잔량",                   offerrem,     char,  10)
    DEFINE_DRDS_LINE0( "매수호가잔량",                   bidrem,       char,  10)
    DEFINE_DRDS_LINE0( "총매도호가잔량",                 T_offerrem,   char,  10)
    DEFINE_DRDS_LINE0( "총매수호가잔량",                 T_bidrem,     char,  10)

    DEFINE_DRDS_LINE0( "누적매수량",					bidvol,		   char,  10)
    DEFINE_DRDS_LINE0( "매수강도",						bidstrong,	   char,   9)
    DEFINE_DRDS_LINE0( "변동거래량2",					movolume2,	   char,   8)	// 신호화면의 설정대비 거래량
    DEFINE_DRDS_LINE0( "누적매수량2",					bidvol2,	   char,  10)	// 신호화면의 설정대비 매수량
    DEFINE_DRDS_LINE0( "매수강도2",						bidstrong2,	   char,   9)	// 신호화면의 설정대비 강도
} DRDS_S31_DATA;

class C_DRDS_S31_DATA
{
public: //Variable
	CString m_szcode;			/* "종목코드"			*/          
	CString m_sztime;			/* "시간(HH:MM:SS)"		*/    
	CString m_szsign;			/* "등락부호"			*/          
	CString m_szchange;			/* "등락폭"				*/            
	CString m_szprice;			/* "현재가"				*/            
	CString m_szchrate;			/* "등락률"				*/            
	CString m_szopenam;			/* "전장시가"			*/          
	CString m_szhigh;			/* "고가"				*/              
	CString m_szlow;			/* "저가"				*/              
	CString m_szoffer;			/* "매도호가"			*/          
	CString m_szbid;			/* "매수호가"			*/          
	CString m_szvolume;			/* "누적거래량"			*/        
	CString m_szmovolume;		/* "변동거래량"			*/        
	CString m_szvalue;			/* "거래대금"			*/          
	CString m_szmaegubun;		/* "매매구분"			*/          
	CString m_szjanggubun;		/* "장구분"				*/            
	CString m_szcbgubun;		/* "CB구분"				*/            
	CString m_szstop;			/* "STOP"				*/              
	CString m_szover_volume;	/* "시간외거래량"		*/      
	CString m_szrep_volume;		/* "시간외대량매매"		*/    
	CString m_szofferrem;		/* "매도호가잔량"		*/      
	CString m_szbidrem;			/* "매수호가잔량"		*/      
	CString m_szT_offerrem;		/* "총매도호가잔량"		*/    
	CString m_szT_bidrem;		/* "총매수호가잔량"		*/    

	CString m_szbidvol;			/* "누적매수량"			*/    
	CString m_szbidstrong;		/* "매수강도"			*/    
	CString m_szmovolume2;		/* "변동거래량2"		*/    
	CString m_szbidvol2;		/* "누적매수량2"		*/    
	CString m_szbidstrong2;		/* "매수강도2"			*/    

public:
	C_DRDS_S31_DATA()
	{
	}

	virtual void  Convert(DRDS_S31_DATA* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, code);			/* "종목코드"			*/	
		DATA_MEMCOPY(sTmp, pInput, time);			/* "시간(HH:MM:SS)"		*/		
		DATA_MEMCOPY(sTmp, pInput, sign);			/* "등락부호"			*/		
		DATA_MEMCOPY(sTmp, pInput, change);			/* "등락폭"				*/		
		DATA_MEMCOPY(sTmp, pInput, price);			/* "현재가"				*/		
		DATA_MEMCOPY(sTmp, pInput, chrate);			/* "등락률"				*/		
		DATA_MEMCOPY(sTmp, pInput, openam);			/* "전장시가"			*/		
		DATA_MEMCOPY(sTmp, pInput, high);			/* "고가"				*/		
		DATA_MEMCOPY(sTmp, pInput, low);			/* "저가"				*/		
		DATA_MEMCOPY(sTmp, pInput, offer);			/* "매도호가"			*/		
		DATA_MEMCOPY(sTmp, pInput, bid);			/* "매수호가"			*/		
		DATA_MEMCOPY(sTmp, pInput, volume);			/* "누적거래량"			*/		
		DATA_MEMCOPY(sTmp, pInput, movolume);		/* "변동거래량"			*/		
		DATA_MEMCOPY(sTmp, pInput, value);			/* "거래대금"			*/		
		DATA_MEMCOPY(sTmp, pInput, maegubun);		/* "매매구분"			*/		
		DATA_MEMCOPY(sTmp, pInput, janggubun);		/* "장구분"				*/		
		DATA_MEMCOPY(sTmp, pInput, cbgubun);		/* "CB구분"				*/		
		DATA_MEMCOPY(sTmp, pInput, stop);			/* "STOP"				*/		
		DATA_MEMCOPY(sTmp, pInput, over_volume);	/* "시간외거래량"		*/		
		DATA_MEMCOPY(sTmp, pInput, rep_volume);		/* "시간외대량매매"		*/		
		DATA_MEMCOPY(sTmp, pInput, offerrem);		/* "매도호가잔량"		*/		
		DATA_MEMCOPY(sTmp, pInput, bidrem);			/* "매수호가잔량"		*/		
		DATA_MEMCOPY(sTmp, pInput, T_offerrem);		/* "총매도호가잔량"		*/		
		DATA_MEMCOPY(sTmp, pInput, T_bidrem);		/* "총매수호가잔량"		*/			
	
		DATA_MEMCOPY(sTmp, pInput, bidvol);			/* "누적매수량"			*/			
		DATA_MEMCOPY(sTmp, pInput, bidstrong);		/* "매수강도"			*/			
		DATA_MEMCOPY(sTmp, pInput, movolume2);		/* "변동거래량2"		*/			
		DATA_MEMCOPY(sTmp, pInput, bidvol2);		/* "누적매수량2"		*/			
		DATA_MEMCOPY(sTmp, pInput, bidstrong2);		/* "매수강도2"			*/			
	}
};

#endif // AFX_S31_DRDSPACKET_BY_JUNOK_LEE_98B1A__INCLUDED_
