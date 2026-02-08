#ifndef __DRDSLOADLIB__HEADER__
#define __DRDSLOADLIB__HEADER__

// 비트 체크 CAlertInfo 클래스의 mp_nComponentType 변수에 셋팅할 데이터
#define SignalNone						0x00	// 어떤 컴포넌트도 받지 않는다.
#define SignalNormal					0x01	// 일반 컴포넌트
#define SignalAlarm						0x02	// 알람 컴포넌트
#define SignalTicker					0x04	// 티커
#define SignalOrder						0x08	// 주문 관련

typedef  void* (WINAPI* DRDS_INIT)(HWND);
typedef  BOOL  (WINAPI* Drds_Term)(void*, HWND);
typedef  BOOL  (WINAPI* Drds_Connect)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Close)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Advise)(void*, HWND,UINT,LPSTR,LPSTR,LPSTR);
typedef  BOOL  (WINAPI* Drds_AdviseEx)(int, HWND, UINT, LPSTR, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_Unadvise)(void*, HWND, UINT, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_UnadviseEx)(int, HWND, UINT, LPSTR, LPSTR, LPSTR, LPSTR);
typedef  BOOL  (WINAPI* Drds_SetSocketHandle)(HWND);
typedef  void  (WINAPI*	Drds_SendCode)();
typedef  HWND  (WINAPI* Drds_SetGetBrodHwnd)();
typedef  void  (WINAPI*	Drds_SetFlag)(BOOL);
typedef  int   (WINAPI*	Drds_UDPCreate)();
typedef  int   (WINAPI*	Drds_UDPCreate2)(LPSTR, LPSTR);
typedef  void  (WINAPI*	Drds_UdpCreate3)(LPSTR);
typedef  void  (WINAPI*	Drds_UDPDestroy)();
typedef  int   (WINAPI*	Drds_SetUserDefData)(LPSTR, LPSTR);
typedef  BOOL  (WINAPI*	Drds_AdviseRealCodeData)(BOOL, int, LPSTR, LPSTR);


#define WM_DRDS_DATA			WM_USER + 200
#define MAX_DRDS_PACKET_LEN		4096
typedef struct  
{
	char	Name[32];
	char	Field[32];
	char	Key[32];
	UINT	Id;
	int		Len;
	char	Data[MAX_DRDS_PACKET_LEN];
} DrdsData;


class CDrdsLoadLib
{
public:
	CDrdsLoadLib()
	{
		m_hLib = NULL;
		m_szLibName = "Drds.dll";
	}


	~CDrdsLoadLib()
	{
		UnLoadLib();
	}

public:
	HINSTANCE m_hLib;
	CString m_szLibName;

	void SetLibName(LPCSTR _szLibName)
	{
		if(_szLibName==NULL)
			m_szLibName = "Drds.dll";
		else
			m_szLibName = _szLibName;
	}


	HINSTANCE LoadLib()
	{
		m_hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( m_hLib == NULL)
		{
			AfxMessageBox("Drds DLL을 Load할 수 없습니다.");
			return NULL;
		}
		return m_hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}

	void* DrdsInit(HWND hWnd)
	{
		if(!m_hLib)	return NULL;

		DRDS_INIT fnDrdsInit;
		fnDrdsInit = (DRDS_INIT)::GetProcAddress(m_hLib,"DrdsInit");

		if(fnDrdsInit == NULL)
		{
			AfxMessageBox("DrdsInit함수를 찾을 수 없습니다.");
			return NULL;
		}

		void* pResult;
		pResult = fnDrdsInit(hWnd);
		return pResult;
	}

	HWND  DrdsGetBrodHwnd()
	{
		if(!m_hLib)	return NULL;

		Drds_SetGetBrodHwnd fnDrdsGetBrodHwnd;
		fnDrdsGetBrodHwnd = (Drds_SetGetBrodHwnd)::GetProcAddress(m_hLib,"DrdsGetBrodHwnd");

		if(fnDrdsGetBrodHwnd == NULL)
		{
			AfxMessageBox("DrdsGetBrodHwnd함수를 찾을 수 없습니다.");
			return NULL;
		}

		HWND pResult;
		pResult = fnDrdsGetBrodHwnd();
		return pResult;
	}

	BOOL  DrdsSetSocketHandle(HWND hWnd)
	{
		if(!m_hLib) return FALSE;
		Drds_SetSocketHandle fnSetSocketHandle;

		fnSetSocketHandle = (Drds_SetSocketHandle) ::GetProcAddress(m_hLib,"DrdsSetSocketHandle");

		if(fnSetSocketHandle == NULL)
		{
			AfxMessageBox("DrdsSetSocketHandle 함수를 찾을 수 없습니다.");
			return FALSE;	
		}

		BOOL bResult;
		bResult = fnSetSocketHandle(hWnd);
		return bResult;

	}

	BOOL DrdsTerm(void* pDrds, HWND hWnd)
	{
		if(!m_hLib) return FALSE;
		Drds_Term fnDrdsTerm;

		fnDrdsTerm = (Drds_Term)::GetProcAddress(m_hLib,"DrdsTerm");

		if(fnDrdsTerm == NULL)
		{
			AfxMessageBox("DrdsTerm함수를 찾을 수 없습니다.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsTerm(pDrds,hWnd);

		return bResult;
	}

	BOOL DrdsConnect(void* pDrds, HWND hWnd,LPSTR DataName)
	{

		if(!m_hLib) return FALSE;
		Drds_Connect fnDrdsConnect;

		fnDrdsConnect = (Drds_Connect)::GetProcAddress(m_hLib,"DrdsConnect");

		if(fnDrdsConnect == NULL)
		{
			AfxMessageBox("DrdsConnect함수를 찾을 수 없습니다.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsConnect(pDrds,hWnd,DataName);
		return bResult;

	}

	BOOL DrdsClose(void* pDrds, HWND hWnd,LPSTR DataName)
	{
		if(!m_hLib) return FALSE;
		Drds_Close fnDrdsClose;
		fnDrdsClose = (Drds_Close)::GetProcAddress(m_hLib,"DrdsClose");

		if(fnDrdsClose == NULL)
		{
			AfxMessageBox("DrdsClose함수를 찾을 수 없습니다.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsClose(pDrds,hWnd,DataName);
		return bResult;
	}

	BOOL DrdsAdvise(void* pDrds, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return FALSE;
		Drds_Advise fnDrdsAdvise;
		fnDrdsAdvise = (Drds_Advise)::GetProcAddress(m_hLib,"DrdsAdvise");

		if(fnDrdsAdvise == NULL)
		{
			AfxMessageBox("DrdsAdvise함수를 찾을 수 없습니다.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsAdvise(pDrds,hWnd,id,DataName,FieldName, KeyName);
		return bResult;
	}


	BOOL DrdsAdviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, 
						 LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return FALSE;
		Drds_AdviseEx fnDrdsAdviseEx;
		fnDrdsAdviseEx = (Drds_AdviseEx)::GetProcAddress(m_hLib,"DrdsAdviseEx");

		if(fnDrdsAdviseEx == NULL)
		{
			AfxMessageBox("DrdsAdviseEx함수를 찾을 수 없습니다.");
			return FALSE;
		}
		BOOL bResult; 
		bResult = fnDrdsAdviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
		return bResult;
	}


	BOOL DrdsUnadvise(void* pDrds, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return FALSE;
		Drds_Unadvise fnDrdsUnadvise;

		fnDrdsUnadvise = (Drds_Unadvise)::GetProcAddress(m_hLib,"DrdsUnadvise");


		if(fnDrdsUnadvise == NULL)
		{
			AfxMessageBox("DrdsUnadvise 함수를 찾을 수 없습니다.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsUnadvise(pDrds,hWnd,id,DataName,FieldName, KeyName);
		return bResult;
	}


	BOOL DrdsUnadviseEx(int nComponentType, HWND hWnd,UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return FALSE;
		Drds_UnadviseEx fnDrdsUnadviseEx;

		fnDrdsUnadviseEx = (Drds_UnadviseEx)::GetProcAddress(m_hLib,"DrdsUnadviseEx");


		if(fnDrdsUnadviseEx == NULL)
		{
			AfxMessageBox("DrdsUnadviseEx 함수를 찾을 수 없습니다.");
			return FALSE;
		}

		BOOL bResult; 
		bResult = fnDrdsUnadviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
		return bResult;
	}


	void	DrdsSendCode()
	{
		if(!m_hLib) return;
		Drds_SendCode	fnDrdsSendCode;

		fnDrdsSendCode = (Drds_SendCode)::GetProcAddress(m_hLib,"DrdsSendCode");

		if(fnDrdsSendCode == NULL)
		{
			AfxMessageBox("DrdsSendCode 함수를 찾을 수 없습니다.");
			return;
		}
		fnDrdsSendCode();
		return;
	}

	void DrdsSetFlag(BOOL bFlag)
	{
		if(!m_hLib)	return;

		Drds_SetFlag fnDrdsSetFlag;
		fnDrdsSetFlag = (Drds_SetFlag)::GetProcAddress(m_hLib,"DrdsSetFlag");

		if(fnDrdsSetFlag == NULL)
		{
			AfxMessageBox("DrdsSetFlag 함수를 찾을 수 없습니다.");
			return;
		}

		fnDrdsSetFlag(bFlag);
	}

	int	UDPCreate()
	{
		if(!m_hLib) return 0;

		Drds_UDPCreate	fnDrdsUDPCreate;

		fnDrdsUDPCreate = (Drds_UDPCreate)::GetProcAddress(m_hLib,"UDPCreate");

		if(fnDrdsUDPCreate== NULL)
		{
			AfxMessageBox("UDPCreate 함수를 찾을 수 없습니다.");
			return 0;
		}
		return fnDrdsUDPCreate();
	}

	int	UDPCreate2(LPSTR strIP, LPSTR strPort)
	{
		if(!m_hLib) return 0;
		Drds_UDPCreate2	fnDrdsUDPCreate2;

		fnDrdsUDPCreate2 = (Drds_UDPCreate2)::GetProcAddress(m_hLib,"UDPCreate2");

		if(fnDrdsUDPCreate2== NULL)
		{
			AfxMessageBox("UDPCreate2 함수를 찾을 수 없습니다.");
			return 0;
		}
		return fnDrdsUDPCreate2(strIP, strPort);
	}

	int UdpCreate3(LPSTR sCfgFullPathName)
	{
		if(!m_hLib) return 0;
		Drds_UdpCreate3	fnDrdsUdpCreate3;

		fnDrdsUdpCreate3 = (Drds_UdpCreate3)::GetProcAddress(m_hLib,"UdpCreate3");

		if(fnDrdsUdpCreate3== NULL)
		{
			AfxMessageBox("UdpCreate3 함수를 찾을 수 없습니다.");
			return 0;
		}
		fnDrdsUdpCreate3(sCfgFullPathName);
		return 1;
	}

	void	UDPDestroy()
	{
		if(!m_hLib) return;
		Drds_UDPDestroy	fnDrdsUDPDestroy;

		fnDrdsUDPDestroy = (Drds_UDPDestroy)::GetProcAddress(m_hLib,"UDPDestroy");

		if(fnDrdsUDPDestroy== NULL)
		{
			AfxMessageBox("UDPDestroy 함수를 찾을 수 없습니다.");
			return;
		}
		fnDrdsUDPDestroy();
		return;
	}


	int SetUserDefData(LPSTR sKeyName, LPSTR sData)
	{

		if(!m_hLib) return 0;
		Drds_SetUserDefData	fnSetUserDefData;

		fnSetUserDefData = (Drds_SetUserDefData)::GetProcAddress(m_hLib,"SetUserDefData");

		if(fnSetUserDefData== NULL)
		{
			AfxMessageBox("SetUserDefData 함수를 찾을 수 없습니다.");
			return 0;
		}
		fnSetUserDefData(sKeyName, sData);
	}




	BOOL AdviseRealCodeData(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList)
	{
		if(!m_hLib) return FALSE;
		Drds_AdviseRealCodeData	fnAdviseRealCodeData;

		fnAdviseRealCodeData = (Drds_AdviseRealCodeData)::GetProcAddress(m_hLib,"AdviseRealCodeData");

		if(fnAdviseRealCodeData== NULL)
		{
			AfxMessageBox("AdviseRealCodeData 함수를 찾을 수 없습니다.");
			return FALSE;
		}
		return fnAdviseRealCodeData(bIsAdd, nCount, sPacketName, sCodeList);
	}


};


#endif //__DRDSLOADLIB__HEADER__
