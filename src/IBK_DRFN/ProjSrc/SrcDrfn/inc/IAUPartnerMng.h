#if !defined(AFX_HANARO_IPARTNERNANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IPARTNERNANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../../inc/IPartnerMng.h"
#include <afxtempl.h>
//#include "./ITrCommMng.h"
#include "./IBaseDefine.h"

struct FID_SENDDATA
{
	CString	szTrCode;
	char*	pData;
	int		nLen;
	HWND	hWnd;
};

struct FID_RECEIVEDATA
{
	int		nKey;
	CString	szTrCode;
	char*	pData;
	int		nLen;
};

// 50번 인터페이스는 사이트마다 다르게 정의한다.
#define		UUID_IPartnerSvrManager		50

// 내부적인 인터페이스 번호
#define		LUID_IPartnerManager		0
//#define		LUID_IChartBozoMsgManager	1	//IChartBozoMsgMng.h
#define		LUID_IPartnerUtilManager	2

//	IPartnerManager* pMng = (IPartnerManager*)AfxGetPctrInterface(UUID_IPartnerManager);
// 만든이  : 이준옥 2009.09.21(월)

//◇위닉스의 플랫폼에 맞게 작업하는 interface
//◇위닉스에 리턴하는 CWnd에서 IPartnerManager를 얻어서 셋팅한다.
//◇IPartnerSite은 DRFN컨텐츠에서 구현해서 생성초기에 넘겨받은 IPartnerManager포인터를 통해
//  등록한다.
//◇DRFN컨텐츠에서는 IPartnerManager를 통해 데이터를 보내고
//   IPartnerSite를 통해 데이터를 받는다.

DECLARE_INTERFACE(IPartnerSite)
{
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(RecieveFidData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen) PURE;

	STDMETHOD(RecieveTRData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen) PURE;

	STDMETHOD(RecieveRealData) (long dwKey, LONG dwData) PURE;

	enum{
		IPS_RK_shareCode=0,
		IPS_RK_codeDlg,
		IPS_RK_gwansim,
		IPS_RK_codeHidDlg,	//과거종목조회 Dialog에서 주는 경우.
		IPS_RK_SaveKeyEnd,	//종료시 보내지는 저장키.
	};
	// nKey 0 : 공유공유전달
	//		1 : 종목 Dialog에서 선택한 코드
	STDMETHOD(RecieveKeyData)(int nKey, LPCSTR szData) {return S_OK;}

	STDMETHOD(RecieveRealDataEx) (long dwKey, LPCTSTR pszCode, LONG dwData) {return S_OK;}

	STDMETHOD(ReceiveOnUser) (WPARAM wParam, LPARAM lParam) {return S_OK;}

	STDMETHOD(ReceiveNoticeData) (LPCTSTR pszCode, LONG dwData) {return S_OK;}
	
};
typedef  IPartnerSite FAR * LPIPartnerSite;

DECLARE_INTERFACE(IPartnerManager)
{
	//DRFN컨텐츠에서 셋팅함.
	STDMETHOD_(void, SetSite)(IPartnerSite* pSite) PURE;
	STDMETHOD_(IPartnerSite*, GetSite)() PURE;

	//DRFN컨텐츠에서 데이터 보낼때 사용함.
	STDMETHOD_(BOOL, SendTrData)(LPCSTR szTR, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE) PURE;
	STDMETHOD_(BOOL, SendFidData)(LPCSTR szFid, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE) PURE;
	STDMETHOD_(BOOL, SendTrDataEx)(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE) PURE;
	STDMETHOD_(BOOL, SendFidDataEx)(long dwExInfo, LPBYTE pData, int nLen, int &nKey, BOOL bAdd=FALSE) PURE;

	// DLL에서 수신후 이 함수 호출 -> IPartnerSite를 통해 DLL쪽의 ReceiveTRData호출됨
	STDMETHOD_(BOOL, ReceiveTRData)(LPCSTR szTR, LPBYTE pData, int nLen, int nKey) PURE;

	//RealSend
	// cKey  A(Advise), U(UnAdvise)
	STDMETHOD_(int, RequestMChartReal)(BYTE cKey, LPCSTR szDrdsKey, LPCSTR szCode) PURE;
	STDMETHOD_(BOOL, ReceiveRealData)(WPARAM wParam, LPARAM lParam) PURE;

	STDMETHOD_(int, directSendFID)(long pSendData) PURE;

	STDMETHOD_(BOOL, ReceiveRealDataEx)(WPARAM wParam, LPARAM lParam) PURE;

	//20100412_JS.Kim FX. 체결데이타 실시간 수신
	STDMETHOD_(BOOL, ReceiveNoticeData)(WPARAM wParam, LPARAM lParam) PURE;

	STDMETHOD_(BOOL, IsRegisterCode)(LPCSTR szCode) PURE;
};

#define clridx_BackGround		3

DECLARE_INTERFACE(IPartnerUtilManager)
{
	// ------------------------------------------------------------------------------
	//									공통설명.
	// ------------------------------------------------------------------------------
	// 아래루틴에서 HWND hWnd; 은 위닉스의 핸들이며, NULL일 경우 내부적으로 첫번째 등록된 핸들을 사용한다.
	// 꼭 자기 윈도우에 해당할 때는 hWnd를 넘겨주어야 하지만 그렇지 않으면 NULL로 넘겨주어도 된다.
	// ------------------------------------------------------------------------------

	// comm : titleCC ChildFrame의 Caption변경.
	STDMETHOD_(LRESULT, DoWinixVariant)(HWND hWnd, int comm, CString data) PURE;

	STDMETHOD_(BOOL, OpenScreen)(HWND hWnd, LPCSTR szScreenNo) PURE;

	//szKeyList <0> : KeyCode
	//			<1> : Code
	STDMETHOD_(BOOL, ShareCode)(HWND hWnd, LPCSTR szShareCode) PURE;

	STDMETHOD_(BOOL, GetLinkScreenInfo)(LPCSTR szScrrenNo, CStringList& szKeyList) PURE;

	STDMETHOD_(BOOL, GetSymbolCode)(LPCSTR szCode, CString& szSymbol) PURE;
	STDMETHOD_(BOOL, GetSymbolCode)(BYTE cChartMarketType, CString& szSymbol) PURE;

	STDMETHOD_(BOOL, GetCodeInfoFromShared)(CString szData, CString& szSymbol, CString& szCode) PURE;
	STDMETHOD_(BOOL, GetChartMarketFromSymbol)(BYTE &cChartMarketType, LPCSTR szSymbol) PURE;

	STDMETHOD_(BOOL, GetCaptionTitle)(HWND hWnd, LPCSTR szScreenNo) PURE;

	STDMETHOD_(CString, Variant)(HWND hWnd, int nComm, CString strData) PURE;
	STDMETHOD_(HBITMAP, GetBitmap)(HWND hWnd, CString strPath) PURE;
	STDMETHOD_(CBrush*, GetBrush)(HWND hWnd, COLORREF clr) PURE;
	STDMETHOD_(CFont*, GetAxFont)(HWND hWnd, CString fName, int point, bool bBold, bool bItalic) PURE;
	STDMETHOD_(COLORREF, GetAxColor)(HWND hWnd, int nColor) PURE;

	STDMETHOD_(int, DoPlatformMenu)(HWND hWnd, CWnd* pWnd, CPoint point, LPCSTR szScreenID) PURE;
	STDMETHOD_(BOOL, GetLinkScreenOpen)(HWND hWnd, LPCSTR szScrrenNo, int nIndex, BOOL bFront) PURE;

	STDMETHOD_(CString, GetCurrentPalette)() PURE;
	STDMETHOD_(BOOL, GetTabColor)(CString szPalette, int nIdx, COLORREF &clrStart, COLORREF &clrEnd) PURE;

	STDMETHOD_(BOOL, AddToHistory)(HWND hWnd, LPCSTR _szCode) PURE;
	STDMETHOD_(BOOL, GetTRCodeFromCode)(BYTE cChartMarketType, LPCSTR szCode, CString &szTRCode) PURE;

	STDMETHOD_(BOOL, IsCodeSymbol)(LPCSTR szSymbol) PURE;
};

//IPartnerManager Advise()/UnAdvise할것.
DECLARE_INTERFACE(IPartnerSvrManager)
{
	STDMETHOD_(IPartnerManager*, AdviseManager)(HWND _hWinixWnd, HWND _hChart) PURE;
	//STDMETHOD_(int, UnAdviseManager)(IPartnerManager* pPtMng) PURE;
	STDMETHOD_(int, UnAdviseManager)(HWND _hWinixWnd, HWND _hChart) PURE;

	// cKey  A(Advise), U(UnAdvise)
	// A : Add, U : Remove
	// STDMETHOD_(BOOL, SetParentHandle)(BYTE cKey, HWND _hWnd) PURE;

	// Handle 리스트중에서 얻기,
	// nIndex는 리스트중에서 얻기.
	// Return값은 전체리스트 카운트 리턴.
	STDMETHOD_(int,  GetParentHandle)(int nIndex, HWND &_hWinixWnd) PURE;

	STDMETHOD_(LONG,  GetInerInterface)(int nIndex, long dwKey=0) PURE;
};

typedef CList<IPartnerManager*, IPartnerManager*> List_IPartnerManager;

#endif //AFX_HANARO_IPARTNERNANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
