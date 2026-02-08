#if !defined(AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_)
#define AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_

// #include "Platform.h"
// **************************************************************
// 2003.02.20 Junok Lee.  mailto:alzioyes@hotmail.com
//
// 두리 플랫폼 관련 부분을 사용하려면
// 01. #include "Platform.h"	// 플랫폼 관련 부분 모음.
// 02. 클래스내에서 USE_DR_PLATFORM() 포함시킨다.
// 03. CtlInterpace.cpp	: 플랫폼관련 추가되는 인터페이스는 여기에 
// 04. CtlPackInOut.cpp : TR I/O 관련 구현
// 05. CtlSharedData.cpp : Local공유관련
// 06. CtlGlobalSharedData.cpp : 글로벌 공유관련
// ==============================================================

// {{ 플랫폼 관련해서 처리하는 부분
#include "CtrlPackBase.h"
#include "GlobalSharedData.h"	// 글로벌공유
#include "LSharedData.h"			// 로컬공유
#include "BaseDesk.h"			// CBaseDesk2관련
//#include "../_COMMON/TrComm.h"		// IAuTrCommManager 사용에 따라 주석처리 ojtaso

#define MakeValInit(xpVal, xxVal)		xpVal=xxVal;
#define	MakeNull(xpVal)	xpVal=NULL;
#define	SafeDelete(xpVal)	if(xpVal) delete xpVal; xpVal=NULL;

// StdAfx.h에서 아래 4줄중 필요한 부분 추가.
//01. #define MakeValInit(xpVal, xxVal)		xpVal=xxVal;
//02. #define	MakeNull(xpVal)	xpVal=NULL;
//03. #define	SafeDelete(xpVal)	if(xpVal) delete xpVal; xpVal=NULL;
//04. #define		IMPLCTRLCLASS			CDispUtil

#define		DGetBaseDeskHWnd()		m_pBaseDesk->GetDeskHwnd();
// }}

// BEGIN_DR_PLATFORM 시작
#define		USE_DR_PLATFORM()		public:					\
	CBaseDesk2*		m_pBaseDesk;	/* CBaseDesk2핸들*/		\
	CResourceManager* m_pResMng;	/* 리소스매니저핸들 */	\
															\
	/* IPlatformObj 인테페이스 구현*/						\
	BEGIN_INTERFACE_PART(PlatformObj, IPlatformObj)								\
		STDMETHOD_(long, PFGetBase)(LPCTSTR szKey) ;							\
		STDMETHOD_(long, PFGetKey)(LPCTSTR szKey) ;								\
				/* szKey : "MainDll", "BaseDesk", "RscMng" */					\
		STDMETHOD_(void, PFSetKey)(LPCTSTR szKey, long dwKey) ;					\
		STDMETHOD_(void, PFInitialize) (long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties) ;	\
		STDMETHOD_(void, PFSetProperty) (STDATAITEM *stData, int nDataCnt);								\
		STDMETHOD_(long, PFGetState) (int nStatus, long dwData1, long dwData2) ;						\
		STDMETHOD_(long, PFSetState) (int nStatus, long dwData1, long dwData2) ;						\
		STDMETHOD_(LRESULT, PFWindowProc) (UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);	\
		STDMETHOD_(LRESULT, PFPreTranslateMessage) (MSG* pMsg) ; \
	END_INTERFACE_PART(PlatformObj)			\
											\
	/* IAWndObj 인테페이스 구현 */			\
	BEGIN_INTERFACE_PART(WndObj, IAWndObj)	\
		STDMETHOD_(void, OnSize) (UINT nType, int cx, int cy);	\
	END_INTERFACE_PART(WndObj)									\
																\
	/* TR IN/OUT 관련 중첩클래스 선언							
	   CBaseCtrlPackIn 을 구현한다.								
	   TR IN에 관여한다.										
	   구현파일 CtlPackInOut.cpp */								\
	class CImplPackIn  : public CBaseCtrlPackIn	\
	{											\
	public:										\
		virtual int GetData(char* buf, int bufLen);		\
	} m_xImplPackIn;									\
	friend class CImplPackIn;							\
														\
	/* CBaseCtrlPackOut 을 구현한다.					
	 TR OUT에 관여한다.								
	 구현파일 CtlPackInOut.cpp */						\
	class CImplPackOut : public CBaseCtrlPackOut		\
	{													\
	public:												\
		CImplPackOut();									\
		virtual int GetPackDataSize(char* pIn);			\
														\
		/* 컨트롤에게 넘겨주는 데이타가 사이즈가 정해진 경우의 데이타 */	\
		virtual int SetData(char* buf, int bufLen);							\
																			\
		/* 컨트롤에게 넘겨주는 데이타가 *인경우일때의 데이타*/				\
		virtual int SetData2(char* buf, int bufLen);						\
																			\
		/* 컨트롤에게 넘겨주는 데이타가 !인경우일때의 데이타*/				\
		virtual int SetData3(char* buf, int bufLen);						\
																			\
		/* 여러타입지정									
		 위에서 언급한 타입도 가능
		 nType  : 1 -> SetData 스타일
		          2 -> SetData2 스타일
		          3 -> SetData3 스타일
		 혹 어떤 스타일이 더 필요하게 될지 모르기 때문에 이 루틴을 추가함.*/	\
		virtual int SetData999(int nType, char* buf, int bufLen);	\
																	\
		char	m_aData[6];											\
		int		m_nLines, m_nBozoSize;								\
	} m_xImplPackOut;												\
	friend class CImplPackOut;										\
																	\
	/* { 공유 데이터 처리 */										\
	CString			m_sLocalSharedList;		/*로컬공유설정 문자열 */	\
	/* Local(화면) SharedData */										\
	ILSharedData*	m_pILSharedData;									\
	ILAdviseSink*	m_pILAdviseSink;									\
	CStringList		m_szLShareName;										\
	CString			m_szLShareData;										\
	BOOL			m_EnableGlobalShare;	/* Global데이터를 세팅할 수 있는 지의 여부를 체크 */ \
	CList	<ST_LSHAREDFORMAT, ST_LSHAREDFORMAT> m_ListLShare;		\
																	\
	BOOL InitLSharedData();		/* Local공유 초기화 */				\
	BOOL InitStart(LPCSTR szShareName, CString& szValue);	/* 로컬공유 초기화시에 처리 */ \
	BOOL InitSet();										\
	BOOL InitGet(LPCSTR szShareName);					\
	BOOL ChangedLShareData(LPCSTR szShareName, LPCSTR szValue);	\
	BOOL ParedShareDataChange(LPCSTR szSharedName, LPCSTR szData);	\
	/* } 공유 데이터 처리 */							\
														\
	CString			m_sGlobalSharedList;	/*글로벌공유설정 문자열*/ \
	/* Global 공유데이터 */											\
	class CGSharedCli : CGlobalSharedClient												\
	{																					\
	public:																				\
		CGSharedCli() : m_pGSharedSvr(NULL),m_bGetGData(FALSE),m_szGSharedData("") {}	\
		CDeskSharedServer*	m_pGSharedSvr;												\
		CList	<ST_GSHAREDFORMAT, ST_GSHAREDFORMAT> m_ListShare;						\
		BOOL GetData(LPCSTR _szName, CString& szValue);									\
		LPCSTR GetData2(LPCSTR _szName);												\
		BOOL SetData(LPCSTR _szName, LPCSTR _szValue);									\
		BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient);						\
		BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient);					\
		void MakeList(CString szList);													\
		POSITION FindShareData(LPCSTR _szName);											\
		/*// 데이터가 변경되었을 대 공유를 발생하도록 요청한다.*/\
		void CallCheckSharedData(LPCSTR _szData);										\
		int  AddSharedNameToServer();													\
		int  RemoveSharedNameToServer();												\
		void IniStartGShareData();														\
		BOOL m_bGetGData;																\
		CString m_szGSharedData;														\
		/*// Virtual*/\
		virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue) ;						\
	} m_xGSharedCli;																	\
	friend class CGSharedCli;

/*//	USE_DR_PLATFORM 끝*/\

// IAuTrCommManager 사용에 따라 주석처리 ojtaso
//#define		USE_DR_TRCOMM()		public:					\
//	/* ICallbackStatus 인테페이스 구현 */							\
//	BEGIN_INTERFACE_PART(CallBack, ICallbackStatus)				\
//		STDMETHOD(ReceiveData) (HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);	\
//		STDMETHOD(StatusChange) (int nState);						\
//	END_INTERFACE_PART(CallBack)										\
//	long		m_lTRKey;		/* 현재 사용하는 TRComm key */		\
//	HRESULT RecieveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen);
///*//	USE_DR_TRCOMM 끝*/\
//
//	
#define		INIT_DR_PLATFORM()			m_pBaseDesk=NULL;		\
	MakeValInit(m_sGlobalSharedList , "");						\
	MakeValInit(m_sLocalSharedList , "" );						\
	MakeValInit(m_pILSharedData , NULL);						\
	MakeValInit(m_pILAdviseSink , NULL);						\
	MakeValInit(m_pResMng, NULL);



//class CGSharedCli : CGlobalSharedClient				
//{													
//public:												
//	CGSharedCli() : m_pGSharedSvr(NULL) {}			
//	CDeskSharedServer*	m_pGSharedSvr;				
//	CList	<ST_GSHAREDFORMAT, ST_GSHAREDFORMAT> m_ListShare;	
//																
//	CString		m_szShareValue;									
//																
//	BOOL		GetData(LPCSTR _szName, CString& szValue);		
//	LPCSTR		GetData2(LPCSTR _szName);						
//	BOOL		SetData(LPCSTR _szName, LPCSTR _szValue);		
//	BOOL		Advise(LPCSTR _szName, CGlobalSharedClient* _pClient);		
//	BOOL		UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient);	
//																			
//	void		MakeList(CString szList);						
//	POSITION	FindShareData(LPCSTR _szName);					
//	/* 데이터가 변경되었을 대 공유를 발생하도록 요청한다. */	
//	void		CallCheckSharedData(LPCSTR _szData);			
//	int			AddSharedNameToServer();						
//	int			RemoveSharedNameToServer();						
//	void		IniStartGShareData();							
//																
//	/* Virtual */												
//	virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue) ;	
//	void		CallCheckSharedData_4Acct(LPCSTR _szData);		
//}; //m_xGSharedCli;												

#endif // !defined(AFX_DRPLATFORM_H__44C03842_6C3B_4036_ABB3_BF9422C675BE__INCLUDED_)
