// ExSignal.h: interface for the CExSignal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXSIGNAL_H__0DF45BCC_C316_4D02_80A3_D848BD32ED88__INCLUDED_)
#define AFX_EXSIGNAL_H__0DF45BCC_C316_4D02_80A3_D848BD32ED88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderSignal;
class CExSignal  
{
public:
	CExSignal();

// Attributes
protected:
	CLoaderSignal*		m_pLoader;
	CDialog*				m_pParent;
	BOOL					m_bCreated;

	void	SetMyBaseDesk2(CBaseDesk2* pBaseDesk);

// Operations
public:
	BOOL	InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSignal(CDialog* pParent, UINT nID, UINT nProperties, CBaseDesk2* pBaseDesk = NULL);

	BOOL	InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSignal(CDialog* pParent, UINT nID, CStringList* pListProperty, CBaseDesk2* pBaseDesk = NULL);

	BOOL	InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSignal(CDialog* pParent, UINT nID, LPCTSTR* lpszProperties, int nSize, CBaseDesk2* pBaseDesk = NULL);

	BOOL	InitSignal(CDialog* pParent, CRect& rcControl, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);
	BOOL	InitSignal(CDialog* pParent, UINT nID, LPCTSTR lpszProperties, CBaseDesk2* pBaseDesk = NULL);

	virtual BOOL InvokeEvent(LPCSTR lpszEventName, LPCSTR lpszParam);

	OLE_HANDLE	GetHWnd();		// OLE_HANDLE
	HWND	GetSafeHwnd();		// HWND

	void	SetFocus();
	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);
	BOOL	OnDrdsAdvise(LPCTSTR lpszCode);
	BOOL	OffDrdsAdvise();

protected:
	class CX2000BaseDesk : public CBaseDesk2
	{
	public:
		LPDISPATCH m_pDisp;
		CX2000BaseDesk() : m_pDisp(NULL) {}
		~CX2000BaseDesk() { if(m_pDisp) m_pDisp->Release(); }

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

		friend class CExSignal;
		
	} m_x2000BaseDesk;
	friend class CX2000BaseDesk;

	CX2000BaseDesk*		m_pBaseDesk2;

// Implementation
public:
	virtual ~CExSignal();
};

#endif // !defined(AFX_EXSIGNAL_H__0DF45BCC_C316_4D02_80A3_D848BD32ED88__INCLUDED_)
