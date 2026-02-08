// PartnerMng.h : main header file for the PARTNERMNG DLL
//

#if !defined(AFX_PARTNERMNG_H__CC4107AE_2C8E_462B_8DED_92B6D8B30D9A__INCLUDED_)
#define AFX_PARTNERMNG_H__CC4107AE_2C8E_462B_8DED_92B6D8B30D9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "RealCounter.h"

/////////////////////////////////////////////////////////////////////////////
// CPartnerMngApp
// See PartnerMng.cpp for the implementation of this class
//

//----------------------------------------------------------------------------
//					CGateManager
//----------------------------------------------------------------------------
#include "../../inc/IGateMng.h"
#include "../../inc/ADKMapDef.h"
#include "DataConverter.h"

#include <AFXMT.H>

//----------------------------------------------------------------------------
//					CGateManager
//----------------------------------------------------------------------------
class CGateManager : public IGateManager
{
public:
	
public:	// Implementation
	STDMETHOD_(LONG, GetInterface)(int nID, long dwKey=0);
	
public:	// Variable
	
};

//----------------------------------------------------------------------------
//					CPartnerManager
//----------------------------------------------------------------------------
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/IMasterDataMng.h"

//----------------------------------------------------------------------------
//					CPartnerManager_SubDataA
//----------------------------------------------------------------------------
class CPartnerManagerSubDataA
{
public:
	BYTE	m_bRealKeyInfo[255-TRKEY_REALSTART];
	BYTE	m_bMarketInfo[255-TRKEY_REALSTART];
	
	CPartnerManagerSubDataA();
};

class CRegisterCodeMng
{
public:
	~CRegisterCodeMng();

	CStringList m_CodeList;
	POSITION FindCode(LPCSTR szCode);
	void RemoveCode(LPCSTR szCode);
	void AddCode(LPCSTR szCode, int nIndex=0);
};

class CPartnerManager : public IPartnerManager
{
public:	
	STDMETHOD_(void, SetSite)(IPartnerSite* pSite);	
	STDMETHOD_(IPartnerSite*, GetSite)() {return m_pPtSite;}

	STDMETHOD_(BOOL, SendTrData)(LPCSTR szTR, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE);
	STDMETHOD_(BOOL, SendFidData)(LPCSTR szFid, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE);
	STDMETHOD_(BOOL, SendTrDataEx)(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE);
	STDMETHOD_(BOOL, SendFidDataEx)(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE);

	STDMETHOD_(BOOL, ReceiveTRData)(LPCSTR szTR, LPBYTE pData, int nLen, int nKey);

	//RealSend
	STDMETHOD_(int, RequestMChartReal)(BYTE cKey, LPCSTR szDrdsKey, LPCSTR szCode);
	STDMETHOD_(BOOL, ReceiveRealData)(WPARAM wParam, LPARAM lParam);

	STDMETHOD_(int, directSendFID)(long pSendData);

	STDMETHOD_(BOOL, ReceiveRealDataEx)(WPARAM wParam, LPARAM lParam) ;

	//20100412_JS.Kim FX. 체결데이타 실시간 수신
	STDMETHOD_(BOOL, ReceiveNoticeData)(WPARAM wParam, LPARAM lParam) ;

	STDMETHOD_(BOOL, IsRegisterCode)(LPCSTR szCode);

public:
	CPartnerManager();
	HWND	SetSendHandle(HWND hSend);
	BOOL	SendTR(CString strTR, char *szData, int nLen, int nKey, int nStat);
	int		GetTrKey(CString strTR, FID_SENDDATA* pSendData=NULL);

	int		GetRealKey(BYTE cKey, LPCSTR szCode, int nCodeType); //A,U
	int		FindRealKey(LPCSTR szCode);

	CArray<FID_SENDDATA, FID_SENDDATA&>	m_arrSend;

protected:
	BOOL ReceiveRealDataEx_Sub(CString& strCode, int nCodeType, int nKey, DWORD* pArrData);

	int _DoRealDataParsing(LPTSTR pData, CMapStringToString *pMap);
	int _DoParser(char cParser, LPTSTR pData, int &nPos, int nEndPos, char* aString);
	//CString _DoParser(CString &strSrc, CString strSub);

	int _DoGetCodeType(LPCSTR szCode);
	IMasterDataManagerLast*	m_pMasterDataMng;

	void _DoPreReceiveData(LPCSTR szTRKey, LPBYTE pData, int nSize);

	HWND	m_hSenComm;
	IPartnerSite* m_pPtSite;
	CDataConverter m_DataConverter;

	CADKMapStringToInt m_mapRealKey;
	CPartnerManagerSubDataA m_subDataA;

	CCriticalSection m_CriticalSection;

	BOOL				m_bUseRegisterCodeMng;
	CRegisterCodeMng	m_RegisterCodeMng;
};


//----------------------------------------------------------------------------
//					CPartnerSvrManager
//----------------------------------------------------------------------------
class CPartnerSvrManager : public IPartnerSvrManager
{
public:
	STDMETHOD_(IPartnerManager*, AdviseManager)(HWND _hWinixWnd, HWND _hChart) ;
	//STDMETHOD_(int, UnAdviseManager)(IPartnerManager* pPtMng) ;
	STDMETHOD_(int, UnAdviseManager)(HWND _hWinixWnd, HWND _hChart);
	STDMETHOD_(int,  GetParentHandle)(int nIndex, HWND &_hWinixWnd);
	STDMETHOD_(LONG,  GetInerInterface)(int nIndex, long dwKey=0) ;

public:
	CPartnerSvrManager();
	~CPartnerSvrManager();

	//int RemoveItem(IPartnerManager* pKey);
	//int RemoveItem(HWND _hWnd);
	int RemoveItem(HWND _hWinixWnd, HWND _hChart);
	void RemoveAll();
	STDMETHOD_(BOOL, SetParentHandle)(BYTE cKey, HWND _hWinixWnd, HWND _hChart);

	//List_IPartnerManager m_List;
	CMapStringToPtr		m_mapList;
	//CPtrArray			m_mapHandleList;
	CStringArray		m_mapHandleList;
};

//----------------------------------------------------------------------------
//					CChartBozoManager
//----------------------------------------------------------------------------
#include "../../inc/IChartBozoMsgMng.h"
class CChartBozoManager : public IChartBozoMsgManager
{
public:
	//szFIDKey : FIDKey
	//pChartBInput : input정보.
	// rString : 보조메시지 얻기.
	STDMETHOD_(int, GetChartBozoMsg)(LPCSTR szFIDKey, CChartBozoInput* pChartBInput, CString &rString, int nOption=0);
	STDMETHOD_(int, ChangeChartBozoMsg)(CString &rString, List_CChartBozoItem* pList) ;

public:
	//STDMETHOD_(void, MakeChtBojo)(LPCTSTR lpszFormat, ...);

	STDMETHOD_(int, GetFidType)(LPCSTR szFIDMajor);
	STDMETHOD_(int, GetFidType)(int nFIDMajor);
	STDMETHOD_(int, GetTrType)(LPCSTR szTrName);

	STDMETHOD_(int, MakeChtBojo_ctstk)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctup)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctsfo)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctfo)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctfrn)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_market)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctfofor)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);
	STDMETHOD_(int, MakeChtBojo_ctfx)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);	//20100310_JS.Kim FX
	STDMETHOD_(int, MakeChtBojo_ctcme)(CChartBozoInput* pChartBInput, CString& rString, int nOption=0);	//20110106 alzioyes CME

public:
	CChartBozoManager();

protected:
	int _DoCalcBozoSize(CString &rString);
	BOOL _FindKeyInBozoString(CString &rString, LPCSTR szKey, int &nStart, int &nLength);
	void _DoPre_GetChartBozoMsg(CChartBozoInput* pChartBInput);
};

//----------------------------------------------------------------------------
//					CPartnerUtilManager
//----------------------------------------------------------------------------
class CPartnerUtilManager : public IPartnerUtilManager
{
public:
	STDMETHOD_(LRESULT, DoWinixVariant)(HWND hWnd, int comm, CString data);

	STDMETHOD_(BOOL, OpenScreen)(HWND hWnd, LPCSTR szScreenNo) ;
	STDMETHOD_(BOOL, ShareCode)(HWND hWnd, LPCSTR szShareCode);
	STDMETHOD_(BOOL, GetLinkScreenInfo)(LPCSTR szScrrenNo, CStringList& szKeyList) ;

	STDMETHOD_(BOOL, GetSymbolCode)(LPCSTR szCode, CString& szSymbol);
	STDMETHOD_(BOOL, GetSymbolCode)(BYTE cChartMarketType, CString& szSymbol);
	//>>JS.Kim_20101025  리얼 요청하는 MarketType코드와 History 구하는 코드가 다르다고 하여 리얼용 함수를 따로 만듬
	STDMETHOD_(BOOL, GetRealSymbolCode)(BYTE cChartMarketType, CString& szSymbol);
	//<<

	STDMETHOD_(BOOL, GetCodeInfoFromShared)(CString szData, CString& szSymbol, CString& szCode);
	STDMETHOD_(BOOL, GetChartMarketFromSymbol)(BYTE &cChartMarketType, LPCSTR szSymbol);

	STDMETHOD_(BOOL, GetCaptionTitle)(HWND hWnd, LPCSTR szScreenNo);

	STDMETHOD_(CString, Variant)(HWND hWnd, int nComm, CString strData);
	STDMETHOD_(HBITMAP, GetBitmap)(HWND hWnd, CString strPath);
	STDMETHOD_(CBrush*, GetBrush)(HWND hWnd, COLORREF clr);
	STDMETHOD_(CFont*, GetAxFont)(HWND hWnd, CString fName, int point, bool bBold, bool bItalic);
	STDMETHOD_(COLORREF, GetAxColor)(HWND hWnd, int nColor);

	STDMETHOD_(int, DoPlatformMenu)(HWND hWnd, CWnd* pWnd, CPoint point, LPCSTR szScreenID) ;
	STDMETHOD_(BOOL, GetLinkScreenOpen)(HWND hWnd, LPCSTR szScrrenNo, int nIndex, BOOL bFront);

	STDMETHOD_(CString, GetCurrentPalette)();
	STDMETHOD_(BOOL, GetTabColor)(CString szPalette, int nIdx, COLORREF &clrStart, COLORREF &clrEnd);

	//종목컨트롤의 History에 코드추가.
	STDMETHOD_(BOOL, AddToHistory)(HWND hWnd, LPCSTR _szCode);
	STDMETHOD_(BOOL, GetTRCodeFromCode)(BYTE cChartMarketType, LPCSTR szCode, CString &szTRCode);

	STDMETHOD_(BOOL, IsCodeSymbol)(LPCSTR szSymbol) ;

public:
	CPartnerUtilManager();

	BOOL GetSiteUpKode(LPCSTR szCode, CString& rCode);

protected:
	BOOL _IsFileExist(LPCSTR szFileName);
	int	 _GetIniSectList(LPCSTR szIniPath, LPCSTR szSect, CStringList& list);
	HWND _GetSiteHandle();
	LPCSTR _DoGetRootDir();

	CString m_szRootDir;
	CString m_szCurPalette;
	CString m_szLastHistoryCode;

	IMasterDataManagerLast*	m_pMasterMng;
};

//----------------------------------------------------------------------------
//					CPartnerMngApp
//----------------------------------------------------------------------------
#define MAX_Partner_INTERFACE	10
typedef struct 
{
	LONG pInterface;
} STUA_INTERFACE;

class CPartnerMngApp : public CWinApp
{
public:
	CPartnerMngApp();

public:
	// 인터페이스 관리 변수(Array)
	STUA_INTERFACE m_uaInterface[MAX_Partner_INTERFACE];
	IGateManager*		m_pPctrGateMng;

	// No.0 IGageManager
	CGateManager		m_GateMng;

	CPartnerSvrManager	m_PtSvrMng;
	CChartBozoManager	m_ChartBozoMng;
	CPartnerUtilManager m_PtUtilMng;

	CRealCounterMng		m_CountMng[1];

	CMapStringToString	m_mapHighLow;

public:
	void DoMapStrObj_UpDnInfo(LPCSTR szKey, LPCSTR sUpLimit, LPCSTR sDnLimit);
	void DoGetMapStrObj_UpDnInfo(LPCSTR szKey, CString &szUpLimit, CString &szDnLimit);
	LONG DoGetPctrInterface(int nID);

protected:
	CMapStringToOb		m_StrObjData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartnerMngApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPartnerMngApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};

extern CPartnerMngApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTNERMNG_H__CC4107AE_2C8E_462B_8DED_92B6D8B30D9A__INCLUDED_)
