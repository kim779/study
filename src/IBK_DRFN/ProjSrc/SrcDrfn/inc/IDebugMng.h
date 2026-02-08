#if !defined(AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_

#include "./IBaseDefine.h"
#include "./IDebugData.h"
#include "./LoadDRDebuger.h"

#define		UUID_IDebugManager					14		// IBaseDefine.h에 정의되어 있음..

#define		IDebugManagerLast					IDebugManager3

//#define		UUID_IDebugManager2					14

/* -------------------------------------------------------------------------
설명 : 디버깅을 지원하기 위한 인터페이스
관련문서 : 정리_디버깅.doc
만든이  : 이준옥 2005.12.07(수)
History : 
----------------------------------------------------------------------------

int nID = 14;
IDebugManager* pManager = (IDebugManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IDebugManager* pManager = (IDebugManager*)AfxGetPctrInterface(nID);

IDebugManager* pIDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
if(pIDebugMng)
{
	CString szMsg;
	szMsg.Format("UP/DOWN:%d, 절대:%d, 상대:%d\n", m_nUpDown, m_ptATableOffset, m_ptRTableOffset.y);
	pIDebugMng->Trace(szMsg);
}
------------------------------------------------------------------------- */

DECLARE_INTERFACE(IDebugManager)
{
	/* --------------------------------------------------- 
	-. 설명[Trace] ListBox에 로그찍기
	-. I-PARAM : Format(..)형태, 최대버퍼는 1024크기
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, Trace)(LPCTSTR lpszFormat, ...) PURE;
	STDMETHOD_(void, Trace)(int nScrKey, LPCTSTR lpszFormat, ...) PURE;
	
	STDMETHOD_(void, WriteLogFile)(void* sData, int nDataLen, LPCSTR szFileName, BOOL bInsertCR=TRUE, LPCSTR szLogDir="c:\\Logs") PURE;
	STDMETHOD_(void, TraceEx)(BYTE cMessageTag, BYTE cIO, LPCSTR pDataName, DWORD lDataSize, LPBYTE pData) PURE;

	//cIO 'I':Input, 'O' Output
	//pTranName는 최대 30자리
	STDMETHOD_(void, TraceTran)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) PURE;
	STDMETHOD_(void, TraceReal)(LPCSTR pRealName, DWORD dwSize, LPBYTE pData) PURE;

	//cAU 'A':Advise, 'U':UnAdvise
	//cIO 'I':Input, 'O' Output
	//pTranName는 최대 30자리
	//hReceiver : WM_COPYDATA받을 윈도우
	//pOnCopyDataKey : WM_COPYDATA에서의 pCopyDataStruct->dwData
	//QCMonitor를 통해 데이터를 받고자 할 때 사용하는 명령.
	STDMETHOD_(void, TraceSR)(BYTE cAU, BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey) PURE;

	STDMETHOD_(void, ShowOutputDebugString)(LPCTSTR lpszFormat, ...) PURE;
};

//IDebugManager2는 //Data 비교Tool를 위한 기능
DECLARE_INTERFACE_(IDebugManager2, IDebugManager)
{
	STDMETHOD_(BOOL, TraceCompData)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) PURE;
};

// DRDebuger.dll 사용용
DECLARE_INTERFACE_(IDebugManager3, IDebugManager2)
{
	STDMETHOD_(LONG, GetDRDebuger()) PURE;
};

// #define DRMONITOR_CLASSNAME		_T("DrMonitorClass")	//실행디버그의 클래스명.
// #define	gDebugWindow			"DrMonitor"				//exe뺀 실행파일명. 
// UINT m_uRMSG_HandleToDebug = ::RegisterWindowMessage("RMSG_HandleToDebug");

typedef struct
{
	BYTE	cTag;			
	DWORD	dwDataSize;
	LPBYTE  dwDataBuff;	//가변적인 데이터 영역.
}STDebugMngTraceEx;


#endif //AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_
