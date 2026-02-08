// 2000. 11.24 Junok Lee

#ifndef __LOCALUTIL__HEADER__
#define __LOCALUTIL__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocalUtil.h : header file
//
	
//#include "../../ToolKit/CodeUtil/codedef.h"
#define		CODELEN_F			8			// 선물코드 길이
#define		NAMELEN_F			30			// 선물종목명 길이

/////////////////////////////////////////////////////////////////////////////
// CCodeUtil window

class CCodeUtil
{
// Construction
public:
	CCodeUtil();

// Attributes
public:

// Operations
public:
	static int  Read_File(LPCSTR szFile, CStringList& list);
	static void GetFileName(LPCSTR szKey, CString& szName, LPCSTR _szINI);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeUtil)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeUtil();

};

class CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr) { return FALSE; }
};

//#define	ONESIZE_JMASTER	(1+1+18+6+7+3+1+1+3)
#define	ONESIZE_JMASTER	(1+1+18+6+7+3+1+1+3)
class CJMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szUnder;		// 1  관리/감리구분
	CString m_szWarn;		// 1  투자유의 구분
	CString m_szName;		// 18 종목명
	CString m_szCode;		// 6  종목코드
	CString m_szSymbol;		// 7  심벌
	CString m_szGubun;		// 1  업종구분
	CString m_szUpCode;		// 3  업종코드
	CString m_szS;			// 1  부구분
	CString m_szC;			// 1  대중소
	CString m_szG;			// 3  그룹코드
	CString m_manuf;		// 1  "1" 제조업
	CString m_szUpCode2;	// 3  금융업 관련 필드 추가
};

// 박성진 2002.09.15
// JMaster 에 그룹사 양식 추가.(파일을 JMaster 와 동일)
// 박성진 2002.09.18 => SK KMaster.dat 파일이 준비가 안됨
#define	ONESIZE_KMASTER	(1+1+18+7+7+1+3+3+3+1+1+3)
class CKMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szUnder;		// 1  관리/감리구분
	CString m_szWarn;		// 1  투자유의 구분
	CString m_szName;		// 18 종목명
	CString m_szCode;		// 7  종목코드
	CString m_szGubun;		// 1  구분
	CString	m_szUpCode;		// 3  업종코드
	
//	CString m_szG;			// 3  그룹코드
//	CString m_szVen;		// 1  벤처구분
	CString m_szUpCodem;	// 3  업종코드(중분류 020 ~ 049)
	CString m_szUpCodel;	// 3  업종코드(대분류 002 ~ 019)
	CString	m_szCapsize;	// 1  시가총액 '1':대 '2':중 '3':소 */
	                                /* '1':KOSDAQ100 '2':KOSDAQMID300 '3':KOSDAQSMALL */
//005 2004.01.29
	CString m_szVen;		// 1  벤처구분
	CString m_stargb;		// 1  스타지수'1'
	CString m_szG;			// 3  그룹코드	
};

#define	ONESIZE_UMASTER	(18+3)
class CUMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 업종명
	CString m_szGubun;		// 1 구분
	CString m_szCode;		// 3  업종코드
};

#define	ONESIZE_KUMASTER	(18+3)
class CKUMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 업종명
	CString m_szGubun;		// 1 구분
	CString m_szCode;		// 3  업종코드
};

#define ONESIZE_UPMASTER (18+3)
class CUPMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 업종명
	CString m_szGubun;		// 1 구분
	CString m_szCode;		// 3  업종코드
};

#define ONESIZE_UDMASTER (18+3)
class CUDMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 업종명
	CString m_szGubun;		// 1  업종구분
	CString m_szCode;		// 3  업종코드
};

// 제3시장
#define	ONESIZE_TMASTER	(40+6)
class CTMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 40 종목명
	CString m_szCode;		//   종목코드
};

#define	ONESIZE_GMASTER	(40+3)
class CGMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 40 그룹명
	CString	m_szGubun;		// 1  구분
	CString m_szCode;		// 3  그룹코드
};

// 선물
// #define	ONESIZE_FMASTER	(20+5)
#define	ONESIZE_FMASTER	(NAMELEN_F+CODELEN_F)
class CFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 30 그룹명
	CString m_szCode;		// 8  그룹코드
};

// 옵션
#define	ONESIZE_PMASTER	(14+8)
class CPMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 14 그룹명
	CString m_szCode;		// 8  그룹코드
};

// 테마
#define	ONESIZE_THMASTER	(40+3)
class CTHMaster : public CMasterBase
{
public:
 	virtual BOOL ReadData(LPCSTR _szStr);
 
 	CString m_szName;		// 40 그룹명
//	CString	m_szGubun;		// 1 구분
 	CString m_szCode;		// 2  테마코드
};

// * 결재년월
#define		ONESIZE_DMaster			(7)
class CDMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szDate;		// 7 결재년월
};

// 박성진 2002.06.27
// 국내, 국외를 구분하기 위해서
// * 거래원코드
#define		ONESIZE_SMaster		(20 + 3 + 1)
class CSMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 20 증권사명
	CString	m_szCode;		//  3 코드
	CString m_szGubun;		//  1 국내(0), 외국(1)
};

//  * 채권
#define		ONESIZE_CMaster			(45+9)
class CCMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	
	CString m_szName,		// 45 채권명
			m_szCode;		//  9 코드
};

// * 코스닥선물
#define		ONESIZE_KFMASTER	(30 + 11)
class CKFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;
	CString m_szCode;
};

// * 코스닥선물합성
#define		ONESIZE_KFTSMASTER	(15)
class CKFTSMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;
	CString m_szName;
};

/* 수익증권,신주인수권  Master Layout */
class CBMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)종목명
	CString m_szName;		// (9)종목코드
} ;
#define		ONESIZE_BMASTER	 sizeof(20+9)

/* 수익증권,신주인수권  Master Layout */
class CBEMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)종목명
	CString m_szName;		// (9)종목코드
} ;
#define		ONESIZE_BEMASTER	 sizeof(20+9)

#define		ONESIZE_BCMASTER	 sizeof(20+9)
/* 신주인수권증서  Master Layout */
class CBCMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)종목명
	CString m_szName;		// (9)종목코드
} ;

class CSTKOPT	: public CMasterBase
{
public:
	virtual BOOL ReadData(LPCTSTR _szStr);
	CString m_szName;		// 종목명
	CString m_szCode;		// 코드
	CString m_szATM;		 // ATM 가격
	CString m_szBaseCode;	// 기초자산 코드
};
#define		ONESIZE_STKOPT		37

// 주식옵션 결제월
#define	ONESIZE_STKDMASTER	6
class CSTKDMASTER : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szDate;		// 7 결재년월
};

// 박성진 2002.10.09
//* ETF(상장지수펀드)
#define		ONESIZE_ETFMASTER		(20+6+1)
class CETFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 20 코드명
	CString	m_szCode;		//  6 코드
	CString m_szGubun;		//  1 거래소1, 코스닥2
};

//#define ONESIZE_BMASTER (12 + 9)
//class CBMaster : public CMasterBase
//{
//public:
//	virtual BOOL ReadData(LPCSTR _szStr);
//	
//	CString m_szName;		//12 종목명
//	CString m_szCode;		// 9 종목코드
//};

///* 채권 Master Layout */
//typedef struct _CMASTER {
//    char    name    [45];     /* 채권명 */
//    char    code    [9];      /* 코드 */
//} CMASTER;
//#define		ONESIZE_CMASTER	 sizeof(CMASTER)
//
///* KOFEX Master Layout */
//typedef struct _KFMASTER {
//    char    name    [30];     /* 종목명 */
//    char    code    [15];      /* 코드 */
//} KFMASTER;
//#define		ONESIZE_KFMASTER	 sizeof(KFMASTER)
//
///* KOFEXTS Master Layout */
//typedef struct _KFTSMASTER {
//    char    code    [15];      /* 코드 */
//} KFTSMASTER;
//#define		ONESIZE_KFTSMASTER	 sizeof(KFTSMASTER)
//
///* 수익증권,신주인수권  Master Layout */
//typedef struct _BEMASTER {
//    char    name   [20];     /* 종목명 */
//    char    code    [9];     /* 종목코드 */
//} BEMASTER;
//#define		ONESIZE_BEMASTER	 sizeof(BEMASTER)

//소팅을 하기위한 클래스  2000.11.28 code by nykim
class CSortClass
{
public:
	CSortClass(CListCtrl * _pWnd, const int _iCol, const bool _bIsNumeric);
	virtual ~CSortClass();
	
	int iCol;
	CListCtrl * pWnd;
	bool bIsNumeric;
	void Sort(const bool bAsc);
	
	static int CALLBACK CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
	static int CALLBACK CompareAscI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
public:
	class CSortItem
	{
	public:
		virtual  ~CSortItem();
		CSortItem(const DWORD _dw, const CString &_txt);
		CString txt;
		DWORD dw;
	};
	class CSortItemInt
	{
	public:
		CSortItemInt(const DWORD _dw, const CString &_txt);
		int iInt ;
		DWORD dw;
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__LOCALUTIL__HEADER__)
