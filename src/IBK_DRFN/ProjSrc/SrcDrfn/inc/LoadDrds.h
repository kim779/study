#ifndef __DRDSLOADLIB__HEADER__
#define __DRDSLOADLIB__HEADER__


#ifndef __DRDSLOADLIB__DEFINE__
	#define __DRDSLOADLIB__DEFINE__
	#define WM_DRDS_DATA				WM_USER + 200
	#define WM_DRDS_DATA_PACKET_END		WM_USER + 202
	#define MAX_DRDS_PACKET_LEN			4096

	typedef struct  
	{
		char Name[32];
		char Field[32];
		char Key[32];
		UINT Id;
		int  Len;
		char Data[MAX_DRDS_PACKET_LEN];
	} DrdsData;
	#define		SIZE_DrdsData		sizeof(DrdsData)
#endif // __DRDSLOADLIB__DEFINE__


// Def 1-5
typedef  void* (WINAPI* Drds_Init)(HWND);
typedef  BOOL  (WINAPI* Drds_Term)(void*, HWND);
typedef  BOOL  (WINAPI* Drds_Connect)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Close)(void*, HWND, LPSTR);
typedef  BOOL  (WINAPI* Drds_Advise)(void*, HWND,UINT,LPSTR,LPSTR,LPSTR);
// Drds.Def 6-10
typedef  BOOL  (WINAPI* Drds_Unadvise)(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
typedef  BOOL  (WINAPI* Drds_Request)(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
typedef  void  (WINAPI* Drds_PropertySet)();
typedef  void  (WINAPI* Drds_TraceOn)();
typedef  void  (WINAPI* Drds_TraceOff)();
// Drds.Def 11-15
typedef  BOOL  (WINAPI* Drds_SetSocketHandle)(HWND);
typedef  void  (WINAPI*	Drds_SendCode)();
typedef  void  (WINAPI* Drds_SetFlag)(bool bTrace);
typedef  void  (WINAPI* Drds_UDPCreate)();
typedef  void  (WINAPI* Drds_UDPDestroy)();
// Drds.Def 16-20
typedef  void  (WINAPI* Drds_UDPCreate2)(LPSTR strIP, LPSTR strPort);
typedef  void  (WINAPI* Drds_UdpCreate3)(LPSTR sCfgFullPathName);
typedef  BOOL  (WINAPI* Drds_AdviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName);
typedef  int   (WINAPI* Drds_SetUserDefData)(LPSTR sKeyName, LPSTR sData);
typedef  BOOL  (WINAPI* Drds_AdviseRealCodeData)(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList);
// Drds.Def 21-25
typedef  BOOL  (WINAPI* Drds_UnadviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName);
typedef  int   (WINAPI* Drds_SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType);
typedef  BOOL  (WINAPI* Drds_OpenSignalMng)(LPSTR sCfgFullPathName,long lMainHwnd);
typedef  int   (WINAPI* Drds_SignalMng_SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType);
typedef  long  (WINAPI* Drds_GetInterface)(UINT nID);
// Drds.Def 26-
typedef  BOOL  (WINAPI* Drds_SetInterface)(UINT nID, long dwData);

						 
class CDrdsLoadLib
{
//	Constructor..Destructor..
public:
	Drds_Init				m_fpDrdsInit;
	Drds_Term				m_fpDrdsTerm;
	Drds_Connect			m_fpDrdsConnect;
	Drds_Close				m_fpDrdsClose;
	Drds_Advise				m_fpDrdsAdvise;
	Drds_AdviseEx			m_fpDrdsAdviseEx;
	Drds_SetSocketHandle	m_fpDrdsSetSocketHandle;
	Drds_SendCode			m_fpDrdsSendCode;
	Drds_Unadvise			m_fpDrdsUnadvise;
	Drds_UnadviseEx			m_fpDrdsUnadviseEx;
	Drds_Request			m_fpDrdsRequest;
	Drds_PropertySet		m_fpDrdsPropertySet;
	Drds_TraceOn			m_fpDrdsTraceOn;
	Drds_TraceOff			m_fpDrdsTraceOff;
	Drds_SetFlag			m_fpDrdsSetFlag;

	Drds_UDPCreate			m_fpDrdsUDPCreate;
	Drds_UDPCreate2			m_fpDrdsUDPCreate2;
	Drds_UDPDestroy			m_fpDrdsUDPDestroy;
	Drds_UdpCreate3			m_fpDrdsUdpCreate3;
	Drds_SetUserDefData		m_fpDrdsSetUserDefData;
	Drds_AdviseRealCodeData	m_fpDrdsAdviseRealCodeData;
	Drds_SetNewRealData		m_fpDrdsSetNewRealData;
	Drds_OpenSignalMng		m_fpDrdsOpenSignalMng;
	Drds_SignalMng_SetNewRealData	m_fpDrdsSignalMng_SetNewRealData;

	Drds_GetInterface		m_fpDrdsGetInterface;
	Drds_SetInterface		m_fpDrdsSetInterface;


public:
	CDrdsLoadLib()
	{
		m_hLib = NULL;

		m_fpDrdsInit = NULL;
		m_fpDrdsTerm = NULL;
		m_fpDrdsConnect = NULL;
		m_fpDrdsClose = NULL;
		m_fpDrdsAdvise = NULL;
		m_fpDrdsAdviseEx = NULL;
		m_fpDrdsSetSocketHandle = NULL;
		m_fpDrdsSendCode = NULL;
		m_fpDrdsUnadvise = NULL;
		m_fpDrdsUnadviseEx = NULL;
		m_fpDrdsRequest = NULL;
		m_fpDrdsPropertySet = NULL;
		m_fpDrdsTraceOn = NULL;
		m_fpDrdsTraceOff = NULL;
		m_fpDrdsSetFlag = NULL;

		m_fpDrdsUDPCreate = NULL;
		m_fpDrdsUDPCreate2 = NULL;
		m_fpDrdsUDPDestroy = NULL;
		m_fpDrdsUdpCreate3 = NULL;
		m_fpDrdsSetUserDefData = NULL;
		m_fpDrdsAdviseRealCodeData = NULL;
		m_fpDrdsSetNewRealData = NULL;
		m_fpDrdsOpenSignalMng = NULL;
		m_fpDrdsSignalMng_SetNewRealData = NULL;
		m_fpDrdsGetInterface = NULL;
		m_fpDrdsSetInterface = NULL;
	}
	

//	CDrdsLoadLib(LPCSTR _szLibName=NULL)
//	{
//		m_hLib = NULL;
//		SetLibName(_szLibName);
//		m_hLib = LoadLib();
//	}

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
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			TRACE("Drds DLL을 Load할 수 없습니다.\n");
			// AfxMessageBox("Drds DLL을 Load할 수 없습니다.");
			return NULL;
		}
		m_hLib = hLib;
		LoadProcess();
		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}

	void LoadProcess()
	{
		if(!m_hLib)	return ;

		m_fpDrdsInit = (Drds_Init)::GetProcAddress(m_hLib, "DrdsInit");
		m_fpDrdsTerm = (Drds_Term)::GetProcAddress(m_hLib, "DrdsTerm");
		m_fpDrdsConnect = (Drds_Connect)::GetProcAddress(m_hLib, "DrdsConnect");
		m_fpDrdsClose = (Drds_Close)::GetProcAddress(m_hLib, "DrdsClose");
		m_fpDrdsAdvise = (Drds_Advise)::GetProcAddress(m_hLib, "DrdsAdvise");
		m_fpDrdsAdviseEx = (Drds_AdviseEx)::GetProcAddress(m_hLib, "DrdsAdviseEx");
		m_fpDrdsSetSocketHandle = (Drds_SetSocketHandle)::GetProcAddress(m_hLib, "DrdsSetSocketHandle");
		m_fpDrdsSendCode = (Drds_SendCode)::GetProcAddress(m_hLib, "DrdsSendCode");
		m_fpDrdsUnadvise = (Drds_Unadvise)::GetProcAddress(m_hLib, "DrdsUnadvise");
		m_fpDrdsUnadviseEx = (Drds_UnadviseEx)::GetProcAddress(m_hLib, "DrdsUnadviseEx");
		m_fpDrdsRequest = (Drds_Request)::GetProcAddress(m_hLib, "DrdsRequest");
		m_fpDrdsPropertySet = (Drds_PropertySet)::GetProcAddress(m_hLib, "DrdsPropertySet");
		m_fpDrdsTraceOn = (Drds_TraceOn)::GetProcAddress(m_hLib, "DrdsTraceOn");
		m_fpDrdsTraceOff = (Drds_TraceOff)::GetProcAddress(m_hLib, "DrdsTraceOff");
		m_fpDrdsSetFlag = (Drds_SetFlag)::GetProcAddress(m_hLib, "DrdsSetFlag");
		m_fpDrdsUDPCreate = (Drds_UDPCreate)::GetProcAddress(m_hLib, "UDPCreate");
		m_fpDrdsUDPCreate2 = (Drds_UDPCreate2)::GetProcAddress(m_hLib, "UDPCreate2");
		m_fpDrdsUDPDestroy = (Drds_UDPDestroy)::GetProcAddress(m_hLib, "UDPDestroy");
		m_fpDrdsUdpCreate3 = (Drds_UdpCreate3)::GetProcAddress(m_hLib, "UdpCreate3");
		m_fpDrdsSetUserDefData = (Drds_SetUserDefData)::GetProcAddress(m_hLib, "SetUserDefData");
		m_fpDrdsAdviseRealCodeData = (Drds_AdviseRealCodeData)::GetProcAddress(m_hLib, "AdviseRealCodeData");
		m_fpDrdsSetNewRealData = (Drds_SetNewRealData)::GetProcAddress(m_hLib, "SetNewRealData");
		m_fpDrdsOpenSignalMng = (Drds_OpenSignalMng)::GetProcAddress(m_hLib, "OpenSignalMng");
		m_fpDrdsSignalMng_SetNewRealData = (Drds_SignalMng_SetNewRealData)::GetProcAddress(m_hLib, "SignalMng_SetNewRealData");
		m_fpDrdsGetInterface = (Drds_GetInterface)::GetProcAddress(m_hLib, "GetInterface");
		m_fpDrdsSetInterface = (Drds_SetInterface)::GetProcAddress(m_hLib, "SetInterface");
	}

	void* DrdsInit(HWND hWnd)
	{
		if(!m_hLib) return NULL;
		return m_fpDrdsInit(hWnd);
	}
	
	BOOL  DrdsTerm(void *Link, HWND hWnd)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsTerm(Link, hWnd);
	}
	
	BOOL  DrdsConnect(void *Link, HWND hWnd, LPSTR DataName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsConnect(Link, hWnd, DataName);
	}
	
	BOOL  DrdsClose(void *Link, HWND hWnd, LPSTR DataName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsClose(Link, hWnd, DataName);
	}
	
	BOOL  DrdsAdvise(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsAdvise(Link, hWnd, id, DataName, FieldName, KeyName);
	}

	BOOL  DrdsAdviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsAdviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
	}
	
	BOOL  DrdsSetSocketHandle(HWND hWnd)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsSetSocketHandle(hWnd);
	}
	
	void  DrdsSendCode()
	{
		if(!m_hLib) return  ;
		m_fpDrdsSendCode();
	}

	
	BOOL  DrdsUnadvise(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsUnadvise(Link, hWnd, id, DataName, FieldName, KeyName);
	}
	
	BOOL  DrdsUnadviseEx(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsUnadviseEx(nComponentType, hWnd, id, DataName, FieldName, KeyName, ObjectName);
	}
	
	BOOL  DrdsRequest(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsRequest(Link, hWnd, id, DataName, FieldName, KeyName);
	}
	
	void  DrdsPropertySet()
	{
		if(!m_hLib) return  ;
		m_fpDrdsPropertySet();
	}
	
	void  DrdsTraceOn()
	{
		if(!m_hLib) return  ;
		m_fpDrdsTraceOn();
	}
	
	void  DrdsTraceOff()
	{
		if(!m_hLib) return  ;
		m_fpDrdsTraceOff();
	}
	
	void  DrdsSetFlag(bool bTrace)
	{
		if(!m_hLib) return  ;
		m_fpDrdsSetFlag(bTrace);
	}
	
	void  UDPCreate()
	{
		if(!m_hLib) return  ;
		m_fpDrdsUDPCreate();
	}
	
	void  UDPCreate2(LPSTR strIP, LPSTR strPort)
	{
		if(!m_hLib) return  ;
		m_fpDrdsUDPCreate2(strIP, strPort);
	}
	
	void  UDPDestroy()
	{
		if(!m_hLib) return  ;
		m_fpDrdsUDPDestroy();
	}
	
	void  UdpCreate3(LPSTR sCfgFullPathName)
	{
		if(!m_hLib) return  ;
		m_fpDrdsUdpCreate3(sCfgFullPathName);
	}
	
	int   SetUserDefData(LPSTR sKeyName, LPSTR sData)
	{
		if(!m_hLib) return  0;
		return m_fpDrdsSetUserDefData(sKeyName, sData);
	}
	
	BOOL  AdviseRealCodeData(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsAdviseRealCodeData(bIsAdd, nCount, sPacketName, sCodeList);
	}
	
	int   SetNewRealData(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType)
	{
		if(!m_hLib) return  0;
		return m_fpDrdsSetNewRealData(pPacket, nPacketLen, nComponentType, nBrocastType);
	}
	
	BOOL  OpenSignalMng(LPSTR sCfgFullPathName,long lMainHwnd)
	{
		if(!m_hLib) return  FALSE;
		return m_fpDrdsOpenSignalMng(sCfgFullPathName,lMainHwnd);
	}
	
	int   SignalMng_SetNewRealData(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType)
	{
		if(!m_hLib) return  0;
		return m_fpDrdsSignalMng_SetNewRealData(pPacket, nPacketLen, nComponentType, nBrocastType);
	}

	long GetInterface(UINT nID)
	{
		if(!m_hLib) return NULL;
		return m_fpDrdsGetInterface(nID);
	}

	BOOL SetInterface(UINT nID,long lData)
	{
		if(!m_hLib) return FALSE;
		return m_fpDrdsSetInterface(nID, lData);
	}


};


#endif //__DRDSLOADLIB__HEADER__
