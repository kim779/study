#ifndef _DEF_GWANSIM_DEFINE__HEADER__
#define _DEF_GWANSIM_DEFINE__HEADER__

// #include "../../inc/Define_Gwansim.h"
//#include "../Dlls/CommonTR/Data_Common.h"
#include "../chart/CommonTR/Data_Common.h"


const UINT RMSG_CONCERNHWND = ::RegisterWindowMessage(_T("RMSG_CONCERNHWND") );
const UINT RMSG_INTREQUEST = ::RegisterWindowMessage(_T("RMSG_INTREQUEST") );

//{{ 관심TR조회관련.
#define TR_88803				_T("88803")
#define REQUEST_INTTOTAL		"01" // 관심 모든 정보 얻기(메모리 카피해야 함) 
#define GWAMSIM_DEF_NAME		"관심그룹"
//}} 관심


typedef struct
{
	char	Igubun[2]	;	// 0 : ERROR, 1 : 1전체 정보(종목리스트 포함), 2 : 조건 그룹 등록, 3 : 종목 등록    
	char	Icnt[3]		;	// 그룹 수
	char	IopenDlg[1]	;	// 0 : Open, 1 : No Open
	void*	Idata		;	// 그룹 데이터들(CPtrArray) 
	char	Ititle[256]	;
}INT_TOTAL;
#define		SIZE_INT_TOTAL	sizeof(INT_TOTAL)

typedef struct
{
	char	Ino		[ 2];	// 그룹 번호 
	char	Ititle	[20];	// 그룹 명
	char	Icnt	[ 3];	// 그룹 안에 종목 수
	void*	Idata		;	// 종목 데이터들(CPtrArray)
	char	Itime	[ 6];
	char	Idate	[ 8];
}INT_GROUP_ST;
#define		SIZE_INT_GROUP_ST	sizeof(INT_GROUP_ST)

typedef struct
{
	char Icode[12];			// 코드  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   시,분,초
	char Idate[8];		   // YYYYMMDD 년,월,일
	char Ivol[10];		   // 수량 	
	char Ival[10];		   // 편입가격
	char ICheck[1];		   // 체크유무
	void* Idata;
	char IName[30];
} INT_ITEM_ST;
#define		SIZE_INT_ITEM_ST	sizeof(INT_ITEM_ST)

class CINT_TOTAL
{
public:
	CString  m_szIgubun		;	// 0 : ERROR, 1 : 1전체 정보(종목리스트 포함), 2 : 조건 그룹 등록, 3 : 종목 등록    
	CString  m_szIcnt		;	// 그룹 수
	CString  m_szIopenDlg	;	// 0 : Open, 1 : No Open
	void*	 m_pIdata		;	// 그룹 데이터들(CPtrArray) 
	CString  m_szItitle		;

public:
	CINT_TOTAL()
	{
	}

	CINT_TOTAL(INT_TOTAL* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_TOTAL* pItem)
	{
		char sTmp[256+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Igubun);
		DATA_MEMCOPY(sTmp, pItem, Icnt);
//		DATA_MEMCOPY(sTmp, pItem, IopenDlg);
		DATA_MEMCOPY(sTmp, pItem, Ititle);

		m_pIdata = pItem->Idata;
	}
};

class CINT_GROUP_ST
{
public:
	CString  m_szIno	;	// 그룹 번호 
	CString  m_szItitle	;	// 그룹 명
	CString  m_szIcnt	;	// 그룹 안에 종목 수
	void*	 m_pIdata	;	// 종목 데이터들(CPtrArray)
	CString  m_szItime	;
	CString  m_szIdate	;

	int      m_nIcnt;
	
public:
	CINT_GROUP_ST()
	{
		m_nIcnt = 0;
	}

	CINT_GROUP_ST(INT_GROUP_ST* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_GROUP_ST* pItem)
	{
		char sTmp[21+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Ino);
		DATA_MEMCOPY(sTmp, pItem, Ititle);
		DATA_MEMCOPY(sTmp, pItem, Icnt);
//		DATA_MEMCOPY(sTmp, pItem, Itime);
//		DATA_MEMCOPY(sTmp, pItem, Idate);

		m_pIdata = pItem->Idata;

		m_nIcnt = atoi(m_szIcnt);
	}

	void LocalTRACE(LPCSTR szMsg)
	{
		TRACE("%s\n", szMsg);
//		TRACE("그룹 번호 [%s]\n", m_szIno);
//		TRACE("그룹 명   [%s]\n", m_szIcnt);
//		TRACE("시간      [%s]\n", m_szItime);
//		TRACE("날자      [%s]\n", m_szIdate);
		TRACE("--------------------------\n");
	}

};


class CINT_ITEM_ST
{
public:
	CString  m_szIcode;			// 코드  
	CString  m_szIgubun;		// 0:Kospi, 1:kosdoq,,,
	CString  m_szItime;			// HHMMSS   시,분,초
	CString  m_szIdate;			// YYYYMMDD 년,월,일
	CString  m_szIvol;			// 수량 	
	CString  m_szIval;			// 편입가격
	CString  m_szICheck;		// 체크유무
	void*    m_pIdata;
	CString  m_szIName;

public:
	CINT_ITEM_ST()
	{
	}

	CINT_ITEM_ST(INT_ITEM_ST* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_ITEM_ST* pItem)
	{
		char sTmp[30+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Icode);		// 코드  
		DATA_MEMCOPY(sTmp, pItem, Igubun);		// 0:Kospi, 1:kosdoq,,,
//		DATA_MEMCOPY(sTmp, pItem, Itime );		// HHMMSS   시,분,초
//		DATA_MEMCOPY(sTmp, pItem, Idate	);		// YYYYMMDD 년,월,일
//		DATA_MEMCOPY(sTmp, pItem, Ivol	);		// 수량 	
//		DATA_MEMCOPY(sTmp, pItem, Ival	);		// 편입가격
//		DATA_MEMCOPY(sTmp, pItem, ICheck);		// 체크유무
		DATA_MEMCOPY(sTmp, pItem, IName	);

		m_pIdata = pItem->Idata;
	}

	// TR88803에서의 룰대로 수정...'1':주식, '2':코스닥
	char GetMyGb()
	{
		if(m_szIgubun=="0") return '2';
		else if(m_szIgubun=="1") return '1'; 
		else return 'E';
	}

	void LocalTRACE(LPCSTR szMsg)
	{
		TRACE("%s\n", szMsg);
//
//		TRACE("코드 [%s]\n", m_szIcode);		// 코드  
//		TRACE("구분 [%s]\n", m_szIgubun);		// 0:Kospi, 1:kosdoq,,,
//		TRACE("시간 [%s]\n", m_szItime );		// HHMMSS   시,분,초
//		TRACE("날자 [%s]\n", m_szIdate	);		// YYYYMMDD 년,월,일
//		TRACE("수량 [%s]\n", m_szIvol	);		// 수량 	
//		TRACE("편입가격 [%s]\n", m_szIval	);	// 편입가격
//		TRACE("체크유무 [%s]\n", m_szICheck);	// 체크유무
//		TRACE("종목명 [%s]\n", m_szIName	);
		TRACE("--------------------------\n");
	}
};
#endif //_DEF_GWANSIM_DEFINE__HEADER__
