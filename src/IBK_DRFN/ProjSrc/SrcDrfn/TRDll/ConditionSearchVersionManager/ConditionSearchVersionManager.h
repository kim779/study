// ConditionSearchVersionManager.h : main header file for the CONDITIONSEARCHVERSIONMANAGER DLL
//

#if !defined(AFX_CONDITIONSEARCHVERSIONMANAGER_H__44AA6016_D99C_4FD9_997D_92D01CFF2522__INCLUDED_)
#define AFX_CONDITIONSEARCHVERSIONMANAGER_H__44AA6016_D99C_4FD9_997D_92D01CFF2522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

extern "C" __declspec(dllexport) void _DLL_Open(CString strFormCode, CWnd* pTargetWnd, CFont* pFontDef);
extern "C" __declspec(dllexport) void _DLL_Close(CWnd* pTargetWnd);

#include <afxmt.h>
#include "resource.h"		// main symbols

#include "RequestMsg.h"
/////////////////////////////////////////////////////////////////////////////
// CConditionSearchVersionManagerApp
// See ConditionSearchVersionManager.cpp for the implementation of this class
//

class CConditionSearchVersionManagerApp : public CWinApp
{
public:
	CConditionSearchVersionManagerApp();

private:
	CRequestMsg*	m_pCommMsg;

	int		m_nVerMngMode; // 1 : 버전파일로 체크, 0 : 압축파일 유무로 체크

	CString m_stLocalDataFilePath;
	CString m_strLoadFormCode;
	
	BOOL m_bLoadLocalVerDataFile;
	BOOL m_bLoadSvrVerDataFile;

	BOOL m_bUnZipLanguageData;
	BOOL m_bUnZipExp;
	BOOL m_bUnZipMap;
	BOOL m_bUnZipOther;
	BOOL m_bUnZipPatternData;
	BOOL m_bUnZipSys;

	CString m_strLocalLanguageDataVer;
	CString m_strLocalExpVer;
	CString m_strLocalMapVer;
	CString m_strLocalOtherVer;
	CString m_strLocalPatternDataVer;
	CString m_strLocalSysVer;

	CString m_strSvrLanguageDataVer;
	CString m_strSvrExpVer;
	CString m_strSvrMapVer;
	CString m_strSvrOtherVer;
	CString m_strSvrPatternDataVer;
	CString m_strSvrSysVer;

	CPtrList m_listLoadWnd;

	CCriticalSection m_CriticalSection;

private:
	BOOL LoadLocalVersionDataFile();
	BOOL LoadSvrVersionDataFile();
	BOOL CheckDataFileVersion(CWnd* pTargetWnd, CFont* pFontDef);
	BOOL SaveLocalDataFileVersion();
	BOOL UnZipDataFile(CString strFilePath, CString strFileName);
	BOOL CheckVersion(CString strZipFilePath, CString strMoveTargetPath, CString strZipFileName, 
		CStringArray* psaPath, CStringArray* psaFileName,
		CString strSvrVer = "", CString strLocalVer = "");

public:
	void LibOpen(CString strFormCode, CWnd* pTargetWnd, CFont* pFontDef);
	void LibClose(CWnd* pTargetWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionSearchVersionManagerApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CConditionSearchVersionManagerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONSEARCHVERSIONMANAGER_H__44AA6016_D99C_4FD9_997D_92D01CFF2522__INCLUDED_)
