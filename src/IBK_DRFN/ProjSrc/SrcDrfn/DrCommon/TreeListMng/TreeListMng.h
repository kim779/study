// TreeListMng.h : main header file for the TREELISTMNG DLL
//

#if !defined(AFX_TREELISTMNG_H__4108172C_1F85_4727_B4CD_8219A5854FB2__INCLUDED_)
#define AFX_TREELISTMNG_H__4108172C_1F85_4727_B4CD_8219A5854FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "AgentTreeList.h"

#include "../../inc/IGateMng.h"
#include "../../inc/IAUGwanMng.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeListMngApp
// See TreeListMng.cpp for the implementation of this class
//

class CTreeListMngApp : public CWinApp
{
public:
	CTreeListMngApp();

	void AppTerminate();
	void NotifyGwamDataChange(long dwKey, long dwSubKey, long dwNotifyData);
	static void CALLBACK DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeListMngApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTreeListMngApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CTreeListManager*	m_pTreeListMng;
	IGateManager*		m_pIGateMng;
	long				m_dwDataMon;

	long GetGwansimDataAll(long pSGwanInterface);
 	void GetGwansimGroupName(long pSGwanInterface, CStringArray &arrKey, CStringArray &arrName);
 	void GetGwansimItem(long pSGwanInterface, CString strKey, CStringArray& arrCode, CStringArray& arrName);

};

extern CTreeListMngApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREELISTMNG_H__4108172C_1F85_4727_B4CD_8219A5854FB2__INCLUDED_)
