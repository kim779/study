#if !defined(AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)
#define AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExCodeInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExCodeInput window

class CLoaderCodeInput;
class CExCodeInput
{
// Construction
public:
	CExCodeInput();

// Attributes
protected:
	CLoaderCodeInput*		m_pLoader;
	CDialog*				m_pParent;
	BOOL					m_bCreated;

	void	SetMyBaseDesk2(CBaseDesk2* pBaseDesk);

// Operations
public:

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/22
	// Return		: short 입력된 코드의 구분값
	// Paramter
	//		LPCTSTR szCode : 코드번호 문자열
	// Comments		: 종목코드, 업종코드등의 구분값을 구한다
	//-----------------------------------------------------------------------------
	short	GetGubunFromCode(LPCTSTR szCode);
	
	void	SetDataText(LPCTSTR szText);
	void	SetDataText2(LPCTSTR szCode);
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/22
	// Return		: CString
	// Paramter
	//		
	// Comments		: 
	//-----------------------------------------------------------------------------
	CString GetDataText();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 코드인풋 초기화(생성) 성공여부
	// Paramter	
	//		CDialog* pParent : 화면dll의 Dialog(CExOCXDLL가 Parent인 Dialog)
	//		CRect& rcControl : 코드 컨트롤의 Rect
	//					   ResourceEditor에서 Rect값을 구하기 위해 컨트롤을 사용했다면 
	//					   pRect값을 입력할 필요없습니다.
	//		UINT nID : 코드컨트롤 리소스 ID 
	//				   리소스 에디터에서 Rect값을 구하기 위해 컨트롤을 사용했다면 그 컨트롤의 
	//				   ID를 입력하십시오. 컨트롤의 Rect를 구해서 코드컨트롤을 생성합니다.
	//		UINT nProperties : 일반적으로 사용되는 속성 구분값
	// Comments		: ocx기반 컨트롤인 코드인풋을 생성합니다.
	//				  CBaseDesk2* 를 넘겨 데스크의 정보를 코드인풋에게 세팅하고
	//				  코드인풋으로부터 이벤트를 받을 수 있도록 합니다.
	//				  코드인풋을 사용하기 위해서는 반드시 호출해야 하는 함수입니다.
	//				  일반적으로 사용되는 속성들을 미리 정의해 놓고 구분값으로 설정합니다.
	//
	//		_INCLUDE/ExCommonHeader.h
	//		const UINT CODEINPUT_PROP_STOCK		=	0;		// 주식 (기본)
	//		const UINT CODEINPUT_PROP_FO		=	1;		// 선물,옵션
	//		const UINT CODEINPUT_PROP_FUT		=	2;		// 선물
	//		const UINT CODEINPUT_PROP_OPT		=	3;		// 옵션
	//		const UINT CODEINPUT_PROP_ITEM		=	4;		// 업종
	//				  설정은 추후 추가될 수 있습니다.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 코드인풋 초기화(생성) 성공여부
	// Paramter	
	//		CDialog* pParent : 화면dll의 Dialog(CExOCXDLL가 Parent인 Dialog)
	//		CRect& rcControl : 코드 컨트롤의 Rect
	//					   ResourceEditor에서 Rect값을 구하기 위해 컨트롤을 사용했다면 
	//					   pRect값을 입력할 필요없습니다.
	//		UINT nID : 코드컨트롤 리소스 ID 
	//				   리소스 에디터에서 Rect값을 구하기 위해 컨트롤을 사용했다면 그 컨트롤의 
	//				   ID를 입력하십시오. 컨트롤의 Rect를 구해서 코드컨트롤을 생성합니다.
	//		CStringList* pListProperty : 코드인풋에 필요한 속성문자열을 가지고 있는 리스트 포인터
	// Comments		: ocx기반 컨트롤인 코드인풋을 생성합니다.
	//				  CBaseDesk2* 를 넘겨 데스크의 정보를 코드인풋에게 세팅하고
	//				  코드인풋으로부터 이벤트를 받을 수 있도록 합니다.
	//				  코드인풋을 사용하기 위해서는 반드시 호출해야 하는 함수입니다.
	//				  StringList를 초기 속성으로 사용합니다. StringList는 모든 속성을 포함하여야 합니다.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 코드인풋 초기화(생성) 성공여부
	// Paramter	
	//		CDialog* pParent : 화면dll의 Dialog(CExOCXDLL가 Parent인 Dialog)
	//		CRect& rcControl : 코드 컨트롤의 Rect
	//					   ResourceEditor에서 Rect값을 구하기 위해 컨트롤을 사용했다면 
	//					   pRect값을 입력할 필요없습니다.
	//		UINT nID : 코드컨트롤 리소스 ID 
	//				   리소스 에디터에서 Rect값을 구하기 위해 컨트롤을 사용했다면 그 컨트롤의 
	//				   ID를 입력하십시오. 컨트롤의 Rect를 구해서 코드컨트롤을 생성합니다.
	//		LPCTSTR* lpszProperties : 속성 문자열의 배열
	//		int nSize : 배열의 수
	//					_countof() 매크로를 이용하십시오.
	// Comments		: ocx기반 컨트롤인 코드인풋을 생성합니다.
	//				  CBaseDesk2* 를 넘겨 데스크의 정보를 코드인풋에게 세팅하고
	//				  코드인풋으로부터 이벤트를 받을 수 있도록 합니다.
	//				  코드인풋을 사용하기 위해서는 반드시 호출해야 하는 함수입니다.
	//				  모든 속성이 포함된 문자열 배열을 사용합니다.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 코드인풋 초기화(생성) 성공여부
	// Paramter	
	//		CDialog* pParent : 화면dll의 Dialog(CExOCXDLL가 Parent인 Dialog)
	//		CRect& rcControl : 코드 컨트롤의 Rect
	//					   ResourceEditor에서 Rect값을 구하기 위해 컨트롤을 사용했다면 
	//					   pRect값을 입력할 필요없습니다.
	//		UINT nID : 코드컨트롤 리소스 ID 
	//				   리소스 에디터에서 Rect값을 구하기 위해 컨트롤을 사용했다면 그 컨트롤의 
	//				   ID를 입력하십시오. 컨트롤의 Rect를 구해서 코드컨트롤을 생성합니다.
	//		LPCTSTR lpszProperties : 원하는 일부분의 속성문자열
	// Comments		: ocx기반 컨트롤인 코드인풋을 생성합니다.
	//				  CBaseDesk2* 를 넘겨 데스크의 정보를 코드인풋에게 세팅하고
	//				  코드인풋으로부터 이벤트를 받을 수 있도록 합니다.
	//				  코드인풋을 사용하기 위해서는 반드시 호출해야 하는 함수입니다.
	//				  모든 속성을 설정하지 않고 일부분만 지정할 수 있습니다.
	//				  기본속성은 주식입니다. 기본속성에 일부분만 변경할 때에는 
	//				  LPCTSTR lpszProperties = _T("CD=1:CH=1:CT=1"); 으로 가능합니다.
	//				  속성간의 구분값은 콜론':'입니다.
	//-----------------------------------------------------------------------------
	BOOL	InitCodeInput(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitCodeInput(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 이벤트를 받았는지 여부(항상 TRUE)
	// Paramter
	//		LPCSTR lpszEventName : 코드인풋으로부터 시작된 이벤트의 이름입니다.
	//		LPCSTR lpszParam : 이벤트로부터 넘겨지는 파라메터 입니다.
	// Comments		: 코드인풋에서 발생한 이벤트를 넘겨받는 함수입니다.
	//				  virtual로 선언되어있으므로 CExCodeInput를 상속받아 사용하는 class에서
	//				  다시 선언하여 사용할 수 있습니다.
	//				  이벤트의 종류와 파라메터 정보는 아래와 같습니다.
	//			[id(1)] void SendTr(BSTR szTrList);
	//			[id(2)] void SelChanged(short nIndex, BSTR selectedCode);
	//			[id(3)] void LengthFull(BSTR szText);
	//			[id(4)] void Enter();
	//			[id(5)] void OnChar(BSTR bstrChar);
	//			[id(6)] void CodeNotFound(BSTR szCode);
	//			[id(DISPID_KEYUP)] void KeyUp();
	//			[id(DISPID_KEYDOWN)] void KeyDown();
	//			[id(7)] void HistoryChanged();
	//				  이벤트는 코드인풋ocx가 추가/삭제됨에 따라 화면 dll 처리도 수정될 수 있습니다.
	//
	//				  화면 Dialog에서 이벤트를 받을때에는 RMSG_EVENT_CODEINPUT 메세지로 받습니다.
	//				  RMSG_EVENT_CODETINPUT는 _INCLUDE/ExCommonHeader.h에 정의되어있습니다.
	//				  WPARAM은 이벤트가 발생한 컨트롤의 핸들입니다.
	//				  LPARAM은 이벤트이름과 파라메터입니다. LPST_CTRLEVENT 구조체 포인터로 넘겨집니다.
	//				  LPST_CTRLEVENT도 _INCLUDE/ExCommonHeader.h에 정의되어있습니다.
	//				  사용 예)
	//
	//			LRESULT CStdDialog::OnEventCodeInput(WPARAM wParam, LPARAM lParam)
	//			{
	//				HWND hCtrl = (HWND)wParam;
	//
	//				if(m_CodeInput.GetSafeHwnd() == hCtrl)
	//				{
	//					LPST_CTRLEVENT pEvent = (LPST_CTRLEVENT)lParam;
	//
	//					LPCSTR szEvent = pEvent->lpszEventName;
	//					LPCSTR szParam = pEvent->lpszParam;
	//				}
	//				else if(m_CodeInput2.GetSafeHwnd() == hCtrl)
	//				{
	//					LPST_CTRLEVENT pEvent = (LPST_CTRLEVENT)lParam;
	//
	//					LPCSTR szEvent = pEvent->lpszEventName;
	//					LPCSTR szParam = pEvent->lpszParam;
	//				}
	//				
	//				return 0L;
	//			}
	//-----------------------------------------------------------------------------
	virtual BOOL InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam);

	void	SetEnabled(BOOL bEnable);
	BOOL	GetEnabled();

	long	GetBase(LPCTSTR lpszKey);

	void	SetFocus();

	void	SetCurSel(short nIndex);

	OLE_HANDLE	GetHWnd();		// OLE_HANDLE
	HWND	GetSafeHwnd();		// HWND

	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);

protected:
	class CX2000BaseDesk : public CBaseDesk2
	{
	public:
		LPDISPATCH m_pDisp;
		CX2000BaseDesk() : m_pDisp(NULL) {}
		~CX2000BaseDesk() { if(m_pDisp) m_pDisp->Release(); }

//		void Initialize();

		// * CBaseDesk Virtual 구현
		virtual LPCSTR GetBasePath() ;
		virtual HWND  GetDeskHwnd() ;
		virtual LPDISPATCH GetUnknown();
		virtual LPUNKNOWN GetCtrlUnknown(LPCTSTR szName) ;
		virtual HWND GetCtrlHwnd(LPCTSTR szName);
		virtual LPVOID GetSharedInterface();
		virtual BOOL InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam);
		virtual BOOL SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl);
		virtual LPVOID GetLSharedData();
		virtual void CallCommand(LPCSTR _szCmd);

		// CBaseDesk2에서 추가되는 내용들
		virtual OLE_COLOR GetUserColor(short nIndex, LPCTSTR szName, short nFlag);
		virtual long GetDeskInfo(LPCSTR szKey);
		virtual long GetUserInfo(LPCSTR szKey);
		virtual long GetFrameInfo(LPCSTR szKey);
		virtual long GetMainInfo(LPCSTR szKey);

//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
		virtual BOOL FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual void FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand);
		virtual LPCSTR FP_sGetMyVBName(HWND hWnd);
		virtual void FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue);
//* 수정 끝 (2002.8.22 001) 

		// CControlPos의 m_awndControls 맴버변수의 포인터를 리턴한다.
		// 이 변수는 리사이즈 될 컴포넌트의 정보를 들고 있다.
		virtual long FP_nGetCtrlPos();

		// 컴포넌트를 리사이즈 시킨다.
		virtual void FP_vResizeCtrl();

		// {{ OCX에서 DESK로 메세지 보낼 수 있도록 요청(김용식) 2003.05.19
		virtual void SetMapMessage(long lType, LPCSTR lpData);
		// }}
		
		friend class CExCodeInput;
		
	} m_x2000BaseDesk;
	friend class CX2000BaseDesk;

	CX2000BaseDesk*		m_pBaseDesk2;

// Implementation
public:
	virtual ~CExCodeInput();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCODEINPUT_H__3511D941_A193_4AAB_8EFF_82C0E8F6BEC3__INCLUDED_)
