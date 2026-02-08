#if !defined(AFX_WNDMAPVIEW_H__14E0C72A_8A4D_4E25_A83F_EBC00F0C9B57__INCLUDED_)
#define AFX_WNDMAPVIEW_H__14E0C72A_8A4D_4E25_A83F_EBC00F0C9B57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndMapView.h : header file
//
//#include "./control/any2desk.h"
#include "../../../include_base/baseDesk.h"
#include "../../../include_base/XfmMapInfo.h"		// NEOPLATFORM Map Open Supports
#include "../../../include_base/ResourceManager.h"
#include "../include/MsgWnd.h"
#include "../../INCLUDE_CONTENTS/baseGShareMng.h"
#include <afxmt.h>
/////////////////////////////////////////////////////////////////////////////
// CWndMapView window

class CAcctHelper;
class CContextMenuManager;
class CWndMapView : public CWnd
{
// Construction
public:
	CWndMapView();
	CWndMapView(LPSTR szFile, LPSTR szRootPath, HWND hSocket, LPVOID pMainRscMng, LPVOID pColorTable, BOOL bMsgBox, HWND hParentView, BOOL bUseSetTR);

// Attributes
public:
	CANY2Desk	m_ctrlNeoDesk;

	LPSTR		m_szRootPath;			// MainDll
	LPSTR		m_szFile;
	LPSTR		m_szUserPath;
	HWND		m_hSocket;				// MainDll
	int			m_nWidth;
	int			m_nHeight;
	BOOL		m_bHasMessageBar;
	LPVOID		m_pColorTable;			// MainDll
	CDeskSharedServer*	m_pDSS;

	CMessageWindow*			m_pMessageMng;
	CResourceManager*		m_pResourceManager;
	CMainResourceManager*	m_pMainRscManager;	// MainDll
	CContextMenuManager*	m_pMnManager;		// 맵 컨택스트 메뉴 매니저

	HWND		m_hWndParentView;

	char		m_szMapID[6];
	BOOL		m_bUseSetTR;
// Operations
public:
	void	GetMapInfo(LPSTR szFile);
	BOOL	SetDlgFormSize(LPSTR szFile);
	void	CreateResourceManager();
	void	DeleteResourceManager();
	void	CallCommand(LPCSTR szCmd);
	void	SetGShareData(LPVOID pShareData);
	void	GetGShareData(LPSTR szValue, CString& strValue);
	void	SettingData(LPCTSTR _szName, LPCTSTR _szValue);
	void	InitGShareDataAccount();
	void	SetParentAllShareData();
	void	SetParentShareData(LPCTSTR _szName, LPCTSTR _szValue);
	void	RefreshPopupMenu();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndMapView)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	class CXBDeskParent : public CBaseDeskParent
	{
	public:
		virtual LPCSTR GetBasePath() ;
		virtual HWND  GetParentHwnd() ;
		virtual LPDISPATCH GetParentDisp() ;

		virtual long GetBaseCodeTable();
		virtual BOOL OpenMap(LPCSTR _szMapID, int _nFlag, LPCSTR _szParam);
		virtual BOOL InvokeEx1(LPCSTR _szFunc, LPCSTR _szParam, CString& szResult);
		virtual BOOL InvokeEx2(LPCSTR _szFunc, long _dwParam, long& dwResult);

		virtual void CallCommand(LPCSTR _szCmd);
		virtual void CallCommand(HWND _hDesk, LPCSTR _szCmd);
		virtual long GetKeyData(LPCSTR _szKey1, LPCSTR _szKey2);
		virtual long GetKeyData(LPCSTR _szKey, int _nKey);

	// Property
		CString		m_szBasePath;
		HWND		m_pParentHwnd;
		LPDISPATCH	m_pParentDisp;
		
	public:
		// CBaseDeskParent2에서 추가되는 내용들
		virtual long GetUserInfo(LPCSTR szKey);
		virtual long GetFrameInfo(LPCSTR szKey);
		virtual long GetMainInfo(LPCSTR szKey);
	} m_xDeskParent;
	friend class CXBDeskParent;

	class CGSharedMng  : public CBaseChildShareManager
	{
	public:
		CGSharedMng();
		virtual ~CGSharedMng();
		
		virtual BOOL	GetData(LPCTSTR _szName, CString& _szValue);
		virtual BOOL	SetData(LPCTSTR _szName, LPCTSTR _szValue, void* _pSvr);
		virtual LPCTSTR GetData2(LPCTSTR _szName);
		
		virtual BOOL	Advise(CDeskSharedServer* _pServer);
		virtual BOOL	UnAdvise(CDeskSharedServer* _pServer);
		
		BOOL		ChangeData(LPCSTR _szName, LPCSTR _szValue, CDeskSharedServer* _pDeskSvr);
		
		void		RemoveAll();
		BOOL		AddData(LPCTSTR _szName, LPCTSTR lpszFormat, ...);
		BOOL		RemoveData(LPCTSTR _szName, LPCTSTR _szValue);
		BOOL		SetOrAddData(LPCTSTR _szName, LPCTSTR _szValue, CDeskSharedServer* _pSvr, BOOL _bNotify, BOOL _bAddValue=FALSE);
		BOOL		SetOrAddDataLocal(LPCTSTR _szName, LPCTSTR _szValue, CDeskSharedServer* _pSvr, BOOL _bNotify, BOOL _bAddValue=FALSE);
		BOOL		NotifyDataToAll(LPCTSTR _szName, LPCTSTR _szValue, CDeskSharedServer* _pSvr);
		POSITION	FindData(LPCTSTR _szName);
		BOOL		OnlyAdd_SingleData(LPCSTR _szName, LPCSTR _szValue, BOOL _bAddValue, BOOL _bChangeDataOnly = FALSE);
		BOOL		OnlyAdd_SingleDataLocal(LPCSTR _szName, LPCSTR _szValue, BOOL _bAddValue, BOOL _bChangeDataOnly = FALSE);
				
		ListShareData* GetShareList();
		
		LPSTR		GetLastUseAccount() {return m_szLastUseAccount;}
		void		SetAcctHelper(CAcctHelper* pAcctHelper) {m_pAcctHelper = pAcctHelper;}
		
	protected:
		CCriticalSection	m_sect;
		CListGlobalServer*	m_pData;
		// 다이얼로그메인:공유관리자 수정 - ojtaso(2004/2/10 017-391-1052)
		CAcctHelper*		m_pAcctHelper;
		
		list<CShareData*>	m_lstShareDataLocal;
		
		//* 수정 시작 (2003.2.11 1) 이남경 계좌관련 수정
		CStringList		m_ListUserAcct;
		//* 수정 끝 (2003.2.11 1) 
		
		char		m_szLastUseAccount[12];
	} m_xGSharedMng;
	friend class CGSharedMng;

	CBaseChildShareManager* m_pParentShare;

// Implementation
public:
	virtual ~CWndMapView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndMapView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnMapMessageNeodeskctrl1(short _nType, LPCTSTR _szMessage, short _nLen);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CWndMapView)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDMAPVIEW_H__14E0C72A_8A4D_4E25_A83F_EBC00F0C9B57__INCLUDED_)
