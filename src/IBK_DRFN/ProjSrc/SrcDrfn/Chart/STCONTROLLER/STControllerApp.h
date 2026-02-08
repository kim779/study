// STController.h : main header file for the STCONTROLLER DLL
//

#if !defined(AFX_STCONTROLLER_H__8E9AB96B_DCA6_4FA9_88E8_40D6EBB5A1D8__INCLUDED_)
#define AFX_STCONTROLLER_H__8E9AB96B_DCA6_4FA9_88E8_40D6EBB5A1D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <afxtempl.h>
//#include "../../../DataMgr/DBMgr.h"


#include "../Include_ST/InnerFuncLoader.h"
#include "../Include_ST/InnerFunction.h"

/////////////////////////////////////////////////////////////////////////////
// CSTControllerApp
// See STController.cpp for the implementation of this class
//

class CApQurier;
class CSTControllerApp : public CWinApp
{
public:
	CSTControllerApp();
	long GetSTController();
	long GetSTControllerCfg();
	BOOL Lookup(long lType, LPCTSTR lpFuncName,CInnerFunction *&pInnerFunction);

	// 20060705 함수인자 제거 : ,HWND hSocket, long pDBMgr
	//CApQurier *GetAPQurier(CWnd *pWnd,HWND hSocket, long pDBMgr);
	CApQurier *GetAPQurier(CWnd *pWnd);

	void DeleteAPQurier(CApQurier *pAPQurier);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTControllerApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSTControllerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CInnerFuncLoader m_innerFuncLoader;
	CInnerFuncLoader m_innerFuncLoader2;
	CTypedPtrMap< CMapStringToPtr, CString, CInnerFunction *> m_mapInnerFunction0;
	CTypedPtrMap< CMapStringToPtr, CString, CInnerFunction *> m_mapInnerFunction1;
	CMap<CWnd*,CWnd*,CApQurier*,CApQurier*> m_mapApQurier;
	CMap<CWnd*,CWnd*,CApQurier*,CApQurier*> m_mapApQurierForIndex;
	CMap<CApQurier*,CApQurier*,int,int> m_mapReferencedCnt;	
	CMap<CApQurier*,CApQurier*,int,int> m_mapReferencedCntForIndex;	
	int m_IDIncreaser;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STCONTROLLER_H__8E9AB96B_DCA6_4FA9_88E8_40D6EBB5A1D8__INCLUDED_)
