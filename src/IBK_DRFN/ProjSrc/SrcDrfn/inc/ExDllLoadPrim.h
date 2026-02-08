#if !defined(AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

//#include "../../common_base/DataMng.h"
//#include "../../common_base/AcctUtil.h"
//
#include "baseGShareMng.h"
#include "OcxDLLDef.h"
//#include "baseGShareMng.h"
//#include "Account.h"
//#include "Platform.h"
//#include "../../include_base/ReserveShareName.h"


// * DLL화면을 만들기 위해 상속받아야 하는 클래스 
class CPrimDialog
{
public:
	long		m_lExtra1;
	long		m_lExtra2;
//	CAcctHelper*	m_pAccHelp;

	HWND		m_hSocket;			// 소켓 핸들
	HWND		m_hParent;			// 부모 윈도우 핸들 

	CString		m_strExtra1;		// Ticker에서 띄울때 최소로 하기 위해서 -> 이유정씨 부탁
	CString		m_strRootPath;		// 루트 경로
	CString		m_strUserDir;		// User 디렉토리
	CString		m_strDataDir;		// Date 디렉토리
	CString		m_strUserID;		// 사용자 id

	CString		m_strMapID;			// 화면번호
	CString		m_strEnvFilePath;	// 환경파일 경로

	CStringList	m_strEnvVariables;	// 환경변수 리스트 (key,value)

	long		m_dwAcctManager;	// 계좌 매니저 
	long		m_pShareMgr;		// 공유 매니져
	
	//void FormLoad()
	void SetData(void* _pData)
	{
		if(_pData == NULL)
			return;

		LPStDllLoadData pData = (LPStDllLoadData)_pData;
		m_hSocket = pData->m_hSocket;
		m_hParent = pData->m_hParent;
		m_strRootPath = pData->m_szRootPath;
		m_strUserDir = m_strRootPath + "\\User";
		m_strDataDir = m_strRootPath + "\\Data";
		m_strUserID = pData->m_szUserID;
		m_strMapID = pData->m_szMapID;
		m_strEnvFilePath = pData->m_szEnvFilePath;
//		m_lExtra1 = pData->m_lExtra1;			// DefaultColorTable*
//		m_strExtra1 = pData->m_szExtra1;			// 이유정씨 부탁
		m_lExtra2 = pData->m_lExtra2;		// BaseDesl	- ojtaso

		fnSetSharedInterface(pData->m_dwInterface, pData->m_dwAcctManager);
	}

	void ClearData()
	{
		ClearSharedInterface();
	}

	void GetUserInfo()
	{
/*		typedef CString	(WINAPI * fnGetEnvVariables) (CString, CString);

		HINSTANCE hInst = LoadLibrary("EnvSetup.dll");

		if(hInst==NULL)
		{
			TRACE("EnvSetup.dll을 로드할 수 없습니다.");
			return;
		}
				
		fnGetEnvVariables fpGetEnvVar = (fnGetEnvVariables)::GetProcAddress(hInst, "fnGetEnvVariables");
		if(fpGetEnvVar)
		{
			CString szUnit;
			CString szRet = fpGetEnvVar(m_szMapID, m_szEnvFilePath);

			// 맨 마지막에 구분자(;)가 있으면 제거한다. 
			if(szRet.Right(1) == ";")
				szRet.Mid(0, szRet.GetLength()-1);

			int nIndex = szRet.Find(";");
			while(nIndex>0)
			{
				szUnit = szRet.Left(nIndex);
				szRet = szRet.Mid(nIndex + 1);
				m_szEnvVariables.AddTail(szUnit);
				nIndex = szRet.Find(";");
				if(nIndex<0)
				{
					m_szEnvVariables.AddTail(szRet);
					break;
				}
			}
		}
		else
		{
			TRACE("fnGetEnvVariables 함수 포인터를 얻을 수 없습니다.");			
		}
*/
	}

	// * 공유 처리 관련 Nasted Class
	class CGSharedSvr  : public CDeskSharedServer
	{
	public:
		~CGSharedSvr() {};
		CList <CListGlobalCliient*, CListGlobalCliient*> m_CliList;
		CBaseChildShareManager*	m_pGSharedMain;
		CString		m_szShareValue;
		
		void RemoveAll();
		BOOL NotifyToClient(LPCSTR _szName, LPCSTR _szValue, CGlobalSharedClient* _pCli);
		
		// Virtual
		// CGlobalSharedClient 에서 호출하는 함수
		virtual BOOL GetData(LPCSTR _szName, CString& szValue)
		{
			return TRUE;
		}
		virtual BOOL SetData(LPCSTR _szName, LPCSTR _szValue, CGlobalSharedClient* _pCli)
		{
			return TRUE;
		}
		virtual BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient)
		{
			return TRUE;
		}
		virtual BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient)
		{
			return TRUE;
		}
		
		// CBaseChildShareManager 에서 호출하는 함수
		virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue)
		{
			MYMETHOD_PROLOGUE(CPrimDialog, GSharedSvr)
		
			return pThis->ChangedGlobalData(_szName, _szValue);
		}
		
		virtual LPCSTR GetData2(LPCSTR _szName)
		{
			return NULL;
		}
		
		CList<CDeskSharedServer*, CDeskSharedServer*> m_DeskSSList;
		virtual void SetDeskSharedServer(long* pList, int nListCnt)
		{
		}

		void NotifyDeskSharedServer(LPCSTR _szName, LPCSTR _szValue);		
	} m_xGSharedSvr;
	friend class CGSharedSvr;

	void fnSetSharedInterface(long dwInterface, long dwAcctManager)
	{
		if(dwInterface && dwAcctManager)
		{
			m_xGSharedSvr.m_pGSharedMain = (CBaseChildShareManager*)dwInterface;

			BOOL ret = m_xGSharedSvr.m_pGSharedMain->Advise(&m_xGSharedSvr);
//			m_dwAcctManager = dwAcctManager;
			m_pShareMgr = dwInterface;

//			m_pAccHelp = (CAcctHelper *) m_dwAcctManager;
		}
	}
	void ClearSharedInterface()
	{
		m_xGSharedSvr.m_pGSharedMain->UnAdvise(&m_xGSharedSvr);
	}
	virtual BOOL ChangedGlobalData(CString szName, CString szValue)
	{
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADDEF_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
