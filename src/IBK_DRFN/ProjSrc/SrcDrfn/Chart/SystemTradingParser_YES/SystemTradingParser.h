// SystemTradingParser.h : main header file for the SYSTEMTRADINGPARSER DLL
//

#if !defined(AFX_SYSTEMTRADINGPARSER_H__87C2D336_731E_406C_BFE8_E8B77A20D8BF__INCLUDED_)
#define AFX_SYSTEMTRADINGPARSER_H__87C2D336_731E_406C_BFE8_E8B77A20D8BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../Include_ST/StructDefine.h"

#ifndef _USESTD_OJTASO

#define _USESTD_OJTASO
#include <vector>
#include <algorithm>
using namespace std;

#endif // #ifndef _USESTD_OJTASO

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingParserApp
// See SystemTradingParser.cpp for the implementation of this class
//

class SysTradingParser;
class CSystemTradingParserApp : public CWinApp
{
public:
	CSystemTradingParserApp();
	
	void	DeleteCurBuffer();
	void	ListupFunction();
	int		ParseFileToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo);
	int		ParseMainToCPP(LPSTR p_szFile, CErrorInfomation* pErrorInfo, BOOL& bIndexFunction, CScriptSecurity* pInfo);
	int		ParseUserFunctionToCPP(LPSTR p_szFile, LPSTR lpszUserFunctionName, CErrorInfomation* pErrorInfo, CScriptSecurity* pInfo);
	BOOL	ParseFileToVBS(LPSTR p_szFile);
	void	DeleteAllVector();
	void	InsertToTemplateFileCPP();
	void	InsertToTemplateFileVBS(UINT p_nUsedMemVariable);
	
	BOOL	InsertToMainFileCPP(SysTradingParser* p_pSysTradingParser, CStringArray* p_pArrLine, CScriptSecurity* pInfo, CErrorInfomation* pErrorInfo);
	void	InsertToUserFunctionFileCPP(SysTradingParser* p_pSysTradingParser, LPSTR p_lpszFunction, CStringArray* p_pArrLine, CScriptSecurity* pInfo, CErrorInfomation* pErrorInfo);

	void	ParseOutputScript(LPVOID p_pOutput, UINT& p_nSize);
	void	WriteMemberVariable(UINT p_nUsedMemVar, CStdioFile* p_pFile);
	void	WriteEndStatement(SysTradingParser* pSysParser);

	void	CheckScriptStatement(LPSTR p_szFile, LPSTR lpszPath);
	void	WriteInnerFuncInfo(LPCSTR lpszFuncName, LPSTR lpszPath, CStringArray* pStrLine);
	void	WriteInputVariable(LPCSTR lpszInputName, LPCSTR lpszInputInit, int nIndex, LPSTR lpszPath);
	BOOL	ChangeIndexString(CString& strName, LPSTR lpszPath);
	void	ParseFunctionArguments(CString& strLine);
	void	GetUsedUserFunction(LPSTR p_szFile, VT_DECLDATA* pVtUsedUserFunction);
	void	GetUsedUserAllFunction(LPSTR p_szFile, VT_DECLDATA* pVtUsedUserFunction);

	void	SetStrategyType(LPSTR p_lpszName);

public:
	LPSTR	m_szRootPath;
	CString m_szUserPath;
	
	VT_SCRIPTINFO*		m_pVtUserFuncInfo;
	VT_INNERFUNCINFO*	m_pVtInnerFuncInfo;
	
//	vector<LPSTR>	m_vtDefFunction;
	vector<LPSTR>	m_vtUserFunction;
	vector<LPSTR>	m_vtMemberVariable;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingParserApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSystemTradingParserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGPARSER_H__87C2D336_731E_406C_BFE8_E8B77A20D8BF__INCLUDED_)
