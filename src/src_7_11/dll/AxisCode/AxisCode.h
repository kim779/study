// AxisCode.h : main header file for the AXISCODE DLL
//

#if !defined(AFX_AXISCODE_H__5B4609E7_B466_460C_8A5D_C9AADA863031__INCLUDED_)
#define AFX_AXISCODE_H__5B4609E7_B466_460C_8A5D_C9AADA863031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CAxisCodeApp
// See AxisCode.cpp for the implementation of this class
//

class CAxisCodeApp : public CWinApp
{
public:
	CAxisCodeApp();

	CString GetString(char *pChar, int nMinLen);

	bool	findJCode(CString code, CString& name);
	BOOL	loadFCode();
	BOOL	loadCOCode();
	BOOL	loadOJCode();
	BOOL	FindCode(int kind, CString code, char* name);
	BOOL	FindName(int kind, CString name, char* code);
	BOOL	loadJCode();
	BOOL    loadELWCode();
	bool	IsCodeType(CString code, int type);
	BOOL	LoadUPCode();
	BOOL	LoadUSCode();
	void	LoadSFCode();
	BOOL	loadPjCode();
	BOOL	LoadCJCode();
	BOOL	LoadFDCode();
	BOOL	LoadUBCode();
	BOOL	LoadAGCode();
	void		LoadGlobal();

	void	LoadAllCode();
public:
	CString m_RootPath;
	CString m_Myfolder;
	CString m_Name;
	int	m_nType;

	CArray <hjcode*, hjcode*>	m_arrayHJCode;
	CArray <elwcode, elwcode>	m_arrayELWCode;

	CArray<CString, CString>	m_ArrayOMonth;
	CTArray<ojcode, ojcode>		m_arrayOcode;
	CArray<FCODE, FCODE>		m_arrayFcode;
	CArray<COCODE, COCODE>		m_arrayCOcode;
	CArray<UPCODE, UPCODE>		m_arrayUpcode;
	CArray<_USCode*, _USCode*>	m_arrayUSCode;
	CArray<FCODE, FCODE>		m_arraySFcode;
	CArray<PCODE, PCODE>		m_arrayPcode;
	CArray <CJCODE*, CJCODE*>	m_arrayCJCode;
	CArray <AGCODE*, AGCODE*>	m_arrayAGCode;
	CArray<_FDCode*, _FDCode*>	m_arrayFDCode;
	CArray <CUNBCODE*, CUNBCODE*>	m_arrayUBCode;


	CArray<CJCODE*, CJCODE*>	m_arrayGukCode;
	CArray<CJCODE*, CJCODE*>	m_arrayTongCode;
	CArray<CJCODE*, CJCODE*>	m_arrayTukCode;
	CArray<CJCODE*, CJCODE*>	m_arrayJiCode;
	CArray<CJCODE*, CJCODE*>	m_arrayGumCode;
	CArray<CJCODE*, CJCODE*>	m_arrayHeuiCode;
	CArray<CJCODE*, CJCODE*>	m_arrayMyCode;

	CArray<CJCODE*, CJCODE*>	m_arrayDangCode;
	CArray<CJCODE*, CJCODE*>	m_arrayBalCode;
	 
	CArray<CJCODESTR*, CJCODESTR*>	m_arrlist;

	CArray<CJCODESTR*, CJCODESTR*>	m_arrGuklist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrTonglist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrTuklist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrJilist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrGumlist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrHeuilist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrMylist;

	CArray<CJCODESTR*, CJCODESTR*>	m_arrDanglist;
	CArray<CJCODESTR*, CJCODESTR*>	m_arrBallist;

//CJongmukDlg, CElwDlg 사용
	CPtrArray*					m_arrayDlgJCode;
	
	CArray<ELWCODE*, ELWCODE*>	m_arrayDlgELWCode;		// ELW
	CArray<PBCODE, PBCODE>		m_arrayPBCode;		// 발행사
	CArray<BASECODE, BASECODE>	m_arrayBASECode;	// 기초자산
	CStringArray				m_arrayElwMonth;


public:
	virtual ~CAxisCodeApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisCodeApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisCodeApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISCODE_H__5B4609E7_B466_460C_8A5D_C9AADA863031__INCLUDED_)
extern CGlobal Global;