// STConfigEnv.h: interface for the CSTDataItem class.
// 전략환경설정에서 세팅하는 정보를 포함하는 구조체임.
//
// 사용할 만한 곳은 STConfigrueBasic, ChartSTIndexAddin, MultiChart, 주문관련된곳.
// 공통으로 사용하기 위해 class를 만듬.
// 이 파일을 수정하면 재컴파일 해줘야 하는 프로젝트
//		- STConfigDlg
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
#define AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_

// #include "../Common_ST/STConfigEnv.h"
#include "STConfigDef.h"
#include "../../inc/ICompanyMng.h"


class CSTConfigEnv_Jumun
{
public:
	char m_szAcctInfo[50+1];		// 계좌번호+계좌명
	int	m_nJAmount;					// 주문수량
	int m_nBuyPrice;				// 진입가격
	int m_nSellPrice;				// 청산가격 0:현재가,1~3매도호가
	int m_nJMethod;					// 주문방법 0:가상, 1:, 2:
	int m_nJTime;					// 주문시점
	int m_nJStartSignal;			// 주문시작신호 
	BOOL m_bSound_Buy_Use;			// 신호발생음_진입신호 사용여부
	CString  m_szSound_Buy_File;	// 신호발생음_진입신호 소리
	BOOL m_bSound_Sell_Use;			// 신호발생음_청산신호 사용여부
	CString  m_szSound_Sell_File;	// 신호발생음_청산신호 소리

	//증전 : KHD 
	int			m_nMkprc_Ent; // 진입가 시장가 여부
	int			m_nMkprc_Clr;// 청산가 시장가 여부
	int			m_nAcct;//적용계좌
	CString  m_szAcctName;//계좌명
	CString  m_szAcctNumber;//계좌번호
	CSTConfigEnv_Jumun*	m_pDefInfo;
public:
	CSTConfigEnv_Jumun()
	{
		memset(m_szAcctInfo, 0, 51);
		m_nJAmount			= 0;
		m_nBuyPrice			= 0;
		m_nSellPrice		= 0;
		m_nJMethod			= 0;
		m_nJTime			= 0;
		m_nJStartSignal		= 0;
		m_bSound_Buy_Use	= FALSE;
		m_bSound_Sell_Use	= FALSE;
		m_pDefInfo = NULL;
	}
	~CSTConfigEnv_Jumun()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Jumun* pTarget)
	{
		sprintf(m_szAcctInfo, "%s", pTarget->m_szAcctInfo);
		m_nJAmount			= pTarget->m_nJAmount;
		m_nBuyPrice			= pTarget->m_nBuyPrice;
		m_nSellPrice		= pTarget->m_nSellPrice;
		m_nJMethod			= pTarget->m_nJMethod;
		m_nJTime			= pTarget->m_nJTime;
		m_nJStartSignal		= pTarget->m_nJStartSignal;
		m_bSound_Buy_Use	= pTarget->m_bSound_Buy_Use;
		m_szSound_Buy_File	= pTarget->m_szSound_Buy_File;
		m_bSound_Sell_Use	= pTarget->m_bSound_Sell_Use;
		m_szSound_Sell_File	= pTarget->m_szSound_Sell_File;
	}

	void LoadInfo(LPCSTR szFileName);
};

class CSTConfigEnv_Signal
{
public:
	int		m_ShapeBuy;
	int		m_ShapeExitlong;
	int		m_ShapeSell;
	int		m_ShapeExitshort;
	CString m_ClrBuy;
	CString m_ClrExitlong;
	CString m_ClrSell;
	CString m_ClrExitshort;

	CSTConfigEnv_Signal* m_pDefInfo;

	CSTConfigEnv_Signal() : m_ShapeBuy(0), m_ShapeExitlong(0), m_ShapeSell(0), m_ShapeExitshort(0)
		,m_ClrBuy("255"), m_ClrExitlong("16711680"), m_ClrSell("16711680"), m_ClrExitshort("255")
	{
		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Signal()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Signal* pTarget)
	{
		m_ShapeExitlong		= pTarget->m_ShapeExitlong;
		m_ShapeSell			= pTarget->m_ShapeSell;
		m_ShapeExitshort	= pTarget->m_ShapeExitshort;
		m_ClrBuy			= pTarget->m_ClrBuy;
		m_ClrExitlong		= pTarget->m_ClrExitlong;
		m_ClrSell			= pTarget->m_ClrSell;
		m_ClrExitshort		= pTarget->m_ClrExitshort;
	}

	void LoadInfo(LPCSTR szFileName);
};


class CSTConfigEnv_Setup
{
public:
	int		m_ClearPoint;			// 강제청산 시점
	CString m_Chk1;					// 손절매(손실률)
	CString m_ED1;
	int		m_P1;					// 손절매(손실률) : P
	int		m_S1;					// 손절매(손실률) : S
	CString m_CLR_1;				// 손절매(손실률) : C

	CString m_Chk2;					// 목표수익
	CString m_ED2;
	int		m_P2;					// 목표수익 : P
	int		m_S2;					// 목표수익 : S
	CString m_CLR_2;				// 목표수익 : C

	CString m_Chk3;					// 최고가대비
	CString m_ED3;
	int		m_P3;					// 최고가대비 : P
	int		m_S3;					// 최고가대비 : S
	CString m_CLR_3;				// 최고가대비 : C

	CString m_Chk4;					// 목표가보존
	CString m_ED4;
	int		m_P4;					// 목표가보존 : P
	int		m_S4;					// 목표가보존 : S
	CString m_CLR_4;				// 목표가보존 : C

	CString m_Chk5;					// 최소가격
	CString m_ED5;
	int		m_P5;					// 최소가격 : P
	CString m_Bong;					// 봉

	CString m_Chk6;					// 당일청산
	int		m_TimeA;				// 오전/오후
	CString m_ED_Time;				// 시간

	CString m_Chk7;					// 이익보존
	CString m_ED6;
	CString m_ED7;
	int		m_P6;					// 이익보존(상승률) : P
	int		m_P7;					// 이익보존(하락률) : P
	int		m_S5;					// 이익보존 : S
	CString m_CLR_5;				// 이익보존 : C

	CSTConfigEnv_Setup* m_pDefInfo;

public:
	CSTConfigEnv_Setup()
	{
		m_ClearPoint = 0;
		//m_Chk1;		
		//m_ED1;
		m_P1		=0;	
		m_S1		=0;	
		m_CLR_1 = _T("16711680");

		//m_Chk2;		
		//m_ED2;
		m_P2		=0;	
		m_S2		=0;	
		m_CLR_2 = _T("16711680");

		//m_Chk3;		
		//m_ED3;
		m_P3		=0;	
		m_S3		=0;	
		m_CLR_3 = _T("16711680");

		//m_Chk4;		
		//m_ED4;
		m_P4		=0;	
		m_S4		=0;	
		m_CLR_4 = _T("16711680");

		//m_Chk5;		
		//m_ED5;
		m_P5		=0;	
		//m_Bong;		

		//m_Chk6;
		m_TimeA		=0;
		//m_ED_Time;

		//m_Chk7;		
		//m_ED6;
		//m_ED7;
		m_P6		=0;	
		m_P7		=0;	
		m_S5		=0;	
		m_CLR_5 = _T("16711680");

		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Setup()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Setup* pTarget)
	{
		m_ClearPoint= pTarget->m_ClearPoint;
		m_Chk1		= pTarget->m_Chk1;		
		m_ED1		= pTarget->m_ED1;
		m_P1		= pTarget->m_P1;		
		m_S1		= pTarget->m_S1;		
		m_CLR_1		= pTarget->m_CLR_1;	

		m_Chk2		= pTarget->m_Chk2;		
		m_ED2		= pTarget->m_ED2;
		m_P2		= pTarget->m_P2;		
		m_S2		= pTarget->m_S2;		
		m_CLR_2		= pTarget->m_CLR_2;	

		m_Chk3		= pTarget->m_Chk3;		
		m_ED3		= pTarget->m_ED3;
		m_P3		= pTarget->m_P3;		
		m_S3		= pTarget->m_S3;		
		m_CLR_3		= pTarget->m_CLR_3;	

		m_Chk4		= pTarget->m_Chk4;		
		m_ED4		= pTarget->m_ED4;
		m_P4		= pTarget->m_P4;		
		m_S4		= pTarget->m_S4;		
		m_CLR_4		= pTarget->m_CLR_4;	

		m_Chk5		= pTarget->m_Chk5;		
		m_ED5		= pTarget->m_ED5;
		m_P5		= pTarget->m_P5;		
		m_Bong		= pTarget->m_Bong;		

		m_Chk6		= pTarget->m_Chk6;		
		m_TimeA		= pTarget->m_TimeA;	
		m_ED_Time	= pTarget->m_ED_Time;

		m_Chk7		= pTarget->m_Chk7;		
		m_ED6		= pTarget->m_ED6;
		m_ED7		= pTarget->m_ED7;
		m_P6		= pTarget->m_P6;		
		m_P7		= pTarget->m_P7;		
		m_S5		= pTarget->m_S5;		
		m_CLR_5		= pTarget->m_CLR_5;	
	}

	void LoadInfo(LPCSTR szFileName);
};

class CSTConfigEnv_Clear
{
public:	
	int	m_Allow;					// 허용포지션
	int m_Repeat;					// 중복진입 여부
	int m_JCount;					// 중복진입 허용회수
	CString m_TaxBuy;				//거래비용_수수료+세금_진입
	CString m_TaxSell;				//거래비용_수수료+세금_청산
	int m_cbTax;					//거래비용_수수료+세금_POINT
	CString m_SlipigyBuy;			//거래비용_슬리피지_진입
	CString m_SlipigySell;			//거래비용_슬리피지_청산
	int m_cbSplipigy;				//거래비용_슬리피지_POINT

	CSTConfigEnv_Clear* m_pDefInfo;

	CSTConfigEnv_Clear() : m_Allow(0), m_Repeat(1), m_JCount(0)
		,m_TaxBuy("0.1"), m_TaxSell("0.4"), m_cbTax(0)
		,m_SlipigyBuy(""), m_SlipigySell(""), m_cbSplipigy(0) 
	{
		m_pDefInfo = NULL;
	}

	~CSTConfigEnv_Clear()
	{
		if(m_pDefInfo) delete m_pDefInfo; m_pDefInfo=NULL;
	}

	void Copy(CSTConfigEnv_Clear* pTarget)
	{
		m_Allow			=pTarget->m_Allow;
		m_Repeat		=pTarget->m_Repeat;
		m_JCount		=pTarget->m_JCount;	
		m_TaxBuy		=pTarget->m_TaxBuy;	
		m_TaxSell		=pTarget->m_TaxSell;	
		m_cbTax			=pTarget->m_cbTax;	
		m_SlipigyBuy	=pTarget->m_SlipigyBuy;	
		m_SlipigySell	=pTarget->m_SlipigySell;	
		m_cbSplipigy	=pTarget->m_cbSplipigy;
	}

	void LoadInfo(LPCSTR szFileName);
};

#define STConfigEnv_Jumun		0x0001
#define STConfigEnv_Signal		0x0002
#define STConfigEnv_Setup		0x0004
#define STConfigEnv_Clear		0x0008
#define STConfigEnv_All			(STConfigEnv_Jumun|STConfigEnv_Signal|STConfigEnv_Setup|STConfigEnv_Clear)
class CSTConfigEnv
{
public:
	CSTConfigEnv_Jumun	m_InfoJumun;
	CSTConfigEnv_Signal	m_InfoSignal;
	CSTConfigEnv_Setup	m_InfoSetup;
	CSTConfigEnv_Clear	m_InfoClear;

// ST파일에서 얻을 수 없는 정보들
	CString m_szSTFileName, m_szSTName;
	CString m_szCode, m_szCodeName;
	int		m_nCodeType;	//MultiChart의 CTabSystemTradingEx::CSTAnalysisInput::GetCodeType()에서 넘겨받는 값.
							// 0:주식, 1:선옵, 2:업종
	int		m_nOrderType;				// VALENTRYSELL 매수진입, VALEXITLONG 매수청산, VALEXITSHORT 매도청산, VALENTRYBUY 매도진입

public:
// Implementation
	CSTConfigEnv()
	{
		m_nCodeType		= 0;
		m_nOrderType	= -1;
	}

	void LoadInfo(LPCSTR szFileName, int nKey=STConfigEnv_Jumun);
	void LoadInfo(LPCSTR szFileName, LPCSTR szDefFileName, int nKey=STConfigEnv_Jumun);

	void Copy(CSTConfigEnv* pTarget)	
	{
		m_szSTFileName	= pTarget->m_szSTFileName;
		m_szSTName		= pTarget->m_szSTName;
		m_szCode		= pTarget->m_szCode;
		m_szCodeName	= pTarget->m_szCodeName;	
		m_nCodeType		= pTarget->m_nCodeType;
		m_nOrderType	= pTarget->m_nOrderType;
		m_InfoJumun.Copy(&pTarget->m_InfoJumun);
	}

	BOOL GetAcctCode(CString& rString);
	BOOL GetAcctName(CString& rString);


//	CSTConfigEnv& operator=(const CSTConfigEnv& src) const
//	{
//	}
};

//*****************************************************************************
// 함  수  명  : inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, LPCSTR szKey)
// 내      용  : 계좌정보는 따로 로딩하지 않는다.
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-15 오전 9:48:39
//*****************************************************************************
inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, int nKey)
{
	if(nKey&STConfigEnv_Jumun)		m_InfoJumun.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Signal)		m_InfoSignal.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Setup)		m_InfoSetup.LoadInfo(szFileName);
	if(nKey&STConfigEnv_Clear)		m_InfoClear.LoadInfo(szFileName);
}

inline void CSTConfigEnv::LoadInfo(LPCSTR szFileName, LPCSTR szDefFileName, int nKey)
{
	if(nKey&STConfigEnv_Jumun)
	{
		m_InfoJumun.m_pDefInfo = new CSTConfigEnv_Jumun;
		m_InfoJumun.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoJumun.LoadInfo(szFileName);
		delete m_InfoJumun.m_pDefInfo;
		m_InfoJumun.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Signal)
	{
		m_InfoSignal.m_pDefInfo = new CSTConfigEnv_Signal;
		m_InfoSignal.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoSignal.LoadInfo(szFileName);
		delete m_InfoSignal.m_pDefInfo;
		m_InfoSignal.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Setup)
	{
		m_InfoSetup.m_pDefInfo = new CSTConfigEnv_Setup;
		m_InfoSetup.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoSetup.LoadInfo(szFileName);
		delete m_InfoSetup.m_pDefInfo;
		m_InfoSetup.m_pDefInfo = NULL;
	}

	if(nKey&STConfigEnv_Clear)
	{
		m_InfoClear.m_pDefInfo = new CSTConfigEnv_Clear;
		m_InfoClear.m_pDefInfo->LoadInfo(szDefFileName);
		m_InfoClear.LoadInfo(szFileName);
		delete m_InfoClear.m_pDefInfo;
		m_InfoClear.m_pDefInfo = NULL;
	}
}

inline BOOL CSTConfigEnv::GetAcctCode(CString& rString)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		rString = m_InfoJumun.m_szAcctInfo;
	}
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
		rString.Format("%d", m_InfoJumun.m_nAcct);
	}

	if(rString.GetLength()>10)
	{
		rString = rString.Left(11);
		return TRUE;
	}

	rString.Empty();
	return FALSE;
}

inline BOOL CSTConfigEnv::GetAcctName(CString& rString)
{
	rString = m_InfoJumun.m_szAcctInfo;
	if(rString.GetLength()>10)
	{
		rString = rString.Mid(12);
		rString.TrimLeft();
		rString.TrimRight();
		return TRUE;
	}

	rString.Empty();
	return FALSE;
}


//*****************************************************************************
// 함  수  명  : inline void CSTConfigEnv_Jumun::LoadInfo(LPCSTR szFileName)
// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-14 오후 8:52:57
//*****************************************************************************
inline void CSTConfigEnv_Jumun::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		//계좌정보
		
		if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		{
			szKey.Format("%s", "AcctInfo");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szAcctInfo, szTmp, MAX_PATH, szFileName);
			sprintf(m_szAcctInfo, "%s", szTmp);	// 주의! 최대 50을 넘으면 안됨.

		}

		else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		{
		//적용계좌
			szKey.Format("%s", "Acct");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			m_szAcctNumber = szTmp;
			m_nAcct = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nAcct, szFileName);

		//계좌명
			szKey.Format("%s", "AcctName");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szAcctName, szTmp, MAX_PATH, szFileName);
			m_szAcctName = szTmp;

			// 진입가 시장가 여부
			szKey.Format("%s", "MKPRC_ENT");
			m_nMkprc_Ent = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nMkprc_Ent, szFileName);

			// 청산가 시장가 여부
			szKey.Format("%s", "MKPRC_CLR");
			m_nMkprc_Clr = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nMkprc_Clr, szFileName);
		}
		//주문수량
		szKey.Format("%s", "JAmount");
		m_nJAmount = GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJAmount, szFileName);

		//진입가격
		szKey.Format("%s", "BuyPrice");
		m_nBuyPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nBuyPrice, szFileName);
		if(m_nBuyPrice<0) m_nBuyPrice=0;

		//청산가격
		szKey.Format("%s", "SellPrice");
		m_nSellPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nSellPrice, szFileName);
		if(m_nSellPrice<0) m_nSellPrice=0;

		//주문방식
		szKey.Format("%s", "JMethod");
		m_nJMethod = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJMethod, szFileName);
		if(m_nJMethod<0) m_nJMethod=0;

		//주문시점
		szKey.Format("%s", "JTime");
		m_nJTime = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJTime, szFileName);
		if(m_nJTime<0) m_nJTime=0;

		//주문시작신호
		szKey.Format("%s", "JStartSignal");
		m_nJStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_nJStartSignal, szFileName);
		if(m_nJStartSignal<0) m_nJStartSignal=0;

		//신호발생음_진입신호 사용여부
		szKey.Format("%s", "Sound_Buy_Use");
		m_bSound_Buy_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_bSound_Buy_Use, szFileName);

		//신호발생음_진입신호 소리
		szKey.Format("%s", "Sound_Buy_File");
		GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szSound_Buy_File, szTmp, MAX_PATH, szFileName);
		m_szSound_Buy_File = szTmp;

		//신호발생음_청산신호 사용여부
		szKey.Format("%s", "Sound_Sell_Use");
		m_bSound_Sell_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, m_pDefInfo->m_bSound_Sell_Use, szFileName);

		//신호발생음_청산신호 소리
		szKey.Format("%s", "Sound_Sell_File");
		GetPrivateProfileString(STCONFIG_ORDER, szKey, m_pDefInfo->m_szSound_Sell_File, szTmp, MAX_PATH, szFileName);
		m_szSound_Sell_File = szTmp;


	}
	else
	{
		if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		{
			//계좌정보
			szKey.Format("%s", "AcctInfo");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			sprintf(m_szAcctInfo, "%s", szTmp);	// 주의! 최대 50을 넘으면 안됨.
		}
		else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		{
		//적용계좌
			szKey.Format("%s", "Acct");
			m_nAcct = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
			if(m_nAcct<0) m_nAcct=0;
			
		//계좌명
			szKey.Format("%s", "AcctName");
			GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szFileName);
			m_szAcctName = szTmp;


			// 진입가 시장가 여부
			szKey.Format("%s", "MKPRC_ENT");
			m_nMkprc_Ent = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

			// 청산가 시장가 여부
			szKey.Format("%s", "MKPRC_CLR");
			m_nMkprc_Clr = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		}
		//주문수량
		szKey.Format("%s", "JAmount");
		m_nJAmount = GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//진입가격
		szKey.Format("%s", "BuyPrice");
		m_nBuyPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nBuyPrice<0) m_nBuyPrice=0;

		//청산가격
		szKey.Format("%s", "SellPrice");
		m_nSellPrice = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nSellPrice<0) m_nSellPrice=0;

		//주문방식
		szKey.Format("%s", "JMethod");
		m_nJMethod = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJMethod<0) m_nJMethod=0;

		//주문시점
		szKey.Format("%s", "JTime");
		m_nJTime = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJTime<0) m_nJTime=0;

		//주문시작신호
		szKey.Format("%s", "JStartSignal");
		m_nJStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);
		if(m_nJStartSignal<0) m_nJStartSignal=0;
		//KHD : 수정 
		char szWindowsPath[MAX_PATH];
		GetWindowsDirectory(szWindowsPath, MAX_PATH-1);

		CString szMediaPath;
		szMediaPath.Format(_T("%s\\Media\\"), szWindowsPath);

		CString szDefSoundPath;
		//신호발생음_진입신호 사용여부
		szKey.Format("%s", "Sound_Buy_Use");
		m_bSound_Buy_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//신호발생음_진입신호 소리
		szKey.Format("%s", "Sound_Buy_File");
		szDefSoundPath = szMediaPath + "ding.wav";
		GetPrivateProfileString(STCONFIG_ORDER, szKey, szDefSoundPath, szTmp, MAX_PATH, szFileName);
		m_szSound_Buy_File = szTmp;

		//신호발생음_청산신호 사용여부
		szKey.Format("%s", "Sound_Sell_Use");
		m_bSound_Sell_Use = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szFileName);

		//신호발생음_청산신호 소리
		szKey.Format("%s", "Sound_Sell_File");
		szDefSoundPath = szMediaPath + "chord.wav";
		GetPrivateProfileString(STCONFIG_ORDER, szKey, szDefSoundPath, szTmp, MAX_PATH, szFileName);
		m_szSound_Sell_File = szTmp;//Modify End

	}
}


//*****************************************************************************
// 함  수  명  : inline void CSTConfigEnv_Signal::LoadInfo(LPCSTR szFileName)

// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-14 오후 8:52:59
//*****************************************************************************
inline void CSTConfigEnv_Signal::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	COLORREF clrSignal = RGB(0,0,0);

	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// 신호정보를 읽는다. Start...
		int nSel = 0;
		// szKey.Format("CB_%s", TRS_ST_BUY);			// 매수
		szKey = SHAPEBUY;
		m_ShapeBuy = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeBuy, szFileName);
		if(m_ShapeBuy<0) m_ShapeBuy=0;

		//szKey.Format("CB_%s", TRS_ST_EXITLONG);		// 매수청산
		szKey = SHAPEEXITLONG;
		m_ShapeExitlong = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeExitlong, szFileName);
		if(m_ShapeExitlong<0) m_ShapeExitlong=0;

		//szKey.Format("CB_%s", TRS_ST_SELL);			// 매도
		szKey = SHAPESELL;
		m_ShapeSell = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeSell, szFileName);
		if(m_ShapeSell<0) m_ShapeSell=0;

		//szKey.Format("CB_%s", TRS_ST_EXITSHORT);	// 매도청산
		szKey = SHAPEEXITSHORT;
		m_ShapeExitshort = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ShapeExitshort, szFileName);
		if(m_ShapeExitshort<0) m_ShapeExitshort=0;
		// 신호정보를 읽는다. End


		// 색상 정보를 읽어온다. Start...
		//szKey.Format("CLR_%s", TRS_ST_BUY);			// 매수
		szKey = COLORBUY;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrBuy, szTmp, MAX_PATH, szFileName);
		m_ClrBuy = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_EXITLONG);		// 매수청산
		szKey = COLOREXITLONG;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrExitlong, szTmp, MAX_PATH, szFileName);
		m_ClrExitlong = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_SELL);			// 매도
		szKey = COLORSELL;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrSell, szTmp, MAX_PATH, szFileName);
		m_ClrSell = szTmp;

		// szKey.Format("CLR_%s", TRS_ST_EXITSHORT);	// 매도청산
		szKey = COLOREXITSHORT;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, m_pDefInfo->m_ClrExitshort, szTmp, MAX_PATH, szFileName);
		m_ClrExitshort = szTmp;
		// 색상 정보를 읽어온다. End
	}
	else
	{
		// 신호정보를 읽는다. Start...
		int nSel = 0;
		// szKey.Format("CB_%s", TRS_ST_BUY);			// 매수
		szKey = SHAPEBUY;
		m_ShapeBuy = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeBuy<0) m_ShapeBuy=0;

		//szKey.Format("CB_%s", TRS_ST_EXITLONG);		// 매수청산
		szKey = SHAPEEXITLONG;
		m_ShapeExitlong = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeExitlong<0) m_ShapeExitlong=0;

		//szKey.Format("CB_%s", TRS_ST_SELL);			// 매도
		szKey = SHAPESELL;
		m_ShapeSell = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeSell<0) m_ShapeSell=0;

		//szKey.Format("CB_%s", TRS_ST_EXITSHORT);	// 매도청산
		szKey = SHAPEEXITSHORT;
		m_ShapeExitshort = ::GetPrivateProfileInt(STCONFIG_SIGNAL, szKey, 0, szFileName);
		if(m_ShapeExitshort<0) m_ShapeExitshort=0;
		// 신호정보를 읽는다. End


		// 색상 정보를 읽어온다. Start...
		//szKey.Format("CLR_%s", TRS_ST_BUY);			// 매수
		szKey = COLORBUY;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "255", szTmp, MAX_PATH, szFileName);
		m_ClrBuy = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_EXITLONG);		// 매수청산
		szKey = COLOREXITLONG;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_ClrExitlong = szTmp;

		//szKey.Format("CLR_%s", TRS_ST_SELL);			// 매도
		szKey = COLORSELL;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_ClrSell = szTmp;

		// szKey.Format("CLR_%s", TRS_ST_EXITSHORT);	// 매도청산
		szKey = COLOREXITSHORT;
		GetPrivateProfileString(STCONFIG_SIGNAL, szKey, "255", szTmp, MAX_PATH, szFileName);
		m_ClrExitshort = szTmp;
		// 색상 정보를 읽어온다. End
	}
}


//*****************************************************************************
// 함  수  명  : inline void CSTConfigEnv_Setup::LoadInfo(LPCSTR szFileName)

// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-14 오후 8:53:02
//*****************************************************************************
inline void CSTConfigEnv_Setup::LoadInfo(LPCSTR szFileName)
{
	CString szKey;

	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// 강제청산 시점
		szKey.Format("%s", "ClearPoint");
		m_ClearPoint = ::GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ClearPoint, szFileName);
		if(m_ClearPoint<0 || m_ClearPoint>1) m_ClearPoint=0;

		//-------- 1 -----------------------------
		//손절매(손실률)
		szKey.Format("%s", "Chk1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk1, szTmp, MAX_PATH, szFileName);
		m_Chk1 = szTmp;

		szKey.Format("%s", "ED1");	
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED1, szTmp, MAX_PATH, szFileName);
		m_ED1 = szTmp;

		// 손절매(손실률) : P
		szKey.Format("%s", "P1");
		m_P1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P1, szFileName);
		if(m_P1 < 0 ) m_P1 = 0;

		// 손절매(손실률) : S
		szKey.Format("%s", "S1");
		m_S1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S1, szFileName);
		if(m_S1 < 0 ) m_S1 = 0;

		// 손절매(손실률) : C
		szKey.Format("CLR_%s", "1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_1, szTmp, MAX_PATH, szFileName);
		m_CLR_1 = szTmp;

		//------- 2 ------------------------------
		// 목표수익	
		szKey.Format("%s", "Chk2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk2, szTmp, MAX_PATH, szFileName);
		m_Chk2 = szTmp;

		szKey.Format("%s", "ED2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED2, szTmp, MAX_PATH, szFileName);
		m_ED2= szTmp;

		// 목표수익 : P
		szKey.Format("%s", "P2");
		m_P2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P2, szFileName);
		if(m_P2 < 0 ) m_P2 = 0;

		// 목표수익 : S
		szKey.Format("%s", "S2");
		m_S2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S2, szFileName);
		if(m_S2 < 0 ) m_S2 = 0;

		// 목표수익 : C
		szKey.Format("CLR_%s", "2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_2, szTmp, MAX_PATH, szFileName);
		m_CLR_2 = szTmp;

		//----------- 3 --------------------------
		// 최고가대비 하락
		szKey.Format("%s", "Chk3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk3, szTmp, MAX_PATH, szFileName);
		m_Chk3 = szTmp;

		szKey.Format("%s", "ED3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED3, szTmp, MAX_PATH, szFileName);
		m_ED3 = szTmp;

		// 최고가대비 하락 : P
		szKey.Format("%s", "P3");
		m_P3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P3, szFileName);
		if(m_P3 < 0 ) m_P3 = 0;

		// 최고가대비 하락 : S
		szKey.Format("%s", "S3");
		m_S3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S3, szFileName);
		if(m_S3 < 0 ) m_S3 = 0;

		// 최고가대비 하락 : C
		szKey.Format("CLR_%s", "3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_3, szTmp, MAX_PATH, szFileName);
		m_CLR_3 = szTmp;

		//----------- 4 --------------------------
		// 목표가보존 청산
		szKey.Format("%s", "Chk4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk4, szTmp, MAX_PATH, szFileName);
		m_Chk4 = szTmp;

		szKey.Format("%s", "ED4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED4, szTmp, MAX_PATH, szFileName);
		m_ED4 = szTmp;

		// 목표가보존 청산 : P
		szKey.Format("%s", "P4");
		m_P4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P4, szFileName);
		if(m_P4 < 0 ) m_P4 = 0;

		// 목표가보존 청산 : S
		szKey.Format("%s", "S4");
		m_S4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S4, szFileName);
		if(m_S4 < 0 ) m_S4 = 0;

		// 목표가보존 청산 : C
		szKey.Format("CLR_%s", "4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_4, szTmp, MAX_PATH, szFileName);
		m_CLR_4 = szTmp;

		//----------- 5 --------------------------
		//최소가격변화
		szKey.Format("%s", "Chk5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk5, szTmp, MAX_PATH, szFileName);
		m_Chk5 = szTmp;

		szKey.Format("%s", "ED5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED5, szTmp, MAX_PATH, szFileName);
		m_ED5 = szTmp;

		// 최소가격변화 : P
		szKey.Format("%s", "P5");
		m_P5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P5, szFileName);
		if(m_P5 < 0 ) m_P5 = 0;

		// 최소가격변화 : S

		// 봉
		szKey.Format("%s", "Bong");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Bong, szTmp, MAX_PATH, szFileName);
		m_Bong = szTmp;


		//----------- 6 --------------------------
		// 당일청산
		szKey.Format("%s", "Chk6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk6, szTmp, MAX_PATH, szFileName);
		m_Chk6 = szTmp;


		// 오전/오후
		szKey.Format("%s", "TimeA");
		m_TimeA = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_TimeA, szFileName);
		if(m_TimeA < 0 ) m_TimeA = 1;

		// 시간
		szKey.Format("%s", "ED_Time");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED_Time, szTmp, MAX_PATH, szFileName);
		m_ED_Time = szTmp;

		//----------- 7 --------------------------
		// 이익보존 청산
		szKey.Format("%s", "Chk7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_Chk7, szTmp, MAX_PATH, szFileName);
		m_Chk7 = szTmp;

		szKey.Format("%s", "ED6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED6, szTmp, MAX_PATH, szFileName);
		m_ED6 = szTmp;

		szKey.Format("%s", "ED7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_ED7, szTmp, MAX_PATH, szFileName);
		m_ED7 = szTmp;

		// 이익보존 청산 : P
		szKey.Format("%s", "P6");
		m_P6 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P6, szFileName);
		if(m_P6 < 0 ) m_P6 = 0;

		// 이익보존 청산 : P
		szKey.Format("%s", "P7");
		m_P7 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_P7, szFileName);
		if(m_P7 < 0 ) m_P7 = 0;

		// 이익보존 청산 : S
		szKey.Format("%s", "S5");
		m_S5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, m_pDefInfo->m_S5, szFileName);
		if(m_S5 < 0 ) m_S5 = 0;

		// 이익보존 청산 : C
		szKey.Format("CLR_%s", "5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, m_pDefInfo->m_CLR_5, szTmp, MAX_PATH, szFileName);
		m_CLR_5 = szTmp;
	}
	else
	{
		// 강제청산 시점
		szKey.Format("%s", "ClearPoint");
		m_ClearPoint = ::GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_ClearPoint<0 || m_ClearPoint>1) m_ClearPoint=0;

		//-------- 1 -----------------------------
		// 손절매(손실률)
		szKey.Format("%s", "Chk1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk1 = szTmp;

		szKey.Format("%s", "ED1");	
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "10", szTmp, MAX_PATH, szFileName);
		m_ED1 = szTmp;

		// 손절매(손실률) : P
		szKey.Format("%s", "P1");
		m_P1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P1 < 0 ) m_P1 = 0;

		// 손절매(손실률) : S
		szKey.Format("%s", "S1");
		m_S1 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_S1 < 0 ) m_S1 = 0;

		// 손절매(손실률) : C
		szKey.Format("CLR_%s", "1");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_1 = szTmp;

		//------- 2 ------------------------------
		// 목표수익	
		szKey.Format("%s", "Chk2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk2 = szTmp;

		szKey.Format("%s", "ED2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "20", szTmp, MAX_PATH, szFileName);
		m_ED2= szTmp;

		// 목표수익 : P
		szKey.Format("%s", "P2");
		m_P2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P2 < 0 ) m_P2 = 0;

		// 목표수익 : S
		szKey.Format("%s", "S2");
		m_S2 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 2, szFileName);
		if(m_S2 < 0 ) m_S2 = 0;

		// 목표수익 : C
		szKey.Format("CLR_%s", "2");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_2 = szTmp;

		//----------- 3 --------------------------
		// 최고가대비 하락
		szKey.Format("%s", "Chk3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk3 = szTmp;

		szKey.Format("%s", "ED3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "5", szTmp, MAX_PATH, szFileName);
		m_ED3 = szTmp;

		// 최고가대비 하락 : P
		szKey.Format("%s", "P3");
		m_P3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P3 < 0 ) m_P3 = 0;

		// 최고가대비 하락 : S
		szKey.Format("%s", "S3");
		m_S3 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 4, szFileName);
		if(m_S3 < 0 ) m_S3 = 0;

		// 최고가대비 하락 : C
		szKey.Format("CLR_%s", "3");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_3 = szTmp;

		//----------- 4 --------------------------
		// 목표가보존 상승
		szKey.Format("%s", "Chk4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk4 = szTmp;

		szKey.Format("%s", "ED4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "30", szTmp, MAX_PATH, szFileName);
		m_ED4 = szTmp;

		// 목표가보존 상승 : P
		szKey.Format("%s", "P4");
		m_P4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P4 < 0 ) m_P4 = 0;

		// 목표가보존 상승 : S
		szKey.Format("%s", "S4");
		m_S4 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 5, szFileName);
		if(m_S4 < 0 ) m_S4 = 0;

		// 목표가보존 상승 : C
		szKey.Format("CLR_%s", "4");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_4 = szTmp;

		//----------- 5 --------------------------
		// 최소가격변화
		szKey.Format("%s", "Chk5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk5 = szTmp;

		szKey.Format("%s", "ED5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "2", szTmp, MAX_PATH, szFileName);
		m_ED5 = szTmp;

		// 최소가격변화 : P
		szKey.Format("%s", "P5");
		m_P5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P5 < 0 ) m_P5 = 0;

		// 최소가격변화 : S

		// 봉
		szKey.Format("%s", "Bong");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "5", szTmp, MAX_PATH, szFileName);
		m_Bong = szTmp;


		//----------- 6 --------------------------
		//당일청산
		szKey.Format("%s", "Chk6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk6 = szTmp;


		// 오전/오후
		szKey.Format("%s", "TimeA");
		m_TimeA = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 1, szFileName);
		if(m_TimeA < 0 ) m_TimeA = 0;

		// 시간
		szKey.Format("%s", "ED_Time");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "02:00", szTmp, MAX_PATH, szFileName);
		m_ED_Time = szTmp;

		//----------- 7 --------------------------
		// 이익보존
		szKey.Format("%s", "Chk7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "", szTmp, MAX_PATH, szFileName);
		m_Chk7 = szTmp;

		szKey.Format("%s", "ED6");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "20", szTmp, MAX_PATH, szFileName);
		m_ED6 = szTmp;

		szKey.Format("%s", "ED7");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "10", szTmp, MAX_PATH, szFileName);
		m_ED7 = szTmp;

		// 이익보존 : P
		szKey.Format("%s", "P6");
		m_P6 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P6 < 0 ) m_P6 = 0;

		// 이익보존 : P
		szKey.Format("%s", "P7");
		m_P7 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szFileName);
		if(m_P7 < 0 ) m_P7 = 0;

		// 이익보존 : S
		szKey.Format("%s", "S5");
		m_S5 = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 6, szFileName);
		if(m_S5 < 0 ) m_S5 = 0;

		// 이익보존 : C
		szKey.Format("CLR_%s", "5");
		GetPrivateProfileString(STCONFIG_CLEAR, szKey, "16711680", szTmp, MAX_PATH, szFileName);
		m_CLR_5 = szTmp;
	}
}


//*****************************************************************************
// 함  수  명  : inline void CSTConfigEnv_Clear::LoadInfo(LPCSTR szFileName)

// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-14 오후 8:53:04
//*****************************************************************************
inline void CSTConfigEnv_Clear::LoadInfo(LPCSTR szFileName)
{
	CString szKey;
	char szTmp[MAX_PATH];

	if(m_pDefInfo)
	{
		// 허용포지션
		szKey.Format("%s", "Allow");
		m_Allow = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_Allow, szFileName);
		if(m_Allow<0) m_Allow=0;

		//-----------------------------------------------------------------------------------------------------
		// 중복진입 여부
		szKey.Format("%s", "Repeat");
		m_Repeat = ::GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_Repeat, szFileName);
		if(m_Repeat<0 || m_Repeat>1) m_Repeat=1;

		// 중복진입 허용회수
		szKey.Format("%s", "JCount");
		m_JCount = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_JCount, szFileName);
		//-----------------------------------------------------------------------------------------------------

		//거래비용_수수료+세금_진입
		szKey.Format("%s", "TaxBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_TaxBuy, szTmp, MAX_PATH, szFileName);
		m_TaxBuy = szTmp;

		//거래비용_수수료+세금_청산
		szKey.Format("%s", "TaxSell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_TaxSell, szTmp, MAX_PATH, szFileName);
		m_TaxSell = szTmp;

		//거래비용_수수료+세금_POINT
		szKey.Format("%s", "cbTax");
		m_cbTax = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_cbTax, szFileName);
		if(m_cbTax<0) m_cbTax=0;

		//거래비용_슬리피지_진입
		szKey.Format("%s", "SlipigyBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_SlipigyBuy, szTmp, MAX_PATH, szFileName);
		m_SlipigyBuy = szTmp;

		//거래비용_슬리피지_청산
		szKey.Format("%s", "SlipigySell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, m_pDefInfo->m_SlipigySell, szTmp, MAX_PATH, szFileName);
		m_SlipigySell = szTmp;

		//거래비용_슬리피지_POINT
		szKey.Format("%s", "cbSplipigy");
		m_cbSplipigy = GetPrivateProfileInt(STCONFIG_SETUP, szKey, m_pDefInfo->m_cbSplipigy, szFileName);
		if(m_cbSplipigy<0) m_cbSplipigy=0;
	}
	else
	{
		// 허용포지션
		szKey.Format("%s", "Allow");
		m_Allow = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_Allow<0) m_Allow=0;

		//-----------------------------------------------------------------------------------------------------
		// 중복진입 여부
		szKey.Format("%s", "Repeat");
		m_Repeat = ::GetPrivateProfileInt(STCONFIG_SETUP, szKey, 1, szFileName);
		if(m_Repeat<0 || m_Repeat>1) m_Repeat=1;

		// 중복진입 허용회수
		szKey.Format("%s", "JCount");
		m_JCount = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		//-----------------------------------------------------------------------------------------------------

		//거래비용_수수료+세금_진입
		szKey.Format("%s", "TaxBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "0.1", szTmp, MAX_PATH, szFileName);
		m_TaxBuy = szTmp;

		//거래비용_수수료+세금_청산
		szKey.Format("%s", "TaxSell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "0.4", szTmp, MAX_PATH, szFileName);
		m_TaxSell = szTmp;

		//거래비용_수수료+세금_POINT
		szKey.Format("%s", "cbTax");
		m_cbTax = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_cbTax<0) m_cbTax=0;

		//거래비용_슬리피지_진입
		szKey.Format("%s", "SlipigyBuy");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "", szTmp, MAX_PATH, szFileName);
		m_SlipigyBuy = szTmp;

		//거래비용_슬리피지_청산
		szKey.Format("%s", "SlipigySell");
		GetPrivateProfileString(STCONFIG_SETUP, szKey, "", szTmp, MAX_PATH, szFileName);
		m_SlipigySell = szTmp;

		//거래비용_슬리피지_POINT
		szKey.Format("%s", "cbSplipigy");
		m_cbSplipigy = GetPrivateProfileInt(STCONFIG_SETUP, szKey, 0, szFileName);
		if(m_cbSplipigy<0) m_cbSplipigy=0;
	}	
}

#endif // !defined(AFX_STCONFIGENV__4D45_4BD3_44DC_9BC1_2F5D345896A4__INCLUDED_)
