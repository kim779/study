// PctrWinix.h : main header file for the PCTRWINIX DLL
//

#if !defined(AFX_PCTRWINIX_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_)
#define AFX_PCTRWINIX_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// #include "../../inc/IGateMng.h"
// #include "../../inc/IAUGwanMng.h"
// #include "../../inc/IWinixPlatform.h"
// #include "../../Inc/IAUTrCommMng.h"

#include "./LocalHeader.h"
#include "./CodeManagerWnd.h"

//#include "../../../SrcSite/include/FxInterestManager.h"

class CGateManager : public IGateManager
{
public:	// Implementation
	STDMETHOD_(LONG, GetInterface)(int nID, long dwKey=0);
	
public:	// Variable
	
};
//tour2k
typedef struct
{
	CStringArray strKey;
	CStringArray strGroupName;
} ST_GwanSimGroup;
// IAUGwanManager
// 관심그룹 관련 인터페이스.
class CAUGwanManager : public IAUGwanManager
{
	// public:
	STDMETHOD_(long, Advise)(LPCSTR szKey);
	STDMETHOD_(void, UnAdvise)(long dwRet);
	
public:
	
	STDMETHOD_(long, GetGwansimDataAll)(long dwRet, int nType, LPCSTR szGroup=NULL);
	
	STDMETHOD_(void, GetGwansimGroupName)(long dwRet, CStringArray &arrKey, CStringArray &arrName);
	STDMETHOD_(void, GetGwansimItem)(long dwRet, CString strKey, CStringArray& arrCode, CStringArray& arrName);
	STDMETHOD_(int,  ShowGwansimDlg)(long dwRet, CWnd* pCaller, CString strCodeList="");	
	STDMETHOD_(BOOL, ADDGwasimItem)(long dwRet, CString strGroup, CString strItem, BOOL bFirst=FALSE);
//	STDMETHOD_(BOOL, ADDGwasimItems)(long dwRet, CString strGroup, CStringArray& arrItem);
	STDMETHOD_(int,  SetRegGwansim)(long dwRet, CWnd* pComboWnd, int nType);
	STDMETHOD_(int,  RegiestToImsiGroup)(long dwRet, CStringArray& arrCode, CWnd* pWnd);
	
	//그룹의 키값리턴
	STDMETHOD_(LPCSTR, GetGwanKeyFromName)(long dwRet, LPCSTR szGwanGroupName);
	STDMETHOD_(int,  ShowGwansimDlgWithList)(long dwRet, CWnd* pCaller, CStringArray& arrItem);

	//위닉스에서 받은 관심 종보를 Parsing해서 필요한 정보만 넘기기
	STDMETHOD_(int, ParseGwanList)(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList);

	//서버에 관심그룹 리스트 요청.
	STDMETHOD_(void, RequestGwansimGroup)(long dwRet);

	//위닉스에서 관심그룹 파싱
	STDMETHOD_(int, ParseGroup)( LPARAM lParam, CStringArray& arrCode, CStringArray& arrName);
public:
	CAUGwanManager();
	~CAUGwanManager();

protected:
	List_CWinixGwanItem m_List;
	void _DoClearList();
	CWinixGwanItem* _GetKeyItem(long dwRet);
	ST_GwanSimGroup m_GwanSimGroup;
};

/////////////////////////////////////////////////////////////////////////////
// CPctrWinixApp
// See PctrWinix.cpp for the implementation of this class
//

#define MAX_Partner_INTERFACE	10
typedef struct 
{
	LONG pInterface;
} STUA_INTERFACE;

class CPctrWinixApp : public CWinApp
{
public:
	CPctrWinixApp();

	// 인터페이스 관리 변수(Array)
	STUA_INTERFACE m_uaInterface[MAX_Partner_INTERFACE];

	// No.0 IGageManager
	CGateManager		m_GateMng;
	CAUGwanManager		m_GwanMng;

	CWinixGwanGroupList m_WinixGList;	//관심List관리용.

	//CPartnerSvrManager	m_PtSvrMng;
	CCodeManagerWnd		m_CodeMasterWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPctrWinixApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPctrWinixApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CPctrWinixApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCTRWINIX_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_)
