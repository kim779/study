// SystemTradingExtLib.h : main header file for the SYSTEMTRADINGEXTLIB DLL
//

#if !defined(AFX_SYSTEMTRADINGEXTLIB_H__E0F94C01_906F_40E6_BBB9_FA4DDA48C2C7__INCLUDED_)
#define AFX_SYSTEMTRADINGEXTLIB_H__E0F94C01_906F_40E6_BBB9_FA4DDA48C2C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../Include_ST/StructDefine.h"
#include "../Include_ST/DeclareDefine.h"

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <vector>
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO



/////////////////////////////////////////////////////////////////////////////
// CSystemTradingExtLibApp
// See SystemTradingExtLib.cpp for the implementation of this class
//

class CInnerFunctionInfo;
class CScriptInfomation;
class CLoadSystemTradingParserHelper;
class CSystemTradingExtLibApp : public CWinApp
{
public:
	CSystemTradingExtLibApp();

	BOOL			m_bInitLib;
	char			m_szRootPath[MAX_PATH];
	char			m_szUserPath[MAX_PATH];

	vector<FpNotifyList>	m_vtNotifyFunction;

	CList<ST_SCRIPTINFO*, ST_SCRIPTINFO*>	m_listScriptInfo;
	vector<CScriptInfomation*>				m_vtUserFuncInfo;
	vector<CInnerFunctionInfo*>				m_vtInnerFuncInfo;

	CLoadSystemTradingParserHelper*			m_pLoadPaser;

	enum {IMG_TREE = 0, IMG_STS, IMG_STS_LOCK, IMG_STS_CERT, IMG_STS_LOCK_CERT, IMG_STS_DLL, IMG_STS_LOCK_DLL,
						IMG_FUNC, IMG_FUNC_LOCK, IMG_FUNC_CERT, IMG_FUNC_LOCK_CERT, IMG_FUNC_DLL, IMG_FUNC_LOCK_DLL};

	void	ClearAllScriptInfo();
	void	ListupScriptFile();
	void	InitUserFuntionList();
	void	InitInnerFunctionList(LPSTR lpszFileName);
	void	LoadSystemTradingParser();
	void	DeleteSystemTradingParser();
	void	AddNotifyFunction(FpNotifyList pFunction);
	void	DeleteNotifyFunction(FpNotifyList pFunction);
	void	NotifyFunction();
	BOOL	DecodingFile(LPCSTR lpszSource, LPCSTR lpszDest, LPSTR lpszLine1 = NULL, LPSTR lpszLine2 = NULL, LPSTR lpszLine3 = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingExtLibApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSystemTradingExtLibApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGEXTLIB_H__E0F94C01_906F_40E6_BBB9_FA4DDA48C2C7__INCLUDED_)
